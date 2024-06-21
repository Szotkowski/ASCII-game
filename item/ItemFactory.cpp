//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "ItemFactory.h"

Item *ItemFactory::createItem(ItemTypes itemType, const std::string &name, int price, ItemRarity rarity, int extraParam, WeaponType extraParam2, PotionType extraParam3) {
    switch (itemType) {
        case WEAPON:
            return new Weapon(name, price, rarity, extraParam, extraParam2);
        case POTION:
            return new Potion(name, price, rarity, extraParam, extraParam3);
        case ARMOR:
            return new Armor(name, price, rarity, extraParam);
        default:
            LOG_WARNING("Invalid item type.");
        std::cerr << RESET_TEXT << "Invalid item type" << RESET_TEXT << std::endl;
            return nullptr;
    }
}

Item* ItemFactory::createItemFromData(std::ifstream &file) {
    ItemTypes itemType;
    file.read(reinterpret_cast<char*>(&itemType), sizeof(itemType));

    size_t nameSize;
    file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    std::string name(nameSize, '\0');
    file.read(&name[0], nameSize);

    int price;
    file.read(reinterpret_cast<char*>(&price), sizeof(price));

    ItemRarity rarity;
    file.read(reinterpret_cast<char*>(&rarity), sizeof(rarity));

    switch (itemType) {
        case WEAPON: {
                WeaponType weaponType;
                int weaponDamage;
                file.read(reinterpret_cast<char*>(&weaponDamage), sizeof(weaponDamage));
                file.read(reinterpret_cast<char*>(&weaponType), sizeof(weaponType));
                LOG_INFO("Creating weapon from data.");
                return new Weapon(name, price, rarity, weaponDamage, weaponType);
            }
        case POTION: {
                PotionType potionType;
                int potionPower;
                file.read(reinterpret_cast<char*>(&potionType), sizeof(potionType));
                file.read(reinterpret_cast<char*>(&potionPower), sizeof(potionPower));
                LOG_INFO("Creating potion from data.");
                return new Potion(name, price, rarity, potionPower, potionType);
        }
        case ARMOR: {
                int armorDefense;
                file.read(reinterpret_cast<char*>(&armorDefense), sizeof(armorDefense));
                LOG_INFO("Creating weapon from data.");
                return new Armor(name, price, rarity, armorDefense);
        }
        default:
            LOG_INFO("Invalid item type.");
            std::cerr << RESET_TEXT << "Invalid item type" << RESET_TEXT << std::endl;
            return nullptr;
    }
}