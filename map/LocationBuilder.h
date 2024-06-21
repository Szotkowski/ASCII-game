//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_LOCATIONBUILDER_H
#define NEW_PROJEKT_ZOO_LOCATIONBUILDER_H

#include "Location.h"

class LocationBuilder {
public:
    virtual ~LocationBuilder() {};

    virtual void emptyLayout() = 0;

    virtual void setDoors(Side doorSide = Random) = 0;

    virtual void setMonsters(int monsterPercentage = 8) = 0;

    virtual void setItems(int itemPercentage = 5) = 0;

    virtual void setGold(int goldPercentage = 2) = 0;

    virtual void setPlayer() = 0;

    virtual Location* getLocation() = 0;
};

#endif //NEW_PROJEKT_ZOO_LOCATIONBUILDER_H
