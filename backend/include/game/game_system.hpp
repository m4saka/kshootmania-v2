#pragma once
#include "game/game_settings.hpp"

class GameSystem
{
private:
    const GameSettings m_settings;
    double m_prevMs;

public:
    explicit GameSystem(const GameSettings & settings, double initialMs);
    ~GameSystem() = default;

    void update(double currentMs);
};
