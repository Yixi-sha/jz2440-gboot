#define MMU_SECTINO       (2<<0)
#define MMU_CACHEENABLE   (1<<3)
#define MMU_BUFFUER       (1<<2)
#define MMU_SPECIAL       (1<<4)
#define MMU_FULL_ACCESS   (3<<10)
#define MMU_DOMAIN        (0<<5)

#define SECTION    (MMU_SECTINO|MMU_SPECIAL|MMU_DOMAIN|MMU_FULL_ACCESS)
#define SECTION_WB (MMU_SECTINO|MMU_SPECIAL|MMU_DOMAIN|MMU_FULL_ACCESS|MMU_BUFFUER|MMU_CACHEENABLE)

void creat_page_table()
{
    unsigned long *ttb = (unsigned long *)0x30000000;
    unsigned long vaddr, paddr;

    vaddr = 0xA0000000;
    paddr = 0x56000000;
    *(ttb + (vaddr >> 20)) = (paddr & 0xFFF00000) | SECTION;

    vaddr = 0x30000000;
    paddr = 0x30000000;
    while (vaddr < 0x34000000)
    {
        *(ttb + (vaddr >> 20)) = (paddr & 0xFFF00000) | SECTION_WB;
        vaddr += 0x100000;
        paddr += 0x100000;
    }
}
void mmu_enable()
{
	   __asm__(
    
    /*设置TTB*/
    "ldr    r0, =0x30000000\n"                  
    "mcr    p15, 0, r0, c2, c0, 0\n"  
    
    /*不进行权限检查*/
    "mvn    r0, #0\n"                   
    "mcr    p15, 0, r0, c3, c0, 0\n"   
    
   /*使能MMU*/
    "mrc    p15, 0, r0, c1, c0, 0\n"    
    "orr    r0, r0, #0x0001\n"          
    "mcr    p15, 0, r0, c1, c0, 0\n"    
    : 
    : 
  );
}
void mmu_init()
{
	creat_page_table();
	mmu_enable();
}
