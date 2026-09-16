MCU = attiny1614
F_CPU = 3333333UL
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
PROGRAMMER_TYPE = serialupdi
PORT = /dev/tty.usbserial-A50285BI # /dev/ttyUSB0  # Adjust this as necessary for your system

# Microchip ATtiny DFP: avr-gcc has no built-in support for the tinyAVR
# 0/1-series, so the device specs and headers come from the pack.
# Download from https://packs.download.microchip.com (Microchip.ATtiny_DFP)
# and unzip it here, or override on the command line: make DFP=/path/to/pack
DFP ?= $(HOME)/local/avr-dfp

CFLAGS = -mmcu=$(MCU) -B $(DFP)/gcc/dev/$(MCU) -isystem $(DFP)/include \
         -DF_CPU=$(F_CPU) -Os -Wall

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

