#include "csgo_settings.hpp"
#include "json.hpp"


namespace csgo_settings
{
    // Aimbot Type 0=Disabled, 1=Smooth, 2=Direct
    int AimbotState = 0;
    // https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    int AimbotKey = 0x58;
    // Aimbot Target Head = 1, Body = 2, or both = 3? (Both = hp >= 50 -> head, below body) 0 to disable.
    int AimbotTarget = 0;
    // Aimbot Assist begins at Nth bullet. (Used when AimbotState is 1)
    int AimbotBullets = 3;
    // Use Bhop?
    bool Bhop = false;
    // Enable Wallhack
    bool Wallhack = true;
    // Enable NoFlash
    bool NoFlash = false;
    // Enable Triggerbot
    bool TriggerBot = false;
    // Key to use triggerbot, set to 0 to make It automatic. (https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes)
    int TriggerBotKey = 0;
    // Enable Triggerbot delay?
    bool TriggerBotDelay = false;
    // Triggerbot random delay (8,15) means we are delaying randomby between 8-15 millisecs. 0 is good to use at minimum.
    int TriggerBotDelayMin = 8;
    int TriggerBotDelayMax = 15;
    // Enable Radar mark?
    bool Radar = false;
    // Vsync
    bool useVsync = false;
    // Show menu
    bool showMenu = false;
    // Weapon Id list
    nlohmann::fifo_map<std::string, int> weaponIds = {
        {"WEAPON_UNKNOWN", 0},
        {"WEAPON_DEAGLE", 1},
        {"WEAPON_ELITE", 2},
        {"WEAPON_FIVESEVEN", 3},
        {"WEAPON_GLOCK", 4},
        {"WEAPON_AK47", 7},
        {"WEAPON_AUG", 8},
        {"WEAPON_AWP", 9},
        {"WEAPON_FAMAS", 10},
        {"WEAPON_G3SG1", 11},
        {"WEAPON_GALILAR", 13},
        {"WEAPON_M249", 14},
        {"WEAPON_M4A1", 16},
        {"WEAPON_MAC10", 17},
        {"WEAPON_P90", 19},
        {"WEAPON_UMP45", 24},
        {"WEAPON_XM1014", 25},
        {"WEAPON_BIZON", 26},
        {"WEAPON_MAG7", 27},
        {"WEAPON_NEGEV", 28},
        {"WEAPON_SAWEDOFF", 29},
        {"WEAPON_TEC9", 30},
        {"WEAPON_TASER", 31},
        {"WEAPON_HKP2000", 32},
        {"WEAPON_MP7", 33},
        {"WEAPON_MP9", 34},
        {"WEAPON_NOVA", 35},
        {"WEAPON_P250", 36},
        {"WEAPON_SCAR20", 38},
        {"WEAPON_SG556", 39},
        {"WEAPON_SSG08", 40},
        {"WEAPON_KNIFE", 42},
        {"WEAPON_FLASHBANG", 43},
        {"WEAPON_HEGRENADE", 44},
        {"WEAPON_SMOKEGRENADE", 45},
        {"WEAPON_MOLOTOV", 46},
        {"WEAPON_DECOY", 47},
        {"WEAPON_INCGRENADE", 48},
        {"WEAPON_C4", 49},
        {"WEAPON_KNIFE_T", 59},
        {"WEAPON_M4A1_SILENCER", 60},
        {"WEAPON_USP_SILENCER", 61},
        {"WEAPON_CZ75A", 63},
        {"WEAPON_REVOLVER", 64},
        {"WEAPON_KNIFE_BAYONET", 500},
        {"WEAPON_KNIFE_FLIP", 505},
        {"WEAPON_KNIFE_GUT", 506},
        {"WEAPON_KNIFE_KARAMBIT", 507},
        {"WEAPON_KNIFE_M9_BAYONET", 508},
        {"WEAPON_KNIFE_TACTICAL", 509},
        {"WEAPON_KNIFE_FALCHION", 512},
        {"WEAPON_KNIFE_SURVIVAL_BOWIE", 514},
        {"WEAPON_KNIFE_BUTTERFLY", 515},
        {"WEAPON_KNIFE_PUSH", 516}
    };
    // We store weapon names by indexes to access easily by iterating the imgui combo helper
    std::unordered_map<int, std::string> weaponIdHelper = {
        {0, "WEAPON_UNKNOWN"},
        {1, "WEAPON_DEAGLE"},
        {2, "WEAPON_ELITE"},
        {3, "WEAPON_FIVESEVEN"},
        {4, "WEAPON_GLOCK"},
        {5, "WEAPON_AK47"},
        {6, "WEAPON_AUG"},
        {7, "WEAPON_AWP"},
        {8, "WEAPON_FAMAS"},
        {9, "WEAPON_G3SG1"},
        {10, "WEAPON_GALILAR"},
        {11, "WEAPON_M249"},
        {12, "WEAPON_M4A1"},
        {13, "WEAPON_MAC10"},
        {14, "WEAPON_P90"},
        {15, "WEAPON_UMP45"},
        {16, "WEAPON_XM1014"},
        {17, "WEAPON_BIZON"},
        {18, "WEAPON_MAG7"},
        {19, "WEAPON_NEGEV"},
        {20, "WEAPON_SAWEDOFF"},
        {21, "WEAPON_TEC9"},
        {22, "WEAPON_TASER"},
        {23, "WEAPON_HKP2000"},
        {24, "WEAPON_MP7"},
        {25, "WEAPON_MP9"},
        {26, "WEAPON_NOVA"},
        {27, "WEAPON_P250"},
        {28, "WEAPON_SCAR20"},
        {29, "WEAPON_SG556"},
        {30, "WEAPON_SSG08"},
        {31, "WEAPON_KNIFE"},
        {32, "WEAPON_FLASHBANG"},
        {33, "WEAPON_HEGRENADE"},
        {34, "WEAPON_SMOKEGRENADE"},
        {35, "WEAPON_MOLOTOV"},
        {36, "WEAPON_DECOY"},
        {37, "WEAPON_INCGRENADE"},
        {38, "WEAPON_C4"},
        {39, "WEAPON_KNIFE_T"},
        {40, "WEAPON_M4A1_SILENCER"},
        {41, "WEAPON_USP_SILENCER"},
        {42, "WEAPON_CZ75A"},
        {43, "WEAPON_REVOLVER"},
        {44, "WEAPON_KNIFE_BAYONET"},
        {45, "WEAPON_KNIFE_FLIP"},
        {46, "WEAPON_KNIFE_GUT"},
        {47, "WEAPON_KNIFE_KARAMBIT"},
        {48, "WEAPON_KNIFE_M9_BAYONET"},
        {49, "WEAPON_KNIFE_TACTICAL"},
        {50, "WEAPON_KNIFE_FALCHION"},
        {51, "WEAPON_KNIFE_SURVIVAL_BOWIE"},
        {52, "WEAPON_KNIFE_BUTTERFLY"},
        {53, "WEAPON_KNIFE_PUSH"}
    };
    // Imgui Combo helper
    bool selectedWeaponIds[53];
    
    const std::vector<std::string> aimbotOptions = {
        "Disabled",
        "Smooth",
        "Direct"
    };

    const std::vector<std::string> aimbotTargets = {
        "Disabled",
        "Smooth",
        "Direct"
    };
    
    void ReadConfig(const char* filename)
    {
        std::ifstream file(filename);

        if (file.is_open())
        {
            nlohmann::json jsonConfig = nullptr;
            try
            {
                // We are reading a small file so we do not need to worry about inefficiency rn
                std::string json((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                
                jsonConfig = nlohmann::json::parse(json);
            }
            catch (...)
            {
                MessageBoxA(0, "Failed to read config!", "Garhal", MB_OK | MB_ICONWARNING);
            }

            if (jsonConfig != NULL)
            {
                csgo_settings::AimbotState = jsonConfig["AimbotState"];
                csgo_settings::AimbotKey = jsonConfig["AimbotKey"];
                csgo_settings::AimbotTarget = jsonConfig["AimbotTarget"];
                csgo_settings::AimbotBullets = jsonConfig["AimbotBullets"];
                csgo_settings::Bhop = jsonConfig["Bhop"];
                csgo_settings::Wallhack = jsonConfig["Wallhack"];
                csgo_settings::NoFlash = jsonConfig["NoFlash"];
                csgo_settings::Radar = jsonConfig["Radar"];
                csgo_settings::TriggerBot = jsonConfig["TriggerBot"];
                csgo_settings::TriggerBotKey = jsonConfig["TriggerBotKey"];
                csgo_settings::TriggerBotDelay = jsonConfig["TriggerBotDelay"];
                csgo_settings::TriggerBotDelayMin = jsonConfig["TriggerBotDelayMin"];
                csgo_settings::TriggerBotDelayMax = jsonConfig["TriggerBotDelayMax"];

                std::vector<bool> selectedWeaponIdsList = jsonConfig["selectedWeaponIds"];
                const int size = *(&csgo_settings::selectedWeaponIds + 1) - csgo_settings::selectedWeaponIds;
                for (size_t i(0); i < size; ++i)
                {
                    csgo_settings::selectedWeaponIds[i] = selectedWeaponIdsList.at(i);
                }

                csgo_settings::useVsync = jsonConfig["useVsync"];
            }

            file.close();
        }
    }

    void WriteConfig(const char* filename)
    {
        FILE* pFile;
        fopen_s(&pFile, filename, "w");

        if (!pFile)
        {
            MessageBoxA(0, "Failed to save config!", "Garhal", MB_OK | MB_ICONWARNING);
            return;
        }
		
        nlohmann::json jsonConfig;
        jsonConfig["AimbotState"] = csgo_settings::AimbotState;
        jsonConfig["AimbotKey"] = csgo_settings::AimbotKey;
        jsonConfig["AimbotTarget"] = csgo_settings::AimbotTarget;
        jsonConfig["AimbotBullets"] = csgo_settings::AimbotBullets;
        jsonConfig["Bhop"] = csgo_settings::Bhop;
        jsonConfig["Wallhack"] = csgo_settings::Wallhack;
        jsonConfig["NoFlash"] = csgo_settings::NoFlash;
        jsonConfig["Radar"] = csgo_settings::Radar;
        jsonConfig["TriggerBot"] = csgo_settings::TriggerBot;
        jsonConfig["TriggerBotKey"] = csgo_settings::TriggerBotKey;
        jsonConfig["TriggerBotDelay"] = csgo_settings::TriggerBotDelay;
        jsonConfig["selectedWeaponIds"] = csgo_settings::selectedWeaponIds;
        jsonConfig["TriggerBotDelayMin"] = csgo_settings::TriggerBotDelayMin;
        jsonConfig["TriggerBotDelayMax"] = csgo_settings::TriggerBotDelayMax;
        jsonConfig["useVsync"] = csgo_settings::useVsync;

        fprintf(pFile, "%s\n", jsonConfig.dump().c_str());

        fclose(pFile);
    }
}
