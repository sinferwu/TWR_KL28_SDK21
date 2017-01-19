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

#include "dset.h"

HOST_DSET host_dsets[MAX_DSET_SIZE];

HOST_DSET *dset_find(uint32_t dset_id)
{
    uint16_t i;
    HOST_DSET *pDset;

    for (i = 0, pDset = host_dsets; i < MAX_DSET_SIZE; i++, pDset++)
    {
        if (pDset->dset_id == dset_id)
            return pDset;
    }

    return NULL;
}

HOST_DSET *dset_insert(uint32_t dset_id)
{
    uint16_t i;
    HOST_DSET *pDset;

    for (i = 0, pDset = host_dsets; i < MAX_DSET_SIZE; i++, pDset++)
    {
        if (pDset->dset_id == 0)
        {
            pDset->dset_id = dset_id;
            if (pDset->data_ptr)
            {
                A_FREE(pDset->data_ptr, MALLOC_ID_CONTEXT);
                pDset->data_ptr = NULL;
            }

            return pDset;
        }
    }

    return NULL;
}

HOST_DSET *dset_get(uint32_t dset_id, uint32_t length)
{
    unsigned char *pbuf;

    HOST_DSET *pDset;

    pDset = dset_find(dset_id);

    if (pDset == NULL)
    {
        pDset = dset_insert(dset_id);
        if (pDset == NULL)
            return pDset;
    }

    /* Free the data buffer and reallocate based on new length */
    if (pDset->data_ptr)
    {
        A_FREE(pDset->data_ptr, MALLOC_ID_CONTEXT);
        pDset->data_ptr = NULL;
    }

    pbuf = (unsigned char *)A_MALLOC(length, MALLOC_ID_CONTEXT);
    if (pbuf == NULL)
    {
        pDset->dset_id = 0;
        return NULL;
    }

    pDset->data_ptr = pbuf;
    pDset->length = length;

    return pDset;
}

uint32_t dset_write(HOST_DSET *pDset, uint8_t *pData, uint32_t offset, uint32_t length)
{
    uint32_t wrt_len;

    if (offset + length > pDset->length)
        wrt_len = pDset->length - offset;
    else
        wrt_len = length;

    memcpy(pDset->data_ptr + offset, pData, wrt_len);

    return wrt_len;
}

uint32_t dset_read(HOST_DSET *pDset, uint8_t *pData, uint32_t offset, uint32_t length)
{
    uint32_t wrt_len;

    if (offset + length > pDset->length)
        wrt_len = pDset->length - offset;
    else
        wrt_len = length;

    memcpy(pData, pDset->data_ptr + offset, wrt_len);

    return wrt_len;
}

uint32_t dset_fill(uint8_t *pData, uint32_t max_dset_count)
{
    uint16_t i, count = 0;
    HOST_DSET *pDset;
    HOST_DSET_ITEM *pItem = (HOST_DSET_ITEM *)pData;

    for (i = 0, pDset = host_dsets; i < MAX_DSET_SIZE && count < max_dset_count; i++, pDset++)
    {
        if (pDset->dset_id != 0)
        {
            pItem->dset_id = pDset->dset_id;
            pItem->length = pDset->length;
            pItem++;
            count++;
        }
    }

    return count;
}

uint16_t next_dset_ndx;

HOST_DSET *dset_get_first(void)
{
    uint16_t i;
    HOST_DSET *pDset;

    for (i = 0, pDset = host_dsets; i < MAX_DSET_SIZE; i++, pDset++)
    {
        if (pDset->dset_id != 0)
        {
            next_dset_ndx = i + 1;
            return pDset;
        }
    }

    return NULL;
}

HOST_DSET *dset_get_next(void)
{
    uint16_t i;
    HOST_DSET *pDset;

    pDset = &host_dsets[next_dset_ndx];

    for (i = next_dset_ndx; i < MAX_DSET_SIZE; i++, pDset++)
    {
        if (pDset->dset_id != 0)
        {
            next_dset_ndx = i + 1;
            return pDset;
        }
    }

    return NULL;
}
