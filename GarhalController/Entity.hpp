#pragma once
#include "data.hpp"

class Entity
{
private:
    int EntityAddress = 0;
public:
    bool InValid = false;

    void SetCorrectGlowStruct(int LocalPlayerTeam, int GlowObject);

    Vector3 getBonePosition(uint32_t boneId);
    Vector3 getAbsolutePosition();
    Vector3 getFeetPosition();
    Vector3 getAimPunch();
    Vector3 getVelocity();
    Vector3 getHeadPosition();

    int getForceAttack();
    void setForceAttack(int value);
    void setForceAttack2(int value);
    void shoot();
    int getCrosshairId();

    int getShotsFired();
    int getBase();
	
    int IsDormant();
    bool IsDefusing();
    bool isOnGround();
    bool isInAir();
    bool isValidPlayer();
    uint32_t getHealth();
    int getTeam();
    void SetFlashAlpha(float num);
    void SetForceJump(int value);
    int GetGlowIndex();
    Entity();
    Entity(int EntityAddress);
    ~Entity();
};
