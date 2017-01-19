Overview
========
This example demonstrates the SDK Peripheral drivers working with different methods.

This example demonstrates use of smartcard driver API to read GSM sim card ICC-ID (Integrated circuit card identifier,
which should be placed on the card). 
Please be aware, that this example is just simple demonstration of smartcard driver API. It may not work with all types
of GSM sim cards.


Toolchain supported
===================
- IAR embedded Workbench 7.80.2
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Micro USB cable
- TWR-KL28Z72M board
- TWR-POSCARD board (The compact SMART CARD interface IC is TDA8035)
- GSM sim card
- Personal Computer

Board settings
==============
The Smart card Example project calls for special hardware configurations when using TWR-POSCARD.
TWR-POSCARD board requires jumpers to be set to positions as follows: J6(4-6), J7(4-6), J8(4-6), J9(empty), J10(empty), 
J11(1-2), J12(1-3;4-6).
TWR-ELEV connection required:

EMVSIM_IO:     J26-30(PTC17)  <-->  Elevator(C61)
EMVSIM_CLK:    J26-27(PTC14)  <-->  Elevator(C58)
EMVSIM_RST:    J26-28(PTC15)  <-->  Elevator(C59)
EMVSIM_VCCEN:  J26-29(PTC16)  <-->  Elevator(C25)
EMVSIM_PD:     J26-31(PTC18)  <-->  Elevator(C22)

Vsel0:         Elevator(B23)  <-->  Elevator(B21)
Prepare the Demo
================
Connect a serial cable from the debug UART port of the board to the PC. Start Tera Term
(http://ttssh2.osdn.jp) and make a connection to the virtual serial port.

1. Start Tera Term
2. New connection -> Serial
3. Set appropriate COMx port (x is port number) in Port context menu. Number is provided by operation
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

***** SMARTCARD Driver Send Receive functionality example *****

Card inserted.
Deactivating card...Done!
Resetting/Activating card...Done!
Selecting Master root file.
Getting response of selection command.
Selecting ICC-ID file.
Reading binary ICC-ID.
Received smartcard ICC-IC: 000000000000000000

Send receive functionality example finished!
~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

