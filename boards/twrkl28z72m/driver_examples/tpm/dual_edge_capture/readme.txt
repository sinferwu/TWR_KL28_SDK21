Overview
========
The TPM project is a demonstration program of the SDK TPM driver's dual-edge capture feature.
This feature is available only on certain SoC's.
The example sets up a TPM channel-pair for dual-edge capture. Once the input signal is received,
this example will print the capture values and period of the input signal on the terminal window.

Toolchain supported
===================
- IAR embedded Workbench 7.80.2
- Keil MDK 5.21a
- GCC ARM Embedded 2016-5-q3
- Kinetis Development Studio IDE 3.2.0
- MCUXpresso0.8

Hardware requirements
=====================
- Mini/micro USB cable
- TWR-KL28Z72M board
- Personal Computer
- Primary Elevator board

Board settings
==============
- Plug the board to the Primary Elevator board.
- Connect input signal to A80-Elevator

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, you can see information from the terminal as below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TPM dual-edge capture example

Once the input signal is received the input capture values are printed

The input signal's pulse width is calculated from the capture values & printed

Capture value C(n)V=0

Capture value C(n+1)V=26d4

Input signals pulse width=82 us
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Customization options
=====================

