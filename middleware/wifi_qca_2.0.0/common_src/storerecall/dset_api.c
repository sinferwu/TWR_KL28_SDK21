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

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <wmi.h>
#include <atheros_wifi_api.h>

#include "dset_api.h"

HOST_DSET_HANDLE dset_handles[MAX_HOST_DSET_SIZE];


//TODO: fixme. dummy fn, implemented in app
PREWEAK_CODE uint32_t POSTWEAK_CODE remote_dset_op(DSET_OP op, HOST_DSET_HANDLE *pDsetHandle)
{
		return -1;
}


HOST_DSET_HANDLE *dset_find_handle(uint32_t dset_id)
{
    uint16_t i;
    HOST_DSET_HANDLE *pDsetHandle = dset_handles;

    for (i = 0; i < MAX_HOST_DSET_SIZE; i++)
    {
        if (pDsetHandle->dset_id == dset_id)
            return pDsetHandle;
    }
    return NULL;
}

void dset_close_handle(HOST_DSET_HANDLE *pDsetHandle)
{
    pDsetHandle->dset_id = INVALID_DSET_ID;
    if (pDsetHandle->data_ptr)
        A_FREE(pDsetHandle->data_ptr, MALLOC_ID_TEMPORARY);
}

HOST_DSET_HANDLE *dset_insert_handle(uint32_t dset_id, uint32_t flags, dset_callback_fn_t cb, void *cb_args)
{
    uint16_t i;
    HOST_DSET_HANDLE *pDsetHandle = dset_handles;

    for (i = 0; i < MAX_HOST_DSET_SIZE; i++, pDsetHandle++)
    {
        if (pDsetHandle->dset_id == INVALID_DSET_ID)
        {
            pDsetHandle->dset_id = dset_id;
            pDsetHandle->flags = flags;
            pDsetHandle->cb = cb;
            pDsetHandle->cb_args = cb_args;
            pDsetHandle->data_ptr = NULL;
            return pDsetHandle;
        };
    }
    return NULL;
}

uint32_t qcom_dset_create(HOST_DSET_HANDLE **pDsetHandle,
                          uint32_t dset_id,
                          uint32_t media_id,
                          uint32_t length,
                          uint32_t flags,
                          dset_callback_fn_t create_cb,
                          void *callback_arg)
{
    uint32_t status;

    *pDsetHandle = dset_find_handle(dset_id);
    if (*pDsetHandle != NULL)
        return false;

    *pDsetHandle = dset_insert_handle(dset_id, flags, create_cb, callback_arg);
    if (*pDsetHandle == NULL)
        return false;

    (*pDsetHandle)->length = length;
    (*pDsetHandle)->media_id = media_id;

    status = remote_dset_op(DSET_OP_CREATE, *pDsetHandle);

    return status;
}

uint32_t qcom_dset_open(
    HOST_DSET_HANDLE **ppDsetHandle, uint32_t dset_id, uint32_t flags, dset_callback_fn_t open_cb, void *callback_arg)
{
    uint32_t status;

    *ppDsetHandle = dset_find_handle(dset_id);
    if (*ppDsetHandle != NULL)
        return A_OK;

    *ppDsetHandle = dset_insert_handle(dset_id, flags, open_cb, callback_arg);
    if (*ppDsetHandle == NULL)
        return false;

    status = remote_dset_op(DSET_OP_OPEN, *ppDsetHandle);

    return status;
}

uint32_t qcom_dset_write(HOST_DSET_HANDLE *pDsetHandle,
                         uint8_t *buffer,
                         uint32_t length,
                         uint32_t offset,
                         uint32_t flags,
                         dset_callback_fn_t write_cb,
                         void *callback_arg)
{
#define DSET_WRITE_HEAD_SIZE (sizeof(WMIX_DSET_OP_WRITE_PARAM_CMD) + +sizeof(WMIX_DSET_CMD_HDR) - 1)

    uint32_t status;

    pDsetHandle->offset = offset;
    pDsetHandle->length = length;
    pDsetHandle->flags = flags;
    pDsetHandle->cb = write_cb;
    pDsetHandle->cb_args = callback_arg;

    if (length + DSET_WRITE_HEAD_SIZE > MAX_DSET_BUFF_SIZE)
        return 0;

    if (pDsetHandle->data_ptr == NULL)
        pDsetHandle->data_ptr = A_MALLOC(MAX_DSET_BUFF_SIZE, MALLOC_ID_TEMPORARY);

    memcpy(pDsetHandle->data_ptr + DSET_WRITE_HEAD_SIZE, buffer, length);

    status = remote_dset_op(DSET_OP_WRITE, pDsetHandle);
    return status;
}

uint32_t qcom_dset_read(HOST_DSET_HANDLE *pDsetHandle,
                        uint8_t *buffer,
                        uint32_t length,
                        uint32_t offset,
                        dset_callback_fn_t read_cb,
                        void *callback_arg)
{
    uint32_t status;

    pDsetHandle->offset = offset;
    pDsetHandle->length = length;

    pDsetHandle->cb = read_cb;
    pDsetHandle->cb_args = callback_arg;

    if (length + sizeof(WMIX_DSET_OP_WRITE_PARAM_CMD) > MAX_DSET_BUFF_SIZE)
        return 0;

    if (pDsetHandle->data_ptr == NULL)
        pDsetHandle->data_ptr = A_MALLOC(MAX_DSET_BUFF_SIZE, MALLOC_ID_TEMPORARY);

    status = remote_dset_op(DSET_OP_READ, pDsetHandle);
    memcpy(buffer, pDsetHandle->data_ptr, length);
    return status;
}

uint32_t qcom_dset_commit(HOST_DSET_HANDLE *pDsetHandle, dset_callback_fn_t commit_cb, void *callback_arg)
{
    uint32_t status;

    pDsetHandle->cb = commit_cb;
    pDsetHandle->cb_args = callback_arg;
    status = remote_dset_op(DSET_OP_COMMIT, pDsetHandle);

    dset_close_handle(pDsetHandle);

    return status;
}

uint32_t qcom_dset_close(HOST_DSET_HANDLE *pDsetHandle, dset_callback_fn_t close_cb, void *callback_arg)
{
    uint32_t status;

    pDsetHandle->cb = close_cb;
    pDsetHandle->cb_args = callback_arg;

    status = remote_dset_op(DSET_OP_CLOSE, pDsetHandle);

    dset_close_handle(pDsetHandle);

    return status;
}

uint32_t qcom_dset_size(HOST_DSET_HANDLE *pDsetHandle, dset_callback_fn_t size_cb, void *callback_arg)
{
    uint32_t status;

    pDsetHandle->cb = size_cb;
    pDsetHandle->cb_args = callback_arg;

    status = remote_dset_op(DSET_OP_SIZE, pDsetHandle);
    return status;
}

uint32_t qcom_dset_delete(uint32_t dset_id, uint32_t flags, dset_callback_fn_t delete_cb, void *callback_arg)
{
    uint32_t status;
    HOST_DSET_HANDLE *pDsetHandle;

    pDsetHandle = dset_find_handle(dset_id);
    if (pDsetHandle == NULL)
    {
        pDsetHandle = dset_insert_handle(dset_id, flags, delete_cb, callback_arg);
        if (pDsetHandle == NULL)
            return false;
    }
    else
    {
        pDsetHandle->cb = delete_cb;
        pDsetHandle->cb_args = callback_arg;
        pDsetHandle->dset_id = dset_id;
        pDsetHandle->flags = flags;
    }

    status = remote_dset_op(DSET_OP_DELETE, pDsetHandle);
    return status;
}
