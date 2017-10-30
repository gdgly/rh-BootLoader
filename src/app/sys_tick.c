

#include "sys_tick.h"


volatile uint32_t sys_tick_cnt;

/*
*Function: delay_1ms      
*Description: 毫秒级延时函数   
*Calls:         
*Data Accessed: sys_tick_cnt：系统滴答计数器  
*Data Updated: sys_tick_cnt    
*Input: ms：要延时的时间     
*Output: 无 
*Return: void        
*Others: 无    
*/
void delay_1ms(uint32_t ms)
{
	sys_tick_cnt = ms;		 
	while(sys_tick_cnt != 0);					//等待延时时间到
}

/*
*Function: SysTick_Handler      
*Description: 系统滴答中断服务程序   
*Calls:         
*Data Accessed: sys_tick_cnt：系统滴答计数器  
*Data Updated: sys_tick_cnt    
*Input: void     
*Output: 无 
*Return: void        
*Others: 无    
*/
void SysTick_Handler(void)
{
	if (sys_tick_cnt > 0)
	{
		sys_tick_cnt--;
	}
}
