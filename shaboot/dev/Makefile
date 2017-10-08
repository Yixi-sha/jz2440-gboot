objs := button.o interrupt.o led.o mmu.o uart.o nand.o dma.o lcd.o ts.o dm9000.o 

all : $(objs)
	arm-linux-ld -r -o dev.o $^
	
%.o : %.c
	arm-linux-gcc ${CFLAGS} s3c2440.h dm9000.h  -c $^
	
%.o : %.S
	arm-linux-gcc ${CFLAGS} s3c2440.h  -c $^

clean:
	rm -f *.o *.bak	
