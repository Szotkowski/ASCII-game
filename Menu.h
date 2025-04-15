//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_MENU_H
#define NEW_PROJEKT_ZOO_MENU_H

#include <conio.h>
#include "logger/Logger.h"
#include "GameEngine.h"

class Menu {
private:
    Menu();

    ~Menu();

    static const std::string options[5];

    static const int numOptions;

    static void drawOption(std::string option, int width, bool isSelected);

    static void drawBorder(int width, bool isEndOfWindow = false);

    static void drawEmptyLines(int width, int count);

    static void drawTitle(int consoleWidth);

    static void buildMenu(int consoleWidth, int consoleHeight, int currentIndex);

    static void handleMenuOption(int curentIndex);

public:
    static Menu &getInstance();

    static void showMenu();

    Menu(const Menu &) = delete;

    Menu &operator=(const Menu &) = delete;

    Menu(Menu &&) = delete;

    Menu &operator=(Menu &&) = delete;
};

#endif //NEW_PROJEKT_ZOO_MENU_H
