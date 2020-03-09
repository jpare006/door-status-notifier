BASE = atmega

DEVICE 	   = atmega168
PROGRAMMER = atmelice_isp

SRC_FILES := $(wildcard src/*.c)
OBJS 	  := $(patsubst %.c, %.o, $(SRC_FILES))
INC_DIRS   = -Iinclude

CFLAGS =-std=c99 -Wall -Wextra -Wpointer-arith -Wcast-align -Wwrite-strings \
		-Wswitch-default -Wunreachable-code -Winit-self -Wmissing-field-initializers \
		-Wno-unknown-pragmas -Wstrict-prototypes -Wundef -Wold-style-definition

all: clean flash

compile: 
	@avr-gcc -g -Os -mmcu=$(DEVICE) $(CFLAGS) -c $(SRC_FILES) $(INC_DIRS)

$(BASE).hex: $(SRC_FILES)
	@avr-gcc -g -Os -mmcu=$(DEVICE) $(CFLAGS) -c $(SRC_FILES) $(INC_DIRS)
	@avr-gcc -g -mmcu=$(DEVICE) -o $(BASE).elf $(OBJS)
	@avr-objcopy -j .text -j .data -O ihex $(BASE).elf $(BASE).hex

flash: $(BASE).hex
	avrdude -c $(PROGRAMMER) -p $(DEVICE) -U flash:w:$(BASE).hex

clean:
	rm -f *.o timer.elf timer.hex