//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_TILE_H
#define NEW_PROJEKT_ZOO_TILE_H

#include "../entity/Entity.h"

class Tile {
private:
    char m_symbol;

    bool m_passable;

    std::unique_ptr<Entity> m_entity;

public:
    Tile(char symbol, bool passable, Entity *entity = nullptr);

    char getSymbol();

    bool isPassable();

    Entity* getEntity();

    void saveTile(std::ofstream &file);

    static Tile *loadTile(std::ifstream &file);
};

#endif //NEW_PROJEKT_ZOO_TILE_H
