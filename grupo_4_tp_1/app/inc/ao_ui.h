#ifndef TASK_UI_H_
#define TASK_UI_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

#include "task_button.h"
#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

typedef struct{
	QueueHandle_t hqueue;
} ao_ui_handle_t;


typedef void (*callback_t)(msg_t* pmsg);

struct msg_t{
    ao_led_handle_t led_h;
    callback_t callback_process;
};

void ao_ui_init(ao_ui_handle_t* hao);
bool ao_ui_send(ao_ui_handle_t* hao, button_type_t button_press_type);

#ifdef __cplusplus
}
#endif

#endif /* TASK_UI_H_ */


