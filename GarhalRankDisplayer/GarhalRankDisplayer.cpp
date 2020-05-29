
#include <iostream>
#include "data.hpp"
#include "offsets.hpp"

// hazedumper namespace
using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

class player_info {
private:
	char __pad[0x10];
public:
	char name[32];
};

int main()
{
	Driver = KeInterface("\\\\.\\garhalop");
	SetConsoleTitle(L"GarHal is the best rank reader fish ever, by DreTaX");

	// Get address of client.dll, engine.dll, and PID.
	ProcessId = Driver.GetTargetPid();
	ClientAddress = Driver.GetClientModule();
	EngineAddress = Driver.GetEngineModule();

	std::cout << "GarHal made by DreTaX" << std::endl;

	if (ProcessId == 0 || ClientAddress == 0)
	{
		std::cout << "Addresses are 0. Start driver & Start CSGO & restart. " << ProcessId << std::endl;
		system("pause");
		return 0;
	}

	std::cout << "Starting..." << std::endl;
	uint32_t clientState = Driver.ReadVirtualMemory<uint32_t>(ProcessId, EngineAddress + dwClientState, sizeof(uint32_t));
	uint32_t infotable = Driver.ReadVirtualMemory<uint32_t>(ProcessId, clientState + dwClientState_PlayerInfo, sizeof(uint32_t));
	uint32_t items = Driver.ReadVirtualMemory<uint32_t>(ProcessId, (infotable + 0x40) + 0xC, sizeof(uint32_t));

	uint32_t PlayerResource = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwPlayerResource, sizeof(uint32_t));
	for (short int i = 0; i < 10; i++)
	{
		uint32_t EntityAddr = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwEntityList + i * 0x10, sizeof(uint32_t));

		if (EntityAddr == NULL)
		{
			continue;
		}
		
		uint8_t Rank = Driver.ReadVirtualMemory<uint8_t>(ProcessId, PlayerResource + m_iCompetitiveRanking + (i * 0x04), sizeof(uint8_t));
		uint16_t Wins = Driver.ReadVirtualMemory<uint16_t>(ProcessId, PlayerResource + m_iCompetitiveWins + (i * 0x04), sizeof(uint16_t));
		//uint32_t RadarBase = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwRadarBase, sizeof(uint32_t));
		//uint32_t Radar = Driver.ReadVirtualMemory<uint32_t>(ProcessId, RadarBase + 0x54, sizeof(uint32_t));

		//wchar_t cName = Driver.ReadVirtualMemory<wchar_t>(ProcessId, Radar + 0x300 + (0x174 * (i - 1)), 32);

		uint32_t asd = Driver.ReadVirtualMemory<uint32_t>(ProcessId, items + 0x28 + i * 0x34, sizeof(uint32_t));
		player_info playerinfo = Driver.ReadVirtualMemory<player_info>(ProcessId, asd, sizeof(player_info));

		
		char* player_name = playerinfo.name;
		
		const char* RankName = Ranks[Rank];

		std::cout << "===Player[" << i << "]===" << std::endl;
		std::cout << player_name << std::endl;
		std::cout << RankName << std::endl;
		std::cout << "Wins: " << Wins << std::endl;
		std::cout << std::endl;
	}

	system("pause");
	return 0;
}
