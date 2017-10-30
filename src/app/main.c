/*************************************************
  Copyright (C), RoadLighting Tech. Co., Ltd.
  File name: main.c     
  Author: George      Version: 1.0        Date: 2015/08/20
  Description:  
  Others: 无        
  Function List:  
    1. jump_to_app
	2. jump_to_ftp
	3. main
  History:                          
    1. Date:
       Author:
       Modification:
*************************************************/
#include <string.h>
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include "bsp.h"
#include "flash.h"
#include "updata.h"

typedef  void (*FunVoidType)(void);

uint32_t jump_addr;
static FunVoidType jump_to_application;


/*
*Function: jump_to_app      
*Description: 跳转到app程序
*Calls: 无  
*Data Accessed: 
	jump_addr
	jump_to_application
*Data Updated:    
*Input: 无
*Output: 无
*Return: void     
*Others: 如果跳转成功，执行完该函数后bootloader程序结束，跳转到应用程序执行，如果跳转不成功，
执行完该函数后继续执行本程序
*/
void jump_to_app(void)
{
	if(((*(volatile uint32_t*)APP_ADDR) & 0x2FFE0000) == 0x20000000) 	// 检查栈顶地址是否合法
	{
		jump_addr = *(volatile uint32_t*)(APP_ADDR + 4);	
		jump_to_application = (FunVoidType)jump_addr;
		__set_MSP(*(volatile uint32_t*)APP_ADDR);  // 初始化APP堆栈指针		
		jump_to_application();
	}
}

/*
*Function: jump_to_ftp      
*Description: 跳转到ftp程序  
*Calls: 无  
*Data Accessed: 
	jump_addr
	jump_to_application
*Data Updated:    
*Input: 无
*Output: 无
*Return: void    
*Others: 如果跳转成功，执行完该函数后bootloader程序结束，跳转到应用程序执行，如果跳转不成功，
执行完该函数后继续执行本程序
*/
void jump_to_ftp(void)
{
	if(((*(volatile uint32_t*)FTP_CLINET_ADDR) & 0x2FFE0000) == 0x20000000) 	// 检查栈顶地址是否合法
	{
		jump_addr = *(volatile uint32_t*)(FTP_CLINET_ADDR + 4);
		jump_to_application = (FunVoidType)jump_addr;
		__set_MSP(*(volatile uint32_t*)FTP_CLINET_ADDR); // 初始化APP堆栈指针	
		jump_to_application();
	}
}	
	
/*
*Function: main      
*Description: 主函数  
*Calls: 无  
*Data Accessed: updata_flag：更新标志位 
*Data Updated: updata_flag    
*Input: 无
*Output: 无
*Return: 无     
*Others: 无    
*/
int main(void)
{
	uint8_t status = 0;
	uint8_t is_updata_flag = 0, download_finish_flag = 0;
	uint8_t check_sum=0;
	uint32_t app_size=0;
	
	bsp_init();		
	

	is_updata_flag = flash_read_byte(FLAG_BASE_ADDR+IS_UPDATA_FLAG_OFFSET);
	download_finish_flag = flash_read_byte(FLAG_BASE_ADDR+DOWNLOAD_FINISH_FLAG_OFFSET);
	

	while (1)
  	{	
		
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2))
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_2);
		}
		else
		{			
			GPIO_SetBits(GPIOB, GPIO_Pin_2);
		}
				
		switch(status)
		{
			case 0:
				if (is_updata_flag == 1)	//需要升级
				{
					status++;
				}	
				else//
				{
					status = 244;
				}
			break;

			case 1://
				if (download_finish_flag == 1)
				{
					status++;
				}
				else
				{
					status = 255;	//跳转到FTP区，下载数据
				}
			break;
			
			case 2:	
				if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2))
				{
					GPIO_ResetBits(GPIOB, GPIO_Pin_2);
				}
				else
				{			
					GPIO_SetBits(GPIOB, GPIO_Pin_2);
				}
				
				flash_page_erase(APP_ADDR, APP_SECTOR_SUM);//擦除APP应用程序区
				
				// 获取应用程序的大小				
				app_size = get_app_size(APP_INFO_BASE_ADDR+APP_SIZE_INFO_OFFSET, APP_SIZE_INFO_SIZE);
				check_sum = updata_app(APP_ADDR, STORAGE_ADDR, app_size);//从FLASH存储区更新到APP程序区
				status++;//
			break;	
		
			case 3://校验写入的数据
				if(check_sum == get_app_check_sum(APP_ADDR, app_size))//校验成功
				{
					status++;//
				}
				else
				{
					status--;//返回上一步继续操作。
				}
			
			break;
			
			case 4:// 清标志位
				flash_page_erase(FLAG_BASE_ADDR, FLAG_SECTOR_SUM);
				flash_write_byte(FLAG_BASE_ADDR+IS_UPDATA_FLAG_OFFSET, 0x00);
				flash_write_byte(FLAG_BASE_ADDR+DOWNLOAD_FINISH_FLAG_OFFSET, 0x00);
				status = 244;	
			break;
			
			case 244:	//跳转到APP程序区	
				if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2))
				{
					GPIO_ResetBits(GPIOB, GPIO_Pin_2);
				}
				else
				{			
					GPIO_SetBits(GPIOB, GPIO_Pin_2);
				}
				jump_to_app();	
				status = 255;
			break;
			
			case 255://跳转到FTP区
				jump_to_ftp();
			break;
			
			default:
			break;
		}
	}
}






