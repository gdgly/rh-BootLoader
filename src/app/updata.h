/*************************************************
  Copyright (C), RoadLighting Tech. Co., Ltd.
  File name: updata.h     
  Author: George      Version: 1.0        Date: 2015/08/20
  Description:  updata.c 头文件
  Others: 无        
  History:                       
    1. Date:
       Author:
       Modification:
*************************************************/
#ifndef _UPDATA_H
#define _UPDATA_H
#include <stm32f10x.h>


uint32_t get_app_size(uint32_t addr, uint32_t len);
uint8_t updata_app(uint32_t dst_addr, uint32_t src_addr, uint32_t len);
uint8_t get_app_check_sum(uint32_t app_addr, uint32_t app_size);
#endif
