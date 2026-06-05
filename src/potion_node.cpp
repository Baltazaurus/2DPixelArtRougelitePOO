#include "potion_node.h"
#include "player_node.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void PotionNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_body_entered", "body"), &PotionNode::onBodyEntered);
}

PotionNode::PotionNode() : healAmount(20) {}
PotionNode::~PotionNode() {}

void PotionNode::_ready() {
    connect("body_entered", Callable(this, "on_body_entered"));
}

void PotionNode::onBodyEntered(Node2D* body) {
    PlayerNode* player = Object::cast_to<PlayerNode>(body);
    if (!player) return;
    player->healPlayer(healAmount);
    UtilityFunctions::print("Potion picked up, healed ", healAmount);
    queue_free();
}
