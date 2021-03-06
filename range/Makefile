TARGET = $(notdir $(CURDIR))
PORT = /dev/ttyACM0
AVRDUDE_PROGRAMMER = stk500v1
MCU = atmega328p 
F_CPU = 16000000L
VARIANT = standard

include ~/.arduino.mk

EMPTY :=
SPACE := $(EMPTY) $(EMPTY)

ALIBS=$(wildcard $(ARDUINO_BASE)/libraries/*) $(wildcard $(ARDUINO_BASE)/libraries/*/utility)
APATH=$(subst $(SPACE),:,$(ALIBS))
AINC=$(addprefix -I,$(ALIBS))

VPATH = $(ARDUINO_LIB):$(APATH):../common

SRC =   WInterrupts.c \
	wiring.c \
	wiring_analog.c \
	wiring_digital.c \
	wiring_pulse.c \
	wiring_shift.c \
	queue.c

CXXSRC = HardwareSerial.cpp \
	Stream.cpp \
	Print.cpp \
	Servo.cpp \
	WString.cpp \
	WMath.cpp \
	Wire.cpp \
	Tone.cpp \
	new.cpp

FORMAT = ihex

# Name of this Makefile (used for "make depend").
MAKEFILE = Makefile

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
DEBUG = stabs

OPT = s

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU) -DARDUINO=101 -DUSB_PID=null -DUSB_VID=null
CXXDEFS = -DF_CPU=$(F_CPU) -DARDUINO=101 -DUSB_PID=null -DUSB_VID=null

# Place -I options here
CINCS = -I$(ARDUINO_LIB) -I$(ARDUINO_INC) $(AINC) -I../common
CXXINCS = -I$(ARDUINO_LIB) -I$(ARDUINO_INC) $(AINC) -I../common

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99
CDEBUG = -g$(DEBUG)
CWARN = -Wall -Wstrict-prototypes
CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)

CFLAGS = $(CDEBUG) $(CDEFS) $(CINCS) -O$(OPT) $(CWARN) $(CSTANDARD) $(CEXTRA)
CXXFLAGS = $(CXXDEFS) $(CXXINCS) -O$(OPT)
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs
LDFLAGS = -lm


# Programming support using avrdude. Settings and variables.
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_FLAGS = -V -F -C $(AVRDUDE_CONF) \
	-p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)

# Program settings
CC = ccache $(AVR_TOOLS_PATH)/avr-gcc
CXX = ccache $(AVR_TOOLS_PATH)/avr-g++
OBJCOPY = $(AVR_TOOLS_PATH)/avr-objcopy
OBJDUMP = $(AVR_TOOLS_PATH)/avr-objdump
AR  = $(AVR_TOOLS_PATH)/avr-ar
SIZE = $(AVR_TOOLS_PATH)/avr-size
NM = $(AVR_TOOLS_PATH)/avr-nm
REMOVE = rm -f
MV = mv -f

# Define all object files.
OBJ = $(notdir $(SRC:.c=.o) $(CXXSRC:.cpp=.o) $(ASRC:.S=.o))

# Define all listing files.
LST = $(OBJ:.o=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS)
ALL_CXXFLAGS = -mmcu=$(MCU) -I. $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

# Default target.
all: build sizeafter

build: elf hex

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss
sym: $(TARGET).sym

# Program the device.
upload: $(TARGET).hex pulsedtr
	./pulsedtr $(PORT)
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

pulsedtr: pulsedtr.c
	gcc $(CWARN) $(CSTANDARD) -o $@ $<

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE)  $(TARGET).elf
sizebefore:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(HEXSIZE); echo; fi

sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(HEXSIZE); echo; fi


# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
	--change-section-address .data-0x800000 \
	--change-section-address .bss-0x800000 \
	--change-section-address .noinit-0x800000 \
	--change-section-address .eeprom-0x810000


coff: $(TARGET).elf
	$(COFFCONVERT) -O coff-avr $(TARGET).elf $(TARGET).cof


extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $(TARGET).elf $(TARGET).cof


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

# Link: create ELF output file from library.
$(TARGET).elf: $(TARGET).o core.a
	$(CC) $(ALL_CFLAGS) -mrelax -o $@ $(TARGET).o -L. core.a $(LDFLAGS)

# $(TARGET).elf: $(TARGET).cpp core.a
#         $(CXX) -c $(ALL_CXXFLAGS) -o $@ $(TARGET).cpp -L. core.a $(LDFLAGS)

core.a: $(OBJ)
	@for i in $(OBJ); do echo $(AR) rcs core.a $$i; $(AR) rcs core.a $$i; done

# Compile: create object files from C++ source files.
.cpp.o:
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@

# Compile: create object files from C source files.
.c.o:
	$(CC) -c $(ALL_CFLAGS) $< -o $@

# Compile: create assembler files from C source files.
.c.s:
	$(CC) -S $(ALL_CFLAGS) $< -o $@

# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


%.d: %.cpp
	@$(SHELL) -ec '$(CC) -MM $(ALL_CXXFLAGS) $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

%.d: %.c
	@$(SHELL) -ec '$(CC) -MM $(ALL_CFLAGS) $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

include $(SRC:.c=.d) 
include $(CXXSRC:.cpp=.d)
include $(TARGET).d

# Target: clean project.
clean:
	$(REMOVE) $(TARGET).hex $(TARGET).eep $(TARGET).cof $(TARGET).elf \
	$(TARGET).map $(TARGET).sym $(TARGET).lss $(TARGET).o $(TARGET).d core.a \
	$(OBJ) $(LST) $(OBJ:.o=.s) $(OBJ:.o=.d) $(CXXSRC:.cpp=.s) $(CXXSRC:.cpp=.d) \
	pulsedtr

test:
	cd test && make test

.PHONY:	all build elf hex eep lss sym program coff extcoff clean depend sizebefore sizeafter test
