



#include <stm32f10x_tim.h>
#include "timer.h"


volatile uint32_t g_tim_cnt[(uint8_t)tim_max] = {0};

void TIM2_IRQHandler(void)
{
	uint16_t i = 0;

	if (SET == TIM_GetITStatus(TIM2, TIM_IT_Update))
    {
        TIM_ClearITPendingBit( TIM2, TIM_IT_Update);

        for (i = 0; i<(uint8_t)tim_max; i++)
        {
            g_tim_cnt[i]++;
        }					  
    }
}


int is_timeout_1MS(uint8_t type, uint32_t count)
{
	int status = 0;
	
	if (g_tim_cnt[(uint8_t)(type)] >= count)
	{
		g_tim_cnt[(uint8_t)(type)] = 0;
		status = 1;
	}
	else
	{
		status = 0;
	}
	
	return 0;
}











