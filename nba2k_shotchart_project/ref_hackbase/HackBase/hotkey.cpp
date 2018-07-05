#include "trainerbase.h"



bool IsKeyDown(DWORD key) {
	if (GetAsyncKeyState(key)) {
		return true;
	}
	return false;
}



void UpdateHotkeys() {
	if (IsKeyDown(VK_F8)) {
		switch (clear_screen)
		{
		case all_clear:
			clear_screen = no_clear;
			break;
		case clear_shotchart:
			clear_screen = all_clear;
			break;
		case no_clear:
			clear_screen = clear_shotchart;
			break;
		default:
			clear_screen = no_clear;
			break;
		}
	}
	IsKeyDown('Z') ? made_shot_Z_down = true : made_shot_Z_down = false;
}
