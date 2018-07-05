#include "trainerbase.h"


// i have never defined struct like this, but it works!
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


void SaveData::Get_Current_Time() {
	time_t rawtime;
	struct tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y%m%d-%H-%M", &timeinfo);
	this->current_time.clear();
	this->current_time = tmp;
}

/*
struct HeaderData {
HeaderData() : score_type_s(0), score_judge_s(0), coordinate_x_100_s(0.0f), coordinate_y_100_s(0.0f), absolute_dist_rim_s(0.0f) {}
HeaderData(int _score_type_s, int _score_judge_s, float _coordinate_x_100_s, float _coordinate_y_100_s, float _absolute_dist_rim_s)
: score_type_s(_score_type_s), score_judge_s(_score_judge_s), coordinate_x_100_s(_coordinate_x_100_s), coordinate_y_100_s(_coordinate_y_100_s), absolute_dist_rim_s(_absolute_dist_rim_s) {}
int score_type_s;
int score_judge_s;
float coordinate_x_100_s;
float coordinate_y_100_s;
float absolute_dist_rim_s;
};
*/

void SaveData::SaveDataFileHeader() {
	this->filename.clear();
	this->Get_Current_Time();
	this->filename = "C:\\Chen\\" + this->current_time + "-game-record.csv";

	csv::ofstream os(this->filename.c_str());	// append mode
	os.set_delimiter(',', "");
	if (os.is_open()) {
		// HeaderData HeaderData0(score_type, score_judge, coordinate_x_100, coordinate_y_100, rim_dist);
		// os << PerShotData0.score_type_s << PerShotData0.score_judge_s << PerShotData0.coordinate_x_100_s << PerShotData0.coordinate_y_100_s << PerShotData0.absolute_dist_rim_s << NEWLINE;
		os << current_time << NEWLINE;  // write game time
	}
	os.flush();
	return;
}

void SaveData::SaveDataFileFooter(
	float min,
	int pts,
	int fga,
	int fgm,
	int pa3,
	int pm3,
	int fta,
	int ftm,
	int freb,
	int breb,
	int ast,
	int stl,
	int blk,
	int tov,
	int plm
) {
	csv::ofstream os(this->filename.c_str());	// append mode
	os.set_delimiter(',', "");
	if (os.is_open()) {
		os << "MIN" << min << NEWLINE;  // write game time
		os << "PTS" << pts << NEWLINE;  
		os << "FGM" << fgm << NEWLINE;  
		os << "FGA" << fga << NEWLINE;  
		os << "3PM" << pm3 << NEWLINE;  
		os << "3PA" << pa3 << NEWLINE; 
		os << "FTM" << ftm << NEWLINE;
		os << "FTA" << fta << NEWLINE;
		os << "REB" << freb + breb << NEWLINE;
		os << "AST" << ast << NEWLINE;
		os << "STL" << stl << NEWLINE;
		os << "BLK" << blk << NEWLINE;
		os << "TOV" << tov << NEWLINE;
		os << "PLM" << plm << NEWLINE;
		os << "---All data saved. Voila!---" << NEWLINE;
	}
	os.flush();
	return;
}


void SaveData::SaveDataFileLines() {
	// char a[] = "C:\\Chen\\testdata.csv";
	csv::ofstream os(this->filename.c_str());	// append mode
	os.set_delimiter(',', "");
	if (os.is_open()) {
		if (is_a_dunk) {
			PerShotData PerShotData0(0, score_judge, 1250.0, 0.0, 0.0);  // approx. x=1250cm, y=0 d=0 obviously
			os << PerShotData0.score_type_s << PerShotData0.score_judge_s << PerShotData0.coordinate_x_100_s << PerShotData0.coordinate_y_100_s << PerShotData0.absolute_dist_rim_s << NEWLINE;
		}
		else {
			PerShotData PerShotData0(score_type, score_judge, coordinate_x_100, coordinate_y_100, rim_dist);
			os << PerShotData0.score_type_s << PerShotData0.score_judge_s << PerShotData0.coordinate_x_100_s << PerShotData0.coordinate_y_100_s << PerShotData0.absolute_dist_rim_s << NEWLINE;
		}
	}
	os.flush();
	return;
}