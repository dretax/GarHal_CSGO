#pragma warning( disable : 4100 4047 4024 4022 4201 4311 4057 4213 4189 4081 4189 4706 4214 4459 4273)

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


NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject)
{
	DbgPrintEx(0, 0, "======================================\n");
	DbgPrintEx(0, 0, "Garhal CSGO External hack By DreTaX\n");
	DbgPrintEx(0, 0, "Lectures by Life45\n");
	DbgPrintEx(0, 0, "Shutting down...\n");
	PsRemoveLoadImageNotifyRoutine(ImageLoadCallback);
	IoDeleteSymbolicLink(&dos);
	IoDeleteDevice(pDriverObject->DeviceObject);

	// Delete the processnotify routine
	PsSetCreateProcessNotifyRoutineEx(ProcessNotifyCallbackEx, TRUE);
	return STATUS_SUCCESS;
}

NTSTATUS CreateCall(PDEVICE_OBJECT DeviceObject, PIRP irp)
{
	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;

	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP irp)
{
	irp->IoStatus.Status = STATUS_SUCCESS;
	irp->IoStatus.Information = 0;

	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}


// Driver Entrypoint
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,
	PUNICODE_STRING pRegistryPath)
{
	DbgPrintEx(0, 0, "======================================\n");
	DbgPrintEx(0, 0, "Garhal CSGO External hack By DreTaX\n");
	DbgPrintEx(0, 0, "Lectures by Life45\n");
	DbgPrintEx(0, 0, "Starting...\n");

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

	// set up createprocess routine
	PsSetCreateProcessNotifyRoutineEx(ProcessNotifyCallbackEx, FALSE);

	return STATUS_SUCCESS;
}
