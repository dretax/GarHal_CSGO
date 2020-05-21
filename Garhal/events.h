#pragma once
#include <ntifs.h>

PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallback(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo);

PCREATE_PROCESS_NOTIFY_ROUTINE_EX ProcessNotifyCallbackEx(HANDLE parentId, HANDLE processId, PPS_CREATE_NOTIFY_INFO notifyInfo);

OB_PREOP_CALLBACK_STATUS OBRegisterCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation);