//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_ITEM_H
#define NEW_PROJEKT_ZOO_ITEM_H

#include "iostream"
#include "../entity/Entity.h"

enum ItemRarity {
    COMMON = 1,
    UNCOMMON = 2,
    RARE = 3,
    LEGENDARY = 4,
};

class Item : public Entity {
protected:
    char m_itemSymbol;

    std::string m_itemName;

    int m_itemPrice;

    ItemRarity m_itemRarity;

public:
    Item(std::string name, int price, ItemRarity rarity);

    char getItemSymbol();

    std::string getItemName();

    int getItemPrice();

    ItemRarity getItemRarity();

    std::string getItemRarityString();

    void saveEntity(std::ofstream &file) override = 0;
};

#endif //NEW_PROJEKT_ZOO_ITEM_H
