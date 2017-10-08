#include "s3c2440.h" 

#define PCLK 50000000
#define BAUD 115200

void uart_init()
{
	//配置引脚功能
	GPHCON &= ~(0xf<<4);
	GPHCON |= (0xa<<4);
	
	//设置数据格式
	ULCON0 = 0x3;
	
	//设置工作模式
	UCON0 = 0x9;
	//设置波特率
 UBRDIV0 =(int)(PCLK/(BAUD*16)-1);
	
}



void putc(unsigned char ch)
{
	while(!(UTRSTAT0 & (1<<2)));
	UTXH0 = ch;
}

unsigned char getc(void)
{
    unsigned char ret;

	while (!(UTRSTAT0 & (1<<0)));
	// 取数据
	ret = URXH0;  
	
	if ( (ret == 0x0d) || (ret == 0x0a) )
	{
	    putc(0x0d);
	    putc(0x0a);	
	}      	
	else
	    putc(ret);

        return ret;
}

void uart_send_string(unsigned char* str)
{
	while(*str)
	{
		putc(*str++);
	}
	
	putc(0x0d);
	putc(0x0a);
}