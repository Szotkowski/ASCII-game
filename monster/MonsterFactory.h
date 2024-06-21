//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_MONSTERFACTORY_H
#define NEW_PROJEKT_ZOO_MONSTERFACTORY_H

#include "Monster.h"
#include "Goblin.h"
#include "Troll.h"
#include "Dragon.h"

enum MonsterType {
    GOBLIN = 1,
    TROLL = 2,
    DRAGON = 3
};

class MonsterFactory {
public:
    static Monster* createMonster(MonsterType monsterType, const std::string& name, int attack, int defense, int hp, MonsterLevel monsterLevel);

    static Monster* createMonsterFromData(std::ifstream& file);
};

#endif //NEW_PROJEKT_ZOO_MONSTERFACTORY_H
