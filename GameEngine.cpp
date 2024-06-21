//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "GameEngine.h"

GameEngine::GameEngine() {
    LOG_INFO("Creating GameEngine object.");
}

GameEngine::~GameEngine() {
    LOG_INFO("Destroying GameEngine object.");
}

void GameEngine::gameLoop() {
    LOG_INFO("Game started.");
    Map::getInstance().displayCurrentMap();
    while (true) {
        if (_kbhit()) {
            char input = static_cast<char>(_getch());
            if (handlePlayerInput(input)) {
                Map::getInstance().displayCurrentMap();
            }
        }
    }
}

bool GameEngine::handlePlayerInput(char input) {
    bool shouldRedrawMap;
    Player* player = Map::getInstance().getCurrentLocation()->getPlayer();
    switch (input) {
    case 'w':
    case 'W':
    case 72: // Up arrow
        shouldRedrawMap = player->movePlayer('w');
        break;
    case 's':
    case 'S':
    case 80: // Down arrow
        shouldRedrawMap = player->movePlayer('s');
        break;
    case 'a':
    case 'A':
    case 75: // Left arrow
        shouldRedrawMap = player->movePlayer('a');
        break;
    case 'd':
    case 'D':
    case 77: // Right arrow
        shouldRedrawMap = player->movePlayer('d');
        break;
    case 'e':
    case 69: // E key
        Inventory::getInstance().printInventory();
        shouldRedrawMap = false;
        break;
    case 27: // Escape key
        saveGame();
        Menu::getInstance().showMenu();
        shouldRedrawMap = false;
        break;
    default:
        shouldRedrawMap = false;
        break;
    }
    return shouldRedrawMap;
}

int GameEngine::runMenu(const std::string& title, const std::vector<std::string>& content, int numOptions) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    int consoleWidth = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
    int consoleHeight = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1 - 4;
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
                if (title == "Load Game:") {
                    return currentIndex;
                } else {
                    handleMenuSelection(title, currentIndex);
                    shouldRedrawMenu = true;
                }
                break;
            case 27: // Escape key
                Menu::getInstance().showMenu();
            default:
                break;
            }
        }
        if (shouldRedrawMenu) {
            buildWindow(consoleWidth, consoleHeight, content, currentIndex, title);
            shouldRedrawMenu = false;
        }
        FlushConsoleInputBuffer(hConsole);
    } while (true);
}

void GameEngine::handleMenuSelection(const std::string& title, int currentIndex) {
    if (title == "Options:") {
        if (currentIndex == 0) {
            Logger::getInstance().showLog();
        } else {
            Menu::getInstance().showMenu();
        }
    } else if (title == "Do you really want to quit?") {
        if (currentIndex == 0) {
            LOG_INFO("Stopping game...");
            getInstance().~GameEngine();
            exit(0);
        }
        Menu::getInstance().showMenu();
    }
}

void GameEngine::buildWindow(int consoleWidth, int consoleHeight, const std::vector<std::string>& content, int currentIndex, const std::string& title) {
    int numEmptyLines = (consoleHeight - content.size() - 4) / 2;
    bool isLoadGame = (title == "Load Game:");

    if (isLoadGame) {
        numEmptyLines = 7;
    }

    drawBorder(consoleWidth);
    drawEmptyLines(consoleWidth, numEmptyLines);

    int padding_left = (consoleWidth - title.length() - 2) / 2;
    int padding_right = consoleWidth - title.length() - 2 - padding_left;
    std::cout << "#" << std::setw(padding_left) << "" << title << std::setw(padding_right + 1) << "#" << std::endl;

    if (isLoadGame) {
        drawEmptyLines(consoleWidth, 2);
    } else {
        drawEmptyLines(consoleWidth, numEmptyLines - 7);
    }

    for (size_t i = 0; i < content.size(); ++i) {
        drawOption(content[i], consoleWidth, i != currentIndex);
        drawEmptyLines(consoleWidth, 1);
    }

    if (isLoadGame) {
        drawEmptyLines(consoleWidth, consoleHeight - (numEmptyLines + 3 + 1 + content.size() * 2) + 3);
    } else {
        drawEmptyLines(consoleWidth, numEmptyLines);
    }
    drawBorder(consoleWidth, true);
}

std::vector<std::string> GameEngine::getSavedMaps(const std::string& saveDirectory) {
    std::vector<std::string> savedMaps;
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(saveDirectory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".bin") {
            savedMaps.push_back(entry.path().stem().string());
        }
    }
    return savedMaps;
}

void GameEngine::loadSelectedMap(const std::string& saveDirectory, const std::string& selectedMap) {
    std::string filePath = saveDirectory + "/" + selectedMap + ".bin";

    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << RED_TEXT << "Failed to open file for reading." << RESET_TEXT << std::endl;
        LOG_ERROR("Failed to open file for reading.");
        Menu::getInstance().showMenu();
        return;
    }

    Map::getInstance().loadMap(file);
    Inventory::getInstance().loadInventory(file);

    if (file.is_open()) {
        file.close();
    }
}

void GameEngine::drawOption(const std::string& option, int width, bool isSelected) {
    const int PADDING_FACTOR = 4;
    std::cout << "#";
    int padding_left = (width - option.length() - PADDING_FACTOR) / 2;
    int padding_right = width - option.length() - PADDING_FACTOR - padding_left;

    if (!isSelected) {
        std::cout << GREEN_TEXT;
        std::cout << std::setw(padding_left) << std::left << "" << option << " <" << std::setw(padding_right - 2) << std::right << "";
    } else {
        std::cout << std::setw(padding_left) << std::left << "" << option << std::setw(padding_right) << std::right << "";
    }

    std::cout << RESET_TEXT << "  #" << std::endl;
}

void GameEngine::drawBorder(int width, bool isEndOfWindow) {
    std::cout << std::string(width, '#');
    if (!isEndOfWindow) {
        std::cout << std::endl;
    }
}

void GameEngine::drawEmptyLines(int width, int count) {
    for (int i = 0; i < count; ++i) {
        std::cout << "#" << std::setw(width - 1) << "#" << std::endl;
    }
}

void GameEngine::deathScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    int consoleHeight = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top - 4;
    const int numOptions = 2;

    int consoleWidth = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
    int numEmptyLines = (consoleHeight - numOptions - 6) / 2;

    drawBorder(consoleWidth);
    drawEmptyLines(consoleWidth, numEmptyLines - 2);
    drawDeathTitle(consoleWidth);
    drawEmptyLines(consoleWidth, numEmptyLines - 2);

    drawOption("Back to menu", consoleWidth, true);
    drawEmptyLines(consoleWidth, 0);

    drawEmptyLines(consoleWidth, numEmptyLines);
    drawBorder(consoleWidth, true);

    while (_getch() != 13) {} // Enter key
    Menu::getInstance().showMenu();
}

GameEngine &GameEngine::getInstance() {
    static GameEngine instance;
    return instance;
}

void GameEngine::newGame() {
    Map::getInstance().setMapName();
    Map::getInstance().setMapDifficulty();
    LOG_INFO("Created map with name: " + Map::getInstance().getMapName());
    Map::getInstance().createNewLocation(true);
    Inventory::getInstance();
    saveGame();
    gameLoop();
}

void GameEngine::saveGame() {
    LOG_INFO("Game " + Map::getInstance().getMapName() + " saving.");
    std::cout << "Game is saving." << std::endl;

    const std::string saveDirectory = "saves";
    std::filesystem::create_directory(saveDirectory);

    std::string filePath = saveDirectory + "/" + Map::getInstance().getMapName() + ".bin";
    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << RED_TEXT << "Failed to open file for writing." << RESET_TEXT << std::endl;
        LOG_ERROR("Failed to open file for writing.");
        Menu::getInstance().showMenu();
        return;
    }

    Map::getInstance().saveMap(file);
    Inventory::getInstance().saveInventory(file);

    if (file.is_open()) {
        file.close();
    }

    std::cout << "Game saved successfully." << std::endl;
    LOG_INFO("Game " + Map::getInstance().getMapName() + " saved.");
}

void GameEngine::loadGame() {
    std::cout << "Game loading." << std::endl;

    const std::string saveDirectory = "saves";

    // Check if the directory exists, if not create it
    if (!std::filesystem::exists(saveDirectory)) {
        if (!std::filesystem::create_directory(saveDirectory)) {
            std::cerr << RED_TEXT << "Failed to create save directory." << RESET_TEXT << std::endl;
            LOG_ERROR("Failed to create save directory.");
        } else {
            std::cout << YELLOW_TEXT << "Save directory created. No saved maps found." << RESET_TEXT << std::endl;
            LOG_WARNING("Save directory created. No saved maps found.");
        }
    }

    std::vector<std::string> savedMaps = getSavedMaps(saveDirectory); // Example content
    const std::string loadGameTitle = "Load Game:";
    const int numSavedMaps = savedMaps.size();

    if (savedMaps.empty()) {
        std::cout << YELLOW_TEXT << "No saved maps found." << RESET_TEXT << std::endl;
        LOG_WARNING("No saved maps found.");
    }

    int selectedMapIndex = runMenu(loadGameTitle, savedMaps, numSavedMaps);
    if (selectedMapIndex < 0) {
        Menu::getInstance().showMenu();
        return;
    }

    std::string selectedMap = savedMaps[selectedMapIndex];
    loadSelectedMap(saveDirectory, selectedMap);

    std::cout << "Game loaded successfully." << std::endl;
    LOG_INFO("Game loaded successfully.");

    gameLoop();
}

void GameEngine::options() {
    std::vector<std::string> optionsContent = {"Open log", "Back"};
    const std::string optionsTitle = "Options:";
    const int numOptions = optionsContent.size();

    runMenu(optionsTitle, optionsContent, numOptions);
}

void GameEngine::quit() {
    std::vector<std::string> quitOptions = {"Yes", "No"};
    const std::string quitTitle = "Do you really want to quit?";
    const int numQuitOptions = quitOptions.size();

    runMenu(quitTitle, quitOptions, numQuitOptions);
}

void GameEngine::drawDeathTitle(int consoleWidth) {
    const int LEFT_PADDING = 25;
    const int RIGHT_PADDING = 96;
    std::string titleLines[7] = {
        "##    ##  #######  ##     ##    ########  #### ######## ######## ",
        " ##  ##  ##     ## ##     ##    ##     ##  ##  ##       ##     ##",
        "  ####   ##     ## ##     ##    ##     ##  ##  ##       ##     ##",
        "   ##    ##     ## ##     ##    ##     ##  ##  ######   ##     ##",
        "   ##    ##     ## ##     ##    ##     ##  ##  ##       ##     ##",
        "   ##    ##     ## ##     ##    ##     ##  ##  ##       ##     ##",
        "   ##     #######   #######     ########  #### ######## ######## ",
    };
    for (const std::string& line : titleLines) {
        std::cout << "#" << GREEN_TEXT << std::setw(consoleWidth - LEFT_PADDING) << line << RESET_TEXT << std::setw(consoleWidth - RIGHT_PADDING) << "#" << std::endl;
    }
}