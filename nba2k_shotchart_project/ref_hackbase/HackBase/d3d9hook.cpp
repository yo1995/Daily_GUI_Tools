#include "hackbase.h"

D3D9Hook *mSingleton = 0;
HackBase *mD3D9Base = 0;

D3D9Hook *D3D9Hook::Singleton() {
	mD3D9Base = HackBase::Singleton();
	if(!mSingleton) {
		if(mD3D9Base)
			mSingleton = new D3D9Hook();
		else
			dbglogln("Error creating D3D9Hook!");
	}
	return mSingleton;
}

typedef HRESULT(__stdcall* D3D9EndScene_t)(LPDIRECT3DDEVICE9);
D3D9EndScene_t pD3D9EndScene;

DWORD EndScene = NULL;
DWORD Device = NULL;

HRESULT __stdcall hkD3D9EndScene(IDirect3DDevice9 *pDevice) {
	__asm pushad

	static bool FirstRun = true;
	if(FirstRun) {
		FirstRun = false;
		if(mD3D9Base) {
			mD3D9Base->setD3DVersion(D3DVersion_D3D9);
			mD3D9Base->setRenderer(new D3D9Renderer(pDevice));
		}
	} else {
		if(mD3D9Base->getRenderer()) {
			mD3D9Base->getRenderer()->BeginScene();

			((D3D9Renderer*)mD3D9Base->getRenderer())->RefreshData(pDevice);

			if(mD3D9Base->mOnRender)
				mD3D9Base->mOnRender(mD3D9Base->getRenderer());
			
			mD3D9Base->getRenderer()->EndScene();
		} else {
			dbglogln("Unable to render: Renderer == NULL");
			MessageBox(0, "Error!", "", 0);
			ExitProcess(0);
		}
	}

	__asm popad
	return pD3D9EndScene(pDevice);
}

DWORD *D3D9VTable = NULL;

LRESULT CALLBACK D3D9MsgProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam){return DefWindowProc(hwnd, uMsg, wParam, lParam);}

D3D9Hook::D3D9Hook() {
	if(Imports::Singleton()->Direct3DCreate9) {
		WNDCLASSEXA wc = {sizeof(WNDCLASSEX),CS_CLASSDC,D3D9MsgProc,0L,0L,GetModuleHandleA(NULL),NULL,NULL,NULL,NULL,"DX",NULL};
		RegisterClassExA(&wc);
		HWND hWnd = CreateWindowA("DX",NULL,WS_OVERLAPPEDWINDOW,100,100,300,300,GetDesktopWindow(),NULL,wc.hInstance,NULL);
		LPDIRECT3D9 pD3D = Imports::Singleton()->Direct3DCreate9( D3D_SDK_VERSION );
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		IDirect3DDevice9 *pd3dDevice;
		pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&pd3dDevice);
		DWORD* pVTable = (DWORD*)pd3dDevice;
		pVTable = (DWORD*)pVTable[0];
		D3D9VTable = pVTable;

		DestroyWindow(hWnd);
		EndScene = D3D9VTable[42];
	} else {
		dbglogln("Failed to create D3D9 hook!");
		MessageBox(0, "Error!", "", 0);
		ExitProcess(0);
	}

	DWORD hookAddress = CheckPointer(EndScene);
#ifdef DEBUGLOG
	if(hookAddress != EndScene) {
		dbglogln("Hooking IDirect3DDevice9::EndScene\nat [0x%08X -> 0x%08X]", EndScene, hookAddress);
	} else {
		dbglogln("Hooking IDirect3DDevice9::EndScene\nat [0x%08X]", EndScene);
	}
#endif
	pD3D9EndScene = (D3D9EndScene_t)DetourCreate((LPVOID)hookAddress, hkD3D9EndScene, DETOUR_TYPE_PUSH_RET);
}

D3D9Hook::~D3D9Hook() {
	if(pD3D9EndScene) {
#ifdef DEBUGLOG
		dbglogln("Removing IDirect3DDevice9::EndScene public hook");
#endif
		DetourRemove(pD3D9EndScene);
		pD3D9EndScene = NULL;
	}
}