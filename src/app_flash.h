/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_FLASH_H
#define APP_FLASH_H

//  ======== includes ==============================================
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>

//  ======== globals ===============================================
struct vtph {
	uint16_t vbat;
	uint16_t temp;
	uint16_t press;
	uint16_t hum;
};

//  ======== defines ===============================================
#define FLASH_DEVICE			DT_HAS_CHOSEN(flash0)
#define FLASH_OFFSET			0x0003F000	// 2x2048 bits at the end of flash
#define FLASH_SIZE				3072		// 3kB to store sensor data
#define SENS_MAX_RECORDS        2       	// 1 structure: 4 samples (vbat, temp, press, hum) = 8 bytes
                                        	// 2 records for the test                                     

//  ======== prototypes ============================================
int8_t app_flash_init(const struct device *dev);
int8_t app_flash_write(const struct device *dev, struct vtph *data);
int8_t app_flash_read(const struct device *dev);
int8_t app_flash_handler(const struct device *dev);

#endif /* APP_FLASH_H */