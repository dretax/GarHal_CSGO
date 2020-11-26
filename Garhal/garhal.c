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
#include "ntos.h"
#include "gstructs.h"

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject)
{
	DebugMessageNormal("======================================\n");
	DebugMessageNormal("Garhal CSGO External hack By DreTaX\n");
	DebugMessageNormal("Shutting down...\n");

	if (!IsManualMapped) 
	{
		PsRemoveLoadImageNotifyRoutine(ImageLoadCallback);
	}
	
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

	DebugMessageNormal("Shutdown Complete!\n");

	return STATUS_SUCCESS;
}

// Driver Entrypoint
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	DebugMessageNormal("======================================\n");
	DebugMessageNormal("Garhal CSGO External hack By DreTaX\n");
	DebugMessageNormal("Starting...\n");

	if (!IsManualMapped) 
	{
		PsSetLoadImageNotifyRoutine(ImageLoadCallback);
	}

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

// Rename the current entry point to DriverInitialize to use this, and remove the unload call registration.
/*NTSTATUS DriverEntry(
	_In_  struct _DRIVER_OBJECT* DriverObject,
	_In_  PUNICODE_STRING RegistryPath
	)
{
	// These parameters are invalid due to nonstandard way of loading and should not be used.
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	IsManualMapped = TRUE;

	DebugMessageNormal("Garhal is swimming as a manual mapped driver, system range start is %p, code mapped at %p\n", MmSystemRangeStart, DriverEntry);


	// This isn't a standard way against better anticheats such as BE, and EAC.
	// Could give you a good example though.
	UNICODE_STRING  drvName;
	NTSTATUS status;

	RtlInitUnicodeString(&drvName, L"\\Driver\\Garhal");
	status = IoCreateDriver(&drvName, &DriverInitialize);

	if (NT_SUCCESS(status))
	{
		DebugMessageNormal("Created driver.\n");
	}

	return status;
}*/


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