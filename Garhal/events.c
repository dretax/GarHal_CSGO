#pragma warning( disable : 4100 4047 4024 4022 4201 4311 4057 4213 4189 4081 4189 4706 4214 4459 4273 4242 4244)

#include "events.h"
#include "data.h"
#include "hide.h"
#include "messages.h"


// Find our required PE image.
PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallback(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo)
{
	//DbgPrintEx(0, 0, "We received a load from: %ls \n", FullImageName->Buffer);

	// Compare our string to input
	if (wcsstr(FullImageName->Buffer, L"\\csgo\\bin\\client_panorama.dll")) 
	{
		DebugMessageNormal("CSGO client_panorama.dll found!\n");
		DebugMessageNormal("Loaded Name: %ls \n", FullImageName->Buffer);
		DebugMessageNormal("Loaded To Process: %d \n", ProcessId);

		ClientAddress = ImageInfo->ImageBase;
		ClientSize = ImageInfo->ImageSize;
		csgoId = ProcessId;
	}
	else if (wcsstr(FullImageName->Buffer, L"\\Counter-Strike Global Offensive\\bin\\engine.dll"))
	{
		DebugMessageNormal("CSGO Engine.dll found!\n");

		EngineAddress = ImageInfo->ImageBase;
		EngineSize = ImageInfo->ImageSize;
	}

	return STATUS_SUCCESS;
}

// Would be used for Hide process, but you would need to have the patchguard removed first.
PCREATE_PROCESS_NOTIFY_ROUTINE_EX ProcessNotifyCallbackEx(HANDLE parentId, HANDLE processId, PPS_CREATE_NOTIFY_INFO notifyInfo)
{
	// if we dont use this statement, we will get a bsod if notifyInfo is empty
	if (notifyInfo) 
	{
		//DbgPrintEx(0, 0, "PID = %d\r\n", processId);
		//DbgPrintEx(0, 0, "Process Full Path: %ls \n", notifyInfo->ImageFileName->Buffer);
		if (wcsstr(notifyInfo->ImageFileName->Buffer, L"\\GarhalController.exe"))
		{
			DebugMessageNormal("Bomb has been planted!\n");
			uintptr_t i = (uintptr_t)processId; // Unsure if this is needed.
			UINT32 intmadafaka = (UINT32)i;

			
			DebugMessageNormal("Hidden APP = %d\r\n", intmadafaka);
			HideProcess(intmadafaka);
		}
	}

	return STATUS_SUCCESS;
}