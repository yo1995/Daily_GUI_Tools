#include "trainerbase.h"
/*
本文件函数用于在按键改变flag后读写内存
*/

DWORD god_mode_byte_addr = 0x0097D9A3;	// nba2k11.exe+57D99D - C7 05 78FAD505 00000000 - mov [nba2k11.exe+595FA78],00000000 | just change the A3 byte to 01



void change_god_mode(HANDLE pHandle) {
	int enabled = 1;
	int disabled = 0;

	if (all_players_god_mode) {
		WriteProcessMemory(pHandle, (LPVOID)god_mode_byte_addr, &enabled, sizeof(enabled), 0);
	}
	else {
		WriteProcessMemory(pHandle, (LPVOID)god_mode_byte_addr, &disabled, sizeof(disabled), 0);
	}
}


void UpdateDMA_afterKeyDown(HANDLE pHandle_r, HANDLE pHandle_w) {
	if (IsKeyDown(VK_F7)) {
		// god mode
		all_players_god_mode ? (F7_text = F7_text_f) : (F7_text = F7_text_t);
		all_players_god_mode = !all_players_god_mode;
		change_god_mode(pHandle_w);
	}
}

/*
template<typename T>
void WriteMem(DWORD Address, T value)
{
DWORD Old;
VirtualProtect((LPVOID)Address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &Old);
//VirtualProtectEx(hProcess, (LPVOID)Address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &Old);
WriteProcessMemory(hProcess, (LPVOID)Address, &value, sizeof(T), NULL);
//VirtualProtectEx(hProcess, (LPVOID)Address, sizeof(DWORD), Old, &Old);
VirtualProtect((LPVOID)Address, sizeof(DWORD), Old, &Old);
};
*/