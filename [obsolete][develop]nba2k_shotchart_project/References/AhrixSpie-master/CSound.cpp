#include "CSound.h"

#pragma comment(lib, "WinMM.Lib")

void cSound::swav::playsound(LPCSTR sound)
{
	sndPlaySound(sound,SND_ASYNC);
}
void cSound::swav::playsoundmem(LPCSTR sound)
{
	sndPlaySound(sound,SND_ASYNC|SND_MEMORY);
}
