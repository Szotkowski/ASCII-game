//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Weapon.h"
#include "ItemFactory.h"

void Weapon::saveEntity(std::ofstream &file) {
    LOG_INFO("Saving Weapon entity to file");
    EntityType entityType = ITEM;
    file.write(reinterpret_cast<const char*>(&entityType), sizeof(entityType));

    ItemTypes itemType = WEAPON;
    file.write(reinterpret_cast<const char*>(&itemType), sizeof(itemType));

    size_t nameLength = m_itemName.size();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(m_itemName.c_str(), nameLength);

    file.write(reinterpret_cast<const char*>(&m_itemPrice), sizeof(m_itemPrice));
    file.write(reinterpret_cast<const char*>(&m_itemRarity), sizeof(m_itemRarity));
    file.write(reinterpret_cast<const char*>(&m_weaponType), sizeof(m_weaponType));
    file.write(reinterpret_cast<const char*>(&m_weaponAttack), sizeof(m_weaponAttack));
}

Weapon::Weapon(std::string name, int price, ItemRarity rarity, int weaponAttack, WeaponType weaponType) : Item(name, price, rarity) {
    switch (weaponType) {
        case SWORD:
            m_itemName = "Sword: " + m_itemName;
            break;
        case AXE:
            m_itemName = "Axe: " + m_itemName;
            break;
        case BOW:
            m_itemName = "Bow: " + m_itemName;
            break;
        case STAFF:
            m_itemName = "Staff: " + m_itemName;
            break;
        default:
            break;
    }
    m_weaponType = weaponType;
    m_weaponAttack = weaponAttack * (int)rarity;
}

WeaponType Weapon::getWeaponType() {
    return m_weaponType;
}

int Weapon::getWeaponAttack() {
    return m_weaponAttack;
}