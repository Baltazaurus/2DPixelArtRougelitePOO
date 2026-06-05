#include "spawner_node.h"
#include "player_node.h"
#include "enemy_node.h"
#include "hud_node.h"
#include "core/game_manager.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/classes/window.hpp>
#include <cstdlib>
#include <cmath>

using namespace godot;

void SpawnerNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_enemy_died"), &SpawnerNode::onEnemyDied);
    ClassDB::bind_method(D_METHOD("set_enemy_scene", "scene"), &SpawnerNode::setEnemyScene);
    ClassDB::bind_method(D_METHOD("get_enemy_scene"), &SpawnerNode::getEnemyScene);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "enemy_scene",
                 PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"),
                 "set_enemy_scene", "get_enemy_scene");
}

SpawnerNode::SpawnerNode()
    : playerRef(nullptr), waveTimer(nullptr),
      spawnRadius(350.0f), minSpawnRadius(150.0f),
      spawning(false),
      enemiesSpawnedThisWave(0), enemiesToSpawnThisWave(0),
      spawnInterval(0.5f), spawnTimer(0.0f),
      wavePauseTimer(0.0f), wavePauseDuration(3.0f) {}

SpawnerNode::~SpawnerNode() {}

void SpawnerNode::_ready() {
    if (Engine::get_singleton()->is_editor_hint()) return;

    if (!enemyScene.is_valid())
        enemyScene = ResourceLoader::get_singleton()->load("res://scenes/enemy_node.tscn");

    SceneTree* tree = get_tree();
    if (tree) {
        TypedArray<Node> players = tree->get_nodes_in_group("player");
        if (players.size() > 0)
            playerRef = Object::cast_to<Node2D>(players[0]);
    }

    GameManager::getInstance()->startGame();
    startNextWave();
}

void SpawnerNode::showWaveAnnounce(int wave) {
    // find HUDNode in CanvasLayer
    SceneTree* tree = get_tree();
    if (!tree) return;
    TypedArray<Node> huds = tree->get_nodes_in_group("hud");
    if (huds.size() > 0) {
        HUDNode* hud = Object::cast_to<HUDNode>(huds[0]);
        if (hud) hud->showWaveAnnouncement(wave);
        return;
    }
    // fallback — search scene tree
    Window* root = tree->get_root();
    if (!root) return;
    Node* canvas = root->find_child("CanvasLayer", true, false);
    if (!canvas) return;
    Node* hudNode = canvas->find_child("HUDNode", true, false);
    if (!hudNode) return;
    HUDNode* hud = Object::cast_to<HUDNode>(hudNode);
    if (hud) hud->showWaveAnnouncement(wave);
}

void SpawnerNode::startNextWave() {
    GameManager* gm = GameManager::getInstance();
    gm->startWave();

    enemiesToSpawnThisWave = gm->getEnemiesForWave();
    enemiesSpawnedThisWave = 0;
    spawnTimer             = 0.0f;
    spawning               = true;
    wavePauseTimer         = 0.0f;

    showWaveAnnounce(gm->getCurrentWave());
    UtilityFunctions::print("=== WAVE ", gm->getCurrentWave(), " START === Enemies: ", enemiesToSpawnThisWave);
}

void SpawnerNode::spawnEnemy() {
    if (!enemyScene.is_valid() || !playerRef) return;

    float angle  = static_cast<float>(rand()) / RAND_MAX * 6.2831f;
    float radius = minSpawnRadius + static_cast<float>(rand()) / RAND_MAX * (spawnRadius - minSpawnRadius);
    Vector2 spawnPos = playerRef->get_global_position() +
                       Vector2(std::cos(angle), std::sin(angle)) * radius;

    Node* instance = Object::cast_to<Node>(enemyScene->instantiate());
    if (!instance) return;

    Node2D* inst2d = Object::cast_to<Node2D>(instance);
    get_parent()->add_child(instance);
    if (inst2d) inst2d->set_global_position(spawnPos);

    EnemyNode* enemy = Object::cast_to<EnemyNode>(instance);
    if (enemy)
        enemy->connect("tree_exited", Callable(this, "on_enemy_died"));

    ++enemiesSpawnedThisWave;
}

void SpawnerNode::onEnemyDied() {
    GameManager::getInstance()->onEnemyDied();
}

void SpawnerNode::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) return;

    GameManager* gm = GameManager::getInstance();

    if (wavePauseTimer > 0.0f) {
        wavePauseTimer -= (float)delta;
        if (wavePauseTimer <= 0.0f)
            startNextWave();
        return;
    }

    if (spawning && enemiesSpawnedThisWave < enemiesToSpawnThisWave) {
        spawnTimer -= (float)delta;
        if (spawnTimer <= 0.0f) {
            spawnEnemy();
            spawnTimer = spawnInterval;
        }
    } else if (spawning && enemiesSpawnedThisWave >= enemiesToSpawnThisWave) {
        spawning = false;
    }

    if (!spawning && gm->isWaveComplete()) {
        UtilityFunctions::print("=== WAVE ", gm->getCurrentWave(), " COMPLETE === Score: ", gm->getScore());
        wavePauseTimer = wavePauseDuration;
        gm->nextWave();
    }
}