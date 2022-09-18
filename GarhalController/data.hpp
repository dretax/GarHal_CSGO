#pragma once
#include <vector>
#include <windows.h>

#include "..\common\bsp\BSPStructure.hpp"
#include "imgui.h"
#include "..\common\bsp\Matrix.hpp"
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

typedef struct _RenderData
{
	// TODO: Implement name for yourself
	const char* name = "Garhal";
	float x = 0, y = 0;
	float inGameDistance = 0;
	ImVec4 color = { 0.f, 0.f, 0.f, 1.f };
	bool scale = true;
	bool render = true;
	ImVec2 headPos;
	std::vector<std::pair<ImVec2, ImVec2>> bones = {};
} RenderData;


// https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/sp/src/public/mathlib/mathlib.h#L237
struct Matrix3x4
{
	float m00; // xAxis.x
	float m10; // yAxis.x
	float m20; // zAxis.x
	float m30; // vecOrigin.x
	float m01; // xAxis.y
	float m11; // yAxis.y
	float m21; // zAxis.y
	float m31; // vecOrigin.y
	float m02; // xAxis.z
	float m12; // yAxis.z
	float m22; // zAxis.z
	float m32; // vecOrigin.z
};

struct Quaternion
{
	float X;
	float Y;
	float Z;
	float W;
};

// https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/sp/src/public/studio.h#L2050
struct StudioHdr
{
	int id;
	int version;
	int checksum;
	BYTE name[64];
	int length;
	Vector3 eyeposition;
	Vector3 illumposition;
	Vector3 hull_min;
	Vector3 hull_max;
	Vector3 view_bbmin;
	Vector3 view_bbmax;
	int flags;
	int numbones;                // bones
	int boneindex;
	int numbonecontrollers;      // bone controllers
	int bonecontrollerindex;
	int numhitboxsets;
	int hitboxsetindex;
};

// https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/sp/src/public/studio.h#L1612
struct StudioHitboxSet
{
	int sznameindex;
	int numhitboxes;
	int hitboxindex;
};

// https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/sp/src/public/studio.h#L420
struct StudioBBox
{
	int bone;
	int group; // intersection group
	Vector3 bbmin; // bounding box
	Vector3 bbmax;
	int szhitboxnameindex; // offset to the name of the hitbox.
	int unused[3];
	float radius; // when radius is -1 it's box, otherwise it's capsule (cylinder with spheres on the end)
	int pad[4];
};

// https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/sp/src/public/studio.h#L238
struct StudioBone
{
	int sznameindex;
	int parent; // parent bone
	int bonecontroller[6]; // bone controller index, -1 == none
	Vector3 pos;
	Quaternion quat;
	Vector3 rot;
	Vector3 posscale;
	Vector3 rotscale;
	Matrix3x4 poseToBone;
	Quaternion qAlignment;
	int flags;
	int proctype;
	int procindex; // procedural rule
	int physicsbone; // index into physically simulated bone
	int surfacepropidx; // index into string tablefor property name
	int contents; // See BSPFlags.h for the contents flags
	int unused[8]; // remove as appropriate
};

struct BoneMatrix
{
	byte pad3[12];
	float x;
	byte pad1[12];
	float y;
	byte pad2[12];
	float z;
};

struct TPlayerInfo
{
	__int64 unknown; //0x0000 
	union
	{
		__int64 steamID64; //0x0008 - SteamID64
		struct
		{
			__int32 xuid_low;
			__int32 xuid_high;
		};
	};

	char szName[128]; //0x0010 - Player Name
	int userId; //0x0090 - Unique Server Identifier
	char szSteamID[20]; //0x0094 - STEAM_X:Y:Z
	char pad_0x00A8[0x10]; //0x00A8
	unsigned long iSteamID; //0x00B8 - SteamID 
	char szFriendsName[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
};

enum GameState
{
	Lobby = 0,
	Loading = 1,
	Connecting = 2,
	Connected = 5,
	InGame = 6,
	UnknownG = -1,
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
#define MAX_STUDIO_BONES 128

inline const float FovRange = 10.0f;
extern DWORD ProcessId, ClientAddress, ClientSize, EngineAddress, EngineSize;
extern KeInterface Driver;