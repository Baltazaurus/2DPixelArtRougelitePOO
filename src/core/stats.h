#pragma once
#include <iostream>
#include "types.h"

class Stats {
private:
    int maxHealth;
    int currentHealth;
    int attack;
    int defense;
    int speed;

public:
    Stats(int hp, int atk, int def, int spd);
    Stats() : maxHealth(1), currentHealth(1), attack(1), defense(1), speed(1) {}
    Stats(const Stats& other);
    Stats& operator=(const Stats& other);
    ~Stats() = default;


    Stats operator+(const Stats& other) const;


    friend std::ostream& operator<<(std::ostream& os, const Stats& s);
    friend std::istream& operator>>(std::istream& is, Stats& s);
    friend bool operator==(const Stats& a, const Stats& b);

    bool isAlive() const { return currentHealth > 0; }
    int getHP() const { return currentHealth; }
    int getMaxHP() const { return maxHealth; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    int getSpeed() const { return speed; }

    void applyDamage(int dmg);
    void heal(int amount);
};
