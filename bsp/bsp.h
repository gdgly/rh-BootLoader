/*************************************************
  Copyright (C), RoadLighting Tech. Co., Ltd.
  File name: bsp.h     
  Author: George      Version: 1.0        Date: 2015/08/20
  Description:  头文件
  Others: 无     
  History:                          
    1. Date:
       Author:
       Modification:
*************************************************/
#ifndef _BSP_H
#define _BSP_H
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_flash.h>
#include <misc.h>



void rcc_config(void);
void bsp_init(void);
#endif



