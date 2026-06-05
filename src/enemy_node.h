#pragma once
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/core/class_db.hpp>
#include "core/enemy.h"

namespace godot {

enum class EnemyState { IDLE, WANDER, CHASE, GATHER };

class EnemyNode : public CharacterBody2D {
    GDCLASS(EnemyNode, CharacterBody2D)

private:
    Enemy*            enemy;
    float             chaseSpeed;
    float             wanderSpeed;
    float             attackRange;
    float             detectionRange;
    float             loseRange;
    float             maxWanderDist;
    float             attackCooldown;
    float             cooldownTimer;
    float             stateTimer;
    float             gatherTimer;
    float             gatherDuration;
    float             knockbackTimer;
    float             knockbackDuration;
    float             knockbackSpeed;
    float             iframeTimer;
    float             iframeDuration;
    bool              isInvincible;
    bool              isAttacking;
    Vector2           knockbackDir;
    Vector2           wanderDir;
    Vector2           lastDir;
    Vector2           homePosition;
    String            lastAnimSuffix;
    EnemyState        state;
    AnimationPlayer*  animPlayer;
    AnimationPlayer*  blinkAnimPlayer;
    CollisionShape2D* hurtboxShape;
    Sprite2D*         sprite;
    Node2D*           playerRef;
    Ref<PackedScene>  potionScene;

    String getDirectionSuffix(Vector2 dir);
    void   playAnim(const String& anim);
    float  randRange(float min, float max);

protected:
    static void _bind_methods();

public:
    EnemyNode();
    ~EnemyNode();

    void _ready()                        override;
    void _physics_process(double delta)  override;
    void onAnimationFinished(StringName anim_name);
    void onHurtboxAreaEntered(Area2D* area);

    void takeDamage(int amount);

    void setPotionScene(const Ref<PackedScene>& scene) { potionScene = scene; }
    Ref<PackedScene> getPotionScene() const { return potionScene; }

    void  setChaseSpeed(float s)  { chaseSpeed = s; }
    float getChaseSpeed()   const { return chaseSpeed; }
    void  setAttackRange(float r) { attackRange = r; }
    float getAttackRange()  const { return attackRange; }
};

} // namespace godot
