//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Troll.h"

void Troll::saveEntity(std::ofstream &file) {
    LOG_INFO("Saving Troll entity to file");
    EntityType entityType = MONSTER;
    file.write(reinterpret_cast<const char*>(&entityType), sizeof(entityType));

    MonsterType monsterType = TROLL;
    file.write(reinterpret_cast<const char*>(&monsterType), sizeof(monsterType));

    size_t nameLength = m_monsterName.size();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(m_monsterName.c_str(), nameLength);

    file.write(reinterpret_cast<const char*>(&m_monsterAttack), sizeof(m_monsterAttack));
    file.write(reinterpret_cast<const char*>(&m_monsterDefense), sizeof(m_monsterDefense));
    file.write(reinterpret_cast<const char*>(&m_monsterHP), sizeof(m_monsterHP));

    int level = m_monsterLevel;
    file.write(reinterpret_cast<const char*>(&level), sizeof(level));
}

Troll::Troll(std::string name, int attack, int defense, int hp, MonsterLevel monsterLevel) : Monster(std::move(name), attack, defense, hp, monsterLevel) {
    m_monsterName = "Troll: " + name;
}