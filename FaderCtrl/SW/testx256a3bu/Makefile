# MCU
MCU = atxmega256a3bu
F_CPU = 32000000UL

# Tools
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# Programmer
PROGRAMMER = avrispmkII
PORT = usb

# Flags
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall
LDFLAGS = -mmcu=$(MCU)

# Target
TARGET = main

# Automatically collect all C files
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: clean program

# Compile .c → .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link objects → ELF
$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

# ELF → HEX
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# program
program: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p x256a3bu -P $(PORT) -U flash:w:$<

clean:
	rm -f *.o *.elf *.hex
