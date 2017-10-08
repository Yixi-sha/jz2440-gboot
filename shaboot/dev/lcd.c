#define GPCCON (*(volatile unsigned long *)0x56000020)
#define GPCUP (*(volatile unsigned long *)0x56000028)

#define GPDCON (*(volatile unsigned long *)0x56000030)
#define GPDUP (*(volatile unsigned long *)0x56000038)

#define GPBCON              (*(volatile unsigned long *)0x56000010)
#define GPBDAT              (*(volatile unsigned long *)0x56000014)
#define GPBUP               (*(volatile unsigned long *)0x56000018)

#define GPGCON              (*(volatile unsigned long *)0x56000060)
#define GPGDAT              (*(volatile unsigned long *)0x56000064)
#define GPGUP               (*(volatile unsigned long *)0x56000068)


// LCD CONTROLLER
#define LCDCON1     (*(volatile unsigned long *)0x4d000000) //LCD control 1
#define LCDCON2     (*(volatile unsigned long *)0x4d000004) //LCD control 2
#define LCDCON3     (*(volatile unsigned long *)0x4d000008) //LCD control 3
#define LCDCON4     (*(volatile unsigned long *)0x4d00000c) //LCD control 4
#define LCDCON5     (*(volatile unsigned long *)0x4d000010) //LCD control 5
#define LCDSADDR1   (*(volatile unsigned long *)0x4d000014) //STN/TFT Frame buffer start address 1
#define LCDSADDR2   (*(volatile unsigned long *)0x4d000018) //STN/TFT Frame buffer start address 2
#define LCDSADDR3   (*(volatile unsigned long *)0x4d00001c) //STN/TFT Virtual screen address set
#define REDLUT      (*(volatile unsigned long *)0x4d000020) //STN Red lookup table
#define GREENLUT    (*(volatile unsigned long *)0x4d000024) //STN Green lookup table 
#define BLUELUT     (*(volatile unsigned long *)0x4d000028) //STN Blue lookup table
#define DITHMODE    (*(volatile unsigned long *)0x4d00004c) //STN Dithering mode
#define TPAL        (*(volatile unsigned long *)0x4d000050) //TFT Temporary palette
#define LCDINTPND   (*(volatile unsigned long *)0x4d000054) //LCD Interrupt pending
#define LCDSRCPND   (*(volatile unsigned long *)0x4d000058) //LCD Interrupt source
#define LCDINTMSK   (*(volatile unsigned long *)0x4d00005c) //LCD Interrupt mask
#define LPCSEL      (*(volatile unsigned long *)0x4d000060) //LPC3600 Control


unsigned short PALETTE[480*272];
unsigned int fb_base_addr;
unsigned int bpp;
unsigned int xsize;
unsigned int ysize;
//#define PALETTE     0x4d000400                              //Palette start address

#define LCD_XSIZE_TFT_240320 	(240)	
#define LCD_YSIZE_TFT_240320 	(320)   //LCD显示范围


#define CLKVAL_TFT_240320	(4)
#define LCDTYPE_TFT     0x3
#define BPPMODE_16BPP   0xC
#define ENVID_DISABLE   0     //LCDCON1


#define VBPD_240320		1
#define LINEVAL_TFT_240320	(LCD_YSIZE_TFT_240320-1)
#define VFPD_240320		1
#define VSPW_240320		9  //LCDCON2

#define HBPD_240320		1
#define HOZVAL_TFT_240320	(LCD_XSIZE_TFT_240320-1)
#define HFPD_240320		1

#define HSPW_240320		40 //LCDCON4

#define FORMAT8BPP_565  1 
#define HSYNC_INV       1
#define VSYNC_INV       1
#define HWSWP           1  //LCDCON5


#define LCDFRAMEBUFFER (unsigned int)PALETTE //LCDARDDR1
#define LOWER21BITS(n)  ((n) & 0x1fffff) 

#define LINEVAL_TFT_240320	(LCD_YSIZE_TFT_240320-1) //LCDARDDR2
#define HOZVAL_TFT_240320	(LCD_XSIZE_TFT_240320-1)




void lcd_port_init(void)
{
	GPCUP = 0xffffffff;  //禁止上拉
  GPCCON  = 0xaaaaaaaa;   // GPIO管脚用于VD[7:0],LCDVF[2:0],VM,VFRAME,VLINE,VCLK,LEND 
  
  
	GPDUP   = 0xffffffff;   // 禁止内部上拉
	GPDCON  = 0xaaaaaaaa;   // GPIO管脚用于VD[23:8]
	
	GPBCON &= ~(3<<0);
	GPBCON |= (1<<0);   // 电源背光
	
}

void init_LCDCON480272(void)
{
	 /* 
         * 设置LCD控制器的控制寄存器LCDCON1~5
         * 1. LCDCON1:
         *    设置VCLK的频率：VCLK(Hz) = HCLK/[(CLKVAL+1)x2]
         *    选择LCD类型: TFT LCD   
         *    设置显示模式: 16BPP
         *    先禁止LCD信号输出
         * 2. LCDCON2/3/4:
         *    设置控制信号的时间参数
         *    设置分辨率，即行数及列数
         * 现在，可以根据公式计算出显示器的频率：
         * 当HCLK=100MHz时，
         * Frame Rate = 1/[{(VSPW+1)+(VBPD+1)+(LIINEVAL+1)+(VFPD+1)}x
         *              {(HSPW+1)+(HBPD+1)+(HFPD+1)+(HOZVAL+1)}x
         *              {2x(CLKVAL+1)/(HCLK)}]
         *            = 60Hz
         * 3. LCDCON5:
         *    设置显示模式为16BPP时的数据格式: 5:6:5
         *    设置HSYNC、VSYNC脉冲的极性(这需要参考具体LCD的接口信号): 反转
         *    半字(2字节)交换使能
         */
        LCDCON1 = (4<<8) | (LCDTYPE_TFT<<5) | \
                  (BPPMODE_16BPP<<1) | (ENVID_DISABLE<<0);
        LCDCON2 = (1<<24) | (271<<14) | \
                  (1<<6) | (9);
        LCDCON3 = (1<<19) | (479<<8) | (1);
        LCDCON4 = 40;
        LCDCON5 = (FORMAT8BPP_565<<11) | (HSYNC_INV<<9) | (VSYNC_INV<<8) | \
                  (HWSWP<<1);
        /*
         * 设置LCD控制器的地址寄存器LCDSADDR1~3
         * 帧内存与视口(view point)完全吻合，
         * 图像数据格式如下：
         *         |----PAGEWIDTH----|
         *    y/x  0   1   2       239
         *     0   rgb rgb rgb ... rgb
         *     1   rgb rgb rgb ... rgb
         * 1. LCDSADDR1:
         *    设置LCDBANK、LCDBASEU
         * 2. LCDSADDR2:
         *    设置LCDBASEL: 帧缓冲区的结束地址A[21:1]
         * 3. LCDSADDR3:
         *    OFFSIZE等于0，PAGEWIDTH等于(240*2/2)
         */
        LCDSADDR1 = ((LCDFRAMEBUFFER>>22)<<21) | LOWER21BITS(LCDFRAMEBUFFER>>1);
        LCDSADDR2 = LOWER21BITS((LCDFRAMEBUFFER+ \
                    (480)*(272)*2)>>1);
        LCDSADDR3 = (0<<11) | (480*2/2);

        /* 禁止临时调色板寄存器 */
        TPAL = 0;

        fb_base_addr = LCDFRAMEBUFFER;
        bpp = 16;
        xsize = 480;
        ysize = 272;
}

void Lcd_PowerEnable(int invpwren, int pwren)
{
    GPGCON = (GPGCON & (~(3<<8))) | (3<<8);   // GPG4用作LCD_PWREN
    GPGUP  = (GPGUP & (~(1<<4))) | (1<<4);    // 禁止内部上拉    
        
    LCDCON5 = (LCDCON5 & (~(1<<5))) | (invpwren<<5);  // 设置LCD_PWREN的极性: 正常/反转
    LCDCON5 = (LCDCON5 & (~(1<<3))) | (pwren<<3);     // 设置是否输出LCD_PWREN
}    


void Lcd_EnvidOnOff(int onoff)
{
    if (onoff == 1)
    {
        LCDCON1 |= 1;         // ENVID ON
		    GPBDAT |= (1<<0);			// Power on
    }
    else
    {
        LCDCON1 &= 0x3fffe;  // ENVID Off
	    GPBDAT &= ~(1<<0);	 // Power off
    }
} 

void PutPixel(unsigned int x, unsigned int y, unsigned int color)
{
    unsigned char red,green,blue;

    switch (bpp){
        case 16:
        {
            unsigned short *addr = (unsigned short *)fb_base_addr + (y * xsize + x);
            red   = (color >> 19) & 0x1f; // 5 BIT
            green = (color >> 10) & 0x3f; // 6 bit
            blue  = (color >>  3) & 0x1f; // 5 bit
            color = (red << 11) | (green << 5) | blue; // 格式5:6:5
            *addr = (unsigned short) color;
            break;
        }
        
        case 8:
        {
            unsigned char *addr = (unsigned char *)fb_base_addr + (y * xsize + x);
            *addr = (unsigned char) color;
            break;
        }

        default:
            break;
    }
}

void ClearScr(unsigned int color)
{   
    unsigned int x,y;
    
    for (y = 0; y < ysize; y++)
        for (x = 0; x < xsize; x++)
            PutPixel(x, y, color);
}



void DrawLine(int x1,int y1,int x2,int y2,int color)
{
    int dx,dy,e;
    dx=x2-x1; 
    dy=y2-y1;
    
    if(dx>=0)
    {
        if(dy >= 0) // dy>=0
        {
            if(dx>=dy) // 1/8 octant
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    PutPixel(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}   
                    x1+=1;
                    e+=dy;
                }
            }
            else        // 2/8 octant
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    PutPixel(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}   
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else           // dy<0
        {
            dy=-dy;   // dy=abs(dy)

            if(dx>=dy) // 8/8 octant
            {
                e=dy-dx/2;
                while(x1<=x2)
                {
                    PutPixel(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}   
                    x1+=1;
                    e+=dy;
                }
            }
            else        // 7/8 octant
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    PutPixel(x1,y1,color);
                    if(e>0){x1+=1;e-=dy;}   
                    y1-=1;
                    e+=dx;
                }
            }
        }   
    }
    else //dx<0
    {
        dx=-dx;     //dx=abs(dx)
        if(dy >= 0) // dy>=0
        {
            if(dx>=dy) // 4/8 octant
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    PutPixel(x1,y1,color);
                    if(e>0){y1+=1;e-=dx;}   
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 3/8 octant
            {
                e=dx-dy/2;
                while(y1<=y2)
                {
                    PutPixel(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}   
                    y1+=1;
                    e+=dx;
                }
            }
        }
        else           // dy<0
        {
            dy=-dy;   // dy=abs(dy)

            if(dx>=dy) // 5/8 octant
            {
                e=dy-dx/2;
                while(x1>=x2)
                {
                    PutPixel(x1,y1,color);
                    if(e>0){y1-=1;e-=dx;}   
                    x1-=1;
                    e+=dy;
                }
            }
            else        // 6/8 octant
            {
                e=dx-dy/2;
                while(y1>=y2)
                {
                    PutPixel(x1,y1,color);
                    if(e>0){x1-=1;e-=dy;}   
                    y1-=1;
                    e+=dx;
                }
            }
        }   
    }
}