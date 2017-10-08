#define NFCONF (*(volatile unsigned long*)0x4E000000)
#define NFCONT (*(volatile unsigned long*)0x4E000004)
#define NFCMD  (*(volatile unsigned char*)0x4E000008)
#define NFADDR (*(volatile unsigned char*)0x4E00000C)
#define NFDATA (*(volatile unsigned char*)0x4E000010)
#define NFSTAT (*(volatile unsigned char*)0x4E000020)


#define TACLS 1
#define TWRPH0 2
#define TWRPH1 1


void select_chip(void)
{
		NFCONT &= ~(1<<1); 
}


void deselect_chip()
{
		NFCONT |= (1<<1); 
}


void send_cmd(unsigned cmd)
{
	NFCMD = cmd;
}


void clear_RnB()
{
	NFSTAT |= (1<<2);
}


void send_addr(unsigned addr)
{
	NFADDR = addr;
}

void wait_RnB()
{
	while(!(NFSTAT&(1<<2)))
	{
		;
	}
}

void nand_reset(void)
{
	//选中nandflash
  select_chip();
	//清除忙信号
	clear_RnB();
	//发送ffh
	send_cmd(0xff);
	//等待忙
		wait_RnB();
	//取消选中nandflash
	deselect_chip();
}
void nand_init(void)
{
    //初始化NFCONF
    NFCONF = (TACLS<<12) | (TWRPH0<<8) | (TWRPH1<<4);
    
    //初始化NFCONT
    NFCONT = (1<<0) | (1<<1);
    
    //复位
    nand_reset();	
	
}


void NF_PageRead(unsigned long addr,unsigned char* buff)
{
	int i;
	
	//选中nandflash
  select_chip();
	//清除忙信号
	clear_RnB();
	
	//发送命令ooh
	send_cmd(0x00);
	//列地址(2)  偏移地址
	send_addr(0x00);
	send_addr(0x00);
	//行地址(3)
	send_addr(addr&0xff);
	send_addr((addr>>8)&0xff);
	send_addr((addr>>16)&0xff);
	//发送命令30h
	send_cmd(0x30);
	//等待忙
	wait_RnB();
	//读取数据
	
	for(i=0;i<2048;i++)
	{
		buff[i] = NFDATA;
	}
	//取消选中nandflash
	deselect_chip();
}

void nand_to_ram(unsigned long start_addr,unsigned char* sdram_addr,int size)
{
	int i;
	for(i=(start_addr>>11);size>0;)
	{
		NF_PageRead(i,sdram_addr);
		sdram_addr +=2048;
		size-=2048;
		i++;
	}
}


int NF_write(unsigned long addr,unsigned char* buff)
{
	int i,ret;
	
	//选中nandflash
  select_chip();
	//清除忙信号
	clear_RnB();
	//发送命令80h
	send_cmd(0x80);
	//发送列地址	
	send_addr(0x00);
	send_addr(0x00);
	//发送行地址
	send_addr(addr&0xff);
	send_addr((addr>>8)&0xff);
	send_addr((addr>>16)&0xff);
	//写入数据
	for(i=0;i<2048;i++)
	{
		NFDATA = buff[i];
	}
	//发送命令10h
	send_cmd(0x10);
	//等待RnB
	wait_RnB();
	//读取写入结果
	ret = NFDATA;
	//取消选中nandflash
	deselect_chip();
	
	return ret;
}

int NF_Erase(unsigned addr)
{
	int ret;
	//选中nandflash
  select_chip();
	//清除忙信号
	clear_RnB();
	//发送命令60h
	send_cmd(0x60);
	//发送行地址
	send_addr(addr&0xff);
	send_addr((addr>>8)&0xff);
	send_addr((addr>>16)&0xff);
	//发送命令d0h
	send_cmd(0xd0);
	//等待忙
	wait_RnB();
	//发送命令70h
	send_cmd(0x70);
	//查看擦除结果
	ret = NFDATA;
	
	//取消选中nandflash
	deselect_chip();
	
	return ret;
}