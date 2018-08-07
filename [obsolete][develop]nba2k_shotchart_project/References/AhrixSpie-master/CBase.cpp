#include "CMenu.h"
#include "CSound.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

cMenu Menu;
copt opt;
cSound Sound;

float ScreenCenterX = 0.0f;
float ScreenCenterY = 0.0f;

void PreReset( LPDIRECT3DDEVICE9 pDevice )
{ 
	Menu.PreReset();
	return;
}

void PostReset( LPDIRECT3DDEVICE9 pDevice )
{ 
	Menu.PostReset(pDevice);
	return;
}

typedef HRESULT ( WINAPI* oReset )( LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters );
oReset pReset;

HRESULT APIENTRY myReset( LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters )
{
	_asm pushad;

	if( Tools.pLine )
        Tools.pLine->OnLostDevice();

	HRESULT hRet = pReset( pDevice, pPresentationParameters );
	
	if( SUCCEEDED( hRet ) )
	{
		if( Tools.pLine )
			Tools.pLine->OnResetDevice();
	}
	_asm popad;

	return hRet;
}

typedef HRESULT	( WINAPI* oEndScene )( LPDIRECT3DDEVICE9 pDevice );
oEndScene pEndScene;

HRESULT APIENTRY myEndScene( LPDIRECT3DDEVICE9 pDevice )
{
	_asm pushad;

	if(!Tools.Init)
	{
			D3DXCreateLine(pDevice,&Tools.pLine);
			Tools.Init = TRUE;
	}
		PostReset(pDevice);
		Menu.ShowMenu(pDevice);
		PreReset(pDevice);

		pDevice->GetViewport(&Tools.g_ViewPort);
		ScreenCenterX = (float)Tools.g_ViewPort.Width / 2;
		ScreenCenterY = (float)Tools.g_ViewPort.Height / 2;

	if(opt.d3d.cross)
	{
		Tools.FillRGB(ScreenCenterX-14, ScreenCenterY, 10, 1,D3DXCOLOR(1.0, 0.0, 0.0, 1.0),pDevice);//Left line
		Tools.FillRGB(ScreenCenterX+5, ScreenCenterY, 10, 1,D3DXCOLOR(1.0, 0.0, 0.0, 1.0),pDevice);//Right line
		Tools.FillRGB(ScreenCenterX, ScreenCenterY-14, 1, 10,D3DXCOLOR(1.0, 0.0, 0.0, 1.0),pDevice);//Top line
		Tools.FillRGB(ScreenCenterX, ScreenCenterY+5, 1, 10,D3DXCOLOR(1.0, 0.0, 0.0, 1.0),pDevice);//Bottom line
		Tools.DrawCircle(D3DXVECTOR2(ScreenCenterX,ScreenCenterY),8,60,D3DXCOLOR(1.0, 0.0, 0.0, 1.0));//Circle
		Tools.DrawPoint(ScreenCenterX,ScreenCenterY,1,1,D3DXCOLOR(0.0, 1.0, 0.0, 1.0),pDevice);//Point
	}
	_asm popad;

	return pEndScene( pDevice );
}

VOID WINAPI DIRECT3D()
{
	PDWORD D3DVTable;
	do
	{
		*(DWORD*)&D3DVTable = *(DWORD*)Tools.D3D9VTable();
	}
	while ( !D3DVTable );
	
	pReset		=	(oReset)	Tools.DetourCreate((PBYTE)D3DVTable[16],(PBYTE)myReset,5);
	pEndScene	=	(oEndScene)	Tools.DetourCreate((PBYTE)D3DVTable[42],(PBYTE)myEndScene,5);
}

BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpReserved)
{
	switch(dwReason)
	{
		case DLL_PROCESS_ATTACH:
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DIRECT3D, hDll, NULL, NULL);

			GetModuleFileNameA( hDll, dlldir, 512 );
			for(int i = ( int )strlen( dlldir ); i > 0; i--)
			{
				if(dlldir[i] == '\\')
				{ 
					dlldir[i+1] = 0;
					break; 
				}
			}
		break;

		case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}	
