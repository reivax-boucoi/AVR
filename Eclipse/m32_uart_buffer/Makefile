MCU = atmega32
FORMAT = ihex
TARGET = rbsh
SRC = main.cpp cmds.cpp uart_interpreter.cpp

CXXFLAGS = -Os -g0 -std=gnu++11 -DF_CPU=16000000 -ffunction-sections -fdata-sections

MATH_LIB = -lm

LDFLAGS = $(EXTMEMOPTS) $(LDMAP) $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB) -Wl,--gc-sections -Wl,-Map=$(TARGET).map -T"./atmega32.ld"

CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
REMOVE = rm -f
MV = mv -f

# Define all object files.
OBJ = $(SRC:.cpp=.o)  

# Define all listing files.
LST = $(SRC:.cpp=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CXXFLAGS = -mmcu=$(MCU) -I. $(CXXFLAGS)

# Default target.
all: build

build: elf hex eep
	$(SIZE) --format=berkeley --mcu=$(MCU) $(TARGET).elf

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss 
sym: $(TARGET).sym

.SUFFIXES: .elf .hex .eep .lss .sym

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

.elf.eep:
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@



# Link: create ELF output file from object files.
$(TARGET).elf: $(OBJ)
	$(CXX) $(ALL_CXXFLAGS) $(OBJ) --output $@ $(LDFLAGS)


# Compile: create object files from C source files.
.cpp.o:
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
.cpp.s:
	$(CXX) -S $(ALL_CXXFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ALL_ASFLAGS) $< -o $@



# Target: clean project.
clean:
	$(REMOVE) $(TARGET).hex $(TARGET).eep $(TARGET).cof $(TARGET).elf \
	$(TARGET).map $(TARGET).sym $(TARGET).lss \
	$(OBJ) $(LST) $(SRC:.cpp=.s) $(SRC:.cpp=.d)

depend:
	if grep '^# DO NOT DELETE' $(MAKEFILE) >/dev/null; \
	then \
		sed -e '/^# DO NOT DELETE/,$$d' $(MAKEFILE) > \
			$(MAKEFILE).$$$$ && \
		$(MV) $(MAKEFILE).$$$$ $(MAKEFILE); \
	fi
	echo '# DO NOT DELETE THIS LINE -- make depend depends on it.' \
		>> $(MAKEFILE); \
	$(CC) -M -mmcu=$(MCU) $(CDEFS) $(CINCS) $(SRC) $(ASRC) >> $(MAKEFILE)

.PHONY: all build elf hex eep lss sym clean depend

