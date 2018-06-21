#include "trainerbase.h"

int border_width = 1;
int column_width = 60;
int column_height = 25;


char* F7_text;
char F7_text_f[] = "F7: Off";
char F7_text_t[] = "F7: On";

bool all_players_god_mode = false;
bool record_shot_chart_and_more = false;
bool made_shot_prediction = false;



void onRender_clear(Renderer *renderer) {
	// by default we do not draw things on the screen. this is the default.
}

void onRender_dashboard(Renderer *renderer) {
	// draw rect background
	renderer->DrawRect(0, 0, 1 * column_width, column_height, GRAY(64));
	renderer->DrawRect(0, 0, 2 * column_width, column_height, GRAY(64));
	renderer->DrawRect(0, 0, 3 * column_width, column_height, GRAY(64));
	renderer->DrawRect(0, 0, 4 * column_width, column_height, GRAY(64));
	// draw border
	renderer->DrawBorder(0, 0, 1 * column_width, column_height, border_width, GREEN(255));
	renderer->DrawBorder(0, 0, 2 * column_width, column_height, border_width, GREEN(255));
	renderer->DrawBorder(0, 0, 3 * column_width, column_height, border_width, GREEN(255));
	renderer->DrawBorder(0, 0, 4 * column_width, column_height, border_width, GREEN(255));
	// draw text
	

	renderer->DrawText(border_width, border_width, FontColor_default, F7_text);
	renderer->DrawText(border_width + column_width, border_width, FontColor_default, score_type_text);
}

void onRender_F7(Renderer *renderer) {
	renderer->DrawText((int)(0.5 * renderer->GetWidth()), (int)(0.5 * renderer->GetHeight()), FontColor_default, F7_text);
}
