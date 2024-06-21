//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_GOBLIN_H
#define NEW_PROJEKT_ZOO_GOBLIN_H

#include "Monster.h"
#include "MonsterFactory.h"

class Goblin : public Monster {
protected:
    void saveEntity(std::ofstream& file) override;

public:
    Goblin(std::string name, int attack, int defense, int hp, MonsterLevel monsterLevel);
};

#endif //NEW_PROJEKT_ZOO_GOBLIN_H
