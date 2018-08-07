// -----------------------------------------------------------------------------
// File: D3DFont.cpp
// Desc: Texture-based font class
// Copyright (c) 1999-2001 Microsoft Corporation. All rights reserved.
//
// Changes by Hans211
// - Added D3D_FONT_RIGHT
// - Added D3D_FONT_SHADOW
// - Stripped not needed functions
//
// 
// -----------------------------------------------------------------------------
#include <stdio.h>
#include <tchar.h>
#include <D3DX9.h>
#include "CFont.h"

//-----------------------------------------------------------------------------
// Custom vertex types for rendering text
//-----------------------------------------------------------------------------
#define MAX_NUM_VERTICES 50*6

struct FONT2DVERTEX { D3DXVECTOR4 p;   DWORD color;     FLOAT tu, tv; };
struct FONT3DVERTEX { D3DXVECTOR3 p;   D3DXVECTOR3 n;   FLOAT tu, tv; };

#define D3DFVF_FONT2DVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_FONT3DVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

inline FONT2DVERTEX InitFont2DVertex( const D3DXVECTOR4& p, D3DCOLOR color,
                                      FLOAT tu, FLOAT tv )
{
    FONT2DVERTEX v;   v.p = p;   v.color = color;   v.tu = tu;   v.tv = tv;
    return v;
}

inline FONT3DVERTEX InitFont3DVertex( const D3DXVECTOR3& p, const D3DXVECTOR3& n,
                                      FLOAT tu, FLOAT tv )
{
    FONT3DVERTEX v;   v.p = p;   v.n = n;   v.tu = tu;   v.tv = tv;
    return v;
}




//-----------------------------------------------------------------------------
// Name: CD3DFont()
// Desc: Font class constructor
//-----------------------------------------------------------------------------
CD3DFont::CD3DFont( TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags )
{
    strcpy( m_strFontName, strFontName );
    m_dwFontHeight         = dwHeight;
    m_dwFontFlags          = dwFlags;

    m_pd3dDevice           = NULL;
    m_pTexture             = NULL;
    m_pVB                  = NULL;

    m_dwSavedStateBlock    = 0L;
    m_dwDrawTextStateBlock = 0L;
}




//-----------------------------------------------------------------------------
// Name: ~CD3DFont()
// Desc: Font class destructor
//-----------------------------------------------------------------------------
CD3DFont::~CD3DFont()
{
    InvalidateDeviceObjects();
    DeleteDeviceObjects();
}




//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initializes device-dependent objects, including the vertex buffer used
//       for rendering text and the texture map which stores the font image.
//-----------------------------------------------------------------------------
HRESULT CD3DFont::InitDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice )
{
    HRESULT hr;

    // Keep a local copy of the device
    m_pd3dDevice = pd3dDevice;

    // Establish the font and texture size
    m_fTextScale  = 1.0f; // Draw fonts into texture without scaling

    // Large fonts need larger textures
    if( m_dwFontHeight > 40 )
        m_dwTexWidth = m_dwTexHeight = 1024;
    else if( m_dwFontHeight > 20 )
        m_dwTexWidth = m_dwTexHeight = 512;
    else
        m_dwTexWidth  = m_dwTexHeight = 256;

    // If requested texture is too big, use a smaller texture and smaller font,
    // and scale up when rendering.
    D3DCAPS9 d3dCaps;
    m_pd3dDevice->GetDeviceCaps( &d3dCaps );

    if( m_dwTexWidth > d3dCaps.MaxTextureWidth )
    {
        m_fTextScale = (FLOAT)d3dCaps.MaxTextureWidth / (FLOAT)m_dwTexWidth;
        m_dwTexWidth = m_dwTexHeight = d3dCaps.MaxTextureWidth;
    }

    // Create a new texture for the font
    hr = m_pd3dDevice->CreateTexture( m_dwTexWidth, m_dwTexHeight, 1,
                                      0, D3DFMT_A4R4G4B4,
                                      D3DPOOL_MANAGED, &m_pTexture, NULL );
    if( FAILED(hr) )
        return hr;

    // Prepare to create a bitmap
    DWORD*      pBitmapBits;
    BITMAPINFO bmi;
    ZeroMemory( &bmi.bmiHeader,  sizeof(BITMAPINFOHEADER) );
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       =  (int)m_dwTexWidth;
    bmi.bmiHeader.biHeight      = -(int)m_dwTexHeight;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount    = 32;

    // Create a DC and a bitmap for the font
    HDC     hDC       = CreateCompatibleDC( NULL );
    HBITMAP hbmBitmap = CreateDIBSection( hDC, &bmi, DIB_RGB_COLORS,
                                          (VOID**)&pBitmapBits, NULL, 0 );
    SetMapMode( hDC, MM_TEXT );

    // Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
    // antialiased font, but this is not guaranteed.
    INT nHeight    = -MulDiv( m_dwFontHeight, 
        (INT)(GetDeviceCaps(hDC, LOGPIXELSY) * m_fTextScale), 72 );
    DWORD dwBold   = (m_dwFontFlags&D3DFONT_BOLD)   ? FW_BOLD : FW_NORMAL;
    DWORD dwItalic = (m_dwFontFlags&D3DFONT_ITALIC) ? TRUE    : FALSE;
    HFONT hFont    = CreateFont( nHeight, 0, 0, 0, dwBold, dwItalic,
                          FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                          VARIABLE_PITCH, m_strFontName );
    if( NULL==hFont )
        return E_FAIL;

    SelectObject( hDC, hbmBitmap );
    SelectObject( hDC, hFont );

    // Set text properties
    SetTextColor( hDC, RGB(255,255,255) );
    SetBkColor(   hDC, 0x00000000 );
    SetTextAlign( hDC, TA_TOP );

    // Loop through all printable character and output them to the bitmap..
    // Meanwhile, keep track of the corresponding tex coords for each character.
    DWORD x = 0;
    DWORD y = 0;
    TCHAR str[2] = _T("x");
    SIZE size;

    // Calculate the spacing between characters based on line height
    GetTextExtentPoint32( hDC, TEXT(" "), 1, &size );
    x = m_dwSpacing = (DWORD) ceil(size.cy * 0.3f);

    for( TCHAR c=32; c<127; c++ )
    {
        str[0] = c;
        GetTextExtentPoint32( hDC, str, 1, &size );

        if( (DWORD)(x+size.cx+1) > m_dwTexWidth )
        {
            x  = m_dwSpacing;
            y += size.cy+1;
        }

        ExtTextOut( hDC, x+0, y+0, ETO_OPAQUE, NULL, str, 1, NULL );

        m_fTexCoords[c-32][0] = ((FLOAT)(x+0      -m_dwSpacing))/m_dwTexWidth;
        m_fTexCoords[c-32][1] = ((FLOAT)(y+0      +0          ))/m_dwTexHeight;
        m_fTexCoords[c-32][2] = ((FLOAT)(x+size.cx+m_dwSpacing))/m_dwTexWidth;
        m_fTexCoords[c-32][3] = ((FLOAT)(y+size.cy+0          ))/m_dwTexHeight;

        x += size.cx+(2 * m_dwSpacing);
    }

    // Lock the surface and write the alpha values for the set pixels
    D3DLOCKED_RECT d3dlr;
    m_pTexture->LockRect( 0, &d3dlr, 0, 0 );
    BYTE* pDstRow = (BYTE*)d3dlr.pBits;
    WORD* pDst16;
    BYTE bAlpha; // 4-bit measure of pixel intensity

    for( y=0; y < m_dwTexHeight; y++ )
    {
        pDst16 = (WORD*)pDstRow;
        for( x=0; x < m_dwTexWidth; x++ )
        {
            bAlpha = (BYTE)((pBitmapBits[m_dwTexWidth*y + x] & 0xff) >> 4);
            if (bAlpha > 0)
            {
                *pDst16++ = (bAlpha << 12) | 0x0fff;
            }
            else
            {
                *pDst16++ = 0x0000;
            }
        }
        pDstRow += d3dlr.Pitch;
    }

    // Done updating texture, so clean up used objects
    m_pTexture->UnlockRect(0);
    DeleteObject( hbmBitmap );
    DeleteDC( hDC );
    DeleteObject( hFont );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DFont::RestoreDeviceObjects()
{
    HRESULT hr;

    // Create vertex buffer for the letters
    if( FAILED( hr = m_pd3dDevice->CreateVertexBuffer( 
		MAX_NUM_VERTICES*sizeof(FONT2DVERTEX),
		D3DUSAGE_WRITEONLY, 0,
		D3DPOOL_DEFAULT, &m_pVB, NULL ) ) )
    {
        return hr;
    }

    // Create the state blocks for rendering text
    for( UINT which=0; which<2; which++ )
    {
        m_pd3dDevice->BeginStateBlock();
        m_pd3dDevice->SetTexture( 0, m_pTexture );

        if ( D3DFONT_ZENABLE & m_dwFontFlags )
            m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
        else
            m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

        m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
        m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
        m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,         0x08 );
        m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
        m_pd3dDevice->SetRenderState( D3DRS_FILLMODE,   D3DFILL_SOLID );
        m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
        m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_CLIPPING,         TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND,      FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
        m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
        m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
        m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
        m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
        m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

        if( which==0 )
            m_pd3dDevice->EndStateBlock( &m_dwSavedStateBlock );
        else
            m_pd3dDevice->EndStateBlock( &m_dwDrawTextStateBlock );
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Destroys all device-dependent objects
//-----------------------------------------------------------------------------
HRESULT CD3DFont::InvalidateDeviceObjects()
{
    m_pVB->Release();

    // Delete the state blocks
    if( m_pd3dDevice )
    {
        if( m_dwSavedStateBlock )
            m_dwSavedStateBlock->Release();
        if( m_dwDrawTextStateBlock )
            m_dwDrawTextStateBlock->Release();
    }

    m_dwSavedStateBlock    = 0L;
    m_dwDrawTextStateBlock = 0L;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Destroys all device-dependent objects
//-----------------------------------------------------------------------------
HRESULT CD3DFont::DeleteDeviceObjects()
{
    m_pTexture->Release();
    m_pd3dDevice = NULL;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: GetTextExtent()
// Desc: Get the dimensions of a text string
//-----------------------------------------------------------------------------
HRESULT CD3DFont::GetTextExtent( TCHAR* strText, SIZE* pSize )
{
    if( NULL==strText || NULL==pSize )
        return E_FAIL;

    FLOAT fRowWidth  = 0.0f;
    FLOAT fRowHeight = (m_fTexCoords[0][3]-m_fTexCoords[0][1])*m_dwTexHeight;
    FLOAT fWidth     = 0.0f;
    FLOAT fHeight    = fRowHeight;

    while( *strText )
    {
        TCHAR c = *strText++;

        if( c == _T('\n') )
        {
            fRowWidth = 0.0f;
            fHeight  += fRowHeight;
        }
	if( (c-32) < 0 || (c-32) >= 128-32 )
            continue;

        FLOAT tx1 = m_fTexCoords[c-32][0];
        FLOAT tx2 = m_fTexCoords[c-32][2];

        fRowWidth += (tx2-tx1)*m_dwTexWidth - 2*m_dwSpacing;

        if( fRowWidth > fWidth )
            fWidth = fRowWidth;
    }

    pSize->cx = (int)fWidth;
    pSize->cy = (int)fHeight;

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: DrawText()
// Desc: Draws 2D text
//-----------------------------------------------------------------------------
HRESULT CD3DFont::DrawText( FLOAT sx, FLOAT sy, DWORD dwColor,
                            TCHAR* strText, DWORD dwFlags )
{
    if( m_pd3dDevice == NULL )
        return E_FAIL;

    // Setup renderstate
    m_dwSavedStateBlock->Capture();
    m_dwDrawTextStateBlock->Apply();
    m_pd3dDevice->SetFVF( D3DFVF_FONT2DVERTEX );
    m_pd3dDevice->SetPixelShader( NULL );
  
    m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(FONT2DVERTEX) );

    // Set filter states
    if( dwFlags & D3DFONT_FILTERED )
    {
        m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
        m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    }
	// Hans211:  small adaption for right an centered alignment
	
	if( dwFlags & DT_RIGHT )
	{ 
	SIZE sz;
	GetTextExtent( strText, &sz ); sx -= (FLOAT)sz.cx;
	}

	if( dwFlags & DT_CENTER )
	{ 
	SIZE sz; 
	GetTextExtent( strText, &sz ); sx -= (FLOAT)(sz.cx / 2.0); 
	} 


    // Adjust for character spacing
    sx -= m_dwSpacing;
    FLOAT fStartX = sx;

    // Fill vertex buffer
    FONT2DVERTEX* pVertices = NULL;
    DWORD         dwNumTriangles = 0;
    m_pVB->Lock( 0, 0, (VOID**)&pVertices, D3DLOCK_DISCARD );

    while( *strText )
    {
        TCHAR c = *strText++;

        if( c == _T('\n') )
        {
            sx = fStartX;
            sy += (m_fTexCoords[0][3]-m_fTexCoords[0][1])*m_dwTexHeight;
        }
        if( (c-32) < 0 || (c-32) >= 128-32 )
            continue;

        FLOAT tx1 = m_fTexCoords[c-32][0];
        FLOAT ty1 = m_fTexCoords[c-32][1];
        FLOAT tx2 = m_fTexCoords[c-32][2];
        FLOAT ty2 = m_fTexCoords[c-32][3];

        FLOAT w = (tx2-tx1) *  m_dwTexWidth / m_fTextScale;
        FLOAT h = (ty2-ty1) * m_dwTexHeight / m_fTextScale;

        if( c != _T(' ') )
        {
			// Hans211: Add shadowing
			if( dwFlags & DT_SHADOW ) {
				float sxa,sya;
				sxa = sx;
				sya = sy;
				sxa=sx+1.0f;
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sxa+0-0.5f,sy+h-0.5f,0.9f,1.0f), 0xff000000, tx1, ty2 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sxa+0-0.5f,sy+0-0.5f,0.9f,1.0f), 0xff000000, tx1, ty1 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sxa+w-0.5f,sy+h-0.5f,0.9f,1.0f), 0xff000000, tx2, ty2 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sxa+w-0.5f,sy+0-0.5f,0.9f,1.0f), 0xff000000, tx2, ty1 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sxa+w-0.5f,sy+h-0.5f,0.9f,1.0f), 0xff000000, tx2, ty2 );
		        *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sxa+0-0.5f,sy+0-0.5f,0.9f,1.0f), 0xff000000, tx1, ty1 );

				sxa=sx-1.0f;
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sxa+0-0.5f,sy+h-0.5f,0.9f,1.0f), 0xff000000, tx1, ty2 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sxa+0-0.5f,sy+0-0.5f,0.9f,1.0f), 0xff000000, tx1, ty1 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sxa+w-0.5f,sy+h-0.5f,0.9f,1.0f), 0xff000000, tx2, ty2 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sxa+w-0.5f,sy+0-0.5f,0.9f,1.0f), 0xff000000, tx2, ty1 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sxa+w-0.5f,sy+h-0.5f,0.9f,1.0f), 0xff000000, tx2, ty2 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sxa+0-0.5f,sy+0-0.5f,0.9f,1.0f), 0xff000000, tx1, ty1 );

				sya=sy-1.0f;
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sya+h-0.5f,0.9f,1.0f), 0xff000000, tx1, ty2 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sya+0-0.5f,0.9f,1.0f), 0xff000000, tx1, ty1 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sya+h-0.5f,0.9f,1.0f), 0xff000000, tx2, ty2 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sya+0-0.5f,0.9f,1.0f), 0xff000000, tx2, ty1 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sya+h-0.5f,0.9f,1.0f), 0xff000000, tx2, ty2 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sya+0-0.5f,0.9f,1.0f), 0xff000000, tx1, ty1 );

				sya=sy+1.0f;
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sya+h-0.5f,0.9f,1.0f), 0xff000000, tx1, ty2 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sya+0-0.5f,0.9f,1.0f), 0xff000000, tx1, ty1 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sya+h-0.5f,0.9f,1.0f), 0xff000000, tx2, ty2 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sya+0-0.5f,0.9f,1.0f), 0xff000000, tx2, ty1 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sya+h-0.5f,0.9f,1.0f), 0xff000000, tx2, ty2 );
	            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sya+0-0.5f,0.9f,1.0f), 0xff000000, tx1, ty1 );
	            dwNumTriangles += 8;
			}
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, tx1, ty2 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, tx1, ty1 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, tx2, ty2 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, tx2, ty1 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+w-0.5f,sy+h-0.5f,0.9f,1.0f), dwColor, tx2, ty2 );
            *pVertices++ = InitFont2DVertex( D3DXVECTOR4(sx+0-0.5f,sy+0-0.5f,0.9f,1.0f), dwColor, tx1, ty1 );
            dwNumTriangles += 2;

			// Hans211: adapt calculation so it works with shadowing also
            if( dwNumTriangles*3 > (MAX_NUM_VERTICES-(dwNumTriangles*3)) )
            {
                // Unlock, render, and relock the vertex buffer
                m_pVB->Unlock();
                m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );
                pVertices = NULL;
                m_pVB->Lock( 0, 0, (VOID**)&pVertices, D3DLOCK_DISCARD );
                dwNumTriangles = 0L;
            }
        }
        sx += w - (2 * m_dwSpacing);
    }

    // Unlock and render the vertex buffer
    m_pVB->Unlock();
    if( dwNumTriangles > 0 )
        m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, dwNumTriangles );

    // Restore the modified renderstates
    m_dwSavedStateBlock->Apply();

    return S_OK;
}



