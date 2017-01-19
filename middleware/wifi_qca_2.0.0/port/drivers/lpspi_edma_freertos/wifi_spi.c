/*
 * Copyright (c) 2016, NXP Semiconductor, Inc.
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
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used tom  endorse or promote products derived from this
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

#include "wifi_common.h"

#include "fsl_lpspi.h"
#include "athdefs.h"
#include "fsl_lpspi_freertos.h"
#include "fsl_dmamux.h"

#include "fsl_edma.h"
#include "fsl_lpspi_edma.h"

#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
#include "fsl_intmux.h"
#endif

edma_config_t userConfig;
lpspi_master_edma_handle_t g_lpspi_edma_m_handle;
edma_handle_t dspiEdmaMasterRxRegToRxDataHandle;
edma_handle_t dspiEdmaMasterTxDataToTxRegHandle;
lpspi_master_handle_t g_m_handle;

static SemaphoreHandle_t mutex;
static SemaphoreHandle_t event;
static int32_t g_dma_chunk = 500;
static enum IRQn g_dma_irqs[][FSL_FEATURE_EDMA_MODULE_CHANNEL] = DMA_CHN_IRQS;
static lpspi_master_config_t masterConfig;

/* DMA handler */
void LPSPI_DMA_MasterUserCallback(LPSPI_Type *base, lpspi_master_edma_handle_t *handle, status_t status, void *userData)
{
    /* disable DMA requests before invoke callback */
    BaseType_t reschedule = pdFALSE;
    xSemaphoreGiveFromISR(event, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}

/* IRQ handler */
void LPSPI_MasterUserCallback(LPSPI_Type *base, lpspi_master_handle_t *handle, status_t status, void *userData)
{
    /* disable IRQ requests before invoke callback */
    BaseType_t reschedule = pdFALSE;
    xSemaphoreGiveFromISR(event, &reschedule);
    portYIELD_FROM_ISR(reschedule);
}

/* Init DSPI wifi driver */
A_STATUS WIFI_Spi_Init(void)
{
    uint32_t baudrate = WIFI_BOARD_SPI_BAUDRATE;

    mutex = xSemaphoreCreateMutex();
    assert(NULL != mutex);
    event = xSemaphoreCreateBinary();
    assert(NULL != event);

#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
    if (WIFI_BOARD_SPI_IRQ > FSL_FEATURE_INTERRUPT_IRQ_MAX)
    {
        INTMUX_EnableInterrupt(WIFI_BOARD_SPI_INTMUX, WIFI_BOARD_SPI_INTMUX_CHNL, WIFI_BOARD_SPI_IRQ);
#   ifdef configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
        NVIC_SetPriority(WIFI_BOARD_SPI_INTMUX_IRQ, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
#   endif
    }
    else
    {
#   ifdef configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
        NVIC_SetPriority(WIFI_BOARD_SPI_IRQ, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
#   endif
    }
#else
#   ifdef configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
    NVIC_SetPriority(WIFI_BOARD_SPI_IRQ, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
#   endif
#endif

#ifdef configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
    NVIC_SetPriority(g_dma_irqs[WIFI_BOARD_EDMA_NUM][WIFI_BOARD_EDMA_SPI_RX_CHNL], configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    NVIC_SetPriority(g_dma_irqs[WIFI_BOARD_EDMA_NUM][WIFI_BOARD_EDMA_SPI_IM_CHNL], configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    NVIC_SetPriority(g_dma_irqs[WIFI_BOARD_EDMA_NUM][WIFI_BOARD_EDMA_SPI_TX_CHNL], configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
#endif

    /* init DMUX */
    DMAMUX_Init(WIFI_BOARD_EDMA_MUX);

    DMAMUX_SetSource(WIFI_BOARD_EDMA_MUX, WIFI_BOARD_EDMA_SPI_RX_CHNL, WIFI_BOARD_EDMA_SPI_RX_REQ_SRC);
    DMAMUX_EnableChannel(WIFI_BOARD_EDMA_MUX, WIFI_BOARD_EDMA_SPI_RX_CHNL);

    DMAMUX_SetSource(WIFI_BOARD_EDMA_MUX, WIFI_BOARD_EDMA_SPI_TX_CHNL, WIFI_BOARD_EDMA_SPI_TX_REQ_SRC);
    DMAMUX_EnableChannel(WIFI_BOARD_EDMA_MUX, WIFI_BOARD_EDMA_SPI_TX_CHNL);

    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(WIFI_BOARD_EDMA, &userConfig);

    /* setup SPI */
    memset(&masterConfig, 0, sizeof(masterConfig));
    LPSPI_MasterGetDefaultConfig(&masterConfig);

    masterConfig.baudRate = baudrate;
    masterConfig.bitsPerFrame = 8;
    masterConfig.cpol = kLPSPI_ClockPolarityActiveLow;
    masterConfig.cpha = kLPSPI_ClockPhaseSecondEdge;
    masterConfig.direction = kLPSPI_MsbFirst;
    masterConfig.pcsToSckDelayInNanoSec = 1000000000 / baudrate;
    masterConfig.lastSckToPcsDelayInNanoSec = 1000000000 / baudrate;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / baudrate;
    masterConfig.whichPcs = WIFI_BOARD_SPI_INIT_CS;
    masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;
    masterConfig.pinCfg = kLPSPI_SdiInSdoOut;
    masterConfig.dataOutConfig = kLpspiDataOutRetained;

    uint32_t srcClock_Hz = CLOCK_GetFreq(WIFI_BOARD_CLOCKSRC_SPI);
    LPSPI_MasterInit(WIFI_BOARD_SPI, &masterConfig, srcClock_Hz);

    /* setup DMA/DSPI handles */
    memset(&(dspiEdmaMasterRxRegToRxDataHandle), 0, sizeof(dspiEdmaMasterRxRegToRxDataHandle));
    memset(&(dspiEdmaMasterTxDataToTxRegHandle), 0, sizeof(dspiEdmaMasterTxDataToTxRegHandle));
    EDMA_CreateHandle(&(dspiEdmaMasterRxRegToRxDataHandle), WIFI_BOARD_EDMA, WIFI_BOARD_EDMA_SPI_RX_CHNL);
    EDMA_CreateHandle(&(dspiEdmaMasterTxDataToTxRegHandle), WIFI_BOARD_EDMA, WIFI_BOARD_EDMA_SPI_TX_CHNL);
    LPSPI_MasterTransferCreateHandleEDMA(WIFI_BOARD_SPI, &g_lpspi_edma_m_handle, LPSPI_DMA_MasterUserCallback, NULL,
                                         &dspiEdmaMasterRxRegToRxDataHandle, &dspiEdmaMasterTxDataToTxRegHandle);

    /* DSPI IRQ non-blocking handle */
    LPSPI_MasterTransferCreateHandle(WIFI_BOARD_SPI, &g_m_handle, LPSPI_MasterUserCallback, NULL);
    /* Ensure that DMA requests are disabled */
    return A_OK;
}

/* Deinit DSPI wifi driver */
A_STATUS WIFI_Spi_Deinit(void)
{
    LPSPI_Deinit(WIFI_BOARD_SPI);
    return A_OK;
    // TODO: deinit mutexes
}

/* transfer data in DMA mode */
static A_STATUS WIFI_SPI_DMA_Transfer(lpspi_transfer_t *transfer)
{
    status_t result = LPSPI_MasterTransferEDMA(WIFI_BOARD_SPI, &g_lpspi_edma_m_handle, transfer);
    if (kStatus_Success != result)
    {
        assert(0);
        return A_ERROR;
    }
    /* semaphore is released in callback fn */
    if (pdTRUE != xSemaphoreTake(event, portMAX_DELAY))
    {
        assert(0);
        return A_ERROR;
    }
    return A_OK;
}

/* transfer data in IRQ mode */
static A_STATUS WIFI_SPI_IRQ_Transfer(lpspi_transfer_t *transfer)
{
    status_t result = LPSPI_MasterTransferNonBlocking(WIFI_BOARD_SPI, &g_m_handle, transfer);
    if (kStatus_Success != result)
    {
        assert(0);
        return A_ERROR;
    }
    if (pdTRUE != xSemaphoreTake(event, portMAX_DELAY))
    {
        assert(0);
        result = A_ERROR;
    }
    return A_OK;
}

/* transfer data */
static A_STATUS WIFI_SPI_Transfer(lpspi_transfer_t *transfer)
{
    A_STATUS result = A_OK;

    /* NOTE: following code expects that SDK drivers do not
     * modify members of 'transfer' argument */
    for (int32_t to_transfer = transfer->dataSize; to_transfer;)
    {
        if (to_transfer < WIFI_BOARD_SPI_THRESHOLD)
        {
            /* DMA is unefficient for small amount of data, so use IRQ mode.
             * IRQ mode can transfer unlimited number of data */
            transfer->dataSize = to_transfer;
            result = WIFI_SPI_IRQ_Transfer(transfer);
            if (A_OK != result)
                break;
            to_transfer = 0;
        }
        else
        {
            /* DSPI over EDMA can transfer only limited number of bytes
             * so we have to split transfer into chunks */
            transfer->dataSize = to_transfer < g_dma_chunk ? to_transfer : g_dma_chunk;
            result = WIFI_SPI_DMA_Transfer(transfer);
            if (A_OK != result)
                break;
            to_transfer -= transfer->dataSize;
            /* recalculate rx/rx offsets */
            if (NULL != transfer->txData)
            {
                transfer->txData += transfer->dataSize;
            }
            if (NULL != transfer->rxData)
            {
                transfer->rxData += transfer->dataSize;
            }
        }
    }

    return result;
}

/* read and write API */
A_STATUS WIFI_Spi_InOutToken(uint32_t OutToken, uint8_t DataSize, uint32_t *pInToken)
{
    A_STATUS result;
    lpspi_transfer_t transfer = {0};

    transfer.txData = (uint8_t *)&OutToken;
    transfer.rxData = (uint8_t *)pInToken;
    transfer.dataSize = DataSize;
    transfer.configFlags = WIFI_BOARD_SPI_XFER_CS;

    /* Protect transmit by mutex */
    if (pdTRUE != xSemaphoreTake(mutex, portMAX_DELAY))
    {
        return A_ERROR;
    }
    result = WIFI_SPI_Transfer(&transfer);
    xSemaphoreGive(mutex);
    return result;
}

/* read or write API */
A_STATUS WIFI_Spi_InOutBuffer(uint8_t *pBuffer, uint16_t length, uint8_t doRead, boolean sync)
{
    A_STATUS result;
    lpspi_transfer_t transfer = {0};

    if (doRead)
    {
        transfer.txData = NULL;
        transfer.rxData = pBuffer;
    }
    else
    {
        transfer.txData = pBuffer;
        transfer.rxData = NULL;
    }
    transfer.dataSize = length;
    transfer.configFlags = WIFI_BOARD_SPI_XFER_CS;

    /* Protect transmit by mutex */
    if (pdTRUE != xSemaphoreTake(mutex, portMAX_DELAY))
    {
        return A_ERROR;
    }
    result = WIFI_SPI_Transfer(&transfer);
    xSemaphoreGive(mutex);
    return result;
}
