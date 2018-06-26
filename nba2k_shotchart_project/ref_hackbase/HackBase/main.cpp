#pragma comment (lib, "..\\Release\\HackBase.lib")
#pragma comment(lib, "d3dx9.lib") // for the helly Drawline function. actually no need, improve later

#include "hackbase.h"
#include "trainerbase.h"

HackBase *mHackBase = 0;




void init_main() {
	char exename[] = "nba2k11.exe"; //GFXTest32.exe nba2k11.exe
	char windowname[] = "NBA 2K11"; //Renderer: [DirectX9], Input: [Raw input], 32 bits  NBA 2K11
	mHackBase = HackBase::Singleton();
	if (!mHackBase->Initialize(onRender_clear, exename, windowname)) {
		MessageBox(0, "Error hooking game. Maybe injected into wrong process...", "Failed to hook...", MB_ICONERROR);
		return;
	}
	HWND hWnd = FindWindowA(0, windowname);
	DWORD pid;
	GetWindowThreadProcessId(hWnd, &pid);
	// PROCESS_VM_READ: read only; PROCESS_VM_OPERATION: r/w - seems not working; PROCESS_VM_WRITE
	HANDLE pHandle_r = OpenProcess(PROCESS_VM_READ, false, pid);
	// https://www.unknowncheats.me/forum/c-and-c-/194439-writeprocessmemory.html
	HANDLE pHandle_w = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, pid);
	if (!hWnd || !pid) {
		MessageBox(0, "Error Finding Window.", "Window mismatch!", MB_ICONERROR);
		return;
	}
	while (true) { //infinite loop! might affect the performance.
		UpdateDMAs(pHandle_r);
		// keys after dma cause we need to depend on Z down
		UpdateHotkeys();
		UpdateDMA_afterKeyDown(pHandle_r, pHandle_w);
		UpdateGraphics(mHackBase);
		Sleep(200); // loop will only start again after 1/5 of a second
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init_main, 0, 0, 0);
		return true;
	case DLL_PROCESS_DETACH:
		SAFE_DELETE(mHackBase);
		return true;
	}
	return false;
}


/*samples */
// MessageBox(0, "We are here hehe.", "nana...", 0);
/*
while (true) {
UpdateDMAs();
UpdateHacks();
Sleep(10);
}


template<typename T>
const bool WriteMem(const SIZE_T Address, const T value)
{
DWORD bytesRead;
WriteProcessMemory(hProcess, (LPVOID)Address, &value, sizeof(T), &bytesRead);
return bytesRead == sizeof(T);
};
*/