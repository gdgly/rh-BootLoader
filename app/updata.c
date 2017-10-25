/*************************************************
  Copyright (C), RoadLighting Tech. Co., Ltd.
  File name: updata.c     
  Author: George      Version: 1.0        Date: 2015/08/20
  Description:  Flash操作
  Others: 无        
  Function List:  
    1. get_app_size
	2. updata_app
	3. get_app_check_sum

  History:                           
    1. Date:
       Author:
       Modification:
*************************************************/
#include <string.h>
#include <stdlib.h>
#include "updata.h"
#include "flash.h"

/*
*Function: get_app_size      
*Description: 从Flash中获取app的大小   
*Calls:  
	flash_read_data
*Data Accessed: 无 
*Data Updated:  无  
*Input: void     
*Output: 无 
*Return: app_zise：从flash中app信息区读取app的大小        
*Others: 无    
*/
uint32_t get_app_size(uint32_t addr, uint32_t len)
{
	uint32_t app_size = 0;
	uint8_t tmp[10] = {0};
	
	flash_read_data(addr, tmp, len);
	app_size = atoi((char*)tmp);
	
	return app_size;
}

/*
*Function: updata_app      
*Description: 更新应用程序，并获取校验和  
*Calls:   
	get_app_size
*Data Accessed: 无  
*Data Updated: 无    
*Input: void     
*Output: 无 
*Return: void        
*Others: 无    
*/
uint8_t updata_app(uint32_t dst_addr, uint32_t src_addr, uint32_t len)
{
	uint32_t i = 0;
	uint8_t data[2] = {0};
	uint8_t check_sum = 0;
	
	for(i=0; i<len; i+=2)
	{
		flash_read_data(src_addr+i, data, 2);
		flash_write_data(dst_addr+i, data, 2);
		check_sum += data[0];
		check_sum += data[1];
	}

	return check_sum;
}


/*
*Function: get_app_check_sum      
*Description: 校验应用程序   
*Calls:  
	get_app_size
*Data Accessed: 
*Data Updated:    
*Input: void     
*Output: 无 
*Return: 成功TRUE  失败FALSH       
*Others: 无    
*/
uint8_t get_app_check_sum(uint32_t app_addr, uint32_t app_size)
{
	uint32_t i = 0;
	uint8_t data[2] = {0};
	uint8_t check_sum = 0; 

	for (i=0; i<app_size; i+=2)
	{
		flash_read_data(app_addr+i, data, 2);
		check_sum += data[0];
		check_sum += data[1];
	}
	
	return check_sum;
}



