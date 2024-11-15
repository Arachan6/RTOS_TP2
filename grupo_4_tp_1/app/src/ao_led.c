#include <ao_led.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"
#include "app.h"
#include "ao_led.h"


#define LED_PERIOD_        			(1000)
#define LED_QUEUE_LENGTH_     		(5)
#define LED_QUEUE_ITEM_SIZE_  		(sizeof(bool))

bool ao_led_send (msg_t *hao){
	ao_led_handle_t* lh = (ao_led_handle_t*)hao->led_h;
	LOGGER_INFO("2");
	if (lh->hqueue == NULL ){
		LOGGER_INFO("cola nula");
	}
	return (pdPASS == xQueueSend(lh->hqueue, &hao, 0));
}

void task_led(void *argument){
	ao_led_handle_t *hao = (ao_led_handle_t *)argument;
	msg_t* pmsg;
	while (true){
		LOGGER_INFO("6");
		if (pdPASS == xQueueReceive(hao->hqueue, (void*)&pmsg, portMAX_DELAY)){
			LOGGER_INFO("4");
			HAL_GPIO_WritePin((GPIO_TypeDef *)hao->led_port, (uint16_t)hao->led_pin, GPIO_PIN_SET);
			LOGGER_INFO("3");
			pmsg->callback_process(pmsg);
		}
	}
}

void ao_led_init (ao_led_handle_t *hao, GPIO_TypeDef *led_port, uint16_t led_pin){
	hao->hqueue = xQueueCreate(LED_QUEUE_LENGTH_, LED_QUEUE_ITEM_SIZE_);
	while (NULL == hao->hqueue){}

	hao->led_port = led_port;
	hao->led_pin  = led_pin;

	BaseType_t status;
	status = xTaskCreate(task_led, "task_ao_led", 128, (void*)hao, tskIDLE_PRIORITY, NULL);
	configASSERT(pdPASS == status);
}
