#ifndef DEVDATA_H
#define DEVDATA_H
#include <stdint.h>
#include "stm32f0xx_hal.h"
#include "leds.h"

#define RCV_BUFF_SIZE 3
extern uint8_t buffer[RCV_BUFF_SIZE];

#define RCV_CMD 	buffer[0]
#define RCV_PARAM buffer[1]
#define RCV_DATA 	buffer[2]

#define VI_CMD_WRITE 	0
#define VI_CMD_READ 	1

//Номера пораметров (адреса)
enum VI_PARAMS
	{
	VI_POS_PERCENT = 0,	//Позиция в процентном соотношении
	VI_POS_LEDS,				//Позиция в  количестве светодиодов
	VI_BRIG_PERCENT, 		//Яркость в процентном соотношении
	VI_NUM_LEDS					//Количество светодиодов
	};
	
extern uint8_t  par_brig_percent; 	//Яркость

extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim14;

void check_i2c_status(void);
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_I2C1_Init(void);
void MX_TIM14_Init(void);
void Error_Handler(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

void set_brig	(uint32_t arg_value); //Задать яркость
void dev_init();


#endif