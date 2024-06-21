//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Tile.h"

Tile::Tile(char symbol, bool passable, Entity *entity) : m_symbol(symbol), m_passable(passable) {
    if (entity != nullptr) {
        m_entity = std::unique_ptr<Entity>(entity);
    }
}

char Tile::getSymbol() {
    return m_symbol;
}

bool Tile::isPassable() {
    return m_passable;
}

Entity* Tile::getEntity() {
    return m_entity.get();
}

void Tile::saveTile(std::ofstream& file) {
    file.write(reinterpret_cast<const char*>(&m_symbol), sizeof(m_symbol));
    file.write(reinterpret_cast<const char*>(&m_passable), sizeof(m_passable));

    bool hasEntity = (m_entity != nullptr);
    file.write(reinterpret_cast<const char*>(&hasEntity), sizeof(hasEntity));
    if (hasEntity) {
        m_entity->saveEntity(file);
    }
}

Tile* Tile::loadTile(std::ifstream& file) {
    char symbol;
    bool passable;
    file.read(reinterpret_cast<char*>(&symbol), sizeof(symbol));
    file.read(reinterpret_cast<char*>(&passable), sizeof(passable));

    Entity* entity = nullptr;
    bool hasEntity;
    file.read(reinterpret_cast<char*>(&hasEntity), sizeof(hasEntity));
    if (hasEntity) {
        entity = Entity::createEntityFromFile(file);
    }

    return new Tile(symbol, passable, entity);
}