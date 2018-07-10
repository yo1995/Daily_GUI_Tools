#include "hackbase.h"

DXGIHook *mSingleton = 0;
HackBase *mDXGIBase = 0;

DXGIHook *DXGIHook::Singleton() {
	mDXGIBase = HackBase::Singleton();
	if(!mSingleton) {
		if(mDXGIBase)
			mSingleton = new DXGIHook();
		else
			dbglogln("Error creating DXGIHook!");
	}
	return mSingleton;
}

typedef HRESULT (__stdcall* DXGIPresent_t)(IDXGISwapChain *SwapChain, UINT SyncInterval, UINT Flags);
DXGIPresent_t pDXGIPresent;

DWORD Present = NULL;
DWORD SwapChain = NULL;

bool DXGIFailedGettingSwapChain = false;


HRESULT __stdcall hkDXGIPresent(IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags) {
	__asm pushad

	static bool FirstRun = true;
	if(FirstRun) {
		FirstRun = false;

		ID3D10Device *pD3D10Device = 0;
		ID3D11Device *pD3D11Device = 0;

		if(!FAILED(pSwapChain->GetDevice(__uuidof(pD3D11Device), (void**)&pD3D11Device))) {
			SAFE_RELEASE(pD3D11Device);
			mDXGIBase->setD3DVersion(D3DVersion_D3D11);
			mDXGIBase->setRenderer(new D3D11Renderer(pSwapChain));
		} else if(!FAILED(pSwapChain->GetDevice(__uuidof(pD3D10Device), (void**)&pD3D10Device))) {
			SAFE_RELEASE(pD3D10Device);
			mDXGIBase->setD3DVersion(D3DVersion_D3D10);
			mDXGIBase->setRenderer(new D3D10Renderer(pSwapChain));
		}
	} else {
		if(mDXGIBase->getRenderer()) {
			mDXGIBase->getRenderer()->BeginScene();

			if(pSwapChain) {
				switch(mDXGIBase->getD3DVersion()) {
				case D3DVersion_D3D10:
					((D3D10Renderer*)mDXGIBase->getRenderer())->RefreshData(pSwapChain);
					break;

				case D3DVersion_D3D11:
					((D3D11Renderer*)mDXGIBase->getRenderer())->RefreshData(pSwapChain);
					break;
				}
			}

			if(mDXGIBase->mOnRender)
				mDXGIBase->mOnRender(mDXGIBase->getRenderer());

			mDXGIBase->getRenderer()->EndScene();
		}
		else {
			dbglogln("Unable to render: Renderer == NULL");
			MessageBox(0, "Error!", "", 0);
			ExitProcess(0);
		}
	}

	__asm popad
	return pDXGIPresent(pSwapChain, SyncInterval, Flags);
}

LRESULT CALLBACK DXGIMsgProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam){return DefWindowProc(hwnd, uMsg, wParam, lParam);}

DXGIHook::DXGIHook() {
	if(Imports::Singleton()->D3D10CreateDeviceAndSwapChain) {
		IDXGISwapChain *pSwapChain = 0;
		ID3D11Device *pDevice = 0;

		WNDCLASSEXA wc = {sizeof(WNDCLASSEX),CS_CLASSDC,DXGIMsgProc,0L,0L,GetModuleHandleA(NULL),NULL,NULL,NULL,NULL,"DX",NULL};
		RegisterClassExA(&wc);
		HWND hWnd = CreateWindowA("DX",NULL,WS_OVERLAPPEDWINDOW,100,100,300,300,NULL,NULL,wc.hInstance,NULL);
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = 1;
		swapChainDesc.BufferDesc.Height = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = TRUE;

		if(!FAILED(Imports::Singleton()->D3D10CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_REFERENCE, NULL, NULL, D3D10_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice))) {
			DWORD *pVTable = (DWORD*)pSwapChain;
			pVTable = (DWORD*)pVTable[0];
			Present = pVTable[8];
		}

		SAFE_RELEASE(pSwapChain);
		SAFE_RELEASE(pDevice);

		DestroyWindow(hWnd);
	}

	if(!Present)
		return;

	DWORD hookAddress = CheckPointer(Present);
#ifdef DEBUGLOG
	if(hookAddress != Present) {
		dbglogln("Hooking IDXGISwapChain::Present\nat [0x%08X -> 0x%08X]", Present, hookAddress);
	} else {
		dbglogln("Hooking IDXGISwapChain::Present\nat [0x%08X]", Present);
	}
#endif
	pDXGIPresent = (DXGIPresent_t)DetourCreate((LPVOID)hookAddress, hkDXGIPresent, DETOUR_TYPE_PUSH_RET);
}

DXGIHook::~DXGIHook()
{
	if(pDXGIPresent) {
#ifdef DEBUGLOG
		dbglogln("Removing IDXGISwapChain::Present public hook");
#endif
		DetourRemove(pDXGIPresent);
		pDXGIPresent = NULL;
	}
}