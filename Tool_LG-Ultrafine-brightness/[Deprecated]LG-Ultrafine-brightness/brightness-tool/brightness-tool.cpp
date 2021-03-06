#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <conio.h>
#include "..\packages\libusb.1.0.21\src\include\libusb-1.0\libusb.h"

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

const std::vector<uint16_t> product_id_enumeration = {
	0x9a40,  // LG Ultrafine 5K 1st gen
	0x9A63  // LG Ultrafine 4k 23.7 2019
};

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

static libusb_device *get_lg_ultrafine_usb_device(libusb_device **devs)
{
	libusb_device *dev, *lgdev = NULL;
	int i = 0, j = 0;
	uint8_t path[8];

	while ((dev = devs[i++]) != NULL)
	{
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0)
		{
			printf("failed to get device descriptor.\n");
			return NULL;
		}

		/*if (desc.idVendor == vendor_id && desc.idProduct == product_id)
		{
			lgdev = dev;
		}*/

		for (size_t i = 0; i < product_id_enumeration.size(); ++i) {
			if (desc.idVendor == vendor_id && desc.idProduct == product_id_enumeration[i])
			{
				lgdev = dev;
				break;
			}
			else {
				printf("Not the correct product_id, try other viable ids.\n");
			}
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

void adjust_brighness(libusb_device_handle *handle)
{
	uint16_t brightness = get_brightness(handle);
	printf("Press '-' or '=' to adjust brightness.\n");
	printf("Press '[' or: ']' to fine tune.\n");
	printf("Press 'p' to use the minimum brightness\n");
	printf("Press '\\' to use the maximum brightness\n");
	printf("Press 'q' to quit.\n");

	bool stop = false;
	while (not stop)
	{
		printf("Current brightness = %d%4s\r", int((float(brightness) / 54000) * 100.0), " ");
		int c = _getch();

		switch (c)
		{
		case '+':
		case '=':
			brightness = next_step(brightness, big_steps);
			set_brightness(handle, brightness);
			break;
		case '-':
		case '_':
			brightness = prev_step(brightness, big_steps);
			set_brightness(handle, brightness);
			break;
		case ']':
			brightness = next_step(brightness, small_steps);
			set_brightness(handle, brightness);
			break;
		case '[':
			brightness = prev_step(brightness, small_steps);
			set_brightness(handle, brightness);
			break;
		case '\\':
			brightness = max_brightness;
			set_brightness(handle, brightness);
			break;
		case 'p':
			brightness = min_brightness;
			set_brightness(handle, brightness);
			break;
		case 'q':
		case '\n':
			printf("You pressed q.\n");
			stop = true;
			break;
		default:
			break;
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc > 2) {
		// print help
		printf("USAGE: lgufb [<brightness>]\n\n");
		printf("EXAMPLE:\n\n");
		printf("  lgufb 80\n");
		printf(" \tSetting display brightness to 80% \n");
		return 1;
	}

	libusb_device **devs, *lgdev;
	int r, openCode, iface = 1;
	ssize_t cnt;
	libusb_device_handle *handle;

	/*initscr();
	noecho();
	cbreak();*/

	r = libusb_init(NULL);
	libusb_set_debug(NULL, LIBUSB_LOG_LEVEL_WARNING);       // LIBUSB_LOG_LEVEL_DEBUG  
	if (r < 0)
	{
		printf("Unable to initialize libusb.\n");
		// endwin();
		return r;
	}

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0)
	{
		printf("Unable to get USB device list (%ld).\n", cnt);
		// endwin();
		return (int)cnt;
	}

	lgdev = get_lg_ultrafine_usb_device(devs);

	if (lgdev == NULL)
	{
		printf("Failed to get LG screen device.\n");
		// endwin();
		return -1;
	}

	openCode = libusb_open(lgdev, &handle);
	if (openCode == 0)
	{
		libusb_set_auto_detach_kernel_driver(handle, 1);
		// r = libusb_detach_kernel_driver(handle, iface);
		// if (r == LIBUSB_SUCCESS) {
		r = libusb_claim_interface(handle, iface);
		if (r == LIBUSB_SUCCESS) {
			if (argc == 1) {
				adjust_brighness(handle);
			}
			else if (argc == 2)
			{
				int percent = std::stoi(argv[1]);
				if (percent > 100) percent = 100;
				int brightness = int((float(percent) / 100.0) * 54000);
				set_brightness(handle, brightness);
			}
			libusb_release_interface(handle, iface);
			libusb_attach_kernel_driver(handle, iface);
		}
		else {
			printf("Failed to claim interface %d. Error: %d\n", iface, r);
			printf("Error: %s\n", libusb_error_name(r));
		}

		// } else {
		//     printw("Failed to detach interface %d. Error: %d\n", iface, r);
		//     printw("Error: %s\n", libusb_error_name(r));
		// }
		libusb_close(handle);
	}
	else
	{
		printf("libusb_open failed and returned %d\n", openCode);
	}

	libusb_free_device_list(devs, 1);

	libusb_exit(NULL);

	// endwin();

	return 0;
}
