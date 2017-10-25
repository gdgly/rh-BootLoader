


#ifndef _UART_H
#define _UART_H
#include <stm32f10x.h>



#define UART_BUFF_LENGHT				   	512

typedef struct
{
	uint16_t index;	
	uint8_t	 data[UART_BUFF_LENGHT];	
} SerialBuffType;

#define SerialBuffDefault() {0,0,{0}}


typedef struct __va_list 
{
	void *__ap; 
} va_list;


#define va_start(ap, parmN) __va_start(ap, parmN)
#define va_arg(ap, type) __va_arg(ap, type)




void receive_data(uint8_t *rece_data, uint8_t *data);
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif




