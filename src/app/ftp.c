
#include <string.h>
#include "ftp.h"
#include "uart.h"
#include "flash.h"
#include "common.h"
#include "sys_tick.h"
#include "timer.h"


extern SerialBuffType  *rece_buff2;
extern uint8_t download_flag;
extern uint8_t updata_flag;


int picked_str(uint8_t *src_str, uint8_t *str1, uint8_t *str2, uint8_t *dst_str)
{
	uint8_t *tmp1 = NULL, *tmp2 = NULL, *str = src_str;
	uint8_t i = 0; 
	int index = 0;
				
	tmp1 = strstr(str, str1);

	if ((tmp1 == NULL) || (str == NULL))
	{
		return -1;
	}

	for(i=0; i<strlen(str1); i++)
	{
		tmp1++;
	}

	tmp2 = strstr(tmp1, str2);
	if (tmp2 == NULL)
	{
		return -1;
	}

	while (tmp1 != tmp2)
	{
		dst_str[index] = *tmp1++;
		index++;
	}
	dst_str[index] = '\0';
	
	return index;
}


int picked_str2(uint8_t *src_str, uint8_t *str1, uint8_t *str2, uint8_t *dst_str)
{
	uint8_t *tmp1 = NULL, *tmp2 = NULL, *str = src_str;
	uint8_t i = 0; 
	int index = 0;
		
	tmp1 = memstr(str, 512, str1);
	if ((tmp1 == NULL) || (str == NULL))
	{
		return -1;
	}

	for(i=0; i<strlen(str1); i++)
	{
		tmp1++;
	}

	tmp2 = memstr(tmp1, 512, str2);
	if (tmp2 == NULL)
	{
		return -1;
	}

	index = tmp2 - tmp1;
	memcpy(dst_str, tmp1, index);
	
	return index;
}


int picked_str3(uint8_t *src_str, uint8_t *str1, uint16_t len, uint8_t *dst_str)
{
	uint8_t *tmp1 = NULL, *tmp2 = NULL, *str = src_str;
	uint8_t i = 0; 
		
	tmp1 = memstr(str, 512, str1);
	if ((tmp1 == NULL) || (str == NULL))
	{
		return -1;
	}

	for(i=0; i<strlen(str1); i++)
	{
		tmp1++;
	}
	
	memcpy(dst_str, tmp1, len);
	
	return len;
}

uint8_t ftp_connect(void)
{
	uint8_t ret = 0;
	uint8_t tmp_data[512] = {0};

	memset(rece_buff2, 0, sizeof(SerialBuffType));
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
			USART_OUT(USART2, "AT^SISO=1\r\n");	
			delay_MS(1000);
			if (IS_TIMEOUT_1MS(tim_ftp, 50000))
			{
				ret = 0;
				break;
			}
		}	
	}	
	memset(rece_buff2, 0, sizeof(SerialBuffType));
	
	return ret;
}


uint32_t ftp_size(uint8_t *cmd, uint8_t *file)
{
	uint32_t len = 0;
	uint8_t ret = 0;
	uint8_t tmp[20] = {0};
	uint8_t tmp_str[512] = {0};

	memset(rece_buff2, 0, sizeof(SerialBuffType));
	while (ret == 0)
	{
		memset(tmp_str, 0, sizeof(tmp_str));
		receive_data(rece_buff2->data, tmp_str);
		if (strstr((char*)tmp_str, "OK") != NULL)
		{
			picked_str(tmp_str, "SIS: 0,4,213,", "\r\n", tmp);
			len = atoi(tmp);
			ret = 1;
		}
		else
		{
			memset(rece_buff2, 0, sizeof(SerialBuffType));
			USART_OUT(USART2, "\r\nAT^FTPCMD=1,%s,%s\r\n",cmd, file);
			delay_MS(3000);
			if (IS_TIMEOUT_1MS(tim_gprs, 20000))
			{
				ret = 0;
				len = 0;
				break;
			}
		}
	}	
	memset(rece_buff2, 0, sizeof(SerialBuffType));
	
	return len;
}


int ftp_read_data(uint8_t *data)
{
	uint8_t ret = 0;
	int len = 0;
	uint32_t str_len = 0; 
	uint8_t tmp_str1[512] = {0};
	uint8_t tmp[20] = {0};
	uint8_t str[30] = {0};
	
	memset(rece_buff2, 0, sizeof(SerialBuffType));
	while (ret == 0)
	{
		memset(tmp_str1, 0, sizeof(tmp_str1));
		receive_data(rece_buff2->data, tmp_str1);
		if (memstr(tmp_str1, 512, "OK") != NULL)
		{
			USART_OUT(USART1, tmp_str1);
			str_len = picked_str2(tmp_str1, "^SISR: 1,", "\r\n", tmp);
			len = atoi(tmp);
			if (len > 0)
			{
				strcat(str, "^SISR: 1,");
				strcat(str, tmp);
				strcat(str, "\r\n");
				picked_str3(tmp_str1, str, len, data);
			}
			ret = 1;
		}
		else
		{
			memset(rece_buff2, 0, sizeof(SerialBuffType));
			USART_OUT(USART2, "\r\nAT^SISR=1,256\r\n");
			delay_MS(1000);
			
			if (IS_TIMEOUT_1MS(tim_gprs, 20000))
			{
				ret = 0;
				len = -1;
				break;
			}
			
		}
		
	}
	memset(rece_buff2, 0, sizeof(SerialBuffType));

	return len;
}


// 获取数据
uint32_t ftp_get(uint8_t *cmd, uint8_t *file)
{
	uint8_t ret = 0;
	uint32_t len = 0;
	uint32_t str_len = 0;
	uint8_t tmp_data[20] = {0};
	uint8_t tmp_str[512] = {0};
	
	memset(rece_buff2, 0, sizeof(SerialBuffType));
	while (ret == 0)
	{
		memset(tmp_str, 0, sizeof(tmp_str));
		receive_data(rece_buff2->data, tmp_str);
		if (strstr((char*)tmp_str, "OK") != NULL)
		{
			ret = 1;
		}
		else
		{
			memset(rece_buff2, 0, sizeof(SerialBuffType));
			USART_OUT(USART2, "\r\nAT^FTPCMD=1,%s,%s\r\n", cmd, file);
			delay_MS(6000);
			if (IS_TIMEOUT_1MS(tim_gprs, 20000))
			{
				ret = 0;
				break;
			}
		}
	}
	memset(rece_buff2, 0, sizeof(SerialBuffType));

	return ret;
}


uint8_t ftp_download_app(void)
{
	uint8_t ftp_get_status = 0;
	uint32_t total_len = 0;
	uint32_t len = 0;
	uint8_t data[256] = {0};
	uint32_t app_size = 0;
	static uint32_t tmp_addr = STORAGE_ADDR;
	
	app_size = ftp_size("SIZE", "led.bin");
	if (app_size == -1)
	{
		return 0;
	}
		
	ftp_get_status = ftp_get("GET", "led.bin");
	if (ftp_get_status != 1)
	{
		return 0;
	}
	
	while (1)
	{
		len = ftp_read_data(data);
		total_len += len;
		USART_OUT(USART1, "\r\ntotal_len = %d\r\n", total_len);
		if (len > 0)
		{
			flash_write_data(tmp_addr, data, len);
			tmp_addr += len;
			len = 0;
		}
		else if ((len == 0) && (app_size == total_len))
		{
			return 1;
		}
		else if ((len == 0) && (app_size != total_len))
		{
			return 0;
		}
	}
}


uint8_t ftp_download_ini(void)
{
	uint8_t tmp[20] = {0};
	uint32_t total_len = 0;
	uint32_t len = 0;
	uint8_t data[256] = {0};
	uint32_t ini_size = 0;
	static uint32_t tmp_addr = PRODUCT_INFO_ADDR;

	ini_size = ftp_size("SIZE", "ftp.ini");
	ftp_get("GET", "ftp.ini");
	
	len = ftp_read_data(data);
	if(len > 0)
	{
		picked_str(data, "download_flag=", "\r\n", tmp);
		download_flag = atoi(tmp);
		
		if(download_flag == 1)
		{
			flash_erase(PRODUCT_INFO_ADDR, PRODUCT_INFO_SIZE);
		}
	}
	
	while (1)
	{	
		total_len += len;
		if (len > 0)
		{
			if(download_flag == 1)
			{
				flash_write_data(tmp_addr, data, len);
				tmp_addr += len;
				len = 0;
			}
		}
		else if ((len == 0) && (ini_size == total_len))
		{
			return 1;
		}
		else if ((len == 0) && (ini_size != total_len))
		{
			return 0;
		}
		
		len = ftp_read_data(data);
	}
}
















