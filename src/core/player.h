#pragma once
#include <string>
#include <iostream>
#include "entity.h"
#include "inventory.h"
#include "consumable.h"

class Enemy;
//////////////////////////////////////////////COMP
class Player : public Entity {
private:
    int gold;
    int kills;
    int level;
    Inventory inventory;

public:
    Player(const std::string& name, Stats s);
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player() override;

    void update(float delta) override;
    void takeDamage(int amount) override;
    EntityType getType() const override { return EntityType::PLAYER; }

    // functii publice complexe
    void attackEnemy(Enemy& target) const;
    void pickupItem(Item* item);
    void pickupGold(int amount);
    void levelUp();
    bool simulateCombat(Enemy& target, int rounds);

    int getGold() const { return gold; }
    int getKills() const { return kills; }
    int getLevel() const { return level; }
    int getPotions() const { return inventory.getCount(); }
    void addKill() { ++kills; }
    Stats& getStats() { return stats; }
    const Stats& getStats() const { return stats; }
    Inventory& getInventory() { return inventory; }

    friend std::ostream& operator<<(std::ostream& os, const Player& p);
};
