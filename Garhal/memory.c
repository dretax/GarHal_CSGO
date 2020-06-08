#pragma warning( disable : 4100 4047 4024 4022 4201 4311 4057 4213 4189 4081 4189 4706 4214 4459 4273 4457 4133)

#include "memory.h"
#include <minwindef.h>
#include <ntstatus.h>
#include <ntdef.h>
#include <stdlib.h>
#include "ntos.h"

ULONG GetWindowsBuildNumber()
{
	// Get the Windows Version table.
	RTL_OSVERSIONINFOEXW osversion;
	osversion.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
	RtlGetVersion(&osversion);

	return osversion.dwBuildNumber;
}

// https://docs.microsoft.com/en-us/windows/release-information/
// https://www.vergiliusproject.com/kernels/x64/Windows 10 | 2016/1903 19H1 (May 2019 Update)/_EPROCESS
int GetCorrectOffset(CHAR* Name, ULONG BuildNumber)
{
	// 1903 & 1909
	if (BuildNumber == 18362 || BuildNumber == 18363)
	{
		if (strcmp(Name, "ImageFileName") == 0)
		{
			return 0x450;
		}
		if (strcmp(Name, "ActiveThreads") == 0)
		{
			return 0x498;
		}
		if (strcmp(Name, "ActiveProcessLinks") == 0)
		{
			return 0x2F0;
		}
	}
	// 2004
	else if (BuildNumber == 19041)
	{
		if (strcmp(Name, "ImageFileName") == 0)
		{
			return 0x5a8;
		}
		if (strcmp(Name, "ActiveThreads") == 0)
		{
			return 0x5f0;
		}
		if (strcmp(Name, "ActiveProcessLinks") == 0)
		{
			return 0x448;
		}
	}

	return 0;
}

NTSTATUS KeReadVirtualMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size)
{
	PSIZE_T Bytes;
	if (NT_SUCCESS(MmCopyVirtualMemory(Process, SourceAddress, PsGetCurrentProcess(),
		TargetAddress, Size, KernelMode, &Bytes)))
	{
		return STATUS_SUCCESS;
	}
	return STATUS_ACCESS_DENIED;
}

NTSTATUS KeWriteVirtualMemory(PEPROCESS Process, PVOID SourceAddress, PVOID TargetAddress, SIZE_T Size)
{
	PSIZE_T Bytes;
	if (NT_SUCCESS(MmCopyVirtualMemory(PsGetCurrentProcess(), SourceAddress, Process,
		TargetAddress, Size, KernelMode, &Bytes)))
	{
		return STATUS_SUCCESS;
	}
	return STATUS_ACCESS_DENIED;
}

NTSTATUS FindProcessByName(CHAR* process_name, vector* ls)
{
	ULONG BuildNumber = GetWindowsBuildNumber();
	PEPROCESS sys_process = PsInitialSystemProcess;
	PEPROCESS cur_entry = sys_process;

	CHAR image_name[15];

	do
	{
		RtlCopyMemory((PVOID)(&image_name), (PVOID)((uintptr_t)cur_entry + GetCorrectOffset("ImageFileName", BuildNumber)) /*EPROCESS->ImageFileName*/, sizeof(image_name));

		if (strstr(image_name, process_name))
		{
			DWORD active_threads;
			RtlCopyMemory((PVOID)&active_threads, (PVOID)((uintptr_t)cur_entry + GetCorrectOffset("ActiveThreads", BuildNumber)) /*EPROCESS->ActiveThreads*/, sizeof(active_threads));
			if (active_threads)
			{
				vector_add(ls, cur_entry);
			}
		}

		PLIST_ENTRY list = (PLIST_ENTRY)((uintptr_t)(cur_entry)+GetCorrectOffset("ActiveProcessLinks", BuildNumber)) /*EPROCESS->ActiveProcessLinks*/;
		cur_entry = (PEPROCESS)((uintptr_t)list->Flink - GetCorrectOffset("ActiveProcessLinks", BuildNumber));

	} while (cur_entry != sys_process);

	return STATUS_SUCCESS;
}