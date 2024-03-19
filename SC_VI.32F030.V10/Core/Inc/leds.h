#ifndef LEDS_H
#define LEDS_H
#include <stdint.h>

struct viind
	{
	uint32_t leds_percent;			//Значение в процентном соотношении
	uint32_t leds_current_pos;	//Значение в процентном соотношении
	uint32_t leds_value;				//Состояние светодиодов в виде uint32_t
	};
	
	
#define VI_LEDS_NUM					32


#define LED_T  GPIOB, GPIO_PIN_12
#define LED_ON(x) 	HAL_GPIO_WritePin(x, GPIO_PIN_SET)
#define LED_OFF(x) 	HAL_GPIO_WritePin(x, GPIO_PIN_RESET)

void 		set_leds_percent (uint32_t arg_value); 	//Зажечь в процентном соотношении
void 		set_leds (uint32_t arg_value); 					//Зажечь количество светодиодов
uint8_t	get_leds (void); 					//Узнать количество светодиодов
uint8_t get_leds_percent (void); 	//Узнать процент

#endif