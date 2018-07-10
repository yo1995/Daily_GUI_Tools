#include "hackbase.h"

D3D11Renderer::D3D11Renderer(IDXGISwapChain *SwapChain) {
	this->rectCreated = false;
	this->pSwapChain = SwapChain;

	pSwapChain->GetDevice(__uuidof(this->pDevice), (void**)&(this->pDevice));
	this->pDevice->GetImmediateContext(&(this->pDeviceContext));

	pIndexBuffer = 0;

	if(!this->CreateRect()) {
#ifdef DEBUGLOG
		logln("Create rect failed!");
#endif
	}
}

D3D11Renderer::~D3D11Renderer() {
	SAFE_RELEASE(this->pSwapChain);
	SAFE_RELEASE(this->pDevice);
	SAFE_RELEASE(this->pDeviceContext);
	SAFE_RELEASE(this->pPixelShader);
	SAFE_RELEASE(this->pVertexShader);
}

void D3D11Renderer::BeginScene() {
	pDeviceContext->IAGetPrimitiveTopology(&PrimitiveTopology);
	pDeviceContext->IAGetIndexBuffer(&pIndexBuffer, &Format, &Offset);
	//pDeviceContext->IAGetVertexBuffers(0, 32, ppVertexBuffers, &pStride, &pOffset);
}

void D3D11Renderer::EndScene() {
	//pDeviceContext->IASetVertexBuffers(0, 32, ppVertexBuffers, &pStride, &pOffset);
	pDeviceContext->IASetIndexBuffer(pIndexBuffer, Format, Offset);
	pDeviceContext->IASetPrimitiveTopology(PrimitiveTopology);
	SAFE_RELEASE(pIndexBuffer);
}

void D3D11Renderer::RefreshData(IDXGISwapChain* SwapChain) {
	if(this->pSwapChain != SwapChain) {
#ifdef DEBUGLOG
		logln("IDXGISwapChain has changed");
#endif
		this->loglnDebugInfo();
		this->pSwapChain = SwapChain;
		this->pSwapChain->GetDevice(__uuidof(this->pDevice), (void**)&(this->pDevice));
		this->pDevice->GetImmediateContext(&(this->pDeviceContext));
	}
}

bool D3D11Renderer::CreateRect() {
	if(!Imports::Singleton())
		return false;

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
	hResult = this->pDevice->CreateVertexShader(pVertextShaderCode->GetBufferPointer(), pVertextShaderCode->GetBufferSize(), NULL, &pVertexShader);
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
	hResult = this->pDevice->CreatePixelShader(pPixelShaderCode->GetBufferPointer(), pPixelShaderCode->GetBufferSize(), NULL, &pPixelShader);
	SAFE_RELEASE(pPixelShaderCode);
	if(FAILED(hResult))
		return false;

	D3D11_BUFFER_DESC bufferDesc = {0};
	bufferDesc.ByteWidth = sizeof(ShaderConstants);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hResult = this->pDevice->CreateBuffer(&bufferDesc, NULL, &pBuffer);
	if(FAILED(hResult))
		return false;

	rectCreated = true;
	return true;
}

void D3D11Renderer::DrawRect(int x, int y, int w, int h, Color color) {
	if(!rectCreated) {
#ifdef DEBUGLOG
		logln("Failed to draw rect!");
#endif
		return;
	}

	this->pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	this->pDeviceContext->PSSetShader(this->pPixelShader, NULL, 0);
	this->pDeviceContext->PSSetConstantBuffers(0, 1, &(this->pBuffer));
	this->pDeviceContext->VSSetShader(this->pVertexShader, NULL, 0);
	this->pDeviceContext->VSSetConstantBuffers(0, 1, &(this->pBuffer));
	this->pDeviceContext->GSSetShader(NULL, NULL, 0);

	ShaderConstants constants = {0};

	D3D11_VIEWPORT vp;
	UINT nvp = 1;
	this->pDeviceContext->RSGetViewports(&nvp, &vp);
	constants.TransformMatrix[0] = w * 2.0f / vp.Width;
	constants.TransformMatrix[12] = -1.0f + x * 2.0f / vp.Width;
	constants.TransformMatrix[5] = h * -2.0f / vp.Height;
	constants.TransformMatrix[13] = 1.0f + y * -2.0f / vp.Height;
	constants.TransformMatrix[10] = 1.0f;
	constants.TransformMatrix[15] = 1.0f;

	for(int i=0;i<4;++i)
		constants.Color[i] = static_cast<FLOAT>((D3DCOLOR_ARGB(color.a, color.b, color.g, color.r) >> (i << 3)) & 0xff) / 255.0f;

	this->pDeviceContext->UpdateSubresource(this->pBuffer, 0, NULL, &constants, 0, 0);

	this->pDeviceContext->Draw(4, 0);
}

void D3D11Renderer::DrawBorder(int x, int y, int w, int h, int d, Color color) {
	this->DrawRect(x + d, y, w - 2 * d, d, color);
	this->DrawRect(x, y, d, h, color);
	this->DrawRect(x + w - d, y, d, h, color);
	this->DrawRect(x + d, y + h - d, w - 2 * d, d, color);
}

void D3D11Renderer::DrawCircle(int x, int y, float radius, int width, UINT samples, Color color) {
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

int D3D11Renderer::GetWidth() {
	D3D11_VIEWPORT vp;
	UINT nvp = 1;
	this->pDeviceContext->RSGetViewports(&nvp, &vp);
	return (int)vp.Width;
}

int D3D11Renderer::GetHeight() {
	D3D11_VIEWPORT vp;
	UINT nvp = 1;
	this->pDeviceContext->RSGetViewports(&nvp, &vp);
	return (int)vp.Height;
}

void D3D11Renderer::loglnDebugInfo() {
#ifdef DEBUGLOG
	logln();
	logln("IDXGISwapChain:      0x%08X", this->pSwapChain);
	logln("ID3D11Device:        0x%08X", this->pDevice);
	logln("ID3D11DeviceContext: 0x%08X", this->pDeviceContext);
	logln();
#endif
}