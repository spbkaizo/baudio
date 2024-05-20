MCU = attiny1614
F_CPU = 3333333UL
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
PROGRAMMER_TYPE = serialupdi
PORT = /dev/tty.usbserial-A50285BI # /dev/ttyUSB0  # Adjust this as necessary for your system

AVR_LIBC_PATH = $(HOME)/local/avr/avr

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os # -I$(AVR_LIBC_PATH)/include
#LDFLAGS = -L$(AVR_LIBC_PATH)/lib

# Source and object files
SRC = main.c
OBJ = $(SRC:.c=.o)

# Output binary
TARGET = main

all: $(TARGET).hex size

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

size: $(TARGET).elf
	@echo
	@avr-size --format=avr --mcu=$(MCU) $<
	@echo

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -P $(PORT) -p $(MCU) -U flash:w:$<:i
	# 1 Mhz.
	#$(AVRDUDE) -c $(PROGRAMMER_TYPE) -P $(PORT) -p $(MCU) -U fuse5:w:0x03:m -U fuse6:w:0x03:m

read:
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -P $(PORT) -p $(MCU) -v


clean:
	rm -f $(OBJ) $(TARGET).elf $(TARGET).hex

.PHONY: all clean flash size

