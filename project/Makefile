# makefile configuration
COMMON_OBJECTS	=
CPU		= msp430g2533
CFLAGS		= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L/opt/ti/msp430_gcc/include -L../lib
# msp430 compilers
CC		= msp430-elf-gcc
AS		= msp430-elf-as -mmcu=${CPU} -c

all: install toy.elf

#additional rules for files
x.elf: lcdx.o shapes.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lTimer -lLcd -lp2sw

toy.elf: ${COMMON_OBJECTS} toyMain.o led.o stateMachines.o buzzer.o wd_interrupt_handler.o shapes.o ../lib/libLcd.a ../lib/libTimer.a ../lib/libp2sw.a
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

install:
	(cd ../lcdLib; make install)
	(cd ../timerLib; make install)
	(cd ../p2swLib; make install)

loadx: x.elf
	msp430loader.sh x.elf

load: toy.elf
	msp430loader.sh toy.elf

clean:
	(cd ../lcdLib; make clean)
	(cd ../timerLib; make clean)
	(cd ../p2swLib; make clean)
	rm -f *.o *.elf

lcdx.o: lcdx.c shapes.o

led.o: led.c led.h

stateMachines.o: led.h stateMachines.h shapes.h

switches.o: switches.h stateMachines.h

buzzer.o: buzzer.h

wd_interrupt_handler.o: stateMachines.h

p2_interrupt_handler.o: switches.h

shapes.o: shapes.c shapes.h
