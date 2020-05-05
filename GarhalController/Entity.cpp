#include "Entity.hpp"

#include <iostream>

#include "offsets.hpp"

// hazedumper namespace
using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

int Entity::IsDormant()
{
	bool isDormant = Driver.ReadVirtualMemory<bool>(ProcessId, EntityAddress + m_bDormant, sizeof(int));
	//std::cout << "isDormant: " << isDormant << " " << EntityAddress << std::endl;
    return isDormant;
}

bool Entity::IsDefusing()
{
    bool Defusing = Driver.ReadVirtualMemory<bool>(ProcessId, EntityAddress + m_bIsDefusing, sizeof(int));
    return Defusing;
}

void Entity::SetFlashAlpha(float &num)
{
    // No Flash if set to 0
    Driver.WriteVirtualMemory(ProcessId, EntityAddress + m_flFlashMaxAlpha, num, 8);
}

int Entity::getTeam()
{
    int OurTeam = Driver.ReadVirtualMemory<int>(ProcessId, EntityAddress + m_iTeamNum, sizeof(int));
    return OurTeam;
}

int Entity::GetGlowIndex()
{
    int GlowIndex = Driver.ReadVirtualMemory<int>(ProcessId, EntityAddress + m_iGlowIndex, sizeof(int));
    return GlowIndex;
}

GlowStruct Entity::GetGlowStruct(int GlowObject)
{
    GlowStruct EGlow;
    EGlow = Driver.ReadVirtualMemory<GlowStruct>(ProcessId, GlowObject + (this->GetGlowIndex() * 0x38), sizeof(GlowStruct));
    EGlow.alpha = 0.5f;
    EGlow.renderWhenOccluded = true;
    EGlow.renderWhenUnOccluded = false;
	EGlow.red = 0.0f;
	EGlow.green = 0.0f;
	EGlow.blue = 0.0f;
	

    return EGlow;
}

void Entity::SetCorrectGlowStruct(int OurTeam, int GlowObject)
{
	int ReadTeam = this->getTeam();
	bool Defusing = this->IsDefusing();
	
	GlowStruct EGlow;
	EGlow = this->GetGlowStruct(GlowObject);
	EGlow.alpha = 0.5f;
	EGlow.renderWhenOccluded = true;
	EGlow.renderWhenUnOccluded = false;

	std::cout << "sas " << OurTeam << " " << ReadTeam << std::endl;

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