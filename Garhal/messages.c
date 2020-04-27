#pragma warning( disable : 4100 4047 4024 4022 4201 4311 4057 4213 4189 4081 4189 4706 4214 4459 4273)

#include "messages.h"
#include <ntddk.h>


void DebugMessageNormal(PCSTR message)
{
	DbgPrintEx(0, 0, message);
}
