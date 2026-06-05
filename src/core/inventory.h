#pragma once
#include <list>
#include <iostream>
#include "item.h"

class Inventory {
private:
    std::list<Item*> items;  ///////////////////////////////////////////////agregare---contine pointer la obiecte   +++ STD LIST
    int capacity;

public:
    explicit Inventory(int cap = 5) : capacity(cap) {}
    Inventory(const Inventory& other) : items(other.items), capacity(other.capacity) {}
    Inventory& operator=(const Inventory& other) {
        if (this != &other) { items = other.items; capacity = other.capacity; }
        return *this;
    }
    ~Inventory() = default;

    bool addItem(Item* item) {
        if ((int)items.size() >= capacity) return false;
        items.push_back(item);
        return true;
    }

    Item* popFirst() {
        if (items.empty()) return nullptr;
        Item* item = items.front();
        items.pop_front();
        return item;
    }

    bool isEmpty() const { return items.empty(); }
    int getCount() const { return (int)items.size(); }

    void printAll() const {
        for (const auto* i : items)
            std::cout << "  " << *i << " - " << i->describe() << "\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const Inventory& inv) {
        return os << "Inventory(" << inv.items.size() << "/" << inv.capacity << ")";
    }
};
