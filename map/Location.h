//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_LOCATION_H
#define NEW_PROJEKT_ZOO_LOCATION_H

#include <unordered_map>
#include <optional>
#include <vector>
#include "Tile.h"
#include "../gold/Gold.h"
#include "../player/Player.h"

enum Side {
    Top,
    Right,
    Bottom,
    Left,
    Random
};

class Location {
private:
    std::vector<std::vector<Tile*>> m_tiles;

    std::unordered_map<std::string, size_t> m_destinations;

    int m_width;

    int m_height;

    Tile* m_previousTile = new Tile(' ', true, nullptr);

    std::string m_name;

    Location(const std::string& name, const std::vector<std::vector<Tile*>>& tiles, const std::unordered_map<std::string, size_t>& destinations, int width, int height, Tile* tile);

public:
    Location(std::string name);

    ~Location();

    std::vector<std::vector<Tile*>> getTiles();

    Player* getPlayer();

    void setTile(int x, int y, Tile* tile);

    bool move(int currentX, int currentY, int newX, int newY, Tile* newTile);

    void addDestination(const std::string& destinationId, size_t locationIndex);

    size_t getDestinationIndex(const std::string& destinationId);

    std::pair<int, int> getOppositeSidePosition(int x, int y, const Location* newLocation);

    Side getSideFromPosition(int x, int y);

    std::string getName();

    bool setPreviousTile(Tile* tile);

    void setHeight(int height);

    void setWidth(int width);

    void setTile(std::vector<std::vector<Tile*>> tiles);

    void saveLocation(std::ofstream &file);

    static Location* loadLocation(std::ifstream &file);
};
#endif //NEW_PROJEKT_ZOO_LOCATION_H
