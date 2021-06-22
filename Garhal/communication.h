#pragma once
#include <ntifs.h>
#include <ntdef.h>
#include "ioctlcodes.h"
#include "ioctldatastructs.h"

// Declare the IoControl function.
NTSTATUS IoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);

NTSTATUS CloseCall(PDEVICE_OBJECT DeviceObject, PIRP irp);

NTSTATUS CreateCall(PDEVICE_OBJECT DeviceObject, PIRP irp);
