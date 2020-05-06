#pragma once
#include <windows.h>
#include "kernelinterface.hpp"
#include "Matrix.hpp"

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

inline DWORD ProcessId, ClientAddress, EngineAddress;
inline KeInterface Driver = NULL;