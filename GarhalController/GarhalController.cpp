#pragma comment(lib,"ntdll.lib")

#include "kernelinterface.hpp"
#include "offsets.hpp"
#include "data.hpp"
#include <iostream>
#include <TlHelp32.h>
#include "Aimbot.hpp"
#include "BSPParser.hpp"
#include "config.hpp"
#include "Engine.hpp"
#include "Entity.hpp"

using namespace std;

// hazedumper namespace
using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

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


int main(int argc, char* argv[], char* envp[])
{
	Driver = KeInterface("\\\\.\\garhalop");
	SetConsoleTitle(L"GarHal is the best fish ever, by DreTaX");
	
	// Get address of client.dll, engine.dll, and PID.
	ProcessId = Driver.GetTargetPid();
	ClientAddress = Driver.GetClientModule();
	EngineAddress = Driver.GetEngineModule();

	// Store the config values here.
	uint32_t AimbotS = 0;
	uint32_t AimbotKey = 0;
	uint32_t AimbotTarget = 0;
	bool Bhop = 0;

	if (ProcessId == 0 || ClientAddress == 0 || EngineAddress == 0)
	{
		std::cout << "Addresses are 0. Start driver & Start CSGO & restart. " << ProcessId << std::endl;
		system("pause");
		return 0;
	}

	Engine engine;
	hazedumper::BSPParser bspParser;

	// Read config values.
	Config config("garhal.cfg", envp);
	AimbotS = config.pInt("AimbotS");
	AimbotKey = config.pHex("AimbotKey");
	AimbotTarget = config.pInt("AimbotTarget");
	Bhop = config.pBool("Bhop");

	std::cout << "GarHal made by DreTaX" << std::endl;

	std::cout << "==== Memory Addresses ====" << std::endl;
	std::cout << "ProcessID: " << ProcessId << std::endl;
	std::cout << "ClientAddress: " << ClientAddress << std::endl;
	std::cout << "EngineAddress: " << EngineAddress << std::endl;

	// Get address of localplayer
	int LocalPlayer = 0;

	int GlowObject = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + dwGlowObjectManager, sizeof(int));

	std::cout << "GlowObject: " << GlowObject << std::endl;

	std::cout << "==== Config Values ====" << std::endl;
	std::cout << "AimbotS: " << AimbotS << std::endl;
	std::cout << "AimbotKey: " << AimbotKey << std::endl;
	std::cout << "AimbotTarget: " << AimbotTarget << std::endl;
	std::cout << "Bhop: " << Bhop << std::endl;

	Aimbot aim = Aimbot(&bspParser);

	DWORD part = CHEST_BONE_ID;
	if (AimbotTarget == 1)
	{
		part = HEAD_BONE_ID;
	}

	while (true)
	{
		if (!engine.IsInGame())
		{
			Sleep(500);
			continue;
		}


		LocalPlayer = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + dwLocalPlayer, sizeof(int));
		Entity LocalPlayerEnt = Entity(LocalPlayer);

		aim.localPlayer = LocalPlayerEnt;
		
		int OurTeam = LocalPlayerEnt.getTeam();
		LocalPlayerEnt.SetFlashAlpha(0.0f);

		for (short int i = 0; i < 64; i++)
		{
			int EntityAddr = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + dwEntityList + i * 0x10, sizeof(int));

			if (EntityAddr == NULL)
			{
				continue;
			}

			Entity ent = CreateEntity(EntityAddr);
			if (ent.isValidPlayer())
			{
				if (!ent.IsDormant())
				{
					ent.SetCorrectGlowStruct(OurTeam, GlowObject);
				}
			}
		}


		if (Bhop) 
		{
			if (GetAsyncKeyState(VK_SPACE) & KEY_DOWN)
			{
				if (LocalPlayerEnt.isValidPlayer())
				{
					if (!LocalPlayerEnt.isInAir())
					{
						LocalPlayerEnt.SetForceJump(6);
					}
					else
					{
						//LocalPlayerEnt.SetForceJump(4);
						//LocalPlayerEnt.SetForceJump(5);
						//LocalPlayerEnt.SetForceJump(4);
					}
				}
			}
		}

		
		if (AimbotS == 1) 
		{
			if (GetAsyncKeyState(AimbotKey) & KEY_DOWN)
			{
				aim.aimAssist(part);
			}
			else
			{
				aim.resetSensitivity();
			}
		}
		else if (AimbotS == 2)
		{
			if (GetAsyncKeyState(AimbotKey) & KEY_DOWN)
			{
				aim.aimBot(part);
			}
		}

		Sleep(3);
	}
	return 0;
}
