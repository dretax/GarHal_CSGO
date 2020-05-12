#include "Entity.hpp"
#include "offsets.hpp"

// hazedumper namespace
using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

bool Entity::IsDormant()
{
	bool isDormant = Driver.ReadVirtualMemory<bool>(ProcessId, EntityAddress + m_bDormant, sizeof(uint8_t));
    return isDormant;
}

bool Entity::IsDefusing()
{
    bool Defusing = Driver.ReadVirtualMemory<bool>(ProcessId, EntityAddress + m_bIsDefusing, sizeof(uint8_t));
    return Defusing;
}

void Entity::SetFlashAlpha(float num)
{
    // No Flash if set to 0
    Driver.WriteVirtualMemory(ProcessId, EntityAddress + m_flFlashMaxAlpha, num, 8);
}

uint8_t Entity::getTeam()
{
	uint8_t OurTeam = Driver.ReadVirtualMemory<uint8_t>(ProcessId, EntityAddress + m_iTeamNum, sizeof(uint8_t));
    return OurTeam;
}

bool Entity::isInAir()
{
	uint32_t flags = Driver.ReadVirtualMemory<uint32_t>(ProcessId, EntityAddress + m_fFlags, sizeof(uint32_t));
	return flags == 256 || flags == 258 || flags == 260 || flags == 262;
}

uint8_t Entity::getHealth()
{
	uint8_t health = Driver.ReadVirtualMemory<uint8_t>(ProcessId, EntityAddress + m_iHealth, sizeof(uint8_t));
	return health;
}


void Entity::SetForceJump(uint8_t value)
{
	Driver.WriteVirtualMemory(ProcessId, ClientAddress + dwForceJump, value, sizeof(value));
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
	//position(2) += Driver.ReadVirtualMemory<float>(ProcessId, EntityAddress + 0x10c, sizeof(float));
	return position;
}

Vector3 Entity::getFeetPosition()
{
	Vector3 position = Driver.ReadVirtualMemory<Vector3>(ProcessId, EntityAddress + m_vecOrigin, sizeof(Vector3));
	return position;
}

Vector3 Entity::getAimPunch()
{
	Vector3 aimPunch = Driver.ReadVirtualMemory<Vector3>(ProcessId, EntityAddress + m_aimPunchAngle, sizeof(Vector3));
	return aimPunch;
}

Vector3 Entity::getVelocity()
{
	Vector3 vel = Driver.ReadVirtualMemory<Vector3>(ProcessId, EntityAddress + m_vecVelocity, sizeof(Vector3));
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

Vector3 Entity::getHeadPosition()
{
	Vector3 Origin = Driver.ReadVirtualMemory<Vector3>(ProcessId, EntityAddress + m_vecOrigin, sizeof(Vector3));
	Vector3 ViewOffset = Driver.ReadVirtualMemory<Vector3>(ProcessId, EntityAddress + m_vecViewOffset, sizeof(Vector3));
	Vector3 LocalEyeOrigin = Origin + ViewOffset;

	return LocalEyeOrigin;
}

uint16_t Entity::getCrosshairId()
{
	return Driver.ReadVirtualMemory<uint16_t>(ProcessId, EntityAddress + m_iCrosshairId, sizeof(uint16_t));
}

uint8_t Entity::getForceAttack()
{
	return Driver.ReadVirtualMemory<uint8_t>(ProcessId, EntityAddress + dwForceAttack, sizeof(uint8_t));
}

void Entity::setForceAttack(uint8_t value)
{
	Driver.WriteVirtualMemory(ProcessId, EntityAddress + dwForceAttack, value, sizeof(value));
}

void Entity::setForceAttack2(uint8_t value)
{
	Driver.WriteVirtualMemory(ProcessId, EntityAddress + dwForceAttack2, value, sizeof(value));
}

void Entity::shoot()
{
	uint8_t forceAttack = getForceAttack();
	if (forceAttack == 4) 
	{
		setForceAttack(5);
	}
	else 
	{
		setForceAttack(4);
	}
}

uint16_t Entity::getShotsFired()
{
	return Driver.ReadVirtualMemory<uint16_t>(ProcessId, EntityAddress + m_iShotsFired, sizeof(uint16_t));
}

uint32_t Entity::getBase()
{
	return EntityAddress;
}

uint32_t Entity::GetGlowIndex()
{
	uint32_t GlowIndex = Driver.ReadVirtualMemory<uint32_t>(ProcessId, EntityAddress + m_iGlowIndex, sizeof(uint32_t));
    return GlowIndex;
}

void Entity::SetCorrectGlowStruct(uint8_t OurTeam, uint32_t GlowObject)
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


Entity::Entity(uint32_t EntityAddress)
{
    this->EntityAddress = EntityAddress;
}

Entity::Entity()
{
}

Entity::~Entity()
{

}