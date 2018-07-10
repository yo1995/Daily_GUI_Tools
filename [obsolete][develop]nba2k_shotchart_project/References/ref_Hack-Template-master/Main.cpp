#include <Windows.h>
#include <iostream>

#include "detours.h"
#include "Hack.h"
#include "Keyboard.h"
#include "D3D9.h"

void HookFunctions()
{

}

void HookConsole()
{
	AllocConsole();

	FILE* file;

	freopen_s(&file, "CONOUT$", "w", stdout);
	freopen_s(&file, "CONOUT$", "w", stderr);
	freopen_s(&file, "CONIN$", "r", stdin);
}

void UpdateDMAs()
{

}

void UpdateHacks()
{

}

void Main(HMODULE hModule)
{
	HookConsole();
	HookFunctions();

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Keyboard::HookKeyboard, NULL, 0, NULL);

	D3D9::Hook();

	while (true) {
		UpdateDMAs();
		UpdateHacks();

		Sleep(10);
	}
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);

	if (dwReason == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Main, hModule, 0, NULL);
	}

	return TRUE;
}