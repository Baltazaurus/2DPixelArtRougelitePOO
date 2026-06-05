#pragma once
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

class HUDNode : public Control {
    GDCLASS(HUDNode, Control)

private:
    TextureRect* fullHearts;
    Label*       waveLabel;
    Label*       scoreLabel;
    Label*       enemiesLabel;
    Label*       waveAnnounceLabel;
    Node*        playerRef;
    float        maxWidth;

    // wave announce animation
    float        announceTimer;
    float        announceFadeIn;
    float        announceHold;
    float        announceFadeOut;
    bool         announcing;

protected:
    static void _bind_methods();

public:
    HUDNode();
    ~HUDNode();

    void _ready()               override;
    void _process(double delta) override;

    void showWaveAnnouncement(int wave);
};

} // namespace godot
