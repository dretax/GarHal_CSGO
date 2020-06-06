#pragma once
#include <ntifs.h>

PVOID OBRegisterHandle = NULL;

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject);

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);

NTSTATUS RegisterOBCallback();

NTSTATUS FreeAllocatedMemory();