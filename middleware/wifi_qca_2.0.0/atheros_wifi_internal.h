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
#ifndef __ATHEROS_WIFI_INTERNAL_H__
#define __ATHEROS_WIFI_INTERNAL_H__

#define ATH_PROG_FEEDBACK_BLANK_FLASH (0x00000001)
#define ATH_PROG_FEEDBACK_LOADING (0x00000002)
#define ATH_PROG_FEEDBACK_EXECUTING (0x00000003)
#define ATH_PROG_FEEDBACK_DONE (0x00000004)
#define ATH_PROG_FEEDBACK_LOAD_FAIL (0x00000005)
#define ATH_PROG_FEEDBACK_COMM_ERROR (0x00000006)

typedef struct
{
    A_STATUS (*Driver_TargetConfig)(void *);
    A_STATUS (*Driver_BootComm)(void *);
    A_STATUS (*Driver_BMIConfig)(void *);
    void (*Api_TxComplete)(void *, void *);
    void (*Api_RxComplete)(void *, void *);
    void (*Boot_Profile)(uint32_t val);
    void (*Custom_Delay)(uint32_t delay);
    void (*Api_GpioDataEventRx)(uint8_t *datap, uint32_t len);
    void (*Custom_reset_measure_timer)(void);
    void (*Custom_Api_PfmDataEventRx)(uint8_t *datap, uint32_t len);
    void (*Custom_Api_PfmDataDoneEventRx)(void *, uint8_t *datap, uint32_t len);
    A_STATUS (*Driver_StoreRecallFirmwareDownload)(void *pCxt);
    boolean skipWmi;
    boolean exitAtBmi;
} ATH_CUSTOM_INIT_T;

extern ATH_CUSTOM_INIT_T ath_custom_init;

typedef struct
{
    uint32_t (*ath_ioctl_handler_ext)(void *, ATH_IOCTL_PARAM_STRUCT_PTR);
} ATH_CUSTOM_MEDIACTL_T;

extern ATH_CUSTOM_MEDIACTL_T ath_custom_mediactl;

typedef struct
{
    A_STATUS (*HTCConnectServiceExch)(void*, HTC_SERVICE_CONNECT_REQ*, HTC_SERVICE_CONNECT_RESP*,
								  HTC_ENDPOINT_ID*, uint32_t*);
    A_STATUS (*HTCSendSetupComplete)(void *);
    A_STATUS (*HTCGetReady)(void *);
} ATH_CUSTOM_HTC_T;

extern ATH_CUSTOM_HTC_T ath_custom_htc;

#endif /* __ATHEROS_WIFI_INTERNAL_H__ */
