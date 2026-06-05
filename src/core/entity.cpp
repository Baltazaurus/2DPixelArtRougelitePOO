#include "entity.h"

int Entity::entityCount = 0;

Entity::Entity(int id, const std::string& name, Stats s)
    : id(id), name(name), stats(s) {
    ++entityCount;
}

Entity::Entity(const Entity& other)
    : id(other.id), name(other.name), stats(other.stats) {
    ++entityCount;
}

Entity& Entity::operator=(const Entity& other) {
    if (this != &other) {
        id = other.id;
        name = other.name;
        stats = other.stats;
    }
    return *this;
}

Entity::~Entity() {
    --entityCount;
}

std::ostream& operator<<(std::ostream& os, const Entity& e) {
    os << "[" << e.name << " #" << e.id << "] " << e.stats;
    return os;
}
