#pragma once
#include "csgo_settings.hpp"
#include "imgui_extensions.h"
#include "overlay.h"

class CSGORender : public OverlayRender
{

    void render_aimbot_settings()
    {        
        static ImGuiTextFilter filter;
        ImGui::CheckboxNew("Enable TriggerBot", &csgo_settings::TriggerBot);

        // TODO: Custom hotkeys are relatively easy to implement.
        // Do them yourself, maybe I'll introduce a solution to the project later.
        ImGui::ListBox("Aimbot State", &csgo_settings::AimbotState, csgo_settings::aimbotOptions);
        if (csgo_settings::AimbotState > 0)
        {
            ImGui::SameLine();
            ImGui::ListBox("AimbotTarget", &csgo_settings::AimbotTarget, csgo_settings::aimbotTargets);
            ImGui::SliderIntNew("AimbotBullets", &csgo_settings::AimbotBullets, 0, 100);
        }

        if (csgo_settings::TriggerBot)
        {
            ImGui::CheckboxNew("Enable TriggerBotDelay", &csgo_settings::TriggerBotDelay);            
            ImGui::SliderIntNew("TriggerBot MinDelay", &csgo_settings::TriggerBotDelayMin, 0, 100);
            ImGui::SliderIntNew("TriggerBot MaxDelay", &csgo_settings::TriggerBotDelayMax, 0, 100);


            filter.Draw();
            if (ImGui::BeginCombo("Allowed WeaponIds", ""))
            {
                int i = 0;
                for (auto iter = csgo_settings::weaponIds.begin(); iter != csgo_settings::weaponIds.end(); ++iter)
                {
                    if (!filter.PassFilter(iter->first.c_str())) continue;
                    ImGui::Selectable(iter->first.c_str(), &csgo_settings::selectedWeaponIds[i],
                        ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups, ImVec2(0, 0));
                    i++;
                }

                ImGui::EndCombo();
            }
        }
    }

    void render_visuals_settings()
    {
        ImGui::CheckboxNew("Use vSync", &csgo_settings::useVsync);
        ImGui::Separator();
        ImGui::CheckboxNew("Enable Wallhack", &csgo_settings::Wallhack);
        ImGui::CheckboxNew("Enable Radar (Flagged)", &csgo_settings::Radar);
        ImGui::CheckboxNew("Enable NoFlash (Flagged)", &csgo_settings::NoFlash);
        ImGui::CheckboxNew("Enable Bhop (Flagged)", &csgo_settings::Bhop);
       
    }
    
    void render_settings()
    {
        ImGui::PushFont(mapFont[18]);
        ImGui::Text("Blah blah whatever info we want here");

        ImGuiStyle* style = &ImGui::GetStyle();
        ImGui::Separator();        

        if (ImGui::Button("Save Settings", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - style->WindowPadding.x * 0.5f, 0)))
        {
            csgo_settings::WriteConfig("settings.json");
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Settings", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
        {
            csgo_settings::ReadConfig("settings.json");
        }
        ImGui::PopFont();
    }

    BOOL useVSync() override
    {
        return csgo_settings::useVsync;
    }
    
public:
    
    CSGORender()
    {
        // Read config values.
        csgo_settings::ReadConfig("settings.json");
    }

    void display() override
    {
        if (ImGui::IsCustomKeyPressed(VK_HOME, false))
        {
            csgo_settings::showMenu = !csgo_settings::showMenu;
        }

        if (!csgo_settings::showMenu)
        {
            return;
        }


        ImGui::SetMenuLocation(ImVec2(xScreen, yScreen));
        ImGui::Begin("All Settings", &csgo_settings::showMenu, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoClipping);
        {
            ImGuiWindow* window = ImGui::GetCurrentWindow();
            ImDrawList* draw = window->DrawList;
            ImGuiStyle* style = &ImGui::GetStyle();

            ImVec2 window_position = ImGui::GetWindowPos();
            ImRect window_background(window_position, window_position + window->SizeFull); //Calculate axes of main window (for getting correct pos in future)
         
            ImRect top_panel(ImFloor(ImVec2(window_position.x, window_position.y)), ImFloor(ImVec2(window_position.x + window->SizeFull.x, window_position.y + window->SizeFull.y * 1/16)));
            ImRect logo_panel(top_panel.Min, ImVec2(top_panel.Min.x + top_panel.GetHeight(), top_panel.Max.y));

            enum Tabs_
            {
                Tab_Info = 0,
                Tab_Aimbot = 1,
                Tab_Visuals = 2
            };
            static int CurrentTab = 0;
            ImGui::SetCursorPos(ImVec2(0, 0));
            ImGui::SetNextWindowBgAlpha(1);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::BeginChild("TabsChild##Top", top_panel.GetSize(), true, ImGuiWindowFlags_NoClipping);            
            {
                ImGuiWindow* chwindow = ImGui::GetCurrentWindow();

                ImRect tabs_panel(ImVec2(logo_panel.Max.x, top_panel.Min.y), top_panel.Max);
                ImRect tab_one(ImFloor(tabs_panel.Min), ImFloor(ImVec2(tabs_panel.Min.x + tabs_panel.GetWidth() * 0.5f, tabs_panel.Max.y)));
                ImRect tab_two(ImFloor(ImVec2(tab_one.Max.x, tabs_panel.Min.y)), ImFloor(tabs_panel.Max));

                ImGui::SetCursorPos(ImVec2(0, 0));
                ImGui::BeginGroup();
                ImGui::PushFont(mapFont[(int)(top_panel.GetHeight() * 0.75f)]);
                if (ImGui::TabNew("##Logo", CurrentTab == Tab_Info, logo_panel.GetSize(), LogoTexture))
                    CurrentTab = Tab_Info;
                ImGui::SameLine();

                if (ImGui::TabNew("Aimbot", CurrentTab == Tab_Aimbot, tab_one.GetSize()))
                    CurrentTab = Tab_Aimbot;

                ImGui::SameLine();
                if (ImGui::TabNew("Visuals", CurrentTab == Tab_Visuals, tab_two.GetSize()))
                    CurrentTab = Tab_Visuals;
                ImGui::PopFont();
                ImGui::EndGroup();
            }
            ImGui::EndChild();
            ImGui::PopStyleVar(2);
            ImGui::Spacing();
            bool border = true;

            if (CurrentTab == Tab_Info)
            {
                ImGui::BeginChild("Info", ImVec2(0, 0), border, ImGuiWindowFlags_NoClipping | ImGuiWindowFlags_AlwaysUseWindowPadding);
                {
                    render_settings();
                }
                ImGui::EndChild();
            }
            else if (CurrentTab == Tab_Aimbot)
            {
                ImGui::BeginChild("Aimbot", ImVec2(0, 0), border, ImGuiWindowFlags_NoClipping | ImGuiWindowFlags_AlwaysUseWindowPadding);
                {
                    render_aimbot_settings();
                }
                ImGui::EndChild();
            }
            else if (CurrentTab == Tab_Visuals)
            {
                ImGui::BeginChild("Visuals", ImVec2(0, 0), border, ImGuiWindowFlags_NoClipping | ImGuiWindowFlags_AlwaysUseWindowPadding);
                {
                    render_visuals_settings();
                }
                ImGui::EndChild();
            }

        }
        ImGui::End();
    }
};