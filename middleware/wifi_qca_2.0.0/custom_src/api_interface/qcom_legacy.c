//------------------------------------------------------------------------------
// Copyright (c) 2014 Qualcomm Atheros, Inc.
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

#if 0

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <driver_cxt.h>
#include <common_api.h>
#include <custom_api.h>
#include <wmi_api.h>
#include <bmi.h>
#include <htc.h>
#include <wmi_host.h>
#if ENABLE_P2P_MODE
#include <wmi.h>
#include "p2p.h"
#endif
#include "mqx.h"
#include "bsp.h"
#include "enet.h"
#include "enetprv.h"
#include "atheros_wifi.h"
#include "enet_wifi.h"
#include "atheros_wifi_api.h"
#include "atheros_wifi_internal.h"
#include "atheros_stack_offload.h"
#include "dset_api.h"
#include "common_stack_offload.h"
#include "qcom_api.h"
#include "qcom_legacy.h"

extern A_VOID *p_Driver_Cxt[];
extern uint_32 Custom_Api_Mediactl( QCA_CONTEXT_STRUCT_PTR qca_ptr, uint_32 command_id, pointer inout_param);
extern uint_32 ath_ioctl_handler(QCA_CONTEXT_STRUCT_PTR qca_ptr, ATH_IOCTL_PARAM_STRUCT_PTR param_ptr);
extern A_STATUS qcom_set_deviceid( A_UINT16 id);


/*-----------------------------------------------------------------------------------------------------------------*/
A_STATUS depr_qcom_set_scan(A_UINT8 device_id)
{
    A_STATUS error = A_OK;
    if(qcom_set_deviceid(device_id) == A_ERROR){
        return A_ERROR;
    }
    if( Custom_Api_Mediactl(p_Driver_Cxt[device_id],QCA_SET_MEDIACTL_SCAN,NULL) != A_OK)
       error = A_ERROR;
  
    return error;
}

A_STATUS depr_qcom_ota_done()
{
    return((A_STATUS)custom_ota_done(p_Driver_Cxt[0], 1));
}

#endif
