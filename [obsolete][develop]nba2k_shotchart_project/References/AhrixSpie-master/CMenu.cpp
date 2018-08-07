#include "StandardIncludes.h"
#include "CMenu.h"
#include "CSound.h"
#include "Sounds.h"

//-----------------------------------------------------------------------------
// Name: PostReset()
// Desc: Creates Fonts and other Resources for Menu
//-----------------------------------------------------------------------------

void cMenu::PostReset(LPDIRECT3DDEVICE9 pDevice)
{
	if(font == NULL)
	{
	font = new CD3DFont("Tahoma", 7, D3DFONT_BOLD);
	font->InitDeviceObjects(pDevice);
	font->RestoreDeviceObjects();
	this->FontCreated = true;
	}
}

//-----------------------------------------------------------------------------
// Name: PreReset()
// Desc: Releases Fonts / objects for menu
//-----------------------------------------------------------------------------

void cMenu::PreReset(void)
{
	font->InvalidateDeviceObjects();
	font->DeleteDeviceObjects();
	font = NULL;
	this->FontCreated = false;
}

//-----------------------------------------------------------------------------
// Name: Init()
// Desc: Load Menu Defaults / Prepare Menu For Usage
//-----------------------------------------------------------------------------

void cMenu::Init()
{
	if (opt.options.reset)INIT=false;
	if(!INIT){
		opt.options.move=0;		// BUG FIX FOR RESET DISABLE MOVE BEFORE SETTING COORDINATES
		X=25;
		Y=15;
		WIDTH=176;			    // MENU WIDTH
		I_OFS=4;			    // OFSET FOR ITEM
		S_OFS=WIDTH-4;			// OFSET FOR STATE
		T_SPC=16;				// Space from Title and Start of Items
		F_SPC=16;				// Footer Spacing Hook Menu
		I_SPC=15;				// Item Spacing
		I_CUR=0;				// Set current Item
		SHOW=true;				// false=hide  true=show
		INIT=true;				// STOP INITATION
	}
}

//-----------------------------------------------------------------------------
// Name: FPS()
// Desc: Keeps Track of Current Frames Persecond Make sure if its already called to specify a 1 as parameter
//-----------------------------------------------------------------------------

char* cMenu::FPS(int en)
{
	static int	 FPScounter = 0;
	static float FPSfLastTickCount = 0.0f;
	static float FPSfCurrentTickCount;
	static char  cfps[6] = "";

	if(!en)
	{
		FPSfCurrentTickCount = clock() * 0.001f;
		FPScounter++;

		if((FPSfCurrentTickCount - FPSfLastTickCount) > 1.0f)
		{
			FPSfLastTickCount = FPSfCurrentTickCount;
			sprintf(cfps,"%d FPS",FPScounter);
			FPScounter = 0;
		}
	}
	return cfps;
}

//-----------------------------------------------------------------------------
// Name: TIME()
// Desc: Outputs Current Time in USA Format
//-----------------------------------------------------------------------------

char* cMenu::TIME(void)
{
	static char ctime[20] = "" ;
	struct tm * current_tm;
	time_t current_time;
	time (&current_time);
	current_tm = localtime (&current_time);
	if(current_tm->tm_hour>12)
		sprintf( ctime, "%d:%02d:%02d PM", current_tm->tm_hour - 12, current_tm->tm_min, current_tm->tm_sec );
	else
		sprintf( ctime, "%d:%02d:%02d AM", current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec );
	return ctime;
}

//-----------------------------------------------------------------------------
// Name: DATE()
// Desc: Outputs Current Date in USA Format
//-----------------------------------------------------------------------------

char* cMenu::DATE(void)
{
	struct tm * current_tm;
	time_t current_time;time (&current_time);
	current_tm = localtime (&current_time);
	char *month	[]={"Jan.","Feb.","Mar.","Apr.","May","June","July","Aug.","Sept.","Oct.","Nov.","Dec."};
	char *logbuf = new char[ 256 ];

	sprintf( logbuf, "%s %02d, 20%d",month[current_tm->tm_mon],current_tm->tm_mday,current_tm->tm_year-100);
	return logbuf;
}

//-----------------------------------------------------------------------------
// Name: oprintf()
// Desc: equiv to sprintf but for output into of dest
//-----------------------------------------------------------------------------

char* cMenu::oprintf (const char *fmt, ...)
{
	static char buffer[225] = "";
	va_list va_alist;
	va_start (va_alist, fmt);
	_vsnprintf (buffer,sizeof(buffer), fmt, va_alist);
	va_end (va_alist);
	return buffer;
}
//-----------------------------------------------------------------------------
// Name: Save()
// Desc: Saves Menu Item states for later Restoration
//-----------------------------------------------------------------------------

void cMenu::Save(char* szSection, char* szKey, int iValue,LPCSTR file)
{
	char szValue[255];
	sprintf(szValue, "%d", iValue);
	WritePrivateProfileString(szSection,  szKey, szValue, file); 
}

//-----------------------------------------------------------------------------
// Name: Load()
// Desc: Loads Menu Item States From Previously Saved File
//-----------------------------------------------------------------------------

int cMenu::Load(char* szSection, char* szKey, int iDefaultValue,LPCSTR file)
{
	int iResult = GetPrivateProfileInt(szSection,  szKey, iDefaultValue, file); 
	return iResult;
}

//-----------------------------------------------------------------------------
// Name: additem()
// Desc: BaseFunction for aitem,acat,and atext
//-----------------------------------------------------------------------------

void cMenu::additem(char *title, char *states,int type, int *var, int show, int when)
{
	if(show==when)
	{
		strcpy(items[NO].title,title);
		getfield(states,items[NO].state,*var+1);
		items[NO].type=type;
		items[NO].max=nofields(states);
		items[NO].val=var;

		NO++;
	}
	if(type!=T_TEXT)
	{
		if (opt.options.load)
			*var = Load("Items", title, *var,Tools.GetDirectoryFile("settings.ini"));
		if (opt.options.save)
			Save("Items", title, *var,Tools.GetDirectoryFile("settings.ini"));
		if (opt.options.reset)
			*var=0;
	}
}

//-----------------------------------------------------------------------------
// Name: acat()
// Desc: Adds Folder/Category to Item index
//-----------------------------------------------------------------------------

void cMenu::acat(char *title, char *states,int *var)
{
	additem(title,states,T_CAT,var,0,0);
}

//-----------------------------------------------------------------------------
// Name: aitem()
// Desc: Adds regular item to Item index
//-----------------------------------------------------------------------------

void cMenu::aitem(char *title, char *states,int *var,int show,int when)
{
	additem(title,states,T_ITEM,var,show,when);
}

//-----------------------------------------------------------------------------
// Name: atext()
// Desc: Adds Text item to item index
//-----------------------------------------------------------------------------

void cMenu::atext(char *title, char *states,int show,int when)
{
	additem(title,states,T_TEXT,&opt.text,show,when);
}

void cMenu::atext(char *title, char *states)
{
	additem(title,states,T_TEXT,&opt.text,1,1);
}

//-----------------------------------------------------------------------------
// Name: nofields()
// Desc: Calculates Number of Fields in a String based on deliminator
//-----------------------------------------------------------------------------

int cMenu::nofields(char *str)
{
	char *ptr;
	int  no;
	for(no=1; (ptr=strchr(str,(char)'|'))!=NULL; no++)str=ptr+1;
	
	return no;
}

//-----------------------------------------------------------------------------
// Name: getfield()
// Desc: Grabs Field in string based on deliminator
//-----------------------------------------------------------------------------

void cMenu::getfield(char *str,char *dst,int no)
{
	char *ptr;
	int  i;
	for(i=1; (ptr=strchr(str,(char)'|'))!=NULL ; i++) 
	{
		if(i==no) break;
		str=ptr+1;
	}
	if(ptr)
	{
		i=(int)(ptr-str);
		strncpy(dst,str,i);
		dst[i]=0;
	}
	else
		strcpy(dst,str);
}

//-----------------------------------------------------------------------------
// Name: DrawBox()
// Desc: Fills Rectangle using DrawPrimitive Up
//-----------------------------------------------------------------------------

void cMenu::DrawBox( int x, int y, int w, int h, D3DCOLOR Color,LPDIRECT3DDEVICE9 pDevice)
{
	struct Vertex 
	{
		float x,y,z,ht;
		DWORD Color;
	}
	V[4] = 
	{
		{(float)x,(float)(y+h), 0.0f, 0.0f, Color},
		{(float)x,(float)y, 0.0f, 0.0f, Color},
		{(float)(x+w),(float)(y+h), 0.0f, 0.0f, Color},
		{(float)(x+w),(float)y, 0.0f, 0.0f, Color}
	};
	pDevice->SetTexture(0, NULL);
	pDevice->SetPixelShader(NULL);
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1); 
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,V,sizeof(Vertex));
	return;
}

//-----------------------------------------------------------------------------
// Name: DrawBorder()
// Desc: Fills outside of Rectangle using 4 calls to DrawBox
//-----------------------------------------------------------------------------

void cMenu::DrawBorder(int x, int y, int w, int h, int s, D3DCOLOR Color, LPDIRECT3DDEVICE9 pDevice)
{
    DrawBox(x, y-1, w, s, Color, pDevice );
    DrawBox(x-1, y, s, h, Color, pDevice );
    DrawBox((x+w), y, s, h, Color, pDevice );
    DrawBox(x, (y+h), w, s, Color, pDevice );
	return;
}

//-----------------------------------------------------------------------------
// Name: DrawGameTextPixelFlag()
//-----------------------------------------------------------------------------

void cMenu::DrawGameTextPixelFlag(int x, int y, DWORD color, DWORD flags, char *szFormat, ... )
{
    if( !szFormat )return;

    char logbuf[256] = { 0 };

    va_list va_alist;
    va_start (va_alist, szFormat);
    _vsnprintf(logbuf, sizeof(logbuf), szFormat, va_alist);
    va_end (va_alist);

    if( flags == DT_RIGHT )
    {
        SIZE Size = {0,0};
        if(font->GetTextExtent(logbuf, &Size) == S_OK)
        {
            x -= Size.cx;
        }
        flags = 0;
    } else if( flags == DT_CENTER )
    {
        SIZE Size = {0,0};
        if(font->GetTextExtent(logbuf, &Size) == S_OK)
        {
            x -= (Size.cx/2);
        }
        flags = 0;
    }
    font->DrawText((float)x+1, (float)y, D3DCOLOR_ARGB( 255, 0, 0, 0 ), logbuf, flags);
    font->DrawText((float)x-1, (float)y, D3DCOLOR_ARGB( 255, 0, 0, 0 ), logbuf, flags);
    font->DrawText((float)x, (float)y+1, D3DCOLOR_ARGB( 255, 0, 0, 0 ), logbuf, flags);
    font->DrawText((float)x, (float)y-1, D3DCOLOR_ARGB( 255, 0, 0, 0 ), logbuf, flags);
    font->DrawText((float)x, (float)y, color, logbuf, flags);
}

//-----------------------------------------------------------------------------
// Name: ShowMenu()
// Desc: Core Menu Function Displays Menu Calls Other Menu functions and navigation
//-----------------------------------------------------------------------------

void cMenu::ShowMenu(LPDIRECT3DDEVICE9 pDevice)
{
	//-----------------------------------------------------------------------------
	// Desc: Check Hotkeys
	//-----------------------------------------------------------------------------
	int	lm		=	GetAsyncKeyState(VK_LBUTTON)&1;
	int	rm		=	GetAsyncKeyState(VK_RBUTTON)&1;
	int	left	=	GetAsyncKeyState(VK_LEFT )&1;
	int	right	=	GetAsyncKeyState(VK_RIGHT)&1;
	int	up		=	GetAsyncKeyState(VK_UP   )&1;
	int	down	=	GetAsyncKeyState(VK_DOWN )&1;
	//-----------------------------------------------------------------------------
	// Desc: Prepare/Reset Menu
	//-----------------------------------------------------------------------------

	Init();

	//-----------------------------------------------------------------------------
	// Desc: Show The Menu
	//-----------------------------------------------------------------------------
	if(GetAsyncKeyState(VK_INSERT)&1) SHOW=(!SHOW);
	if(SHOW){

		//-----------------------------------------------------------------------------
		// Desc: Get Mouse Coordinates and translate
		//-----------------------------------------------------------------------------
		
		//-----------------------------------------------------------------------------
		// Desc: Move Menu
		//-----------------------------------------------------------------------------

		GetCursorPos(&mpos);// Update Mouse Coordinates
		ScreenToClient(GetForegroundWindow(),&mpos); // Translate to Current Window

		if(opt.options.move && GetAsyncKeyState(VK_LCONTROL))	// If Options for Left Control is Set
		{
			X =(((X<=mpos.x+4) && (X>=mpos.x)) || ((X>=mpos.x-4) && (X<=mpos.x)))?(X=mpos.x):(X+=((mpos.x-X)/4));
			Y =(((Y<=mpos.y+4) && (Y>=mpos.y)) || ((Y>=mpos.y-4) && (Y<=mpos.y)))?(Y=mpos.y):(Y+=((mpos.y-Y)/4));
		}

		//-----------------------------------------------------------------------------
		// Desc: Draw Menu Header
		//-----------------------------------------------------------------------------

		DrawBox(X,Y+3,WIDTH,T_SPC,C_BOX2,pDevice);// Render Menu Header Backround
		DrawBorder(X,Y+3,WIDTH,T_SPC,1,C_BORDER,pDevice);// set-up border
    	DrawGameTextPixelFlag(MC_FCTR(X,MC_MAXX),Y+5,C_TITLE,DT_CENTER|DT_SHADOW,"~|| D3D Menu 2013 ||~");
         
		//-----------------------------------------------------------------------------
		// Desc: Fill Item Array and Render Menu Border and Box
		//-----------------------------------------------------------------------------

		DrawBox(X,MC_MSY,WIDTH,(NO*I_SPC),C_BOX,pDevice);// Render Menu Backround for items
		DrawBorder(X,MC_MSY,WIDTH,(NO*I_SPC),1,C_BORDER,pDevice);// set-up border

		//Draw Left Box
		DrawBox(X-10,MC_MSY,5,(NO*I_SPC)+42,C_BOX2,pDevice);
		DrawBorder(X-10,MC_MSY,5,(NO*I_SPC)+42,1,C_BORDER,pDevice);

		//Draw Right Box
		DrawBox(X+WIDTH+5,MC_MSY,5,(NO*I_SPC)+42,C_BOX2,pDevice);
		DrawBorder(X+WIDTH+5,MC_MSY,5,(NO*I_SPC)+42,1,C_BORDER,pDevice);

		NO=0;
		acat("Direct3D","Show|Hide",&opt.d3d.main);
			aitem("CrossHair","OFF|ON",&opt.d3d.cross,opt.d3d.main,1);
			aitem("No Flash","OFF|ON",&opt.d3d.flash,opt.d3d.main,1);
			aitem("No Smoke","OFF|ON",&opt.d3d.smoke,opt.d3d.main,1);
			aitem("No Scope","OFF|ON",&opt.d3d.scope,opt.d3d.main,1);
			aitem("Asus","OFF|ON",&opt.d3d.asus,opt.d3d.main,1);
			aitem("FullBright","OFF|ON",&opt.d3d.fbright,opt.d3d.main,1);

		acat("OPTIONS","Show|Hide",&opt.options.main);
			aitem("Move Menu","OFF|L-Ctrl",&opt.options.move,opt.options.main,1);
			aitem("Save Set","OFF|Saving",&opt.options.save,opt.options.main,1);
			aitem("Load Set","OFF|Loading",&opt.options.load,opt.options.main,1);
			aitem("Reset Menu","OFF|Resetting",&opt.options.reset,opt.options.main,1);

		opt.options.save=0;
		opt.options.load=0;

		//-----------------------------------------------------------------------------
		// Desc: Loop Threw Item Index and Render Items
		//-----------------------------------------------------------------------------

		for(int no=0; no<NO; no++)
		{
			D3DCOLOR text;
			D3DCOLOR text2;
			D3DCOLOR text3;

			text=(*(items[no].val)>0)?C_ON:C_OFF;
			text2=(*(items[no].val)>0)?C_ON2:C_OFF2;
			text3=(*(items[no].val)>0)?C_ON2:C_OFF3;

			if(items[no].type==T_CAT)
				text=C_CAT;

			if(I_CUR==no){
				DrawBox(X,MC_ITEMY(no),WIDTH,I_SPC,C_CUR,pDevice);
			}

			if(mpos.x>X && mpos.x<MC_MAXX && mpos.y>MC_ITEMY(no) && mpos.y<MC_ITEMY(no)+I_SPC){
				DrawBox(X,MC_ITEMY(no),WIDTH,I_SPC,C_CUR,pDevice);
			}

			if(items[no].type!=T_CAT){
				DrawBox(X+5, MC_ITEMY(no)+5, 5, 5, text3, pDevice);
				DrawBorder(X+5, MC_ITEMY(no)+5, 5, 5,1, C_BORDER,pDevice);
			}

			if(items[no].type==T_CAT){
				text2=C_CAT;
				DrawGameTextPixelFlag(X+I_OFS,MC_ITEMY(no)+2,text,DT_SHADOW,items[no].title);
			}
			else{
				DrawGameTextPixelFlag(X+I_OFS+12,MC_ITEMY(no)+2,text,DT_SHADOW,items[no].title);
			}
			DrawGameTextPixelFlag(X+S_OFS,MC_ITEMY(no)+2,text2,DT_RIGHT|DT_SHADOW,items[no].state);
		}

		//-----------------------------------------------------------------------------
		// Desc: Draw Info and FrameRate()
		//-----------------------------------------------------------------------------

		// Draw Info
		DrawBox(X,MC_ITEMY1(NO)+(F_SPC/2)-2,WIDTH/2,(F_SPC),C_BOX,pDevice);
		DrawBorder(X,MC_ITEMY1(NO)+(F_SPC/2)-2,WIDTH/2,(F_SPC),1,C_BORDER,pDevice);
		DrawGameTextPixelFlag(X+(WIDTH)/4,MC_ITEMY1(NO)+(F_SPC/2)+1,C_INFO,DT_CENTER|DT_SHADOW,"Public");
		// Draw FrameRate
		DrawBox(X+(WIDTH)/2,MC_ITEMY1(NO)+(F_SPC/2)-2,WIDTH/2,(F_SPC),C_BOX,pDevice);
		DrawBorder(X+(WIDTH)/2,MC_ITEMY1(NO)+(F_SPC/2)-2,WIDTH/2,(F_SPC),1,C_BORDER,pDevice);
		DrawGameTextPixelFlag(X+((WIDTH)/4)*3,MC_ITEMY1(NO)+(F_SPC/2)+1,C_INFO,DT_CENTER|DT_SHADOW,FPS(0));

		//-----------------------------------------------------------------------------
		// Desc: Draw Menu Date and Time()
		//-----------------------------------------------------------------------------

		// Draw Date
		DrawBox(X,MC_ITEMY2(NO)+(F_SPC/2)-2,WIDTH/2,(F_SPC),C_BOX,pDevice);
		DrawBorder(X,MC_ITEMY2(NO)+(F_SPC/2)-2,WIDTH/2,(F_SPC),1,C_BORDER,pDevice);
		DrawGameTextPixelFlag(X+(WIDTH)/4,MC_ITEMY2(NO)+(F_SPC/2)+1,C_INFO2,DT_CENTER|DT_SHADOW,DATE());
		// Draw Time
		DrawBox(X+(WIDTH)/2,MC_ITEMY2(NO)+(F_SPC/2)-2,WIDTH/2,(F_SPC),C_BOX,pDevice);
		DrawBorder(X+(WIDTH)/2,MC_ITEMY2(NO)+(F_SPC/2)-2,WIDTH/2,(F_SPC),1,C_BORDER,pDevice);
		DrawGameTextPixelFlag(X+((WIDTH)/4)*3,MC_ITEMY2(NO)+(F_SPC/2)+1,C_INFO2,DT_CENTER|DT_SHADOW,TIME());

		//-----------------------------------------------------------------------------
		// Desc: Draw Menu Footer()
		//-----------------------------------------------------------------------------

		DrawBox(X,MC_ITEMY3(NO)+(F_SPC/2)-2,WIDTH,(F_SPC),C_BOX2,pDevice);// Draw Footer Filled
		DrawBorder(X,MC_ITEMY3(NO)+(F_SPC/2)-2,WIDTH,(F_SPC),1,C_BORDER,pDevice);// Footer Border
		DrawGameTextPixelFlag(MC_FCTR(X,MC_MAXX),MC_ITEMY3(NO)+(F_SPC/2),C_FOOTER,DT_CENTER|DT_SHADOW,"~|| www.uc-forum.com ||~");

		//-----------------------------------------------------------------------------
		// Desc: Mouse Navigation
		//-----------------------------------------------------------------------------

		int mno = 999; // Item Mouse is on

		if(mpos.x>X && mpos.x<MC_MAXX && mpos.y<MC_ITEMY(NO) && mpos.y>Y)
			mno = ((mpos.y-T_SPC)>Y)?((mpos.y - Y - T_SPC - (T_SPC/2)) / I_SPC):(999);

		if(mno!=999 && rm && (*items[mno].val)>0)
		{
			(*items[mno].val)-=1;
			Sound.wav.playsoundmem((LPCSTR)SWITCH);
		}

		if(mno!=999 && lm && (*items[mno].val)<(items[mno].max-1))
		{
			(*items[mno].val)+=1;
			Sound.wav.playsoundmem((LPCSTR)SWITCH);
		}

		//-----------------------------------------------------------------------------
		// Desc: Keyboard Navigation()
		//-----------------------------------------------------------------------------

		if(left && (*items[I_CUR].val)>0)
		{
			(*items[I_CUR].val)-=1;
			Sound.wav.playsoundmem((LPCSTR)SWITCH);
		}

		if(right && (*items[I_CUR].val)<(items[I_CUR].max-1))
		{
			(*items[I_CUR].val)+=1;
			Sound.wav.playsoundmem((LPCSTR)SWITCH);
		}

		if(up){
			do{
				I_CUR=(I_CUR==0)?(NO-1):(I_CUR-1);
			}while (items[I_CUR].type==T_TEXT);
			Sound.wav.playsoundmem((LPCSTR)MOVE);
		}

		if(down || items[I_CUR].type==T_TEXT){
			do{
				I_CUR=(I_CUR+1)%NO;
			}while (items[I_CUR].type==T_TEXT);
			Sound.wav.playsoundmem((LPCSTR)MOVE);
		}

		//-----------------------------------------------------------------------------
		// Desc: Menu Saftey / Sanity Checks and ReEncryption
		//-----------------------------------------------------------------------------

		if(I_CUR>NO)I_CUR=NO-1; // Prevent Selection From Hiding be hind hidden item

		//-----------------------------------------------------------------------------

		//Draw a Cursor; Full credits to Credzis
		Tools.DrawMouse(D3DXVECTOR2((float)mpos.x,(float)mpos.y),D3DCOLOR_RGBA(0,255,0,255));
		//-----------------------------------------------------------------------------
	}
}
