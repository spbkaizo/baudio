MCU = attiny1614
F_CPU = 8000000
PROGRAMMER = usbtiny
PORT = usb
SRC = main.c
OBJ = $(SRC:.c=.o)
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -Os
TARGET = main

all: $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -R .eeprom $< $@

$(TARGET).elf: $(OBJ)
	avr-gcc $(CFLAGS) -o $@ $^

%.o: %.c
	avr-gcc $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET).elf $(TARGET).hex $(OBJ)

flash: $(TARGET).hex
	avrdude -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -U flash:w:$<:i

fuse:
	avrdude -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -U lfuse:w:0x???:m -U hfuse:w:0x???:m

.PHONY: all clean flash fuse

