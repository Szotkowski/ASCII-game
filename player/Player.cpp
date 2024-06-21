//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Player.h"
#include "Warrior.h"
#include "Mage.h"
#include "Archer.h"
#include "../Menu.h"

Player::Player() {
    m_playerSymbol = 'P';
    m_playerHP = 100;
    m_playerAttack = 0;
    m_playerDefense = 0;
    m_playerGold = 10;
}

Player::~Player() {
    LOG_INFO("Destroying Player entity.");
    if (m_playerArmor != nullptr)
        delete m_playerArmor;
    if (m_playerWeapon != nullptr)
        delete m_playerWeapon;
}

Player* Player::createPlayer() {
    std::cout << "Choose action: [1] Warrior  [2] Mage  [3] Archer" << std::endl;
    int choice = 0;
    while (true) {
        char input = _getch();
        if (input >= '1' && input <= '3') {
            choice = input - '0'; // Convert character to integer
            break;
        }
        std::cout << "Invalid input, please re-enter." << std::endl;
    }
    switch (choice) {
        case 1:
            return new Warrior();
        case 2:
            return new Mage();
        case 3:
            return new Archer();
        default:
            return nullptr;
    }
}

char Player::getPlayerSymbol()  {
    return m_playerSymbol;
}

std::string Player::getPlayerName() {
    return m_playerName;
}

void Player::hitPlayer(int hp) {
    if (hp >= 0 && m_playerHP <= 100 && m_playerHP > 0) {
        LOG_INFO("Player hit for " + std::to_string(hp) + " HP.");
        m_playerHP -= hp;
        if (m_playerHP <= 0) {
            LOG_INFO("Player died! F");
            m_playerHP = 0;
            std::cout << "You died! F" << std::endl;
            Map::getInstance().getCurrentLocation()->setTile(Map::getInstance().getCurrentLocation()->getPlayer()->getX(), Map::getInstance().getCurrentLocation()->getPlayer()->getY(), new Tile(' ', true));
            delete Map::getInstance().getCurrentLocation()->getPlayer();
            GameEngine::getInstance().saveGame();
            GameEngine::getInstance().deathScreen();
        }
    } else {
        LOG_WARNING("Negative amount or invalid HP for hitPlayer.");
        std::cerr << RED_TEXT << "Negative amount or invalid HP for hitPlayer." << RESET_TEXT << std::endl;
    }
}

void Player::healPlayer(int hp) {
    if (hp >= 0 && m_playerHP <= 100 && m_playerHP > 0) {
        LOG_INFO("Player healed for " + std::to_string(hp) + " HP.");
        m_playerHP += hp;
        if (m_playerHP > 100) {
            m_playerHP = 100;
        }
    } else {
        LOG_WARNING("Negative amount or invalid HP for healPlayer.");
        std::cerr << RED_TEXT << "Negative amount or invalid HP for healPlayer." << RESET_TEXT << std::endl;
    }
}

int Player::getPlayerHP() {
    return m_playerHP;
}

int Player::getPlayerAttack() {
    return m_playerAttack;
}

int Player::getPlayerDefense() {
    return m_playerDefense;
}

void Player::addGold(int amount) {
    if (amount >= 0) {
        LOG_INFO("Gold added: " + std::to_string(amount) + ".");
        m_playerGold += amount;
    } else {
        LOG_WARNING("Negative amount for addGold.");
        std::cerr << RED_TEXT << "Negative amount for addGold." << RESET_TEXT << std::endl;
    }
}

void Player::removeGold(int amount) {
    if (amount >= 0 && m_playerGold >= amount) {
        LOG_INFO("Gold removed: " + std::to_string(amount) + ".");
        m_playerGold -= amount;
    } else {
        LOG_WARNING("Insufficient gold or negative amount for removeGold.");
        std::cerr << RED_TEXT << "Insufficient gold or negative amount for removeGold" << RESET_TEXT << std::endl;
    }
}

int Player::getPlayerGold() {
    return m_playerGold;
}

bool Player::movePlayer(char direction) {
    Location* location = Map::getInstance().getCurrentLocation();
    int targetX = m_x;
    int targetY = m_y;
    int maxRows = location->getTiles().size();
    int maxCols = location->getTiles()[0].size();
    switch(direction) {
        case 'w':
            if (targetX > 0)
                targetX--;
            break;
        case 's':
            if (targetX < maxRows - 1)
                targetX++;
            break;
        case 'a':
            if (targetY > 0)
                targetY--;
            break;
        case 'd':
            if (targetY < maxCols - 1)
                targetY++;
            break;
        default:
            return false;
    }
    if (targetY >= 0 && targetY < maxCols && targetX >= 0 && targetX < maxRows && location->move(m_x, m_y, targetX, targetY, new Tile(m_playerSymbol, false, this))) {
        m_x = targetX;
        m_y = targetY;
        return true;
    }
    return false;
}

int Player::getX() {
    return m_x;
}

int Player::getY() {
    return m_y;
}

void Player::setXY(std::pair<int, int> xy) {
    m_x = xy.first;
    m_y = xy.second;
}

PlayerType Player::getPlayerType() {
    return m_playerType;
}

std::string Player::getPlayerTypeString() {
    switch (m_playerType) {
        case WARRIOR:
            return "Warrior";
        case MAGE:
            return "Mage";
        case ARCHER:
            return "Archer";
        default:
            return "Unknown";
    }
}

void Player::setPlayerArmor(Armor *armor) {
    if (armor != nullptr) {
        m_playerArmor = armor;
        m_playerDefense = armor->getArmorDefense();
    } else {
        LOG_WARNING("Null armor for setPlayerArmor.");
        std::cerr << RED_TEXT << "Null armor for setPlayerArmor." << RESET_TEXT << std::endl;
    }
}

void Player::removePlayerArmor() {
    m_playerArmor = nullptr;
    m_playerDefense = 0;
}

Armor *Player::getPlayerArmor() {
    return m_playerArmor;
}

void Player::setPlayerWeapon(Weapon *weapon) {
    if (weapon != nullptr) {
        m_playerWeapon = weapon;
        m_playerAttack = weapon->getWeaponAttack();
    } else {
        LOG_WARNING("Null weapon for setPlayerWeapon.");
        std::cerr << RED_TEXT << "Null weapon for setPlayerWeapon." << RESET_TEXT << std::endl;
    }
}

void Player::removePlayerWeapon() {
    m_playerWeapon = nullptr;
    m_playerAttack = 0;
}

Weapon *Player::getPlayerWeapon() {
    return m_playerWeapon;
}

Entity* Player::loadEntity(std::ifstream& file) {
    int playerTypeInt;
    file.read(reinterpret_cast<char*>(&playerTypeInt), sizeof(playerTypeInt));
    PlayerType playerType = static_cast<PlayerType>(playerTypeInt);

    // Create the appropriate player subclass instance based on the player type
    switch (playerType) {
    case WARRIOR:
        return Warrior::loadWarrior(file);
    case MAGE:
        return Mage::loadMage(file);
    case ARCHER:
        return Archer::loadArcher(file);
    default:
        LOG_WARNING("Unknown player type: " + std::to_string(playerTypeInt));
        std::cerr << RED_TEXT << "Unknown player type: " << playerTypeInt << RESET_TEXT << std::endl;
        return nullptr;
    }
}