#pragma comment(lib,"ntdll.lib")

#include "kernelinterface.hpp"
#include "offsets.hpp"
#include "data.hpp"
#include <iostream>
#include <TlHelp32.h>
#include "Engine.hpp"
#include "Entity.hpp"

using namespace std;

// hazedumper namespace
using namespace hazedumper::netvars;
using namespace hazedumper::signatures;


//DWORD ProcessId, ClientAddress, EngineAddress;
int GlowObject;

Entity CreateEntity(int Address)
{
	//int ent = Driver.ReadVirtualMemory<int>(ProcessId, Address, sizeof(int));
	if (Address > 0)
	{
		Entity entity(Address);
		entity.InValid = false;
		return entity;
	}
	
	Entity dummy;
	dummy.InValid = true;
	return dummy;
}


int main()
{
	Driver = KeInterface("\\\\.\\garhalop");
	SetConsoleTitle(L"GarHal is the best fish ever, by DreTaX");
	
	// Get address of client.dll, engine.dll, and PID.

	ProcessId = Driver.GetTargetPid();
	ClientAddress = Driver.GetClientModule();
	EngineAddress = Driver.GetEngineModule();

	int UseAimAssist = 0;
	int UseBhop = 0;

	if (ProcessId == 0)
	{
		std::cout << "ProcessID is 0. Start driver & restart. " << ProcessId << std::endl;
		cin >> UseBhop;
		return 0;
	}

	Engine engine;
	
	/*std::cout << "Use AimAssist? (Y: 1, N: Anything)" << std::endl;
	cin >> UseAimAssist;
	if (UseAimAssist != 1)
	{
		UseAimAssist = 0;
	}

	std::cout << "Use UseBhop? (Y: 1, N: Anything)" << std::endl;
	cin >> UseBhop;
	if (UseBhop != 1)
	{
		UseBhop = 0;
	}*/

	std::cout << "GarHal made by DreTaX" << std::endl;

	std::cout << "ProcessID: " << ProcessId << std::endl;
	std::cout << "ClientAddress: " << ClientAddress << std::endl;
	std::cout << "EngineAddress: " << EngineAddress << std::endl;


	// Get address of localplayer
	DWORD LocalPlayer = 0;

	GlowObject = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + dwGlowObjectManager, sizeof(int));

	std::cout << "GlowObject: " << GlowObject << std::endl;

	while (true)
	{
		/*if (!engine.IsInGame())
		{
			Sleep(500);
			continue;
		}*/

		/*LocalPlayer = Driver.ReadVirtualMemory<DWORD>(ProcessId, ClientAddress + dwLocalPlayer, sizeof(ULONG));
		int OurTeam = Driver.ReadVirtualMemory<int>(ProcessId, LocalPlayer + m_iTeamNum, sizeof(int));
		Driver.WriteVirtualMemory(ProcessId, LocalPlayer + m_flFlashMaxAlpha, 0.0f, 8);

		for (short int i = 0; i < 64; i++)
		{
			int EntityAddr = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + dwEntityList + i * 0x10, sizeof(int));

			if (EntityAddr == NULL)
			{
				continue;
			}

			Entity Entity = CreateEntity(EntityAddr);
			if (!Entity.InValid)
			{
				if (!Entity.IsDormant()) 
				{
					Entity.SetCorrectGlowStruct(OurTeam, GlowObject);
				}
			}
		}*/

		// No Flash
		Driver.WriteVirtualMemory(ProcessId, LocalPlayer + m_flFlashMaxAlpha, 0.0f, 8);

		int OurTeam = Driver.ReadVirtualMemory<int>(ProcessId, LocalPlayer + m_iTeamNum, sizeof(int));

		for (short int i = 0; i < 64; i++)
		{
			int Entity = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + dwEntityList + i * 0x10, sizeof(int));

			if (Entity != NULL) 
			{
				int ReadTeam = Driver.ReadVirtualMemory<int>(ProcessId, Entity + m_iTeamNum, sizeof(int));
				int GlowIndex = Driver.ReadVirtualMemory<int>(ProcessId, Entity + m_iGlowIndex, sizeof(int));

				int isDormant = Driver.ReadVirtualMemory<int>(ProcessId, Entity + m_bDormant, sizeof(int));

				if (!isDormant) 
				{
					bool Defusing = Driver.ReadVirtualMemory<bool>(ProcessId, Entity + m_bIsDefusing, sizeof(int));
					
					GlowStruct EGlow;
					EGlow = Driver.ReadVirtualMemory<GlowStruct>(ProcessId, GlowObject + (GlowIndex * 0x38), sizeof(GlowStruct));
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

					Driver.WriteVirtualMemory(ProcessId, GlowObject + (GlowIndex * 0x38), EGlow, sizeof(EGlow));
				}

			}
		}
		Sleep(3);
	}
	return 0;
}
