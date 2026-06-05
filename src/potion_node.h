#pragma once
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

class PotionNode : public Area2D {
    GDCLASS(PotionNode, Area2D)

private:
    int healAmount;

protected:
    static void _bind_methods();

public:
    PotionNode();
    ~PotionNode();

    void _ready()                  override;
    void onBodyEntered(Node2D* body);
};

} // namespace godot
