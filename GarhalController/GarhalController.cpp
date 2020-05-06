#pragma comment(lib,"ntdll.lib")

#include "kernelinterface.hpp"
#include "offsets.hpp"
#include "data.hpp"
#include <iostream>
#include <TlHelp32.h>

#include "Aimbot.hpp"
#include "BSPParser.hpp"
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
	if (Address > 0)
	{
		Entity entity(Address);
		return entity;
	}
	
	Entity dummy(0);
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
	hazedumper::BSPParser bspParser;
	
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
	int LocalPlayer = 0;

	GlowObject = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + dwGlowObjectManager, sizeof(int));

	std::cout << "GlowObject: " << GlowObject << std::endl;

	Aimbot aim;
	const float FOV_RANGE = 10.f;

	while (true)
	{
		if (!engine.IsInGame())
		{
			Sleep(500);
			continue;
		}

		LocalPlayer = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + dwLocalPlayer, sizeof(int));
		Entity LocalPlayerEnt = Entity(LocalPlayer);
		
		int OurTeam = LocalPlayerEnt.getTeam();
		LocalPlayerEnt.SetFlashAlpha(0.0f);

		/*if (GetAsyncKeyState(VK_SPACE) & KEY_DOWN) 
		{
			if (LocalPlayerEnt.getHealth() > 0)
			{
				if (!LocalPlayerEnt.isInAir())
				{
					LocalPlayerEnt.SetForceJump(5);
				}
				else
				{
					LocalPlayerEnt.SetForceJump(4);
					LocalPlayerEnt.SetForceJump(5);
					LocalPlayerEnt.SetForceJump(4);
				}
			}
		}*/

		
		for (short int i = 0; i < 64; i++)
		{
			int EntityAddr = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + dwEntityList + i * 0x10, sizeof(int));

			if (EntityAddr == NULL)
			{
				continue;
			}

			Entity ent = CreateEntity(EntityAddr);
			if (!ent.InValid)
			{
				if (!ent.IsDormant())
				{
					ent.SetCorrectGlowStruct(OurTeam, GlowObject);
				}
			}
		}

		Sleep(3);
	}
	return 0;
}
