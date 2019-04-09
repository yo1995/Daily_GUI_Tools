#include "Brightness-Slider.h"

using namespace System;
using namespace System::Windows::Forms;
// [STAThread]

int main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	CLRForm::BrightnessSlider slider;
	Application::Run(%slider);


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
	
}
