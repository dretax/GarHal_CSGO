#pragma once
#include "data.hpp"

namespace engine
{
    bool worldToScreen(const Vector3& from, Vector3& to);
    bool IsInGame();
    GameState GetGameState(uint8_t State);
    Vector3 getViewAngles();
    void setViewAngles(Vector3& viewAngles);
}
