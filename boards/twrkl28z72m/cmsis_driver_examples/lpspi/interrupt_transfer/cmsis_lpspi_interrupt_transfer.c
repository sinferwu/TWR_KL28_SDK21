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
*/

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi.h"
#include "board.h"
#include "fsl_lpspi_cmsis.h"

#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
#include "fsl_intmux.h"
#endif

#include "fsl_common.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*Master related*/
#define EXAMPLE_LPSPI_MASTER_IRQN (LPSPI0_IRQn)
#define EXAMPLE_LPSPI_MASTER_CLOCK_NAME (kCLOCK_Lpspi0)
#define EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)
/*Slave related*/
#define EXAMPLE_LPSPI_SLAVE_IRQN (LPSPI2_IRQn)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_NAME (kCLOCK_Lpspi2)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)
/*INTMUX setting*/
#define EXAMPLE_LPSPI_INTMUX_CHANNLE (0U)
/* Driver mapping */
#define DRIVER_MASTER_SPI Driver_SPI0
#define DRIVER_SLAVE_SPI Driver_SPI2
#define TRANSFER_SIZE (512U)        /*! Transfer dataSize */
#define TRANSFER_BAUDRATE (500000U) /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* LPSPI user SignalEvent */
void LPSPI_MasterSignalEvent_t(uint32_t event);
void LPSPI_SlaveSignalEvent_t(uint32_t event);
/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t masterRxData[TRANSFER_SIZE] = {0U};
uint8_t masterTxData[TRANSFER_SIZE] = {0U};
uint8_t slaveRxData[TRANSFER_SIZE] = {0U};
uint8_t slaveTxData[TRANSFER_SIZE] = {0U};

bool isMasterIrqInIntmux = false;
bool isSlaveIrqInIntmux = false;
volatile bool isTransferCompleted = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t LPSPI0_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpspi0);
}
uint32_t LPSPI1_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpspi1);
}
uint32_t LPSPI2_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Lpspi2);
}
void LPSPI_MasterSignalEvent_t(uint32_t event)
{
    /* user code */
}
void LPSPI_SlaveSignalEvent_t(uint32_t event)
{
    /* user code */
    isTransferCompleted = true;
    PRINTF("This is LPSPI_SlaveSignalEvent_t\r\n");
}
/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t errorCount;
    uint32_t i;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Set clock source for LPSPI and get master clock source*/
    CLOCK_SetIpSrc(EXAMPLE_LPSPI_MASTER_CLOCK_NAME, EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE);
    CLOCK_SetIpSrc(EXAMPLE_LPSPI_SLAVE_CLOCK_NAME, EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE);

    PRINTF("LPSPI CMSIS interrupt example start.\r\n");
    PRINTF("This example use one lpspi instance as master and another as slave on one board.\r\n");
    PRINTF("Master uses interrupt way and slave uses interrupt way.\r\n");
    PRINTF(
        "Note that some LPSPI instances interrupt is in INTMUX ,"
        "you should set the intmux when you porting this example accordingly \r\n");

    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("LPSPI_master -- LPSPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS \r\n");
    PRINTF("   SOUT     --    SIN  \r\n");
    PRINTF("   SIN      --    SOUT \r\n");

#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
    INTMUX_Init(INTMUX0);
    INTMUX_SetChannelMode(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNLE, kINTMUX_ChannelLogicOR);

    if (EXAMPLE_LPSPI_SLAVE_IRQN > FSL_FEATURE_INTERRUPT_IRQ_MAX)
    {
        isSlaveIrqInIntmux = true;
        INTMUX_EnableInterrupt(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNLE, EXAMPLE_LPSPI_SLAVE_IRQN);
        NVIC_SetPriority(INTMUX0_0_IRQn, 0);
    }

    if (EXAMPLE_LPSPI_MASTER_IRQN > FSL_FEATURE_INTERRUPT_IRQ_MAX)
    {
        isMasterIrqInIntmux = true;
        INTMUX_EnableInterrupt(INTMUX0, EXAMPLE_LPSPI_INTMUX_CHANNLE, EXAMPLE_LPSPI_MASTER_IRQN);
        NVIC_SetPriority(INTMUX0_0_IRQn, 1);
    }
#endif

    if (!isMasterIrqInIntmux)
    {
        NVIC_SetPriority(EXAMPLE_LPSPI_MASTER_IRQN, 1);
    }

    if (!isSlaveIrqInIntmux)
    {
        NVIC_SetPriority(EXAMPLE_LPSPI_SLAVE_IRQN, 0);
    }

    /*LPSPI master init*/
    DRIVER_MASTER_SPI.Initialize(LPSPI_MasterSignalEvent_t);
    DRIVER_MASTER_SPI.PowerControl(ARM_POWER_FULL);
    DRIVER_MASTER_SPI.Control(ARM_SPI_MODE_MASTER, TRANSFER_BAUDRATE);

    /*LPSPI slave init*/
    DRIVER_SLAVE_SPI.Initialize(LPSPI_SlaveSignalEvent_t);
    DRIVER_SLAVE_SPI.PowerControl(ARM_POWER_FULL);
    DRIVER_SLAVE_SPI.Control(ARM_SPI_MODE_SLAVE, false);

    /* Set up the transfer data */
    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        masterTxData[i] = i % 512U;
        masterRxData[i] = 0U;

        slaveTxData[i] = ~masterTxData[i];
        slaveRxData[i] = 0U;
    }

    isTransferCompleted = false;
    /* Start slave transfer */
    DRIVER_SLAVE_SPI.Transfer(slaveTxData, slaveRxData, TRANSFER_SIZE);

    /* Start master transfer */
    DRIVER_MASTER_SPI.Transfer(masterTxData, masterRxData, TRANSFER_SIZE);

    /* Wait slave received all data. */
    while (!isTransferCompleted)
    {
    }

    errorCount = 0U;
    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        if (masterTxData[i] != slaveRxData[i])
        {
            errorCount++;
        }

        if (slaveTxData[i] != masterRxData[i])
        {
            errorCount++;
        }
    }
    if (errorCount == 0U)
    {
        PRINTF(" \r\nLPSPI transfer all data matched! \r\n");
    }
    else
    {
        PRINTF(" \r\nError occured in LPSPI transfer ! \r\n");
    }

    DRIVER_MASTER_SPI.PowerControl(ARM_POWER_OFF);
    DRIVER_SLAVE_SPI.PowerControl(ARM_POWER_OFF);
    DRIVER_MASTER_SPI.Uninitialize();
    DRIVER_SLAVE_SPI.Uninitialize();

    while (1)
    {
    }
}
