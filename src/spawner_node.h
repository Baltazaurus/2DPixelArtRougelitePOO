#pragma once
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

class SpawnerNode : public Node2D {
    GDCLASS(SpawnerNode, Node2D)

private:
    Ref<PackedScene> enemyScene;
    Node2D*          playerRef;
    Timer*           waveTimer;
    float            spawnRadius;
    float            minSpawnRadius;
    bool             spawning;
    int              enemiesSpawnedThisWave;
    int              enemiesToSpawnThisWave;
    float            spawnInterval;
    float            spawnTimer;
    float            wavePauseTimer;
    float            wavePauseDuration;

    void spawnEnemy();
    void startNextWave();
    void showWaveAnnounce(int wave);

protected:
    static void _bind_methods();

public:
    SpawnerNode();
    ~SpawnerNode();

    void _ready()               override;
    void _process(double delta) override;

    void onEnemyDied();

    void setEnemyScene(const Ref<PackedScene>& s) { enemyScene = s; }
    Ref<PackedScene> getEnemyScene() const { return enemyScene; }
};

} // namespace godot
