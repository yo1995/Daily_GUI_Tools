![logo](./logo.png)

## Update

### 180904

changed something, forgot.

### 190530

update table of version numbers

more to add - a python version

## Description

A simple GUI tool to convert Adobe Premiere Pro CC project file versions.

People always got haunted by newer version of the software, since it produces some project file that the 'even 1 subversion behind' cannot read. PS provides nice solution to it, on contrary Pr Ae and many more do not.

This tool works until Pr reaches version 40. Just need to manually assign if later versions are released or wait for my update.

## Chart

| Product Name | Version |
| ------ | ------ |
| Pr CC 2019.4 | 37 |
| Pr CC 2019   | 36 |
| Pr CC 2018   | 35 |
| Pr CC 2017.4 | 34 |
| Pr CC 2017.1 | 33 |
| Pr CC 2017   | 32 |
| Pr CC 2015.5 | 31 |
| Pr CC 2015.2 | 30 |

## Versions

### 20180721

- added first version. works fine with mine.

- works with 35->33 conversion between my CC2017 Win7 desktop renderer and CC2018 Win10 laptop.

## Usage

0. [Microsoft .NET Framework 4.5](https://www.microsoft.com/en-us/download/details.aspx?id=30653) or later to run

1. 打开文件

2. 选择转换到的代数 或自定义数值 最小为0即可

3. 转换按钮并选择文件保存位置，默认为同一路径

## Keywords

Adobe Premiere Pro CC | .prproj | prproj | Pr project file| Premiere Downgrader | PremiereDowngrader | tool | easy | C#

## 具体实现

### 文件读取

点击按钮，打开文件dialog，正确打开则读取文件名。

### 版本选择

combobox下拉列表选择对应键值，switch-case对应目标版本号。

### .prproj文件处理

GZipStream类实现gzip压缩/解压缩；解压缩后使用XML类操作K-V pairs。改变project->attribute("version")即可。

## Refs

[Path Class (System.IO)](https://msdn.microsoft.com/en-us/library/system.io.path(v=vs.110).aspx

[XmlDocument Class](https://msdn.microsoft.com/en-us/library/system.xml.xmldocument%28v=vs.110%29.aspx?f=255&MSPPError=-2147217396)

[PR工程版本转换攻略](http://www.gfxtr1.com/Tutorials/Magazines/2017-09-25/6457.html)

[C#压缩与解压缩流类 GZipStream](https://www.cnblogs.com/csMapx/archive/2012/06/04/2534396.html)
