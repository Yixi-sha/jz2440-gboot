#define GPFCON (volatile unsigned long*)0x56000050
#define GPFDAT (volatile unsigned long*)0x56000054
#define GPFUP  (volatile unsigned long*)0x56000058


void led_init()
{
		*(GPFCON) = 0x1500;
		*(GPFUP) = 0x0;
}

void led_on()
{
	  *(GPFDAT) |= (0x7<<4);   // ����LEDϨ��
	  *(GPFDAT) &= ~(1<<4);      // LED1����;
    *(GPFDAT) &= ~(1<<6);      // LED1����;
}

void led_off()
{
	 *(GPFDAT) |= (0x7<<4);   // ����LEDϨ��

}
	
