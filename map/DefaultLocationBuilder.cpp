//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "DefaultLocationBuilder.h"

DefaultLocationBuilder::DefaultLocationBuilder() {
    LOG_INFO("Creating DefaultLocationBuilder instance.");
    m_location = new Location("Default Location");
}

DefaultLocationBuilder::~DefaultLocationBuilder() {
    LOG_INFO("Destroying DefaultLocationBuilder instance.");
    delete m_location;
}

void DefaultLocationBuilder::emptyLayout() {
    LOG_INFO("Creating tiles.");
    std::vector<std::vector<Tile*>> tiles;
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Generate random width and height between 5 and 10
    constexpr int MIN_SIZE = 7;
    constexpr int MAX_SIZE = 12;
    int height = std::rand() % (MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE;
    int width = std::rand() % (height - 4) + height;

    // Initialize the location with spaces
    for (int y = 0; y < height; ++y) {
        std::vector<Tile*> row;
        for (int x = 0; x < width; ++x) {
            if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                // Borders
                row.push_back(new Tile('#', false,nullptr));
            } else {
                // Inner tiles
                row.push_back(new Tile(' ', true,nullptr));
            }
        }
        tiles.push_back(row);
    }
    m_location->setHeight(height);
    m_location->setWidth(width);
    m_location->setTile(tiles);
}

void DefaultLocationBuilder::setDoors(Side doorSide) {
    int width = m_location->getTiles().size();
    int height = m_location->getTiles()[0].size();
    std::vector<std::vector<Tile*>> tiles = m_location->getTiles();
    std::set<Side> doorSides;

    // Place the door on the specified side
    switch (doorSide) {
        case Side::Left: {
            int x = rand() % (width - 2) + 1; // Avoid corners
            tiles[x][0] = new Tile(' ', true);
            doorSides.insert(Side::Left);
            break;
        }
        case Side::Right: {
            int x = rand() % (width - 2) + 1; // Avoid corners
            tiles[x][height - 1] = new Tile(' ', true);
            doorSides.insert(Side::Right);
            break;
        }
        case Side::Top: {
            int y = rand() % (height - 2) + 1; // Avoid corners
            tiles[0][y] = new Tile(' ', true);
            doorSides.insert(Side::Top);
            break;
        }
        case Side::Bottom: {
            int y = rand() % (height - 2) + 1; // Avoid corners
            tiles[width - 1][y] = new Tile(' ', true);
            doorSides.insert(Side::Bottom);
            break;
        }
    }

    // Place an additional door on a different random side if needed
    if (doorSides.size() <= 2) {
        Side additionalSide;
        do {
            additionalSide = static_cast<Side>(rand() % 4);
        } while (doorSides.count(additionalSide) > 0);

        switch (additionalSide) {
            case Side::Top: {
                int y = rand() % (height - 2) + 1; // Avoid corners
                tiles[0][y] = new Tile(' ', true);
                break;
            }
            case Side::Bottom: {
                int y = rand() % (height - 2) + 1; // Avoid corners
                tiles[width - 1][y] = new Tile(' ', true);
                break;
            }
            case Side::Left: {
                int x = rand() % (width - 2) + 1; // Avoid corners
                tiles[x][0] = new Tile(' ', true);
                break;
            }
            case Side::Right: {
                int x = rand() % (width - 2) + 1; // Avoid corners
                tiles[x][height - 1] = new Tile(' ', true);
                break;
            }
        }
    }
    m_location->setTile(tiles);
}

void DefaultLocationBuilder::setMonsters(int monsterPercentage) {
    int width = m_location->getTiles()[0].size();
    int height = m_location->getTiles().size();
    int totalTiles = (width - 2) * (height - 2);
    int numMonsters = totalTiles * monsterPercentage / 100;

    // Create a list of available positions excluding borders
    std::vector<std::pair<int, int>> availablePositions;
    for (int y = 1; y < width - 1; ++y) {
        for (int x = 1; x < height - 1; ++x) {
            if (m_location->getTiles()[x][y]->isPassable()) {
                availablePositions.push_back({x, y});
            }
        }
    }

    // Shuffle the available positions
    std::shuffle(availablePositions.begin(), availablePositions.end(), std::mt19937(std::random_device()()));

    for (int i = 0; i < numMonsters && !availablePositions.empty(); ++i) {
        MonsterType monsterType = static_cast<MonsterType>(rand() % 3 + 1); // Random item type between 1 and 3
        std::string monsterName = "name" + std::to_string(rand() % 100 + 1); // Random item name
        int attack = rand() % 10 + 1; // Random attack between 1 and 10
        int defense = rand() % 10 + 1; // Random defense between 1 and 10
        int hp = rand() % 11 + 5; // Random HP between 5 and 15

        MonsterLevel monsterLevel = M_EASY;
        int randomValue = rand() % 100 + 1; // Random number between 1 and 100
        if (randomValue <= 50) {
            monsterLevel = M_EASY; // 50% chance
        } else if (randomValue <= 80) {
            monsterLevel = M_MEDIUM; // 30% chance
        } else {
            monsterLevel = M_HARD; // 5% chance
        }

        MonsterFactory monsterFactory;
        Monster* monster = monsterFactory.createMonster(monsterType, monsterName, attack, defense, hp, monsterLevel);

        auto pos = availablePositions.back();
        availablePositions.pop_back();
        m_location->setTile(pos.first, pos.second, new Tile(monster->getMonsterSymbol(), false, monster));
    }
}

void DefaultLocationBuilder::setItems(int itemPercentage) {
    int width = m_location->getTiles()[0].size();
    int height = m_location->getTiles().size();
    int totalTiles = (width - 2) * (height - 2);
    int numItems = totalTiles * itemPercentage / 100;

    // Create a list of available positions excluding borders
    std::vector<std::pair<int, int>> availablePositions;
    for (int y = 1; y < width - 1; ++y) {
        for (int x = 1; x < height - 1; ++x) {
            if (m_location->getTiles()[x][y]->isPassable()) {
                    availablePositions.push_back({x, y});
            }
        }
    }

    // Shuffle the available positions
    std::shuffle(availablePositions.begin(), availablePositions.end(), std::mt19937(std::random_device()()));

    for (int i = 0; i < numItems && !availablePositions.empty(); ++i) {
        ItemTypes itemType = static_cast<ItemTypes>(rand() % 3 + 1); // Random item type between 1 and 3
        std::string itemName = "name" + std::to_string(rand() % 100 + 1); // Random item name
        int itemPower = rand() % 10 + 1; // Random item power between 1 and 10

        ItemRarity itemRarity = COMMON;
        int randomValue = rand() % 100 + 1; // Random number between 1 and 100
        if (randomValue <= 50) {
            itemRarity = COMMON; // 50% chance
        } else if (randomValue <= 80) {
            itemRarity = UNCOMMON; // 30% chance
        } else if (randomValue <= 95) {
            itemRarity = RARE; // 15% chance
        } else {
            itemRarity = LEGENDARY; // 5% chance
        }

        int itemWeight = rand() % 5 + 1; // Random weight between 1 and 5

        ItemFactory itemFactory;
        Item* item = itemFactory.createItem(itemType, itemName, itemPower, itemRarity, itemWeight);

        auto pos = availablePositions.back();
        availablePositions.pop_back();
        m_location->setTile(pos.first, pos.second, new Tile(item->getItemSymbol(), false, item));
    }
}

void DefaultLocationBuilder::setGold(int goldPercentage) {
    int width = m_location->getTiles()[0].size();
    int height = m_location->getTiles().size();
    int totalTiles = (width - 2) * (height - 2);
    int numGoldPiles = totalTiles * goldPercentage / 100;

    // Create a list of available positions excluding borders
    std::vector<std::pair<int, int>> availablePositions;
    for (int y = 1; y < width - 1; ++y) {
        for (int x = 1; x < height - 1; ++x) {
            if (m_location->getTiles()[x][y]->isPassable()) {
                availablePositions.push_back({x, y});
            }
        }
    }

    // Shuffle the available positions
    std::shuffle(availablePositions.begin(), availablePositions.end(), std::mt19937(std::random_device()()));

    for (int i = 0; i < numGoldPiles && !availablePositions.empty(); ++i) {
        int goldAmount = rand() % 101 + 50; // Random gold amount between 50 and 150
        Gold* gold = new Gold(goldAmount);

        auto pos = availablePositions.back();
        availablePositions.pop_back();
        m_location->setTile(pos.first, pos.second, new Tile(gold->getGoldSymbol(), false, gold));
    }
}

void DefaultLocationBuilder::setPlayer() {
    int width = m_location->getTiles()[0].size();
    int height = m_location->getTiles().size();
    int x = rand() % (height - 2) + 1;
    int y = rand() % (width - 2) + 1;

    Player* player = Player::createPlayer();
    LOG_INFO("Player created as: " + player->getPlayerTypeString());
    std::cout << x << y;
    std::cout << m_location->getTiles()[x][y]->getSymbol();
    player->setXY(std::make_pair(x, y));
    m_location->setTile(x, y, new Tile(player->getPlayerSymbol(), false, player));
}

Location* DefaultLocationBuilder::getLocation() {
    return m_location;
}