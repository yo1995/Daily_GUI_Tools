#pragma comment (lib, "..\\Release\\HackBase.lib")
#pragma comment(lib, "d3dx9.lib") // for the helly Drawline function.

#include "hackbase.h"
#include "hotkey.h"

HackBase *mHackBase = 0;
char inject_text[] = "injected and hooked by yo1995.";

/*
void onRender(Renderer *renderer) {
	// renderer->DrawBorder(0, 0, renderer->GetWidth(), renderer->GetHeight(), 5, GREEN(255));
	// renderer->DrawLine(0, 0, renderer->GetWidth(), renderer->GetHeight(), 2, false, Color(255, 0, 255, 0));
	// renderer->DrawText((int)(0.5 * renderer->GetWidth()), (int)(0.5 * renderer->GetHeight()), inject_text);
	// renderer->DrawCircle((int)(0.5 * renderer->GetWidth()), (int)(0.5 * renderer->GetHeight()), 30, 2, 30, Color(255, 0, 255, 0));
}
*/


void UpdateDMAs() {
	// aka direct memory access. manipulate the memory region.

}

void UpdateHacks() {

}

void init_main() {
	char exename[] = "nba2k11.exe"; //GFXTest32.exe nba2k11.exe
	char windowname[] = "NBA 2K11"; //Renderer: [DirectX9], Input: [Raw input], 32 bits  NBA 2K11
	mHackBase = HackBase::Singleton();
	if (!mHackBase->Initialize(onRender, exename, windowname))
		MessageBox(0, "Error hooking game. Maybe injected into wrong process...", "Failed to hook...", MB_ICONERROR);
	// MessageBox(0, "We are here hehe.", "nana...", 0);
	/*
	while (true) {
		UpdateDMAs();
		UpdateHacks();
		Sleep(10);
	}
	*/
	while (true) { //infinite loop! might affect the performance.
		UpdateHotkey(mHackBase);
		
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