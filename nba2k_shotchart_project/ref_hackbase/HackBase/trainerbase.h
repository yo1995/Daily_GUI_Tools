#ifndef __TRAINERBASE_H__
#define __TRAINERBASE_H__

/*
The original hackbase was not specifically designed for toggle menus or other things
like drawing on the screen. (actually it's just a demo of dxhook, but I started my project on it!)
therefore I just implement a rather verbose/inefficient snippet to achieve our drawing needs.
OMG WTH for several time i just wanted to refactor the whole piece of s**t! so convoluted!
*/
#include "hackbase.h"
#include "minicsv.h"

#define VERBOSEMODE false

// F7 was dedicated to be toggle of god mode.
extern char* F7_text;
extern char F7_text_f[];
extern char F7_text_t[];
extern bool all_players_god_mode;

// to decide the genre of ball into the rim.
extern int score_type;
extern int score_judge;
extern char* score_type_text;
extern char* score_judge_text;
extern char score_type_0[];
extern char score_type_1[];
extern char score_type_2[];
extern char score_type_3[];
extern char score_type_8[];
extern char score_type_other[];
extern char score_judge_0[];
extern char score_judge_1[];

// time related
extern char total_time_elapsed_text[];
extern char game_time_elapsed_text[];
extern char shot_triggered_time_text[];

// general flags for toggling.
extern bool record_shot_chart_and_more;
extern bool clear_screen;
extern bool clear_screen_already_cleared;

// shotchart related
extern bool redraw_shotchart;	// if true, then read from vector and redraw; otherwise omit.
extern float coordinate_x_100;
extern float coordinate_y_100;
extern float rim_dist;
extern float shot_triggered_time;
extern bool is_a_dunk;
extern bool made_shot_Z_down;

void onRender_clear(Renderer *renderer); 

void onRender_dashboard(Renderer *renderer);
void onRender_shotchart(Renderer *renderer);

// void onRender_F7(Renderer *renderer) {};

// 读写判断值，读写显示字符串，实现开关模式
void SaveDataFileHeader();
bool IsKeyDown(DWORD key);
void UpdateHotkeys();
// 读取判断值，读写内存数据，实现读取投篮数据/写入无敌模式
void update_shot_coordinates(HANDLE pHandle);	// only used in sub, no need to put here
void update_score_type(HANDLE pHandle);
void change_god_mode(HANDLE pHandle);
void SaveDataFileLines();
void UpdateDMAs(HANDLE pHandle);
void UpdateDMA_afterKeyDown(HANDLE pHandle_r, HANDLE pHandle_w);
// 读取判断值，绘制屏幕
void UpdateBools();
void UpdateGraphics(HackBase *mHackBase);


// dll hijacking
typedef void* (WINAPI* Direct3DCreate9Type)(UINT SDKVersion);
extern Direct3DCreate9Type RealDirect3DCreate9;
#include <SDKDDKVer.h>
#include <tchar.h>


#endif
