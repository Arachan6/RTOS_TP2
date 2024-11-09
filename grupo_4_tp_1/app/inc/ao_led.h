#ifndef TASK_LED_H_
#define TASK_LED_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

/********************** macros ***********************************************/

/********************** typedef **********************************************/

typedef struct {
    QueueHandle_t hqueue;
    GPIO_TypeDef * led_port;
	uint16_t led_pin;
} ao_led_handle_t;

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

void ao_led_init(ao_led_handle_t* hao, GPIO_TypeDef* led_port, uint16_t led_pin);

bool ao_led_send(ao_led_handle_t* hao);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_LED_H_ */
/********************** end of file ******************************************/
