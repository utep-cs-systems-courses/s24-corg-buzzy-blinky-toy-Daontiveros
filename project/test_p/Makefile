# makfile configuration
COMMON_OBJECTS  = main.o 
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -I../h
LDFLAGS		= -L/opt/ti/msp430_gcc/include 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as

all: led.elf 

#additional rules for files
led.elf: ${COMMON_OBJECTS} buzzer.o main.o 
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ ../lib/libTimer.a

load: led.elf
	msp430loader.sh led.elf

clean:
	rm -f *.o *.elf
	rm -f buzzer.c~
	rm -f buzzer.h~
	rm -f Makefile~

main.o: led.h

wdInterruptHandler.o:  led.h

buzzer.c: buzzer.h
buzzerMain.c: buzzer.h
