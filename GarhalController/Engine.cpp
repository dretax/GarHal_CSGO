#include "Engine.hpp"
#include "offsets.hpp"

// hazedumper namespace
using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

bool Engine::worldToScreen(const Vector3& from, Vector3& to)
{
    WorldToScreenMatrix matrix = Driver.ReadVirtualMemory<WorldToScreenMatrix>(ProcessId, ClientAddress + dwViewMatrix, sizeof(WorldToScreenMatrix));

    float w = 0.0f;

    to(0) = matrix(0, 0) * from(0) + matrix(0, 1) * from(1) + matrix(0, 2) * from(2) + matrix(0, 3);
    to(1) = matrix(1, 0) * from(0) + matrix(1, 1) * from(1) + matrix(1, 2) * from(2) + matrix(1, 3);
    w = matrix(3, 0) * from(0) + matrix(3, 1) * from(1) + matrix(3, 2) * from(2) + matrix(3, 3);

    if (w < 0.01f)
        return false;

    float wInverse = 1.0f / w;
    to(0) *= wInverse;
    to(1) *= wInverse;

    float x = SCREEN_WIDTH / 2;
    float y = SCREEN_HEIGHT / 2;

    x += 0.5 * to(0) * SCREEN_WIDTH + 0.5;
    y -= 0.5 * to(1) * SCREEN_HEIGHT + 0.5;

    to(0) = x;
    to(1) = y;

    return true;
}

bool Engine::IsInGame()
{
    uint32_t ClientState = Driver.ReadVirtualMemory<uint32_t>(ProcessId, EngineAddress + dwClientState, sizeof(uint32_t));
    uint32_t Second = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientState + dwClientState_State, sizeof(uint32_t));
    return Second == 6;
}

Vector3 Engine::getViewAngles()
{
    int ClientState = Driver.ReadVirtualMemory<int>(ProcessId, EngineAddress + dwClientState, sizeof(int));
    return Driver.ReadVirtualMemory<Vector3>(ProcessId, ClientState + dwClientState_ViewAngles, sizeof(Vector3));
}

void Engine::setViewAngles(Vector3& viewAngles)
{
    int clientState = Driver.ReadVirtualMemory<int>(ProcessId, EngineAddress + dwClientState, sizeof(int));
    Driver.WriteVirtualMemory<Vector3>(ProcessId, clientState + dwClientState_ViewAngles, viewAngles, sizeof(viewAngles));
}


Engine::Engine()
{
    
}


Engine::~Engine()
{
}