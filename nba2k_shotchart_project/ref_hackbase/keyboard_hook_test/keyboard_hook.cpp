
#include "..\\HackBase\\hackbase.h"

#pragma data_seg (".shared")
static HHOOK hhook = NULL;
static HINSTANCE hins = NULL;
static HWND hwnd = NULL;
static bool globalKeys[256] = { false };
static int required[256] = { 0 };
static int nReq = 0;
#pragma data_seg ()
#pragma comment(linker, "/SECTION:.shared,RWS")

KBDLLHOOKSTRUCT Key;


// Finding target window


extern "C" LRESULT CALLBACK KeyboardHook(int Code, WPARAM Type, LPARAM Keyboard) 
{
	MessageBox(0, "We are here hehe.", "2", 0);
	PostMessage(hwnd, WM_USER + 755, Type, Keyboard);
	if (Code != HC_ACTION) {
		return CallNextHookEx(NULL, Code, Type, Keyboard);
		MessageBox(0, "We are here hehe.", "nana...", 0);
	}
	if (Code == HC_ACTION) {
		if (Type == WM_KEYDOWN || Type == WM_SYSKEYDOWN) {
			// MessageBox(0, "We are here hehe.", "nana...", 0);
			Key = *((KBDLLHOOKSTRUCT*)Keyboard);
			if (Key.vkCode == VK_F7) {
				// e.g. maybe do something here. maybe not.
				// 可以先试试按键然后显示
				MessageBox(NULL, "F7 is pressed!", "key pressed", MB_ICONINFORMATION);
			}
		}
	}
	return CallNextHookEx(hhook, Code, Type, Keyboard);
}

extern "C" void InstallHook(HWND hWnd)
{
	HWND hwnd = FindWindow(NULL, "Renderer: [DirectX9], Input: [Window Messages], 32 bits");
	if (hwnd != NULL) {
		MessageBox(0, "window found.", "0", 0);
	}
	DWORD pid = NULL;
	DWORD tid = GetWindowThreadProcessId(hwnd, &pid);
	hhook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardHook, hins, 0);
	MessageBox(0, "We are here hehe.", "1", 0);
	if (NULL == hhook) {
		TCHAR msg[256];
		wsprintf(msg, TEXT("Cannot install keyboard hook, code: %d"), GetLastError());
		MessageBox(hwnd, msg, TEXT("error"), MB_ICONERROR);
	}
	MSG msg;
	while (!GetMessageA(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

extern "C" void ReleaseHook()
{
	if (hhook != NULL) {
		BOOL bRes = UnhookWindowsHookEx(hhook);
		if (!bRes) MessageBox(hwnd, TEXT("Cannot remove keyboard hook."), TEXT("error"), MB_ICONERROR);
	}
}

bool WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	if (NULL == hins) hins = hInstance;
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)InstallHook, 0, 0, 0);
		return true;
	case DLL_PROCESS_DETACH:
		ReleaseHook();
		return true;
	}
	return false;
}
