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
#include <netbuf.h>
#include <cust_netbuf.h>
#include <htc_api.h>

void Driver_ReportReverseCredits(void *pReq);

#if defined(DRIVER_CONFIG_IMPLEMENT_RX_FREE_MULTIPLE_QUEUE)
uint8_t GET_QUEUE_INDEX(A_NETBUF_QUEUE_T *q)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        if (&GET_FREE_QUEUE(i) == q)
            break;
    }

    return i;
}
#endif

#if defined(DRIVER_CONFIG_IMPLEMENT_RX_FREE_MULTIPLE_QUEUE)

extern uint8_t reverse_credits_init;
uint8_t credits_test = 0;

void a_rxbuf_enqueue(A_NETBUF_QUEUE_T *q, void *pReq)
{
    uint8_t epid;
    uint32_t bufCtrlNdx;

    a_netbuf_enqueue(q, pReq);

    if (reverse_credits_init == 0)
        return;

    epid = A_NETBUF_GET_ELEM(pReq, A_REQ_EPID);
    if (epid == ENDPOINT_0)
        return;

    //    i = GET_QUEUE_INDEX(q);
    bufCtrlNdx = GetQueueCtrlIndexByEPID(epid);
    if (bufCtrlNdx < 8 && bufCtrlNdx != 7)
    {
        CREDIT_INC(bufCtrlNdx);
        //        Driver_ReportReverseCredits(pReq);
    }
    else
    {
        //       printf("wrong\n");
    }
}

void *a_rxbuf_dequeue(A_NETBUF_QUEUE_T *q)
{
    void *pReq;
    //    uint8_t     i;

    pReq = a_netbuf_dequeue(q);
    if (pReq == NULL)
    {
        return pReq;
    }

    /*    i = GET_QUEUE_INDEX(q);

        if ( i < 8)
            CREDIT_DEC(i);
    */
    return pReq;
}
#endif

void *a_netbuf_peek_queue(A_NETBUF_QUEUE_T *q)
{
    return q->head;
}

int32_t a_netbuf_queue_size(A_NETBUF_QUEUE_T *q)
{
    return q->count;
}

int32_t a_netbuf_queue_empty(A_NETBUF_QUEUE_T *q)
{
    return ((q->count == 0) ? 1 : 0);
}

/* EOF */
