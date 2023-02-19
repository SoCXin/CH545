
南京沁恒微电子有限公司 2021.04
最新资料请到我们的网站上查询
http://wch.cn
http://winchiphead.com


文件目录EVT
  |
  |-- README.TXT：目录文件说明
  | 
  |-- EXAM: C例程
  |     |--CH545.uvproj:CH545 keil4工程文件
  |     |--Public：
  |     |      |--CH545.H：CH545 C语言头文件，SFR定义
  |     |      |--CH545.INC：CH545汇编头文件，SFR定义
  |     |      |--Debug.C&H：UART0&1初始化、常用延时函数、软复位函数、UART0&1数据收发子函数,看门狗初始化
  |     |--ADC：
  |     |      |-- MAIN.C：ADC使用示例
  |     |      |-- ADC.C&H：ADC初始化，采样时钟设定，通道变化和电压比较功能函数定义
  |     |      |-- ADC.hex
  |     |--DATAFLASH：
  |     |      |-- MAIN.C：DataFlash使用示例
  |     |      |-- Flash.C&H：Flash读写函数定义
  |     |      |-- DataFlash.hex
  |     |--GPIO：
  |     |      |-- MAIN.C：GPIO使用示例
  |     |      |-- GPIO.C&H：GPIO设置函数定义
  |     |      |-- GPIO.hex
  |     |--I2C：
  |     |      |-- MAIN.C：I2C使用示例
  |     |      |-- I2C.C&H：GPIO设置函数定义
  |     |      |-- I2C.hex
  |     |--RGB：
  |     |      |-- MAIN.C：RGB使用示例
  |     |      |-- RGB.C&H：RGB初始化函数定义
  |     |      |-- RGBLOAD.ASM：RGB中断服务函数 
  |     |      |-- RGB.hex   
  |     |--SPI：
  |     |      |-- SPI_MASTER：  
  |     |      |      |-- MAINMASTER.C：SPI主模式使用示例
  |     |      |      |-- MAINMASTER.hex
  |     |      |-- SPI_SLAVE：  
  |     |      |      |-- MAINSLAVE.C：SPI从模式使用示例
  |     |      |      |-- MAINSLAVE.hex
  |     |      |-- SPI.C&H：SPI主从设置和收发函数定义  
  |     |--TIMER：
  |     |      |-- MAIN.C：Timer0&1&2定时，计数设置和捕捉功能使用示例
  |     |      |-- TIMER.C&H：Timer0&1&2定时，计数设置和捕捉函数定义
  |     |      |-- TIMER.hex  
  |     |--TOUCHKEY：
  |     |      |-- MAIN.C：Touchkey功能使用示例
  |     |      |-- TOUCHKEY.C&H：TOUCHKEY初始化和滤波函数定义
  |     |      |-- TOUCHKEY.hex 
  |     |--UART：
  |     |      |-- MAIN.C：串口查询、中断演示  
  |     |      |-- UART.C&H：串口收发函数与初始化
  |     |      |-- UART.hex 
  |     |--USB：
  |     |      |-- Device：
  |     |      |      |-- CompatibilityHID.C：模拟HID兼容设备，支持中断上下传
  |     |      |      |-- CompatibilityHID.hex    
  |     |      |      |-- CompositeKM.C：模拟键鼠例程，支持部分类命令
  |     |      |      |-- CompositeKM.hex 
  |     |      |      |-- VendorDefinedDev.C：模拟厂商自定义设备，需要安装CH372驱动，可以通过372test.exe调试(批量数据收发)
  |     |      |      |-- VendorDefinedDev.hex
  |     |      |-- Host：
  |     |      |      |-- HOST.C：USB主机应用例子,初始化和枚举USB端口连接的设备,支持一级外部HUB,可以操作USB键鼠和HUB，打印机,包含HID类命令处理
  |     |      |      |-- HOST.hex:      
  |     |      |-- U_DISK：
  |     |      |      |-- EXAM1.C：C示例源程序,以字节为单位读写文件，包括文件创建、删除、修改文件属性,修改文件名
  |     |      |      |-- EXAM1.hex
  |     |      |      |-- EXAM11.C：C示例源程序,枚举根目录或者指定目下的文件
  |     |      |      |-- EXAM11.hex：
  |     |      |-- USB_LIB：
  |     |      |      |-- CH545UFI.C&H：CH545 U盘读写文件系统库常用接口函数
  |     |      |      |-- CH545UFI.LIB：CH545 U盘文件系统库文件(编译器内存模式设置成:small)
  |     |      |      |-- USBHOST.C&H：CH545 USB主机控制传输函数定义 
  |- PUB：评估板说明
  |     |-- CH545EVT.PDF：评估板和EVT资料包说明
  |     |-- CH545SCH.PDF：评估板原理图