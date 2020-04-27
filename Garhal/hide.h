#pragma once
#include <ntifs.h>


void RemoveLinks(PLIST_ENTRY Current);

PCHAR HideProcess(UINT32 pid);

ULONG FindPIDOffset();