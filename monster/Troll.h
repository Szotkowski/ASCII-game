//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_TROLL_H
#define NEW_PROJEKT_ZOO_TROLL_H

#include "Monster.h"
#include "MonsterFactory.h"

class Troll : public Monster {
protected:
    void saveEntity(std::ofstream& file) override;

public:
    Troll(std::string name, int attack, int defense, int hp, MonsterLevel monsterLevel);
};

#endif //NEW_PROJEKT_ZOO_TROLL_H
