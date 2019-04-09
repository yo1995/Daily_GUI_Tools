#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <conio.h>  // for getchar only. remove later

#include "libusb.h"


// From the HID spec:

static const int HID_GET_REPORT = 0x01;
static const int HID_SET_REPORT = 0x09;
static const int HID_REPORT_TYPE_INPUT = 0x01;
static const int HID_REPORT_TYPE_OUTPUT = 0x02;
static const int HID_REPORT_TYPE_FEATURE = 0x03;

using std::vector;
const uint16_t vendor_id = 0x43e;
const uint16_t product_id = 0x9a40;
const uint16_t max_brightness = 0xd2f0;
const uint16_t min_brightness = 0x0000;

const std::vector<uint16_t> small_steps = {
	0x0000,
	0x0190, 0x01af, 0x01d2, 0x01f7,
	0x021f, 0x024a, 0x0279, 0x02ac,
	0x02e2, 0x031d, 0x035c, 0x03a1,
	0x03eb, 0x043b, 0x0491, 0x04ee,
	0x0553, 0x05c0, 0x0635, 0x06b3,
	0x073c, 0x07d0, 0x086f, 0x091b,
	0x09d5, 0x0a9d, 0x0b76, 0x0c60,
	0x0d5c, 0x0e6c, 0x0f93, 0x10d0,
	0x1227, 0x1399, 0x1529, 0x16d9,
	0x18aa, 0x1aa2, 0x1cc1, 0x1f0b,
	0x2184, 0x2430, 0x2712, 0x2a2e,
	0x2d8b, 0x312b, 0x3516, 0x3951,
	0x3de2, 0x42cf, 0x4822, 0x4de1,
	0x5415, 0x5ac8, 0x6203, 0x69d2,
	0x7240, 0x7b5a, 0x852d, 0x8fc9,
	0x9b3d, 0xa79b, 0xb4f5, 0xc35f,
	0xd2f0 };

const std::vector<uint16_t> big_steps = {
	0x0000,
	0x0190, 0x021f, 0x02e2, 0x03eb,
	0x0553, 0x073c, 0x09d5, 0x0d5c,
	0x1227, 0x18aa, 0x2184, 0x2d8b,
	0x3de2, 0x5415, 0x7240, 0x9b3d,
	0xd2f0 };


uint16_t next_step(uint16_t val, const vector<uint16_t> &steps)
{
	auto start = 0;
	auto end = steps.size() - 1;
	while (start + 1 < end)
	{
		auto mid = start + (end - start) / 2;
		if (steps[mid] > val)
		{
			end = mid;
		}
		else
		{
			start = mid;
		}
	}
	return steps[end];
}

uint16_t prev_step(uint16_t val, const vector<uint16_t> &steps)
{
	auto start = 0;
	auto end = steps.size() - 1;
	while (start + 1 < end)
	{
		auto mid = start + (end - start) / 2;
		if (steps[mid] >= val)
		{
			end = mid;
		}
		else
		{
			start = mid;
		}
	}
	return steps[start];
}

uint16_t get_brightness(libusb_device_handle *handle)
{
	u_char data[8] = { 0x00 };
	// int res = hid_get_feature_report(handle, data, sizeof(data));
	int res = libusb_control_transfer(handle,
		LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
		HID_GET_REPORT, (HID_REPORT_TYPE_FEATURE << 8) | 0, 1, data, sizeof(data), 0);

	if (res < 0)
	{
		printf("Unable to get brightness.\n");
		printf("libusb_control_transfer error: %s (%d)\n", libusb_error_name(res), res);
	}
	else {
		// for (int i = 0; i < sizeof(data); i++) {
		//     printf("0x%x  ", data[i]);
		// }
		// printf("\n");
	}

	uint16_t val = data[0] + (data[1] << 8);
	// printf("val=%d (0x%x 0x%x 0x%x)\n", val, data[0], data[1], data[2]);

	// return int((float(val) / 54000) * 100.0);
	return val;
}

void set_brightness(libusb_device_handle *handle, uint16_t val)
{
	u_char data[6] = {
		u_char(val & 0x00ff),
		u_char((val >> 8) & 0x00ff), 0x00, 0x00, 0x00, 0x00 };
	// int res = hid_send_feature_report(handle, data, sizeof(data));
	int res = libusb_control_transfer(handle,
		LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
		HID_SET_REPORT, (HID_REPORT_TYPE_FEATURE << 8) | 0, 1, data, sizeof(data), 0);

	if (res < 0)
	{
		printf("Unable to set brightness.\n");
		printf("libusb_control_transfer error: %s\n", libusb_error_name(res));
	}
	// else {
	//     get_brightness(handle);
	// }
}

static libusb_device *get_lg_ultrafine_usb_device(libusb_device **devs) {
	libusb_device *dev, *lgdev = NULL;
	int i = 0, j = 0;
	// uint8_t path[8];

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			printf("failed to get device descriptor");
			return NULL;
		}

		if (desc.idVendor == vendor_id && desc.idProduct == product_id) {
			lgdev = dev;
		}

		// r = libusb_get_port_numbers(dev, path, sizeof(path));
		// if (r > 0)
		// {
		//     printf(" path: %d", path[0]);
		//     for (j = 1; j < r; j++)
		//         printf(".%d", path[j]);
		// }
		// printf("\n");
	}

	return lgdev;
}

int brightness_to_percent(int brightness) {
	if (brightness > 54000) return 100;
	if (brightness < 0) return 0;
	return int((float(brightness) / 54000) * 100.0);
}

int percent_to_brightness(int percent) {
	if (percent > 100) return 54000;
	if (percent < 0) return 0;
	return int((float(percent) / 100.0) * 54000);
}


// declare the libusb handle as a global variable
libusb_device_handle *handle = NULL;
libusb_device **devs = NULL;
libusb_device *lgdev = NULL;
int r, openCode, iface = 1;
ssize_t cnt;

namespace CLRForm {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// BrightnessSlider 摘要
	/// </summary>
	public ref class BrightnessSlider : public System::Windows::Forms::Form
	{
	public:

		BrightnessSlider(void)
		{
			InitializeComponent();
			//
			//TODO:  在此处添加构造函数代码
			//
			if (Initializelibusb() == EXIT_FAILURE) {
				this->Text = "Error! Check your connection";
				this->trackBar1->Enabled = false;  // disable the slider
			}
		}

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~BrightnessSlider()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TrackBar^  trackBar1;
	protected:

	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要修改
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void) {
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			this->SuspendLayout();
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(6, 6);
			this->trackBar1->Margin = System::Windows::Forms::Padding(2);
			this->trackBar1->Maximum = 100;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(273, 45);
			this->trackBar1->TabIndex = 0;
			this->trackBar1->TickFrequency = 10;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &BrightnessSlider::trackBar1_Scroll);
			this->trackBar1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &BrightnessSlider::trackBar1_KeyPress);
			// 
			// BrightnessSlider
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(285, 54);
			this->Controls->Add(this->trackBar1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"BrightnessSlider";
			this->Text = L"BrightnessSlider";
			this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &BrightnessSlider::BrightnessSlider_KeyPress);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

		int Initializelibusb(void) {
			r = libusb_init(NULL);
			libusb_set_debug(NULL, LIBUSB_LOG_LEVEL_WARNING);       // LIBUSB_LOG_LEVEL_DEBUG  
			if (r < 0) {
				printf("Unable to initialize libusb.\n");
				return EXIT_FAILURE;
			}

			cnt = libusb_get_device_list(NULL, &devs);
			if (cnt < 0) {
				printf("Unable to get USB device list (%d).\n", int(cnt));
				return EXIT_FAILURE;
			}

			lgdev = get_lg_ultrafine_usb_device(devs);

			if (lgdev == NULL) {
				printf("Failed to get LG screen device.\n");
				return EXIT_FAILURE;
			}

			openCode = libusb_open(lgdev, &handle);  // open and acquire the handle
			if (openCode == 0) {
				libusb_set_auto_detach_kernel_driver(handle, 1);
				// r = libusb_detach_kernel_driver(handle, iface);
				// if (r == LIBUSB_SUCCESS) {
				r = libusb_claim_interface(handle, iface);
				if (r == LIBUSB_SUCCESS) {
					uint16_t brightness = get_brightness(handle);
					if (brightness > 54000) brightness = 54000;  // avoid overflow
					// printf("Original brightness = %d%4s\r", brightness_to_percent(brightness), " ");
					this->Text = "Original brightness = " + brightness_to_percent(brightness).ToString() + "%";
					set_brightness(handle, brightness);	
					this->trackBar1->Value = brightness_to_percent(brightness);

					libusb_free_device_list(devs, 1);
					return EXIT_SUCCESS;
				}
				else {
					printf("Failed to claim interface %d. Error: %d\n", iface, r);
					printf("Error: %s\n", libusb_error_name(r));
					return EXIT_FAILURE;
				}
			}
			else {
				printf("libusb_open failed and returned %d\n", openCode);
				return EXIT_FAILURE;
			}
		}
#pragma endregion
	private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) {
		int percent = this->trackBar1->Value;
		if (percent > 100) percent = 100;
		if (percent < 0) percent = 0;
		set_brightness(handle, percent_to_brightness(percent));
		this->Text = this->trackBar1->Value.ToString() + "%";
	}

	private: System::Void BrightnessSlider_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
		// wrongly added, do nothing here.
	}

	private: System::Void trackBar1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
		int percent = this->trackBar1->Value;
		if (percent > 100) percent = 100;
		if (percent < 0) percent = 0;
		int brightness = percent_to_brightness(percent);

		switch (e->KeyChar) {
		case ']':
			brightness = next_step(brightness, big_steps);
			set_brightness(handle, brightness);
			if (brightness_to_percent(brightness) - percent < 1) {
				this->trackBar1->Value = percent + 1;
			}
			else {
				this->trackBar1->Value = brightness_to_percent(brightness);
			}
			this->Text = this->trackBar1->Value.ToString() + "%";
			break;
		case '[':
			brightness = prev_step(brightness, big_steps);
			set_brightness(handle, brightness);
			this->trackBar1->Value = brightness_to_percent(brightness);;
			this->Text = this->trackBar1->Value.ToString() + "%";
			break;
		default:
			this->Text = L"Press [ or ] to adjust brightness.";
			break;
		}
	}
};
}
