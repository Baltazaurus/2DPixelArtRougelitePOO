#pragma once
#include <godot_cpp/classes/static_body2d.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

class BushNode : public StaticBody2D {
    GDCLASS(BushNode, StaticBody2D)

private:
    AnimationPlayer* animPlayer;
    CollisionShape2D* collision;
    bool              destroyed;

protected:
    static void _bind_methods();

public:
    BushNode();
    ~BushNode();

    void _ready() override;

    // called by player when they walk into or attack the bush
    void takeDamage();
};

} // namespace godot
