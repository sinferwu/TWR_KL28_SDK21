#ifndef __dset_h__
#define __dset_h__

/**HEADER********************************************************************
*
*
* Copyright (c) 2014    Qualcomm Atheros Inc.;
* All Rights Reserved
*
*
* $FileName: dset.h$
* $Version : 3.8.40.0$
* $Date    : May-28-2014$
*
* Comments:
*
*   This file contains the defines, externs and data
*   structure definitions required by application
*   programs in order to use the Ethernet packet driver.
*
*END************************************************************************/

/*--------------------------------------------------------------------------*/
/*
**                            CONSTANT DEFINITIONS
*/

#define MAX_DSET_SIZE 16

/*--------------------------------------------------------------------------*/
/*
**                            TYPE DEFINITIONS
*/

typedef struct host_dset_struct
{
    uint32_t dset_id;  /* dset id    */
    uint32_t length;   /* dset length            */
    uint8_t *data_ptr; /* dset buffer address */
} HOST_DSET;

typedef struct host_dset_item
{
    uint32_t dset_id; /* dset id    */
    uint32_t length;  /* dset length            */
} HOST_DSET_ITEM;

/*--------------------------------------------------------------------------*/
/*
**                            PROTOTYPES AND GLOBAL EXTERNS
*/

HOST_DSET *dset_find(uint32_t dset_id);
HOST_DSET *dset_get(uint32_t dset_id, uint32_t length);
uint32_t dset_write(HOST_DSET *pDset, uint8_t *pData, uint32_t offset, uint32_t length);
uint32_t dset_read(HOST_DSET *pDset, uint8_t *pData, uint32_t offset, uint32_t length);
uint32_t dset_fill(uint8_t *pData, uint32_t max_dset_count);

HOST_DSET *dset_get_first(void);
HOST_DSET *dset_get_next(void);

#endif
