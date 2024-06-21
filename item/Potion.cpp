//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Potion.h"
#include "ItemFactory.h"

void Potion::saveEntity(std::ofstream &file) {
    LOG_INFO("Saving Potion entity to file");
    EntityType entityType = ITEM;
    file.write(reinterpret_cast<const char*>(&entityType), sizeof(entityType));

    ItemTypes itemType = POTION;
    file.write(reinterpret_cast<const char*>(&itemType), sizeof(itemType));

    size_t nameLength = m_itemName.size();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(m_itemName.c_str(), nameLength);

    file.write(reinterpret_cast<const char*>(&m_itemPrice), sizeof(m_itemPrice));
    file.write(reinterpret_cast<const char*>(&m_itemRarity), sizeof(m_itemRarity));
    file.write(reinterpret_cast<const char*>(&m_potionType), sizeof(m_potionType));
    file.write(reinterpret_cast<const char*>(&m_potionPower), sizeof(m_potionPower));
}

Potion::Potion(std::string name, int price, ItemRarity rarity, int power, PotionType potionType) : Item(name, price, rarity) {
    switch (potionType) {
        case HEALTH:
            m_itemName = "Health potion: " + m_itemName;
            break;
        case STRENGTH:
            m_itemName = "Strength potion: " + m_itemName;
            break;
        case DEFENSE:
            m_itemName = "Defense potion: " + m_itemName;
            break;
        default:
            break;
    }
    m_potionType = potionType;
    m_potionPower = power * (int)rarity;
}

PotionType Potion::getPotionType() {
    return m_potionType;
}

int Potion::getPotionPower() {
    return m_potionPower;
}