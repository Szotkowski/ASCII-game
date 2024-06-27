//
// Created by Michael Szotkowski on 6/17/2024.
//

#undef max
#define NOMINMAX
#include "Location.h"
#include "../GameEngine.h"

Location::Location(const std::string& name, const std::vector<std::vector<Tile*>>& tiles, const std::unordered_map<std::string, size_t>& destinations, int width, int height, Tile* tile) {
    LOG_INFO("Creating location with name: " + name + " and size: " + std::to_string(width) + "x" + std::to_string(height) + ".");
    m_name = name;
    m_tiles = tiles;
    m_destinations = destinations;
    m_width = width;
    m_height = height;
    m_previousTile = tile;
}

Location::Location(std::string name) {
    LOG_INFO("Creating location with name: " + name + ".");
    m_name = std::move(name);
}

Location::~Location() {
    LOG_INFO("Destroying location: " + m_name + ".");
    // Delete all dynamically allocated Tile objects
    for (auto& row : m_tiles) {
        for (auto& tile : row) {
            delete tile;
        }
    }

    // Clear the vectors
    m_tiles.clear();
    m_destinations.clear();

    // Delete the previous tile
    delete m_previousTile;
}

std::vector<std::vector<Tile*>> Location::getTiles() {
    return m_tiles;
}

Player* Location::getPlayer() {
    for (int y = 0; y < m_width; ++y) {
        for (int x = 0; x < m_height; ++x) {
            if (Player* player = dynamic_cast<Player*>(m_tiles[x][y]->getEntity())) {
                return player;
            }
        }
    }
    LOG_ERROR("No player found in the location.");
    std::cerr << RED_TEXT << "Error: No player found in the location." << RESET_TEXT << std::endl;
    return nullptr;
}

void Location::setTile(int x, int y, Tile *tile) {
    if (x >= 0 && y >= 0 && x <= m_width && y <= m_height) {
        m_tiles[x][y] = tile;
    }
}

bool Location::move(int currentX, int currentY, int newX, int newY, Tile* newTile) {
    LOG_INFO("Moving from (" + std::to_string(currentX) + ", " + std::to_string(currentY) + ") to (" + std::to_string(newX) + ", " + std::to_string(newY) + ").");
    // Ensure both current and new positions are within bounds
    if (currentY < 0 || currentX < 0 || currentY >= m_width || currentX >= m_height ||
        newY < 0 || newX < 0 || newY >= m_width || newX >= m_height) {
        std::cerr << RED_TEXT << "Invalid move, out of bounds." << RED_TEXT << std::endl;
        return false;
    }
    if (currentX == newX && currentY == newY) {
        std::string destinationId = std::to_string(newX) + "," + std::to_string(newY);
        size_t destinationIndex = getDestinationIndex(destinationId);
        if (destinationIndex != std::numeric_limits<size_t>::max()) {
            Map::getInstance().switchLocation(destinationIndex);
        } else {
            Map::getInstance().createNewLocation();
        }
        Map::getInstance().displayCurrentMap();
        GameEngine::getInstance().saveGame();
        return false;
    }
    // Check if the new tile at the destination is passable
    if (m_tiles[newX][newY]->isPassable()) {
        // Move the previous tile to the current position
        m_tiles[currentX][currentY] = m_previousTile;
        // Store the symbol of the previous passable tile
        m_previousTile = m_tiles[newX][newY];
        // Move the new tile to the destination
        m_tiles[newX][newY] = newTile;
        return true; // Move successful
    }
    // If the new tile is not passable, check if it contains a monster
    if (Monster* monster = dynamic_cast<Monster*>(m_tiles[newX][newY]->getEntity())) {
        Map::getInstance().displayCurrentMap();
        return false; // Combat took place, but move not completed
    }
    // Check if the tile contains gold
    if (Gold* gold = dynamic_cast<Gold*>(m_tiles[newX][newY]->getEntity())) {
        // Add the gold to the player's inventory
        Map::getInstance().getCurrentLocation()->getPlayer()->addGold(gold->getGoldAmount());
        // Make the tile passable after picking up the gold
        m_tiles[newX][newY] = new Tile(' ', true);
        Map::getInstance().displayCurrentMap();
        return false; // Move not completed regardless of gold action
    }
    // Check if the tile contains an item
    if (Item* item = dynamic_cast<Item*>(m_tiles[newX][newY]->getEntity())) {
        // Check the type of item and print its properties accordingly
        // Print common item properties
        std::cout << item->getItemName() << ", Item price: " << item->getItemPrice() << ", Item rarity: " << item->getItemRarityString();
        if (Potion* potion = dynamic_cast<Potion*>(item)) {
            // Print potion properties
            std::cout << ", Potion strength: " << potion->getPotionPower() << std::endl;
        } else if (Weapon* weapon = dynamic_cast<Weapon*>(item)) {
            // Print weapon properties
            std::cout << ", Weapon attack: " << weapon->getWeaponAttack() << std::endl;
        } else if (Armor* armor = dynamic_cast<Armor*>(item)) {
            // Print armor properties
            std::cout << ", Armor defense: " << armor->getArmorDefense() << std::endl;
        }
        std::cout << "Choose action: [1] Pick up  [2] Leave it" << std::endl;
        int choice = 0;
        while (true) {
            char input = _getch(); // Get a character from the keyboard without needing to press enter
            if (input >= '1' && input <= '2') {
                choice = input - '0'; // Convert character to integer
                break; // Exit the loop if a valid choice is entered
            } else {
                LOG_ERROR("Invalid input.");
                std::cerr << RED_TEXT << "Invalid input, please enter 1 or 2." << RESET_TEXT << std::endl;
            }
        }
        switch (choice) {
            case 1:
                // Add the item to the inventory if possible
                if (Inventory::getInstance().addItem(item)) {
                    // Make the tile passable after picking up the item
                    m_tiles[newX][newY] = new Tile(' ', true);
                }
                break;
            case 2:
                break;
            default:
                LOG_ERROR("Invalid choice.");
                std::cerr << RED_TEXT << "Invalid choice." << RESET_TEXT << std::endl;
                break;
        }
        Map::getInstance().displayCurrentMap();
        return false; // Move not completed regardless of item action
    }
    return false; // Invalid destination, tile not passable
}

void Location::addDestination(const std::string& destinationId, size_t locationIndex) {
    m_destinations[destinationId] = locationIndex;
}

std::pair<int, int> Location::getOppositeSidePosition(int x, int y, const Location* newLocation) {
    Side playerSide = getSideFromPosition(x, y);

    int newX = x;
    int newY = y;

    switch (playerSide) {
        case Bottom:
            newX = newLocation->m_height - 1;
            for (int i = 0; i < newLocation->m_width; ++i) {
                if (newLocation->m_tiles[newX][i]->isPassable() && newLocation->m_tiles[newX][i]->getSymbol() != '#') {
                    newY = i;
                    break;
                }
            }
            break;

        case Top:
            newX = 0;
            for (int i = 0; i < newLocation->m_width; ++i) {
                if (newLocation->m_tiles[newX][i]->isPassable() && newLocation->m_tiles[newX][i]->getSymbol() != '#') {
                    newY = i;
                    break;
                }
            }
            break;

        case Right:
            newY = newLocation->m_width - 1;
            for (int i = 0; i < newLocation->m_height; ++i) {
                if (newLocation->m_tiles[i][newY]->isPassable() && newLocation->m_tiles[i][newY]->getSymbol() != '#') {
                    newX = i;
                    break;
                }
            }
            break;

        case Left:
            newY = 0;
            for (int i = 0; i < newLocation->m_height; ++i) {
                if (newLocation->m_tiles[i][newY]->isPassable() && newLocation->m_tiles[i][newY]->getSymbol() != '#') {
                    newX = i;
                    break;
                }
            }
            break;

        default:
            return std::make_pair(x, y);
    }

    return std::make_pair(newX, newY);
}

Side Location::getSideFromPosition(int x, int y) {
    int width = m_width;
    int height = m_height;

    if (x == 0) {
        return Bottom;
    } else if (x == height - 1) {
        return Top;
    } else if (y == 0) {
        return Right;
    } else if (y == width - 1) {
        return Left;
    } else {
        return Random;
    }
}

size_t Location::getDestinationIndex(const std::string& destinationId) {
    auto it = m_destinations.find(destinationId);
    if (it != m_destinations.end()) {
        return it->second;
    }
    return std::numeric_limits<size_t>::max();
}

std::string Location::getName() {
    return m_name;
}

bool Location::setPreviousTile(Tile *tile) {
    if (m_previousTile->getEntity() == nullptr) {
        m_previousTile = tile;
        return true;
    }
    return false;
}

void Location::setHeight(int height) {
    m_height = height;
}

void Location::setWidth(int width) {
    m_width = width;
}

void Location::setTile(std::vector<std::vector<Tile*>> tiles) {
    m_tiles = tiles;
}

void Location::saveLocation(std::ofstream& file) {
    LOG_INFO("Saving location: " + m_name + ".");
    size_t numRows = m_tiles.size();
    file.write(reinterpret_cast<const char*>(&numRows), sizeof(size_t));
    for (const auto& row : m_tiles) {
        size_t numTiles = row.size();
        file.write(reinterpret_cast<const char*>(&numTiles), sizeof(size_t));
        for (const auto& tile : row) {
            tile->saveTile(file);
        }
    }

    // Save destinations
    size_t numDestinations = m_destinations.size();
    file.write(reinterpret_cast<const char*>(&numDestinations), sizeof(size_t));
    for (const auto& door : m_destinations) {
        size_t destinationIdLength = door.first.size();
        file.write(reinterpret_cast<const char*>(&destinationIdLength), sizeof(size_t));
        file.write(door.first.c_str(), destinationIdLength);
        file.write(reinterpret_cast<const char*>(&door.second), sizeof(size_t)); // Save index of the destination location
    }

    file.write(reinterpret_cast<const char*>(&m_width), sizeof(m_width));
    file.write(reinterpret_cast<const char*>(&m_height), sizeof(m_height));
    if (m_previousTile) {
        m_previousTile->saveTile(file);
    }

    size_t nameSize = m_name.size();
    file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    file.write(m_name.c_str(), nameSize);

    // Save if this is the current location
    bool isCurrentLocation = (this == Map::getInstance().getCurrentLocation());
    file.write(reinterpret_cast<const char*>(&isCurrentLocation), sizeof(bool));
}

Location* Location::loadLocation(std::ifstream& file) {
    size_t numRows;
    file.read(reinterpret_cast<char*>(&numRows), sizeof(size_t));
    std::vector<std::vector<Tile*>> tiles;
    for (size_t i = 0; i < numRows; ++i) {
        size_t numTiles;
        file.read(reinterpret_cast<char*>(&numTiles), sizeof(size_t));
        std::vector<Tile*> row;
        for (size_t j = 0; j < numTiles; ++j) {
            Tile* tile = Tile::loadTile(file);
            row.push_back(tile);
        }
        tiles.push_back(row);
    }

    // Load destinations
    size_t numDestinations;
    file.read(reinterpret_cast<char*>(&numDestinations), sizeof(size_t));
    std::unordered_map<std::string, size_t> destinations;
    for (size_t i = 0; i < numDestinations; ++i) {
        size_t destinationIdLength;
        file.read(reinterpret_cast<char*>(&destinationIdLength), sizeof(size_t));
        std::string destinationId(destinationIdLength, '\0');
        file.read(&destinationId[0], destinationIdLength);

        size_t locationIndex;
        file.read(reinterpret_cast<char*>(&locationIndex), sizeof(size_t)); // Load index of the destination location
        destinations[destinationId] = locationIndex;
    }

    int width, height;
    file.read(reinterpret_cast<char*>(&width), sizeof(width));
    file.read(reinterpret_cast<char*>(&height), sizeof(height));
    Tile* previousTile = Tile::loadTile(file);

    size_t name_length;
    file.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
    std::string name(name_length, '\0');
    file.read(&name[0], name_length);

    LOG_INFO("Creating location with name: " + name + " and size: " + std::to_string(width) + "x" + std::to_string(height) + ".");
    Location* location = new Location(name, tiles, destinations, width, height, previousTile);

    // Load if this is the current location
    bool isCurrentLocation;
    file.read(reinterpret_cast<char*>(&isCurrentLocation), sizeof(bool));
    if (isCurrentLocation) {
        Map::getInstance().setCurrentLocation(location);
        Player* player = Map::getInstance().getCurrentLocation()->getPlayer();
        if (player == nullptr) {
            LOG_ERROR("Player is dead.");
            GameEngine::getInstance().deathScreen();
        }
    }

    return location;
}