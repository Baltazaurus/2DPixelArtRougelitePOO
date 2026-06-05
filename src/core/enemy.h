#pragma once
#include "entity.h"

class Enemy : public Entity {
protected:
    DamageType dmgType;
    int damage;
    static int spawnedCount;          //////////////////////////static

public:
    Enemy(int id, const std::string& name, Stats s, DamageType dt, int dmg);
    Enemy(const Enemy& other);
    Enemy& operator=(const Enemy& other);
    ~Enemy() override;

    void update(float delta) override;
    void takeDamage(int amount) override;
    EntityType getType() const override { return EntityType::ENEMY; }

    int getDamage() const { return damage; }
    void attackTarget(Entity& target) const;

    static int getSpawnedCount() { return spawnedCount; }

    friend std::ostream& operator<<(std::ostream& os, const Enemy& e);
};
