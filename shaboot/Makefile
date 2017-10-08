OBJS := start.o main.o dev/dev.o lib/lib.o net/net.o

CFLAGS := -fno-builtin -I$(shell pwd)/include
export CFLAGS

shaboot.bin : shaboot.elf
	arm-linux-objcopy -O binary shaboot.elf shaboot.bin

shaboot.elf : $(OBJS)	
	arm-linux-ld -Tshaboot.lds -o shaboot.elf $^
	
%.o : %.S
	arm-linux-gcc -g -c $^
	
%.o : %.c
	arm-linux-gcc $(CFLAGS) -c $^

lib/lib.o : 
	make -C lib all

dev/dev.o :
	make -C dev all

net/net.o :
	make -C net all

	
.PHONY: clean
clean:
	make -C lib clean
	make -C dev clean
	make -C net clean
	rm -f *.o *.elf *.bin *.bak
  
