#define GPFCON (volatile unsigned long*)0x56000050
#define GPFDAT (volatile unsigned long*)0x56000054
#define GPFUP  (volatile unsigned long*)0x56000058

#define GPIF0_msk (3<<(0*2))
#define GPIF2_msk (3<<(2*2))

#define GPF0_int  (0x2<<(0*2))
#define GPF2_int  (0x2<<(2*2))

void button_init()
{
	*(GPFCON) &= ~(GPIF0_msk|GPIF2_msk);
	*(GPFCON) |= GPF0_int|GPF2_int;
}