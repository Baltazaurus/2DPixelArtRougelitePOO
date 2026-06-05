#include "hud_node.h"
#include "core/game_manager.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/label.hpp>

using namespace godot;

void HUDNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("show_wave_announcement", "wave"), &HUDNode::showWaveAnnouncement);
}

HUDNode::HUDNode()
    : fullHearts(nullptr), waveLabel(nullptr),
      scoreLabel(nullptr), enemiesLabel(nullptr),
      waveAnnounceLabel(nullptr), playerRef(nullptr),
      maxWidth(0.0f),
      announceTimer(0.0f), announceFadeIn(0.5f),
      announceHold(1.0f), announceFadeOut(0.5f),
      announcing(false) {}

HUDNode::~HUDNode() {}

void HUDNode::_ready() {
    // find FullHearts via sibling HeartsUI
    Node* parent = get_parent();
    if (parent && parent->has_node("HeartsUI")) {
        Node* heartsUI = parent->get_node<Node>("HeartsUI");
        if (heartsUI && heartsUI->has_node("FullHearts")) {
            fullHearts = heartsUI->get_node<TextureRect>("FullHearts");
            maxWidth   = fullHearts->get_size().x;
        }
    }

    if (has_node("WaveLabel"))
        waveLabel = get_node<Label>("WaveLabel");
    if (has_node("ScoreLabel"))
        scoreLabel = get_node<Label>("ScoreLabel");
    if (has_node("EnemiesLabel"))
        enemiesLabel = get_node<Label>("EnemiesLabel");

    // try as child first, then as sibling
    if (has_node("WaveAnnounceLabel")) {
        waveAnnounceLabel = get_node<Label>("WaveAnnounceLabel");
    } else if (parent && parent->has_node("WaveAnnounceLabel")) {
        waveAnnounceLabel = parent->get_node<Label>("WaveAnnounceLabel");
    }

    if (waveAnnounceLabel)
        waveAnnounceLabel->set_modulate(Color(1, 1, 1, 0));

    UtilityFunctions::print("WaveAnnounceLabel found: ", waveAnnounceLabel != nullptr);

    SceneTree* tree = get_tree();
    if (tree) {
        TypedArray<Node> players = tree->get_nodes_in_group("player");
        if (players.size() > 0)
            playerRef = Object::cast_to<Node>(players[0]);
    }

    UtilityFunctions::print("HUDNode ready. maxWidth: ", maxWidth);
}

void HUDNode::showWaveAnnouncement(int wave) {
    UtilityFunctions::print("showWaveAnnouncement called, wave: ", wave, " label: ", waveAnnounceLabel != nullptr);
    if (!waveAnnounceLabel) return;
    waveAnnounceLabel->set_text("WAVE " + String::num_int64(wave));
    waveAnnounceLabel->set_modulate(Color(1, 1, 1, 0));
    announceTimer = 0.0f;
    announcing    = true;
}

void HUDNode::_process(double delta) {
    // update hearts
    if (fullHearts && playerRef) {
        if (maxWidth <= 0.0f)
            maxWidth = fullHearts->get_size().x;

        int hp    = playerRef->call("get_hp");
        int maxHp = playerRef->call("get_max_hp");

        if (maxHp > 0) {
            float heartWidth = maxWidth / 5.0f;
            int   hearts     = hp / 20;
            fullHearts->set_size(Vector2(heartWidth * hearts, fullHearts->get_size().y));
        }
    }

    // update labels
    GameManager* gm = GameManager::getInstance();

    if (waveLabel)
        waveLabel->set_text("Wave " + String::num_int64(gm->getCurrentWave()));
    if (scoreLabel)
        scoreLabel->set_text("Score: " + String::num_int64(gm->getScore()));
    if (enemiesLabel)
        enemiesLabel->set_text("Enemies Remaining: " + String::num_int64(gm->getEnemiesAlive()));

    // wave announcement fade
    if (announcing && waveAnnounceLabel) {
        announceTimer += (float)delta;
        float totalDuration = announceFadeIn + announceHold + announceFadeOut;
        float alpha = 0.0f;

        if (announceTimer < announceFadeIn) {
            alpha = announceTimer / announceFadeIn;
        } else if (announceTimer < announceFadeIn + announceHold) {
            alpha = 1.0f;
        } else if (announceTimer < totalDuration) {
            float t = (announceTimer - announceFadeIn - announceHold) / announceFadeOut;
            alpha = 1.0f - t;
        } else {
            alpha      = 0.0f;
            announcing = false;
        }

        waveAnnounceLabel->set_modulate(Color(1, 1, 1, alpha));
    }
}