#pragma warning( disable : 4100 4047 4024 4022 4201 4311 4057 4213 4189 4081 4189 4706 4214 4459 4273 4242 4244 4127)

#include "data.h"
#include "messages.h"
#include "events.h"
#include "hide.h"
#include "memory.h"

// Find our required PE image.
PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallback(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo)
{
	//DebugMessageNormal("We received a load from: %ls \n", FullImageName->Buffer);

	// Compare our string to input
	if (wcsstr(FullImageName->Buffer, L"\\csgo\\bin\\client.dll"))
	{
		DebugMessageNormal("CSGO client.dll found!\n");
		DebugMessageNormal("Loaded Name: %ls \n", FullImageName->Buffer);
		DebugMessageNormal("Loaded To Process: %d \n", (ULONG) ProcessId);

		ClientAddress = (ULONG) ImageInfo->ImageBase;
		ClientSize = (ULONG) ImageInfo->ImageSize;
		CsgoID = (ULONG) ProcessId;

		// Free, and re-init the vector every time CSGO loads up.
		vector_free(&CSRSSList);
		vector_init(&CSRSSList);

		FindProcessByName("csrss.exe", &CSRSSList);

		DebugMessageNormal("csrss.exe Count: %d\r\n", vector_total(&CSRSSList));
	}
	else if (wcsstr(FullImageName->Buffer, L"\\Counter-Strike Global Offensive\\bin\\engine.dll"))
	{
		DebugMessageNormal("CSGO Engine.dll found!\n");

		EngineAddress = (ULONG) ImageInfo->ImageBase;
		EngineSize = (ULONG) ImageInfo->ImageSize;
	}

	return STATUS_SUCCESS;
}

PCREATE_PROCESS_NOTIFY_ROUTINE_EX ProcessNotifyCallbackEx(HANDLE parentId, HANDLE processId, PPS_CREATE_NOTIFY_INFO notifyInfo)
{
	// NotifyInfo is filled when a process is created. Otherwise terminated.
	if (notifyInfo)
	{
		//DebugMessageNormal("PID = %d\r\n", processId);
		//DebugMessageNormal("Process Full Path: %ls \n", notifyInfo->ImageFileName->Buffer);
		if (wcsstr(notifyInfo->ImageFileName->Buffer, L"\\GarhalController.exe"))
		{
			DebugMessageNormal("Bomb has been planted!\n");
			ControllerID = (ULONG) processId;
			DebugMessageNormal("Controller ProcessID: %d\r\n", ControllerID);

			if (EnableProcessHiding == 1)
			{
				UINT32 intmadafaka = (UINT32) processId;

				DebugMessageNormal("Hidden APP = %d\r\n", intmadafaka);
				HideProcess(intmadafaka);
			}
		}
		else if (wcsstr(notifyInfo->ImageFileName->Buffer, L"\\GarhalRankDisplayer.exe"))
		{
			RankReaderID = (ULONG) processId;
			DebugMessageNormal("RankReaderID ProcessID: %d\r\n", RankReaderID);

			if (EnableProcessHiding == 1)
			{
				UINT32 intmadafaka = (UINT32) processId;

				DebugMessageNormal("Hidden APP = %d\r\n", intmadafaka);
				HideProcess(intmadafaka);
			}
		}
	}
	else
	{
		ULONG ProcID = (ULONG) processId;
		if (ControllerID == ProcID)
		{
			DebugMessageNormal("Controller Shutdown detected, disabling protection. %d\r\n", ControllerID);
			ControllerID = 0;
			ProtectController = 0;
		}
		else if (RankReaderID == ProcID)
		{
			DebugMessageNormal("RankReader Shutdown detected, disabling protection. %d\r\n", RankReaderID);
			RankReaderID = 0;
			ProtectRankReader = 0;
		}
		else if (CsgoID == ProcID)
		{
			DebugMessageNormal("CSGO Shutdown detected, zeroing addresses. %d\r\n", CsgoID);
			CsgoID = 0;
			ClientAddress = 0;
			EngineAddress = 0;
			ClientSize = 0;
			EngineSize = 0;
		}
	}

	return STATUS_SUCCESS;
}

// TBT 2014 https://www.unknowncheats.me/wiki/Valve_Anti-Cheat:VAC_external_tool_detection_(and_more)
// Since a recent silent update Valve is being retarded about open handles(which are caused by OpenProcess).
// You can avoid this by restricting access to your external hack and csgo by using ObRegisterCallbacks
// You should go with ObRegisterCallbacks and permit query on csgo(0x0400 PROCESS QUERY INFORMATION) / protect your external hack from all access.
// Even Mambda mentioned the same in 2016 on guidedhacking, and a 2013 reversal validates it:
// Opens a process to every handle running with query_information and vm_read , tries to get their name and do some more things that i can't see yet.
// Course of action here for my external :
// Strip handles of those values^, i don't really care about anything else. They can't get my name if they dont have the privileges to.Also they couldn't find it on file if they tried.
OB_PREOP_CALLBACK_STATUS OBRegisterCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation)
{
	// Our controller or CSGO is not running yet.
	if (ControllerID == 0 || CsgoID == 0)
	{
		return OB_PREOP_SUCCESS;
	}

	PEPROCESS OpenedProcess = (PEPROCESS)OperationInformation->Object;
	ULONG OpenedProcessID = (ULONG)PsGetProcessId(OpenedProcess);
	PEPROCESS CurrentProcess = PsGetCurrentProcess();

	// Allow any driver to get the handle.
	if (OperationInformation->KernelHandle == 1 || OpenedProcess == CurrentProcess)
	{
		return OB_PREOP_SUCCESS;
	}

	// Make sure not to strip csrss's Handle, will cause BSOD
	for (int i = 0; i < vector_total(&CSRSSList); i++)
	{
		PEPROCESS CSRSS = (PEPROCESS)vector_get(&CSRSSList, i);
		ULONG procid = (ULONG)PsGetProcessId(CSRSS);
		if (procid == OpenedProcessID)
		{
			return OB_PREOP_SUCCESS;
		}
	}

	// Ensure we don't fuck with CSGO.
	if (CsgoID == OpenedProcessID)
	{
		return OB_PREOP_SUCCESS;
	}

	// If VAC is trying get information say fuck you. (Change the access completely)
	// https://docs.microsoft.com/en-us/windows/win32/procthread/process-security-and-access-rights
	if (OpenedProcessID == ControllerID && ProtectController == 1)
	{
		if (OperationInformation->Operation == OB_OPERATION_HANDLE_CREATE) 
		{
			OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = (SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION);
			DebugMessageNormal("OB_OPERATION_HANDLE_CREATE for Controller denied.\n");
		}
		else
		{
			OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess = (SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION);
			DebugMessageNormal("OB_OPERATION_HANDLE_CREATE Duplicate for Controller denied.\n");
		}
	}
	else if (OpenedProcessID == RankReaderID && ProtectRankReader == 1)
	{
		if (OperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)
		{
			OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = (SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION);
			DebugMessageNormal("OB_OPERATION_HANDLE_CREATE for RankReader denied.\n");
		}
		else
		{
			OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess = (SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION);
			DebugMessageNormal("OB_OPERATION_HANDLE_CREATE Duplicate for RankReader denied.\n");
		}
	}

	return OB_PREOP_SUCCESS;
}