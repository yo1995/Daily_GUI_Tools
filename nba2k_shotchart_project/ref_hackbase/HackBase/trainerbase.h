#ifndef __TRAINERBASE_H__
#define __TRAINERBASE_H__

/*
The original hackbase was not specifically designed for toggle menus or other things
like drawing on the screen. (actually it's just a demo of dxhook, but I started my project on it!)
therefore I just implement a rather verbose/inefficient snippet to achieve our drawing needs.
OMG WTH for several time i just wanted to refactor the whole piece of s**t! so convoluted!
*/
#include "hackbase.h"

#pragma region hotkey related things
#include "hotkey.h"
#pragma endregion

#pragma region DMA related things
#include "memoryaccess.h"
#pragma endregion

extern int score_type;

extern char* F7_text;
extern char F7_text_f[];
extern char F7_text_t[];

extern char* score_type_text;
extern char score_type_0[];
extern char score_type_1[];
extern char score_type_2[];
extern char score_type_3[];
extern char score_type_other[];

extern bool all_players_god_mode;
extern bool record_shot_chart_and_more;
extern bool made_shot_prediction;

void onRender_clear(Renderer *renderer); 

void onRender_dashboard(Renderer *renderer);

void onRender_F7(Renderer *renderer);


#endif
