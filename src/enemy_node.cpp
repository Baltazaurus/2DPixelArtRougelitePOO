#include "enemy_node.h"
#include "player_node.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <cstdlib>
#include <ctime>

using namespace godot;

void EnemyNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("take_damage", "amount"), &EnemyNode::takeDamage);
    ClassDB::bind_method(D_METHOD("on_animation_finished"), &EnemyNode::onAnimationFinished);
    ClassDB::bind_method(D_METHOD("on_hurtbox_area_entered", "area"), &EnemyNode::onHurtboxAreaEntered);
    ClassDB::bind_method(D_METHOD("set_chase_speed", "speed"), &EnemyNode::setChaseSpeed);
    ClassDB::bind_method(D_METHOD("get_chase_speed"), &EnemyNode::getChaseSpeed);
    ClassDB::bind_method(D_METHOD("set_attack_range", "range"), &EnemyNode::setAttackRange);
    ClassDB::bind_method(D_METHOD("get_attack_range"), &EnemyNode::getAttackRange);
    ClassDB::bind_method(D_METHOD("set_potion_scene", "scene"), &EnemyNode::setPotionScene);
    ClassDB::bind_method(D_METHOD("get_potion_scene"), &EnemyNode::getPotionScene);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "chase_speed"), "set_chase_speed", "get_chase_speed");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_range"), "set_attack_range", "get_attack_range");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "potion_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"),
                 "set_potion_scene", "get_potion_scene");
}

EnemyNode::EnemyNode()
    : enemy(nullptr), chaseSpeed(80.0f), wanderSpeed(40.0f),
      attackRange(60.0f), detectionRange(100.0f), loseRange(150.0f),
      maxWanderDist(200.0f),
      attackCooldown(1.5f), cooldownTimer(0.0f),
      stateTimer(2.0f), gatherTimer(0.0f), gatherDuration(0.5f),
      knockbackTimer(0.0f), knockbackDuration(0.2f), knockbackSpeed(120.0f),
      iframeTimer(0.0f), iframeDuration(0.4f),
      isInvincible(false), isAttacking(false),
      knockbackDir(Vector2(0,0)), wanderDir(Vector2(1,0)), lastDir(Vector2(1,0)),
      homePosition(Vector2(0,0)), lastAnimSuffix("down"),
      state(EnemyState::IDLE),
      animPlayer(nullptr), blinkAnimPlayer(nullptr),
      hurtboxShape(nullptr), sprite(nullptr), playerRef(nullptr) {

    Stats s(40, 20, 2, 4);
    enemy = new Enemy(Enemy::getSpawnedCount() + 1, "Goblin", s,
                      DamageType::PHYSICAL, 20);
}

EnemyNode::~EnemyNode() { delete enemy; }

float EnemyNode::randRange(float mn, float mx) {
    return mn + static_cast<float>(rand()) / RAND_MAX * (mx - mn);
}

String EnemyNode::getDirectionSuffix(Vector2 dir) {
    float ax = Math::abs(dir.x);
    float ay = Math::abs(dir.y);
    float threshold = 0.4f;
    String result;
    if (ay > ax + threshold)      result = dir.y > 0 ? "down" : "up";
    else if (ax > ay + threshold) result = dir.x > 0 ? "right" : "left";
    else                          result = lastAnimSuffix;
    lastAnimSuffix = result;
    return result;
}

void EnemyNode::playAnim(const String& anim) {
    if (animPlayer && animPlayer->get_current_animation() != anim)
        animPlayer->play(anim);
}

void EnemyNode::_ready() {
    srand(static_cast<unsigned int>(time(nullptr)));
    homePosition = get_global_position();

    if (has_node("AnimationPlayer")) {
        animPlayer = get_node<AnimationPlayer>("AnimationPlayer");
        animPlayer->connect("animation_finished", Callable(this, "on_animation_finished"));
    }
    if (has_node("BlinkAnimationPlayer"))
        blinkAnimPlayer = get_node<AnimationPlayer>("BlinkAnimationPlayer");
    if (has_node("Sprite2D"))
        sprite = get_node<Sprite2D>("Sprite2D");
    if (has_node("Hurtbox")) {
        Area2D* hurtbox = get_node<Area2D>("Hurtbox");
        hurtbox->connect("area_entered", Callable(this, "on_hurtbox_area_entered"));
        if (hurtbox->has_node("CollisionShape2D"))
            hurtboxShape = hurtbox->get_node<CollisionShape2D>("CollisionShape2D");
    }

    SceneTree* tree = get_tree();
    if (tree) {
        TypedArray<Node> players = tree->get_nodes_in_group("player");
        if (players.size() > 0)
            playerRef = Object::cast_to<Node2D>(players[0]);
    }

    // load potion scene if not set via inspector
    if (!potionScene.is_valid()) {
        potionScene = ResourceLoader::get_singleton()->load("res://scenes/potion.tscn");
    }

    stateTimer = randRange(1.0f, 3.0f);
    UtilityFunctions::print("Enemy spawned. Total: ", Enemy::getSpawnedCount());
}

void EnemyNode::onHurtboxAreaEntered(Area2D* area) {
    if (isInvincible) return;
    Node* parent = area->get_parent();
    if (!parent) return;
    if (playerRef) {
        knockbackDir   = (get_global_position() - playerRef->get_global_position()).normalized();
        knockbackTimer = knockbackDuration;
    }
    takeDamage(20);
}

void EnemyNode::onAnimationFinished(StringName anim_name) {
    String name = anim_name;
    if (name.begins_with("attack_")) {
        isAttacking    = false;
        knockbackDir   = -lastDir;
        knockbackTimer = knockbackDuration;
        state          = EnemyState::GATHER;
        gatherTimer    = gatherDuration;
        if (animPlayer)
            animPlayer->play("idle_" + getDirectionSuffix(lastDir));
    }
}

void EnemyNode::takeDamage(int amount) {
    if (isInvincible) return;

    isInvincible = true;
    iframeTimer  = iframeDuration;

    enemy->takeDamage(amount);
    UtilityFunctions::print(enemy->getName().c_str(), " HP: ", enemy->getStats().getHP());

    if (hurtboxShape)
        hurtboxShape->set_deferred("disabled", true);
    if (blinkAnimPlayer)
        blinkAnimPlayer->play("blink");

    if (!enemy->isAlive()) {
        UtilityFunctions::print(enemy->getName().c_str(), " defeated.");

        // 1/10 chance to drop potion
        if (rand() % 5 == 0 && potionScene.is_valid()) {
            Node* instance = Object::cast_to<Node>(potionScene->instantiate());
            if (instance) {
                Node2D* inst2d = Object::cast_to<Node2D>(instance);
                get_parent()->add_child(instance);
                if (inst2d)
                    inst2d->set_global_position(get_global_position());
            }
        }

        queue_free();
    }
}

void EnemyNode::_physics_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) return;
    if (!enemy->isAlive()) return;

    cooldownTimer  -= (float)delta;
    knockbackTimer -= (float)delta;
    stateTimer     -= (float)delta;
    gatherTimer    -= (float)delta;

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

    if (knockbackTimer > 0.0f) {
        set_velocity(knockbackDir * knockbackSpeed);
        move_and_slide();
        return;
    }

    if (isAttacking) {
        set_velocity(Vector2(0, 0));
        move_and_slide();
        return;
    }

    float distToPlayer = 999999.0f;
    if (playerRef)
        distToPlayer = get_global_position().distance_to(playerRef->get_global_position());

    if (state != EnemyState::GATHER) {
        if (state != EnemyState::CHASE && distToPlayer < detectionRange)
            state = EnemyState::CHASE;
        else if (state == EnemyState::CHASE && distToPlayer > loseRange) {
            state = EnemyState::IDLE;
            stateTimer = randRange(1.0f, 3.0f);
        }
    } else if (gatherTimer <= 0.0f) {
        state = distToPlayer < loseRange ? EnemyState::CHASE : EnemyState::IDLE;
        if (state == EnemyState::IDLE)
            stateTimer = randRange(1.0f, 3.0f);
    }

    switch (state) {
        case EnemyState::IDLE: {
            set_velocity(Vector2(0, 0));
            move_and_slide();
            playAnim("idle_" + getDirectionSuffix(lastDir));
            if (stateTimer <= 0.0f) {
                state = EnemyState::WANDER;
                stateTimer = randRange(1.0f, 2.0f);
                float angle = randRange(0.0f, 6.2831f);
                wanderDir = Vector2(Math::cos(angle), Math::sin(angle));
                lastDir = wanderDir;
            }
            break;
        }
        case EnemyState::WANDER: {
            float distFromHome = get_global_position().distance_to(homePosition);
            if (distFromHome > maxWanderDist) {
                wanderDir = (homePosition - get_global_position()).normalized();
                lastDir = wanderDir;
            }
            set_velocity(wanderDir * wanderSpeed);
            playAnim("run_" + getDirectionSuffix(wanderDir));
            move_and_slide();
            if (stateTimer <= 0.0f) {
                state = EnemyState::IDLE;
                stateTimer = randRange(1.0f, 3.0f);
            }
            break;
        }
        case EnemyState::GATHER: {
            set_velocity(Vector2(0, 0));
            move_and_slide();
            playAnim("idle_" + getDirectionSuffix(lastDir));
            break;
        }
        case EnemyState::CHASE: {
            if (!playerRef) break;
            Vector2 toPlayer = playerRef->get_global_position() - get_global_position();
            if (distToPlayer < attackRange) {
                set_velocity(Vector2(0, 0));
                move_and_slide();
                if (cooldownTimer <= 0.0f) {
                    cooldownTimer = attackCooldown;
                    isAttacking   = true;
                    playerRef->call("take_damage", enemy->getDamage());
                    if (animPlayer)
                        animPlayer->play("attack_" + getDirectionSuffix(lastDir));
                }
            } else {
                Vector2 dir = toPlayer.normalized();
                lastDir = dir;
                set_velocity(dir * chaseSpeed);
                playAnim("run_" + getDirectionSuffix(dir));
                move_and_slide();
            }
            break;
        }
    }
}
