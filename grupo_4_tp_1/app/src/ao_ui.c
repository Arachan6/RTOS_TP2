#include <ao_led.h>
#include <ao_ui.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"

#include "task_button.h"

#define AO_UI_QUEUE_LENGTH_    (5)
#define AO_UI_QUEUE_ITEM_SIZE_ (sizeof(button_type_t))

extern ao_led_handle_t ao_red_led_h;
extern ao_led_handle_t ao_green_led_h;
extern ao_led_handle_t ao_blue_led_h;

static void callback_process_completed_(msg_t* pmsg) {
  memory_pool_block_put(hmp, (void*)pmsg);
  LOGGER_INFO("Memoria liberada desde button");
  msg_wip_--;
  LOGGER_INFO("Mensajes en proceso: %d", msg_wip_);
}


void task_ui(void *argument){
	ao_ui_handle_t *huiao = (ao_ui_handle_t *)argument;
	button_type_t button_press_type;

	while (true){
		if (pdTRUE == xQueueReceive(huiao->hqueue, (void *)&button_press_type, portMAX_DELAY)){
			switch (button_press_type){
				case BUTTON_TYPE_PULSE:
					LOGGER_INFO("UI red led blink");
					msg_t* pmsg = (msg_t*)pvPortMalloc(sizeof(msg_t));
					if(NULL != pmsg) {
						LOGGER_INFO("Memoria alocada: %d", sizeof(msg_t));
						pmsg->led_h = &ao_red_led_h;
						if(pdPASS == ao_led_send(&pmsg)) {
							LOGGER_INFO("Mensaje enviado");
						} else {
							LOGGER_INFO("Mensaje no enviado");
							vPortFree((void*)pmsg);
							LOGGER_INFO("Memoria liberada desde button");
						}
					} else {
					  LOGGER_INFO("Memoria insuficiente");
					}
					break;
				case BUTTON_TYPE_SHORT:
					LOGGER_INFO("UI green led blink");
					ao_led_send(&ao_green_led_h);
					break;
				case BUTTON_TYPE_LONG:
					LOGGER_INFO("UI blue led blink");
					ao_led_send(&ao_blue_led_h);
					break;
				default:
					LOGGER_INFO("Unknown event for UI object");
					break;
			}
		}
	}
}

bool ao_ui_send(ao_ui_handle_t *hao, button_type_t btn_press_type){
    return (pdPASS == xQueueSend(hao->hqueue, (void *)&btn_press_type, 0));
}

void ao_ui_init(ao_ui_handle_t *hao){
    hao->hqueue = xQueueCreate(AO_UI_QUEUE_LENGTH_, AO_UI_QUEUE_ITEM_SIZE_);
    while (NULL == hao->hqueue){}

    BaseType_t status;
    status = xTaskCreate(task_ui, "task_ao_ui", 128, (void*)hao, tskIDLE_PRIORITY, NULL);
    configASSERT(pdPASS == status);
}

