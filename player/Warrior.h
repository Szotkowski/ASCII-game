//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_WARRIOR_H
#define NEW_PROJEKT_ZOO_WARRIOR_H

#include "Player.h"

class Warrior : public Player {
protected:
    void saveEntity(std::ofstream& file) override;

    Warrior(std::string& name, int hp, int attack, int defense, int gold, int x, int y, Armor* armor, Weapon* weapon);

public:
    Warrior();

    static  Warrior* loadWarrior(std::ifstream& file);
};

#endif //NEW_PROJEKT_ZOO_WARRIOR_H
