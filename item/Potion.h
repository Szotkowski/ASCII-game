//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_POTION_H
#define NEW_PROJEKT_ZOO_POTION_H

#include "Item.h"

enum PotionType {
    HEALTH,
    STRENGTH,
    DEFENSE
};

class Potion : public Item {
private:
    PotionType m_potionType;

    int m_potionPower;

public:
    void saveEntity(std::ofstream& file) override;

    Potion(std::string name, int price, ItemRarity rarity, int power, PotionType potionType);

    PotionType getPotionType();

    int getPotionPower();
};

#endif //NEW_PROJEKT_ZOO_POTION_H
