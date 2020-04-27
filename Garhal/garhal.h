#pragma once
#include "communication.h"
#include "memory.h"

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject);

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);