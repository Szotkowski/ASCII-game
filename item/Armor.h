//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_ARMOR_H
#define NEW_PROJEKT_ZOO_ARMOR_H

#include "Item.h"
#include "ItemFactory.h"

class Armor : public Item {
private:
    int m_armorDefense;

public:
    void saveEntity(std::ofstream& file) override;

    Armor(std::string name, int price, ItemRarity rarity, int armorDefense);

    int getArmorDefense();
};

#endif //NEW_PROJEKT_ZOO_ARMOR_H
