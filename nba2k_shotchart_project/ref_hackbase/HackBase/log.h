#ifndef __LOG_H__
#define __LOG_H__

#include "hackbase.h"

#define dbglog(Text, ...) _log(__FILE__, __LINE__, Text,__VA_ARGS__);
#define dbglogln(Text, ...) _logln(__FILE__, __LINE__, Text, __VA_ARGS__);

void log(const char *Text, ...);
void logln();
void logln(const char *Text, ...);
void _log(char *File, int Line, const char *Text, ...);
void _logln(char *File, int Line, const char *Text, ...);

#endif