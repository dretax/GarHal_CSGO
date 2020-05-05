#pragma once
#include "data.hpp"
#include "kernelinterface.hpp"

class Entity
{
private:
    int EntityAddress;
public:
    bool InValid;

    GlowStruct GetGlowStruct(int GlowObject);
    void SetCorrectGlowStruct(int LocalPlayerTeam, int GlowObject);
	
    int IsDormant();
    bool IsDefusing();
    bool isOnGround();
    bool isInAir();
    uint32_t getHealth();
    int getTeam();
    void SetFlashAlpha(float &num);
    void SetForceJump(int value);
    int GetGlowIndex();
    Entity();
    Entity(int EntityAddress);
    ~Entity();
};
