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

	template <typename type>
	bool WriteVirtualMemory(ULONG ProcessId, ULONG WriteAddress, type WriteValue, SIZE_T WriteSize)
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;
		DWORD Bytes;

		KERNEL_WRITE_REQUEST  WriteRequest;
		WriteRequest.ProcessId = ProcessId;
		WriteRequest.Address = WriteAddress;

		//send driver the 'address' of our specified type WriteValue so our driver can copy the data we want to write from this address to WriteAddress
		WriteRequest.pBuff = &WriteValue;

		WriteRequest.Size = WriteSize;

		if (DeviceIoControl(hDriver, IO_WRITE_REQUEST, &WriteRequest, sizeof(WriteRequest), 0, 0, &Bytes, NULL)) 
		{
			return true;
		}
		return false;
	}

	bool ReadSpecial(ULONG ProcessId, DWORD dwAddress, LPVOID lpBuffer, DWORD dwSize)
	{
		SIZE_T Out = NULL;

		KERNEL_READ_REQUEST ReadRequest;

		ReadRequest.ProcessId = ProcessId;
		ReadRequest.Address = dwAddress;

		ReadRequest.pBuff = &lpBuffer;

		ReadRequest.Size = dwSize;

		return (DeviceIoControl(hDriver, IO_READ_REQUEST, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0) == TRUE);
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

	DWORD GetClientModuleSize()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;

		ULONG Address;
		DWORD Bytes;

		if (DeviceIoControl(hDriver, IO_GET_MODULE_REQUEST_LENGTH, &Address, sizeof(Address),
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

	DWORD GetEngineModuleSize()
	{
		if (hDriver == INVALID_HANDLE_VALUE)
			return false;

		ULONG Address;
		DWORD Bytes;

		if (DeviceIoControl(hDriver, IO_GET_ENGINE_MODULE_REQUEST_LENGTH, &Address, sizeof(Address),
			&Address, sizeof(Address), &Bytes, NULL))
		{
			return Address;
		}
		return false;
	}

	BOOL DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask)
	{
		for (; *pszMask; ++pszMask, ++pData, ++pMask)
		{
			if (*pszMask == 'x' && *pData != *pMask)
			{
				return FALSE;
			}
		}
		return (*pszMask == 0);
	}

	DWORD Scan(ULONG ProcessId, DWORD dwStart, DWORD dwSize, const char* pSignature, const char* pMask)
	{
		BYTE* pRemote = new BYTE[dwSize];
		if (!ReadSpecial(ProcessId, dwStart, pRemote, dwSize))
		{
			delete[] pRemote;
			return NULL;
		}
		for (DWORD dwIndex = 0; dwIndex < dwSize; dwIndex++)
		{
			if (DataCompare((const BYTE*)(pRemote + dwIndex), (const BYTE*)pSignature, pMask))
			{
				delete[] pRemote;
				return (dwStart + dwIndex);
			}
		}
		delete[] pRemote;
		return NULL;
	}
};