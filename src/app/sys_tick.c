

#include "sys_tick.h"


volatile uint32_t sys_tick_cnt;

/*
*Function: delay_1ms      
*Description: ���뼶��ʱ����   
*Calls:         
*Data Accessed: sys_tick_cnt��ϵͳ�δ������  
*Data Updated: sys_tick_cnt    
*Input: ms��Ҫ��ʱ��ʱ��     
*Output: �� 
*Return: void        
*Others: ��    
*/
void delay_1ms(uint32_t ms)
{
	sys_tick_cnt = ms;		 
	while(sys_tick_cnt != 0);					//�ȴ���ʱʱ�䵽
}

/*
*Function: SysTick_Handler      
*Description: ϵͳ�δ��жϷ������   
*Calls:         
*Data Accessed: sys_tick_cnt��ϵͳ�δ������  
*Data Updated: sys_tick_cnt    
*Input: void     
*Output: �� 
*Return: void        
*Others: ��    
*/
void SysTick_Handler(void)
{
	if (sys_tick_cnt > 0)
	{
		sys_tick_cnt--;
	}
}
