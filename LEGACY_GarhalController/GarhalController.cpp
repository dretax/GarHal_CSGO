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
#include <windows.h>

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

std::string GenerateStr(const int len)
{
	std::string str;
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	srand((unsigned)time(NULL) * _getpid());

	for (int i = 0; i < len; ++i) 
	{
		str += alphanum[rand() % (sizeof(alphanum) - 1)];
	}


	return str;
}

vector<string> Split(string s, string delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos) 
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}


// TODO: Implement https://guidedhacking.com/threads/external-silent-aim-proof-of-concept-no-shellcode.13595/
// https://www.unknowncheats.me/forum/counterstrike-global-offensive/144597-matchmaking-colors.html

// TODO: On close free all global values from the memory.
inline Engine engine;
inline Aimbot aim = NULL;

void TriggerBotThread()
{
	while (TriggerBot)
	{
		if (!engine.IsInGame())
		{
			Sleep(500);
			continue;
		}

		if (!aim.localPlayer.isValidPlayer())
		{
			continue;
		}

		//uint32_t LocalPlayer = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwLocalPlayer, sizeof(uint32_t));
		Entity LocalPlayerEnt = aim.localPlayer;
			
		if (TriggerBotKey == 0 || (GetAsyncKeyState(TriggerBotKey) & KEY_DOWN))
		{
			bool usable = false;
			uint16_t weaponid = LocalPlayerEnt.GetCurrentWeaponID();
			for (uint16_t wep : WeaponIDs)
			{
				if (wep == 0 || (weaponid > 0 && wep == weaponid))
				{
					usable = true;
					break;
				}
			}

			if (usable)
			{
				aim.TriggerBot();
			}
		}

		Sleep(2);
	}
}

int main(int argc, char* argv[], char* envp[])
{
	Driver = KeInterface("\\\\.\\garhalop");
	std::string random = GenerateStr(20);
	std::wstring stemp = std::wstring(random.begin(), random.end());
	LPCWSTR sw = stemp.c_str();
	SetConsoleTitle(sw);
	
	// Get address of client.dll, engine.dll, and PID.
	ProcessId = Driver.GetTargetPid();
	ClientAddress = Driver.GetClientModule();
	EngineAddress = Driver.GetEngineModule();
	ClientSize = Driver.GetClientModuleSize();
	EngineSize = Driver.GetEngineModuleSize();

	bool PrintOnce = false;

	while (ProcessId == 0 || ClientAddress == 0 || EngineAddress == 0 || ClientSize == 0 || EngineSize == 0)
	{
		if (!PrintOnce) 
		{
			std::cout << "Addresses are 0x0. Waiting for CSGO... " << std::endl;
			PrintOnce = true;
		}
		
		Sleep(1000);
		ProcessId = Driver.GetTargetPid();
		ClientAddress = Driver.GetClientModule();
		EngineAddress = Driver.GetEngineModule();
		ClientSize = Driver.GetClientModuleSize();
		EngineSize = Driver.GetEngineModuleSize();
	}

	std::cout << "Addresses look good. Starting..." << std::endl;

	hazedumper::BSPParser bspParser;

	// Read config values.
	Config config("garhal.cfg", envp);
	AimbotS = config.pInt("AimbotS");
	AimbotKey = config.pHex("AimbotKey");
	AimbotTarget = config.pInt("AimbotTarget");
	AimbotBullets = config.pInt("AimbotBullets");
	Bhop = config.pBool("Bhop");
	Wallhack = config.pBool("Wallhack");
	NoFlash = config.pBool("NoFlash");
	TriggerBot = config.pBool("TriggerBot");
	TriggerBotKey = config.pHex("TriggerBotKey");
	TriggerBotDelay = config.pBool("TriggerBotDelay");
	Radar = config.pBool("Radar");

	std::string weapons = config.pString("TriggerBotAllowed");
	vector<string> dweapons = Split(weapons, ",");
	for (string w : dweapons)
	{
		char cstr[10];
		strcpy_s(cstr, w.c_str());
		WeaponIDs.push_back(atoi(cstr));
	}

	std::string delays = config.pString("TriggerBotDelayNum");
	vector<string> ddelays = Split(delays, ",");
	for (uint8_t rand = 0; rand < ddelays.size() ; rand++)
	{
		char cstr[10];
		strcpy_s(cstr, ddelays.at(rand).c_str());
		if (rand == 0)
		{
			TriggerBotDelayMin = atoi(cstr);
		}
		else
		{
			TriggerBotDelayMax = atoi(cstr);
		}
	}

	std::cout << "GarHal made by DreTaX" << std::endl;

	std::cout << "==== Memory Addresses ====" << std::endl;
	std::cout << "ProcessID: " << ProcessId << std::endl;
	std::cout << "ClientAddress: " << std::hex << ClientAddress << std::endl;
	std::cout << "EngineAddress: " << std::hex << EngineAddress << std::endl;
	std::cout << "ClientSize: " << std::hex << ClientSize << std::endl;

	// Store address of localplayer
	uint32_t LocalPlayer = 0;

	uint32_t GlowObject = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwGlowObjectManager, sizeof(uint32_t));

	std::cout << "GlowObject: " << GlowObject << std::endl;

	std::cout << "==== Config Values ====" << std::endl;
	std::cout << "AimbotS: " << unsigned(AimbotS) << std::endl;
	std::cout << "AimbotKey: " << unsigned(AimbotKey) << std::endl;
	std::cout << "AimbotTarget: " << unsigned(AimbotTarget) << std::endl;
	std::cout << "AimbotBullets: " << unsigned(AimbotBullets) << std::endl;
	std::cout << "Bhop: " << std::boolalpha << Bhop << std::endl;
	std::cout << "Wallhack: " << std::boolalpha << Wallhack << std::endl;
	std::cout << "NoFlash: " << std::boolalpha << NoFlash << std::endl;
	std::cout << "TriggerBot: " << std::boolalpha << TriggerBot << std::endl;
	std::cout << "TriggerBotKey: " << unsigned(TriggerBotKey) << std::endl;
	std::cout << "TriggerBotDelay: " << std::boolalpha << TriggerBotDelay << std::endl;
	std::cout << "Radar: " << std::boolalpha << Radar << std::endl;

	aim = Aimbot(&bspParser);

	std::thread TriggerBotT(TriggerBotThread);

	Driver.RequestProtection();

	while (true)
	{
		if (!engine.IsInGame())
		{
			Sleep(500);
			continue;
		}

		if (Wallhack)
		{
			GlowObject = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwGlowObjectManager, sizeof(uint32_t));
		}

		LocalPlayer = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwLocalPlayer, sizeof(uint32_t));
		Entity LocalPlayerEnt = Entity(LocalPlayer);

		if (aim.localPlayer.GetEntityAddress() != LocalPlayerEnt.GetEntityAddress())
		{
			aim.localPlayer = LocalPlayerEnt;
		}
		
		uint8_t OurTeam = LocalPlayerEnt.getTeam();
		if (NoFlash) 
		{
			LocalPlayerEnt.SetFlashAlpha(0.0f);
		}

		for (short int i = 0; i < 64; i++)
		{
			uint32_t EntityAddr = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwEntityList + i * 0x10, sizeof(uint32_t));

			if (EntityAddr == NULL)
			{
				continue;
			}
			
			if (Wallhack)
			{
				Entity ent = CreateEntity(EntityAddr);
				if (ent.isValidPlayer() && !ent.IsDormant())
				{
					ent.SetCorrectGlowStruct(OurTeam, GlowObject);
				}
			}
			
			if (Radar)
			{
				Entity ent = CreateEntity(EntityAddr);
				if (ent.isValidPlayer() && !ent.IsDormant()) 
				{
					Driver.WriteVirtualMemory<bool>(ProcessId, EntityAddr + m_bSpotted, true, sizeof(bool));
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