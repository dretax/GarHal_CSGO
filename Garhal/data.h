#pragma once
#include <ntifs.h>

// We store every value here.
// Thanks Zeromem for the tuts, and Life45 for explaining.

ULONG csgoId, ClientAddress;

PDEVICE_OBJECT pDeviceObject;
UNICODE_STRING dev, dos;

PDEVICE_OBJECT DeviceObject;