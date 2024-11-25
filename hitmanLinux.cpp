#include <gtk/gtk.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#define CONFIG_FILE_PATH "/.config/process_hitman"

bool hookEnabled = false;
GtkWidget *button, *checkbox1, *checkbox2, *checkbox3;
Display *display;

pid_t GetActiveWindowPID() {
    Atom prop = XInternAtom(display, "_NET_WM_PID", True);
    Window window;
    int revert;
    XGetInputFocus(display, &window, &revert);

    if (prop == None || window == None) {
        return 0;
    }

    Atom actualType;
    int actualFormat;
    unsigned long nItems, bytesAfter;
    unsigned char *propPID = nullptr;

    if (XGetWindowProperty(display, window, prop, 0, 1, False, XA_CARDINAL,
                           &actualType, &actualFormat, &nItems, &bytesAfter,
                           &propPID) == Success && propPID != nullptr) {
        pid_t pid = *(pid_t *)propPID;
        XFree(propPID);
        return pid;
    }
    return 0;
}

void KillProcess(pid_t pid) {
    if (pid != 0) {
        if (kill(pid, SIGKILL) == -1) {
            std::cerr << "Failed to kill process!" << std::endl;
        }
    }
}

void ToggleHook(GtkButton *btn) {
    hookEnabled = !hookEnabled;
    gtk_button_set_label(btn, hookEnabled ? "Disable Alt+F5" : "Enable Alt+F5");
}

gboolean OnKeyPress() {
    if (!hookEnabled)
        return FALSE;

    pid_t pid = GetActiveWindowPID();
    if (pid != 0) {
        std::cout << "Found active window PID: " << pid << std::endl;
        KillProcess(pid);
    } else {
        std::cout << "No active window found" << std::endl;
    }
    return FALSE;
}

void ShutdownFunction() {
    const char *home = getenv("HOME");
    if (!home) 
        return;

    std::string configDir = std::string(home) + CONFIG_FILE_PATH;
    
    struct stat info;
    if (stat(configDir.c_str(), &info) != 0) {
        mkdir(configDir.c_str(), 0755);
    }

    std::ofstream configFile(configDir + "/config");
    if (configFile.is_open()) {
        configFile << (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbox1)) ? "1" : "0") << std::endl;
        configFile << (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbox2)) ? "1" : "0") << std::endl;
        configFile << (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbox3)) ? "1" : "0") << std::endl;
        configFile.close();
    }

    gtk_main_quit();
}

void LoadCheckboxStates() {
    const char *home = getenv("HOME");
    if (!home) return;

    std::ifstream configFile(std::string(home) + CONFIG_FILE_PATH + "/config");
    if (configFile.is_open()) {
        std::string line;
        if (std::getline(configFile, line)) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbox1), line == "1");
        }
        if (std::getline(configFile, line)) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbox2), line == "1");
        }
        if (std::getline(configFile, line)) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbox3), line == "1");
        }
        configFile.close();
    }
}

void CreateUI(GtkWidget *window) {
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *label = gtk_label_new("Process Hitman by zyapguy");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    checkbox1 = gtk_check_button_new_with_label("Open on startup");
    gtk_box_pack_start(GTK_BOX(vbox), checkbox1, FALSE, FALSE, 0);

    checkbox2 = gtk_check_button_new_with_label("Start hidden");
    gtk_box_pack_start(GTK_BOX(vbox), checkbox2, FALSE, FALSE, 0);

    checkbox3 = gtk_check_button_new_with_label("Enable on start");
    gtk_box_pack_start(GTK_BOX(vbox), checkbox3, FALSE, FALSE, 0);

    button = gtk_button_new_with_label("Enable Alt+F5");
    g_signal_connect(button, "clicked", G_CALLBACK(ToggleHook), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    LoadCheckboxStates();
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Failed to open X display!" << std::endl;
        return 1;
    }

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Process Hitman");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    g_signal_connect(window, "destroy", G_CALLBACK(ShutdownFunction), NULL);
    
    CreateUI(window);
    gtk_widget_show_all(window);

    KeyCode key_f5 = XKeysymToKeycode(display, XStringToKeysym("F5"));
    XGrabKey(display, key_f5, Mod1Mask, DefaultRootWindow(display), True, GrabModeAsync, GrabModeAsync);

    while (true) {
        while (XPending(display)) {
            XEvent event;
            XNextEvent(display, &event);

            if (event.type == KeyPress) {
                if (event.xkey.keycode == key_f5 && (event.xkey.state & Mod1Mask)) {
                    OnKeyPress();
                }
            }
        }

        g_main_context_iteration(NULL, FALSE);
    }

    XCloseDisplay(display);
    return 0;
}
