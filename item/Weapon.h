//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_WEAPON_H
#define NEW_PROJEKT_ZOO_WEAPON_H

#include "Item.h"

enum WeaponType {
    SWORD,
    AXE,
    BOW,
    STAFF
};

class Weapon : public Item {
private:
    WeaponType m_weaponType;

    int m_weaponAttack;

public:
    void saveEntity(std::ofstream& file) override;

    Weapon(std::string name, int price, ItemRarity rarity, int weponAttack, WeaponType weaponType);

    WeaponType getWeaponType();

    int getWeaponAttack();
};

#endif //NEW_PROJEKT_ZOO_WEAPON_H
