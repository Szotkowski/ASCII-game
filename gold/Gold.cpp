//
// Created by Michael Szotkowski on 6/17/2024.
//

#include "Gold.h"

Gold::Gold(int goldAmount) {
    LOG_INFO("Creating Gold object.");
    m_goldSymbol = 'G';
    m_goldAmount = goldAmount;
}

int Gold::getGoldAmount() {
    return m_goldAmount;
}

char Gold::getGoldSymbol() {
    return m_goldSymbol;
}

void Gold::saveEntity(std::ofstream& file) {
    LOG_INFO("Saving Gold entity to file.");
    int entityType = GOLD;
    file.write(reinterpret_cast<const char*>(&entityType), sizeof(entityType));
    file.write((char*)&m_goldAmount, sizeof(m_goldAmount));
}

Gold* Gold::loadEntity(std::ifstream& file) {
    int goldAmount;
    file.read((char*)&goldAmount, sizeof(goldAmount));

    LOG_INFO("Creating Gold entity from file.");
    return new Gold(goldAmount);
}