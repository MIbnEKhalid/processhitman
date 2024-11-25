# Process Hitman
Use Windows Key + Alt + F5 to kill any process, more efficiently than Alt+F4

*Have you ever been in a situation where an application stops responding, gets stuck in a loading screen, or is just annoying. You immediately press Alt+F4, but the application doesn't shut down?*

**Process Hitman will ensure the process is killed.**

### Note: If the pre-built application from the release does not work, you can download the repository and run `compile.bat`. Ensure you have `g++` installed, and it should work seamlessly.

**Credit**: **[github.com/zyapguy/processhitman](https://github.com/zyapguy/processhitman/tree/main)**

## What's New in This Forked Repo?
The functionality remains the same, except that the **original source code uses** **`Alt + F5`** to kill a process, while in this version, **I’ve modified it to** **`Windows Key + Alt + F5`**. This change was made because I have a few applications that use Alt + F5 for other purposes. And There Is Little Bit Change in UI.
**Note:** All changes are limited to the `hitman.cpp` script in the `Windows Version` of the repository. 
Below is the code comparison:
### Original Code:
```cpp 
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && hookEnabled)
    {
        if (GetAsyncKeyState(VK_F5) & 0x8000)
        {
            if (GetAsyncKeyState(VK_MENU) & 0x8000)
            {
                DWORD processID = GetProcessIdOfActiveWindow();
                if (processID != 0)
                {
                    KillProcess(processID); 
                }
            }
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
```

### Modified Code (My Version):
```cpp
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && hookEnabled)
    {
        if ((GetAsyncKeyState(VK_F5) & 0x8000) &&                                           // F5
            (GetAsyncKeyState(VK_MENU) & 0x8000) &&                                         // Alt
            ((GetAsyncKeyState(VK_LWIN) & 0x8000) || (GetAsyncKeyState(VK_RWIN) & 0x8000))) // Windows key 
        {
            DWORD processID = GetProcessIdOfActiveWindow();
            if (processID != 0)
            {
                KillProcess(processID);
            }
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
```


## Purpose
Some programs and games are annoying to close once opened. Some games I play don't work with Alt+F4, and I am too lazy to go back through tons of menus to exit a game. There are also some apps that have long splash screens, so if you accidentally open one of them, you have to either wait to close it, or use Task Manager.

I built this because I needed a program like this, nothing groundbreaking, but I hope it helps someone out.

## How it works
It's very simple, if you press Windows Key + Alt + F5, it will kill the current process. This will not show any "Save unsaved changes" menu, it will not wait for the application to save and exit. It will kill the process, hence the name, Process Hitman.

Built using C++ and the Windows API.

## Contributing
Contributors are welcome to fork the repository or send pull requests directly to this repository. I don't intend on releasing any updates to this software (except for bug fixes for major security issues), as its purpose is quite simple.  

## Building
You should ideally use the compile.bat script included in the repository. You need the G++ compiler, but you can probably use other compilers as well. I used the MinGW G++ compiler.

```shell
compile.bat
```

## License

MIT License

Copyright (c) 2024 zyapguy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


## Attribution

Unedited App Icon by freepik: [Link](https://www.freepik.com/free-vector/mysterious-gangster-character_7075529.htm#fromView=keyword&page=1&position=6&uuid=77c07a48-2a0d-497c-be29-154325ce7e35)
