#include "stats.h"
#include <algorithm>

Stats::Stats(int hp, int atk, int def, int spd)
    : maxHealth(hp), currentHealth(hp), attack(atk), defense(def), speed(spd) {}

Stats::Stats(const Stats& other)
    : maxHealth(other.maxHealth), currentHealth(other.currentHealth),
      attack(other.attack), defense(other.defense), speed(other.speed) {}

Stats& Stats::operator=(const Stats& other) {
    if (this != &other) {
        maxHealth = other.maxHealth;
        currentHealth = other.currentHealth;
        attack = other.attack;
        defense = other.defense;
        speed = other.speed;
    }
    return *this;
}

Stats Stats::operator+(const Stats& other) const {
    return Stats(
        maxHealth + other.maxHealth,
        attack + other.attack,
        defense + other.defense,
        speed + other.speed
    );
}

void Stats::applyDamage(int dmg) {
    currentHealth = std::max(0, currentHealth - dmg);
}

void Stats::heal(int amount) {
    currentHealth = std::min(maxHealth, currentHealth + amount);
}

std::ostream& operator<<(std::ostream& os, const Stats& s) {
    os << "HP:" << s.currentHealth << "/" << s.maxHealth
       << " ATK:" << s.attack
       << " DEF:" << s.defense
       << " SPD:" << s.speed;
    return os;
}

std::istream& operator>>(std::istream& is, Stats& s) {
    std::cout << "  Max HP: ";  is >> s.maxHealth;
    s.currentHealth = s.maxHealth;
    std::cout << "  Attack:  "; is >> s.attack;
    std::cout << "  Defense: "; is >> s.defense;
    std::cout << "  Speed:   "; is >> s.speed;
    return is;
}

bool operator==(const Stats& a, const Stats& b) {
    return a.maxHealth == b.maxHealth &&
           a.attack    == b.attack    &&
           a.defense   == b.defense   &&
           a.speed     == b.speed;
}