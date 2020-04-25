#pragma comment(lib,"ntdll.lib")

#include "kernelinterface.h"
#include "offsets.h"

#include <iostream>
#include <map>
#include <vector>
#include <TlHelp32.h>
#include <algorithm>

using namespace std;

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

	float zero = 0.f;
	float one = 1.0f;
	ULONG ulongzero = *((unsigned long*)&zero); // Represents color off.
	ULONG ulongone = *((unsigned long*)&one); // Represents color on.


	while (true)
	{
		if (ProcessId == 0)
		{
			continue;
		}


		LocalPlayer = Driver.ReadVirtualMemory<DWORD>(ProcessId, ClientAddress + LocalPlayer_Offset, sizeof(ULONG));
		int myteam = Driver.ReadVirtualMemory<int>(ProcessId, LocalPlayer + TeamNumber_Offset, sizeof(int));

		// No Flash

		Driver.WriteVirtualMemory(ProcessId, LocalPlayer + FlashMaxAlpha_Offset, ulongzero, 8);

		DWORD glowObjectManager = Driver.ReadVirtualMemory<DWORD>(ProcessId, ClientAddress + dwGlowObjectManager_Offset, sizeof(ULONG));
		int glowObjectCount = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + dwGlowObjectManager_Offset + 0x4, sizeof(int));


		if (glowObjectCount > 0)
		{

			for (int i = 0; i < glowObjectCount; i++)
			{
				int currentPlayer = Driver.ReadVirtualMemory<int>(ProcessId, ClientAddress + dwEntityList_Offset + (i * 0x10), sizeof(int));
				if (currentPlayer > 0)
				{
					int life = Driver.ReadVirtualMemory<int>(ProcessId, currentPlayer + LifeState_Offset, sizeof(int));
					if (life != 0)
					{
						//Sleep(5);
						continue;
					}

					int GlowIndex = Driver.ReadVirtualMemory<int>(ProcessId, currentPlayer + GlowIndex_Offset, sizeof(int));
					int team = Driver.ReadVirtualMemory<int>(ProcessId, currentPlayer + TeamNumber_Offset, sizeof(int));

					if (team == 0)
					{
						//Sleep(5);
						continue;
					}

					int GlowAddress = glowObjectManager + (GlowIndex * 56);

					// If our team doesn't match show red
					if (myteam != team)
					{
						Driver.WriteVirtualMemory(ProcessId, GlowAddress + 0x4, ulongone, sizeof(float));
						Driver.WriteVirtualMemory(ProcessId, GlowAddress + 0x8, ulongzero, sizeof(float));
						Driver.WriteVirtualMemory(ProcessId, GlowAddress + 0xC, ulongzero, sizeof(float));
						Driver.WriteVirtualMemory(ProcessId, GlowAddress + 0x10, ulongone, sizeof(float));
						Driver.WriteVirtualMemory(ProcessId, GlowAddress + 0x24, true, sizeof(BOOL));
						Driver.WriteVirtualMemory(ProcessId, GlowAddress + 0x25, false, sizeof(BOOL));
					}
					else
					{
						Driver.WriteVirtualMemory(ProcessId, GlowAddress + 0x4, ulongzero, sizeof(float));
						Driver.WriteVirtualMemory(ProcessId, GlowAddress + 0x8, ulongone, sizeof(ULONG));
						Driver.WriteVirtualMemory(ProcessId, GlowAddress + 0xC, ulongzero, sizeof(float));
						Driver.WriteVirtualMemory(ProcessId, GlowAddress + 0x10, ulongone, sizeof(float));
						Driver.WriteVirtualMemory(ProcessId, GlowAddress + 0x24, true, sizeof(BOOL));
						Driver.WriteVirtualMemory(ProcessId, GlowAddress + 0x25, false, sizeof(BOOL));
					}
				}
			}
		}

		Sleep(10);
	}
	return 0;
}
