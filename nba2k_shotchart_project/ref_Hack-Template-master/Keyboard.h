#pragma once

#include <Windows.h>


namespace Keyboard {
	HHOOK hHook;

	LRESULT CALLBACK KeyboardHook(int Code, WPARAM Type, LPARAM Keyboard)
	{
		if (Code >= HC_ACTION) {
			// type is the key you pressed.
			PKBDLLHOOKSTRUCT Key = (PKBDLLHOOKSTRUCT)Keyboard;

			
		}

		return CallNextHookEx(NULL, Code, Type, Keyboard);
	}

	void HookKeyboard()
	{
		hHook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardHook, NULL, 0);

		MSG msg;
		while (!GetMessageA(&msg, NULL, NULL, NULL)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
};