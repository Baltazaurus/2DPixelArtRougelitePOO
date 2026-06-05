#include "bush_node.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void BushNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("take_damage"), &BushNode::takeDamage);
}

BushNode::BushNode()
    : animPlayer(nullptr), collision(nullptr), destroyed(false) {}

BushNode::~BushNode() {}

void BushNode::_ready() {
    if (has_node("AnimationPlayer"))
        animPlayer = get_node<AnimationPlayer>("AnimationPlayer");
    if (has_node("CollisionShape2D"))
        collision = get_node<CollisionShape2D>("CollisionShape2D");
}

void BushNode::takeDamage() {
    if (destroyed) return;  // ignore hits during death animation
    destroyed = true;

    // disable collision immediately so nothing interacts mid-animation
    if (collision)
        collision->set_deferred("disabled", true);

    if (animPlayer) {
        animPlayer->play("destroy");
        // queue_free is connected to animation_finished signal in the scene
    } else {
        queue_free();
    }
}
