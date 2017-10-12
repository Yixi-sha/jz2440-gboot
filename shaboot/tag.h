#define ATAG_CORE	0x54410001
#define ATAG_MEM	0x54410002
#define ATAG_CMDLINE	0x54410009
#define ATAG_NONE	0x00000000

const char *cmdline = "console=ttySAC0,115200 init=/init";


typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

#define tag_size(type)	((sizeof(struct tag_header) + sizeof(struct type)) >> 2)
#define tag_next(t)	((struct tag *)((u32 *)(t) + (t)->hdr.size))

struct tag_header {
	unsigned int size;
	unsigned int tag;
};


struct tag_core {
	unsigned int flags;		/* bit 0 = read-only */
	unsigned int pagesize;
	unsigned int rootdev;
};

struct tag_mem32 {
	unsigned int	size;
	unsigned int	start;	/* physical start address */
};

struct tag_cmdline {
	char	cmdline[1];	/* this is the minimum size */
};
struct tag {
	struct tag_header hdr;
	union {
		struct tag_core		core;
		struct tag_mem32	mem;
		struct tag_cmdline	cmdline;
	} u;
};