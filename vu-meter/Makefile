MCU = attiny1614
F_CPU = 20000000UL 
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
PROGRAMMER_TYPE = serialupdi
PORT = /dev/tty.usbserial-A50285BI  # Adjust this as necessary for your system

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os
LDFLAGS = 

# Source and object files
SRC = main.c adc.c knight.c binary.c vu1.c led_control.c cylon.c fixfft.c spectrum.c timer.c
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
	@avr-size -B $<
	avr-objdump -h -S $< > main.lst
	@echo

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -P $(PORT) -p $(MCU) -U flash:w:$<:i

read:
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -P $(PORT) -p $(MCU) -v

clean:
	rm -f $(OBJ) $(TARGET).elf $(TARGET).hex

.PHONY: all clean flash size read

