
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


/*    lcd_port_init();                     // ����LCD����
    init_LCDCON480272(); // ��ʼ��LCD������
    Lcd_PowerEnable(0, 1);               // ����LCD_PWREN��Ч�������ڴ�LCD�ĵ�Դ
    Lcd_EnvidOnOff(1);                   // ʹ��LCD����������ź�
    
 
    ClearScr(0x4567);  // ��������ɫ

    DrawLine(0  , 0  , 479, 0  , 0xff0000);    // ��ɫ
    DrawLine(0  , 0  , 0  , 271, 0x00ff00);    // ��ɫ
    DrawLine(479, 0  , 479, 271, 0x0000ff);    // ��ɫ
    DrawLine(0  , 271, 479, 271, 0xffffff);    // ��ɫ
    DrawLine(0  , 0  , 479, 271, 0xffff00);    // ��ɫ
    DrawLine(479, 0  , 0  , 271, 0x8000ff);    // ��ɫ
    DrawLine(240, 0  , 240, 271, 0xe6e8fa);    // ��ɫ
    DrawLine(0  , 136, 479, 136, 0xcd7f32);    // ��ɫ
    
    ClearScr(0x0);  // ��������ɫ

    DrawLine(0  , 0  , 479, 0  , 0xff0000);    // ��ɫ
    DrawLine(0  , 0  , 0  , 271, 0x00ff00);    // ��ɫ
    DrawLine(479, 0  , 479, 271, 0x0000ff);    // ��ɫ
    DrawLine(0  , 271, 479, 271, 0xffffff);    // ��ɫ
    DrawLine(0  , 0  , 479, 271, 0xffff00);    // ��ɫ
    DrawLine(479, 0  , 0  , 271, 0x8000ff);    // ��ɫ
    DrawLine(240, 0  , 240, 271, 0xe6e8fa);    // ��ɫ
    DrawLine(0  , 136, 479, 136, 0xcd7f32);    // ��ɫ
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
