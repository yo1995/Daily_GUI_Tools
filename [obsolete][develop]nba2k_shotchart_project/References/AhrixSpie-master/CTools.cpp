#include "CTools.h"
#include <math.h>

CTools Tools;

ofstream ofile;	
char dlldir[320];

char *CTools::GetDirectoryFile(char *filename)
{
	static char path[320];
	strcpy(path, dlldir);
	strcat(path, filename);
	return path;
}

void __cdecl CTools::LogError(const char * fmt, ...)
{
	va_list va_alist;
	char logbuf[256];
	FILE * fp;
	struct tm * current_tm;
	time_t current_time;

	time (&current_time);
	current_tm = localtime (&current_time);

	sprintf(logbuf,"");

	va_start (va_alist, fmt);
	_vsnprintf (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
	va_end (va_alist);

	if ( (fp = fopen ( GetDirectoryFile("Error.txt"), "a")) != NULL )
	{
		fprintf ( fp, "%s\n", logbuf );
		fclose (fp);
	}
}

DWORD CTools::D3D9VTable()
{
    DWORD dwObjBase = (DWORD)LoadLibraryA("D3D9.DLL");
    while ( dwObjBase++ < dwObjBase + 0x127850 )
    {
        if ( (*(WORD*)(dwObjBase + 0x00)) == 0x06C7 && (*(WORD*)(dwObjBase + 0x06)) == 0x8689 && (*(WORD*)(dwObjBase + 0x0C)) == 0x8689 ) 
		{ 
			dwObjBase += 2; 
			break; 
		}
    }
    return ( dwObjBase );
}

VOID CTools::FillRGB( int x, int y, int w, int h, D3DCOLOR color, LPDIRECT3DDEVICE9 pDevice )
{
	D3DRECT rec = { x, y, x + w, y + h };
	pDevice->Clear( 1, &rec, D3DCLEAR_TARGET, color, 0, 0 );
	return;
}

VOID CTools::DrawPoint(int x, int y, int w, int h, D3DCOLOR color, LPDIRECT3DDEVICE9 pDevice )
{
    FillRGB((int)x, (int)y, (int)w, (int)h, color,pDevice);
}

VOID CTools::DrawFilledRectangle(D3DXVECTOR2 Pos, D3DXVECTOR2 Size, D3DCOLOR Color)
{
	D3DXVECTOR2 points[2];

	points[0] = Pos+D3DXVECTOR2((Size.x/2),0);
	points[1] = D3DXVECTOR2(Pos.x+(Size.x/2),Pos.y+Size.y);

	ID3DXLine *dLine = this->pLine;
	bool antianalias = dLine->GetAntialias();
	bool gllines = dLine->GetGLLines();
	float width = dLine->GetWidth();
	dLine->SetAntialias(false);
	dLine->SetGLLines(false);
	dLine->SetWidth(Size.x);
	dLine->Begin();
	dLine->Draw(points,2,Color);
	dLine->End();
	dLine->SetAntialias(antianalias);
	dLine->SetGLLines(gllines);
	dLine->SetWidth(width);
	return;
}

VOID CTools::DrawMouse(D3DXVECTOR2 Pos, D3DCOLOR Color)
{
	int bList[9] = {12,10,8,6,5,4,3,2,1};
	int cList[6] = {8,6,4,3,2,1};

	for(int i=0;i<9;i++)
		this->DrawFilledRectangle(D3DXVECTOR2(Pos.x+i,Pos.y+i),D3DXVECTOR2(1,bList[i]),D3DCOLOR_RGBA(000,000,000,255));
	for(int i=0;i<6;i++)
		this->DrawFilledRectangle(D3DXVECTOR2(Pos.x+(i+1),Pos.y+(i+2)),D3DXVECTOR2(1,cList[i]),Color);
	return;
}

VOID CTools::DrawCircle(D3DXVECTOR2 Pos,float Radius,int Sides,D3DCOLOR Color) 
{
	D3DXVECTOR2 Line[128];
	ID3DXLine *dLine = this->pLine;
	float Step = D3DX_PI * 2.0 / Sides;
	int Count = 0;
	for (float a=0; a < D3DX_PI*2.0; a += Step) {
		float X1 = Radius * cos(a) + Pos.x;
		float Y1 = Radius * sin(a) + Pos.y;
		float X2 = Radius * cos(a+Step) + Pos.x;
		float Y2 = Radius * sin(a+Step) + Pos.y;
		Line[Count].x = X1;
		Line[Count].y = Y1;
		Line[Count+1].x = X2;
		Line[Count+1].y = Y2;
		Count += 2;
	}
	bool antianalias = dLine->GetAntialias();
	bool gllines = dLine->GetGLLines();
	float width = dLine->GetWidth();
	dLine->Begin();
	dLine->Draw(Line,Count,Color);
	dLine->End();
	dLine->SetAntialias(antianalias);
	dLine->SetGLLines(gllines);
	dLine->SetWidth(width);
	return;
}

VOID *CTools::DetourCreate(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp = (BYTE*)malloc(len+5);
	DWORD dwBack;

	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwBack);
	memcpy(jmp, src, len);	
	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp+1) = (DWORD)(src+len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD*)(src+1) = (DWORD)(dst - src) - 5;
	for (int i=5; i<len; i++)  src[i]=0x90;
	VirtualProtect(src, len, dwBack, &dwBack);
	return (jmp-len);
}
