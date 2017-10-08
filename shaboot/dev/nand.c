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
	//ѡ��nandflash
  select_chip();
	//���æ�ź�
	clear_RnB();
	//����ffh
	send_cmd(0xff);
	//�ȴ�æ
		wait_RnB();
	//ȡ��ѡ��nandflash
	deselect_chip();
}
void nand_init(void)
{
    //��ʼ��NFCONF
    NFCONF = (TACLS<<12) | (TWRPH0<<8) | (TWRPH1<<4);
    
    //��ʼ��NFCONT
    NFCONT = (1<<0) | (1<<1);
    
    //��λ
    nand_reset();	
	
}


void NF_PageRead(unsigned long addr,unsigned char* buff)
{
	int i;
	
	//ѡ��nandflash
  select_chip();
	//���æ�ź�
	clear_RnB();
	
	//��������ooh
	send_cmd(0x00);
	//�е�ַ(2)  ƫ�Ƶ�ַ
	send_addr(0x00);
	send_addr(0x00);
	//�е�ַ(3)
	send_addr(addr&0xff);
	send_addr((addr>>8)&0xff);
	send_addr((addr>>16)&0xff);
	//��������30h
	send_cmd(0x30);
	//�ȴ�æ
	wait_RnB();
	//��ȡ����
	
	for(i=0;i<2048;i++)
	{
		buff[i] = NFDATA;
	}
	//ȡ��ѡ��nandflash
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
	
	//ѡ��nandflash
  select_chip();
	//���æ�ź�
	clear_RnB();
	//��������80h
	send_cmd(0x80);
	//�����е�ַ	
	send_addr(0x00);
	send_addr(0x00);
	//�����е�ַ
	send_addr(addr&0xff);
	send_addr((addr>>8)&0xff);
	send_addr((addr>>16)&0xff);
	//д������
	for(i=0;i<2048;i++)
	{
		NFDATA = buff[i];
	}
	//��������10h
	send_cmd(0x10);
	//�ȴ�RnB
	wait_RnB();
	//��ȡд����
	ret = NFDATA;
	//ȡ��ѡ��nandflash
	deselect_chip();
	
	return ret;
}

int NF_Erase(unsigned addr)
{
	int ret;
	//ѡ��nandflash
  select_chip();
	//���æ�ź�
	clear_RnB();
	//��������60h
	send_cmd(0x60);
	//�����е�ַ
	send_addr(addr&0xff);
	send_addr((addr>>8)&0xff);
	send_addr((addr>>16)&0xff);
	//��������d0h
	send_cmd(0xd0);
	//�ȴ�æ
	wait_RnB();
	//��������70h
	send_cmd(0x70);
	//�鿴�������
	ret = NFDATA;
	
	//ȡ��ѡ��nandflash
	deselect_chip();
	
	return ret;
}