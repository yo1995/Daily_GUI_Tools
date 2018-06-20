#pragma once

#include "Windows.h"
#include <vector>

using std::vector;

typedef uintptr_t ADDRESS;
typedef uintptr_t OFFSET;

class Hack {
private:
	union {
		ADDRESS BaseAddress;
		ADDRESS StaticAddress;
	};
	vector<OFFSET> Offsets;
	int pLevel = -1;

public:
	Hack() = delete;
	Hack(ADDRESS StaticAddress);
	Hack(ADDRESS BaseAddy, vector<OFFSET> Offsets);

	ADDRESS GetAddress();
	bool IsPointer();

	void SetValue(void* ValueAddress, int Bytes);

	void GetValue(void* DestAddress, int Bytes);

	template<typename T>
	static void SetValue(Hack hack, T Value) {
		ADDRESS Address = hack.GetAddress();

		*(T*)Address = Value;
	}

	template<typename T>
	static T GetValue(Hack hack) {
		ADDRESS Address = hack.GetAddress();

		return (Address) ? *(T*)Address : NULL;
	}
};