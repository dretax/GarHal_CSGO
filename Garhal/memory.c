#pragma warning( disable : 4100 4047 4024 4022 4201 4311 4057 4213 4189 4081 4189 4706 4214 4459 4273 4457)

#include "memory.h"
#include <minwindef.h>
#include <ntstatus.h>
#include <ntdef.h>
#include <stdlib.h>
#include "ntos.h"

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
	PEPROCESS sys_process = PsInitialSystemProcess;
	PEPROCESS cur_entry = sys_process;

	CHAR image_name[15];

	do
	{
		RtlCopyMemory((PVOID)(&image_name), (PVOID)((uintptr_t)cur_entry + 0x450) /*EPROCESS->ImageFileName*/, sizeof(image_name));

		if (strstr(image_name, process_name))
		{
			DWORD active_threads;
			RtlCopyMemory((PVOID)&active_threads, (PVOID)((uintptr_t)cur_entry + 0x498) /*EPROCESS->ActiveThreads*/, sizeof(active_threads));
			if (active_threads)
			{
				// Get PREPROCESS's ID.
				//ULONG procid = (ULONG) PsGetProcessId(cur_entry);
				vector_add(ls, cur_entry);
			}
		}

		PLIST_ENTRY list = (PLIST_ENTRY)((uintptr_t)(cur_entry)+0x2F0) /*EPROCESS->ActiveProcessLinks*/;
		cur_entry = (PEPROCESS)((uintptr_t)list->Flink - 0x2F0);

	} while (cur_entry != sys_process);

	return STATUS_SUCCESS;
}