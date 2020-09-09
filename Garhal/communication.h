#pragma once
#include <ntifs.h>
#include <ntdef.h>

// Request to read virtual user memory (memory of a program) from kernel space
#define IO_READ_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0666 /* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request to write virtual user memory (memory of a program) from kernel space
#define IO_WRITE_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0667 /* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request to retrieve the process id of csgo process, from kernel space
#define IO_GET_ID_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0668 /* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request to retrieve the base address of client.dll in csgo.exe from kernel space
#define IO_GET_MODULE_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0669 /* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request to retrieve the base address of engine.dll in csgo.exe from kernel space
#define IO_GET_ENGINE_MODULE_REQUEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0670 /* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request size of Client.dll
#define IO_GET_MODULE_REQUEST_LENGTH CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0671 /* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request size of Engine.dll
#define IO_GET_ENGINE_MODULE_REQUEST_LENGTH CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0672 /* Our Custom Code */, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Order driver to apply full protection on Controller
#define IO_PROTECT_CONTROLLER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0673 /* Our Custom Code */, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

// Order driver to apply full protection on RankReader
#define IO_PROTECT_RANKREADER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0674 /* Our Custom Code */, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

// Declare the IoControl function.
NTSTATUS IoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);

NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP irp);

NTSTATUS CreateCall(PDEVICE_OBJECT DeviceObject, PIRP irp);


// datatype for read request
typedef struct _KERNEL_READ_REQUEST
{
	ULONG ProcessId;

	ULONG Address;
	PVOID pBuff;
	ULONG Size;

} KERNEL_READ_REQUEST, * PKERNEL_READ_REQUEST;

typedef struct _KERNEL_WRITE_REQUEST
{
	ULONG ProcessId;

	ULONG Address;
	PVOID pBuff;
	ULONG Size;

} KERNEL_WRITE_REQUEST, * PKERNEL_WRITE_REQUEST;