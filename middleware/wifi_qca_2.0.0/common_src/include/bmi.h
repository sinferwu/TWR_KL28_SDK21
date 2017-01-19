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
#ifndef _BMI_H_
#define _BMI_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Header files */
#include "a_config.h"
#include "athdefs.h"
#include "a_types.h"
//#include "hif.h"
#include "a_osapi.h"
#include "bmi_msg.h"

A_STATUS
BMIInit(void *pCxt);

void BMICleanup(void);

A_STATUS
BMIDone(void *pCxt);

A_STATUS
BMIGetTargetInfo(void *pCxt, struct bmi_target_info *targ_info);

A_STATUS
BMIReadMemory(void *pCxt, uint32_t address, uint8_t *buffer, uint32_t length);

A_STATUS
BMIWriteMemory(void *pCxt, uint32_t address, uint8_t *buffer, uint32_t length);

A_STATUS
BMIExecute(void *pCxt, uint32_t address, uint32_t *param);

A_STATUS
BMIReadSOCRegister(void *pCxt, uint32_t address, uint32_t *param);

A_STATUS
BMIWriteSOCRegister(void *pCxt, uint32_t address, uint32_t param);

A_STATUS
BMIRawWrite(void *pCxt, uint8_t *buffer, uint32_t length);

A_STATUS
BMIRawRead(void *pCxt, uint8_t *buffer, uint32_t length, boolean want_timeout);

#ifdef __cplusplus
}
#endif

#endif /* _BMI_H_ */
