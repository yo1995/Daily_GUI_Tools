#pragma

#include "StandardIncludes.h"

class CTools
{
public:

	VOID	__cdecl LogError(const char * fmt, ...);
	char    *GetDirectoryFile( char *filename );

	D3DVIEWPORT9 g_ViewPort;
	ID3DXLine	*pLine;
	BOOL	Init;
	VOID	*DetourCreate( BYTE *src, const BYTE *dst, const int len );
	VOID	FillRGB( int x, int y, int w, int h, D3DCOLOR color, LPDIRECT3DDEVICE9 pDevice );
	VOID	DrawPoint(int x, int y, int w, int h, D3DCOLOR color, LPDIRECT3DDEVICE9 pDevice );
	VOID	DrawFilledRectangle(D3DXVECTOR2 Pos,D3DXVECTOR2 Size,D3DCOLOR Color);
	VOID	DrawCircle(D3DXVECTOR2 Pos,float Radius,int Sides,D3DCOLOR Color);
	VOID	DrawMouse(D3DXVECTOR2 Pos, D3DCOLOR Color);
	DWORD	D3D9VTable();
};

extern CTools Tools;

extern ofstream ofile;
extern char dlldir[320];
