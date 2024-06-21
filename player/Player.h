//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_PLAYER_H
#define NEW_PROJEKT_ZOO_PLAYER_H

#include "../item/Armor.h"
#include "../item/Weapon.h"

enum PlayerType {
    WARRIOR,
    MAGE,
    ARCHER
};

class Player : public Entity {
protected:
    char m_playerSymbol;

    std::string m_playerName;

    int m_playerHP;

    int m_playerAttack;

    int m_playerDefense;

    int m_playerGold;

    int m_x;

    int m_y;

    PlayerType m_playerType;

    Armor *m_playerArmor = nullptr;

    Weapon *m_playerWeapon = nullptr;

    Player();

public:
    ~Player();

    static Player *createPlayer();

    char getPlayerSymbol();

    std::string getPlayerName();

    void hitPlayer(int hp);

    void healPlayer(int hp);

    int getPlayerHP();

    int getPlayerAttack();

    int getPlayerDefense();

    void addGold(int amount);

    void removeGold(int amount);

    int getPlayerGold();

    bool movePlayer(char direction);

    int getX();

    int getY();

    void setXY(std::pair<int, int> xy);

    PlayerType getPlayerType();

    std::string getPlayerTypeString();

    void setPlayerArmor(Armor *armor);

    void removePlayerArmor();

    Armor *getPlayerArmor();

    void setPlayerWeapon(Weapon *weapon);

    void removePlayerWeapon();

    Weapon *getPlayerWeapon();

    void saveEntity(std::ofstream &file) override = 0;

    static Entity *loadEntity(std::ifstream &file);
};

#endif //NEW_PROJEKT_ZOO_PLAYER_H
