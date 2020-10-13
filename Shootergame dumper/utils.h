#pragma once
#include	<Windows.h>
#include	<Psapi.h>
#include	<string>
#include	<fstream>
#include	<iomanip>
#include	<sstream>
#include	<locale>
#include	<codecvt>

namespace Utils
{
	MODULEINFO getModuleInfo(const LPCTSTR lpModuleName)
	{
		MODULEINFO  miInfos{ };
		auto* const hmModule = GetModuleHandle(lpModuleName);

		if (hmModule)
		{
			GetModuleInformation(GetCurrentProcess(), hmModule, &miInfos, sizeof(MODULEINFO));
		}

		return miInfos;
	}

	std::string ws2s(const std::wstring& wstr)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}
}