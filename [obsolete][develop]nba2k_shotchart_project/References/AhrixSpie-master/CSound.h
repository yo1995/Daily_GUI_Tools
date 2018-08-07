#include "StandardIncludes.h"
#include <mmsystem.h>

#pragma once

// Stripped Sound Class kinda pointless for public release since i stripped all the cool functions but more pain to remove lol

class cSound
{
	
public:

	struct smp3
	{
	
	}mp3;

	struct swav
	{
	void playsound(LPCSTR sound);
	void playsoundmem(LPCSTR sound);
	}wav;

};
extern cSound Sound;



