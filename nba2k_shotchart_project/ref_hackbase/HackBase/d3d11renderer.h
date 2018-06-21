#ifndef __D3D11RENDERER_H__
#define __D3D11RENDERER_H__

#include "hackbase.h"

class D3D11Renderer : public Renderer
{
private:
	IDXGISwapChain *pSwapChain;
	ID3D11Device *pDevice;
	ID3D11DeviceContext *pDeviceContext;

	ID3D11PixelShader *pPixelShader;
	ID3D11VertexShader *pVertexShader;
	ID3D11Buffer *pBuffer;
	
#pragma region BackUpStuff
	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology;
	ID3D11Buffer *pIndexBuffer;
	DXGI_FORMAT Format;
	UINT Offset;
	ID3D11Buffer *ppVertexBuffers[32];
	UINT pStride;
	UINT pOffset;  
#pragma endregion

	bool CreateRect();
	bool rectCreated;

public:
	D3D11Renderer(IDXGISwapChain *SwapChain);
	~D3D11Renderer();
	void BeginScene();
	void EndScene();
	void RefreshData(IDXGISwapChain *SwapChain);
	void DrawRect(int x, int y, int w, int h, Color color);
	void DrawBorder(int x, int y, int w, int h, int d, Color color);
	void DrawLine(float x1, float y1, float x2, float y2, int size, bool antialias, Color color) {};
	void DrawText(int x, int y, Color FontColor, char *Text, ...) {}; // could use fw1font or spritefont, implement in the future.
	void DrawCircle(int x, int y, float radius, int width, UINT samples, Color color);
	int GetWidth();
	int GetHeight();
	void loglnDebugInfo();
};

#endif