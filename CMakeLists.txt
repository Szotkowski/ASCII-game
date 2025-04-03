cmake_minimum_required(VERSION 3.28)
project(ASCII_Game)

set(CMAKE_CXX_STANDARD 26)

#set(CMAKE_BUILD_TYPE Release)

#set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(ASCII_Game main.cpp
        GameEngine.cpp
        GameEngine.h
        Menu.cpp
        Menu.h
        inventory/Inventory.cpp
        inventory/Inventory.h
        item/Armor.cpp
        item/Armor.h
        item/Item.cpp
        item/Item.h
        item/ItemFactory.cpp
        item/ItemFactory.h
        item/Potion.cpp
        item/Potion.h
        item/Weapon.cpp
        item/Weapon.h
        map/DefaultLocationBuilder.cpp
        map/DefaultLocationBuilder.h
        map/Location.cpp
        map/Location.h
        map/LocationBuilder.cpp
        map/LocationBuilder.h
        map/LocationDirector.cpp
        map/LocationDirector.h
        map/Map.cpp
        map/Map.h
        map/Tile.cpp
        map/Tile.h
        monster/Dragon.cpp
        monster/Dragon.h
        monster/Goblin.cpp
        monster/Goblin.h
        monster/Monster.cpp
        monster/Monster.h
        monster/MonsterFactory.cpp
        monster/MonsterFactory.h
        monster/Troll.cpp
        monster/Troll.h
        player/Archer.cpp
        player/Archer.h
        player/Mage.cpp
        player/Mage.h
        player/Player.cpp
        player/Player.h
        player/Warrior.cpp
        player/Warrior.h
        entity/Entity.cpp
        entity/Entity.h
        gold/Gold.cpp
        gold/Gold.h
        logger/Logger.cpp
        logger/Logger.h
        Menu.cpp
)