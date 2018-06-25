#ifndef __D3D9RENDERER_H__
#define __D3D9RENDERER_H__

#include "hackbase.h"

class D3D9Renderer : public Renderer
{
private:
	IDirect3DDevice9 *pDevice;
	
	bool CreateFont();

public:
	D3D9Renderer(IDirect3DDevice9 *Device);
	~D3D9Renderer();
	void BeginScene() {}
	void EndScene() {}
	void RefreshData(IDirect3DDevice9 *Device);
	void DrawPic(int x, int y);
	void DrawRect(int x, int y, int w, int h, Color color);
	void DrawBorder(int x, int y, int w, int h, int d, Color color);
	void DrawLine(float x1, float y1, float x2, float y2, int size, bool antialias, Color color);
	void DrawText(int x, int y, Color FontColor, char *Text, ...);
	void DrawCircle(int x, int y, float radius, int width, UINT samples, Color color);
	int GetWidth();
	int GetHeight();
	void loglnDebugInfo();
};

#endif