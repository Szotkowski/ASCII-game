//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Map.h"

Map* Map::instance = nullptr;

Map::Map() {
    LOG_INFO("Creating Map instance.");
    m_locationDirector = LocationDirector();
}

Map::~Map() {
    LOG_INFO("Destroying Map instance.");
    for (auto& location : m_locations) {
        delete location;
    }
    // Clear the vector of locations
    m_locations.clear();
}

void Map::setMapName() {
    std::cout << "Enter map name: ";
    std::string mapName;
    std::getline(std::cin >> std::ws, mapName);
    // Check for empty or whitespace-only input
    if (mapName.empty() || std::all_of(mapName.begin(), mapName.end(), isspace)) {
        std::cout << "Invalid map name. Please enter a non-empty name." << std::endl;
        // Clear the input buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return; // Exit the function without setting the map name
    }
    // Check for maximum length
    constexpr size_t MAX_MAP_NAME_LENGTH = 50;
    if (mapName.length() > MAX_MAP_NAME_LENGTH) {
        std::cout << "Map name is too long. Please enter a name with at most " << MAX_MAP_NAME_LENGTH << " characters." << std::endl;
        return; // Exit the function without setting the map name
    }
    // Set the map name
    m_mapName = mapName;
}


void Map::setMapDifficulty() {
    std::cout << "Select difficulty level: [1] Easy  [2] Medium  [3] Hard" << std::endl;
    while (true) {
        char input = _getch();
        if (input >= '1' && input <= '3') {
            break;
        }
        std::cout << "Invalid choice. Please enter a number between 1 and 3." << std::endl;
    }
    /*switch (choice) {
    case 1:
        //m_locationDirector.setBuilder(new EasyLocationBuilder());
            //break;
                return;
    case 2:
        //m_locationDirector.setBuilder(new MediumLocationBuilder());
            //break;
                return;
    case 3:
        //m_locationDirector.setBuilder(new HardLocationBuilder());
            //break;
                return;
    default:
        return;
    }*/
}

std::string Map::getMapName() {
    return m_mapName;
}

Location* Map::getCurrentLocation() {
    return m_currentLocation;
}

Map& Map::getInstance() {
    if (instance == nullptr) {
        instance = new Map();
    }
    return *instance;
}

void Map::destroyInstance() {
    delete instance;
    instance = nullptr;
}

void Map::displayCurrentMap() {
    if (m_currentLocation) {
        std::cout << "Current Location: " << m_currentLocation->getName() << std::endl;
        for (const auto& row : m_currentLocation->getTiles()) {
            for (const auto& tile : row) {
                std::cout << tile->getSymbol() << " ";
            }
            std::cout << std::endl;
        }
    } else {
        LOG_ERROR("No current location set.");
        std::cerr << RED_TEXT << "No current location set." << RESET_TEXT << std::endl;
    }
}

void Map::setCurrentLocation(Location* location) {
    m_currentLocation = location;
}

void Map::switchLocation(size_t locationIndex) {
    Player* player = getInstance().getCurrentLocation()->getPlayer();
    Location* location = m_locations[locationIndex];
    std::pair<int, int> oppositeSidePosition = m_currentLocation->getOppositeSidePosition(player->getX(), player->getY(), location);
    m_currentLocation->setTile(player->getX(), player->getY(), new Tile(' ', true));
    player->setXY(oppositeSidePosition);
    Map::getInstance().displayCurrentMap();
    location->setTile(player->getX(), player->getY(), new Tile(player->getPlayerSymbol(), false, player));
    m_currentLocation = location;
}

Location* Map::createNewLocation(bool first) {
    // Check if there are existing locations
    if (!first) {
        Player* player = getInstance().getCurrentLocation()->getPlayer();
        Side doorSide = getInstance().getCurrentLocation()->getSideFromPosition(player->getX(), player->getY());
        Location* newLocation = m_locationDirector.buildLocation(doorSide);
        m_locations.push_back(newLocation);

        // Add destination to current location
        m_currentLocation->addDestination(std::to_string(player->getX()) + "," + std::to_string(player->getY()), m_locations.size() - 1);

        // Move player to the opposite side
        std::pair<int, int> oppositeSidePosition = m_currentLocation->getOppositeSidePosition(player->getX(), player->getY(), newLocation);
        m_currentLocation->setTile(player->getX(), player->getY(), new Tile(' ', true));
        player->setXY(oppositeSidePosition);

        // Add destination to the new location
        int index = std::find(m_locations.begin(), m_locations.end(), m_currentLocation) - m_locations.begin();
        m_locations.back()->addDestination(std::to_string(player->getX()) + "," + std::to_string(player->getY()), index);

        // Set player's tile in the new location
        m_locations.back()->setTile(player->getX(), player->getY(), new Tile(player->getPlayerSymbol(), false, player));

        // Update current location
        m_currentLocation = newLocation;
        return newLocation;
    } else {
        // If there are no existing locations, create a new one without specifying the door side
        Location* newLocation = m_locationDirector.buildLocation(Random, true);
        m_locations.push_back(newLocation);
        m_currentLocation = newLocation;
        return newLocation;
    }
}

void Map::saveMap(std::ofstream &file) {
    LOG_INFO("Saving Map instance to file.");
    size_t nameSize = m_mapName.size();
    file.write(reinterpret_cast<const char*>(&nameSize), sizeof(size_t));
    file.write(m_mapName.c_str(), nameSize);

    size_t numLocations = m_locations.size();
    file.write(reinterpret_cast<const char*>(&numLocations), sizeof(size_t));
    for (const auto& loc : m_locations) {
        loc->saveLocation(file);
    }
}

void Map::loadMap(std::ifstream &file) {
    LOG_INFO("Creating Map instance from file.");
    destroyInstance();

    Map& mapInstance = getInstance();

    size_t nameSize;
    file.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
    std::string tempMapName(nameSize, '\0');
    file.read(&tempMapName[0], nameSize);

    size_t numLocations;
    file.read(reinterpret_cast<char*>(&numLocations), sizeof(size_t));
    std::vector<Location*> tempLocations;
    for (size_t i = 0; i < numLocations; ++i) {
        Location* loc = Location::loadLocation(file);
        tempLocations.push_back(loc);
    }

    mapInstance.m_mapName = tempMapName;
    mapInstance.m_locations = tempLocations;
}