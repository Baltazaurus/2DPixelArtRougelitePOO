#include "player_node.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>

using namespace godot;

void PlayerNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_hp"),           &PlayerNode::getHP);
    ClassDB::bind_method(D_METHOD("get_max_hp"),       &PlayerNode::getMaxHP);
    ClassDB::bind_method(D_METHOD("get_gold"),         &PlayerNode::getGold);
    ClassDB::bind_method(D_METHOD("get_kills"),        &PlayerNode::getKills);
    ClassDB::bind_method(D_METHOD("get_input_vector"), &PlayerNode::getInputVector);
    ClassDB::bind_method(D_METHOD("take_damage", "amount"), &PlayerNode::takeDamage);
    ClassDB::bind_method(D_METHOD("heal_player", "amount"), &PlayerNode::healPlayer);
    ClassDB::bind_method(D_METHOD("on_hurtbox_area_entered", "area"), &PlayerNode::onHurtboxAreaEntered);
}

PlayerNode::PlayerNode()
    : player(nullptr), speed(100.0f), rollSpeed(125.0f),
      animTree(nullptr), blinkAnimPlayer(nullptr), hurtboxShape(nullptr),
      lastDir(Vector2(0, -1)), inputVector(Vector2(0, 0)),
      iframeTimer(0.0f), iframeDuration(0.8f), isInvincible(false),
      contactDamageCooldown(1.0f), contactDamageTimer(0.0f),
      deathTimer(0.5f), isDead(false) {
    Stats s(100, 12, 4, 8);
    player = new Player("Hero", s);
}

PlayerNode::~PlayerNode() {
    delete player;
}

void PlayerNode::updateBlendPositions(Vector2 dir) {
    if (!animTree) return;
    Vector2 d = Vector2(dir.x, -dir.y);
    animTree->set("parameters/StateMachine/MoveState/RunState/blend_position", d);
    animTree->set("parameters/StateMachine/MoveState/StandState/blend_position", d);
    animTree->set("parameters/StateMachine/AttackState/blend_position", d);
    animTree->set("parameters/StateMachine/RollState/blend_position", d);
}

void PlayerNode::_ready() {
    if (has_node("AnimationTree")) {
        animTree = get_node<AnimationTree>("AnimationTree");
        Variant pb = animTree->get("parameters/StateMachine/playback");
        stateMachine = pb;
        if (!stateMachine.is_valid()) {
            pb = animTree->get("parameters/playback");
            stateMachine = pb;
        }
        UtilityFunctions::print("StateMachine valid: ", stateMachine.is_valid());
        updateBlendPositions(lastDir);
    }

    if (has_node("BlinkAnimationPlayer"))
        blinkAnimPlayer = get_node<AnimationPlayer>("BlinkAnimationPlayer");

    if (has_node("Hurtbox")) {
        Area2D* hurtbox = get_node<Area2D>("Hurtbox");
        hurtbox->connect("area_entered", Callable(this, "on_hurtbox_area_entered"));
        if (hurtbox->has_node("CollisionShape2D"))
            hurtboxShape = hurtbox->get_node<CollisionShape2D>("CollisionShape2D");
    }

    UtilityFunctions::print("Player ready. HP: ", player->getStats().getHP());
}

void PlayerNode::onHurtboxAreaEntered(Area2D* area) {
    if (isInvincible || isDead) return;
    if (contactDamageTimer > 0.0f) return;
    contactDamageTimer = contactDamageCooldown;
    takeDamage(20);
}

void PlayerNode::takeDamage(int amount) {
    if (isInvincible || isDead) return;

    isInvincible = true;
    iframeTimer  = iframeDuration;

    player->takeDamage(amount);
    UtilityFunctions::print("Player HP: ", player->getStats().getHP());

    if (!player->isAlive()) {
        isDead = true;
        if (hurtboxShape)
            hurtboxShape->set_deferred("disabled", true);
        if (blinkAnimPlayer)
            blinkAnimPlayer->play("blink");
        return;
    }

    if (hurtboxShape)
        hurtboxShape->set_deferred("disabled", true);
    if (blinkAnimPlayer)
        blinkAnimPlayer->play("blink");
}

void PlayerNode::healPlayer(int amount) {
    if (isDead) return;
    player->getStats().heal(amount);
    UtilityFunctions::print("Player healed. HP: ", player->getStats().getHP());
}

void PlayerNode::_physics_process(double delta) {
    contactDamageTimer -= (float)delta;

    if (isDead) {
        deathTimer -= (float)delta;
        if (deathTimer <= 0.0f)
            queue_free();
        return;
    }

    if (isInvincible) {
        iframeTimer -= (float)delta;
        if (iframeTimer <= 0.0f) {
            isInvincible = false;
            if (hurtboxShape)
                hurtboxShape->set_deferred("disabled", false);
            if (blinkAnimPlayer)
                blinkAnimPlayer->play("RESET");
        }
    }

    Input* input = Input::get_singleton();

    if (!stateMachine.is_valid()) {
        Vector2 vel;
        if (input->is_action_pressed("move_left"))  vel.x -= 1;
        if (input->is_action_pressed("move_right")) vel.x += 1;
        if (input->is_action_pressed("move_up"))    vel.y -= 1;
        if (input->is_action_pressed("move_down"))  vel.y += 1;
        inputVector = vel;
        if (vel != Vector2(0,0)) lastDir = vel.normalized();
        set_velocity(vel.normalized() * speed);
        move_and_slide();
        return;
    }

    StringName state = stateMachine->get_current_node();

    if (state == StringName("MoveState")) {
        moveState();
    } else if (state == StringName("RollState")) {
        rollState();
    }
}

void PlayerNode::moveState() {
    Input* input = Input::get_singleton();

    Vector2 vel = Vector2(0, 0);
    if (input->is_action_pressed("move_left"))  vel.x -= 1;
    if (input->is_action_pressed("move_right")) vel.x += 1;
    if (input->is_action_pressed("move_up"))    vel.y -= 1;
    if (input->is_action_pressed("move_down"))  vel.y += 1;

    if (vel.length() > 1.0f)
        vel = vel.normalized();

    inputVector = vel;

    if (vel != Vector2(0, 0)) {
        float ax = Math::abs(vel.x);
        float ay = Math::abs(vel.y);
        if (Math::abs(ax - ay) > 0.4f)
            lastDir = vel.normalized();
        updateBlendPositions(lastDir);
    }

    if (input->is_action_just_pressed("attack")) {
        stateMachine->travel("AttackState");
        return;
    }
    if (input->is_action_just_pressed("roll")) {
        stateMachine->travel("RollState");
        return;
    }

    set_velocity(vel * speed);
    move_and_slide();
}

void PlayerNode::rollState() {
    set_velocity(lastDir.normalized() * rollSpeed);
    move_and_slide();
}
