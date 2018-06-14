#include <Windows.h>
#include <Psapi.h>
#include <vector>
#include <string>
#include <iostream>

#define MAX_CLASSNAME 255
#define MAX_WNDNAME MAX_CLASSNAME

using namespace std;

struct OverlayFinderParams {
	DWORD pidOwner = NULL;
	wstring wndClassName = L"";
	wstring wndName = L"";
	RECT pos = { 0, 0, 0, 0 }; // GetSystemMetrics with SM_CXSCREEN and SM_CYSCREEN can be useful here
	POINT res = { 0, 0 };
	float percentAllScreens = 0.0f;
	float percentMainScreen = 0.0f;
	DWORD style = NULL;
	DWORD styleEx = NULL;
	bool satisfyAllCriteria = false;
	vector<HWND> hwnds;
};

BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam);
vector<HWND> OverlayFinder(OverlayFinderParams params);

int main() {
	cout << "Search for suspicious windows presenting the characteristics of game cheats overlays." << endl;
	cout << "Play with it to try to detect your own overlay and improve your system accordingly." << endl;
	cout << endl;

	OverlayFinderParams params;
	params.style = WS_VISIBLE;
	params.styleEx = WS_EX_LAYERED | WS_EX_TRANSPARENT;
	params.percentMainScreen = 90.0f;
	params.satisfyAllCriteria = true;
	vector<HWND> hwnds = OverlayFinder(params);

	cout << "Searching for windows WS_VISIBLE, WS_EX_LAYERED, ES_EX_TRANSPARENT, taking 90%+ of the screen..." << endl;
	cout << endl;

	for (int i(0); i < hwnds.size(); ++i) {
		DWORD pid = 0;
		DWORD tid = GetWindowThreadProcessId(hwnds[i], &pid);
		cout << "Window #" << i + 1 << " found: HWND 0x" << hex << (int)hwnds[i] << " | Thread: " << dec << tid << " | PID: " << pid << endl;
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
		if (hProcess) {
			char cheatPath[MAX_PATH] = "";
			GetProcessImageFileNameA(hProcess, (LPSTR)&cheatPath, MAX_PATH);
			CloseHandle(hProcess);
			string cheatPathStr = cheatPath;
			cout << cheatPathStr << endl;
		}
		cout << "----------------" << endl;
	}

	cout << endl;
	system("pause");

	return EXIT_SUCCESS;
}

BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam) {
	OverlayFinderParams& params = *(OverlayFinderParams*)lParam;

	unsigned char satisfiedCriteria = 0, unSatisfiedCriteria = 0;

	// If looking for windows of a specific PDI
	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);
	if (params.pidOwner != NULL)
		if (params.pidOwner == pid)
			++satisfiedCriteria; // Doesn't belong to the process targeted
		else
			++unSatisfiedCriteria;

	// If looking for windows of a specific class
	wchar_t className[MAX_CLASSNAME] = L"";
	GetClassName(hwnd, className, MAX_CLASSNAME);
	wstring classNameWstr = className;
	if (params.wndClassName != L"")
		if (params.wndClassName == classNameWstr)
			++satisfiedCriteria; // Not the class targeted
		else
			++unSatisfiedCriteria;

	// If looking for windows with a specific name
	wchar_t windowName[MAX_WNDNAME] = L"";
	GetWindowText(hwnd, windowName, MAX_CLASSNAME);
	wstring windowNameWstr = windowName;
	if (params.wndName != L"")
		if (params.wndName == windowNameWstr)
			++satisfiedCriteria; // Not the class targeted
		else
			++unSatisfiedCriteria;

	// If looking for window at a specific position
	RECT pos;
	GetWindowRect(hwnd, &pos);
	if (params.pos.left || params.pos.top || params.pos.right || params.pos.bottom)
		if (params.pos.left == pos.left && params.pos.top == pos.top && params.pos.right == pos.right && params.pos.bottom == pos.bottom)
			++satisfiedCriteria;
		else
			++unSatisfiedCriteria;

	// If looking for window of a specific size
	POINT res = { pos.right - pos.left, pos.bottom - pos.top };
	if (params.res.x || params.res.y)
		if (res.x == params.res.x && res.y == params.res.y)
			++satisfiedCriteria;
		else
			++unSatisfiedCriteria;

	// If looking for windows taking more than a specific percentage of all the screens
	float ratioAllScreensX = res.x / GetSystemMetrics(SM_CXSCREEN);
	float ratioAllScreensY = res.y / GetSystemMetrics(SM_CYSCREEN);
	float percentAllScreens = ratioAllScreensX * ratioAllScreensY * 100;
	if (params.percentAllScreens != 0.0f)
		if (percentAllScreens >= params.percentAllScreens)
			++satisfiedCriteria;
		else
			++unSatisfiedCriteria;

	// If looking for windows taking more than a specific percentage or the main screen
	RECT desktopRect;
	GetWindowRect(GetDesktopWindow(), &desktopRect);
	POINT desktopRes = { desktopRect.right - desktopRect.left, desktopRect.bottom - desktopRect.top };
	float ratioMainScreenX = res.x / desktopRes.x;
	float ratioMainScreenY = res.y / desktopRes.y;
	float percentMainScreen = ratioMainScreenX * ratioMainScreenY * 100;
	if (params.percentMainScreen != 0.0f)
		if (percentAllScreens >= params.percentMainScreen)
			++satisfiedCriteria;
		else
			++unSatisfiedCriteria;

	// Looking for windows with specific styles
	LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
	if (params.style)
		if (params.style & style)
			++satisfiedCriteria;
		else
			++unSatisfiedCriteria;

	// Looking for windows with specific extended styles
	LONG_PTR styleEx = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
	if (params.styleEx)
		if (params.styleEx & styleEx)
			++satisfiedCriteria;
		else
			++unSatisfiedCriteria;

	if (!satisfiedCriteria)
		return TRUE;

	if (params.satisfyAllCriteria && unSatisfiedCriteria)
		return TRUE;

	// If looking for multiple windows
	params.hwnds.push_back(hwnd);
	return TRUE;
}

vector<HWND> OverlayFinder(OverlayFinderParams params) {
	EnumWindows(EnumWindowsCallback, (LPARAM)&params);
	return params.hwnds;
}
