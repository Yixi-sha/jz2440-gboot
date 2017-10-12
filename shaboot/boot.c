#include "tag.h"
#include "string.h"

#define SDRAM_KERNRL_START 0x31000000
#define SDRAM_TASS_START 0x30000100
#define SDRAM_ADDR_START 0x30000000
#define SDRAM_ADDDR_SIZE 0x04000000
void (*theKernel)(int,int,unsigned int);

struct tag *pCurTage;

void setup_core_tag(void)
{
	pCurTage = SDRAM_TASS_START;
	pCurTage->hdr.tag = ATAG_CORE;
	pCurTage->hdr.size = tag_size(tag_core);
	
	pCurTage->u.core.flags = 0;
	pCurTage->u.core.pagesize = 4096;
	pCurTage->u.core.rootdev = 0;
	
	pCurTage = tag_next(pCurTage); 
}

void setup_mem_tag(void)
{
	pCurTage->hdr.tag = ATAG_MEM;
	pCurTage->hdr.size = tag_size(tag_mem32);
	
	pCurTage->u.mem.start = SDRAM_ADDR_START;
	pCurTage->u.mem.size = SDRAM_ADDDR_SIZE;
	
	pCurTage = tag_next(pCurTage); 
}


void setup_cmdline_tag(void)
{
	int linelen = strlen(cmdline);
	
	pCurTage->hdr.tag = ATAG_CMDLINE;
	pCurTage->hdr.size = (sizeof (struct tag_header) + linelen + 1 + 4) >> 2;
	
	strcpy(pCurTage->u.cmdline.cmdline,cmdline);
	
	pCurTage = tag_next(pCurTage); 
	
}

void setup_end_tag(void)
{
	pCurTage->hdr.tag = ATAG_NONE;
	pCurTage->hdr.size = 0;
}

void boot_linux(void)
{
	//获取启动地址
	 theKernel =(void (*)(int,int,unsigned int))SDRAM_KERNRL_START;
	
	// 设置启动参数
		//设置核心启动参数
		 setup_core_tag();
	
		//设置内存参数
		setup_mem_tag();
		//设置命令行参数
		setup_cmdline_tag();
		//设置结束标志
		setup_end_tag();
	// 启动Linux系统
	theKernel(0,168,SDRAM_TASS_START);
}