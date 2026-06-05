#pragma once
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/animation_node_state_machine_playback.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include "core/player.h"

namespace godot {

class PlayerNode : public CharacterBody2D {
    GDCLASS(PlayerNode, CharacterBody2D)

private:
    Player*           player;
    float             speed;
    float             rollSpeed;
    AnimationTree*    animTree;
    AnimationPlayer*  blinkAnimPlayer;
    CollisionShape2D* hurtboxShape;
    Ref<AnimationNodeStateMachinePlayback> stateMachine;
    Vector2           lastDir;
    Vector2           inputVector;
    float             iframeTimer;
    float             iframeDuration;
    bool              isInvincible;
    float             contactDamageCooldown;
    float             contactDamageTimer;
    float             deathTimer;
    bool              isDead;

    void moveState();
    void rollState();
    void updateBlendPositions(Vector2 dir);

protected:
    static void _bind_methods();

public:
    PlayerNode();
    ~PlayerNode();

    void _ready()                        override;
    void _physics_process(double delta)  override;

    int     getHP()          const { return player->getStats().getHP(); }
    int     getMaxHP()       const { return player->getStats().getMaxHP(); }
    int     getGold()        const { return player->getGold(); }
    int     getKills()       const { return player->getKills(); }
    Vector2 getInputVector() const { return inputVector; }

    void takeDamage(int amount);
    void healPlayer(int amount);
    void onHurtboxAreaEntered(Area2D* area);
};

} // namespace godot
