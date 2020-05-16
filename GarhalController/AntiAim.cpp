#pragma warning (disable : 26495 4302 4311)

#include "AntiAim.hpp"
#include "offsets.hpp"


void AntiAim::SetUserCMDViewAngles(Vector3 viewAngles)
{
	Driver.WriteVirtualMemory<float>(ProcessId, cmdAngleX, viewAngles(0), sizeof(viewAngles(0)));
	Driver.WriteVirtualMemory<float>(ProcessId, cmdAngleX, viewAngles(1), sizeof(viewAngles(1)));
	Driver.WriteVirtualMemory<float>(ProcessId, cmdAngleX, viewAngles(2), sizeof(viewAngles(2)));
}

void AntiAim::SetUserCmd(UserCMD_ShellCode cmd)
{
	cmd.shouldSet = true;
	Driver.WriteVirtualMemory<UserCMD_ShellCode>(ProcessId, CMDAddress, cmd, sizeof(cmd));
}

Vector3 AntiAim::getLocalViewAngles()
{
	uint32_t clientState = Driver.ReadVirtualMemory<uint32_t>(ProcessId, EngineAddress + hazedumper::signatures::dwClientState, sizeof(uint32_t));
	return Driver.ReadVirtualMemory<Vector3>(ProcessId, clientState + hazedumper::signatures::dwClientState_ViewAngles, sizeof(Vector3));
}

Vector3 AntiAim::getUserCMDViewAngles()
{
	Vector3 buffer;
	buffer(0) = Driver.ReadVirtualMemory<float>(ProcessId, cmdAngleX, sizeof(float));
	buffer(1) = Driver.ReadVirtualMemory<float>(ProcessId, cmdAngleY, sizeof(float));
	buffer(2) = Driver.ReadVirtualMemory<float>(ProcessId, cmdAngleZ, sizeof(float));
	return buffer;
}

// Credits: sethxi
DWORD AntiAim::GetVFunc(DWORD inst, int Index)
{
	DWORD table = Driver.ReadVirtualMemory<DWORD>(ProcessId, inst, sizeof(inst));
	DWORD func = table + sizeof(DWORD) * Index;
	return func;
}

void AntiAim::Hook(DWORD Instance, int Index, DWORD HookFunc)
{
	uintptr_t VFunc = GetVFunc(Instance, Index);
	DWORD OldProtect;
	
	VirtualProtectEx(ProcessHandle, (LPVOID)VFunc, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &OldProtect);
	Driver.WriteVirtualMemory<DWORD>(ProcessId, VFunc, HookFunc, sizeof(HookFunc));
	VirtualProtectEx(ProcessHandle, (LPVOID)VFunc, sizeof(DWORD), OldProtect, &OldProtect);
}

void AntiAim::HookCreateMove()
{
	BYTE hook[] = "\x55\x89\xE5\x8B\x55\x0C\x8B\x0D\x00\x00\x00\x00\x83\xF9\x01\x74\x02\xEB\x2A\xA1\x00\x00\x00\x00\x89\x42\x0C\xA1\x00\x00\x00\x00\x89\x42\x10\xA1\x00\x00\x00\x00\x89\x42\x14\xA1\x00\x00\x00\x00\x89\x42\x24\xA1\x00\x00\x00\x00\x89\x42\x28\xEB\x00\x5D\xC2\x08\x00";
	/*
	asm (with forward/sidemove):
	push ebp
	mov ebp,esp
	mov edx,DWORD PTR [ebp+0xc]
	mov ecx,[0x00000000]
	cmp ecx, 1
	je setCmd
	jmp done
	setCmd:
	mov eax,[0x00000000]
	mov [edx+0xc], eax
	mov eax,[0x00000000]
	mov [edx+0x10], eax
	mov eax,[0x00000000]
	mov [edx+0x14], eax
	mov eax,[0x00000000]
	mov [edx+0x24], eax
	mov eax,[0x00000000]
	mov [edx+0x28], eax
	jmp done
	done:
	pop ebp
	ret 8
	*/

	// TODO: Use shell code struct.
	cmdAngleX = (DWORD) VirtualAllocEx(ProcessHandle, NULL, sizeof(float) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	cmdAngleY = (DWORD) VirtualAllocEx(ProcessHandle, NULL, sizeof(float) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	cmdAngleZ = (DWORD) VirtualAllocEx(ProcessHandle, NULL, sizeof(float) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	shouldSetAngles = (DWORD) VirtualAllocEx(ProcessHandle, NULL, sizeof(bool) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	CMDAddress = (DWORD) VirtualAllocEx(ProcessHandle, NULL, sizeof(UserCMD_ShellCode) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	LPVOID shellCodeAddress = VirtualAllocEx(ProcessHandle, NULL, sizeof(hook) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	const float zero = 0.f;
	if (!Driver.WriteVirtualMemory<float>(ProcessId, CMDAddress, zero, sizeof(zero)))
	{
		return;
	}

	// Fill in addresses that (inside CS:GO) point towards the values we set
	*(DWORD*)(&hook[0x14]) = CMDAddress;
	*(DWORD*)(&hook[0x1C]) = CMDAddress + 0x4;
	*(DWORD*)(&hook[0x24]) = CMDAddress + 0x8;
	*(DWORD*)(&hook[0x2C]) = CMDAddress + 0xC;
	*(DWORD*)(&hook[0x34]) = CMDAddress + 0x10;
	// Should Set
	*(DWORD*)(&hook[0x8]) = CMDAddress + 0x14;

	if (!Driver.WriteVirtualMemory(ProcessId, (ULONG) shellCodeAddress, &hook[0], sizeof(hook)))
	{
		return;
	}

	Hook(IClientMode, 24, (DWORD)shellCodeAddress);
}

void AntiAim::DoAntiAim()
{
	if (!GetAsyncKeyState(0x1))
	{
		ShouldSetViewAnglesThisTick = true;
		Vector3 currentViewAngles = getLocalViewAngles();
		currentViewAngles(0) = 89.f;
		SetUserCMDViewAngles(currentViewAngles);
	}
	else
	{
		ShouldSetViewAnglesThisTick = false;
	}
	Driver.WriteVirtualMemory<bool>(ProcessId, shouldSetAngles, ShouldSetViewAnglesThisTick, sizeof(bool));
}

void AntiAim::Enable()
{
	ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
}

AntiAim::AntiAim()
{
	
}

AntiAim::~AntiAim()
{
	if (ProcessHandle != NULL)
	{
		CloseHandle(ProcessHandle);
	}
}
