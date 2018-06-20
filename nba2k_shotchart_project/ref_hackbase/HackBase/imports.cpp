#include "hackbase.h"

Imports* mSingleton = 0;

Imports* Imports::Singleton()
{
	if(!mSingleton)
		mSingleton = new Imports();
	return mSingleton;
}

Imports::Imports() {
	this->D3D10CreateDeviceAndSwapChain = (myD3D10CreateDeviceAndSwapChain)GetProcAddress(LoadLibrary("d3d10.dll"), "D3D10CreateDeviceAndSwapChain");
	this->Direct3DCreate9 = (myDirect3DCreate9)GetProcAddress(LoadLibrary("d3d9.dll"), "Direct3DCreate9");
	this->D3DCompile = (myD3DCompile)GetProcAddress(GetD3DCompiler(), "D3DCompile");
	this->D3DXCreateFont = (myD3DXCreateFont)GetProcAddress(GetD3DX9(), "D3DXCreateFontA");
	this->D3D10CompileEffectFromMemory = (myD3D10CompileEffectFromMemory)GetProcAddress(LoadLibrary("d3d10.dll"), "D3D10CompileEffectFromMemory");
	this->D3D10CreateEffectFromMemory = (myD3D10CreateEffectFromMemory)GetProcAddress(LoadLibrary("d3d10.dll"), "D3D10CreateEffectFromMemory");
	this->D3D10CreateStateBlock = (myD3D10CreateStateBlock)GetProcAddress(LoadLibrary("d3d10.dll"), "D3D10CreateStateBlock");
	this->D3D10StateBlockMaskEnableAll = (myD3D10StateBlockMaskEnableAll)GetProcAddress(LoadLibrary("d3d10.dll"), "D3D10StateBlockMaskEnableAll");
	this->D3DX10CreateFontA = (myD3DX10CreateFont)GetProcAddress(GetD3DX10(), "D3DX10CreateFontA");
}

Imports::~Imports() {
	mSingleton = 0;
}