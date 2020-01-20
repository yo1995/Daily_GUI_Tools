#include "Brightness-Slider.h"
#include "cfgr.h"

using namespace System;
using namespace System::Windows::Forms;

// assign the global variables
libusb_device_handle *handle = NULL;
libusb_device **devs = NULL;
libusb_device *lgdev = NULL;
int r, openCode, iface = 1;
ssize_t cnt;

// global variable array for read parameters from the conf file
std::vector<uint16_t> product_id_enumeration_from_conf;

std::string GetProgramDir() {
	wchar_t exeFullPath[MAX_PATH];  // Full path   
	std::string strPath = "";
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	char CharString[MAX_PATH];
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, CharString, MAX_PATH, exeFullPath, _TRUNCATE);
	strPath = (std::string)CharString;  // Get full path of the file   
	int pos = strPath.find_last_of('\\', strPath.length());
	return strPath.substr(0, pos);  // Return the directory without the file name   
}

void read_prod_id_conf() {

	// open ostream for ./config.ini
	std::string strConfigFileName(GetProgramDir()+"\\LGconfig.ini");

	// std::ofstream out(strConfigFileName);
	// write comment line
	// out << "# test for config read and write\n";
	// out << "product_id = 0x9A40\n";
	// out.close();

	// init Config
	Config config(strConfigFileName);

	// read K-V pairs
	std::string strKey = "product_id";
	std::string strValue;
	strValue = config.Read<std::string>(strKey);
	int myInt(std::stoi(strValue, NULL, 0));
	uint16_t myInt16(0);
	if (myInt <= static_cast<int>(UINT16_MAX) && myInt >= 0) {
		myInt16 = static_cast<uint16_t>(myInt);
	}
	product_id_enumeration.push_back(myInt16);
}

int main() {

	read_prod_id_conf();

	// LOG_FUNC_BEGIN
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	CLRForm::BrightnessSlider slider;
	Application::Run(%slider);
	// printf("we are here after running");
	
	if (handle) {
		libusb_release_interface(handle, iface);
		libusb_attach_kernel_driver(handle, iface);
		libusb_close(handle);
		if (devs != NULL) {
			libusb_free_device_list(devs, 1);
		}
		if (r) {
			libusb_exit(NULL);
		}
	}
	// LOG_FUNC_END
}
