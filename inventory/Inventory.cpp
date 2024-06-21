//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Inventory.h"
#include "../GameEngine.h"

Inventory* Inventory::instance = nullptr;

Inventory::Inventory() {
    LOG_INFO("Creating Inventory instance.");
    for (Item*& m_inventoryItem : m_inventoryItems) {
        m_inventoryItem = nullptr;
    }
}

Inventory::~Inventory() {
    LOG_INFO("Destroying Inventory instance.");
    for (Item*& item : m_inventoryItems) {
        delete item;
    }
}

Inventory& Inventory::getInstance() {
    if (instance == nullptr) {
        instance = new Inventory();
    }
    return *instance;
}

void Inventory::destroyInstance() {
    delete instance;
    instance = nullptr;
}

bool Inventory::addItem(Item *item) {
    for (Item*& m_inventoryItem : m_inventoryItems) {
        if (m_inventoryItem == nullptr) {
            m_inventoryItem = item;
            return true;
        }
    }
    LOG_WARNING("Inventory is full.");
    std::cout << YELLOW_TEXT << "Inventory is full" << RESET_TEXT << std::endl;
    return false;
}

void Inventory::removeItem(Item *item) {
    for (Item*& inventoryItem: m_inventoryItems) {
        if (inventoryItem == item) {
            inventoryItem = nullptr;
            return;
        }
    }
    LOG_WARNING("Item not found.");
    std::cerr << YELLOW_TEXT << "Item not found" << RESET_TEXT << std::endl;
}

Item* Inventory::getItem(int index) {
    return m_inventoryItems[index];
}

Item* Inventory::getInventoryItems() {
    return *m_inventoryItems;
}

void Inventory::printInventory() {
    LOG_INFO("Inventory opened.");
    int currentIndex = 0;
    bool exitInventory = false;

    do {
        displayInventory(currentIndex);
        int key = _getch();
        processKeyInput(key, currentIndex, exitInventory);
    } while (!exitInventory);
}

void Inventory::displayInventory(int currentIndex) {
    std::cout << "Inventory:" << std::endl;
    std::cout << "+---+---+---+---+---+---+---+---+---+" << std::endl;

    for (int i = 0; i < 9; ++i) {
        if (m_inventoryItems[i] != nullptr) {
            std::cout << "| ";
            if (i == currentIndex) {
                std::cout << "\033[1;32m";
            }
            std::cout << m_inventoryItems[i]->getItemSymbol() << "\033[0m ";
        } else {
            std::cout << "|   ";
        }
    }
    std::cout << "|" << std::endl;
    std::cout << "+---+---+---+---+---+---+---+---+---+" << std::endl;

    Item* item = m_inventoryItems[currentIndex];
    if (item != nullptr) {
        std::cout << item->getItemName() << ", Item price: " << item->getItemPrice() << ", Item rarity: " << item->getItemRarityString();
        if (Potion* potion = dynamic_cast<Potion*>(item)) {
            std::cout << ", Potion strength: " << potion->getPotionPower() << std::endl;
        } else if (Weapon* weapon = dynamic_cast<Weapon*>(item)) {
            std::cout << ", Weapon attack: " << weapon->getWeaponAttack() << std::endl;
        } else if (Armor* armor = dynamic_cast<Armor*>(item)) {
            std::cout << ", Armor defense: " << armor->getArmorDefense() << std::endl;
        }
    }
}

void Inventory::processKeyInput(int key, int &currentIndex, bool &exitInventory) {
    if (key == 224) {
        key = _getch();
        switch (key) {
            case 75: // Left arrow
                currentIndex = (currentIndex - 1 + 9) % 9;
                break;
            case 77: // Right arrow
                currentIndex = (currentIndex + 1) % 9;
                break;
        }
    } else if (key == 'e' || key == 'E') {
        Map::getInstance().displayCurrentMap();
        exitInventory = true;
    } else if (key == 13 && m_inventoryItems[currentIndex] != nullptr) {
        handleActionSelection(currentIndex);
    }
}

void Inventory::handleActionSelection(int currentIndex) {
    std::cout << "Choose action: [1] Use  [2] Drop  [3] Leave it" << std::endl;
    int choice = 0;
    while (true) {
        char input = _getch();
        if (input == '1' || input == '2' || input == '3') {
            choice = input - '0';
            break;
        } else {
            std::cerr << RED_TEXT << "Invalid input, please re-enter." << RESET_TEXT << std::endl;
        }
    }

    switch (choice) {
        case 1:
            useItem(currentIndex);
            break;
        case 2:
            dropItem(currentIndex);
            break;
        case 3:
            // Leave it
            break;
        default:
            LOG_ERROR("Invalid choice.");
            std::cerr << RED_TEXT << "Invalid choice" << RESET_TEXT << std::endl;
    }
}

void Inventory::useItem(int currentIndex) {
    if (Potion* potion = dynamic_cast<Potion*>(m_inventoryItems[currentIndex])) {
        Inventory::getInstance().removeItem(m_inventoryItems[currentIndex]);
    } else if (Weapon* weapon = dynamic_cast<Weapon*>(m_inventoryItems[currentIndex])) {
        Weapon* current = Map::getInstance().getCurrentLocation()->getPlayer()->getPlayerWeapon();
        Map::getInstance().getCurrentLocation()->getPlayer()->setPlayerWeapon(weapon);
        Inventory::getInstance().removeItem(weapon);
        if (current != nullptr) {
            Inventory::getInstance().addItem(dynamic_cast<Item*>(current));
        }
    } else if (Armor* armor = dynamic_cast<Armor*>(m_inventoryItems[currentIndex])) {
        Armor* current = Map::getInstance().getCurrentLocation()->getPlayer()->getPlayerArmor();
        Map::getInstance().getCurrentLocation()->getPlayer()->setPlayerArmor(armor);
        getInstance().removeItem(armor);
        if (current != nullptr) {
            getInstance().addItem(current);
        }
    }
}

void Inventory::dropItem(int currentIndex) {
    if (m_inventoryItems[currentIndex] != nullptr) {
        Item* itemToDrop = m_inventoryItems[currentIndex];
        if (Map::getInstance().getCurrentLocation()->getPlayer()->getX() == 0 ||  Map::getInstance().getCurrentLocation()->getPlayer()->getY() == 0 || Map::getInstance().getCurrentLocation()->getPlayer()->getX() == Map::getInstance().getCurrentLocation()->getTiles().size() - 1 || Map::getInstance().getCurrentLocation()->getPlayer()->getY() == Map::getInstance().getCurrentLocation()->getTiles()[0].size() - 1) {
            LOG_WARNING("Cannot drop item here.");
            std::cerr << RED_TEXT << "Cannot drop item here" << RESET_TEXT << std::endl;
            return;
        }
        if (Map::getInstance().getCurrentLocation()->setPreviousTile(new Tile(itemToDrop->getItemSymbol(), false, itemToDrop))) {
            getInstance().removeItem(itemToDrop);
        } else {
            LOG_WARNING("Cannot drop item here.");
            std::cerr << RED_TEXT << "Cannot drop item here" << RESET_TEXT << std::endl;
        }
    }
}

void Inventory::saveInventory(std::ofstream& file) {
    LOG_INFO("Saving Inventory instance to file.");
    for (const auto& item : m_inventoryItems) {
        bool hasItem = item != nullptr;
        file.write(reinterpret_cast<const char*>(&hasItem), sizeof(hasItem));
        if (hasItem) {
            item->saveEntity(file);
        }
    }
}

void Inventory::loadInventory(std::ifstream& file) {
    LOG_INFO("Creating Inventory instance from file.");
    destroyInstance();

    Inventory& inventory = getInstance();

    for (size_t i = 0; i < 9; ++i) {
        bool hasItem;
        file.read(reinterpret_cast<char*>(&hasItem), sizeof(hasItem));
        if (hasItem) {
            Item* item = dynamic_cast<Item*>(Entity::createEntityFromFile(file));
            if (item == nullptr) {
                LOG_ERROR("Failed to load item at index: " + std::to_string(i) + ".");
                std::cerr << "Failed to load item at index: " << i << std::endl;
            }
            inventory.m_inventoryItems[i] = item;
        } else {
            inventory.m_inventoryItems[i] = nullptr;
        }
    }
}