/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_vbat.h"
#include "app_bme280.h"
#include "app_flash.h"

//  ======== globals ============================================
int8_t ind;		// declaration of isr index

//  ======== app_flash_init =====================================
int8_t app_flash_init(const struct device *dev)
{
	int8_t ret;
	ssize_t size;

	dev = FLASH_DEVICE;
	if (dev == NULL) {
		printk("no eeprom device found. error: %d\n", dev);
		return 0;
	}
	
   if (!device_is_ready(dev)) {
		printk("eeprom is not ready\n");
		return 0;
	} else {
        printk("- found device \"%s\", writing/reading data\n", dev->name);
    }

	// erasing 3kB storage partition at 0x0003F000
	ret  = flash_erase(dev, FLASH_OFFSET, FLASH_SIZE);
	if (ret) {
		printk("error erasing flash. error: %d\n", ret);
	} else {
		printk("erased all pages\n");
	}
	
	// checking allocated size
	size = eeprom_get_size(dev);
	printk("using flash memory with size of: %zu.\n", size);

	// initialisation of isr index
	ind = 0;	
	return 0;
}

//  ======== app_flash_write ====================================
int8_t app_flash_write(const struct device *dev, struct vtph *data)
{	
	int8_t ret;
	dev = FLASH_DEVICE;
	
	// writing data in the first page of 2kB for this test
	ret = flash_write(dev, FLASH_OFFSET, &data, sizeof(data));
	if (ret) {
		printk("error writing data. error: %d\n", ret);
	} else {
		printk("wrote %zu bytes to address 0x0003f000\n", sizeof(data));
	}

	// printing data to be stored in memory
	for (ind = 0; ind < SENS_MAX_RECORDS; ind++) {
		printk("wrt -> vbat: %d, temp: %d, press: %d, hum: %d\n", data[ind].vbat, data[ind].temp, data[ind].press, data[ind].hum);
	}
	return 0;
}

//  ======== app_flash_read =====================================
int8_t app_flash_read(const struct device *dev)
{
	int8_t ret;
	dev = FLASH_DEVICE;
	struct vtph data[SENS_MAX_RECORDS];

	// reading the first page of 2kB for this test
	ret = flash_read(dev, FLASH_OFFSET, &data, sizeof(data));
	if (ret) {
		printk("error reading data. error: %d\n", ret);
	} else {
		printk("read %zu bytes from address 0x0003f000\n", sizeof(data));
	}

	// printing data stored in memory
	for (ind = 0; ind < SENS_MAX_RECORDS; ind++) {
		printk("rd -> vbat: %d, temp: %d, press: %d, hum: %d\n", data[ind].vbat, data[ind].temp, data[ind].press, data[ind].hum);
	}
	return 0;
}
	
//  ======== app_flash_handler ==================================
int8_t app_flash_handler(const struct device *dev)
{
	uint16_t vbat, temp, press, hum;
	const struct device *bme_dev;
	const struct device *bat_dev;
	const struct device *flash_dev;
	struct vtph data[SENS_MAX_RECORDS];

	// getting all sensor devices
	bat_dev = DEVICE_DT_GET_ONE(st_stm32_vbat);
	bme_dev = DEVICE_DT_GET_ANY(bosch_bme280);

	// putting n structures in fisrt page of 2kB for this test
	while (ind < SENS_MAX_RECORDS) {
		data[ind].vbat = app_stm32_get_vbat(bat_dev);
		data[ind].temp = app_bme280_get_temp(bme_dev);
		data[ind].press = app_bme280_get_press(bme_dev);
		data[ind].hum = app_bme280_get_hum(bme_dev);
		ind++;
	}
	// writing and reading stored data
	app_flash_write(flash_dev, &data);
	app_flash_read(flash_dev);

	// cleaning data storage partition
	(void)flash_erase(flash_dev, FLASH_OFFSET, FLASH_SIZE);
	ind = 0;
	return 0;
}

