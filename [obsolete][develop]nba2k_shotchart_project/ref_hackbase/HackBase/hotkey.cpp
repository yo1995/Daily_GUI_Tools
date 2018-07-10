#include "trainerbase.h"

bool IsKeyDown(DWORD key) {
	if (GetAsyncKeyState(key)) {
		return true;
	}
	return false;
}

void UpdateHotkeys(int &delay) {
	//IsKeyDown('Z') ? made_shot_Z_down = true : made_shot_Z_down = false;
	if (IsKeyDown('Z')) {  // first time detection
		made_shot_Z_down = true;
		delay = 1;
	}

	if (delay != 0) {
		if (delay > 5) {
			made_shot_Z_down = false;
			delay = 0;
		}
		++delay;
	}

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
	if (IsKeyDown(VK_F5)) {
		record_mode == 1 ? record_mode = 2 : record_mode = 1;
	}
}
