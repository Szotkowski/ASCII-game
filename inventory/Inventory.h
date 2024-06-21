//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_INVENTORY_H
#define NEW_PROJEKT_ZOO_INVENTORY_H

#include "../item/ItemFactory.h"

class Inventory {
private:
    Inventory();

    static Inventory* instance;

    Item* m_inventoryItems[9];

    void displayInventory(int currentIndex);

    void processKeyInput(int key, int& currentIndex, bool& exitInventory);

    void handleActionSelection(int currentIndex);

    void useItem(int currentIndex);

    void dropItem(int currentIndex);

public:
    static Inventory &getInstance();

    ~Inventory();

    static void destroyInstance();

    bool addItem(Item *item);

    void removeItem(Item *item);

    Item *getItem(int index);

    Item *getInventoryItems();

    void printInventory();

    void saveInventory(std::ofstream &file);

    static void loadInventory(std::ifstream &file);

    Inventory(const Inventory &) = delete;

    Inventory &operator=(const Inventory &) = delete;

    Inventory(Inventory &&) = delete;

    Inventory &operator=(Inventory &&) = delete;
};

#endif //NEW_PROJEKT_ZOO_INVENTORY_H
