//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_ITEMFACTORY_H
#define NEW_PROJEKT_ZOO_ITEMFACTORY_H

#include "Weapon.h"
#include "Potion.h"
#include "Armor.h"

enum ItemTypes {
    WEAPON = 1,
    POTION = 2,
    ARMOR = 3
};

class ItemFactory {
public:
    static Item* createItem(ItemTypes itemType, const std::string& name, int price, ItemRarity rarity, int extraParam = 0, WeaponType extraParam2 = SWORD, PotionType extraParam3 = HEALTH);

    static Item* createItemFromData(std::ifstream& file);
};

#endif //NEW_PROJEKT_ZOO_ITEMFACTORY_H
