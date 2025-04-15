//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Menu.h"

Menu::Menu() {
    LOG_INFO("Creating Menu instance.");
}

Menu::~Menu() {
    LOG_INFO("Destroying Menu instance.");
}

const std::string Menu::options[] = {"New game", "Load game", "Options", "Help", "Quit"};

const int Menu::numOptions = std::size(options);

void Menu::drawOption(std::string option, int width, bool isSelected) {
    const int PADDING_FACTOR = 4;
    std::cout << "#";

    int padding_left = (width - option.length() - PADDING_FACTOR) / 2;
    int padding_right = width - option.length() - PADDING_FACTOR - padding_left;

    if (!isSelected) {
        std::cout << GREEN_TEXT << std::setw(padding_left) << std::left << "" << option << " <" << std::setw(padding_right - 2) << std::right << "";
    } else {
        std::cout << std::setw(padding_left) << std::left << "" << option << std::setw(padding_right) << std::right << "";
    }
    std::cout << RESET_TEXT << "  #" << std::endl;
}

void Menu::drawBorder(int width, bool isEndOfWindow) {
    std::cout << std::string(width, '#');
    if (!isEndOfWindow) {
        std::cout << std::endl;
    }
}

void Menu::drawEmptyLines(int width, int count) {
    for (int i = 0; i < count; ++i) {
        std::cout << "#" << std::setw(width - 1) << "#" << std::endl;
    }
}

void Menu::drawTitle(int consoleWidth) {
    const int LEFT_PADDING = 14;
    const int RIGHT_PADDING = 107;
    std::string titleLines[7] = {
            "######## ##       ########  ######## ########     ######   ##       ##    ## ########  ##     ##",
            "##       ##       ##     ## ##       ##     ##   ##    ##  ##        ##  ##  ##     ## ##     ##",
            "##       ##       ##     ## ##       ##     ##   ##        ##         ####   ##     ## ##     ##",
            "######   ##       ##     ## ######   ########    ##   #### ##          ##    ########  #########",
            "##       ##       ##     ## ##       ##   ##     ##    ##  ##          ##    ##        ##     ##",
            "##       ##       ##     ## ##       ##    ##    ##    ##  ##          ##    ##        ##     ##",
            "######## ######## ########  ######## ##     ##    ######   ########    ##    ##        ##     ##"
    };
    for (const std::string& line : titleLines) {
        std::cout << "#" << GREEN_TEXT << std::setw(consoleWidth - LEFT_PADDING) << line << RESET_TEXT << std::setw(consoleWidth - RIGHT_PADDING) << "#" << std::endl;
    }
}

void Menu::buildMenu(int consoleWidth, int consoleHeight, int currentIndex) {
    int numEmptyLines = (consoleHeight - numOptions - 6) / 2;

    drawBorder(consoleWidth);
    drawEmptyLines(consoleWidth, numEmptyLines - 7);
    drawTitle(consoleWidth);
    drawEmptyLines(consoleWidth, numEmptyLines - 6);

    for (int i = 0; i < numOptions; ++i) {
        drawOption(options[i], consoleWidth, i != currentIndex);
        drawEmptyLines(consoleWidth, 1);
    }

    drawEmptyLines(consoleWidth, numEmptyLines - 3);
    drawBorder(consoleWidth, true);
}

void Menu::handleMenuOption(int currentIndex) {
    std::string selectedOption = options[currentIndex];
    if (selectedOption == "New game") {
        LOG_INFO("Creating new game.");
        GameEngine::getInstance().newGame();
    } else if (selectedOption == "Load game") {
        LOG_INFO("Loading game.");
        GameEngine::getInstance().loadGame();
    } else if (selectedOption == "Options") {
        LOG_INFO("Opening options.");
        GameEngine::getInstance().options();
    } else if (selectedOption == "Quit") {
        LOG_INFO("Stopping game...");
        GameEngine::getInstance().quit();
        ExitProcess(0);
    }
}

Menu &Menu::getInstance() {
    static Menu instance;
    return instance;
}

void Menu::showMenu() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    int consoleHeight = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1 - 5;
    int currentIndex = 0;
    bool shouldRedrawMenu = true;

    do {
        if (_kbhit()) {
            char input = static_cast<char>(_getch());
            switch (input) {
                case 'w': case 72: // Up arrow
                    currentIndex = (currentIndex == 0) ? numOptions - 1 : currentIndex - 1;
                    shouldRedrawMenu = true;
                    break;
                case 's': case 80: // Down arrow
                    currentIndex = (currentIndex == numOptions - 1) ? 0 : currentIndex + 1;
                    shouldRedrawMenu = true;
                    break;
                case 13: // Enter key
                    handleMenuOption(currentIndex);
                    shouldRedrawMenu = false;
                    break;
                default:
                    break;
            }
        }
        if (shouldRedrawMenu) {
            buildMenu(consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1, consoleHeight + 5, currentIndex);
            shouldRedrawMenu = false;
        }
        FlushConsoleInputBuffer(hConsole);
    } while (true);
}