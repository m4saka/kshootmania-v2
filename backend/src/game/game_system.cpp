#include "game/game_system.hpp"

GameSystem::GameSystem(const GameSettings & settings, double initialMs)
    : m_prevMs(initialMs)
    , m_settings(settings)
{
}

void GameSystem::update(double currentMs)
{
    m_prevMs = currentMs;
}
