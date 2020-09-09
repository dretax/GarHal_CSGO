#pragma once
#include <ntifs.h>
#include "gstructs.h"
#include "vector.h"

ULONG GetWindowsBuildNumber();

int GetCorrectOffset(CHAR* Name, ULONG BuildNumber);

NTSTATUS KeReadVirtualMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size);

NTSTATUS KeWriteVirtualMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size);

NTSTATUS FindProcessByName(CHAR* process_name, vector* list);

MODULEENTRY GetProcessModule(PEPROCESS Process, LPCWSTR ModuleName);