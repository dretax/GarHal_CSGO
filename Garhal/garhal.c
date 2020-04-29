#pragma warning( disable : 4100 4047 4024 4022 4201 4311 4057 4213 4189 4081 4189 4706 4214 4459 4273 4127)

/*
 * GarHal CSGO Specific Kernel Driver by DreTaX.
 * Lectured by Life45
 */

#include "garhal.h"
#include <ntddk.h>
#include <ntdef.h>
#include "communication.h"
#include "ntos.h"
#include "data.h"
#include "events.h"
#include "messages.h"


NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject)
{
	DebugMessageNormal("======================================\n");
	DebugMessageNormal("Garhal CSGO External hack By DreTaX\n");
	DebugMessageNormal("Lectures by Life45\n");
	DebugMessageNormal("Shutting down...\n");
	PsRemoveLoadImageNotifyRoutine(ImageLoadCallback);
	IoDeleteSymbolicLink(&dos);
	IoDeleteDevice(pDriverObject->DeviceObject);

	// Delete the processnotify routine
	if (EnableProcessNotifyCallbackEx == 1) 
	{
		PsSetCreateProcessNotifyRoutineEx(ProcessNotifyCallbackEx, TRUE);
	}
	return STATUS_SUCCESS;
}

// Driver Entrypoint
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	DebugMessageNormal("======================================\n");
	DebugMessageNormal("Garhal CSGO External hack By DreTaX\n");
	DebugMessageNormal("Lectures by Life45\n");
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

	// Set up the CreateProcess routine. Change in data.h to enable.
	if (EnableProcessNotifyCallbackEx == 1) 
	{
		DebugMessageNormal("Process hiding feature is enabled.\n");
		DebugMessageNormal("You may get BSOD if you have the Windows PatchGuard running. (Very frequently.)\n");
		PsSetCreateProcessNotifyRoutineEx(ProcessNotifyCallbackEx, FALSE);
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

	return STATUS_SUCCESS;
}
