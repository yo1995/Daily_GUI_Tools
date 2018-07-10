// Win32Project1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "Includes.h"
#include "OverlayClasses.h"

class D3DApp:public DXOverlay
{
public:
	D3DApp(String title,UINT width,UINT height,HINSTANCE inst,String target,UINT msaa, String sprftpath);
	~D3DApp();
	void DrawScene(); 
	void ClearScene();
};

D3DApp::D3DApp(String title,UINT width,UINT height,HINSTANCE inst,String target,UINT msaa, String sprftpath):DXOverlay(title,width,height,inst,target,msaa, sprftpath)
{
	//just call base class constructor
}
D3DApp::~D3DApp()
{
	
}

void D3DApp::ClearScene() {
	assert(m_pImmediateDeviceContext);
	assert(m_pSwapChain);
	float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
	m_pImmediateDeviceContext->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<const float*>(&clearColor));
	HR(m_pSwapChain->Present(0, 0));
}

void D3DApp::DrawScene()
{
	assert(m_pImmediateDeviceContext);
	assert(m_pSwapChain);

	float clearColor[4]={0.0f,0.0f,0.0f,0.0f};
	float blend[4]={0};
	// SYSTEMTIME sys;
	// GetLocalTime(&sys);
		
	m_pImmediateDeviceContext->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<const float*>(&clearColor));
	m_pImmediateDeviceContext->OMSetBlendState( m_pAlphaOnBlendState, blend, 0xffffffff );
	
	DrawString(XMFLOAT2(50,50),1.0f,false, "Width:%.2f Height:%.2f", (float)m_width, (float)m_height);
	// "现在是%02d月%02d日%02d时%02d分", sys.wMonth, sys.wDay, sys.wHour, sys.wMinute
	// (float)m_width/2, (float)m_height/2
	// 
	XMVECTOR pos1={ (float)m_width/2,(float)m_height/2+1};
	XMVECTOR pos2={ (float)m_width/2,(float)m_height/2-1};
	DrawLine(pos1,pos2,Colors::Red);
	// note that when minimized XMScalarNearEqual will assert
	DrawCircle(pos1,Colors::Green,30,30);
	m_pImmediateDeviceContext->OMSetBlendState( m_pAlphaOffBlendState, blend, 0xffffffff );
	HR(m_pSwapChain->Present(0, 0));
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	String title = (L"Overlay");
	String target_window_name = (L"新建.txt - 记事本");	// L"新建.txt - 记事本" L"NBA 2K11"
	String sprite_font_path = (L"C:\\Users\\yo-ch\\Downloads\\nba2k11 tools\\DX11Overlay-master\\Calibri.spritefont");
	D3DApp app(title,600,600,hInstance, target_window_name,4, sprite_font_path);
	app.MakeWindow();
	app.InitializeDX();
	app.SetToTarget();
	return app.RunOverlay();
}



