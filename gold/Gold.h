//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef GOLD_H
#define GOLD_H

#include "../entity/Entity.h"
#include <stdexcept>

class Gold : public Entity {
private:
    char m_goldSymbol;

    int m_goldAmount;

public:
    Gold(int goldAmount);

    int getGoldAmount();

    char getGoldSymbol();

    void saveEntity(std::ofstream& file) override;

    static Gold *loadEntity(std::ifstream& file);
};

#endif //GOLD_H
