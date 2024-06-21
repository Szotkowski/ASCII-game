//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "MonsterFactory.h"

Monster *MonsterFactory::createMonster(MonsterType monsterType, const std::string& name, int attack, int defense, int hp, MonsterLevel monsterLevel) {
    switch (monsterType) {
        case GOBLIN:
            return new Goblin(name, attack, defense, hp, monsterLevel);
        case TROLL:
            return new Troll(name, attack, defense, hp, monsterLevel);
        case DRAGON:
            return new Dragon(name, attack, defense, hp, monsterLevel);
        default:
            LOG_WARNING("Invalid monster type.");
            std::cerr << RESET_TEXT << "Invalid monster type" << RESET_TEXT << std::endl;
            return nullptr;
    }
}

Monster* MonsterFactory::createMonsterFromData(std::ifstream& file) {
    MonsterType monsterType;
    file.read(reinterpret_cast<char*>(&monsterType), sizeof(monsterType));

    size_t nameSize;
    file.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
    std::string name(nameSize, '\0');
    file.read(&name[0], nameSize);

    int attack, defense, hp;
    file.read(reinterpret_cast<char*>(&attack), sizeof(attack));
    file.read(reinterpret_cast<char*>(&defense), sizeof(defense));
    file.read(reinterpret_cast<char*>(&hp), sizeof(hp));

    MonsterLevel monsterLevel;
    file.read(reinterpret_cast<char*>(&monsterLevel), sizeof(monsterLevel));

    LOG_INFO("Creating monster from data.");
    return createMonster(monsterType, name, attack, defense, hp, monsterLevel);
}