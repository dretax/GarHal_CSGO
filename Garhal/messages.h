#pragma once

#include <ntdef.h>
#define DebugMessageNormal(x, ...) DbgPrintEx(0, 0, x, __VA_ARGS__)
