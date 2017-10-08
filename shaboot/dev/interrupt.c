#define SRCPND              (volatile unsigned long *)0x4A000000
#define INTMOD              (volatile unsigned long *)0x4A000004
#define INTMSK              (volatile unsigned long *)0x4A000008
#define PRIORITY            (volatile unsigned long *)0x4A00000c
#define INTPND              (volatile unsigned long *)0x4A000010
#define INTOFFSET           (volatile unsigned long *)0x4A000014
#define SUBSRCPND           (volatile unsigned long *)0x4A000018
#define INTSUBMSK           (volatile unsigned long *)0x4A00001c

#define EINTMASK            (volatile unsigned long *)0x560000a4
#define EINTPEND            (volatile unsigned long *)0x560000a8



#define GPFDAT              (volatile unsigned long *)0x56000054

void init_irq()
{
	
	 	*(INTMSK) &= (~(1<<0)) & (~(1<<2));
	 
    __asm__( 
    /*���ж�*/  
    "mrs r0,cpsr\n"
    "bic r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"            
    : 
    : 
  );
  

}


void handle_int()
{
	unsigned long value = *(INTOFFSET);
	
	
	 switch( value )
    {
        // S2������
        case (0): 
        {   
          led_off();   // ����LEDϨ��
          
            break;
        }
        
        // S3������
        case (2):
        {   
            led_on();   // ����LEDϨ��
            break;
        }
		   case (4): //EINT4~K2
		    int_issue();
		    break;
		    
        case (31):
        {   
            ts_handle();   // ����LEDϨ��
            break;
        }
        default:
            break;
    }
	if(value == 4)
	*(EINTPEND) = (1 << 4);
	*(SRCPND) = 1 << value;
	*(INTPND) = 1 << value;
}
