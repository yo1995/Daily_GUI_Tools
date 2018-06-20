#include "hackbase.h"

D3D10Renderer::D3D10Renderer(IDXGISwapChain* SwapChain) {
	this->rectCreated = false;
	this->pSwapChain = SwapChain;

	pSwapChain->GetDevice(__uuidof(this->pDevice), (void**)&(this->pDevice));

	if(this->CreateRect()) {
#ifdef DEBUGLOG
		logln("D3D10Renderer::CreateRect failed!");
#endif
	}
}

D3D10Renderer::~D3D10Renderer() {
	SAFE_RELEASE(this->pSwapChain);
	SAFE_RELEASE(this->pDevice);
	SAFE_RELEASE(this->pPixelShader);
	SAFE_RELEASE(this->pVertexShader);
}

void D3D10Renderer::RefreshData(IDXGISwapChain* SwapChain) {
	if(this->pSwapChain != SwapChain) {
#ifdef DEBUGLOG
		logln("IDXGISwapChain has changed");
#endif
		this->loglnDebugInfo();
		this->pSwapChain = SwapChain;
		pSwapChain->GetDevice(__uuidof(this->pDevice), (void**)&(this->pDevice));
	}
}

bool D3D10Renderer::CreateRect() {
	if(!Imports::Singleton()->D3DCompile)
		return false;

	ID3DBlob *pVertextShaderCode;
	ID3DBlob *pPixelShaderCode;
	HRESULT hResult = S_OK;

	const char vsQuad[] =
		"cbuffer ShaderConstants : register(b0) {"
		"   float4x4 TransformMatrix : packoffset(c0);"
		"};"
		"float4 VS(uint VertexIndex : SV_VertexID) : SV_Position{"
		"   const float2 corners[4] = {"
		"      float2(0.0f, 1.0f),"
		"      float2(0.0f, 0.0f),"
		"      float2(1.0f, 1.0f),"
		"      float2(1.0f, 0.0f)"
		"   };"
		"   return mul(TransformMatrix, float4(corners[VertexIndex].xy, 0.0f, 1.0f));"
		"}";
	hResult = Imports::Singleton()->D3DCompile(vsQuad, sizeof(vsQuad), NULL, NULL, NULL, "VS", "vs_4_0", 0, 0, &pVertextShaderCode, NULL);
	if(FAILED(hResult))
		return false;
	hResult = this->pDevice->CreateVertexShader(pVertextShaderCode->GetBufferPointer(), pVertextShaderCode->GetBufferSize(), &pVertexShader);
	SAFE_RELEASE(pVertextShaderCode);
	if(FAILED(hResult))
		return false;

	const char psColor[] =
		"cbuffer ShaderConstants : register(b0) {"
		"   float4 Color : packoffset(c4);"
		"};"
		"float4 PS() : SV_Target {"
		"   return Color;"
		"}";
	hResult = Imports::Singleton()->D3DCompile(psColor, sizeof(psColor),NULL, NULL, NULL, "PS", "ps_4_0", 0, 0, &pPixelShaderCode, NULL);
	if(FAILED(hResult))
		return false;
	hResult = this->pDevice->CreatePixelShader(pPixelShaderCode->GetBufferPointer(), pPixelShaderCode->GetBufferSize(), &pPixelShader);
	SAFE_RELEASE(pPixelShaderCode);
	if(FAILED(hResult))
		return false;

	D3D10_BUFFER_DESC bufferDesc = {0};
	bufferDesc.ByteWidth = sizeof(ShaderConstants);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D10_USAGE_DEFAULT;
	hResult = this->pDevice->CreateBuffer(&bufferDesc, NULL, &pBuffer);
	if(FAILED(hResult))
		return false;

	rectCreated = true;
	return true;
}

bool D3D10Renderer::CreateFont() {
	if(!Imports::Singleton())
		return false;

	if(!Imports::Singleton()->D3DX10CreateFontA)
		return false;

	if(FAILED(Imports::Singleton()->D3DX10CreateFontA(pDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &(this->mFont))))
		return false;
	return true;
}

void D3D10Renderer::DrawRect(int x, int y, int w, int h, Color color) {
	if(!rectCreated) {
#ifdef DEBUGLOG
		logln("Failed to draw rect!");
#endif
		return;
	}

	this->pDevice->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	this->pDevice->PSSetShader(this->pPixelShader);
	this->pDevice->PSSetConstantBuffers(0, 1, &(this->pBuffer));
	this->pDevice->VSSetShader(this->pVertexShader);
	this->pDevice->VSSetConstantBuffers(0, 1, &(this->pBuffer));
	this->pDevice->GSSetShader(NULL);

	ShaderConstants constants = {0};

	D3D10_VIEWPORT vp;
	UINT nvp = 1;
	this->pDevice->RSGetViewports(&nvp, &vp);
	constants.TransformMatrix[0] = w * 2.0f / vp.Width;
	constants.TransformMatrix[12] = -1.0f + x * 2.0f / vp.Width;
	constants.TransformMatrix[5] = h * -2.0f / vp.Height;
	constants.TransformMatrix[13] = 1.0f + y * -2.0f / vp.Height;
	constants.TransformMatrix[10] = 1.0f;
	constants.TransformMatrix[15] = 1.0f;

	for(int i=0;i<4;++i)
		constants.Color[i] = static_cast<FLOAT>((D3DCOLOR_ARGB(color.a, color.b, color.g, color.r) >> (i << 3)) & 0xff) / 255.0f;

	this->pDevice->UpdateSubresource(this->pBuffer, 0, NULL, &constants, 0, 0);

	this->pDevice->Draw(4, 0);
}

void D3D10Renderer::DrawBorder(int x, int y, int w, int h, int d, Color color) {
	this->DrawRect(x + d, y, w - 2 * d, d, color);
	this->DrawRect(x, y, d, h, color);
	this->DrawRect(x + w - d, y, d, h, color);
	this->DrawRect(x + d, y + h - d, w - 2 * d, d, color);
}

void D3D10Renderer::DrawCircle(int x, int y, float radius, int width, UINT samples, Color color) {
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

void D3D10Renderer::DrawText(int x, int y, char *Text, ...) {
	if(!Text)
		return;

	if(!this->fontCreated)
		return;

	char buf[1024];
	va_list arglist;
	va_start(arglist, Text);
	vsprintf_s(buf, 1023, Text, arglist);
	va_end(arglist);

	RECT rcScreen = { x, y, x + 2000, y + 1000 };
	mFont->DrawText( NULL, buf, strlen(buf), &rcScreen, DT_CENTER, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
}

int D3D10Renderer::GetWidth() {
	D3D10_VIEWPORT vp;
	UINT nvp = 1;
	this->pDevice->RSGetViewports(&nvp, &vp);
	return (int)vp.Width;
}

int D3D10Renderer::GetHeight() {
	D3D10_VIEWPORT vp;
	UINT nvp = 1;
	this->pDevice->RSGetViewports(&nvp, &vp);
	return (int)vp.Height;
}

void D3D10Renderer::loglnDebugInfo() {
#ifdef DEBUGLOG
	logln();
	logln("IDXGISwapChain: 0x%08X", this->pSwapChain);
	logln("ID3D10Device:   0x%08X", this->pDevice);
	logln();
#endif
}