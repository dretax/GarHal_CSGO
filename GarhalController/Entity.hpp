#pragma once
#include "data.hpp"

class Entity
{
private:
    uint32_t EntityAddress = 0;
public:
    Vector3 getBonePosition(uint32_t boneId);
    Vector3 getAbsolutePosition();
    Vector3 getFeetPosition();
    Vector3 getAimPunch();
    Vector3 getVelocity();
    Vector3 getHeadPosition();
    Vector3 GetBonePosition(uint32_t targetBone);

    uint32_t GetEntityAddress();
    uint32_t GetGlowIndex();
    uint16_t getCrosshairId();
    uint16_t getShotsFired();
    uint8_t getHealth();
    uint8_t getForceAttack();
    uint8_t getTeam();
    void SetFlashAlpha(float num);
    void SetForceJump(uint8_t value);
    void setForceAttack(uint8_t value);
    void setForceAttack2(uint8_t value);
    void shoot();
    void SetCorrectGlowStruct(uint8_t LocalPlayerTeam, uint32_t GlowObject);
	
    bool IsDormant();
    bool IsDefusing();
    bool isInAir();
    bool isValidPlayer();
    bool IsCrouching();
    Entity();
    Entity(uint32_t EntityAddress);
    ~Entity();
};
