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
	//����ADת��ʱ��
	ADCCON = (1<<14)|(49<<6);
	//�����ж�����
	
    INTMSK = ~(1<<31);
    INTSUBMSK = ~(1<<9);
	//����ȴ��ж�ģʽ
	
	ADCTSC =  0xd3;
	
}

void ts_handle()
{
	//����XY�����Զ�ת��
	ADCTSC = (1<<2);
	ADCCON |= (1<<0);
	//�ȴ�ת�����
	while(!(ADCCON & (1<<15)));
	//��ȡ����
	xdata = ADCDAT0 & (0x3ff);
	
	ydata = ADCDAT1 & (0x3ff);
	//����ж�
    SUBSRCPND |= (1<<9);
    SRCPND = (1<<31);
    INTPND = (1<<31);
	//�ȴ�����TS�ж�
	ADCTSC =  0xd3;
	ADCTSC|= (1<<8);
	
	while(1)
    {
        if(SUBSRCPND &(1<<9))	
    	    break;
    }
	//��������ж�
	  SUBSRCPND |= (1<<9);
    SRCPND = (1<<31);
    INTPND = (1<<31); 
    
    printf("x is %d,y is %d\n",xdata,ydata);
	//�ȴ������ж�
	ADCTSC = 0xd3;
}