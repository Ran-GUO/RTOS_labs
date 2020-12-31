/****************************************************************************
 * Copyright (C) 2020 by Fabrice Muller                                     *
 *                                                                          *
 * This file is useful for ESP32 Design course.                             *
 *                                                                          *
 ****************************************************************************/

/**
 * @file my_helper_fct.h
 * @author Fabrice Muller
 * @date 6 Oct. 2020
 * @brief File containing the lab1-1 of Part 3.
 *
 * @see https://github.com/fmuller-pns/esp32-vscode-project-template
 */

#ifndef _MY_HELPER_FCT_
#define _MY_HELPER_FCT_

#include <unistd.h>
#include "esp_log.h"


/* Core constants fot xTaskCreatePinnedToCore() function */
const uint32_t CORE_0 = 0;
const uint32_t CORE_1 = 1;
const uint32_t PRO_CPU = 0;
const uint32_t APP_CPU = 1;

/**
 * @brief Macro to display the buffer with arguments. msg is a formatted string as printf() function.
 * 
 */
#define DISPLAY(msg, ...) printf("%d:%d>\t"msg"\r\n", xTaskGetTickCount(), xPortGetCoreID(), ##__VA_ARGS__);


/**
 * @brief Macro to display error/warning/info for the buffer with arguments. msg is a formatted string as printf() function.
 * 
 */
#define DISPLAYE(tag, msg, ...) ESP_LOGE(tag, "%d:%d>\t"msg"", xTaskGetTickCount(), xPortGetCoreID(), ##__VA_ARGS__);

#define DISPLAYW(tag, msg, ...) ESP_LOGW(tag, "%d:%d>\t"msg"", xTaskGetTickCount(), xPortGetCoreID(), ##__VA_ARGS__);

#define DISPLAYI(tag, msg, ...) ESP_LOGI(tag, "%d:%d>\t"msg"", xTaskGetTickCount(), xPortGetCoreID(), ##__VA_ARGS__);

/**
 * @brief Macro to display the buffer without argument
 * 
 */
#define DISPLAYB(msg) printf("%d:%d>\t", xTaskGetTickCount(), xPortGetCoreID());\
	printf(msg);\
	printf("\r\n");


#define COMPUTE_IN_TICK(tick) ets_delay_us(1000*(tick*1000)/configTICK_RATE_HZ);

#define COMPUTE_IN_TIME_MS(time) { \
		int max = xTaskGetTickCount() + pdMS_TO_TICKS(time);\
		while (xTaskGetTickCount() < max);\
		}

#endif