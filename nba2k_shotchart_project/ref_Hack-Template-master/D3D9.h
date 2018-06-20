#pragma once
#pragma comment(lib, "d3d9.lib")

#include <Windows.h>
#include <d3d9.h>
#include <iostream>

#include "detours.h"
#include "Hack.h"
#include "Memory.h"

typedef HRESULT(WINAPI* tEndScene)(LPDIRECT3DDEVICE9);
typedef HRESULT(WINAPI* tDIP)(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount);

LPDIRECT3DDEVICE9 pGameDevice;

tEndScene oEndScene;
tDIP oDIP;

namespace D3D9 {
	HRESULT GenerateTexture(IDirect3DDevice9* pD3Ddev, IDirect3DTexture9** ppD3Dtex, DWORD colour32)
	{
		if (FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, NULL)))
			return E_FAIL;

		WORD colour16 = ((WORD)((colour32 >> 28) & 0xF) << 12)
			| (WORD)(((colour32 >> 20) & 0xF) << 8)
			| (WORD)(((colour32 >> 12) & 0xF) << 4)
			| (WORD)(((colour32 >> 4) & 0xF) << 0);

		D3DLOCKED_RECT d3dlr;
		(*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
		WORD *pDst16 = (WORD*)d3dlr.pBits;

		for (int xy = 0; xy < 8 * 8; xy++)
			*pDst16++ = colour16;

		(*ppD3Dtex)->UnlockRect(0);

		return S_OK;
	}

	HRESULT WINAPI HandleEndScene(LPDIRECT3DDEVICE9 pDevice)
	{
		pGameDevice = pDevice;

		return oEndScene(pDevice);
	}

	HRESULT WINAPI HandleDIP(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount)
	{
		LPDIRECT3DVERTEXBUFFER9 Stream_Data;
		UINT Offset = 0;
		UINT Stride = 0;

		if (pDevice->GetStreamSource(0, &Stream_Data, &Offset, &Stride) == D3D_OK)
			Stream_Data->Release();

		return oDIP(pDevice, Type, BaseVertexIndex, MinIndex, NumVertices, StartIndex, PrimitiveCount);
	}

	void Hook() {
		DWORD D3D = 0;

		while (!D3D)
		{
			D3D = (DWORD)LoadLibraryA("d3d9.dll");
			Sleep(10);
		}

		DWORD Table = Memory::FindPattern(D3D, 0x128000, (BYTE*)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx"); // mov register, [ADDY] -> c7 0something 44 33 22 11

		DWORD* VTable = *(DWORD**)(Table + 2);

		oEndScene = (tEndScene)DetourFunction((PBYTE)VTable[42], (PBYTE)HandleEndScene);
		oDIP = (tDIP)DetourFunction((PBYTE)VTable[82], (PBYTE)HandleDIP);
	}
};
