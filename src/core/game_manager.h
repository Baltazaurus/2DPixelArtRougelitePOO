#pragma once
#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include "types.h"

class GameManager { ///////////////////////////are constr pe privat
private:
    static GameManager* instance;

    int currentWave;
    int enemiesAlive;
    int score;
    std::vector<int> waveEnemyCounts;                             /////////////////////////////////////////////////vector si stack
    std::stack<GameState> stateHistory;
    GameState currentState;

    GameManager();
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

public:///////////////////////////////////////////////////////////////////////////////////////static
    static GameManager* getInstance();
    static void destroyInstance();

    void  startGame();
    void  startWave();
    void  onEnemyDied();
    bool  isWaveComplete() const { return enemiesAlive <= 0; }
    void  nextWave();

    int   getCurrentWave()  const { return currentWave; }
    int   getEnemiesAlive() const { return enemiesAlive; }
    int   getScore() const { return score; }
    int   getEnemiesForWave() const;
    bool  hasMoreWaves()  const { return currentWave <= (int)waveEnemyCounts.size(); }

    void  pushState(GameState s)  { stateHistory.push(s); currentState = s; }
    void  popState();
    GameState getState() const { return currentState; }

    void  addScore(int points) { score += points; }

    friend std::ostream& operator<<(std::ostream& os, const GameManager& gm);
};
