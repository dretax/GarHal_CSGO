#include "Entity.hpp"

#include <iostream>

#include "offsets.hpp"

// hazedumper namespace
using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

int Entity::IsDormant()
{
	bool isDormant = Driver.ReadVirtualMemory<bool>(ProcessId, EntityAddress + m_bDormant, sizeof(int));
    return isDormant;
}

bool Entity::IsDefusing()
{
    bool Defusing = Driver.ReadVirtualMemory<bool>(ProcessId, EntityAddress + m_bIsDefusing, sizeof(int));
    return Defusing;
}

void Entity::SetFlashAlpha(float num)
{
    // No Flash if set to 0
    Driver.WriteVirtualMemory(ProcessId, EntityAddress + m_flFlashMaxAlpha, num, 8);
}

int Entity::getTeam()
{
    int OurTeam = Driver.ReadVirtualMemory<int>(ProcessId, EntityAddress + m_iTeamNum, sizeof(int));
    return OurTeam;
}

bool Entity::isInAir()
{
	int flags = Driver.ReadVirtualMemory<int>(ProcessId, EntityAddress + m_fFlags, sizeof(int));
	return flags == 256 || flags == 258 || flags == 260 || flags == 262;
}

uint32_t Entity::getHealth()
{
	int health = Driver.ReadVirtualMemory<int>(ProcessId, EntityAddress + m_iHealth, sizeof(int));
	return health;
}


void Entity::SetForceJump(int value)
{
	Driver.WriteVirtualMemory(ProcessId, ClientAddress + dwForceJump, value, sizeof(int));
}

bool Entity::isValidPlayer()
{
	int health = getHealth();
	bool isDormant = IsDormant();

	return health > 0 && health <= 100 && !isDormant;
}

Vector3 Entity::getAbsolutePosition()
{
	Vector3 position = getFeetPosition();
	position(2) += Driver.ReadVirtualMemory<int>(ProcessId, EntityAddress + 0x10c, sizeof(Vector3));
	return position;
}

Vector3 Entity::getFeetPosition()
{
	Vector3 position = Driver.ReadVirtualMemory<int>(ProcessId, EntityAddress + m_vecOrigin, sizeof(Vector3));
	return position;
}

Vector3 Entity::getAimPunch()
{
	Vector3 aimPunch = Driver.ReadVirtualMemory<int>(ProcessId, EntityAddress + m_aimPunchAngle, sizeof(Vector3));
	return aimPunch;
}

Vector3 Entity::getVelocity()
{
	Vector3 vel = Driver.ReadVirtualMemory<int>(ProcessId, EntityAddress + m_vecVelocity, sizeof(Vector3));
	return vel;
}

Vector3 Entity::getBonePosition(uint32_t boneId)
{
	int boneBase = Driver.ReadVirtualMemory<int>(ProcessId, EntityAddress + m_dwBoneMatrix, sizeof(int));

	Vector3 bonePosition;
	bonePosition(0) = Driver.ReadVirtualMemory<float>(ProcessId, boneBase + 0x30 * boneId + 0x0C, sizeof(float));
	bonePosition(1) = Driver.ReadVirtualMemory<float>(ProcessId, boneBase + 0x30 * boneId + 0x1C, sizeof(float));
	bonePosition(2) = Driver.ReadVirtualMemory<float>(ProcessId, boneBase + 0x30 * boneId + 0x2C, sizeof(float));

	return bonePosition;
}

int Entity::getCrosshairId()
{
	return Driver.ReadVirtualMemory<int>(ProcessId, EntityAddress + m_iCrosshairId, sizeof(int));
}

int Entity::getForceAttack()
{
	return Driver.ReadVirtualMemory<int>(ProcessId, EntityAddress + dwForceAttack, sizeof(int));
}

void Entity::setForceAttack(int value)
{
	Driver.WriteVirtualMemory(ProcessId, EntityAddress + dwForceAttack, value, sizeof(int));
}

void Entity::setForceAttack2(int value)
{
	Driver.WriteVirtualMemory(ProcessId, EntityAddress + dwForceAttack2, value, sizeof(int));
}

void Entity::shoot()
{
	uint32_t forceAttack = getForceAttack();
	if (forceAttack == 4)
		setForceAttack(5);
	else
		setForceAttack(4);
}

int Entity::getShotsFired()
{
	return Driver.ReadVirtualMemory<int>(ProcessId, EntityAddress + m_iShotsFired, sizeof(int));
}

int Entity::getBase()
{
	return EntityAddress;
}

int Entity::GetGlowIndex()
{
    int GlowIndex = Driver.ReadVirtualMemory<int>(ProcessId, EntityAddress + m_iGlowIndex, sizeof(int));
    return GlowIndex;
}

void Entity::SetCorrectGlowStruct(int OurTeam, int GlowObject)
{
	int ReadTeam = this->getTeam();
	bool Defusing = this->IsDefusing();
	
	GlowStruct EGlow;
	EGlow = Driver.ReadVirtualMemory<GlowStruct>(ProcessId, GlowObject + (this->GetGlowIndex() * 0x38), sizeof(GlowStruct));
	EGlow.alpha = 0.5f;
	EGlow.renderWhenOccluded = true;
	EGlow.renderWhenUnOccluded = false;

	if (OurTeam != ReadTeam)
	{
		EGlow.red = 255.0f;
		EGlow.green = 0.0f;
		EGlow.blue = 0.0f;

		if (Defusing)
		{
			EGlow.green = 60.0f;
		}
	}
	else
	{
		EGlow.red = 0.0f;
		EGlow.green = 0.0f;
		EGlow.blue = 255.0f;

		if (Defusing)
		{
			EGlow.green = 60.0f;
		}
	}

	Driver.WriteVirtualMemory(ProcessId, GlowObject + (this->GetGlowIndex() * 0x38), EGlow, sizeof(EGlow));
}


Entity::Entity(int EntityAddress)
{
    this->EntityAddress = EntityAddress;
}

Entity::Entity()
{
}

Entity::~Entity()
{

}