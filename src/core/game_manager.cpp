#include "game_manager.h"
#include <iostream>

GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
    : currentWave(1), enemiesAlive(0), score(0),
      currentState(GameState::MAIN_MENU) {
    waveEnemyCounts = {7, 10, 21};
    stateHistory.push(GameState::MAIN_MENU);
}

GameManager* GameManager::getInstance() {
    if (!instance)
        instance = new GameManager();
    return instance;
}

void GameManager::destroyInstance() {
    delete instance;
    instance = nullptr;
}

int GameManager::getEnemiesForWave() const {
    if (currentWave <= (int)waveEnemyCounts.size())
        return waveEnemyCounts[currentWave - 1];
    return waveEnemyCounts.back() + (currentWave - (int)waveEnemyCounts.size()) * 5;
}

void GameManager::startGame() {
    currentWave = 1;
    enemiesAlive = 0;
    score = 0;
    pushState(GameState::PLAYING);
}

void GameManager::startWave() {
    enemiesAlive = getEnemiesForWave();
    pushState(GameState::PLAYING);
    std::cout << "Wave " << currentWave << " started. Enemies: " << enemiesAlive << "\n";
}

void GameManager::onEnemyDied() {
    if (enemiesAlive > 0)
        --enemiesAlive;

    score += 10;

    if (enemiesAlive <= 0)
        pushState(GameState::WAVE_COMPLETE);
}

void GameManager::nextWave() {
    ++currentWave;
    startWave();
}

void GameManager::popState() {
    if (stateHistory.size() > 1)
        stateHistory.pop();
    currentState = stateHistory.top();
}

std::ostream& operator<<(std::ostream& os, const GameManager& gm) {
    os << "Wave:" << gm.currentWave
       << " Enemies:" << gm.enemiesAlive
       << " Score:" << gm.score;
    return os;
}
