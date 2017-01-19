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
#include <common_api.h>
#include <custom_wlan_api.h>
#include <htc.h>
#include <atheros_wifi_api.h>

#if (!ENABLE_STACK_OFFLOAD)

uint32_t Custom_Api_Send(
    QCA_CONTEXT_STRUCT_PTR qca_ptr, PCB_PTR pcb_ptr, uint32_t size, uint32_t frags, uint32_t flags);

/*****************************************************************************/
/*  Custom_DeliverFrameToNetworkStack - Called by API_RxComplete to
 *   deliver a data frame to the network stack. This code will perform
 *   platform specific operations.
 *      void *pCxt - the driver context.
 *      void *pReq - the packet.
 *****************************************************************************/
void Custom_DeliverFrameToNetworkStack(void *pCxt, void *pReq)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)pReq;
    QCA_CONTEXT_STRUCT_PTR qca_ptr = (QCA_CONTEXT_STRUCT_PTR)GET_DRIVER_CXT(pCxt)->pUpperCxt[0];
    QCA_ECB_STRUCT_PTR RxECB;
    uint32_t len;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    ATH_PROMISCUOUS_CB prom_cb = (ATH_PROMISCUOUS_CB)(GET_DRIVER_CXT(pCxt)->promiscuous_cb);

    if (a_netbuf_ptr)
    {
        len = A_NETBUF_LEN(pReq);
        _DCACHE_INVALIDATE_MBYTES(A_NETBUF_DATA(pReq), len);

        if (pDCxt->promiscuous_mode)
        {
            if (prom_cb)
            {
                /* send frame to callback function rather than an QCA_receiver */
                a_netbuf_ptr->native.FRAG[0].LENGTH = len;
                a_netbuf_ptr->native.FRAG[0].FRAGMENT = A_NETBUF_DATA(pReq);
                prom_cb((void *)&(a_netbuf_ptr->native));
            }
            else
            {
                A_NETBUF_FREE(pReq);
            }
        }
        else if ((RxECB = QCA_find_receiver(qca_ptr, (QCA_HEADER_PTR)A_NETBUF_DATA(pReq), &len)) != NULL)
        {
            // Call the receiver's service function to pass the PCB to the receiver
            a_netbuf_ptr->native.FRAG[0].LENGTH = len;
            a_netbuf_ptr->native.FRAG[0].FRAGMENT = A_NETBUF_DATA(pReq);
            RxECB->SERVICE((PCB_PTR) & (a_netbuf_ptr->native), RxECB->PRIVATE);
        }
        else
        {
            /* failed to find a receiver for this data packet. */
            A_NETBUF_FREE(pReq);
        }
    }
}

/*****************************************************************************/
/*  Custom_Api_Send - Entry point for MQX driver interface to send a packet.
 *	 This is specific to MQX. This function is NOT called from within the
 *	 driver.
 *      QCA_CONTEXT_STRUCT_PTR  qca_ptr - the MQX driver context.
 *		PCB_PTR              pcb_ptr - the MQX packet object.
 *		uint32_t              size - the length in bytes of pcb_ptr.
 *		uint32_t              frags - the number of fragments in pcb_ptr.
 *		uint32_t              flags - optional flags for transmit.
 *****************************************************************************/
uint32_t Custom_Api_Send(QCA_CONTEXT_STRUCT_PTR qca_ptr,
                         /* [IN] the Ethernet state structure */
                         PCB_PTR pcb_ptr,
                         /* [IN] the packet to send */
                         uint32_t size,
                         /* [IN] total size of the packet */
                         uint32_t frags,
                         /* [IN] total fragments in the packet */
                         uint32_t flags
                         /* [IN] optional flags, zero = default */
                         )
{
    uint32_t error = QCA_OK;
    A_NETBUF *a_netbuf_ptr;
    UNUSED_ARGUMENT(flags);
    UNUSED_ARGUMENT(size);
#if 0    
if(g_sampleTotAlloc != g_totAlloc){
	A_PRINTF("send alloc: %d\n", g_totAlloc);
	g_sampleTotAlloc = g_totAlloc;
}
#endif
    /* create an atheros pcb and continue or fail. */
    do
    {
        /* provide enough space in the top buffer to store the 14 byte
         * header which will be copied from its position in the original
         * buffer. this will allow wmi_dix_2_dot3() to function properly.
         */
        if ((a_netbuf_ptr = (A_NETBUF *)A_NETBUF_ALLOC(sizeof(ATH_MAC_HDR))) == NULL)
        {
            error = ENETERR_ALLOC_PCB;
            break;
        }

        a_netbuf_ptr->num_frags = (uint8_t)frags;
        /* HACK: copy the first part of the fragment to the new buf in order to allow
         * wmi_dix_2_dot3() to function properly. */
        A_ASSERT(pcb_ptr->FRAG[0].LENGTH >= sizeof(ATH_MAC_HDR));
        A_NETBUF_PUT_DATA(a_netbuf_ptr, (uint8_t *)pcb_ptr->FRAG[0].FRAGMENT, sizeof(ATH_MAC_HDR));
        /*(char*)pcb_ptr->FRAG[0].FRAGMENT += sizeof(ATH_MAC_HDR);*/
        pcb_ptr->FRAG[0].FRAGMENT = (uint8_t *)((uint32_t)pcb_ptr->FRAG[0].FRAGMENT + sizeof(ATH_MAC_HDR));
        pcb_ptr->FRAG[0].LENGTH -= sizeof(ATH_MAC_HDR);

        // ensure there is enough headroom to complete the tx operation
        if (A_NETBUF_HEADROOM(a_netbuf_ptr) <
            sizeof(ATH_MAC_HDR) + sizeof(ATH_LLC_SNAP_HDR) + sizeof(WMI_DATA_HDR) + HTC_HDR_LENGTH + WMI_MAX_TX_META_SZ)
        {
            error = ENETERR_ALLOC_PCB;
            break;
        }
        // carry the original around until completion.
        // it is freed by A_NETBUF_FREE
        a_netbuf_ptr->native_orig = pcb_ptr;

        if (A_OK != Api_DataTxStart((void *)qca_ptr->MAC_CONTEXT_PTR, (void *)a_netbuf_ptr))
        {
            error = QCA_ERROR;
            break;
        }

    } while (0);

    if (error != QCA_OK)
    {
        /* in the event of api failure, the original pcb should be returned to the caller un-touched
         * and the a_netbuf_ptr should be freed */
        a_netbuf_ptr->native_orig = NULL;
        A_NETBUF_FREE((void *)a_netbuf_ptr);
    }

    return error;
}

#endif
