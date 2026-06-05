#pragma once
#include "item.h"
#include <string>

class Consumable : public Item {
private:
    int healAmount;

public:
    Consumable(const std::string& n, int heal)
        : Item(n, ItemType::CONSUMABLE), healAmount(heal) {}

    Consumable(const Consumable& other)
        : Item(other), healAmount(other.healAmount) {}

    Consumable& operator=(const Consumable& other) {
        if (this != &other) {
            Item::operator=(other);
            healAmount = other.healAmount;
        }
        return *this;
    }

    ~Consumable() override = default;

    void use(class Player& p) override;

    std::string describe() const override {
        return "Heals " + std::to_string(healAmount) + " HP";
    }

    int getHealAmount() const { return healAmount; }
};
