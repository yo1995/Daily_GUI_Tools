#pragma once

#include <Windows.h>
#include <Psapi.h>
#include "Hack.h"

namespace Memory
{
	bool Compare(LPBYTE lpData, LPBYTE lpMask, LPSTR szMask)
	{
		if (lpData == nullptr || lpMask == nullptr || szMask == nullptr)
			return FALSE;

		for (; *szMask; ++szMask, ++lpData, ++lpMask)
			if (*szMask == 'x' && *lpData != *lpMask)
				return FALSE;

		return ((*szMask) == 0) ? true : false;
	}

	ADDRESS FindPattern(ADDRESS dwStartAddress, ADDRESS dwSearchLen, LPBYTE lpMask, LPSTR szMask)
	{
		if (dwStartAddress == 0 || dwSearchLen == 0 || lpMask == nullptr || szMask == nullptr)
			return 0;

		for (UINT i = 0; i <= dwSearchLen; i++)
			if (Compare((LPBYTE)(dwStartAddress + i), lpMask, szMask))
				return (dwStartAddress + i);

		return 0;
	}

	ADDRESS GetModuleLength(LPCWSTR ModuleName)
	{
		HMODULE hModule = GetModuleHandle(ModuleName);
		MODULEINFO modInfo;

		GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(modInfo));

		return modInfo.SizeOfImage;
	}
}