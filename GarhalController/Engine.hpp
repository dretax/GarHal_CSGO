#pragma once
#include "data.hpp"

class Engine
{
public:
    bool worldToScreen(const Vector3& from, Vector3& to);
    bool IsInGame();
    Vector3 getViewAngles();
    void setViewAngles(Vector3& viewAngles);
    Engine();
    ~Engine();
};
