#include "trainerbase.h"

bool IsKeyDown(DWORD key) {
	if (GetAsyncKeyState(key)) {
		return true;
	}
	return false;
}

void UpdateHotkey(HackBase *mHackBase) {
	// Renderer *renderer_original = mHackBase->getRenderer();
	if (IsKeyDown(VK_F7)) {

		made_shot_prediction ? (F7_text = F7_text_f) : (F7_text = F7_text_t);
		// renderer是根据每一帧刷新进行hook的，如果想要长时间留在屏幕上，要么让render卡住，要么重复在每一帧写直到多帧
		// mHackBase->getRenderer()->DrawText((int)(0.5 * mHackBase->getRenderer()->GetWidth()), (int)(0.5 * mHackBase->getRenderer()->GetHeight()), FontColor_default, F7_text);

		mHackBase->setOnRender(onRender_F7);
		Sleep(1000);
		mHackBase->setOnRender(onRender_dashboard);
		made_shot_prediction = !made_shot_prediction;
	}
	if (IsKeyDown(VK_F8)) {
		mHackBase->setOnRender(onRender_clear);
	}
}
