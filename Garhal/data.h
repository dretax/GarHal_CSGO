#pragma once
#include <ntifs.h>

// We store every value here.
// Thanks Zeromem for the tuts, and Life45 for explaining.

ULONG csgoId, ClientAddress, EngineAddress;
PDEVICE_OBJECT pDeviceObject;
UNICODE_STRING dev, dos;
PDEVICE_OBJECT DeviceObject;


// Constants
#define EnableProcessNotifyCallbackEx 0 // Used to hide our process.
#define EnableDriverHiding 0 // Used to hide our driver.