#include "Engine.hpp"
#include "offsets.hpp"
#include "imgui.h"

// hazedumper namespace
using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

bool Engine::worldToScreen(const Vector3& from, Vector3& to)
{
    WorldToScreenMatrix matrix = Driver.ReadVirtualMemory<WorldToScreenMatrix>(ProcessId, ClientAddress + dwViewMatrix, sizeof(WorldToScreenMatrix));

    const auto w = matrix(3, 0) * from(0) + matrix(3, 1) * from(1) + matrix(3, 2) * from(2) + matrix(3, 3);
    if (w < 0.001f)
        return false;

    to(0) = ImGui::GetIO().DisplaySize.x / 2.0f;
    to(1) = ImGui::GetIO().DisplaySize.y / 2.0f;
    to(0) *= 1.0f + (matrix(0, 0) * from(0) + matrix(0, 1) * from(1) + matrix(0, 2) * from(2) + matrix(0, 3)) / w;
    to(1) *= 1.0f - (matrix(1, 0) * from(0) + matrix(1, 1) * from(1) + matrix(1, 2) * from(2) + matrix(1, 3)) / w;

    return true;
}

bool Engine::IsInGame()
{
    uint32_t ClientState = Driver.ReadVirtualMemory<uint32_t>(ProcessId, EngineAddress + dwClientState, sizeof(uint32_t));
    uint32_t Second = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientState + dwClientState_State, sizeof(uint32_t));
    return GetGameState(Second) == InGame;
}

GameState Engine::GetGameState(uint8_t State)
{
	switch (State)
    {
		case 0: return Lobby;
        case 1: return Loading;
        case 2: return Connecting;
        case 5: return Connected;
        case 6: return InGame;
    }
    return UnknownG;
}

Vector3 Engine::getViewAngles()
{
    uint32_t ClientState = Driver.ReadVirtualMemory<uint32_t>(ProcessId, EngineAddress + dwClientState, sizeof(uint32_t));
    return Driver.ReadVirtualMemory<Vector3>(ProcessId, ClientState + dwClientState_ViewAngles, sizeof(Vector3));
}

void Engine::setViewAngles(Vector3& viewAngles)
{
    uint32_t clientState = Driver.ReadVirtualMemory<uint32_t>(ProcessId, EngineAddress + dwClientState, sizeof(uint32_t));
    Driver.WriteVirtualMemory<Vector3>(ProcessId, clientState + dwClientState_ViewAngles, viewAngles, sizeof(viewAngles));
}


Engine::Engine()
{
    
}


Engine::~Engine()
{
}