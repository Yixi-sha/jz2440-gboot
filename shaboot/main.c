
//char *buf = "Hello World";


int shaboot_main()
{
	 int num;
//	unsigned char buff[2048];
//  mmu_init();
	
    led_init();
    button_init();
    init_irq();
    
    uart_init();
    
//    dma_init();
    
    dma_start();
//   ts_init();


/*    lcd_port_init();                     // 设置LCD引脚
    init_LCDCON480272(); // 初始化LCD控制器
    Lcd_PowerEnable(0, 1);               // 设置LCD_PWREN有效，它用于打开LCD的电源
    Lcd_EnvidOnOff(1);                   // 使能LCD控制器输出信号
    
 
    ClearScr(0x4567);  // 清屏，黑色

    DrawLine(0  , 0  , 479, 0  , 0xff0000);    // 红色
    DrawLine(0  , 0  , 0  , 271, 0x00ff00);    // 绿色
    DrawLine(479, 0  , 479, 271, 0x0000ff);    // 蓝色
    DrawLine(0  , 271, 479, 271, 0xffffff);    // 白色
    DrawLine(0  , 0  , 479, 271, 0xffff00);    // 黄色
    DrawLine(479, 0  , 0  , 271, 0x8000ff);    // 紫色
    DrawLine(240, 0  , 240, 271, 0xe6e8fa);    // 银色
    DrawLine(0  , 136, 479, 136, 0xcd7f32);    // 金色
    
    ClearScr(0x0);  // 清屏，黑色

    DrawLine(0  , 0  , 479, 0  , 0xff0000);    // 红色
    DrawLine(0  , 0  , 0  , 271, 0x00ff00);    // 绿色
    DrawLine(479, 0  , 479, 271, 0x0000ff);    // 蓝色
    DrawLine(0  , 271, 479, 271, 0xffffff);    // 白色
    DrawLine(0  , 0  , 479, 271, 0xffff00);    // 黄色
    DrawLine(479, 0  , 0  , 271, 0x8000ff);    // 紫色
    DrawLine(240, 0  , 240, 271, 0xe6e8fa);    // 银色
    DrawLine(0  , 136, 479, 136, 0xcd7f32);    // 金色
    */
    
    dm9000_init();
//    dm9000_arp();
		
  
//   uart_send_string(buf);
//    
    

   
    while(1)
    {
    	
			printf("\n***************************************\n\r");
    	printf("\n*****************SHABOOT*****************\n\r");
    	printf("1:Download Linux Kernel from TFTP Server!\n\r");
    	printf("2:Boot Linux from RAM!\n\r");
    	printf("3:Boor Linux from Nand Flash!\n\r");
    	printf("\n Plese Select:");
    	
    	scanf("%d",&num);
    
        switch (num)
        {
            case 1:
            	//tftp_load();
            	
            	arp_request();
            	break;
            
            case 2:
            	//boot_linux_ram();
            	tftp_send_request("zImage");
            break;
            
            case 3:
            //boot_linux_nand();
            boot_linux();
            break;
            
            default:
                printf("Error: wrong selection!\n\r");
            break;	
        }
    	
    }
    return 0;    
}
