//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Item.h"

Item::Item(std::string name, int price, ItemRarity rarity) {
    LOG_INFO("Creating Item entity.");
    m_itemSymbol = 'I';
    m_itemName = name;
    m_itemPrice = price * (int)rarity;
    m_itemRarity = rarity;
}

char Item::getItemSymbol() {
    return m_itemSymbol;
}

std::string Item::getItemName() {
    return m_itemName;
}

int Item::getItemPrice() {
    return m_itemPrice;
}

ItemRarity Item::getItemRarity() {
    return m_itemRarity;
}

std::string Item::getItemRarityString() {
    switch (m_itemRarity) {
        case COMMON:
            return "Common";
        case UNCOMMON:
            return "Uncommon";
        case RARE:
            return "Rare";
        case LEGENDARY:
            return "Legendary";
        default:
            return "Invalid rarity";
    }
}