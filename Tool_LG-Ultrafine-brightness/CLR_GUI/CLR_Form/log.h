#pragma once

#ifndef LOG_H    
#define LOG_H    
// log file path 
#define LOG_FILE_NAME "CLRlog.txt"  
// log toggle
#define LOG_ENABLE  

#include <fstream>    
#include <string>    
#include <ctime> 
#include <direct.h>

using namespace std;

class CLog
{
public:
	static string GetLogFilePath()
	{
		char szPath[MAX_PATH];
		GetModuleFileNameA(NULL, szPath, MAX_PATH);
		// _getcwd(szPath, MAX_PATH);  // for working directory instead of executable file path
		string::size_type pos = string(szPath).find_last_of("\\/");
		string s = string(szPath).substr(0, pos);
		s += "\\";
		s += LOG_FILE_NAME;
		return s;
	}
	// 输出一个内容，可以是字符串(ascii)、整数、浮点数、布尔、枚举  
	// 格式为：[2011-11-11 11:11:11] aaaaaaa并换行  
	template <class T>
	static void WriteLog(T x)
	{
		ofstream fout(GetLogFilePath(), ios::app);
		fout.seekp(ios::end);
		fout << GetSystemTime() << x << endl;
		fout.close();
	}

	// 输出2个内容，以等号连接。一般用于前面是一个变量的描述字符串，后面接这个变量的值  
	template<class T1, class T2>
	static void WriteLog2(T1 x1, T2 x2)
	{
		ofstream fout(GetLogFilePath(), ios::app);
		fout.seekp(ios::end);  // for ios::app
		fout << GetSystemTime() << x1 << " = " << x2 << endl;
		fout.close();
	}

	// 输出一行当前函数开始的标志,宏传入__FUNCTION__  
	template <class T>
	static void WriteFuncBegin(T x)
	{
		ofstream fout(GetLogFilePath(), ios::out);
		// fout.seekp(ios::end);
		fout << GetSystemTime() << "--------------------" << x << "  Begin--------------------" << endl;
		fout.close();
	}

	// 输出一行当前函数结束的标志，宏传入__FUNCTION__  
	template <class T>
	static void WriteFuncEnd(T x)
	{
		ofstream fout(GetLogFilePath(), ios::app);
		fout.seekp(ios::end);
		fout << GetSystemTime() << "--------------------" << x << "  End  --------------------" << endl;
		fout.close();
	}


private:
	//获取本地时间，格式如"[2011-11-11 11:11:11] ";   
	static string GetSystemTime()
	{
		time_t tNowTime;
		time(&tNowTime);

		tm tLocalTime{};
		localtime_s(&tLocalTime, &tNowTime);
		char szTime[30] = { '\0' };
		strftime(szTime, 30, "[%Y-%m-%d %H:%M:%S] ", &tLocalTime);
		string strTime = szTime;
		return strTime;
	}

};

#ifdef LOG_ENABLE  

//用下面这些宏来使用本文件  
#define LOG(x)          CLog::WriteLog(x);						//括号内可以是字符串(ascii)、整数、浮点数、bool等  
#define LOG2(x1,x2)     CLog::WriteLog2(x1,x2);  
#define LOG_FUNC        LOG(__FUNCTION__)						//输出当前所在函数名  
#define LOG_LINE        LOG(__LINE__)							//输出当前行号  
#define LOG_FUNC_BEGIN  CLog::WriteFuncBegin(__FUNCTION__);     //形式如：[时间]"------------FuncName  Begin------------"  
#define LOG_FUNC_END    CLog::WriteFuncEnd(__FUNCTION__);		//形式如：[时间]"------------FuncName  End------------"  

#else  

#define LOG(x)                
#define LOG2(x1,x2)       
#define LOG_FUNC          
#define LOG_LINE          
#define LOG_FUNC_BEGIN    
#define LOG_FUNC_END      

#endif  

#endif