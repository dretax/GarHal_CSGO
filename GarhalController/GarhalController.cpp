#pragma comment(lib,"ntdll.lib")

#include "kernelinterface.hpp"
#include "offsets.hpp"
#include "data.hpp"
#include <iostream>
#include <TlHelp32.h>

using namespace std;

// hazedumper namespace
using namespace hazedumper::netvars;
using namespace hazedumper::signatures;


DWORD ProcessId, ClientAddress;
int GlowObject;


int main()
{
	KeInterface Driver("\\\\.\\garhalop");
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

					if (OurTeam != ReadTeam) 
					{
						GlowStruct EGlow;
						EGlow = Driver.ReadVirtualMemory<GlowStruct>(ProcessId, GlowObject + (GlowIndex * 0x38), sizeof(GlowStruct));
						EGlow.red = 255.0f;
						EGlow.green = 0.0f;
						EGlow.blue = 0.0f;
						EGlow.alpha = 0.5f;
						EGlow.renderWhenOccluded = true;
						EGlow.renderWhenUnOccluded = false;
						Driver.WriteVirtualMemory(ProcessId, GlowObject + (GlowIndex * 0x38), EGlow, sizeof(EGlow));
					}
					else
					{
						GlowStruct TGlow;
						TGlow = Driver.ReadVirtualMemory<GlowStruct>(ProcessId, GlowObject + (GlowIndex * 0x38), sizeof(GlowStruct));
						TGlow.red = 0.0f;
						TGlow.green = 0.0f;
						TGlow.blue = 255.0f;
						TGlow.alpha = 0.5f;
						TGlow.renderWhenOccluded = true;
						TGlow.renderWhenUnOccluded = false;
						Driver.WriteVirtualMemory(ProcessId, GlowObject + (GlowIndex * 0x38), TGlow, sizeof(TGlow));
					}
				}

			}
		}
		Sleep(3);
	}
	return 0;
}
