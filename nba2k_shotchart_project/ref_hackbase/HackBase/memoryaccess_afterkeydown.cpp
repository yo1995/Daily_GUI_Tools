#include "trainerbase.h"
/*
本文件函数用于在按键改变flag后读写内存
*/

DWORD god_mode_byte_addr = 0x0097D9A3;	// nba2k11.exe+57D99D - C7 05 78FAD505 00000000 - mov [nba2k11.exe+595FA78],00000000 | just change the A3 byte to 01


// 2. the following data are for SG player, particular for jordan mplayer mode
DWORD MIN_ADDR = 0x05C31B98;  // IN FLOAT seconds
DWORD PTS_ADDR = 0x05C31874;
DWORD FGA_ADDR = 0x05C31884;
DWORD FGM_ADDR = 0x05C31880;
DWORD PA3_ADDR = 0x05C3188C;
DWORD PM3_ADDR = 0x05C31888;
DWORD FTA_ADDR = 0x05C3187C;
DWORD FTM_ADDR = 0x05C31878;
DWORD FREB_ADDR = 0x05C31A3C;  // 前场板
DWORD BREB_ADDR = 0x05C31A40;  // 后场板
DWORD AST_ADDR = 0x05C31A78;
DWORD STL_ADDR = 0x05C31A6C;
DWORD BLK_ADDR = 0x05C31A70;
DWORD TOV_ADDR = 0x05C31A7C;
DWORD PLM_ADDR = 0x05C31A8C;  // 正负值


void read_end_of_game_data(HANDLE pHandle,
	float &min,
	int &pts,
	int &fga,
	int &fgm,
	int &pa3,
	int &pm3,
	int &fta,
	int &ftm,
	int &freb,
	int &breb,
	int &ast,
	int &stl,
	int &blk,
	int &tov,
	int &plm
) {
	ReadProcessMemory(pHandle, (LPVOID)MIN_ADDR, &min, sizeof(min), 0);
	ReadProcessMemory(pHandle, (LPVOID)PTS_ADDR, &pts, sizeof(pts), 0);
	ReadProcessMemory(pHandle, (LPVOID)FGA_ADDR, &fga, sizeof(fga), 0);
	ReadProcessMemory(pHandle, (LPVOID)FGM_ADDR, &fgm, sizeof(fgm), 0);
	ReadProcessMemory(pHandle, (LPVOID)PA3_ADDR, &pa3, sizeof(pa3), 0);
	ReadProcessMemory(pHandle, (LPVOID)PM3_ADDR, &pm3, sizeof(pm3), 0);
	ReadProcessMemory(pHandle, (LPVOID)FTA_ADDR, &fta, sizeof(fta), 0);
	ReadProcessMemory(pHandle, (LPVOID)FTM_ADDR, &ftm, sizeof(ftm), 0);
	ReadProcessMemory(pHandle, (LPVOID)FREB_ADDR, &freb, sizeof(freb), 0);
	ReadProcessMemory(pHandle, (LPVOID)BREB_ADDR, &breb, sizeof(breb), 0);
	ReadProcessMemory(pHandle, (LPVOID)AST_ADDR, &ast, sizeof(ast), 0);
	ReadProcessMemory(pHandle, (LPVOID)STL_ADDR, &stl, sizeof(stl), 0);
	ReadProcessMemory(pHandle, (LPVOID)BLK_ADDR, &blk, sizeof(blk), 0);
	ReadProcessMemory(pHandle, (LPVOID)TOV_ADDR, &tov, sizeof(tov), 0);
	ReadProcessMemory(pHandle, (LPVOID)PLM_ADDR, &plm, sizeof(plm), 0);
	// read the points scored out and print it to see if addresses are correct.
	ReadProcessMemory(pHandle, (LPVOID)PTS_ADDR, &PTS, sizeof(PTS), 0);
}

void change_god_mode(HANDLE pHandle) {
	int enabled = 1;
	int disabled = 0;

	if (all_players_god_mode) {
		WriteProcessMemory(pHandle, (LPVOID)god_mode_byte_addr, &enabled, sizeof(enabled), 0);
	}
	else {
		WriteProcessMemory(pHandle, (LPVOID)god_mode_byte_addr, &disabled, sizeof(disabled), 0);
	}
}



void UpdateDMA_afterKeyDown(HANDLE pHandle_r, HANDLE pHandle_w, SaveData *mSaveData) {
	if (IsKeyDown(VK_F6)) {
		if (record_shot_chart_and_more) {
			record_shot_chart_and_more = false;
			float min = 0;
			int pts = 0;
			int fga = 0;
			int fgm = 0;
			int pa3 = 0;
			int pm3 = 0;
			int fta = 0;
			int ftm = 0;
			int freb = 0;
			int breb = 0;
			int ast = 0;
			int stl = 0;
			int blk = 0;
			int tov = 0;
			int plm = 0;
			read_end_of_game_data(pHandle_r, min, pts, fga, fgm, pa3, pm3, fta, ftm, freb, breb, ast, stl, blk, tov, plm);
			mSaveData->SaveDataFileFooter(min, pts, fga, fgm, pa3, pm3, fta, ftm, freb, breb, ast, stl, blk, tov, plm);
		}
		else {
			record_shot_chart_and_more = true;
			mSaveData->SaveDataFileInitandOpen();
			mSaveData->SaveDataFileHeader();
		}
		// 显示：当前数据已记录，在graphic的 update bool中
	}

	if (IsKeyDown(VK_F7)) {
		// god mode
		all_players_god_mode ? (F7_text = F7_text_f) : (F7_text = F7_text_t);
		all_players_god_mode = !all_players_god_mode;
		change_god_mode(pHandle_w);
	}
}

/*
template<typename T>
void WriteMem(DWORD Address, T value)
{
DWORD Old;
VirtualProtect((LPVOID)Address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &Old);
//VirtualProtectEx(hProcess, (LPVOID)Address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &Old);
WriteProcessMemory(hProcess, (LPVOID)Address, &value, sizeof(T), NULL);
//VirtualProtectEx(hProcess, (LPVOID)Address, sizeof(DWORD), Old, &Old);
VirtualProtect((LPVOID)Address, sizeof(DWORD), Old, &Old);
};
*/