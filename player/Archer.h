//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_ARCHER_H
#define NEW_PROJEKT_ZOO_ARCHER_H

#include "Player.h"

class Archer : public Player {
protected:
    void saveEntity(std::ofstream& file) override;

    Archer(std::string& name, int hp, int attack, int defense, int gold, int x, int y, Armor* armor, Weapon* weapon);

public:
    Archer();

    static Archer* loadArcher(std::ifstream& file);
};

#endif //NEW_PROJEKT_ZOO_ARCHER_H
