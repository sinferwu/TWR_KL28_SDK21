/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Copyright (c) 2016, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductors, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
PinsProfile:
- !!product 'Pins v2.0'
- !!processor 'MKL28Z512xxx7'
- !!package 'MKL28Z512VLL7'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.0.0'
- !!board 'TWR-KL28Z72M'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {prefix: BOARD_, coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '35', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH2/PTA1/LPUART0_RX/TPM2_CH0}
  - {pin_num: '36', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/TPM2_CH1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Clock Gate Control: 0x01u */

  PORT_SetPinMux(PORTA, PIN1_IDX, kPORT_MuxAlt2);            /* PORTA1 (pin 35) is configured as LPUART0_RX */
  PORT_SetPinMux(PORTA, PIN2_IDX, kPORT_MuxAlt2);            /* PORTA2 (pin 36) is configured as LPUART0_TX */
}


#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPSPI0_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '93', peripheral: LPSPI0, signal: PCS0, pin_signal: PTD0/LLWU_P12/LPSPI0_PCS0/LPUART2_RTS_b/TPM0_CH0/FXIO0_D0}
  - {pin_num: '94', peripheral: LPSPI0, signal: SCK, pin_signal: ADC0_SE5b/PTD1/LPSPI0_SCK/LPUART2_CTS_b/TPM0_CH1/FXIO0_D1}
  - {pin_num: '95', peripheral: LPSPI0, signal: SOUT, pin_signal: PTD2/LLWU_P13/LPSPI0_SOUT/LPUART2_RX/TPM0_CH2/FXIO0_D2}
  - {pin_num: '96', peripheral: LPSPI0, signal: SIN, pin_signal: PTD3/LPSPI0_SIN/LPUART2_TX/TPM0_CH3/FXIO0_D3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPSPI0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPSPI0_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Clock Gate Control: 0x01u */

  PORT_SetPinMux(PORTD, PIN0_IDX, kPORT_MuxAlt2);            /* PORTD0 (pin 93) is configured as LPSPI0_PCS0 */
  PORT_SetPinMux(PORTD, PIN1_IDX, kPORT_MuxAlt2);            /* PORTD1 (pin 94) is configured as LPSPI0_SCK */
  PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAlt2);            /* PORTD2 (pin 95) is configured as LPSPI0_SOUT */
  PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAlt2);            /* PORTD3 (pin 96) is configured as LPSPI0_SIN */
}


#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPSPI0_DeinitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '93', peripheral: n/a, signal: disabled, pin_signal: PTD0/LLWU_P12/LPSPI0_PCS0/LPUART2_RTS_b/TPM0_CH0/FXIO0_D0}
  - {pin_num: '95', peripheral: n/a, signal: disabled, pin_signal: PTD2/LLWU_P13/LPSPI0_SOUT/LPUART2_RX/TPM0_CH2/FXIO0_D2}
  - {pin_num: '96', peripheral: n/a, signal: disabled, pin_signal: PTD3/LPSPI0_SIN/LPUART2_TX/TPM0_CH3/FXIO0_D3}
  - {pin_num: '94', peripheral: ADC0, signal: 'SE, 5b', pin_signal: ADC0_SE5b/PTD1/LPSPI0_SCK/LPUART2_CTS_b/TPM0_CH1/FXIO0_D1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPSPI0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPSPI0_DeinitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Clock Gate Control: 0x01u */

  PORT_SetPinMux(PORTD, PIN0_IDX, kPORT_PinDisabledOrAnalog); /* PORTD0 (pin 93) is disabled */
  PORT_SetPinMux(PORTD, PIN1_IDX, kPORT_PinDisabledOrAnalog); /* PORTD1 (pin 94) is configured as ADC0_SE5b */
  PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_PinDisabledOrAnalog); /* PORTD2 (pin 95) is disabled */
  PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_PinDisabledOrAnalog); /* PORTD3 (pin 96) is disabled */
}


#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPSPI1_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '97', peripheral: LPSPI1, signal: PCS0, pin_signal: PTD4/LLWU_P14/LPSPI1_PCS0/LPUART2_RX/TPM0_CH4/LPUART0_RTS_b/FXIO0_D4}
  - {pin_num: '98', peripheral: LPSPI1, signal: SCK, pin_signal: ADC0_SE6b/PTD5/LPSPI1_SCK/LPUART2_TX/TPM0_CH5/LPUART0_CTS_b/FXIO0_D5}
  - {pin_num: '99', peripheral: LPSPI1, signal: SOUT, pin_signal: ADC0_SE7b/PTD6/LLWU_P15/LPSPI1_SOUT/LPUART0_RX/FXIO0_D6}
  - {pin_num: '100', peripheral: LPSPI1, signal: SIN, pin_signal: PTD7/LPSPI1_SIN/LPUART0_TX/FXIO0_D7}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPSPI1_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPSPI1_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Clock Gate Control: 0x01u */

  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_MuxAlt2);            /* PORTD4 (pin 97) is configured as LPSPI1_PCS0 */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAlt2);            /* PORTD5 (pin 98) is configured as LPSPI1_SCK */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_MuxAlt2);            /* PORTD6 (pin 99) is configured as LPSPI1_SOUT */
  PORT_SetPinMux(PORTD, PIN7_IDX, kPORT_MuxAlt2);            /* PORTD7 (pin 100) is configured as LPSPI1_SIN */
}


#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPSPI1_DeinitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '97', peripheral: n/a, signal: disabled, pin_signal: PTD4/LLWU_P14/LPSPI1_PCS0/LPUART2_RX/TPM0_CH4/LPUART0_RTS_b/FXIO0_D4}
  - {pin_num: '100', peripheral: n/a, signal: disabled, pin_signal: PTD7/LPSPI1_SIN/LPUART0_TX/FXIO0_D7}
  - {pin_num: '98', peripheral: ADC0, signal: 'SE, 6b', pin_signal: ADC0_SE6b/PTD5/LPSPI1_SCK/LPUART2_TX/TPM0_CH5/LPUART0_CTS_b/FXIO0_D5}
  - {pin_num: '99', peripheral: ADC0, signal: 'SE, 7b', pin_signal: ADC0_SE7b/PTD6/LLWU_P15/LPSPI1_SOUT/LPUART0_RX/FXIO0_D6}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPSPI1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPSPI1_DeinitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Clock Gate Control: 0x01u */

  PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTD4 (pin 97) is disabled */
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_PinDisabledOrAnalog); /* PORTD5 (pin 98) is configured as ADC0_SE6b */
  PORT_SetPinMux(PORTD, PIN6_IDX, kPORT_PinDisabledOrAnalog); /* PORTD6 (pin 99) is configured as ADC0_SE7b */
  PORT_SetPinMux(PORTD, PIN7_IDX, kPORT_PinDisabledOrAnalog); /* PORTD7 (pin 100) is disabled */
}


#define PIN20_IDX                       20u   /*!< Pin number for pin 20 in a port */
#define PIN21_IDX                       21u   /*!< Pin number for pin 21 in a port */
#define PIN22_IDX                       22u   /*!< Pin number for pin 22 in a port */
#define PIN23_IDX                       23u   /*!< Pin number for pin 23 in a port */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPSPI2_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '66', peripheral: LPSPI2, signal: PCS0, pin_signal: PTB20/LPSPI2_PCS0/CMP0_OUT}
  - {pin_num: '67', peripheral: LPSPI2, signal: SCK, pin_signal: PTB21/LPSPI2_SCK/CMP1_OUT}
  - {pin_num: '68', peripheral: LPSPI2, signal: SOUT, pin_signal: PTB22/LPSPI2_SOUT}
  - {pin_num: '69', peripheral: LPSPI2, signal: SIN, pin_signal: PTB23/LPSPI2_SIN}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPSPI2_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPSPI2_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Clock Gate Control: 0x01u */

  PORT_SetPinMux(PORTB, PIN20_IDX, kPORT_MuxAlt2);           /* PORTB20 (pin 66) is configured as LPSPI2_PCS0 */
  PORT_SetPinMux(PORTB, PIN21_IDX, kPORT_MuxAlt2);           /* PORTB21 (pin 67) is configured as LPSPI2_SCK */
  PORT_SetPinMux(PORTB, PIN22_IDX, kPORT_MuxAlt2);           /* PORTB22 (pin 68) is configured as LPSPI2_SOUT */
  PORT_SetPinMux(PORTB, PIN23_IDX, kPORT_MuxAlt2);           /* PORTB23 (pin 69) is configured as LPSPI2_SIN */
}


#define PIN20_IDX                       20u   /*!< Pin number for pin 20 in a port */
#define PIN21_IDX                       21u   /*!< Pin number for pin 21 in a port */
#define PIN22_IDX                       22u   /*!< Pin number for pin 22 in a port */
#define PIN23_IDX                       23u   /*!< Pin number for pin 23 in a port */
/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
LPSPI2_DeinitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '66', peripheral: n/a, signal: disabled, pin_signal: PTB20/LPSPI2_PCS0/CMP0_OUT}
  - {pin_num: '67', peripheral: n/a, signal: disabled, pin_signal: PTB21/LPSPI2_SCK/CMP1_OUT}
  - {pin_num: '68', peripheral: n/a, signal: disabled, pin_signal: PTB22/LPSPI2_SOUT}
  - {pin_num: '69', peripheral: n/a, signal: disabled, pin_signal: PTB23/LPSPI2_SIN}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPSPI2_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void LPSPI2_DeinitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Clock Gate Control: 0x01u */

  PORT_SetPinMux(PORTB, PIN20_IDX, kPORT_PinDisabledOrAnalog); /* PORTB20 (pin 66) is disabled */
  PORT_SetPinMux(PORTB, PIN21_IDX, kPORT_PinDisabledOrAnalog); /* PORTB21 (pin 67) is disabled */
  PORT_SetPinMux(PORTB, PIN22_IDX, kPORT_PinDisabledOrAnalog); /* PORTB22 (pin 68) is disabled */
  PORT_SetPinMux(PORTB, PIN23_IDX, kPORT_PinDisabledOrAnalog); /* PORTB23 (pin 69) is disabled */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
