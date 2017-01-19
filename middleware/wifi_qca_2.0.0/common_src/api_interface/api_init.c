//------------------------------------------------------------------------------
// Copyright (c) 2011 Qualcomm Atheros, Inc.
// All Rights Reserved.
// Qualcomm Atheros Confidential and Proprietary.
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is
// hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE
// INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
// USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#include "wifi_common.h"

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <driver_cxt.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <wmi_api.h>
#include <wlan_api.h>
#include <AR6K_version.h>
#include <atheros_wifi_api.h>
#include <atheros_wifi_internal.h>
#include <atheros_stack_offload.h>
#include <common_stack_offload.h>

const uint8_t max_performance_power_param = MAX_PERF_POWER;
const WMI_STORERECALL_CONFIGURE_CMD default_strrcl_config_cmd = {1, STRRCL_RECIPIENT_HOST};
const WMI_POWER_PARAMS_CMD default_power_param = {0,
                                                  A_CPU2LE16(1), // nonzero value requires endian correction
                                                  0,
                                                  0,
                                                  A_CPU2LE16(1), // nonzero value requires endian correction
                                                  IGNORE_POWER_SAVE_FAIL_EVENT_DURING_SCAN};

const WMI_SCAN_PARAMS_CMD default_scan_param = {0, 0, 0, 0, 0, WMI_SHORTSCANRATIO_DEFAULT, DEFAULT_SCAN_CTRL_FLAGS,
                                                0, 0, 0};

#if ENABLE_P2P_MODE
const WMI_P2P_FW_SET_CONFIG_CMD default_p2p_config = {0, "US", 81, 6, 81, 6, A_CPU2LE32(3000), 10};
#endif

void Api_BootProfile(void *pCxt, uint8_t val)
{
    if (ath_custom_init.Boot_Profile != NULL)
    {
        ath_custom_init.Boot_Profile(val);
    }
}

/*****************************************************************************/
/*  Api_InitStart - implements common code for initializing the driver.
 *	 This should be called before Api_InitFinish().
 *      void *pCxt - the driver context.
 *****************************************************************************/
A_STATUS
Api_InitStart(void *pCxt)
{
    A_STATUS status;

    do
    {
        /* 1 - initialize context */
        if (A_OK != (status = Driver_ContextInit(pCxt)))
        {
            break;
        }
        /* 2 - insert driver into system if necessary */
        if (A_OK != (status = DRIVER_INSERT(pCxt)))
        {
            break;
        }

        /*3 - initilize socket context*/
        if (A_OK != (status = SOCKET_CONTEXT_INIT))
        {
            break;
        }

    } while (0);

    return status;
}

/*****************************************************************************/
/*  Api_InitFinish - implements common code for initializing the driver.
 *	 This should be called after Api_InitStart().
 *      void *pCxt - the driver context.
 *****************************************************************************/
A_STATUS
Api_InitFinish(void *pCxt)
{
    // int32_t     timeleft;
    // int16_t     i;
    A_STATUS ret = A_OK;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    if (ath_custom_init.skipWmi)
    {
        return A_OK;
    }

    do
    {
        //		Api_BootProfile(pCxt, 0x00);
        /* Wait for Wmi event to be ready Implementation of this macro is platform specific
         * as multi-threaded systems will implement differently from single threaded. */
        /* Increased the WMI_READY timeout from 5 seconds to 15 seconds to allow
           firmware download over SPI interface */
        if (A_OK != DRIVER_WAIT_FOR_CONDITION(pCxt, &(pDCxt->wmiReady), true, 15000))
        {
            ret = A_ERROR;
            break;
        }

        //        Api_BootProfile(pCxt, BOOT_PROFILE_WMI_READY);

        // printf("%d\r\n", pDCxt->wmiReady);
        if (pDCxt->wmiReady != true)
        {
            ret = A_ERROR;
            break;
        }
        /* WMI_EVENT will have populated arVersion so now confirm that version is compatible */
        if (pDCxt->abiVersion != 1)
        {
            if ((pDCxt->abiVersion & ABI_MAJOR_MASK) != (AR6K_ABI_VERSION & ABI_MAJOR_MASK))
            {
                ret = A_ERROR;
                break;
            }
        }

    } while (0);

    return ret;
}

static A_STATUS wmi_cmd_process(void *pCxt, WMI_COMMAND_ID cmd, const void *pParam, uint16_t length)
{
    A_STATUS status;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    status = wmi_cmd_start(pDCxt->pWmiCxt, pParam, cmd, length);

    if (status == A_NO_MEMORY)
    {
        pDCxt->tx_complete_pend = true;

        if (A_OK != DRIVER_WAIT_FOR_CONDITION(pCxt, &(pDCxt->tx_complete_pend), false, 5000))
        {
            A_ASSERT(0);
        }
    }
    else if (status != A_OK)
    {
        A_ASSERT(0);
    }

    return status;
}

/*****************************************************************************/
/*  Api_WMIInitFinish - implements common code for sending default wmi commands
 *                      to target.
 *	 This should be called after Api_InitFinish().
 *      void *pCxt - the driver context.
 *****************************************************************************/
void Api_WMIInitFinish(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    A_STATUS status;
    WMI_ALLOW_AGGR_CMD allow_aggr_cmd;

    if (pDCxt->wmiReady == true)
    {
        do
        {
            status = STACK_INIT(pCxt);

            if (status == A_OK)
            {
                WAIT_FOR_WMI_RESPONSE(pCxt);
                break;
            }
            else if (status == A_NO_MEMORY)
            {
                pDCxt->tx_complete_pend = true;

                if (A_OK != DRIVER_WAIT_FOR_CONDITION(pCxt, &(pDCxt->tx_complete_pend), false, 5000))
                {
                    A_ASSERT(0);
                }
            }
            else
            {
                A_ASSERT(0);
            }
        } while (1);

/* issue some default WMI commands appropriate for most systems */
#if WLAN_CONFIG_IGNORE_POWER_SAVE_FAIL_EVENT_DURING_SCAN
        wmi_cmd_process(pCxt, WMI_SET_POWER_PARAMS_CMDID, &default_power_param, sizeof(WMI_POWER_PARAMS_CMD));
#endif
        wmi_cmd_process(pCxt, WMI_SET_SCAN_PARAMS_CMDID, &default_scan_param, sizeof(WMI_SCAN_PARAMS_CMD));

        wmi_cmd_process(pCxt, WMI_STORERECALL_CONFIGURE_CMDID, &default_strrcl_config_cmd,
                        sizeof(WMI_STORERECALL_CONFIGURE_CMD));
        pDCxt->strrclState = STRRCL_ST_INIT;
        /* technically this call to wmi_allow_aggr_cmd is not necessary if both
         * masks are 0 as the firmware has 0,0 as the default. */
        allow_aggr_cmd.tx_allow_aggr = A_CPU2LE16(pDCxt->txAggrTidMask);
        allow_aggr_cmd.rx_allow_aggr = A_CPU2LE16(pDCxt->rxAggrTidMask);
        wmi_cmd_process(pCxt, WMI_ALLOW_AGGR_CMDID, &allow_aggr_cmd, sizeof(WMI_ALLOW_AGGR_CMD));
#if ENABLE_P2P_MODE
        if (WLAN_NUM_OF_DEVICES == 2)
        {
            /* Device-0 is P2P Device and Device-1 is Legacy STA.\
               Set Default P2P Params
             */
            wmi_cmd_process(pCxt, WMI_P2P_SET_CONFIG_CMDID, &default_p2p_config, sizeof(WMI_P2P_FW_SET_CONFIG_CMD));
        }
#endif
        /* Set the BSS Filter to None. If this is not set, by default the firmware
       sets to forward the beacons to host. This causes unnecessary BSSINFO events in
       the host even after connecting to the AP */
        wmi_bssfilter_cmd(pDCxt->pWmiCxt, NONE_BSS_FILTER, 0);
    }
}

/*****************************************************************************/
/*  Api_DeInitStart - implements common code for de-initializing the driver.
 *	 This should be called before Api_DeInitFinish().
 *      void *pCxt - the driver context.
 *****************************************************************************/
A_STATUS
Api_DeInitStart(void *pCxt)
{
    UNUSED_ARGUMENT(pCxt);
    /* This is a place holder for future enhancements */
    return A_OK;
}

/*****************************************************************************/
/*  Api_DeInitFinish - implements common code for de-initializing the driver.
 *	 This should be called after Api_DeInitStart().
 *      void *pCxt - the driver context.
 *****************************************************************************/
A_STATUS
Api_DeInitFinish(void *pCxt)
{
    A_STATUS status = A_ERROR;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /* 1 - block until driver thread is cleaned up. in systems without
         * 		a driver thread this is a NOP. */
        if (A_OK != DRIVER_WAIT_FOR_CONDITION(pCxt, &(pDCxt->driver_up), false, 5000))
        {
            break;
        }
        /* 2 - remove driver from system */
        DRIVER_REMOVE(pCxt);
        /* 3 - De-initialize context */
        Driver_ContextDeInit(pCxt);

        /*4 - Deinitialize socket context*/
        SOCKET_CONTEXT_DEINIT();

        status = A_OK;
    } while (0);

    return status;
}
