//------------------------------------------------------------------------------
// <copyright file="custom_stack_offload.h" company="Atheros">
// Copyright (c) 2004-2007 Atheros Corporation.  All rights reserved.
// Copyright (c) 2011-2015 Qualcomm Atheros, Inc.
// All Rights Reserved.
// Qualcomm Atheros Confidential and Proprietary.
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#ifndef _CUSTOM_STACK_OFFLOAD_H_
#define _CUSTOM_STACK_OFFLOAD_H_

#if ENABLE_STACK_OFFLOAD

#include <atheros_stack_offload.h>

#define IS_SOCKET_BLOCKED(index) isSocketBlocked((index))

/* Headroom definitions*/
#define UDP_HEADROOM 44
#define TCP_HEADROOM 64
#define UDP6_HEADROOM 64
#define TCP6_HEADROOM 88
#define TCP6_HEADROOM_WITH_NO_OPTION 84

#define TX_PKT_OVERHEAD (TCP6_HEADROOM + sizeof(DRV_BUFFER) + sizeof(uint32_t))

#if ZERO_COPY
extern A_NETBUF_QUEUE_T zero_copy_free_queue;
#endif

uint32_t isSocketBlocked(void *ctxt);

A_STATUS custom_receive_tcpip(void *pCxt, void *pReq);
void txpkt_free(void *buffPtr);
A_STATUS custom_send_tcpip(void *pCxt,
                           /* [IN] the Ethernet state structure */
                           DRV_BUFFER_PTR db_ptr,
                           /* [IN] the packet to send */
                           uint32_t size,
                           /* [IN] total size of the packet */
                           uint32_t frags,
                           /* [IN] total fragments in the packet */
                           uint8_t *header,
                           uint32_t header_size

                           );

void custom_free(void *buf);
void *custom_alloc(uint32_t size);
uint32_t get_total_pkts_buffered(void);
#endif // ENABLE_STACK_OFFLOAD
#endif
