Overview
========
The True Random Number Generator (TRNG) is a hardware accelerator module that generates a 512-bit
entropy as needed by an entropy consuming module or by other post processing functions. The TRNG
Example project is a demonstration program that uses the KSDK software to generate random numbers
and prints them to the terminal.


Toolchain supported
===================
- IAR embedded Workbench 7.80.2
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini USB cable
- TWR-KL28Z72M board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.

Prepare the Demo
================
Connect a serial cable from the debug UART port of the board to the PC. Start Tera Term
(http://ttssh2.osdn.jp) and make a connection to the virtual serial port.

1. Start Tera Term
2. New connection -> Serial
3. Set apropriate COMx port (x is port number) in Port context menu. Number is provided by operation
   system and could be different from computer to computer. Select COM number related to virtual
   serial port. Confirm selected port by OK button.
4. Set following connection parameters in menu Setup->Serial port...
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: one
5.  Confirm selected parameters by OK button.
Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~
TRNG Peripheral Driver Example
Generate 10 random numbers:
Random[0] = 0xF84D63F2
Random[1] = 0xC480F1AC
Random[2] = 0x554277AE
Random[3] = 0x13EAFA4E
Random[4] = 0x3FDF87B0
Random[5] = 0x502E3E7A
Random[6] = 0x6B692AAE
Random[7] = 0x8906DB6C
Random[8] = 0x8BC1AC01
Random[9] = 0xD9C74853

 Press any key to continue...
~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

