#include "Brightness-Slider.h"

using namespace System;
using namespace System::Windows::Forms;

// assign the global variables
libusb_device_handle *handle = NULL;
libusb_device **devs = NULL;
libusb_device *lgdev = NULL;
int r, openCode, iface = 1;
ssize_t cnt;

int main() {
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
