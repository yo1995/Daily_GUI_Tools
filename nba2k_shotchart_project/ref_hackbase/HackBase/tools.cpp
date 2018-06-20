#include "hackbase.h"

WindowsVersion getWindowsVersion() {
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
		return WindowsVersion_XP;
	else if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
		return WindowsVersion_Vista;
	else if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
		return WindowsVersion_Win7;
	else if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
		return WindowsVersion_Win8;
	//else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3)
	//	return WindowsVersion_Win8;
	//else if (osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0) //https://msdn.microsoft.com/en-us/library/windows/desktop/ms724834%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
	//	return WindowsVersion_Win10;
	return WindowsVersion_Unknown;
}

void createConsole(const char *Title) {
	AllocConsole();
	SetConsoleTitle(Title);
	int hConHandle;
	long lStdHandle;
	FILE *fp;
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
}

HWND GetConsoleHwnd() {
#define MY_BUFSIZE 1024
	HWND hwndFound;
	char pszNewWindowTitle[MY_BUFSIZE];
	char pszOldWindowTitle[MY_BUFSIZE];
	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);
	wsprintf(pszNewWindowTitle, "%d/%d", GetTickCount(), GetCurrentProcessId());
	SetConsoleTitle(pszNewWindowTitle);
	Sleep(40);
	hwndFound = FindWindow(NULL, pszNewWindowTitle);
	SetConsoleTitle(pszOldWindowTitle);
	return(hwndFound);
}

HMODULE GetD3DX9() {
	if(LoadLibrary("D3DX9_45.dll"))
		return GetModuleHandle("D3DX9_45.dll");
	else if(LoadLibrary("D3DX9_44.dll"))
		return GetModuleHandle("D3DX9_44.dll");
	else if(LoadLibrary("D3DX9_43.dll"))
		return GetModuleHandle("D3DX9_43.dll");
	else if(LoadLibrary("D3DX9_42.dll"))
		return GetModuleHandle("D3DX9_42.dll");
	else if(LoadLibrary("D3DX9_41.dll"))
		return GetModuleHandle("D3DX9_41.dll");
	else if(LoadLibrary("D3DX9_40.dll"))
		return GetModuleHandle("D3DX9_40.dll");
	else if(LoadLibrary("D3DX9_39.dll"))
		return GetModuleHandle("D3DX9_39.dll");
	else if(LoadLibrary("D3DX9_38.dll"))
		return GetModuleHandle("D3DX9_38.dll");
	else if(LoadLibrary("D3DX9_37.dll"))
		return GetModuleHandle("D3DX9_37.dll");
	else if(LoadLibrary("D3DX9_36.dll"))
		return GetModuleHandle("D3DX9_36.dll");
	else if(LoadLibrary("D3DX9_35.dll"))
		return GetModuleHandle("D3DX9_35.dll");
	else if(LoadLibrary("D3DX9_34.dll"))
		return GetModuleHandle("D3DX9_34.dll");
	else if(LoadLibrary("D3DX9_33.dll"))
		return GetModuleHandle("D3DX9_33.dll");
	else if(LoadLibrary("D3DX9_32.dll"))
		return GetModuleHandle("D3DX9_32.dll");
	else if(LoadLibrary("D3DX9_31.dll"))
		return GetModuleHandle("D3DX9_31.dll");
	else if(LoadLibrary("D3DX9_30.dll"))
		return GetModuleHandle("D3DX9_30.dll");
	else
		return NULL;
}

HMODULE GetD3DX10() {
	if(LoadLibrary("D3DX10_45.dll"))
		return GetModuleHandle("D3DX10_45.dll");
	else if(LoadLibrary("D3DX10_44.dll"))
		return GetModuleHandle("D3DX10_44.dll");
	else if(LoadLibrary("D3DX10_43.dll"))
		return GetModuleHandle("D3DX10_43.dll");
	else if(LoadLibrary("D3DX10_42.dll"))
		return GetModuleHandle("D3DX10_42.dll");
	else if(LoadLibrary("D3DX10_41.dll"))
		return GetModuleHandle("D3DX10_41.dll");
	else if(LoadLibrary("D3DX10_40.dll"))
		return GetModuleHandle("D3DX10_40.dll");
	else if(LoadLibrary("D3DX10_39.dll"))
		return GetModuleHandle("D3DX10_39.dll");
	else if(LoadLibrary("D3DX10_38.dll"))
		return GetModuleHandle("D3DX10_38.dll");
	else if(LoadLibrary("D3DX10_37.dll"))
		return GetModuleHandle("D3DX10_37.dll");
	else if(LoadLibrary("D3DX10_36.dll"))
		return GetModuleHandle("D3DX10_36.dll");
	else if(LoadLibrary("D3DX10_35.dll"))
		return GetModuleHandle("D3DX10_35.dll");
	else if(LoadLibrary("D3DX10_34.dll"))
		return GetModuleHandle("D3DX10_34.dll");
	else if(LoadLibrary("D3DX10_33.dll"))
		return GetModuleHandle("D3DX10_33.dll");
	else if(LoadLibrary("D3DX10_32.dll"))
		return GetModuleHandle("D3DX10_32.dll");
	else if(LoadLibrary("D3DX10_31.dll"))
		return GetModuleHandle("D3DX10_31.dll");
	else if(LoadLibrary("D3DX10_30.dll"))
		return GetModuleHandle("D3DX10_30.dll");
	else
		return NULL;
}

HMODULE GetD3DX11() {
	if(LoadLibrary("D3DX11_45.dll"))
		return GetModuleHandle("D3DX11_45.dll");
	else if(LoadLibrary("D3DX11_44.dll"))
		return GetModuleHandle("D3DX11_44.dll");
	else if(LoadLibrary("D3DX11_43.dll"))
		return GetModuleHandle("D3DX11_43.dll");
	else if(LoadLibrary("D3DX11_42.dll"))
		return GetModuleHandle("D3DX11_42.dll");
	else if(LoadLibrary("D3DX11_41.dll"))
		return GetModuleHandle("D3DX11_41.dll");
	else if(LoadLibrary("D3DX11_40.dll"))
		return GetModuleHandle("D3DX11_40.dll");
	else if(LoadLibrary("D3DX11_39.dll"))
		return GetModuleHandle("D3DX11_39.dll");
	else if(LoadLibrary("D3DX11_38.dll"))
		return GetModuleHandle("D3DX11_38.dll");
	else if(LoadLibrary("D3DX11_37.dll"))
		return GetModuleHandle("D3DX11_37.dll");
	else if(LoadLibrary("D3DX11_36.dll"))
		return GetModuleHandle("D3DX11_36.dll");
	else if(LoadLibrary("D3DX11_35.dll"))
		return GetModuleHandle("D3DX11_35.dll");
	else if(LoadLibrary("D3DX11_34.dll"))
		return GetModuleHandle("D3DX11_34.dll");
	else if(LoadLibrary("D3DX11_33.dll"))
		return GetModuleHandle("D3DX11_33.dll");
	else if(LoadLibrary("D3DX11_32.dll"))
		return GetModuleHandle("D3DX11_32.dll");
	else if(LoadLibrary("D3DX11_31.dll"))
		return GetModuleHandle("D3DX11_31.dll");
	else if(LoadLibrary("D3DX11_30.dll"))
		return GetModuleHandle("D3DX11_30.dll");
	else
		return NULL;
}

HMODULE GetD3DCompiler() {
	if(LoadLibrary("D3DCompiler_45.dll"))
		return GetModuleHandle("D3DCompiler_45.dll");
	else if(LoadLibrary("D3DCompiler_44.dll"))
		return GetModuleHandle("D3DCompiler_44.dll");
	else if(LoadLibrary("D3DCompiler_43.dll"))
		return GetModuleHandle("D3DCompiler_43.dll");
	else if(LoadLibrary("D3DCompiler_42.dll"))
		return GetModuleHandle("D3DCompiler_42.dll");
	else if(LoadLibrary("D3DCompiler_41.dll"))
		return GetModuleHandle("D3DCompiler_41.dll");
	else if(LoadLibrary("D3DCompiler_40.dll"))
		return GetModuleHandle("D3DCompiler_40.dll");
	else if(LoadLibrary("D3DCompiler_39.dll"))
		return GetModuleHandle("D3DCompiler_39.dll");
	else if(LoadLibrary("D3DCompiler_38.dll"))
		return GetModuleHandle("D3DCompiler_38.dll");
	else if(LoadLibrary("D3DCompiler_37.dll"))
		return GetModuleHandle("D3DCompiler_37.dll");
	else if(LoadLibrary("D3DCompiler_36.dll"))
		return GetModuleHandle("D3DCompiler_36.dll");
	else if(LoadLibrary("D3DCompiler_35.dll"))
		return GetModuleHandle("D3DCompiler_35.dll");
	else if(LoadLibrary("D3DCompiler_34.dll"))
		return GetModuleHandle("D3DCompiler_34.dll");
	else if(LoadLibrary("D3DCompiler_33.dll"))
		return GetModuleHandle("D3DCompiler_33.dll");
	else if(LoadLibrary("D3DCompiler_32.dll"))
		return GetModuleHandle("D3DCompiler_32.dll");
	else if(LoadLibrary("D3DCompiler_31.dll"))
		return GetModuleHandle("D3DCompiler_31.dll");
	else if(LoadLibrary("D3DCompiler_30.dll"))
		return GetModuleHandle("D3DCompiler_30.dll");
	else
		return NULL;
}

void WriteMemory(PVOID dwAdd, void *val, int bytes) {
	DWORD d, ds;
	VirtualProtect(dwAdd, bytes, PAGE_EXECUTE_READWRITE, &d);
	memcpy(dwAdd, val, bytes);
	VirtualProtect(dwAdd,bytes,d,&ds);
}

#pragma warning (push)
#pragma warning (disable: 4996)
DWORD GetModuleSize(LPSTR strModuleName) {
	MODULEENTRY32 lpme = {0};
	DWORD dwSize = 0;
	DWORD PID = GetCurrentProcessId();
	BOOL isMod = 0;
	char chModName[256];

	strcpy_s(chModName, 255, strModuleName);
	_strlwr_s(chModName, 255);

	HANDLE hSnapshotModule=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE ,PID);
	if (hSnapshotModule)
	{
		lpme.dwSize=sizeof(lpme);
		isMod=Module32First(hSnapshotModule,&lpme);
		while(isMod)
		{
			if (strcmp(_strlwr(lpme.szExePath),chModName))
			{
				dwSize=(DWORD)lpme.modBaseSize;
				CloseHandle(hSnapshotModule);
				return dwSize;
			}
			isMod=Module32Next(hSnapshotModule,&lpme);
		}
	}
	CloseHandle(hSnapshotModule);

	return 0;
}
#pragma warning (pop)

DWORD FindPattern(char *ModuleName, char *pattern, char *mask) {
	return FindPattern((DWORD)GetModuleHandle(ModuleName), GetModuleSize(ModuleName), pattern, mask);
}

DWORD FindPattern(DWORD startAddres, DWORD fileSize, char *pattern, char *mask) {
	DWORD pos = 0;
	int searchLen = strlen(mask) - 1;

	for (DWORD retAddress = startAddres; retAddress < startAddres + fileSize; retAddress++) {
		if (*(PBYTE)retAddress == ((PBYTE)pattern)[pos] || mask[pos] == '?') {
			if (mask[ pos + 1 ] == '\0')
				return (retAddress - searchLen);

			pos++;
		}
		else
			pos = 0;
	}
	return NULL;
}

struct EnumWindowsCallbackArgs {
	EnumWindowsCallbackArgs(DWORD p) : pid(p) { }
	const DWORD pid;
	std::vector<HWND> handles;
};

static BOOL CALLBACK EnumWindowsCallback(HWND hnd, LPARAM lParam) {
	EnumWindowsCallbackArgs *args = (EnumWindowsCallbackArgs*)lParam;

	DWORD windowPID;
	GetWindowThreadProcessId(hnd, &windowPID);
	if (windowPID == args->pid)
		args->handles.push_back(hnd);

	return TRUE;
}

std::vector<HWND> getToplevelWindows() {
	EnumWindowsCallbackArgs args(GetCurrentProcessId());
	if (EnumWindows(&EnumWindowsCallback, (LPARAM)&args) == FALSE)
		return std::vector<HWND>();
	return args.handles;
}