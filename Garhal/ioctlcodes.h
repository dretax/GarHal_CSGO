#pragma once
// Device type, you can also use your own code instead of FILE_DEVICE_UNKNOWN (values of less than 0x8000 are reserved for Microsoft).
#define IOCTL_DRIVER_DEVICE FILE_DEVICE_UNKNOWN

// Function code, identifies the function to be performed by the driver (values of less than 0x800 are reserved for Microsoft).
#define IOCTL_FIRST_INDEX (0x800)

// Request to read virtual user memory (memory of a program) from kernel space
#define IO_READ_REQUEST CTL_CODE(IOCTL_DRIVER_DEVICE, (IOCTL_FIRST_INDEX + 0x1), METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request to write virtual user memory (memory of a program) from kernel space
#define IO_WRITE_REQUEST CTL_CODE(IOCTL_DRIVER_DEVICE, (IOCTL_FIRST_INDEX + 0x2), METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request to retrieve the process id of csgo process, from kernel space
#define IO_GET_ID_REQUEST CTL_CODE(IOCTL_DRIVER_DEVICE, (IOCTL_FIRST_INDEX + 0x3), METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request to retrieve the base address of client.dll in csgo.exe from kernel space
#define IO_GET_MODULE_REQUEST CTL_CODE(IOCTL_DRIVER_DEVICE, (IOCTL_FIRST_INDEX + 0x4), METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request to retrieve the base address of engine.dll in csgo.exe from kernel space
#define IO_GET_ENGINE_MODULE_REQUEST CTL_CODE(IOCTL_DRIVER_DEVICE, (IOCTL_FIRST_INDEX + 0x5), METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request size of Client.dll
#define IO_GET_MODULE_REQUEST_LENGTH CTL_CODE(IOCTL_DRIVER_DEVICE, (IOCTL_FIRST_INDEX + 0x6), METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Request size of Engine.dll
#define IO_GET_ENGINE_MODULE_REQUEST_LENGTH CTL_CODE(IOCTL_DRIVER_DEVICE, (IOCTL_FIRST_INDEX + 0x7), METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

// Order driver to apply full protection on Controller
#define IO_PROTECT_CONTROLLER CTL_CODE(IOCTL_DRIVER_DEVICE, (IOCTL_FIRST_INDEX + 0x8), METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

// Order driver to apply full protection on RankReader
#define IO_PROTECT_RANKREADER CTL_CODE(IOCTL_DRIVER_DEVICE, (IOCTL_FIRST_INDEX + 0x9), METHOD_OUT_DIRECT, FILE_ANY_ACCESS)