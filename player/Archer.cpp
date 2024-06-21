//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Archer.h"

void Archer::saveEntity(std::ofstream& file) {
    LOG_INFO("Saving Archer entity to file");
    EntityType entityType = PLAYER;
    file.write(reinterpret_cast<const char*>(&entityType), sizeof(entityType));

    // Save the monster type as well
    PlayerType playerType = ARCHER;
    file.write(reinterpret_cast<const char*>(&playerType), sizeof(playerType));

    size_t nameLength = m_playerName.size();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(m_playerName.c_str(), nameLength);

    file.write(reinterpret_cast<const char*>(&m_playerHP), sizeof(m_playerHP));
    file.write(reinterpret_cast<const char*>(&m_playerAttack), sizeof(m_playerAttack));
    file.write(reinterpret_cast<const char*>(&m_playerDefense), sizeof(m_playerDefense));
    file.write(reinterpret_cast<const char*>(&m_playerGold), sizeof(m_playerGold));
    file.write(reinterpret_cast<const char*>(&m_x), sizeof(m_x));
    file.write(reinterpret_cast<const char*>(&m_y), sizeof(m_y));

    // Serialize Armor and Weapon objects
    bool hasArmor = (m_playerArmor != nullptr);
    file.write(reinterpret_cast<const char*>(&hasArmor), sizeof(hasArmor));
    if (hasArmor) {
        m_playerArmor->saveEntity(file);
    }

    bool hasWeapon = (m_playerWeapon != nullptr);
    file.write(reinterpret_cast<const char*>(&hasWeapon), sizeof(hasWeapon));
    if (hasWeapon) {
        m_playerWeapon->saveEntity(file);
    }
}

Archer::Archer(std::string& name, int hp, int attack, int defense, int gold, int x, int y, Armor* armor, Weapon* weapon) {
    m_playerName = name;
    m_playerType = ARCHER;
    m_playerHP = hp;
    m_playerAttack = attack;
    m_playerDefense = defense;
    m_playerGold = gold;
    m_x = x;
    m_y = y;
    m_playerArmor = armor;
    m_playerWeapon = weapon;
}

Archer::Archer() {
    LOG_INFO("Creating Archer entity.");
    m_playerName = "Archer";
    m_playerType = ARCHER;
    setPlayerArmor(dynamic_cast<Armor *>(ItemFactory::createItem(ARMOR, "Default", 10, COMMON, 15)));
    setPlayerWeapon(dynamic_cast<Weapon *>(ItemFactory::createItem(WEAPON, "Default", 15, UNCOMMON, 15, BOW)));
}

Archer* Archer::loadArcher(std::ifstream& file) {
    LOG_INFO("Loading Archer entity from file.");
    if (!file.is_open()) {
        std::cerr << RED_TEXT << "Could not open file for reading." << RESET_TEXT << std::endl;
        LOG_ERROR("Could not open file for reading.");
        return nullptr;
    }

    size_t nameLength;
    std::string name;
    int hp;
    int attack;
    int defense;
    int gold;
    int x;
    int y;

    // Deserialize the name length and name
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    name.resize(nameLength);
    file.read(&name[0], nameLength);

    file.read(reinterpret_cast<char*>(&hp), sizeof(hp));
    file.read(reinterpret_cast<char*>(&attack), sizeof(attack));
    file.read(reinterpret_cast<char*>(&defense), sizeof(defense));
    file.read(reinterpret_cast<char*>(&gold), sizeof(gold));
    file.read(reinterpret_cast<char*>(&x), sizeof(x));
    file.read(reinterpret_cast<char*>(&y), sizeof(y));

    // Deserialize Armor and Weapon objects
    Armor* armor = nullptr;
    bool hasArmor;
    file.read(reinterpret_cast<char*>(&hasArmor), sizeof(hasArmor));
    if (hasArmor) {
        armor = dynamic_cast<Armor*>(createEntityFromFile(file));
    }

    Weapon* weapon = nullptr;
    bool hasWeapon;
    file.read(reinterpret_cast<char*>(&hasWeapon), sizeof(hasWeapon));
    if (hasWeapon) {
        weapon = dynamic_cast<Weapon*>(createEntityFromFile(file));
    }

    LOG_INFO("Archer entity loaded successfully.");
    return new Archer(name, hp, attack, defense, gold, x, y, armor, weapon);
}