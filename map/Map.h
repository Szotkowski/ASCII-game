//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_MAP_H
#define NEW_PROJEKT_ZOO_MAP_H

#include <conio.h>
#include <optional>
#include <limits>
#include "LocationDirector.h"

enum Difficulty {
    EASY,
    MEDIUM,
    HARD
};

class Map {
private:
    std::vector<Location*> m_locations;

    Location* m_currentLocation = nullptr;

    Map();

    std::string m_mapName;

    static Map* instance;

    LocationDirector m_locationDirector;

public:
    ~Map();

    void setMapName();

    void setMapDifficulty();

    static Map& getInstance();

    static void destroyInstance();

    std::string getMapName();

    Map(const Map &) = delete;

    Map &operator=(const Map &) = delete;

    Map(Map &&) = delete;

    Map &operator=(Map &&) = delete;

    Location* getCurrentLocation();

    void displayCurrentMap();

    void setCurrentLocation(Location* newLocation);

    void switchLocation(size_t locationIndex);

    Location *createNewLocation(bool first = false);

    void saveMap(std::ofstream &file);

    static void loadMap(std::ifstream &file);
};

#endif //NEW_PROJEKT_ZOO_MAP_H
