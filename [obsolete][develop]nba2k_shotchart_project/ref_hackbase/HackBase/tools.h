#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "hackbase.h"

WindowsVersion getWindowsVersion();

void createConsole(const char *Title);
HWND GetConsoleHwnd();

HMODULE GetD3DX9();
HMODULE GetD3DX10();
HMODULE GetD3DX11();
HMODULE GetD3DCompiler();

void WriteMemory(PVOID dwAdd, void *val, int bytes);

DWORD GetModuleSize(LPSTR strModuleName);
DWORD FindPattern(char *ModuleName, char *pattern, char *mask);
DWORD FindPattern(DWORD startAddres, DWORD fileSize, char *pattern, char *mask);

std::vector<HWND> getToplevelWindows();

#endif