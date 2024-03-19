#include "stdio.h" 
#include "EventRecorder.h"
#include "devdata.h" 
	

	
int main(void)
	{
	HAL_StatusTypeDef ret;
	EventRecorderInitialize( EventRecordAll, 1 );
	dev_init();	
		
	printf("Start program\n");
	while (1)
		{ 
		ret = HAL_I2C_Slave_Receive(&hi2c1, buffer, RCV_BUFF_SIZE, 500); //HAL_MAX_DELAY
		if (ret == HAL_OK)				
		  	{
				LED_ON(LED_T);
				//////////////////////////////////////////////////////////////////////////////////////////		
				if (RCV_CMD == VI_CMD_READ)	//Мастер хочет читать
					{
					
					switch(RCV_DATA) //Смотрим что прочитать
							{
							case VI_NUM_LEDS: 	
												{
												uint8_t num_leds = VI_LEDS_NUM;
												HAL_I2C_Slave_Transmit(&hi2c1, &num_leds, sizeof(num_leds),100);
												break;
												}
												
							case VI_POS_PERCENT :	
												{
												uint8_t cur_percent = get_leds_percent();
												HAL_I2C_Slave_Transmit(&hi2c1, &cur_percent, sizeof(cur_percent),100);
												break;
												}
												
							case VI_BRIG_PERCENT: 	
												HAL_I2C_Slave_Transmit(&hi2c1, &par_brig_percent, sizeof(par_brig_percent),100);
												break;
							
							case VI_POS_LEDS: 	
												{
												uint8_t cur_pos_leds = get_leds();	
												HAL_I2C_Slave_Transmit(&hi2c1, &cur_pos_leds, sizeof(cur_pos_leds),100);
												break;
												}
							default:;	
							}
					}
				//////////////////////////////////////////////////////////////////////////////////////////////////////	
				if (RCV_CMD == VI_CMD_WRITE)	
						{
						switch( RCV_PARAM )
								{
								case VI_POS_PERCENT:	set_leds_percent(RCV_DATA);
																			break;
								case VI_POS_LEDS: 		set_leds(RCV_DATA);
																			break;
								case VI_BRIG_PERCENT: set_brig(RCV_DATA); 
																			break;
								default:;
								}
						}		
				//////////////////////////////////////////////////////////////////////////////////////////////////////	
				}
		else									
			{
			LED_OFF(LED_T);	
			check_i2c();
			}												
	} //while
} //main


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
