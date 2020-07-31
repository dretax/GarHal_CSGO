#pragma once
#pragma warning( disable : 4101 4244)

#include "communications.hpp"

// The interface handler for IOCTL. Thanks Zeromem.
class KeInterface
{

public:
	HANDLE hDriver; // Handle to driver

					// Initializer
	KeInterface(LPCSTR RegistryPath)
	{
		hDriver = CreateFileA(RegistryPath, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	}

	template <typename type>
	type ReadVirtualMemory(ULONG ProcessId, ULONG ReadAddress, SIZE_T Size)
	{
		// allocate a buffer with specified type to allow our driver to write our wanted data inside this buffer
		type Buffer;

		DWORD Return, Bytes;
		KERNEL_READ_REQUEST ReadRequest;


		ReadRequest.ProcessId = ProcessId;
		ReadRequest.Address = ReadAddress;

		//send the 'address' of the buffer so our driver can know where to write the data
		ReadRequest.pBuff = &Buffer;

		ReadRequest.Size = Size;

		// send code to our driver with the arguments
		if (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0))
		{
			//return our buffer
			return Buffer;
		}
		return Buffer;
	}

	DWORD GetTargetPid()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;

		ULONG Id;
		DWORD Bytes;

		if (DeviceIoControl(hDriver, IO_GET_ID_REQUEST, &Id, sizeof(Id),
			&Id, sizeof(Id), &Bytes, NULL))
		{
			return Id;
		}

		return false;
	}

	DWORD GetClientModule()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;

		ULONG Address;
		DWORD Bytes;

		if (DeviceIoControl(hDriver, IO_GET_MODULE_REQUEST, &Address, sizeof(Address),
			&Address, sizeof(Address), &Bytes, NULL))
		{
			return Address;
		}
		return false;
	}

	DWORD GetEngineModule()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;

		ULONG Address;
		DWORD Bytes;

		if (DeviceIoControl(hDriver, IO_GET_ENGINE_MODULE_REQUEST, &Address, sizeof(Address),
			&Address, sizeof(Address), &Bytes, NULL))
		{
			return Address;
		}
		return false;
	}

	bool RequestProtectionRank()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;

		return DeviceIoControl(hDriver, IO_PROTECT_RANKREADER, 0, 0, 0, 0, 0, 0);
	}
};