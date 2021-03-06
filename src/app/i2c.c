
#include "i2c.h"
#include "common.h"
#include "bsp.h"
#include "sys_tick.h"


/*
** 
** 描述：i2c 开始信号
**
**
*/ 
bool i2c_start(void)
{
	SDA_HIGH();
	SCL_HIGH();
	delay(5);
	if(!SDA_READ())
	{
		return FALSE;		//SDA线为低电平则总线忙,退出
	}
	SDA_LOW();
	if(SDA_READ()) 
	{
		return FALSE; 		//SDA线为高电平则总线出错,退出
	}
	SDA_LOW();
	delay(5);
	return TRUE;
}

/*
** 
** 描述：i2c 停止信号
**
**
*/ 
void i2c_stop(void)
{
	SCL_LOW();
	delay(5);
	SDA_LOW();
	delay(5);
	SCL_HIGH();
	SDA_HIGH();
	delay(5);
}

/*
** 
** 描述：i2c 等待应答
**
**
*/ 
bool i2c_wait_ack(void) //返回为:=1有ACK,=0无ACK
{
	SCL_LOW();
	delay(5);
	SDA_HIGH();
	delay(5);
	SCL_HIGH();
	delay(5);
	if(SDA_READ())
	{
		SCL_LOW();
		return FALSE;
	}
	SCL_LOW();
	return TRUE;
}

/*
** 
** 描述：i2c 发送应答位
**
**
*/ 
void i2c_ack(void)
{
	SCL_LOW();
	SDA_LOW();
	delay(5);
	SCL_HIGH();
	delay(5);
	SCL_LOW();
	delay(5);
}

/*
** 
** 描述：i2c 无应答
**
**
*/ 
void i2c_no_ack(void)
{ 
	SCL_LOW();
	SDA_HIGH();
	delay(5);
	SCL_HIGH();
	delay(5);
	SCL_LOW();
	delay(5);
}

/*
** 
** 描述：i2c 发送一个字节
**
**
*/ 
void i2c_send_byte(uint8_t byte) //数据从高位到低位//
{
	uint8_t i = 8;
	while (i--)
	{
		SCL_LOW();
		delay(5);
		if (byte&0x80)
		{
			SDA_HIGH();
		}
		else 
		{
			SDA_LOW();
		}
		byte<<=1;
		SCL_HIGH();
		delay(5);
	}
	
	SCL_LOW();
}


/*
** 
** 描述：i2c 接收一个字节
**
**
*/ 
uint8_t i2c_rece_byte(void)    //数据从高位到低位//
{ 
	uint8_t i=8;
	uint8_t byte=0;

	SDA_HIGH();
	while (i--)
	{
		byte <<= 1;
		SCL_LOW();
		delay(5);
		SCL_HIGH();
		if (SDA_READ())
		{
			byte |= 0x01;
		}
	}
	SCL_LOW();
	return byte;
}


/////////////////////////////////////////////////////////////////////////

/*
** 函数名：ee_byte_write
** 描述：写一个字节到EEPROM的某个地址
** 参数：
	dev_addr：设备地址
	ee_addr：设备EEPROM地址
	ch：要写入的数据
**
*/ 
bool ee_byte_write(uint8_t dev_addr, uint16_t ee_addr, uint8_t ch)
{
	uint8_t addr = 0;
	
	addr = dev_addr<<1;
	
	if (!i2c_start()) 
		return FALSE;
	
	i2c_send_byte(addr);		//设置器件地址+段地址 
	if (!i2c_wait_ack())
	{
		i2c_stop(); 
		return FALSE;
	}
	
	i2c_send_byte(ee_addr>>8);	//设置段内地址
	i2c_wait_ack();
	
	i2c_send_byte(ee_addr&0xFF);	//设置段内地址
	i2c_wait_ack();
	
	i2c_send_byte(ch);
	i2c_wait_ack();
	
	i2c_stop();
	
	delay_1ms(5);
	return TRUE;
}

/*
** 函数名：ee_random_read
** 描述: 从EEPROM某个地址读一个字节的数据
** 参数：
	dev_addr：设备地址
	ee_addr：设备EEPROM地址
	ch：要写入的数据
**
*/ 
uint8_t ee_random_read(uint8_t dev_addr, uint16_t ee_addr)
{
	uint8_t ch = 0;
	uint8_t addr_write = 0, addr_read = 0;
		
	addr_write = dev_addr<<1;
	addr_read = (dev_addr<<1) + 1;
	
	if (!i2c_start()) 
		return FALSE;
	
	i2c_send_byte(addr_write);		//设置器件地址+段地址 
	if (!i2c_wait_ack())
	{
		i2c_stop(); 
		return FALSE;
	}

	i2c_send_byte(ee_addr>>8);  //设置高地址
	i2c_wait_ack();
	i2c_send_byte(ee_addr&0xFF);  //设置低地址
	i2c_wait_ack();
	
	i2c_start();
	i2c_send_byte(addr_read);
	i2c_wait_ack();
		
	ch = i2c_rece_byte();
	i2c_no_ack();
				
	i2c_stop();
	
	return ch;	
}


/*
** 函数名：ee_random_read
** 描述: 从EEPROM某个地址开始写入len个长度的数据data
** 参数：
	dev_addr：设备地址
	ee_addr：设备EEPROM地址
	data：要写入的数�
	len：写入数据的长度
**
*/ 
bool ee_page_write(uint8_t dev_addr, uint16_t ee_addr, uint8_t *data, uint16_t len)
{
	uint8_t addr = 0;
	
	addr = dev_addr<<1;
	
	if (!i2c_start()) 
		return FALSE;
	
	i2c_send_byte(addr);		//设置器件地址+段地址 
	if (!i2c_wait_ack())
	{
		i2c_stop(); 
		return FALSE;
	}
	
	i2c_send_byte(ee_addr>>8);	//设置段内地址
	i2c_wait_ack();
	
	i2c_send_byte(ee_addr&0xFF);	//设置段内地址
	i2c_wait_ack();
	
	while(len--)
	{
		i2c_send_byte(*data);
		i2c_wait_ack();
		data++;
	}
	i2c_stop();
	
	delay_1ms(5);
	return TRUE;
}

/*
** 函数名：ee_sequential_read
** 描述: 从EEPROM某个地址开始读取len长度的数据
** 参数：
	dev_addr：设备地址
	ee_addr：设备EEPROM地址
	data：读出的数据
	len：要读取的数据长度
**
*/ 
bool ee_sequential_read(uint8_t dev_addr, uint16_t ee_addr, uint8_t *data, uint16_t len)
{	
	uint8_t addr_write = 0, addr_read = 0;
		
	addr_write = dev_addr<<1;
	addr_read = (dev_addr<<1) + 1;
	
	if (!i2c_start()) 
		return FALSE;
	
	i2c_send_byte(addr_write);		//设置器件地址+段地址 
	if (!i2c_wait_ack())
	{
		i2c_stop(); 
		return FALSE;
	}

	i2c_send_byte(ee_addr>>8);  //设置高地址
	i2c_wait_ack();
	i2c_send_byte(ee_addr&0xFF);  //设置低地址
	i2c_wait_ack();
	
	i2c_start();
	i2c_send_byte(addr_read);
	i2c_wait_ack();
	
	while(len)
	{
		*data = i2c_rece_byte();
		if(len == 1)
		{		
			i2c_no_ack();
		}
		else
		{
			i2c_ack();
		}
		data++;
		len--;
	}
	i2c_stop();
	
	return TRUE;
}


