#include "trainerbase.h"

#include "minicsv.h"

/*
本文件函数用于循环读取内存以更新显示值
*/

// address table. might not be universal.
// will test after all wrapedup.


// 0. the followings are for all courts. 
DWORD exe_base_addr				= 0x00400000;	// nba2k11.exe基址，不知道会不会变化。后面的地址都是基址+偏移量后的地址。
// e.g. single practice, blacktop street, shooting practice, etc. 
DWORD score_type_addr			= 0x05D5FA70;	//  球与篮筐关系，0三不沾1砸框2空刷3打板 5大号空刷/偏出？ 7大号砸框进/砸板上沿8弹框而出，经过圆柱体
DWORD score_judge_addr			= 0x05D5FA78;	//  进球判断
DWORD coordinate_x_100_addr		= 0x05d5f728;	//  百倍横坐标 - 解说视角坐标系
DWORD coordinate_y_100_addr		= 0x05d5f720;	//  百倍纵坐标
DWORD absolute_dist_rim_addr	= 0x05d5f730;	//  距离篮筐绝对距离
DWORD projected_percent_addr	= 0x05d5f738;	// 预计投篮命中率，可参考以提升投篮选择
DWORD shot_triggered_time_addr	= 0x05db028c;	// 投篮释放时间，即按下z投篮键后键抬起的时间，可用于判断是否扣篮，秒数
DWORD total_time_elapsed_addr	= 0x05439C1C;	// 总流逝时间，包括暂停表演等，不包括esc游戏暂停

// 1. the followings are for quartered games, e.g.
// dynasty, quick game, mplayer, jordan, etc.
DWORD game_time_elapsed_addr	= 0x05544DF4;	// 比赛流逝时间，仅适用于分节比赛的模式，秒数

struct PerShotData {
	PerShotData() : score_type_s(0), score_judge_s(0), coordinate_x_100_s(0.0f), coordinate_y_100_s(0.0f), absolute_dist_rim_s(0.0f) {}
	PerShotData(int _score_type_s, int _score_judge_s, float _coordinate_x_100_s, float _coordinate_y_100_s, float _absolute_dist_rim_s)
		: score_type_s(_score_type_s), score_judge_s(_score_judge_s), coordinate_x_100_s(_coordinate_x_100_s), coordinate_y_100_s(_coordinate_y_100_s), absolute_dist_rim_s(_absolute_dist_rim_s) {}
	int score_type_s;
	int score_judge_s;
	float coordinate_x_100_s;
	float coordinate_y_100_s;
	float absolute_dist_rim_s;
};


void update_shot_coordinates(HANDLE pHandle) {
	ReadProcessMemory(pHandle, (LPVOID)coordinate_x_100_addr, &coordinate_x_100, sizeof(coordinate_x_100), 0);
	ReadProcessMemory(pHandle, (LPVOID)coordinate_y_100_addr, &coordinate_y_100, sizeof(coordinate_y_100), 0);
	ReadProcessMemory(pHandle, (LPVOID)absolute_dist_rim_addr, &rim_dist, sizeof(rim_dist), 0);
}


void update_shot_triggered_time(HANDLE pHandle) {
	ReadProcessMemory(pHandle, (LPVOID)shot_triggered_time_addr, &shot_triggered_time, sizeof(shot_triggered_time), 0);
}

void update_score_type(HANDLE pHandle) {
	ReadProcessMemory(pHandle, (LPVOID)score_type_addr, &score_type, sizeof(score_type), 0);
	ReadProcessMemory(pHandle, (LPVOID)score_judge_addr, &score_judge, sizeof(score_judge), 0);
}


void SaveDataFileLines() {
	char a[] = "C:\\Chen\\testdata.csv";
	csv::ofstream os(a);	// append mode
	os.set_delimiter(',', "");
	if (os.is_open()) {
		PerShotData PerShotData0(score_type, score_judge, coordinate_x_100, coordinate_y_100, rim_dist);
		os << PerShotData0.score_type_s << PerShotData0.score_judge_s << PerShotData0.coordinate_x_100_s << PerShotData0.coordinate_y_100_s << PerShotData0.absolute_dist_rim_s << NEWLINE;
	}
	os.flush();
	return;
}

// only read from global flags and the handle for address
void UpdateDMAs(HANDLE pHandle) {
	float shot_time_temp = shot_triggered_time;
	float coordinate_x_temp = coordinate_x_100;
	// aka direct memory access. manipulate the memory region.
	update_score_type(pHandle);
	update_shot_triggered_time(pHandle);
	if (shot_time_temp != shot_triggered_time) {
		if (made_shot_Z_down) {
			// then myself triggered a shot. update the x-ys
			update_shot_coordinates(pHandle);
			(coordinate_x_temp == coordinate_x_100) ? (is_a_dunk = true) : (is_a_dunk = false);	// the tricy part ;-)
			SaveDataFileLines();
			redraw_shotchart = true;	// serve as a lock to control the read from vector
			made_shot_Z_down = false;	// reset the state and wait for another shot
		}
	}
	else {
		redraw_shotchart = false;
	}
}