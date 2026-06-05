#pragma once
#include <string>
#include <iostream>
#include "types.h"

class Item {
protected:
    std::string name;
    ItemType type;

public:
    Item(const std::string& n, ItemType t) : name(n), type(t) {}
    Item(const Item& other) : name(other.name), type(other.type) {}
    Item& operator=(const Item& other) {
        if (this != &other) { name = other.name; type = other.type; }
        return *this;
    }
    virtual ~Item() = default;
///////////////////////////////////////////////////////////////////////virtual 
    virtual void use(class Player& p) = 0;
    virtual std::string describe() const = 0;

    const std::string& getName() const { return name; }
    ItemType getType() const { return type; }

    friend std::ostream& operator<<(std::ostream& os, const Item& i) {
        return os << "[" << i.name << "]";
    }
};
