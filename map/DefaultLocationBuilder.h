//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_DEFAULTLOCATIONBUILDER_H
#define NEW_PROJEKT_ZOO_DEFAULTLOCATIONBUILDER_H

#include <algorithm>
#include "LocationBuilder.h"
#include "Location.h"
#include "../monster/Monster.h"
#include "../monster/MonsterFactory.h"

class DefaultLocationBuilder : public LocationBuilder {
private:
    Location* m_location;

public:
    DefaultLocationBuilder();

    ~DefaultLocationBuilder() override;

    void emptyLayout() override;

    void setDoors(Side doorSide) override;

    void setMonsters(int monsterPercentage) override;

    void setItems(int itemPercentage) override;

    void setGold(int goldPercentage) override;

    void setPlayer() override;

    Location* getLocation() override;
};

#endif //NEW_PROJEKT_ZOO_DEFAULTLOCATIONBUILDER_H
