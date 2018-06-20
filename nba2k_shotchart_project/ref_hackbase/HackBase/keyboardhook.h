// obsolete, cannot figure out how to deal with the thread-specific keyboard hook. TAT

#pragma once

#include <Windows.h>

KBDLLHOOKSTRUCT Key; // just read from it and do not write in the outer functions.
// might be a bad practice but whatever.

#pragma data_seg(".shared")
HHOOK hHook = NULL;
#pragma data_seg()


namespace Keyboard {
	// HHOOK hHook = NULL;

	LRESULT CALLBACK KeyboardHook(int Code, WPARAM Type, LPARAM Keyboard) {
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
					MessageBox(NULL, "F7 is pressed!", "key pressed", MB_ICONINFORMATION);
				}
			}	
		}
		return CallNextHookEx(hHook, Code, Type, Keyboard);
	}


	void SetHookKeyboard(const String ExecuteableFile, HINSTANCE g_hinstDLL, DWORD threadID) {
		// still not correct
		// HINSTANCE hinst = GetModuleHandle(NULL);
		if (!(hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHook, g_hinstDLL, 0))) {
			MessageBox(NULL, "Failed to install keyboard hook!", "Error", MB_ICONERROR);
		}
		// mind the message loop! without it you cannot grab key msg.
		
		MSG msg;
		while (!GetMessageA(&msg, NULL, NULL, NULL)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
	}

	void ReleaseHookKeyboard() {
		BOOL release = UnhookWindowsHookEx(hHook);
		if (!release) MessageBox(NULL, "Failed to release keyboard hook!", "Error", MB_ICONERROR);
	}
};
