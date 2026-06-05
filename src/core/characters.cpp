#include "player.h"
#include "enemy.h"
#include <iostream>

//////////////////////////////////////////////////ENEMY

int Enemy::spawnedCount = 0;

Enemy::Enemy(int id, const std::string& name, Stats s, DamageType dt, int dmg)
    : Entity(id, name, s), dmgType(dt), damage(dmg) {
    ++spawnedCount;
}

Enemy::Enemy(const Enemy& other)
    : Entity(other), dmgType(other.dmgType), damage(other.damage) {
    ++spawnedCount;
}

Enemy& Enemy::operator=(const Enemy& other) {
    if (this != &other) {
        Entity::operator=(other);
        dmgType = other.dmgType;
        damage = other.damage;
    }
    return *this;
}

Enemy::~Enemy() { --spawnedCount; }

void Enemy::update(float delta) { (void)delta; }

void Enemy::takeDamage(int amount) {
    stats.applyDamage(amount);
}

void Enemy::attackTarget(Entity& target) const {
    target.takeDamage(damage);
}

std::ostream& operator<<(std::ostream& os, const Enemy& e) {
    os << static_cast<const Entity&>(e);
    return os;
}