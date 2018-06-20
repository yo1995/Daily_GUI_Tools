#ifndef __DXGIHOOK_H__
#define __DXGIHOOK_H__

#include "hackbase.h"

class HackBase;

class DXGIHook
{
private:
	DXGIHook();

public:
	static DXGIHook *Singleton();
	~DXGIHook();
};

#endif