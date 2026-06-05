#include "player.h"
#include "enemy.h"
#include <iostream>
#include <algorithm>
/////////////////////////////////////////////////////////////////////lista de inititalizare cu const parintelui
Player::Player(const std::string& name, Stats s)
    : Entity(Entity::entityCount + 1, name, s),
      gold(0), kills(0), level(1), inventory(5) {}

Player::Player(const Player& other)
    : Entity(other), gold(other.gold), kills(other.kills),
      level(other.level), inventory(other.inventory) {}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        Entity::operator=(other);
        gold = other.gold;
        kills = other.kills;
        level = other.level;
        inventory = other.inventory;
    }
    return *this;
}

Player::~Player() {}

void Player::update(float delta) { (void)delta; }
////////////////////////////////////////////////////////////////////////////override pe virtuale
void Player::takeDamage(int amount) {
    stats.applyDamage(amount);
}

void Player::attackEnemy(Enemy& target) const {
    target.takeDamage(stats.getAttack());
}
//////////////////////////////////////////////////////////////////////////////////////////////DOWNCAST
void Player::pickupItem(Item* item) {
    if (!item) return;
    Consumable* potion = dynamic_cast<Consumable*>(item);
    if (potion) {
        potion->use(*this);
        delete potion;
    } else {
        inventory.addItem(item);
    }
}

void Player::pickupGold(int amount) {
    if (amount <= 0) return;
    gold += amount;
}
                                                                   // <-------------------------------functii publice complexe
void Player::levelUp() {
    ++level;
    // la fiecare level cresc statsurile
    Stats bonus(20, 5, 1, 1);
    stats = stats + bonus;
    // heal la max
    stats.heal(stats.getMaxHP());
    std::cout << name << " a ajuns la nivelul " << level << "!\n";
    std::cout << "Stats noi: " << stats << "\n";
}

bool Player::simulateCombat(Enemy& target, int rounds) {
    std::cout << "=== Simulare combat: " << name
              << " vs " << target.getName() << " ===\n";

    for (int i = 0; i < rounds; i++) {
        if (!isAlive() || !target.isAlive()) break;

        target.takeDamage(stats.getAttack());
        std::cout << "Round " << (i+1) << ": " << name
                  << " ataca pentru " << stats.getAttack()
                  << " damage. Enemy HP: " << target.getStats().getHP() << "\n";

        if (!target.isAlive()) {
            std::cout << target.getName() << " a fost invins!\n";
            ++kills;
            return true;
        }

        int enemyDmg = target.getDamage();
        takeDamage(enemyDmg);
        std::cout << "Round " << (i+1) << ": " << target.getName()
                  << " ataca pentru " << enemyDmg
                  << " damage. Player HP: " << stats.getHP() << "\n";

        if (!isAlive()) {
            std::cout << name << " a fost invins!\n";
            return false;
        }
    }

    std::cout << "Combat terminat dupa " << rounds << " runde. Niciun invingator.\n";
    return false;
}

std::ostream& operator<<(std::ostream& os, const Player& p) {
    os << static_cast<const Entity&>(p)
       << " | Level:" << p.level
       << " Gold:" << p.gold
       << " Kills:" << p.kills
       << " Potions:" << p.inventory.getCount();
    return os;
}
