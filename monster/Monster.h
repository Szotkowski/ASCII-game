//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_MONSTER_H
#define NEW_PROJEKT_ZOO_MONSTER_H

#include "../entity/Entity.h"

enum MonsterLevel {
    M_EASY = 1,
    M_MEDIUM = 2,
    M_HARD = 3
};

class Monster : public Entity {
protected:
    char m_monsterSymbol;

    std::string m_monsterName;

    int m_monsterAttack;

    int m_monsterDefense;

    int m_monsterHP;

    MonsterLevel m_monsterLevel;

public:
    Monster(std::string name, int attack, int defense, int hp, MonsterLevel monsterLevel);

    char getMonsterSymbol();

    std::string getMonsterName();

    int getMonsterAttack();

    int getMonsterDefense();

    int getMonsterHP();

    MonsterLevel getMonsterLevel();

    std::string getMonsterLevelString();

    void hitMonster(int hp);

    void saveEntity(std::ofstream &file) override = 0;
};

#endif //NEW_PROJEKT_ZOO_MONSTER_H
