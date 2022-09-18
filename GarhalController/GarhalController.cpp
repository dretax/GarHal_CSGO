#pragma comment(lib,"ntdll.lib")
#pragma warning(disable : 26451) // Bug in VS according to Stackoverflow.

#include "kernelinterface.hpp"
#include "offsets.hpp"
#include "data.hpp"
#include <iostream>
#include <TlHelp32.h>
#include "Aimbot.hpp"
#include "..\common\bsp\BSPParser.hpp"
#include "engine.hpp"
#include "Entity.hpp"
#include <Windows.h>
#include "csgo_menu.hpp"
#include "csgo_settings.hpp"
#include "imgui_extensions.h"
#include "utils.hpp"

// hazedumper namespace
using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

// Declarations
inline Aimbot aim = NULL;
DWORD ProcessId, ClientAddress, ClientSize, EngineAddress, EngineSize;
KeInterface Driver = NULL;

[[noreturn]]
void TriggerBotThread()
{
    while (true)
    {
        if (!csgo_settings::TriggerBot)
        {
            Sleep(100);
            continue;
        }

        if (!engine::IsInGame())
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

        if (csgo_settings::TriggerBotKey == 0 || ImGui::IsCustomKeyPressed(csgo_settings::TriggerBotKey, true))
        {
            bool usable = false;
            uint16_t weaponid = LocalPlayerEnt.GetCurrentWeaponID();
            const int size = *(&csgo_settings::selectedWeaponIds + 1) - csgo_settings::selectedWeaponIds;
            for (int i(0); i < size; ++i)
            {
                std::string weaponName = csgo_settings::weaponIdHelper[i];
                int matchingWeaponId = csgo_settings::weaponIds[weaponName];
                // This index is selected in the combo &
                if (csgo_settings::selectedWeaponIds[i] && weaponid == matchingWeaponId)
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
    if (!utils::IsProcessElevated(GetCurrentProcess()))
    {
        std::cout << "Process is not elevated!" << std::endl;
        MessageBoxA(0, "Process is not elevated!", "Garhal", MB_OK | MB_ICONWARNING);
        return 0x1;
    }

    Driver = KeInterface("\\\\.\\garhalop");
    std::string random = utils::GenerateStr(20);
    SetConsoleTitleA(random.c_str());

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

    // Initialize our renderer
    CSGORender csgoRender;
    if (!csgoRender.createWindow())
    {
        std::cout << "Failed to create window!" << std::endl;
        MessageBoxA(0, "Failed to create window!", "Garhal", MB_OK | MB_ICONWARNING);
        return 0x1;
    }

    // Prep parser
    hazedumper::BSPParser bspParser;

    std::cout << "GarHal made by DreTaX" << std::endl;
    std::cout << "==== Memory Addresses ====" << std::endl;
    std::cout << "ProcessID: " << ProcessId << std::endl;
    std::cout << "ClientAddress: " << std::hex << ClientAddress << std::endl;
    std::cout << "EngineAddress: " << std::hex << EngineAddress << std::endl;
    std::cout << "ClientSize: " << std::hex << ClientSize << std::endl;

    // Old glow is now flagged.
    //uint32_t GlowObject = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwGlowObjectManager, sizeof(uint32_t));
    //std::cout << "GlowObject: " << GlowObject << std::endl;

    std::cout << "==== Config Values ====" << std::endl;
    std::cout << "AimbotState: " << static_cast<unsigned>(csgo_settings::AimbotState) << std::endl;
    std::cout << "AimbotKey: " << static_cast<unsigned>(csgo_settings::AimbotKey) << std::endl;
    std::cout << "AimbotTarget: " << static_cast<unsigned>(csgo_settings::AimbotTarget) << std::endl;
    std::cout << "AimbotBullets: " << static_cast<unsigned>(csgo_settings::AimbotBullets) << std::endl;
    std::cout << "Bhop: " << std::boolalpha << csgo_settings::Bhop << std::endl;
    std::cout << "Wallhack: " << std::boolalpha << csgo_settings::Wallhack << std::endl;
    std::cout << "NoFlash: " << std::boolalpha << csgo_settings::NoFlash << std::endl;
    std::cout << "TriggerBot: " << std::boolalpha << csgo_settings::TriggerBot << std::endl;
    std::cout << "TriggerBotKey: " << static_cast<unsigned>(csgo_settings::TriggerBotKey) << std::endl;
    std::cout << "TriggerBotDelay: " << std::boolalpha << csgo_settings::TriggerBotDelay << std::endl;
    std::cout << "Radar: " << std::boolalpha << csgo_settings::Radar << std::endl;

    aim = Aimbot(&bspParser);

    std::thread TriggerBotT(TriggerBotThread);

    Driver.RequestProtection();

    // We are creating a renderData list where you can dump all the informations you wish to render
    // Note that we are currently only using this to render bones, It is up to you what you make out of it.
    std::vector<RenderData> renderDatas;

    while (true)
    {
        // Clear render datas
        renderDatas.clear();
        
        if (!engine::IsInGame())
        {
            Sleep(500);
            continue;
        }

        // Ready to read, reserve a potentional amount in the memory.
        renderDatas.reserve(128);

        /* Old glow is now flagged.
        if (csgo_settings::Wallhack)
        {
            GlowObject = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwGlowObjectManager, sizeof(uint32_t));
        }*/

        uint32_t LocalPlayer = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwLocalPlayer, sizeof(uint32_t));
        Entity LocalPlayerEnt = Entity(LocalPlayer);

        if (aim.localPlayer.GetEntityAddress() != LocalPlayerEnt.GetEntityAddress())
        {
            aim.localPlayer = LocalPlayerEnt;
        }

        uint8_t OurTeam = LocalPlayerEnt.getTeam();
        if (csgo_settings::NoFlash)
        {
            LocalPlayerEnt.SetFlashAlpha(0.0f);
        }

        Vector3 myPosition = LocalPlayerEnt.getAbsolutePosition();

        for (short int i = 0; i < 64; i++)
        {
            uint32_t EntityAddr = Driver.ReadVirtualMemory<uint32_t>(ProcessId, ClientAddress + dwEntityList + i * 0x10,
                                                                     sizeof(uint32_t));

            if (EntityAddr == NULL)
            {
                continue;
            }

            if (csgo_settings::Wallhack)
            {
                Entity ent = utils::CreateEntity(EntityAddr);
                if (ent.isValidPlayer() && !ent.IsDormant())
                {
                    Vector3 screenPos;
                    Vector3 position = ent.getAbsolutePosition();
                    if (engine::worldToScreen(position, screenPos))
                    {
                        ent.BuildBonePairs();

                        const float distance = (position - myPosition).norm();
                        RenderData renderData = ent.getRenderData(OurTeam, screenPos, distance);

                        Vector3 headScreenPos;
                        if (engine::worldToScreen(ent.getHeadPosition(), headScreenPos))
                        {
                            renderData.headPos = ImVec2(headScreenPos.at(0), headScreenPos.at(1));
                        }
                        
                        for (size_t y = 0; y < ent.CurrentBonePairs; ++y)
                        {
                            const auto& pair = ent.BonePairs[y];

                            if (pair.first == pair.second || pair.first < 0 || pair.second < 0 || pair.first > 128 || pair.second > 128)
                            {
                                continue;
                            }

                            Vector3 boneScreenPos;
                            Vector3 boneScreenPos2;
                            if (engine::worldToScreen(ent.BonePositions[pair.first], boneScreenPos)
                                && engine::worldToScreen(ent.BonePositions[pair.second], boneScreenPos2))
                            {
                                renderData.bones.emplace_back(ImVec2(boneScreenPos.at(0), boneScreenPos.at(1)),
                                    ImVec2(boneScreenPos2.at(0), boneScreenPos2.at(1)));
                            }
                        }
                        
                        renderDatas.push_back(renderData);
                    }
                    // Old glow is now flagged.
                    //ent.SetCorrectGlowStruct(OurTeam, GlowObject);
                }
            }

            // TODO: Implement your own 2D radar into the rendering as using this will flag you.
            if (csgo_settings::Radar)
            {
                Entity ent = utils::CreateEntity(EntityAddr);
                if (ent.isValidPlayer() && !ent.IsDormant())
                {
                    Driver.WriteVirtualMemory<bool>(ProcessId, EntityAddr + m_bSpotted, true, sizeof(bool));
                }
            }
        }


        // Render all
        csgoRender.PollSystem();
        csgoRender.render(renderDatas);

        // This is flagged, use It only by understanding that you might get banned.
        if (csgo_settings::Bhop)
        {
            if (ImGui::IsCustomKeyPressed(VK_SPACE, true))
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

        if (csgo_settings::AimbotState == 1)
        {
            if (ImGui::IsCustomKeyPressed(csgo_settings::AimbotKey, true))
            {
                aim.aimAssist();
            }
            else
            {
                aim.resetSensitivity();
            }
        }
        else if (csgo_settings::AimbotState == 2)
        {
            if (ImGui::IsCustomKeyPressed(csgo_settings::AimbotKey, true))
            {
                aim.aimBot();
            }
        }


        Sleep(3);
    }
}
