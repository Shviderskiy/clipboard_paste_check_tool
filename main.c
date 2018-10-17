#define _WIN32_WINNT 0x0500


#include <windows.h>


#ifndef MOD_NOREPEAT
#define MOD_NOREPEAT 0x4000
#endif // MOD_NOREPEAT


#ifndef KEYEVENTF_KEYDOWN
#define KEYEVENTF_KEYDOWN 0
#endif // KEYEVENTF_KEYDOWN


#define VK_V 0x56


enum
{
    ID_CTRL_WIN,
    ID_CTRL_V,
};


static WINBOOL ClipboardPaste(VOID)
{
    INPUT input[] =
    {
        {
            .type = INPUT_KEYBOARD,
            .ki   =
            {
                .wVk         = VK_CONTROL,
                .wScan       = 0,
                .dwFlags     = KEYEVENTF_KEYDOWN,
                .time        = 0,
                .dwExtraInfo = 0,
            },
        },
        {
            .type = INPUT_KEYBOARD,
            .ki   =
            {
                .wVk         = VK_V,
                .wScan       = 0,
                .dwFlags     = KEYEVENTF_KEYDOWN,
                .time        = 0,
                .dwExtraInfo = 0,
            },
        },
        {
            .type = INPUT_KEYBOARD,
            .ki   =
            {
                .wVk         = VK_V,
                .wScan       = 0,
                .dwFlags     = KEYEVENTF_KEYUP,
                .time        = 0,
                .dwExtraInfo = 0,
            },
        },
        {
            .type = INPUT_KEYBOARD,
            .ki   =
            {
                .wVk         = VK_CONTROL,
                .wScan       = 0,
                .dwFlags     = KEYEVENTF_KEYUP,
                .time        = 0,
                .dwExtraInfo = 0,
            },
        },
    };

    return SendInput(4, input, sizeof(INPUT)) == 4 ? TRUE : FALSE;
}


static VOID EntryPoint(VOID)
{
    WINBOOL winbool;

    winbool = RegisterHotKey(NULL,
                             ID_CTRL_WIN,
                             MOD_CONTROL | MOD_WIN | MOD_NOREPEAT,
                             0);
    if (winbool == FALSE)
    {
        MessageBoxW(NULL,
                    L"Error while register hotkey Ctrl+Win",
                    L"Error",
                    MB_OK | MB_ICONERROR);
        ExitProcess(EXIT_FAILURE);
    }

    winbool = RegisterHotKey(NULL,
                             ID_CTRL_V,
                             MOD_CONTROL | MOD_NOREPEAT,
                             VK_V);
    if (winbool == FALSE)
    {
        MessageBoxW(NULL,
                    L"Error while register hotkey Ctrl+C",
                    L"Error",
                    MB_OK | MB_ICONERROR);
        ExitProcess(EXIT_FAILURE);
    }

    int button;

    button = MessageBoxW(NULL,
                         L"Some help!\nRun this shit?",
                         L"Clipboard paste check tool",
                         MB_OKCANCEL | MB_ICONINFORMATION);
    if (button != IDOK)
    {
        ExitProcess(EXIT_SUCCESS);
    }

    MSG message;
    while (GetMessageA(&message, NULL, 0, 0) != FALSE)
    {
        if (message.message == WM_HOTKEY)
        {
            int id= (int)message.wParam;
            if (id == ID_CTRL_WIN)
            {
                button = MessageBoxW(NULL,
                                     L"Stop this shit?",
                                     L"Stop this shit?",
                                     MB_OKCANCEL | MB_ICONQUESTION);
                if (button == 0)
                {
                    ExitProcess(EXIT_FAILURE);
                }
                if (button == IDOK)
                {
                    ExitProcess(EXIT_SUCCESS);
                }
            }
            else if (id == ID_CTRL_V)
            {
                winbool = OpenClipboard(NULL);
                if (winbool == FALSE)
                {
                    MessageBoxW(NULL,
                                L"'OpenClipboard' failed",
                                L"Error",
                                MB_OK | MB_ICONERROR);
                    ExitProcess(EXIT_FAILURE);
                }

                HANDLE clipboardData = GetClipboardData(CF_UNICODETEXT);
                if (clipboardData != NULL)
                {
                    LPCWSTR text = (LPCWSTR)GlobalLock(clipboardData);
                    if (text != NULL)
                    {
                        button = MessageBoxW(NULL,
                                             text,
                                             L"Text on clipboard:",
                                             MB_OKCANCEL | MB_ICONWARNING);

                        winbool = GlobalUnlock(clipboardData);
                        if (winbool == FALSE)
                        {
                            MessageBoxW(NULL,
                                        L"'GlobalUnlock' failed",
                                        L"Error",
                                        MB_OK | MB_ICONERROR);
                            CloseClipboard();
                            ExitProcess(EXIT_FAILURE);
                        }

                        if (button != IDOK)
                        {
                            winbool = CloseClipboard();
                            if (winbool == FALSE)
                            {
                                MessageBoxW(NULL,
                                            L"'CloseClipboard' failed",
                                            L"Error",
                                            MB_OK | MB_ICONERROR);
                                ExitProcess(EXIT_FAILURE);
                            }
                        }

                        if (button == 0)
                        {
                            MessageBoxW(NULL,
                                        L"'MessageBoxW' failed",
                                        L"Error",
                                        MB_OK | MB_ICONERROR);
                            CloseClipboard();
                            ExitProcess(EXIT_FAILURE);
                        }

                        if (button != IDOK)
                        {
                            continue;
                        }
                    }
                }

                winbool = CloseClipboard();
                if (winbool == FALSE)
                {
                    MessageBoxW(NULL,
                                L"'CloseClipboard' failed",
                                L"Error",
                                MB_OK | MB_ICONERROR);
                    ExitProcess(EXIT_FAILURE);
                }

                winbool = UnregisterHotKey(NULL, ID_CTRL_V);
                if (winbool == FALSE)
                {
                    MessageBoxW(NULL,
                                L"'UnregisterHotKey' failed",
                                L"Error",
                                MB_OK | MB_ICONERROR);
                    CloseClipboard();
                    ExitProcess(EXIT_FAILURE);
                }

                winbool = ClipboardPaste();
                if (winbool == FALSE)
                {
                    MessageBoxW(NULL,
                                L"'ClipboardPaste' failed",
                                L"Error",
                                MB_OK | MB_ICONERROR);
                    CloseClipboard();
                    ExitProcess(EXIT_FAILURE);
                }

                winbool = RegisterHotKey(NULL,
                                         ID_CTRL_V,
                                         MOD_CONTROL | MOD_NOREPEAT,
                                         VK_V);
                if (winbool == FALSE)
                {
                    MessageBoxW(NULL,
                                L"Error while register hotkey Ctrl+C",
                                L"Error",
                                MB_OK | MB_ICONERROR);
                    CloseClipboard();
                    ExitProcess(EXIT_FAILURE);
                }
            }
        }
    }
}


int CALLBACK WinMain(HINSTANCE instance,
                     HINSTANCE prevInstance,
                     LPSTR commandLine,
                     int showCommand)
{
    (VOID)instance;
    (VOID)prevInstance;
    (VOID)commandLine;
    (VOID)showCommand;

    EntryPoint();
    return EXIT_SUCCESS;
}
