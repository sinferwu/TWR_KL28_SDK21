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
#ifndef __ATHEROS_WIFI_H__
#define __ATHEROS_WIFI_H__

/* PORT_NOTE: atheros_wifi.h is intended to provide any system specific data
 * 	structures and definitions required by the OS for a WIFI/Ethernet driver.
 * 	Hence everything in this file is Customized to the requirements of the
 *	host OS. */

#include <a_osapi.h>

#include <wlan_config.h>

// ============================================================================
// This is the ONLY place where the enet.h header files are included!
// ============================================================================

// typedef struct wlan_hardware_config_s {
////	/// The SPI bus instance number. Usually 0 or 1
////	int spi_instance;
////	/// The SPI chip select (dspi_which_pcs_config_t)
////	int spi_cs;
////	/// The SPI baudrate
////	uint32_t spi_baudrate;
////
////	/// The WLAN interrupt's IRQ number from the IRQn enum in MK22F51212.h (or similar)
////	int wlan_irq_number;
////	/// The GPIO ping used for power control of the WLAN chip, e.g. GPIO_MAKE_PIN(GPIOB_IDX, 19)
////	int wlan_power_pin;
//} wlan_hardware_config_t;

#include "qca_structs.h"

#endif /* __ATHEROS_WIFI_H__ */
