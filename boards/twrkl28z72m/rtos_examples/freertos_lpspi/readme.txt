Overview
========
The lpspi_interrupt example shows how to use LPSPI driver in interrupt way:

In this example , one lpspi instance used as LPSPI master and another lpspi instance used as LPSPI slave .

1. LPSPI master send/received data to/from LPSPI slave in interrupt . (LPSPI Slave using interrupt to receive/send the data)



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
- Primary Elevator
- Personal Computer

Board settings
==============
To make this example work, connections needed to be as follows:
          LPSPI0(Master)         connected to               LPSPI2(Slave)
PCSO     PTD0(Elevator-A80)       -->           PCSO      PTB20(Elevator-B9)
SCK      PTD1(Elevator-A79)       -->           SCK       PTB21(Elevator-B7)
SOUT     PTD2(Elevator-A78)       -->           SIN       PTB23(Elevator-B11)
SIN      PTD3(Elevator-A77)       -->           SOUT      PTB22(Elevator-B10)

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
When the example runs successfully, you can see the similar information from the terminal as below.

FreeRTOS LPSPI example start.
This example use one lpspi instance as master and another as slave on a single board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
LPSPI_master -- LPSPI_slave
    CLK      --    CLK
    PCS      --    PCS
    SOUT     --    SIN
    SIN      --    SOUT

LPSPI master transfer completed successfully.
Slave-to-master data verified ok.
LPSPI slave transfer completed successfully.
Master-to-slave data verified ok.
Customization options
=====================

