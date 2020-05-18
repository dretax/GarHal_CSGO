#pragma comment(lib,"ntdll.lib")
#pragma warning(disable : 26451) // Bug in VS according to Stackoverflow.

#include "kernelinterface.hpp"
#include "offsets.hpp"
#include "data.hpp"
#include <iostream>
#include <TlHelp32.h>
#include "Aimbot.hpp"
#include "AntiAim.hpp"
#include "BSPParser.hpp"
#include "ClientMode.h"
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
	ClientSize = Driver.GetClientModuleSize();
	EngineSize = Driver.GetEngineModuleSize();

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
	AimbotBullets = config.pInt("AimbotBullets");
	Bhop = config.pBool("Bhop");
	AntiAimS = config.pBool("AntiAim");

	std::cout << "GarHal made by DreTaX" << std::endl;

	std::cout << "==== Memory Addresses ====" << std::endl;
	std::cout << "ProcessID: " << ProcessId << std::endl;
	std::cout << "ClientAddress: " << ClientAddress << std::endl;
	std::cout << "EngineAddress: " << EngineAddress << std::endl;
	std::cout << "ClientSize: " << ClientSize << std::endl;

	// Get address of localplayer
	uint32_t LocalPlayer = 0;

	uint32_t GlowObject = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwGlowObjectManager, sizeof(uint32_t));

	std::cout << "GlowObject: " << GlowObject << std::endl;

	std::cout << "==== Config Values ====" << std::endl;
	std::cout << "AimbotS: " << unsigned(AimbotS) << std::endl;
	std::cout << "AimbotKey: " << unsigned(AimbotKey) << std::endl;
	std::cout << "AimbotTarget: " << unsigned(AimbotTarget) << std::endl;
	std::cout << "AimbotBullets: " << unsigned(AimbotBullets) << std::endl;
	std::cout << "AntiAim: " << AntiAimS << std::endl;
	std::cout << "Bhop: " << Bhop << std::endl;

	Aimbot aim = Aimbot(&bspParser);
	AntiAim antiaim = AntiAim();

	if (AntiAimS)
	{
		ClientMode* clientMode;
		clientMode = **(ClientMode***)((*(uintptr_t**)ClientAddress)[10] + 0x5);
		IClientMode = (DWORD**)clientMode;
		
		antiaim.Enable();
		antiaim.HookCreateMove();
		std::cout << "~AntiAim Create Move hooked!" << std::endl;
	}

	//std::cout << "IClientMode: " << IClientMode << std::endl;

	while (true)
	{
		if (!engine.IsInGame())
		{
			Sleep(500);
			continue;
		}

		if (GlowObject == 0)
		{
			GlowObject = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwGlowObjectManager, sizeof(uint32_t));
		}


		LocalPlayer = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwLocalPlayer, sizeof(uint32_t));
		Entity LocalPlayerEnt = Entity(LocalPlayer);

		aim.localPlayer = LocalPlayerEnt;
		
		uint8_t OurTeam = LocalPlayerEnt.getTeam();
		LocalPlayerEnt.SetFlashAlpha(0.0f);

		for (short int i = 0; i < 64; i++)
		{
			uint32_t EntityAddr = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwEntityList + i * 0x10, sizeof(uint32_t));

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
				}
			}
		}

		if (AntiAimS)
		{
			antiaim.DoAntiAim();
		}

		
		if (AimbotS == 1) 
		{
			if (GetAsyncKeyState(AimbotKey) & KEY_DOWN)
			{
				aim.aimAssist();
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
				aim.aimBot();
			}
		}

		Sleep(3);
	}

	return 0;
}
