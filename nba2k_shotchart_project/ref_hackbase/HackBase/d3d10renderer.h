#ifndef __D3D10RENDERER_H__
#define __D3D10RENDERER_H__

#include "hackbase.h"

class D3D10Renderer : public Renderer
{
private:
	IDXGISwapChain *pSwapChain;
	ID3D10Device *pDevice;

	ID3D10PixelShader *pPixelShader;
	ID3D10VertexShader *pVertexShader;
	ID3D10Buffer *pBuffer;

	ID3DX10Font *mFont;
	ID3DX10Sprite *mSprite;

	bool CreateRect();
	bool CreateFont();
	bool rectCreated;
	bool fontCreated;

public:
	D3D10Renderer(IDXGISwapChain *SwapChain);
	~D3D10Renderer();
	void BeginScene() {}
	void EndScene() {}
	void RefreshData(IDXGISwapChain *SwapChain);
	void DrawPic(int x, int y) {}; // if needed implement later
	void DrawRect(int x, int y, int w, int h, Color color);
	void DrawBorder(int x, int y, int w, int h, int d, Color color);
	void DrawLine(float x1, float y1, float x2, float y2, int size, bool antialias, Color color) {};
	void DrawText(int x, int y, Color FontColor, char *Text, ...);
	void DrawCircle(int x, int y, float radius, int width, UINT samples, Color color);
	int GetWidth();
	int GetHeight();
	void loglnDebugInfo();
};

#endif