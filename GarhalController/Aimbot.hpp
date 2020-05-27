#pragma once
#include "BSPParser.hpp"
#include "data.hpp"
#include "Entity.hpp"

class Aimbot
{
private:
    hazedumper::BSPParser* bspParser;
    float defaultSensitivity;
    Entity findClosestEnemyToFOV();
    Vector3 angleDifferenceToEntity(Entity& localPlayer, Entity& entity);
    Vector3 getViewAngles();
    Vector3 aimAnglesTo(Vector3& entity);
    void normalizeAngles(Vector3& angles);
    void clampAngles(Vector3& angles);
    void setViewAngles(Vector3& viewAngles);
    void setSensitivity(float sens);
    float getSensitivity();
    bool enemyIsInCrossHair();
    const char* getMapDirectory();
    const char* getGameDirectory();
public:
    Entity localPlayer;
    bool aimAssist();
    void aimBot();
    void walkBot();
    void resetSensitivity();
    void TriggerBot();
    bool EnemyIsInCrossHair();
    Aimbot(hazedumper::BSPParser* bspParser);
    Aimbot();
    ~Aimbot();
};
