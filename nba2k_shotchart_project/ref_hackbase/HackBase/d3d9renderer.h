#ifndef __D3D9RENDERER_H__
#define __D3D9RENDERER_H__

#include "hackbase.h"

class D3D9Renderer : public Renderer
{
private:
	IDirect3DDevice9 *pDevice;
	ID3DXFont *mFont;
	bool CreateFont();

public:
	D3D9Renderer(IDirect3DDevice9 *Device);
	~D3D9Renderer();
	void BeginScene() {}
	void EndScene() {}
	void RefreshData(IDirect3DDevice9 *Device);
	void DrawRect(int x, int y, int w, int h, Color color);
	void DrawBorder(int x, int y, int w, int h, int d, Color color);
	void DrawLine(int x1, int y1, int x2, int y2, float size, bool antialias, Color color);
	void DrawText(int x, int y, Color FontColor, char *Text, ...);
	void DrawCircle(int x, int y, float radius, int width, UINT samples, Color color);
	int GetWidth();
	int GetHeight();
	void loglnDebugInfo();
};

#endif