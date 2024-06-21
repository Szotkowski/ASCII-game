//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "LocationDirector.h"

LocationDirector::LocationDirector(LocationBuilder* builder) {
    LOG_INFO("Creating LocationDirector instance.");
    m_builder = builder;
}

LocationDirector::~LocationDirector() {
    LOG_INFO("Destroying LocationDirector instance.");
}

void LocationDirector::setBuilder(LocationBuilder* builder) {
    m_builder = builder;
}

Location* LocationDirector::buildLocation(Side doorSide, bool firstLocation) {
    m_builder->emptyLayout();
    m_builder->setDoors(doorSide);
    m_builder->setGold();
    m_builder->setItems();
    m_builder->setMonsters();
    if (firstLocation) {
        m_builder->setPlayer();
    }
    return new Location(*m_builder->getLocation());
}