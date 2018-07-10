#include "hackbase.h"

void log(const char *Text, ...) {
#ifdef LOGTOFILE
	FILE *logfile;
	fopen_s(&logfile, LOGTOFILE, "a");
#endif

#if defined LOGTOCONSOLE | defined LOGTOFILE
	char buf[1024];
	va_list arglist;
	va_start(arglist, Text);
	vsnprintf_s(buf, 1023, Text, arglist);
	va_end(arglist);
#endif

#ifdef LOGTOFILE
	if(logfile) {
		fprintf(logfile, "%s", buf);
		fclose(logfile);
	}
#endif

#ifdef LOGTOCONSOLE
	printf("%s", buf);
#endif
}

void logln() {
	logln("");
}

void logln(const char *Text, ...) {
#if defined LOGTOCONSOLE | defined LOGTOFILE
	char buf[1024];
	va_list arglist;
	va_start(arglist, Text);
	vsnprintf_s(buf, 1023, Text, arglist);
	va_end(arglist);

	log("%s\n", buf);
#endif
}

void _log(char *File, int Line, const char *Text, ...) {
#if defined LOGTOCONSOLE | defined LOGTOFILE
	char buf[1024];
	va_list arglist;
	va_start(arglist, Text);
	vsnprintf_s(buf, 1023, Text, arglist);
	va_end(arglist);

	log("[%s:%d] %s", File, Line, buf);
#endif
}

void _logln(char *File, int Line, const char *Text, ...) {
#if defined LOGTOCONSOLE | defined LOGTOFILE
	char buf[1024];
	va_list arglist;
	va_start(arglist, Text);
	vsnprintf_s(buf, 1023, Text, arglist);
	va_end(arglist);

	log("[%s:%d] %s\n", File, Line, buf);
#endif
}