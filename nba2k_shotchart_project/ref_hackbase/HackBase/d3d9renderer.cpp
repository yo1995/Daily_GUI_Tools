#include "hackbase.h"



D3D9Renderer::D3D9Renderer(IDirect3DDevice9 *Device) {
	this->pDevice = Device;
}

D3D9Renderer::~D3D9Renderer() {
	SAFE_RELEASE(this->pDevice);
}

void D3D9Renderer::RefreshData(IDirect3DDevice9 *Device) {
	if(this->pDevice != Device) {
#ifdef DEBUGLOG
		logln("IDirect3DDevice9 has changed (0x%08X -> 0x%08X)", this->pDevice, Device);
#endif
		this->loglnDebugInfo();
		this->pDevice = Device;
	}
}

void D3D9Renderer::DrawRect(int x, int y, int w, int h, Color color) {
	if(this->pDevice) {
		D3DRECT rec = {x, y, x + w, y + h};
		this->pDevice->Clear(1, &rec, D3DCLEAR_TARGET, D3DCOLOR_ARGB(color.a, color.r, color.g, color.b), 0, 0);
	}
}


void D3D9Renderer::DrawBorder(int x, int y, int w, int h, int d, Color color) {
	this->DrawRect(x + d, y, w - 2 * d, d, color);
	this->DrawRect(x, y, d, h, color);
	this->DrawRect(x + w - d, y, d, h, color);
	this->DrawRect(x + d, y + h - d, w - 2 * d, d, color);
}

void D3D9Renderer::DrawLine(int x1, int y1, int x2, int y2, float width, bool antialias, Color color) {
	// dont actually need to draw an angled line. might implement in the future.
	ID3DXLine *m_Line;
	D3DXCreateLine(pDevice, &m_Line); // this one depends on d3dx9.lib which should be deprecated. change in the future?
	D3DXVECTOR2 line[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };
	m_Line->SetWidth(width);
	//if (antialias) 
	//  m_Line->SetAntialias(1);
	m_Line->Begin();
	m_Line->Draw(line, 2, D3DCOLOR_ARGB(color.a, color.r, color.g, color.b));
	m_Line->End();
	m_Line->Release();
}


bool D3D9Renderer::CreateFont() {
	if (!Imports::Singleton())
		return false;
	if (!Imports::Singleton()->D3DXCreateFont)
		return false;
	if (FAILED(Imports::Singleton()->D3DXCreateFont(pDevice, FontSize_default, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, FontFamily_default, &(this->mFont))))
		return false;
	return true;
}


void D3D9Renderer::DrawText(int x, int y, Color color, char *Text, ...) {
	if (!Text)
		return;
	if (!this->CreateFont())
		return;
	char buf[1024];
	va_list arglist;
	va_start(arglist, Text);
	vsprintf_s(buf, 1023, Text, arglist);
	va_end(arglist);
	RECT rcScreen = { x, y, x + 200, y + 100 };
	mFont->DrawText(NULL, buf, strlen(buf), &rcScreen, DT_CENTER, D3DCOLOR_ARGB(color.a, color.r, color.g, color.b));
	mFont->Release();
}

void D3D9Renderer::DrawCircle(int x, int y, float radius, int width, UINT samples, Color color) {
	float Angle = (2.0f * PI) / samples; //to radians

	float Cos = cos(Angle);
	float Sin = sin(Angle);
	float rotX;
	float rotY;
	float vecX = radius;
	float vecY = 0;

	for (unsigned short i = 0; i < samples; ++i)
	{
		rotX = Cos * vecX - Sin * vecY;
		rotY = Sin * vecX + Cos * vecY;
		rotX += x;
		rotY += y;
		vecX += x;
		vecY += y;
		//D3DDrawLine(vec.x,vec.y,rot.x,rot.y,color,pDev);
		DrawLine(vecX, vecY, rotX, rotY, width, false, color);
		vecX = rotX - x;
		vecY = rotY - y;
	}
}

int D3D9Renderer::GetWidth() {
	if(this->pDevice) {
		D3DVIEWPORT9 View;
		if(!FAILED(pDevice->GetViewport(&View)))
			return View.Width;
	}
	return 0;
}

int D3D9Renderer::GetHeight() {
	if(this->pDevice) {
		D3DVIEWPORT9 View;
		if(!FAILED(pDevice->GetViewport(&View)))
			return View.Height;
	}
	return 0;
}

void D3D9Renderer::loglnDebugInfo() {
#ifdef DEBUGLOG
	logln();
	logln("IDirect3DDevice9: 0x%08X", this->pDevice);
	logln();
#endif
}