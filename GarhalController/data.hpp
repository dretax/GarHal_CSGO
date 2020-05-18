#pragma once
#include <windows.h>
#include "Matrix.hpp"
#include "kernelinterface.hpp"

using Vector3 = Matrix<float, 3, 1>;
using WorldToScreenMatrix = Matrix<float, 4, 4>;

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

struct UserCMD_ShellCode
{
	Vector3		m_vecViewAngles;     // 0x0C
	float		m_flForwardmove;     // 0x24
	float		m_flSidemove;        // 0x28
	bool		shouldSet;
};

enum CSGO_Weapon_ID
{
	WEAPON_UNKNOWN = 0,
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516
};

inline bool IsWeaponNonAim(int iWeaponID)
{
	if (iWeaponID == WEAPON_KNIFE_BAYONET 
		|| iWeaponID == WEAPON_KNIFE_FLIP
		|| iWeaponID == WEAPON_KNIFE
		|| iWeaponID == WEAPON_KNIFE_T
		|| iWeaponID == WEAPON_KNIFE_GUT
		|| iWeaponID == WEAPON_KNIFE_KARAMBIT
		|| iWeaponID == WEAPON_KNIFE_M9_BAYONET
		|| iWeaponID == WEAPON_KNIFE_TACTICAL
		|| iWeaponID == WEAPON_KNIFE_FALCHION
		|| iWeaponID == WEAPON_KNIFE_SURVIVAL_BOWIE
		|| iWeaponID == WEAPON_KNIFE_BUTTERFLY
		|| iWeaponID == WEAPON_KNIFE_PUSH
		|| iWeaponID == WEAPON_FLASHBANG
		|| iWeaponID == WEAPON_HEGRENADE
		|| iWeaponID == WEAPON_SMOKEGRENADE
		|| iWeaponID == WEAPON_MOLOTOV
		|| iWeaponID == WEAPON_DECOY
		|| iWeaponID == WEAPON_INCGRENADE
		|| iWeaponID == WEAPON_C4)
	{
		return true;
	}

	return false;
}

inline bool IsWeaponPistol(int iWeaponID)
{
	return iWeaponID == WEAPON_DEAGLE
		|| iWeaponID == WEAPON_ELITE
		|| iWeaponID == WEAPON_FIVESEVEN
		|| iWeaponID == WEAPON_GLOCK
		|| iWeaponID == WEAPON_P250
		|| iWeaponID == WEAPON_USP_SILENCER
		|| iWeaponID == WEAPON_CZ75A
		|| iWeaponID == WEAPON_REVOLVER
		|| iWeaponID == WEAPON_TEC9
		|| iWeaponID == WEAPON_TASER
		|| iWeaponID == WEAPON_HKP2000;
}

inline bool IsWeaponSniper(int iWeaponID)
{
	return iWeaponID == WEAPON_AWP
		|| iWeaponID == WEAPON_SSG08;
}

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define	FL_ONGROUND (1<<0)
#define IN_ATTACK		(1<<0)
#define IN_ATTACK2	(1<<11)
#define IN_RELOAD		(1<<13)
#define KEY_DOWN (0x80000000)
#define MOUSE_UP (0x100)
#define HEAD_BONE_ID ((DWORD) 8)
#define CHEST_BONE_ID ((DWORD) 37)

inline const float FovRange = 10.0f;
inline DWORD ProcessId, ClientAddress, ClientSize, EngineAddress, EngineSize, CMDAddress;
inline DWORD** IClientMode;
inline KeInterface Driver = NULL;

// Store the config values here.
inline uint8_t AimbotS = 0;
inline uint16_t AimbotKey = 0;
inline uint8_t AimbotTarget = 0;
inline uint8_t AimbotBullets = 0;
inline bool Bhop = false;
inline bool AntiAimS = false;