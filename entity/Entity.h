//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_ENTITY_H
#define NEW_PROJEKT_ZOO_ENTITY_H

#include "../logger/Logger.h"
#include <fstream>
#include <iostream>

enum EntityType {
    PLAYER = 1,
    MONSTER = 2,
    GOLD = 3,
    ITEM = 4
};

class Entity {
public:
    virtual void saveEntity(std::ofstream &file) = 0;

    static Entity* createEntityFromFile(std::ifstream &file);
};

#endif //NEW_PROJEKT_ZOO_ENTITY_H
