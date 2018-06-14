#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <directxmath.h>
#define NOMINMAX

#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "Effects.h"
#include "GeometricPrimitive.h"
#include "Model.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"


using namespace DirectX;


#include <d3d11.h>
#include <dxgi.h>
// #include <dxerr.h> legacy
#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <windowsx.h>
#include <Uxtheme.h>
#include <dwmapi.h>
#include <DirectXMath.h> //changed by https://msdn.microsoft.com/en-us/library/ee663275.aspx
#include <d3d9types.h>
#include <d3d11.h>

/*
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "DXErr.lib")

*/
#pragma comment(lib, "dwmapi.lib")

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
