#pragma once
#include <Windows.h>
#include "data.hpp"

class AntiAim
{
public:
	AntiAim();
	~AntiAim();
	void DoAntiAim();
	void SetUserCMDViewAngles(Vector3 viewAngles);
	void Hook(DWORD Instance, int Index, DWORD HookFunc);
	void HookCreateMove();
	void SetUserCmd(UserCMD_ShellCode cmd);
	void Enable();
	Vector3 getLocalViewAngles();
	Vector3 getUserCMDViewAngles();
	DWORD GetVFunc(DWORD inst, int Index);
	
private:
	HANDLE ProcessHandle;
	bool ShouldSetViewAnglesThisTick = false;

	// Should set angle "pointer" (technically a pointer but it points to a "bool" in another address space)
	// If true, the game will rely on us to provide viewangles for the userCMD structure (if we don't, they are never changed)
	// If false, the game does the viewangles for the userCMD itself
	DWORD shouldSetAngles;
	//userCMD viewangle x "pointer"
	DWORD cmdAngleX;
	//userCMD viewangle y "pointer"
	DWORD cmdAngleY;
	//userCMD viewangle z "pointer"
	DWORD cmdAngleZ;
};
