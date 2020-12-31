/****************************************************************************
 * Copyright (C) 2020 by Fabrice Muller                                     *
 *                                                                          *
 * This file is useful for ESP32 Design course.                             *
 *                                                                          *
 ****************************************************************************/

/**
 * @file lab2-4_main.c
 * @author Fabrice Muller
 * @date 13 Oct. 2020
 * @brief File containing the lab2-4 of Part 3.
 *
 * @see https://github.com/fmuller-pns/esp32-vscode-project-template
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "esp_log.h"
#include "soc/rtc_wdt.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "my_helper_fct.h"

static const char* TAG = "MsgQ";

// Push button for interrupt
static const gpio_num_t PIN_PUSH_BUTTON = 15;
uint32_t isrCount = 0;

/* Stack size for all tasks */
const uint32_t TASK_STACK_SIZE = 4096;
static const uint32_t T1_PRIO = 5;
QueueHandle_t xQueue1 = NULL;

void vCounterTask(void *pvParameters);
static void IRAM_ATTR Push_button_isr_handler(void *args);

void app_main(void) {

	/* Config GPIO */
	/* Config GPIO */
	gpio_config_t config_in ;
	config_in . intr_type = GPIO_INTR_NEGEDGE ; // falling edge interrupt
	config_in . mode = GPIO_MODE_INPUT ; // Input mode ( push button )
	config_in . pull_down_en = false ;
	config_in . pull_up_en = true ; // Pull -up resistor
	config_in . pin_bit_mask =  (1ULL << PIN_PUSH_BUTTON ) ; // Pin number
	gpio_config (& config_in ) ;

	/* Create Message Queue and Check if created */
	xQueue1 = xQueueCreate(5, sizeof(uint32_t *));
	printf("Hello ESP32 !\n");
	/* Create vCounterTask task */		
	xTaskCreate(vCounterTask, "Task 1", TASK_STACK_SIZE, (void*) "Task1", T1_PRIO, NULL);			

	/* Install ISR */
	/* Install ISR */
	gpio_install_isr_service (0) ;
	gpio_isr_handler_add ( PIN_PUSH_BUTTON , Push_button_isr_handler , ( void
	*) PIN_PUSH_BUTTON ) ;

	/* to ensure its exit is clean */
	//vTaskDelete(NULL);
}

void vCounterTask(void *pvParameters) {
	char *pcTaskName;
	uint32_t RecievedValue;
	pcTaskName = (char*) pvParameters;
	DISPLAY("Start of %s task, priority = %d",pcTaskName, uxTaskPriorityGet(NULL));


	for (;; ) {

		/* Wait for message with 5 sec. otherwise DISPLAY a message to push it */
		vTaskDelay (pdMS_TO_TICKS (5000));
		/* If pushed */
		
		if(gpio_get_level(PIN_PUSH_BUTTON)==0){

		
			// DISPLAY "Button pushed" and the isrCount variable
			DISPLAY("Button pushed. isrCount = %d", isrCount);
			// Get the number of items in the queue
			if (xQueueReceive(xQueue1, &RecievedValue, portMAX_DELAY)){
			// DISPLAYI (Information display) number of items if greater than 1
			DISPLAYI(TAG, "Task %s, recieved value = %d",pcTaskName,RecievedValue);
			}
			// Waiting for push button signal is 1 again (test every 20 ms)
				while(1){
					COMPUTE_IN_TICK(2);
					if(gpio_get_level(PIN_PUSH_BUTTON))
					break;
				}
			 DISPLAY ("Button released");
		}
		// Else If Time out 
		else{
			 DISPLAY ("Please, push the button!");
		}
		
	}
}


static void IRAM_ATTR Push_button_isr_handler(void *args) {
	uint32_t uIn =(uint32_t) args;
	//DISPLAY("isr-isr");
	// Increment isrCount
	isrCount++;
	// Send message
	xQueueSendFromISR(xQueue1, &uIn, NULL);

}
