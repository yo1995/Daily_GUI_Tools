#include "HackBase.h"

HackBase* mSingleton = 0;

void onRenderEvent(Renderer *renderer) {
	HackBase *hackbase = HackBase::Singleton();

	static bool FirstRun = true;
	if(FirstRun) {
		FirstRun = false;
		hackbase->cleanUpD3DHooks();

#ifdef DEBUGLOG
		hackbase->getRenderer()->loglnDebugInfo();

		switch(getWindowsVersion()) {
		case WindowsVersion_XP:
			logln("Win version: XP");
			break;
		case WindowsVersion_Vista:
			logln("Win version: Vista");
			break;
		case WindowsVersion_Win7:
			logln("Win version: Win7");
			break;
		case WindowsVersion_Win8:
			logln("Win version: Win8 or greater");
			break;
		}
		switch(hackbase->getD3DVersion()) {
		case D3DVersion_D3D9:
			logln("D3D version: D3D9");
			break;
		case D3DVersion_D3D10:
			logln("D3D version: D3D10");
			break;
		case D3DVersion_D3D11:
			logln("D3D version: D3D11");
			break;
		}

		if(renderer)
			logln("Viewport:    %dx%d", renderer->GetWidth(), renderer->GetHeight());
		else
			logln("Viewport:    unknown");
		logln();
#endif
	}

	hackbase->onRender(renderer);
}

HackBase* HackBase::Singleton(){
	if(!mSingleton)
		mSingleton = new HackBase();
	return mSingleton;
}

void HackBase::cleanUpD3DHooks() {
	D3DVersion mD3DVer = this->getD3DVersion();
	if(mD3DVer != D3DVersion_D3D9)
		SAFE_DELETE(this->mD3D9Hook);
	if(mD3DVer != D3DVersion_D3D10 && mD3DVer != D3DVersion_D3D11)
		SAFE_DELETE(this->mDXGIHook);
}

HackBase::HackBase() {
	DisableThreadLibraryCalls(GetModuleHandle(NULL));

	mConsoleWindow = 0;
#ifdef LOGTOCONSOLE
	createConsole("");
	mConsoleWindow = GetConsoleWindow();
#endif

#ifdef LOGTOFILE
	remove(LOGTOFILE);
#endif

	/*
	logln(decode(XOR("\x7C\x74\x21\x7C\x4E\x40\x60\x7E\x4C\x4E\x6B\x7D\x40\x5D\x7B\x7D\x7E\x4E\x42\x7E"
		"\x4D\x4D\x29\x61\x4F\x29\x21\x79\x4F\x28\x6F\x7D\x5D\x60\x2D\x51\x4E\x28\x29\x40"
		"\x4B\x70\x2D\x4F\x4B\x73\x68\x57\x5A\x5A\x5D\x4A\x4B\x4D\x74\x52\x5D\x5D\x6C\x49"
		"\x4E\x4E\x5A\x4A\x4B\x4E\x5A\x7C\x4E\x74\x6C\x7E\x4B\x73\x28\x49\x4D\x29\x6B\x4A"
		"\x4F\x5E\x5E\x55\x4B\x29\x55\x4A\x56\x59\x25\x25", 24)).c_str()); // HackBase by ChaosLeader - visit http://www.unknowncheats.me/forum/
	*/
	this->mRenderer = 0;

	this->onRender = 0;
	this->mOnRender = onRenderEvent;

	this->mD3DVersion = D3DVersion_None;

	this->mImports = Imports::Singleton();
}

bool HackBase::Initialize(void (__cdecl *onRender)(Renderer *), const String ExecuteableFile, const String WindowName, const String WindowClass) {
	if(ExecuteableFile) {
		if(!GetModuleHandle(ExecuteableFile)) {
			logln("Process mismatch! Aborting...");
			SAFE_DELETE(mSingleton);
			return false;
		}
	}

	if(WindowName || WindowClass) {
		if(!isWindowHooked(WindowName, WindowClass)) {
			logln("Window mismatch! Aborting...");
			SAFE_DELETE(mSingleton);
			return false;
		}
	}

	this->onRender = onRender;

	this->mD3D9Hook = D3D9Hook::Singleton();
	this->mDXGIHook = DXGIHook::Singleton();

	return true;
}

void HackBase::setOnRender(void(__cdecl *onRender)(Renderer *)) {
	this->onRender = onRender;
}

// hey you cannot just close me like that!
/*
void HackBase::unsetOnRender() {
	this->onRender = 0;
}
*/

HackBase::~HackBase() {
	SAFE_DELETE(this->mD3D9Hook);
	SAFE_DELETE(this->mDXGIHook);
	SAFE_DELETE(this->mRenderer);
	SAFE_DELETE(this->mImports);

	mSingleton = 0;
}

bool HackBase::isWindowHooked(String WindowName, String ClassName) {
	if(!WindowName && !ClassName)
		return false;

    HWND mWindow = FindWindow(ClassName, WindowName);
    if(!mWindow)
		return false;

	std::vector<HWND> mWindows = getToplevelWindows();

    for(uint i = 0; i < mWindows.size(); i++)
		if(mWindows.at(i) == mWindow)
			return true;

    return false;
}