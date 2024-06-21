//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Entity.h"
#include "../GameEngine.h"

Entity* Entity::createEntityFromFile(std::ifstream &file) {
    int entityType;
    file.read(reinterpret_cast<char*>(&entityType), sizeof(entityType));

    switch (entityType) {
        case PLAYER:
            return Player::loadEntity(file);
        case MONSTER:
            return MonsterFactory::createMonsterFromData(file);
        case GOLD:
            return Gold::loadEntity(file);
        case ITEM:
            return ItemFactory::createItemFromData(file);
        default:
            LOG_ERROR("Invalid entity type.");
            std::cerr << RED_TEXT << "Invalid entity type: " << entityType << RESET_TEXT << std::endl;
            return nullptr;
    }
}