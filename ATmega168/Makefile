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

$(OBJS): src/%.o : src/%.c
	@avr-gcc -g -Os -mmcu=$(DEVICE) $(CFLAGS) -c $< $(INC_DIRS) -o $@

$(BASE).hex: $(OBJS)
	@avr-gcc -g -mmcu=$(DEVICE) -o src/$(BASE).elf $(OBJS)
	@avr-objcopy -j .text -j .data -O ihex src/$(BASE).elf src/$(BASE).hex

flash: $(BASE).hex
	avrdude -c $(PROGRAMMER) -p $(DEVICE) -U flash:w:src/$(BASE).hex

clean:
	rm -f src/*.o src/atmega.elf src/atmega.hex