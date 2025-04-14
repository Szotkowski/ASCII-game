//
// Created by Michael Szotkowski on 6/17/2024.
//

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

#ifdef _WIN32
#include <windows.h>
#endif
#include "Menu.h"
#include "logger/Logger.h"

int main() {
    LOG_INFO("Starting game...");
    SetConsoleTitle("ElderGlyph");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        std::cerr << RED_TEXT << "Error: Unable to get hConsole handle\n" << RESET_TEXT;
        LOG_ERROR("Error: Unable to get hConsole handle");
        return 1;
    }
    DWORD dwMode = 0;
    GetConsoleMode(hConsole, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, dwMode);

    // Set hConsole window size
    SMALL_RECT rect = {0, 0, 119, 29}; // 80 columns, 25 rows
    if (!SetConsoleWindowInfo(hConsole, TRUE, &rect)) {
        std::cerr << RED_TEXT << "Error: Unable to set hConsole window size\n" << RESET_TEXT;
        LOG_ERROR("Error: Unable to set hConsole window size");
        return 1;
    }

    // Lock the hConsole window size
    HWND consoleWindow = GetConsoleWindow();
    if (consoleWindow == nullptr) {
        std::cerr << RED_TEXT << "Error: Unable to get hConsole window handle\n" << RESET_TEXT;
        LOG_ERROR("Error: Unable to get hConsole window handle");
        return 1;
    }
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_THICKFRAME);
    SetWindowPos(consoleWindow, nullptr, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

    // Disable mouse input
    DWORD prevMode;
    if (!GetConsoleMode(hConsole, &prevMode)) {
        std::cerr << RED_TEXT << "Error: Unable to get hConsole mode\n" << RESET_TEXT;
        LOG_ERROR("Error: Unable to get hConsole mode");
        return 1;
    }
    DWORD newMode = prevMode & ~ENABLE_MOUSE_INPUT;
    if (!SetConsoleMode(hConsole, newMode)) {
        std::cerr << RED_TEXT << "Error: Unable to set hConsole mode\n" << RESET_TEXT;
        LOG_ERROR("Error: Unable to set hConsole mode");
        return 1;
    }

    Menu::getInstance().showMenu();
    return 0;
}
