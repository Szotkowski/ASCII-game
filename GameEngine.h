//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_GAMEENGINE_H
#define NEW_PROJEKT_ZOO_GAMEENGINE_H

#include <vector>
#include "map/Map.h"
#include "Menu.h"
#include "inventory/Inventory.h"

class GameEngine {
private:
    GameEngine();

    ~GameEngine();

    static void gameLoop();

    static bool handlePlayerInput(char input);

    static int runMenu(const std::string& title, const std::vector<std::string>& content, int numOptions);

    static void handleMenuSelection(const std::string& title, int currentIndex);

    static void buildWindow(int consoleWidth, int consoleHeight, const std::vector<std::string>& options, int currentIndex, const std::string& title);

    static std::vector<std::string> getSavedMaps(const std::string& saveDirectory);

    static void loadSelectedMap(const std::string& saveDirectory, const std::string& selectedMap);

    static void drawOption(const std::string& option, int width, bool isSelected);

    static void drawBorder(int width, bool isEndOfWindow = false);

    static void drawEmptyLines(int width, int count);

    static void drawDeathTitle(int consoleWidth);

public:
    static GameEngine &getInstance();

    static void newGame();

    static void saveGame();

    static void loadGame();

    static void options();

    static void quit();

    static void deathScreen();

    GameEngine(const GameEngine &) = delete;

    GameEngine &operator=(const GameEngine &) = delete;

    GameEngine(GameEngine &&) = delete;

    GameEngine &operator=(GameEngine &&) = delete;
};

#endif //NEW_PROJEKT_ZOO_GAMEENGINE_H
