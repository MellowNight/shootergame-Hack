#pragma once
#include "globals.h"


namespace Utils
{
	DWORD FindProcessId(const std::wstring& processName);



	uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
}