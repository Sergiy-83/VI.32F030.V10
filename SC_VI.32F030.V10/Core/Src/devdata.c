#include "devdata.h"

I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim14;

//Буффер приема
uint8_t buffer[RCV_BUFF_SIZE];

//Параметры

uint8_t 	par_brig_percent; 	//Яркость

uint32_t 	par_pwm_max;				//max - 65535 Для таймера. Определит частоту шима
uint32_t 	par_pwm_cur;				//Оприделит скважность

void dev_init()
	{
	par_pwm_max = 50000;
	par_pwm_cur = 40000;
		
	RCV_CMD 	= 0;
	RCV_PARAM = 0;
	RCV_DATA 	= 0;
		


	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_TIM14_Init();
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
	
	//HAL_Delay(10);
	//set_leds_percent(0);	
	set_leds(1);
	set_brig(50);	
	}

void set_brig	(uint32_t arg_value)
	{
	if (arg_value > 100) 	arg_value = 100;
	if (arg_value == 0) 	arg_value = 1;
		
	par_brig_percent = arg_value;
	par_pwm_cur = par_pwm_max * par_brig_percent / 100; 
	
		
	HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);	
	MX_TIM14_Init(); //Перенастройка таймера PWM		
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
	}	
	
void set_pos (uint32_t arg_value)
	{	
		
	set_leds_percent (arg_value); //Зажечь в процентном соотношении
	}	
	
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
		{
    Error_Handler();
		}

  //Initializes the CPU, AHB and APB buses clocks
  
  RCC_ClkInitStruct.ClockType 			= RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
		{
    Error_Handler();
		}
		
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
		{
    Error_Handler();
		}
}

void MX_I2C1_Init(void)
	{
  hi2c1.Instance 							= I2C1;
  hi2c1.Init.Timing 					= 0x2000090E;
  hi2c1.Init.OwnAddress1 			= 20;
  hi2c1.Init.AddressingMode 	= I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode 	= I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 			= 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode 	= I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode 		= I2C_NOSTRETCH_DISABLE;
	
	HAL_I2C_MspInit(&hi2c1);
		
		
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
		{
    Error_Handler();
		}

  //Configure Analogue filter
 // if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	//	{
 //   Error_Handler();
	//	}

  //Configure Digital filter
 // if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	//	{
 //   Error_Handler();
	//	}
	
	 
}


//TIM14 Initialization Function
void MX_TIM14_Init(void)
	{
  TIM_OC_InitTypeDef sConfigOC = {0};
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 0;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = par_pwm_max;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  
	if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
		{
    Error_Handler();
		}
  
	if (HAL_TIM_PWM_Init(&htim14) != HAL_OK)
		{
    Error_Handler();
		}
		
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = par_pwm_cur;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
		{
    Error_Handler();
		}
  
	HAL_TIM_MspPostInit(&htim14);
	}


	void MX_GPIO_Init(void)
	{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12
                          |GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|
													GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC14 PC15 */
	
  GPIO_InitStruct.Pin 	= GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 	= GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PF0 PF1 PF6 PF7 */
	
  GPIO_InitStruct.Pin 	= GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 	= GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 PA5 PA6 PA7 PA8 PA9 PA10 PA11 PA12 PA15 */
	
  GPIO_InitStruct.Pin 	= GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 | GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15;
  GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 	= GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB12 PB13 PB14 PB15 PB3 PB4 PB5 PB6 PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | 
												GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6|GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
												
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	/*
	GPIO_InitStruct.Pin 	= GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStruct.Mode 	= GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull 	= GPIO_PULLUP;						//Подтяжка шины i2c
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); */
	}


void HAL_MspInit(void)
	{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
	}
	
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    
    //PB10 -> I2C1_SCL
    //PB11 -> I2C1_SDA
    
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);
  }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hi2c->Instance==I2C1)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB10     ------> I2C1_SCL
    PB11     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  }

}



/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM14)
  {
  /* Peripheral clock enable */
  __HAL_RCC_TIM14_CLK_ENABLE();
  }
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(htim->Instance==TIM14)
  {
  /* USER CODE BEGIN TIM14_MspPostInit 0 */

  /* USER CODE END TIM14_MspPostInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM14 GPIO Configuration
    PA4     ------> TIM14_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_TIM14;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM14_MspPostInit 1 */

  /* USER CODE END TIM14_MspPostInit 1 */
  }

}
/**
* @brief TIM_Base MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM14)
  {
  __HAL_RCC_TIM14_CLK_DISABLE();
  }
}



//Если что то случилось то моргаем светодиодом быстро
#define CNT_ERR 400000
#define CNT_ERR2 800000

void Error_Handler(void)
	{
	uint32_t i; 	
  __disable_irq();
  while (1)
		{
		LED_ON(LED_T);
		for(i=0;i<CNT_ERR;i++)
			{
				__NOP();
			}
			
		LED_OFF(LED_T);
		for(i=0;i<CNT_ERR2;i++)
			{
				__NOP();
			}
		}
	}

//Если двумя мастерами долбить индикатор то i2c отваливается. При этом одна из линий в нуле
//Проверяем и переинициализируем i2c	
void check_i2c(void)
	{
	uint32_t pin10 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10);
	uint32_t pin11 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
	if (pin10 == 0 && pin11 == 1)
		{
		HAL_Delay(50);
		pin10 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10);
		pin11 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
		if (pin10 == 0 && pin11 == 1)
			{
			HAL_I2C_MspDeInit(&hi2c1);
			HAL_I2C_DeInit(&hi2c1);
			MX_I2C1_Init();
			printf( "%d : reinit i2c\n",HAL_GetTick() );
			}
		}
	}	