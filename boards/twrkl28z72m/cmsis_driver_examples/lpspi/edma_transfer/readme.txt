Overview
========
CMSIS-Driver defines generic peripheral driver interfaces for middleware making it reusable across a wide 
range of supported microcontroller devices. The API connects microcontroller peripherals with middleware 
that implements for example communication stacks, file systems, or graphic user interfaces. 
More information and usage methord please refer to http://www.keil.com/pack/doc/cmsis/Driver/html/index.html.

The cmsis_lpspi_edma_transfer example shows how to use CMSIS LPSPI driver in edma way:

In this example , one lpspi instance used as LPSPI master and another lpspi instance used as LPSPI slave in the same board.

LPSPI master send/received data to/from LPSPI slave in edma . (LPSPI Slave using edma to receive/send the data)


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
- Primary Elevator

Board settings
==============
To make the example work, connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
       MASTER           connect to      SLAVE
Pin Name   Board Location     Pin Name  Board Location
SOUT       Elevator(A78)        SIN       Elevator(B11)
SIN        Elevator(A77)        SOUT      Elevator(B10)
SCK        Elevator(A79)        SCK       Elevator(B7)
PCS0       Elevator(A80)        PCS0      Elevator(B9)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a mini USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for OpenSDA serial device with these settings:
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

CMSIS LPSPI interrupt transfer example start.
This example use one lpspi instance as master and another as slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is:
LPSPI_master -- LPSPI_slave
   CLK       --    CLK
   PCS       --    PCS
   SOUT      --    SIN
   SIN       --    SOUT
This is SlaveSignalEvent.

LPSPI transfer all data matched!

Customization options
=====================

