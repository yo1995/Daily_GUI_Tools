#ifndef __IMPORTS_H__
#define __IMPORTS_H__

#include "hackbase.h"

typedef HRESULT (__stdcall* myD3D10CreateDeviceAndSwapChain)(IDXGIAdapter *pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, UINT SDKVersion, DXGI_SWAP_CHAIN_DESC *pSwapChainDesc, IDXGISwapChain **ppSwapChain, ID3D11Device **ppDevice);
typedef HRESULT (__stdcall* myD3DCompile)(LPCVOID pSrcData, SIZE_T SrcDataSize, LPCSTR pSourceName, const D3D_SHADER_MACRO *pDefines, ID3DInclude *pInclude, LPCSTR pEntrypoint, LPCSTR pTarget, UINT Flags1, UINT Flags2, ID3DBlob **ppCode, ID3DBlob *ppErrorMsgs);
typedef IDirect3D9* (__stdcall* myDirect3DCreate9)(UINT SDKVersion);
typedef HRESULT (__stdcall* myD3DXCreateFont)(IDirect3DDevice9 *pDevice, INT Height, UINT Width, UINT Weight, UINT MipLevels, BOOL Italic, DWORD CharSet, DWORD OutputPrecision, DWORD Quality, DWORD PitchAndFamily, LPCTSTR pFacename, LPD3DXFONT *ppFont);
typedef HRESULT (__stdcall* myD3D10CompileEffectFromMemory)(void *pData, SIZE_T DataLength, LPCSTR pSrcFileName, const D3D10_SHADER_MACRO *pDefines, ID3D10Include *pInclude, UINT HLSLFlags, UINT FXFlags, ID3D10Blob **ppCompiledEffect, ID3D10Blob **ppErrors);
typedef HRESULT (__stdcall* myD3D10CreateEffectFromMemory)(void *pData, SIZE_T DataLength, UINT FXFlags, ID3D10Device *pDevice, ID3D10EffectPool *pEffectPool, ID3D10Effect **ppEffect);
typedef HRESULT (__stdcall* myD3D10CreateStateBlock)(ID3D10Device *pDevice, D3D10_STATE_BLOCK_MASK *pStateBlockMask, ID3D10StateBlock **ppStateBlock);
typedef HRESULT (__stdcall* myD3D10StateBlockMaskEnableAll)(D3D10_STATE_BLOCK_MASK *pMask);
typedef HRESULT (__stdcall* myD3DX10CreateFont)(ID3D10Device *pDevice, INT Height, UINT Width, UINT Weight, UINT MipLevels, BOOL Italic, UINT CharSet, UINT OutputPrecision, UINT Quality, UINT PitchAndFamily, LPCTSTR pFaceName, LPD3DX10FONT *ppFont);

class Imports {
private:
	Imports();

public:
	static Imports* Singleton();
	~Imports();

	myD3D10CreateDeviceAndSwapChain D3D10CreateDeviceAndSwapChain;
	myD3DCompile D3DCompile;
	myDirect3DCreate9 Direct3DCreate9;
	myD3DXCreateFont D3DXCreateFont;
	myD3D10CompileEffectFromMemory D3D10CompileEffectFromMemory;
	myD3D10CreateEffectFromMemory D3D10CreateEffectFromMemory;
	myD3D10CreateStateBlock D3D10CreateStateBlock;
	myD3D10StateBlockMaskEnableAll D3D10StateBlockMaskEnableAll;
	myD3DX10CreateFont D3DX10CreateFont;
};

#endif