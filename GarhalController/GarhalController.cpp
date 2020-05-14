#pragma comment(lib,"ntdll.lib")
#pragma warning(disable : 26451) // Bug in VS according to Stackoverflow.

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
	AntiAim = config.pBool("AntiAim");

	std::cout << "GarHal made by DreTaX" << std::endl;

	std::cout << "==== Memory Addresses ====" << std::endl;
	std::cout << "ProcessID: " << ProcessId << std::endl;
	std::cout << "ClientAddress: " << ClientAddress << std::endl;
	std::cout << "EngineAddress: " << EngineAddress << std::endl;

	// Get address of localplayer
	uint32_t LocalPlayer = 0;

	uint32_t GlowObject = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwGlowObjectManager, sizeof(uint32_t));

	std::cout << "GlowObject: " << GlowObject << std::endl;

	std::cout << "==== Config Values ====" << std::endl;
	std::cout << "AimbotS: " << unsigned(AimbotS) << std::endl;
	std::cout << "AimbotKey: " << unsigned(AimbotKey) << std::endl;
	std::cout << "AimbotTarget: " << unsigned(AimbotTarget) << std::endl;
	std::cout << "AimbotBullets: " << unsigned(AimbotBullets) << std::endl;
	std::cout << "AntiAim: " << AntiAim << std::endl;
	std::cout << "Bhop: " << Bhop << std::endl;

	const int8_t four = 4;
	const int8_t three = 3;

	Aimbot aim = Aimbot(&bspParser);

	while (true)
	{
		if (!engine.IsInGame())
		{
			Sleep(500);
			continue;
		}


		LocalPlayer = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwLocalPlayer, sizeof(uint32_t));
		Entity LocalPlayerEnt = Entity(LocalPlayer);

		aim.localPlayer = LocalPlayerEnt;
		
		uint8_t OurTeam = LocalPlayerEnt.getTeam();
		LocalPlayerEnt.SetFlashAlpha(0.0f);

		for (short int i = 0; i < 64; i++)
		{
			uint32_t EntityAddr = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwEntityList + i * 0x10, sizeof(int));

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

		if (AntiAim)
		{
			// W
			if (GetAsyncKeyState(0x57))
			{
				Driver.WriteVirtualMemory(ProcessId, ClientAddress + dwForceForward, four, sizeof(four));
				Driver.WriteVirtualMemory(ProcessId, ClientAddress + dwForceBackward, three, sizeof(three));
			}
			else 
			{
				Driver.WriteVirtualMemory(ProcessId, ClientAddress + dwForceBackward, four, sizeof(four));
			}

			// S
			if (GetAsyncKeyState(0x53))
			{
				Driver.WriteVirtualMemory(ProcessId, ClientAddress + dwForceBackward, four, sizeof(four));
				Driver.WriteVirtualMemory(ProcessId, ClientAddress + dwForceForward, three, sizeof(three));
			}
			else
			{
				Driver.WriteVirtualMemory(ProcessId, ClientAddress + dwForceForward, four, sizeof(four));
			}

			// A
			if (GetAsyncKeyState(0x41))
			{
				Driver.WriteVirtualMemory(ProcessId, ClientAddress + dwForceLeft, four, sizeof(four));
				Driver.WriteVirtualMemory(ProcessId, ClientAddress + dwForceRight, three, sizeof(three));
			}
			else
			{
				Driver.WriteVirtualMemory(ProcessId, ClientAddress + dwForceRight, four, sizeof(four));
			}

			// D
			if (GetAsyncKeyState(0x44))
			{
				Driver.WriteVirtualMemory(ProcessId, ClientAddress + dwForceRight, four, sizeof(four));
				Driver.WriteVirtualMemory(ProcessId, ClientAddress + dwForceLeft, three, sizeof(three));
			}
			else
			{
				Driver.WriteVirtualMemory(ProcessId, ClientAddress + dwForceLeft, four, sizeof(four));
			}
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
