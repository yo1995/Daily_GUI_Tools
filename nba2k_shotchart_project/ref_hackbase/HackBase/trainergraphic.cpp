#include "trainerbase.h"

int border_width = 2;
int column_width = 100;
int column_height = 20;
int court_bg_x = 240;	// in pixels
int court_bg_y = 225;
int dot_size = 8;

// F6 display data and coordinates
// char* score_type_text;
int score_type = 0;
int score_judge = 0;
char* score_type_text = NULL;
char* score_judge_text = NULL;
char score_type_0[] = "三不沾";
char score_type_1[] = "砸框";
char score_type_2[] = "空刷";
char score_type_3[] = "打板";
char score_type_8[] = "弹框出";
char score_type_other[] = "其它";
char score_judge_0[] = "没进";
char score_judge_1[] = "进了";


// F7 god_mode related
// char 
char* F7_text = F7_text_f;
char F7_text_f[] = "GodMode: Off";
char F7_text_t[] = "GodMode: On";


// F8 toggle dashboard on/off


bool record_shot_chart_and_more = false;
bool made_shot_prediction = false;
bool clear_screen = false;
bool clear_screen_already_cleared = false;
bool all_players_god_mode = false;
bool redraw_shotchart = false;
bool is_a_dunk = false;

float coordinate_x_100 = 0;
float coordinate_y_100 = 0;
float rim_dist = 0;
float shot_triggered_time = 0;

void onRender_clear(Renderer *renderer) {
	// by default we do not draw things on the screen. this is the default.
}

void onRender_shotchart(Renderer *renderer) {
	// the bg is rotated 90 deg, therefore flip
	int x_0 = renderer->GetWidth() - 0.5 * court_bg_x;
	int y_0 = renderer->GetHeight() - court_bg_y;
	float ratioy = court_bg_x / 1500.0f;	// 1500*1410
	float ratiox = court_bg_y / 1440.0f;
	// flip x-y coordinate if x < 0 (court change in quarters - actually no need)
	// what about backcourt scorers?


	// windowed mode have borders and offsets
	renderer->DrawPic((renderer->GetWidth() - court_bg_x - 8 * border_width), (renderer->GetHeight() - court_bg_y - 15 * border_width));
	if (redraw_shotchart) {	// means that i'm not writing into the coordinates
		if (is_a_dunk) {
			if (score_judge) { // scored
				renderer->DrawRect((renderer->GetWidth() + court_bg_x + 8 * border_width + 118), (renderer->GetHeight() + court_bg_y + 15 * border_width + 26), dot_size, dot_size, GREEN(255));
			}
			else {	// missed
				renderer->DrawRect((renderer->GetWidth() + court_bg_x + 8 * border_width + 118), (renderer->GetHeight() + court_bg_y + 15 * border_width + 26), dot_size, dot_size, RED(255));
			}
			//120, 26 is the coord of basket
		}
		else {
			if (score_judge) { // scored
				renderer->DrawRect(x_0 + coordinate_y_100 * ratioy + 1, y_0 + coordinate_x_100 * ratiox + 1, dot_size, dot_size, GREEN(255));
			}
			else {	// missed
				renderer->DrawRect(x_0 + coordinate_y_100 * ratioy + 1, y_0 + coordinate_x_100 * ratiox + 1, dot_size, dot_size, RED(255));
			}
		}
	}
		
	// pass in a vector<missed, pair<x, y> >
	// related with r/w mutex, cannot read when writing to the var. 
	// maybe i can use a bool to serve as a lock? since bool is always thread safe...?
	
}

void onRender_dashboard(Renderer *renderer) {
	//renderer->InitCreateFuncs();
	// draw rect background
	/*
	renderer->DrawRect(0, 0, 1 * column_width, column_height, GRAY(64));
	renderer->DrawRect(0, 0, 2 * column_width, column_height, GRAY(64));
	renderer->DrawRect(0, 0, 3 * column_width, column_height, GRAY(64));
	renderer->DrawRect(0, 0, 4 * column_width, column_height, GRAY(64));
	*/
	// draw border
	renderer->DrawBorder(0, 0, column_width, 1 * column_height, border_width, GREEN(255));
	renderer->DrawBorder(0, 0, column_width, 2 * column_height, border_width, GREEN(255));
	renderer->DrawBorder(0, 0, column_width, 3 * column_height, border_width, GREEN(255));
	// renderer->DrawBorder(0, 0, 4 * column_width, column_height, border_width, GREEN(255));
	// draw text
	
	renderer->DrawTxt(border_width, 1 + border_width, FontColor_default, F7_text);
	renderer->DrawTxt(border_width, 1 + border_width + column_height, FontColor_default, score_type_text);
	renderer->DrawTxt(border_width, 1 + border_width + 2 * column_height, FontColor_default, score_judge_text);

	/* do not need to expose the raw data to regular users.
	char temp_str[255];
	sprintf_s(temp_str, "%.2f", coordinate_x_100); //将100转为16进制表示的字符串。
	renderer->DrawTxt(border_width, column_height + border_width, FontColor_default, temp_str);
	sprintf_s(temp_str, "%.2f", coordinate_y_100);
	renderer->DrawTxt(border_width + column_width, column_height + border_width, FontColor_default, temp_str);
	sprintf_s(temp_str, "%.2f", rim_dist);
	renderer->DrawTxt(border_width + 2 * column_width, column_height + border_width, FontColor_default, temp_str);
	*/

	onRender_shotchart(renderer);
}



void UpdateBools() {
	// 球筐类型判断
	switch (score_type) {
	case 0:
		score_type_text = score_type_0;
		break;
	case 1:
		score_type_text = score_type_1;
		break;
	case 2:
		score_type_text = score_type_2;
		break;
	case 3:
		score_type_text = score_type_3;
		break;
	case 8:
		score_type_text = score_type_8;
		break;
	default:
		score_type_text = score_type_other;
		break;
	}
	// 进球判断
	switch (score_judge) {
	case 0:
		score_judge_text = score_judge_0;
		break;
	case 1:
		score_judge_text = score_judge_1;
		break;
	default:
		break;
	}
}
// only read from global flags and the render handle for graphic
void UpdateGraphics(HackBase *mHackBase) {
	UpdateBools();

	if (clear_screen && clear_screen_already_cleared) {
		return;
	}
	else if (clear_screen && !clear_screen_already_cleared) {
		mHackBase->setOnRender(onRender_clear);
		clear_screen_already_cleared = true;
	}
	else {
		if (clear_screen_already_cleared) {
			clear_screen_already_cleared = false;	// reset flag 
		}
		mHackBase->setOnRender(onRender_dashboard);
	}
}




/*samples */
/*
void onRender_F7(Renderer *renderer) {
	renderer->DrawText((int)(0.5 * renderer->GetWidth()), (int)(0.5 * renderer->GetHeight()), FontColor_default, F7_text);
}


*/

/*
void onRender(Renderer *renderer) {
// renderer->DrawBorder(0, 0, renderer->GetWidth(), renderer->GetHeight(), 5, GREEN(255));
// renderer->DrawLine(0, 0, renderer->GetWidth(), renderer->GetHeight(), 2, false, Color(255, 0, 255, 0));
// renderer->DrawText((int)(0.5 * renderer->GetWidth()), (int)(0.5 * renderer->GetHeight()), inject_text);
// renderer->DrawCircle((int)(0.5 * renderer->GetWidth()), (int)(0.5 * renderer->GetHeight()), 30, 2, 30, Color(255, 0, 255, 0));
}
*/
