//
// Created by Michael Szotkowski on 6/17/2024.
//

#ifndef NEW_PROJEKT_ZOO_LOCATIONDIRECTOR_H
#define NEW_PROJEKT_ZOO_LOCATIONDIRECTOR_H

#include "LocationBuilder.h"
#include "DefaultLocationBuilder.h"

class LocationDirector {
private:
    LocationBuilder* m_builder;

public:
    LocationDirector(LocationBuilder *builder = new DefaultLocationBuilder());

    ~LocationDirector();

    void setBuilder(LocationBuilder *builder);

    Location *buildLocation(Side doorSide = Random, bool firstLocation = false);
};

#endif //NEW_PROJEKT_ZOO_LOCATIONDIRECTOR_H
