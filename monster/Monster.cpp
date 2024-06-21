//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Monster.h"

Monster::Monster(std::string name, int attack, int defense, int hp, MonsterLevel monsterLevel) {
    LOG_INFO("Creating Monster entity.");
    m_monsterSymbol = 'M';
    m_monsterName = name;
    m_monsterAttack = attack * (int)monsterLevel;
    m_monsterDefense = defense * (int)monsterLevel;
    m_monsterHP = hp * (int)monsterLevel;
    m_monsterLevel = monsterLevel;
}

char Monster::getMonsterSymbol() {
    return m_monsterSymbol;
}

std::string Monster::getMonsterName() {
    return m_monsterName;
}

int Monster::getMonsterAttack() {
    return m_monsterAttack;
}

int Monster::getMonsterDefense() {
    return m_monsterDefense;
}

int Monster::getMonsterHP() {
    return m_monsterHP;
}

MonsterLevel Monster::getMonsterLevel() {
    return m_monsterLevel;
}

std::string Monster::getMonsterLevelString() {
    switch (m_monsterLevel) {
        case MonsterLevel::M_EASY:
            return "Easy";
        case MonsterLevel::M_MEDIUM:
            return "Medium";
        case MonsterLevel::M_HARD:
            return "Hard";
        default:
            return "Unknown";
    }
}

void Monster::hitMonster(int hp) {
    if (hp >= 0 && m_monsterHP > 0) {
        LOG_INFO("Monster hit by " + std::to_string(hp) + " HP.");
        m_monsterHP -= hp;
        if (m_monsterHP <= 0) {
            LOG_INFO("Monster has been defeated.");
            m_monsterHP = 0;
        }
    } else {
        LOG_WARNING("Negative amount or invalid HP for hitMonster.");
        std::cerr << RED_TEXT << "Negative amount or invalid HP for hitMonster." << RESET_TEXT << std::endl;
    }
}