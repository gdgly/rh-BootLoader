


#ifndef _TIMER_H
#define _TIMER_H
#include <stm32f10x.h>




enum timer
{
    tim1_cnt,
    tim2_cnt,
	tim3_cnt,
	tim4_cnt,
	tim5_cnt,
	tim_uart2,
	tim_ftp,
	tim_gprs,
	tim_at,
	tim_ymodem,
	tim_max
};




//如果index对应的时间超时，清零并返回1，未超时返回0
#define IS_TIMEOUT_1MS(index, count)    ((g_tim_cnt[(uint8_t)(index)] >= (count)) ?  	\
                                        ((g_tim_cnt[(uint8_t)(index)] = 0) == 0) : 0)



extern volatile uint32_t g_tim_cnt[(uint8_t)tim_max];


#endif

