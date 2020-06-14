#pragma warning( disable : 4100 4047 4024 4022 4201 4311 4057 4213 4189 4081 4189 4706 4214 4459 4273 4127 26451 4133 4244)

/*
 * GarHal CSGO Specific Kernel Driver by DreTaX.
 * Lectured by Life45
 */

#include "garhal.h"
#include "data.h"
#include "messages.h"
#include "communication.h"
#include "events.h"



NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject)
{
	DebugMessageNormal("======================================\n");
	DebugMessageNormal("Garhal CSGO External hack By DreTaX\n");
	DebugMessageNormal("Shutting down...\n");
	PsRemoveLoadImageNotifyRoutine(ImageLoadCallback);
	IoDeleteSymbolicLink(&dos);
	IoDeleteDevice(pDriverObject->DeviceObject);

	// Delete the processnotify routine
	PsSetCreateProcessNotifyRoutineEx(ProcessNotifyCallbackEx, TRUE);

	vector_free(&CSRSSList);

	if (OBRegisterHandle)
	{
		ObUnRegisterCallbacks(OBRegisterHandle);
		OBRegisterHandle = NULL;
	}

	/*DebugMessageNormal("Attempting to free used memory.\n");
	NTSTATUS status = FreeAllocatedMemory();
	if (status == STATUS_SUCCESS)
	{
		DebugMessageNormal("FreeMemory Succeeded.\n");
	}
	else
	{
		DebugMessageNormal("FreeMemory Failed!\n");
	}*/

	DebugMessageNormal("Shutdown Complete!\n");

	return STATUS_SUCCESS;
}

// Driver Entrypoint
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	DebugMessageNormal("======================================\n");
	DebugMessageNormal("Garhal CSGO External hack By DreTaX\n");
	DebugMessageNormal("Starting...\n");

	PsSetLoadImageNotifyRoutine(ImageLoadCallback);

	RtlInitUnicodeString(&dev, L"\\Device\\garhalop");
	RtlInitUnicodeString(&dos, L"\\DosDevices\\garhalop");

	IoCreateDevice(pDriverObject, 0, &dev, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDeviceObject);
	IoCreateSymbolicLink(&dos, &dev);

	pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateCall;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseCall;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControl;
	pDriverObject->DriverUnload = UnloadDriver;

	pDeviceObject->Flags |= DO_DIRECT_IO;
	pDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	NTSTATUS reg = RegisterOBCallback();
	if (reg == STATUS_SUCCESS)
	{
		DebugMessageNormal("ObRegisterCallbacks Succeeded.\n");
	}
	else
	{
		DebugMessageNormal("ObRegisterCallbacks Failed!\n");
	}

	// Set up the CreateProcess routine.
	PsSetCreateProcessNotifyRoutineEx(ProcessNotifyCallbackEx, FALSE);

	// Change in data.h to enable.
	if (EnableProcessHiding == 1)
	{
		DebugMessageNormal("Process hiding feature is enabled.\n");
		DebugMessageNormal("You may get BSOD if you have the Windows PatchGuard running. (Very frequently.)\n");
	}
	else
	{
		DebugMessageNormal("Process hiding feature is disabled.\n");
	}

	// Change in data.h to enable.
	if (EnableDriverHiding == 1)
	{
		PLDR_DATA_TABLE_ENTRY CurDriverEntry = (PLDR_DATA_TABLE_ENTRY)pDriverObject->DriverSection;
		PLDR_DATA_TABLE_ENTRY NextDriverEntry = (PLDR_DATA_TABLE_ENTRY)CurDriverEntry->InLoadOrderLinks.Flink;
		PLDR_DATA_TABLE_ENTRY PrevDriverEntry = (PLDR_DATA_TABLE_ENTRY)CurDriverEntry->InLoadOrderLinks.Blink;

		PrevDriverEntry->InLoadOrderLinks.Flink = CurDriverEntry->InLoadOrderLinks.Flink;
		NextDriverEntry->InLoadOrderLinks.Blink = CurDriverEntry->InLoadOrderLinks.Blink;

		CurDriverEntry->InLoadOrderLinks.Flink = (PLIST_ENTRY)CurDriverEntry;
		CurDriverEntry->InLoadOrderLinks.Blink = (PLIST_ENTRY)CurDriverEntry;

		DebugMessageNormal("Driver hiding feature is enabled.\n");
		DebugMessageNormal("You may get BSOD if you have the Windows PatchGuard running. (10-30mins)\n");
	}
	else
	{
		DebugMessageNormal("Driver hiding feature is disabled.\n");
	}

	DebugMessageNormal("Successfully started!\n");

	return STATUS_SUCCESS;
}

NTSTATUS RegisterOBCallback()
{
	OB_OPERATION_REGISTRATION OBOperationRegistration;
	OB_CALLBACK_REGISTRATION OBOCallbackRegistration;
	REG_CONTEXT regContext;
	UNICODE_STRING usAltitude;
	memset(&OBOperationRegistration, 0, sizeof(OB_OPERATION_REGISTRATION));
	memset(&OBOCallbackRegistration, 0, sizeof(OB_CALLBACK_REGISTRATION));
	memset(&regContext, 0, sizeof(REG_CONTEXT));
	regContext.ulIndex = 1;
	regContext.Version = 120;
	RtlInitUnicodeString(&usAltitude, L"666");

	NTSTATUS Status = STATUS_UNSUCCESSFUL;

	if ((USHORT)ObGetFilterVersion() == OB_FLT_REGISTRATION_VERSION)
	{
		OBOperationRegistration.ObjectType = PsProcessType; // Use To Strip Handle Permissions For Threads PsThreadType
		OBOperationRegistration.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
		OBOperationRegistration.PostOperation = NULL;
		OBOperationRegistration.PreOperation = OBRegisterCallback;

		OBOCallbackRegistration.Altitude = usAltitude;
		OBOCallbackRegistration.OperationRegistration = &OBOperationRegistration;
		OBOCallbackRegistration.RegistrationContext = &regContext;
		OBOCallbackRegistration.Version = OB_FLT_REGISTRATION_VERSION;
		OBOCallbackRegistration.OperationRegistrationCount = (USHORT)1;

		Status = ObRegisterCallbacks(&OBOCallbackRegistration, &OBRegisterHandle); // Register The CallBack
	}

	return Status;
}

// Todo: Continue.
NTSTATUS FreeAllocatedMemory()
{
	try
	{
		if (csgoId)
		{
			MmFreeNonCachedMemory(csgoId, sizeof(csgoId));
		}

		if (ClientAddress)
		{
			MmFreeNonCachedMemory(ClientAddress, sizeof(ClientAddress));
		}

		if (EngineAddress)
		{
			MmFreeNonCachedMemory(EngineAddress, sizeof(EngineAddress));
		}

		if (ClientSize)
		{
			MmFreeNonCachedMemory(ClientSize, sizeof(ClientSize));
		}

		if (EngineSize)
		{
			MmFreeNonCachedMemory(EngineSize, sizeof(EngineSize));
		}

		if (ControllerID)
		{
			MmFreeNonCachedMemory(ControllerID, sizeof(ControllerID));
		}

		if (RankReaderID)
		{
			MmFreeNonCachedMemory(RankReaderID, sizeof(RankReaderID));
		}

		if (pDeviceObject)
		{
			MmFreeNonCachedMemory(pDeviceObject, sizeof(pDeviceObject));
		}
	}
	except(EXCEPTION_EXECUTE_HANDLER)
	{
		return STATUS_UNSUCCESSFUL;
	}


	return STATUS_SUCCESS;
}