#pragma once
#include "game/game_settings.hpp"

class GameSystem
{
private:
    const GameSettings m_settings;

public:
    explicit GameSystem(const GameSettings & settings);
    ~GameSystem() = default;

    void update();
};
