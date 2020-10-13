#pragma once
#include "utils.h"

namespace Driver
{
	template <typename T>
	T write(HANDLE handle, DWORD64 address, T* buffer)
	{
		T result;
		result = WriteProcessMemory(handle, (PVOID)(address), buffer, sizeof(T), NULL);

		return result;
	}


	template <typename T>
	T read(HANDLE handle, DWORD64 address, T* OutBuffer = NULL)
	{

		if (OutBuffer != NULL)
		{
			ReadProcessMemory(handle, (PVOID)(address), OutBuffer, sizeof(T), NULL);

			return *OutBuffer;
		}

		T result;
		ReadProcessMemory(handle, (PVOID)(address), &result, sizeof(T), NULL);

		return result;
	}
}
