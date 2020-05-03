# makefile configuration
COMMON_OBJECTS	=
CPU		= msp430g2533
CFLAGS		= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L/..lib -L/opt/ti/msp430_gcc/include
# msp430 compilers
CC		= msp430-elf-gcc
AS		= msp430-elf-as -mmcu=${CPU} -c

all: install toy.elf

#additional rules for files
toy.elf: ${COMMON_OBJECTS} toyMain.o led.o stateMachines.o buzzer.o wd_interrupt_handler.o ../lib/libLcd.a ../lib/libTimer.a ../lib/libp2sw.a
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

install:
	(cd ../lcdLib; make install)
	(cd ../timerLib; make install)
	(cd ../p2swLib; make install)

load: toy.elf
	msp430loader.sh toy.elf

clean:
	(cd ../lcdLib; make clean)
	(cd ../timerLib; make clean)
	(cd ../p2swLib; make clean)
	rm -f *.o *.elf

led.o: led.c led.h

stateMachines.o: led.h stateMachines.h

switches.o: switches.h stateMachines.h

buzzer.o: buzzer.h

wd_interrupt_handler.o: stateMachines.h

p2_interrupt_handler.o: switches.h
