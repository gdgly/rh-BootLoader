
#include <string.h>
#include "gprs.h"
#include "bsp.h"
#include "uart.h"
#include "common.h"
#include "sys_tick.h"
#include "timer.h"
#include "flash.h"

extern SerialBuffType  *rece_buff2;

uint8_t send_at_flag = 0;	

uint8_t send_at_cmd1(uint8_t *cmd)
{
	uint8_t ret = 0;
	uint8_t tmp_data[512] = {0};

	send_at_flag = 0;
	memset(rece_buff2, 0, sizeof(SerialBuffType));
	IS_TIMEOUT_1MS(tim_gprs, 0);
	while (ret == 0)
	{
		memset(tmp_data, 0, sizeof(tmp_data));
		receive_data(rece_buff2->data, tmp_data);
		if (strstr((char*)tmp_data, "OK") != NULL)
		{
			ret = 1;
		}
		else
		{		
			memset(rece_buff2, 0, sizeof(SerialBuffType));
			if (send_at_flag == 0)
			{
				USART_OUT(USART2, cmd);
				send_at_flag = 1;
				IS_TIMEOUT_1MS(tim_at, 0);
			}
			delay_MS(1000);
			if (IS_TIMEOUT_1MS(tim_gprs, 20000))
			{
				ret = 0;
				break;
			}
		}
		
		if (IS_TIMEOUT_1MS(tim_at, 6000))
		{
			send_at_flag = 0;
		}
	}	
	memset(rece_buff2, 0, sizeof(SerialBuffType));
	
	return ret;
}


uint8_t send_at_cmd(uint8_t *cmd, uint8_t *str, uint32_t timeout)
{	
	uint8_t ret = 0;
	uint8_t tmp_data[512] = {0};

	send_at_flag = 0;
	memset(rece_buff2, 0, sizeof(SerialBuffType));
	IS_TIMEOUT_1MS(tim_gprs, 0);
	while (ret == 0)
	{
		memset(tmp_data, 0, sizeof(tmp_data));
		receive_data(rece_buff2->data, tmp_data);
		if (strstr((char*)tmp_data, str) != NULL)
		{
			ret = 1;
		}
		else
		{		
			memset(rece_buff2, 0, sizeof(SerialBuffType));
			if (send_at_flag == 0)
			{
				USART_OUT(USART2, cmd);
				send_at_flag = 1;
				IS_TIMEOUT_1MS(tim_at, 0);
			}
			delay_MS(1000);
			if (IS_TIMEOUT_1MS(tim_gprs, timeout))
			{
				ret = 0;
				break;
			}
		}
		
		if (IS_TIMEOUT_1MS(tim_at, 6000))
		{
			send_at_flag = 0;
		}
	}	
	memset(rece_buff2, 0, sizeof(SerialBuffType));
	
	return ret;
}


uint8_t gprs_status = 0;
extern uint8_t g_err_count;
uint8_t gprs_init(void)
{
	uint8_t ret = 0;
	
	switch (gprs_status)
	{
		case 0:
			gprs_config();
			g_err_count++;
			if (g_err_count >=2)
			{
				;
			}
			delay_MS(10000);
			gprs_status = 1;
		break;
	
		case 1:
			ret = send_at_cmd("\r\nAT+CSQ\r\n", "OK", 20000);	
			if(ret == 1)
			{			
				gprs_status = 2;
			}
			else
			{
				gprs_status = 0;
			}
		break;
		
		case 2:
			ret = send_at_cmd("\r\nAT+CREG=1\r\n", "OK", 20000);
			if(ret == 1)
			{			
				gprs_status = 3;
			}
			else
			{
				gprs_status = 0;
			}	
		break;
		
		case 3:
			ret = send_at_cmd("AT+CREG?\r\n", "OK", 20000);
			if(ret == 1)
			{			
				gprs_status = 4;
			}
			else
			{
				gprs_status = 0;
			}	
		break;
		
		case 4:
			ret = send_at_cmd("AT^SICS=0,conType,GPRS0\r\n", "OK", 20000);
			if(ret == 1)
			{			
				gprs_status = 5;
			}
			else
			{
				gprs_status = 0;
			}
		break;
		
		case 5:
			ret = send_at_cmd("AT^SICS=0,apn,CMNET\r\n", "OK", 20000);
			if(ret == 1)
			{			
				gprs_status = 6;
			}
			else
			{
				gprs_status = 0;
			}
		break;
		
		case 6:
			ret = send_at_cmd("\r\nAT^IPCFL=14,1\r\n", "OK", 20000);
			if(ret == 1)
			{			
				gprs_status = 7;
			}
			else
			{
				gprs_status = 0;
			}
		break;
		
		case 7:
			ret = send_at_cmd("AT^SISS=1,srvType,ftp\r\n", "OK", 20000);
			if(ret == 1)
			{			
				gprs_status = 8;
			}
			else
			{
				gprs_status = 0;
			}
		break;
		
		case 8:
			ret = send_at_cmd("AT^SISS=1,conId,0\r\n", "OK", 20000);
			if(ret == 1)
			{			
				gprs_status = 9;
			}
			else
			{
				gprs_status = 0;
			}
		break;
				
		case 9:
			ret = send_at_cmd("AT^SISS=1,address,\"ftp://wzy8943:520xiao@010.3vftp.com\"\r\n", "OK", 20000);
			if(ret == 1)
			{			
				gprs_status = 10;
			}
			else
			{
				gprs_status = 0;
			}
		break;
		
		case 10:
			ret = send_at_cmd("AT^SISO=1\r\n", "OK", 20000);
			if(ret == 1)
			{			
				gprs_status = 11;
			}
			else
			{
				gprs_status = 0;
			}
		break;
	}
	
	return gprs_status;
}










