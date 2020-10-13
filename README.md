﻿# [CH545](https://github.com/SoCXin/CH545)

[![sites](http://182.61.61.133/link/resources/SoC.png)](http://www.SoC.Xin)

#### [Vendor](https://github.com/SoCXin/Vendor)：[WCH](https://github.com/SoCXin/WCH)
#### [Core](https://github.com/SoCXin/8051)：[E8051](https://github.com/SoCXin/8051)
#### [Level](https://github.com/SoCXin/Level)：12/48MHz

## [CH545简介](https://github.com/SoCXin/CH545/wiki)

[CH545](https://github.com/SoCXin/CH545)是一款兼容MCS51的增强型E8051内核单片机，79%的指令是单字节单周期指令，运行速度快，内置64KB Flash-ROM，8K+256B RAM。
CH545内嵌USB主机控制器和收发器，内置4端口USB root-hub根集线器，支持USB Host主机模式和USB Device 设备模式，支持USB 2.0全速12Mpbs或者低速1.5Mbps；内嵌4通道全速复合USB设备控制器和收发器，每个复合USB设备控制器包括一个device-hub和三个功能子设备。支持最大64字节数据包，内置FIFO，支持DMA。
CH545提供丰富的接口资源，包括2组异步串口、6路PWM、14通道电容触摸按键、1组IIC主机、4组IIC从机、128组RGB三色LED控制器，其他包括2路SPI、14路12位ADC，支持电压比较；内置3组定时器和2路信号捕捉；支持最多58个GPIO。


* 内置60KB Code Flash、1KB Data Flash和3KB BootLoader，支持USB和串口ISP；
* 内置8KB XRAM和内部256B RAM；
* 内嵌USB主机控制器和收发器，内置4端口root-hub根集线器，支持USB 2.0全速和低速主机或设备，支持DMA；
* 内嵌4通道全速复合USB设备控制器和收发器，每个复合设备包括1个device-hub和3个功能子设备，支持DMA；
* 提供支持 FAT12/FAT16/FAT32 文件系统的 U 盘文件级子程序库，实现读写 U 盘文件；
* 提供3组定时器/计数器，支持2路引脚信号捕捉和6路PWM输出；
* 提供2个全双工异步串口；
* 提供2个SPI通信接口，SPI0支持主/从模式，SPI1支持主机模式；
* 提供1组IIC主机和4组IIC从机；
* 提供LED驱动器，支持384只单色LED或128组RGB三色LED；
* 提供与GPIO复用的14通道12位ADC模数转换器；
* 提供14通道电容触摸按键；
* 内置时钟和PLL，也可支持外部晶振；

封装：LQFP48、LQFP64。

### [资源收录](https://github.com/SoCXin/CH545)

* [文档](docs/)
* [资源](src/)

### [选型建议](https://github.com/SoCXin)

[CH545](https://github.com/SoCXin/CH545)

CH544是CH545的简化版，去掉了RGB三色LED的PWM模块，仅提供2通道复合USB设备控制器和3组IIC，其他的与CH545相同，可直接参考CH545手册和资料。

###  [SoC芯平台](http://www.SoC.Xin)
