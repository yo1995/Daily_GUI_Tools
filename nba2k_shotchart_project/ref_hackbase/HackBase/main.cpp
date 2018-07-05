#pragma comment (lib, "..\\Release\\HackBase.lib")
#pragma comment(lib, "d3dx9.lib") // for the helly Drawline function. actually no need, improve later

#include "hackbase.h"
#include "trainerbase.h"

HackBase *mHackBase = 0;
SaveData *mSaveData = 0;
HMODULE g_d3d9Module = NULL;

bool d3d9hijack(HMODULE hModule) {
	TCHAR processPath[MAX_PATH];
	TCHAR msg[MAX_PATH + 20];
	GetModuleFileName(GetModuleHandle(NULL), processPath, MAX_PATH);
	if (VERBOSEMODE) {
		_tcscpy_s(msg, _T("注入了进程 "));
		_tcscat_s(msg, processPath);
		MessageBox(NULL, msg, _T(""), MB_OK);
	}
	
	char dllPath[MAX_PATH];
	GetSystemDirectory(dllPath, MAX_PATH);
	strcat_s(dllPath, "\\d3d9.dll");
	
	// 加载原DLL，获取真正的Direct3DCreate9地址
	g_d3d9Module = LoadLibrary(dllPath);
	RealDirect3DCreate9 = (Direct3DCreate9Type)GetProcAddress(g_d3d9Module, "Direct3DCreate9");
	if (RealDirect3DCreate9 == NULL)
	{
		MessageBox(NULL, _T("获取Direct3DCreate9地址失败"), _T("Error"), MB_ICONERROR);
		return false;
	}
	return true;
}

void init_main() {
	Sleep(8000);  // a silly way to load my trainer after game itself initialized.
	char exename[] = "nba2k11.exe"; //GFXTest32.exe nba2k11.exe
	char windowname[] = "NBA 2K11"; //Renderer: [DirectX9], Input: [Raw input], 32 bits  NBA 2K11
	mHackBase = HackBase::Singleton();  // new a hackbase to hook d3d
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
	// create a filename class to handle record adata file.
	mSaveData = new SaveData();

	while (true) { //infinite loop! might affect the performance.
		UpdateDMAs(pHandle_r, mSaveData);
		// keys after dma cause we need to depend on Z down
		UpdateHotkeys();
		UpdateDMA_afterKeyDown(pHandle_r, pHandle_w, mSaveData);
		UpdateGraphics(mHackBase);
		Sleep(200); // loop will only start again after 1/5 of a second
	}
}

BOOL InitInstance(HMODULE hModule) {
	if (!d3d9hijack(hModule)) {
		MessageBox(0, "d3d9 DLL hijack failed!", "Error", MB_ICONERROR);
		return false;
	}
	return true;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hinstDLL);
			if (!InitInstance(hinstDLL)) {
				return false;
			};
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init_main, 0, 0, 0);
			break;
		case DLL_PROCESS_DETACH:
			SAFE_DELETE(mHackBase);
			if (VERBOSEMODE) {
				MessageBox(NULL, _T("DLL卸载中"), _T(""), MB_OK);
			}
			FreeLibrary(g_d3d9Module);
			break;
	}
	return true;
}


/*samples */
// MessageBox(0, "We are here hehe.", "nana...", 0);
/*

template<typename T>
const bool WriteMem(const SIZE_T Address, const T value)
{
DWORD bytesRead;
WriteProcessMemory(hProcess, (LPVOID)Address, &value, sizeof(T), &bytesRead);
return bytesRead == sizeof(T);
};
*/