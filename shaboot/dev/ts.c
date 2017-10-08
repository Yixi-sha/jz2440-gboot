#define ADCCON    (*(volatile unsigned *)0x58000000)   //ADC control
#define ADCTSC    (*(volatile unsigned *)0x58000004)   //ADC touch screen control
#define ADCDLY    (*(volatile unsigned *)0x58000008)   //ADC start or Interval Delay
#define ADCDAT0   (*(volatile unsigned *)0x5800000c)   //ADC conversion data 0
#define ADCDAT1   (*(volatile unsigned *)0x58000010)   //ADC conversion data 1

/*interrupt registes*/
#define SRCPND              (*(volatile unsigned long *)0x4A000000)
#define INTMSK              (*(volatile unsigned long *)0x4A000008)
#define INTOFFSET           (*(volatile unsigned long *)0x4A000014)
#define SUBSRCPND           (*(volatile unsigned long *)0x4A000018)
#define INTSUBMSK           (*(volatile unsigned long *)0x4A00001c)
#define INTPND              (*(volatile unsigned long *)0x4A000010)
int xdata,ydata;
void ts_init(void)
{
	//设置AD转换时钟
	ADCCON = (1<<14)|(49<<6);
	//设置中断屏蔽
	
    INTMSK = ~(1<<31);
    INTSUBMSK = ~(1<<9);
	//进入等待中断模式
	
	ADCTSC =  0xd3;
	
}

void ts_handle()
{
	//启动XY坐标自动转换
	ADCTSC = (1<<2);
	ADCCON |= (1<<0);
	//等待转换完成
	while(!(ADCCON & (1<<15)));
	//获取坐标
	xdata = ADCDAT0 & (0x3ff);
	
	ydata = ADCDAT1 & (0x3ff);
	//清除中断
    SUBSRCPND |= (1<<9);
    SRCPND = (1<<31);
    INTPND = (1<<31);
	//等待弹起TS中断
	ADCTSC =  0xd3;
	ADCTSC|= (1<<8);
	
	while(1)
    {
        if(SUBSRCPND &(1<<9))	
    	    break;
    }
	//清除弹起中断
	  SUBSRCPND |= (1<<9);
    SRCPND = (1<<31);
    INTPND = (1<<31); 
    
    printf("x is %d,y is %d\n",xdata,ydata);
	//等待按下中断
	ADCTSC = 0xd3;
}