//------------------------------------------------------------------------------
// Copyright (c) 2015 Qualcomm Atheros, Inc.
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
#ifndef _HCD_API_H_
#define _HCD_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <htc_api.h>
#include <driver_cxt.h>

A_STATUS
Hcd_Request(void *pCxt, void *pReq);
boolean Hcd_ReadCPUInterrupt(void *pCxt, uint8_t *cpuIntrCause);
void Hcd_ClearCPUInterrupt(void *pCxt);
A_STATUS
Hcd_UnmaskInterrupts(void *pCxt, uint16_t Mask); // IGX_UD_CHANGES
A_STATUS
Hcd_UnmaskInterrupts(void *pCxt, uint16_t Mask);
A_STATUS
Hcd_GetLookAhead(void *pCxt);
boolean Hcd_BusInterrupt(void *pCxt);
A_STATUS
Hcd_ReinitTarget(void *pCxt);
void Hcd_Deinitialize(void *pCxt);
A_STATUS
Hcd_Init(void *pCxt);
void Hcd_EnableCPUInterrupt(void *pCxt);
A_STATUS
Hcd_DoPioExternalAccess(void *pCxt, void *pReq);
void Hcd_MaskInterrupts(void *pCxt, uint16_t Mask);
A_STATUS
Hcd_ReinitTarget(void *pCxt);
A_STATUS
Hcd_UnmaskInterrupts(void *pCxt, uint16_t Mask);
A_STATUS
Bus_InOutDescriptorSet(void *pCxt, void *pReq);

#if 0
/*
* The below 3-functions are need to implement and  
* HCD_SUSPEND_CB and HCD_RESUME_CB structures are not avialable with 
* current driver source
*/

void Hcd_AckInterrupt(void *pCxt, uint16_t Mask);
void Hcd_RegisterSuspendHandler(HCD_SUSPEND_CB *suspendCallback, void *pSuspContext);
void Hcd_RegisterResumeHandler(HCD_RESUME_CB *resumeCallback, void *pSuspContext);
void 
Hcd_RegisterInterruptHandler(HCD_INTR_CALLBACK_T *callback, void *pContext);
#endif //#if 0

#endif //_HCD_API_H_
