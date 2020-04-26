#pragma comment(lib,"ntdll.lib")

#include "kernelinterface.h"
#include "offsets.hpp"

#include <iostream>
#include <map>
#include <vector>
#include <TlHelp32.h>
#include <algorithm>

//glow struct -- maybe move to a more organized .h ?
struct GlowStruct
{
	BYTE base[4];
	float red;
	float green;
	float blue;
	float alpha;
	BYTE buffer[16];
	bool renderWhenOccluded;
	bool renderWhenUnOccluded;
	bool fullBloom;
	BYTE buffer1[5];
	int glowStyle;
};

using namespace std;

// hazedumper namespace
using namespace hazedumper::netvars;
using namespace hazedumper::signatures;


KeInterface Driver("\\\\.\\garhalop");

DWORD ProcessId, ClientAddress;
int GlowObject;

void SetEnemyGlow(int glowIndex)
{
	GlowStruct EGlow;
	EGlow = Driver.ReadVirtualMemory<GlowStruct>(ProcessId, GlowObject + (glowIndex * 0x38), sizeof(GlowStruct));
	EGlow.red = 255.0f;
	EGlow.green = 0.0f;
	EGlow.blue = 0.0f;
	EGlow.alpha = 0.5f;
	EGlow.renderWhenOccluded = true;
	EGlow.renderWhenUnOccluded = false;
	Driver.WriteVirtualMemory(ProcessId, GlowObject + (glowIndex * 0x38), EGlow, sizeof(EGlow));
}

void SetTeamGlow(int glowIndex)
{
	GlowStruct TGlow;
	TGlow = Driver.ReadVirtualMemory<GlowStruct>(ProcessId, GlowObject + (glowIndex * 0x38), sizeof(GlowStruct));
	TGlow.red = 0.0f;
	TGlow.green = 0.0f;
	TGlow.blue = 255.0f;
	TGlow.alpha = 0.5f;
	TGlow.renderWhenOccluded = true;
	TGlow.renderWhenUnOccluded = false;
	Driver.WriteVirtualMemory(ProcessId, GlowObject + (glowIndex * 0x38), TGlow, sizeof(TGlow));
}

int main()
{

	SetConsoleTitle(L"GarHal is the best fish ever");
	
	// Get address of client.dll & pid of csgo from our driver
	DWORD ProcessId = Driver.GetTargetPid();
	DWORD ClientAddress = Driver.GetClientModule();

	std::cout << "GarHal made by DreTaX" << std::endl;

	std::cout << "ProcessID: " << ProcessId << std::endl;
	std::cout << "ClientAddress: " << ClientAddress << std::endl;


	// Get address of localplayer
	DWORD LocalPlayer = 0;

	GlowObject = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + dwGlowObjectManager, sizeof(int));

	while (true)
	{
		if (ProcessId == 0)
		{
			continue;
		}


		LocalPlayer = Driver.ReadVirtualMemory<DWORD>(ProcessId, ClientAddress + dwLocalPlayer, sizeof(ULONG));

		// No Flash

		Driver.WriteVirtualMemory(ProcessId, LocalPlayer + m_flFlashMaxAlpha, 0.0f, 8);

		//

		int OurTeam = Driver.ReadVirtualMemory<int>(ProcessId, LocalPlayer + m_iTeamNum, sizeof(int));

		for (short int i = 0; i < 64; i++)
		{
			int Entity = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + dwEntityList + i * 0x10, sizeof(int));

			if (Entity != NULL) {

				int ReadTeam = Driver.ReadVirtualMemory<int>(ProcessId, Entity + m_iTeamNum, sizeof(int));
				int GlowIndex = Driver.ReadVirtualMemory<int>(ProcessId, Entity + m_iGlowIndex, sizeof(int));

				int isDormant = Driver.ReadVirtualMemory<int>(ProcessId, Entity + m_bDormant, sizeof(int));

				if (!isDormant) {

					if (OurTeam != ReadTeam)
						SetEnemyGlow(GlowIndex);
					else
						SetTeamGlow(GlowIndex);
				}

			}

		}
		Sleep(3);
	}
	return 0;
}
