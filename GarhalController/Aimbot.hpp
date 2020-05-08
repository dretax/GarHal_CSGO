#pragma once
#include "BSPParser.hpp"
#include "data.hpp"
#include "Entity.hpp"

class Aimbot
{
private:
    hazedumper::BSPParser* bspParser;
    float defaultSensitivity;
    Entity findClosestEnemyToFOV(float fov, uint32_t boneId);
    Vector3 angleDifferenceToEntity(Entity& localPlayer, Entity& entity, uint32_t boneId);
    Vector3 aimAnglesTo(Vector3& entity);
    void normalizeAngles(Vector3& angles);
    void clampAngles(Vector3& angles);
    bool enemyIsInCrossHair();
    Vector3 getViewAngles();
    void setViewAngles(Vector3& viewAngles);
    void setSensitivity(float sens);
    float getSensitivity();
    const char* getMapDirectory();
    const char* getGameDirectory();
public:
    Entity localPlayer;
    bool aimAssist(float fov, int boneId);  // legit aimbot - if low then aim chest
    void aimBot(float fov, int boneId);     // blatant aimbot
    void inCrossTriggerBot();
    void walkBot();                         // can be used with aimAssist to make a deathmatch bot
    void resetSensitivity();
    Aimbot(hazedumper::BSPParser* bspParser);
    Aimbot();
    ~Aimbot();
};
