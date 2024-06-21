//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Armor.h"

void Armor::saveEntity(std::ofstream &file) {
    LOG_INFO("Saving Armor entity to file");
    EntityType entityType = ITEM;
    file.write(reinterpret_cast<const char*>(&entityType), sizeof(entityType));

    ItemTypes itemType = ARMOR;
    file.write(reinterpret_cast<const char*>(&itemType), sizeof(itemType));

    size_t nameLength = m_itemName.size();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(m_itemName.c_str(), nameLength);

    file.write(reinterpret_cast<const char*>(&m_itemPrice), sizeof(m_itemPrice));
    file.write(reinterpret_cast<const char*>(&m_itemRarity), sizeof(m_itemRarity));
    file.write(reinterpret_cast<const char*>(&m_armorDefense), sizeof(m_armorDefense));
}

Armor::Armor(std::string name, int price, ItemRarity rarity, int armorDefense) : Item(name, price, rarity) {
    m_itemName = "Armor: " + name;
    m_armorDefense = armorDefense * (int)rarity;
}

int Armor::getArmorDefense() {
    return m_armorDefense;
}