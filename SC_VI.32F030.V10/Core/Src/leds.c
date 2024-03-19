#include "leds.h"
#include "stm32f0xx_hal.h"

#define LED_ON_OFF(x)  if (arg_value & (1<<x) ) 	LED_ON(LED_##x);	else LED_OFF(LED_##x);
#define LED_0  GPIOA, GPIO_PIN_2
#define LED_1  GPIOA, GPIO_PIN_1
#define LED_2  GPIOA, GPIO_PIN_0
#define LED_3  GPIOA, GPIO_PIN_3
#define LED_4  GPIOA, GPIO_PIN_5
#define LED_5  GPIOA, GPIO_PIN_6
#define LED_6  GPIOA, GPIO_PIN_7
#define LED_7  GPIOB, GPIO_PIN_0
#define LED_8  GPIOB, GPIO_PIN_1
#define LED_9  GPIOB, GPIO_PIN_2
#define LED_10 GPIOF, GPIO_PIN_1
#define LED_11 GPIOF, GPIO_PIN_0
#define LED_12 GPIOC, GPIO_PIN_15
#define LED_13 GPIOC, GPIO_PIN_14
#define LED_14 GPIOB, GPIO_PIN_9
#define LED_15 GPIOB, GPIO_PIN_8
#define LED_16 GPIOB, GPIO_PIN_7
#define LED_17 GPIOB, GPIO_PIN_6
#define LED_18 GPIOB, GPIO_PIN_5
#define LED_19 GPIOB, GPIO_PIN_4
#define LED_20 GPIOB, GPIO_PIN_3
#define LED_21 GPIOA, GPIO_PIN_15
#define LED_22 GPIOF, GPIO_PIN_7
#define LED_23 GPIOF, GPIO_PIN_6
#define LED_24 GPIOA, GPIO_PIN_12
#define LED_25 GPIOA, GPIO_PIN_11
#define LED_26 GPIOA, GPIO_PIN_10
#define LED_27 GPIOA, GPIO_PIN_9
#define LED_28 GPIOA, GPIO_PIN_8
#define LED_29 GPIOB, GPIO_PIN_15
#define LED_30 GPIOB, GPIO_PIN_13
#define LED_31 GPIOB, GPIO_PIN_14


struct viind vi;

//void set_leds_uint		(uint32_t arg_value); 
//void set_leds_visual_0(uint32_t arg_value); 

//Зажечь как биты байта
void set_leds_uint(uint32_t arg_value)
	{
	vi.leds_value = arg_value;
	LED_ON_OFF(0);
	LED_ON_OFF(1);
	LED_ON_OFF(2);
	LED_ON_OFF(3);
	LED_ON_OFF(4);
	LED_ON_OFF(5);
	LED_ON_OFF(6);
	LED_ON_OFF(7);
	LED_ON_OFF(8);
	LED_ON_OFF(9);
	LED_ON_OFF(10);
	LED_ON_OFF(11);
	LED_ON_OFF(12);
	LED_ON_OFF(13);
	LED_ON_OFF(14);
	LED_ON_OFF(15);
	LED_ON_OFF(16);
	LED_ON_OFF(17);
	LED_ON_OFF(18);
	LED_ON_OFF(19);
	LED_ON_OFF(20);
	LED_ON_OFF(21);
	LED_ON_OFF(22);
	LED_ON_OFF(23);
	LED_ON_OFF(24);
	LED_ON_OFF(25);
	LED_ON_OFF(26);
	LED_ON_OFF(27);
	LED_ON_OFF(28);
	LED_ON_OFF(29);
	LED_ON_OFF(30);
	LED_ON_OFF(31);
	}
	
//Зажечь с визуальным оформлением
void  set_leds_visual_0(uint32_t arg_value)
	{
	if 	(arg_value > VI_LEDS_NUM) 
		arg_value = VI_LEDS_NUM;
	
	set_leds_uint((1<<arg_value)-1);	
	}
	
//Зажечь количество светодиодов	
void set_leds (uint32_t arg_value)
	{
	if (arg_value>VI_LEDS_NUM)	
		arg_value = VI_LEDS_NUM;
	vi.leds_current_pos	= arg_value;
	set_leds_visual_0(arg_value);
	}
//Зажечь в процентном соотношении 
void	set_leds_percent(uint32_t arg_value)
	{
	if (arg_value > 100) arg_value = 100;
	vi.leds_percent = arg_value;
	
	set_leds( arg_value * (float)VI_LEDS_NUM / 100.0 );
	}
	
uint8_t	get_leds (void) 					//Узнать количество светодиодов
	{
	return vi.leds_current_pos;
	}
	
uint8_t get_leds_percent (void)
	{
	return vi.leds_percent;
	}	
