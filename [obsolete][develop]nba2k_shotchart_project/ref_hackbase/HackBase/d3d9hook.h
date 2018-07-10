#ifndef __D3D9HOOK_H__
#define __D3D9HOOK_H__

#include "hackbase.h"

class HackBase;

class D3D9Hook
{
private:
	D3D9Hook();

public:
	static D3D9Hook *Singleton();
	~D3D9Hook();
};

#endif