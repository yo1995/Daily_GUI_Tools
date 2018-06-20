#ifndef __STRING_H__
#define __STRING_H__

#include "hackbase.h"

#pragma warning (push)
#pragma warning (disable: 4996)
class String {
private:
	char *_data;

public:
	String() {
		_data = new char[1];
		_data[0] = '\0';
	}

	String(char *s) {
		//int size = strlen(s) + 1;
		//_data = new char[size];
		//strcpy(_data, s);
		//_data[size - 1] = '\0';
		_data = s;
	}

	void operator=(char *s) {
		//int size = strlen(s) + 1;
		//_data = new char[size];
		//strcpy(_data, s);
		//_data[size - 1] = '\0';
		if(_data)
			delete[] _data;
		_data = s;
	}

	char *operator+(const char *s) const {
		int size = strlen(_data) + strlen(s) + 1;
		char *ret = new char[size];
		strcpy(ret, _data);
		strcat(ret, s);
		ret[size - 1] = '\0';
		return ret;
	}

	void operator+=(const char *s) {
		int size = strlen(_data) + strlen(s) + 1;
		char *ret = new char[size];
		strcpy(ret, _data);
		strcat(ret, s);
		ret[size - 1] = '\0';
		delete[] _data;
		_data = ret;
	}

	char operator[](const int i) const {
		return _data[i];
	}

	operator char*() const {
		return _data;
	}

	bool operator==(const char *s) const {
		return !strcmp(_data, s);
	}

#pragma warning (push)
#pragma warning (disable: 4800)
	bool operator!=(const char *s) const {
		return strcmp(_data, s);
	}
#pragma warning (pop)

	int length() const {
		return strlen(_data);
	}
};
#pragma warning (pop)

#endif