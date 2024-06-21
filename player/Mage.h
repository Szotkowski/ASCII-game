//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_MAGE_H
#define NEW_PROJEKT_ZOO_MAGE_H

#include "Player.h"

class Mage : public Player {
protected:
    void saveEntity(std::ofstream& file) override;

    Mage(std::string& name, int hp, int attack, int defense, int gold, int x, int y, Armor* armor, Weapon* weapon);

public:
    Mage();

    static Mage* loadMage(std::ifstream& file);
};

#endif //NEW_PROJEKT_ZOO_MAGE_H
