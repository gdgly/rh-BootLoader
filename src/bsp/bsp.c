/*************************************************
  Copyright (C), RoadLighting Tech. Co., Ltd.
  File name: flash.c     
  Author: George      Version: 1.0        Date: 2015.08.20
  Description:  底层初始化
  Others: 无        
  Function List:  
    1. rcc_config
	2. bsp_init
	3. gpio_config
  History:                     
    1. Date: 2015.9.14
       Author: George
       Modification: gpio_config 
*************************************************/
#include "bsp.h"


/*
*Function: rcc_config      
*Description: 系统时钟初始化 
*Calls: 无
*Data Accessed: 无
*Data Updated: 无   
*Input: 无     
*Output: 无
*Return: 无       
*Others: 无   
*/
void rcc_config(void)
{
	ErrorStatus err_status;

	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);

	err_status = RCC_WaitForHSEStartUp();

	if (err_status == SUCCESS)
	{
		/* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        
        /* Flash 2 wait state */
        FLASH_SetLatency(FLASH_Latency_2);
        
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1); 
        
        /* PCLK2 = HCLK */
        RCC_PCLK2Config(RCC_HCLK_Div1); 
        
        /* PCLK1 = HCLK */
        RCC_PCLK1Config(RCC_HCLK_Div1);
        
        /* PLLCLK = 8MHz * 9 = 72 MHz */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);
        
        /* Enable PLL */ 
        RCC_PLLCmd(ENABLE);
	
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        
        /* Wait till PLL is used as system clock source */
        while(RCC_GetSYSCLKSource() != 0x08);
	}	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE);
}

/*
*Function: gpio_config      
*Description: gpio初始化  
*Calls: 无
*Data Accessed: 无
*Data Updated: 无   
*Input: 无    
*Output: 无
*Return: 无      
*Others: 无   
*/
void gpio_config(void)
{
	GPIO_InitTypeDef gpio_init_structure;
	
	gpio_init_structure.GPIO_Pin = GPIO_Pin_2;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init_structure);
}

void watchdot_clear(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_2);
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
}


/*
*Function: bsp_init      
*Description: 底层初始化主函数  
*Calls:  
    rcc_config
*Data Accessed: 无
*Data Updated: 无   
*Input: 无    
*Output: 无
*Return: 无      
*Others: 无   
*/
void bsp_init(void)
{
	rcc_config();
	gpio_config();
}



