#include "Hack.h"

Hack::Hack(ADDRESS StaticAddress) {
	this->StaticAddress = StaticAddress;
}

Hack::Hack(ADDRESS BaseAddress, vector<OFFSET> Offsets) {
	this->BaseAddress = BaseAddress + (ADDRESS)GetModuleHandle(NULL);
	this->Offsets = Offsets;
	this->pLevel = Offsets.size();
}

ADDRESS Hack::GetAddress() {
	if (IsPointer()) {
		ADDRESS Ptr = *(ADDRESS*)BaseAddress;

		if (!Ptr) return NULL;

		for (int i = 0; i < pLevel - 1; i++) {
			Ptr = *(ADDRESS*)(Ptr + Offsets[i]);

			if (!Ptr) return NULL;
		}

		Ptr += Offsets[pLevel - 1];

		return Ptr;
	}
	else {
		return StaticAddress;
	}
}

bool Hack::IsPointer() {
	return pLevel != -1;
}

void Hack::SetValue(void* ValueAddress, int Bytes) {
	memcpy((void*)GetAddress(), ValueAddress, Bytes);
}

void Hack::GetValue(void* DestAddress, int Bytes) {
	ADDRESS Address = GetAddress();

	if (Address)
		memcpy(DestAddress, (void*)Address, Bytes);
}