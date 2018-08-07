#include "StandardIncludes.h"
#include "CFont.h"
#include "CTools.h"

#pragma once

#define ITEM_ARRAY 64 // Maximum of 64 Items change to reduce Static address size

// ============ SOME CALCULATION MACROS FOR LONG CALCULATIONS ================
// ============                THE PREFIX IS MC				  ================
#define MC_MAXX					(X+WIDTH)												// X of Menu + Width
#define MC_FCTR(x,y)			((x+y)/2)												// Calculate Center Point Between two items
#define MC_ITEMY(index)			(Y+(index*I_SPC)+T_SPC+(T_SPC/2))						// Calculate Y of Item using Index or calculate Menu Maximum Y
#define MC_ITEMY1(index)		(Y+(index*I_SPC)+T_SPC+(T_SPC/2)-1)
#define MC_ITEMY2(index)		(Y+(index*I_SPC)+T_SPC+(T_SPC/2)+20)
#define MC_ITEMY3(index)		(Y+(index*I_SPC)+T_SPC+(T_SPC/2)+41)
#define MC_ITEMY4(index)		(Y+(index*I_SPC)+T_SPC+(T_SPC/2)+60)
#define MC_ITEMY5(index)		(Y+(index*I_SPC)+T_SPC+(T_SPC/2)+81)
#define MC_MSY					(Y+T_SPC+(T_SPC/2))

// ============ Menu Manger Class ================
class cMenu{

public:

//-----------------------------------------------------------------------------
// Desc: Item Index Structure
//-----------------------------------------------------------------------------
	struct sitem{
			char title[50];		// Name Of Item to Display
			char state[50];	// States of Item
			int	 type;			// Type of Item ie. Cat , Items
			int  max;			// Number of States
			int  no;			// number inside the Array
			int	 *val;			// Variable Being Controlled
	} items[ITEM_ARRAY];
//-----------------------------------------------------------------------------
// Desc: Item Color Enumeration
//-----------------------------------------------------------------------------
	enum COLORS{  // Normal Colors
		WHITE	=	D3DCOLOR_ARGB(255,255,255,255),
		WHITE2	=	D3DCOLOR_ARGB(160,255,255,255),
		RED		=	D3DCOLOR_ARGB(255,255,0,0),
		GREEN	=	D3DCOLOR_ARGB(255,0,255,0),
		BLUE	=	D3DCOLOR_ARGB(255,0,0,255),
		BLACK	=	D3DCOLOR_ARGB(255,0,0,0),
		PURPLE	=	D3DCOLOR_ARGB(255,125,0,255),
		GREY	=	D3DCOLOR_ARGB(255,128,128,128),
		YELLOW  =	D3DCOLOR_ARGB(255,255,255,0),
		ORANGE	=	D3DCOLOR_ARGB(255,255,125,0)
	};
//-----------------------------------------------------------------------------
// Desc: Item Color Enumeration
//-----------------------------------------------------------------------------
	enum I_COLORS{  // Item Colors
		C_OFF	 =	D3DCOLOR_ARGB(255, 200, 200, 200),
		C_ON	 =  D3DCOLOR_ARGB(255, 255, 255, 255),
		C_OFF2	 =	D3DCOLOR_ARGB(255, 200, 200, 200),
		C_OFF3	 =	D3DCOLOR_ARGB(255, 255,   0,  0 ),
		C_ON2	 =  D3DCOLOR_ARGB(255,   0, 255,  0 ),
		C_CAT	 =	D3DCOLOR_ARGB(255, 255,   0,  0 ),
		C_INFO	 =	D3DCOLOR_ARGB(255,	 0, 191, 255),
		C_INFO2	 =	D3DCOLOR_ARGB(255, 250, 250, 250),
		C_CUR	 =	D3DCOLOR_ARGB(255,	 0, 191, 255),
		C_FOOTER =	D3DCOLOR_ARGB(255,   0, 255,  0 ),
		C_TITLE	 =	D3DCOLOR_ARGB(255,   0, 255,  0 )
	};
//-----------------------------------------------------------------------------
// Desc: BackGround Color Enumeration
//-----------------------------------------------------------------------------
	enum BK_COLORS{ // BackGround Colors
		C_BOX    = D3DCOLOR_ARGB(220,50,50,50),
		C_BOX2   = D3DCOLOR_ARGB(220,255,0,0),
		C_BORDER = D3DCOLOR_ARGB(220,5,5,5)
	};

//-----------------------------------------------------------------------------
// Desc: Menu Types Enumeration
//-----------------------------------------------------------------------------

	enum I_TYPES{
		T_TEXT = 0,
		T_ITEM = 1,
		T_CAT  = 2
	};

//-----------------------------------------------------------------------------
// Desc: Class Global Variables
//-----------------------------------------------------------------------------

	CD3DFont *font;
	bool FontCreated;
	int  X,Y;			// TOP LEFT OF MENU
	POINT mofs;			// Offset From Mouse Last Point
	POINT mpos;			// Mouse Coordinates
	int  M_DRAG;		// is Menu Being Dragged
	int  WIDTH;			// Width of Menu
	int  I_OFS,S_OFS;	// Item Title Column Start
	int  I_SPC,I_TSPC;  // Title Spacing and Item Spacing
	int  T_SPC,F_SPC;	// Title Spacing from Menu Items and Footer Spacing
	int  NO;			// Number of Menu Items
	int  I_CUR;			// Current Menu Item
	bool SHOW;			// Is menu shown
	bool INIT;			// Has Menu Been Setup

//-----------------------------------------------------------------------------
// Desc: Class Global Functions
//-----------------------------------------------------------------------------

	void	DrawBox( int x, int y, int w, int h, D3DCOLOR Color,LPDIRECT3DDEVICE9 pDevice);
	void	DrawBorder(int x, int y, int w, int h, int s, D3DCOLOR Color, LPDIRECT3DDEVICE9 device);
	void	DrawGameTextPixelFlag(int x, int y, DWORD color, DWORD flags, char *szFormat, ... );
	char*	oprintf (const char *fmt, ...);
	void	PostReset(LPDIRECT3DDEVICE9 pDevice);
	void	PreReset(void);
    char*	FPS(int en = 1);
	char*	TIME(void);
	char*	DATE(void);
	void	ShowMenu(LPDIRECT3DDEVICE9 pDevice);
	void	ShowBox(LPDIRECT3DDEVICE9 pDevice);

//-----------------------------------------------------------------------------
private:

//-----------------------------------------------------------------------------
// Desc: Class Private Functions
//-----------------------------------------------------------------------------
	void	acat(char *title, char *states, int *var);
	void	aitem(char *title, char *states,int *var, int show, int when);
	void	atext(char *title, char *states,int show, int when);
	void	atext(char *title, char *states);
	void	additem(char *title, char *states,int type, int *var, int show, int when);
	void	getfield(char *str,char *dst,int no);
	int		nofields(char *str);
	void	Init(void);
	int		Load(char* szSection, char* szKey, int iDefaultValue,LPCSTR file);
	void	Save(char* szSection, char* szKey, int iValue,LPCSTR file);

//-----------------------------------------------------------------------------

};
extern cMenu Menu;

// Item Manager Struct
struct copt{
//Global Variables
int text; // Text Items
		struct sinfo{
				int main;
		}info;
		struct sd3d{
				int main;
				int cross;
				int asus;
				int naked;
				int flash;
				int smoke;
				int scope;
				int fbright;
		}d3d;
		struct soptions{
				int main;
				int move;
				int reset;
				int save;
				int load;
		}options;
};
extern copt opt;
