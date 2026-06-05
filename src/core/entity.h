#pragma once
#include <iostream>
#include <string>
#include "types.h"
#include "stats.h"

class Entity {
protected:
    int id;
    std::string name;
    Stats stats;////////////////////////////////////////////////////////////////comp
    static int entityCount;

public:
    Entity(int id, const std::string& name, Stats s);
    Entity(const Entity& other);
    Entity& operator=(const Entity& other);
    virtual ~Entity();
    /////////////////////////////////////////////////////////////////////////////////////////////////////virtual
    virtual void update(float delta) = 0;
    virtual void takeDamage(int amount) = 0;
    virtual EntityType getType() const = 0;

    int getId() const { return id; }
    const std::string& getName() const { return name; }
    const Stats& getStats() const { return stats; }
    Stats& getStats() { return stats; }
    bool isAlive() const { return stats.isAlive(); }

    static int getEntityCount() { return entityCount; }

    friend std::ostream& operator<<(std::ostream& os, const Entity& e);
};
