![logo](/Tool_LG-Ultrafine-brightness/brightness.png)

## Download

~~**For commandline version, use the directory without GUI in its path.**~~  
the command line version is not actively maintained. you can always try it with a latest version libusb .dll.

**For GUI version, download 2 files in [this folder](/Tool_LG-Ultrafine-brightness/CLR_GUI/Released) and launch them.**

For BootCamp users, Boot Camp Support Software 6.1.10 released recently should mitigate this problem. Please update accordingly via Apple Software Update.

## Description

This is a working command line tool for adjusting brightness for LG UltraFine series monitors.  
Works fine with my LG UltraFine 5K - 27MD5K monitor on Windows 10 1803.  
Also reportedly work with LG UltraFine 4K - 24MD4KL 23.7 2019, per #5.  
**x64 only.**

A friendly fork of 

- [unknownzerx/lguf-brightness](https://github.com/unknownzerx/lguf-brightness/)

	the obsolete LG UltraFine brightness adjustment tool.

- [csujedihy/LG-Ultrafine-Brightness](https://github.com/csujedihy/LG-Ultrafine-Brightness)
	
	the friendly fork of the above with a not working@20190407 `hidapi` [lib](https://github.com/signal11/hidapi).

- [velum/lguf-brightness](https://github.com/velum/lguf-brightness/)

	the working version for Linux users.

Notice: both tools compiles with `Release x64` settings. Other schemes need to be brushed later.

## Versions

### 20200119

- Added new device id (idProduct) for 27MD5KL-B and fixed #7
	
	so it supports not only original 5K(27MD5K) but also latest 4K and latest 5K. will consider extracting all the conf into a separate JSON file in the future. (I don't like to deal with C++ btw...) https://github.com/nothings/single_file_libs#json

### 20190719

- Added logging, also fixed #5
	
	so it supports not only 5K but also latest 4K

### 20190529

- upgraded libusb version to v1.0.22

	to mitigate issue #3: Per report in #3, the latest version might not work with Windows 10 1903 yet.

### 20190408

- added the visual c++ GUI version with a super simple design. 

	can be run as a standalone executable.

### 20190407

- added first version. works fine with mine.

	manually imported the [libusb](https://github.com/libusb/libusb) library even with nuget package manager installed... need to figure out the correct way of using it.

## Known issues

- both solutions are not warning free. something to do with libusb itself as well as casting.

- on Windows, the brightness steps are not identical with the OSX steps.

- when adjusting with `[` or `]`, the increasing steps are not smooth due to some logistic issue. check the source code to understand.

## Usage

### Release

Download from `Released` [folder](/Tool_LG-Ultrafine-brightness/LG-Ultrafine-brightness/Released/) for the newest version.

For GUI version, download 2 files in from `Released` in [this folder](/Tool_LG-Ultrafine-brightness/CLR_GUI/Released) and launch them.

### Brightness Steps

refer to [this issue](https://github.com/unknownzerx/lguf-brightness/issues/10)

compile parameters are in the src.

---

### Borrowed from @csujedihy 

A tool to adjust brightness of LG Ultrafine 4k/5K on Windows without the help of Bootcamp. I built my own PC and made this tool to adjust brightness through my PC.

The program simply send set feature report to the device via USB and then the LG 4/5K monitor adjust the brightness upon receiving such reports.

The original tool was unable to adjust brightness in Windows 10 and I also made some minor improvements to it like showing current brightness and efficiency while adjusting.

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

~~I had the 1.0.21 version of libusb with nuget. you can always manually build it.~~  
update 190529: v1.0.22. please update the files in 

## Potential updates

- Might change it into a standalone single file executable by linking the static library of libusb, but currently just decide to left out the .dll so that people can use their trusted version of libusb.

- better UI design incorporated with system UI.

## Keywords

easy | tool | LG UltraFine | lg-ultrafine | brightness | 4K | 5K 
