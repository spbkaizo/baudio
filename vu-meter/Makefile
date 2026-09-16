MCU = attiny1614
F_CPU = 20000000UL 
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
PROGRAMMER_TYPE = serialupdi
PORT = /dev/tty.usbserial-A50285BI  # Adjust this as necessary for your system

# Microchip ATtiny DFP: avr-gcc has no built-in support for the tinyAVR
# 0/1-series, so the device specs and headers come from the pack.
# Download from https://packs.download.microchip.com (Microchip.ATtiny_DFP)
# and unzip it here, or override on the command line: make DFP=/path/to/pack
DFP ?= $(HOME)/local/avr-dfp

CFLAGS = -mmcu=$(MCU) -B $(DFP)/gcc/dev/$(MCU) -isystem $(DFP)/include \
         -DF_CPU=$(F_CPU) -Os -Wall
LDFLAGS = 

# Source and object files
SRC = main.c adc.c knight.c binary.c vu2.c led_control.c cylon.c fixfft.c spectrum.c timer.c lcd.c
OBJ = $(SRC:.c=.o)

# Output binary
TARGET = main

all: $(TARGET).hex size

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lm

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

size: $(TARGET).elf
	@echo
	@avr-size -B $<
	avr-objdump -h -S $< > $(TARGET).lst
	@echo

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -P $(PORT) -p $(MCU) -U flash:w:$<:i

read:
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -P $(PORT) -p $(MCU) -v

clean:
	rm -f $(OBJ) $(TARGET).elf $(TARGET).hex $(TARGET).lst

.PHONY: all clean flash size read

