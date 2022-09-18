#pragma once
#include "data.hpp"

class Entity
{
private:
    uint32_t EntityAddress = 0;
    uint32_t StudioHdrAddress = 0;
    StudioHdr StudioHdrSt = {};
    StudioHitboxSet StudioHitBoxSet = {};
    StudioBBox StudioHitBoxes[MAX_STUDIO_BONES];
    StudioBone StudioBones[MAX_STUDIO_BONES];
public:
    Vector3 BonePositions[MAX_STUDIO_BONES];
    std::pair<int, int> BonePairs[MAX_STUDIO_BONES];
    uint8_t CurrentBonePairs = 0;
    
    //Vector3 getBonePosition(uint32_t boneId);
    Vector3 getAbsolutePosition();
    Vector3 getFeetPosition();
    Vector3 getAimPunch();
    Vector3 getVelocity();
    Vector3 getHeadPosition();
    Vector3 GetBonePosition(uint32_t targetBone);
    RenderData getRenderData(uint8_t OurTeam, Vector3 screenPos, float inGameDistance);

    DWORD GetWeaponHandle();
    DWORD GetCurrentWeapon();
	
    uint32_t GetEntityAddress();
    uint32_t GetGlowIndex();
    uint16_t GetCurrentWeaponID();
    uint16_t GetWeaponIndex();
    uint16_t getCrosshairId();
    uint16_t getShotsFired();
    uint8_t getHealth();
    uint8_t getForceAttack();
    uint8_t getTeam();
    void BuildBonePairs();
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
