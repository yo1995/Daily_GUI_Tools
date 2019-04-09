
## Download

For commandline version, use the directory without GUI in its path.

**For GUI version, download 2 files in [this folder](/Tool_LG-Ultrafine-brightness/CLR_GUI/Released) and launch them.**

## Description

This is a working command line tool for adjusting brightness for LG UltraFine series monitors. Works fine with my LG UltraFine 5K - 27MD5K monitor on Windows 10 1803.

A friendly fork of 

- [unknownzerx/lguf-brightness](https://github.com/unknownzerx/lguf-brightness/)

	the obsolete LG UltraFine brightness adjustment tool.

- [csujedihy/LG-Ultrafine-Brightness](https://github.com/csujedihy/LG-Ultrafine-Brightness)
	
	the friendly fork of the above with a not working@20190407 `hidapi` [lib](https://github.com/signal11/hidapi).

- [velum/lguf-brightness](https://github.com/velum/lguf-brightness/)

	the working version for Linux users.

## Versions

### 20190407

- added first version. works fine with mine.

	manually imported the [libusb](https://github.com/libusb/libusb) library even with nuget package manager installed... need to figure out the correct way of using it.

### 20190408

- added the visual c++ GUI version with a super simple design. 

	can be run as a standalone executable.

## Known issues

both solutions are not warning free. something to do with libusb itself as well as casting.

## Usage

### Release

Download from `Released` [folder](/Tool_LG-Ultrafine-brightness/LG-Ultrafine-brightness/Released/) for the newest version.

For GUI version, download 2 files in from `Released` in [this folder](/Tool_LG-Ultrafine-brightness/CLR_GUI/Released) and launch them.

### Borrowed from @csujedihy 

---

A tool to adjust brightness of LG Ultrafine 4k/5K on Windows without the help of Bootcamp. I built my own PC and made this tool to adjust brightness through my PC.

The program simply send set feature report to the device via USB and then the LG 4/5K monitor adjust the brightness upon receiving such reports.

This project is derived from [https://github.com/unknownzerx/lguf-brightness](https://github.com/unknownzerx/lguf-brightness). 

Thanks @unknownzerx 

The original tool was unable to adjust brightness in Windows 10 and I also made some minor improvements to it like showing current brightness and efficiency while adjusting.

### Linux

Please go to this repo [https://github.com/velum/lguf-brightness](https://github.com/velum/lguf-brightness).

### Usage

```
Press '-' or '=' to adjust brightness.
Press '[' or: ']' to fine tune.
Press 'p' to use the minimum brightness
Press '\' to use the maximum brightness
Press 'q' to quit.
```

### Build

1. Open the `sln` file by Visual Studio.
2. Select `Release` -> `x64` configuration.
3. Build

~~Note that I put the hidapi.dll in the project and if you don't believe it's secure, you can build a new one (x64) and replace it.~~

I had the 1.0.21 version of libusb with nuget. you can always manually build it.

## Keywords

easy | tool | LG UltraFine | lg-ultrafine | brightness
