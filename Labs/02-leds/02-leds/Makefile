#
# A common settings for all projects in this folder.
#

TARGET   = DEMO
MCU      = atmega328p
MCU_DUDE = m328p
F_CPU    = 16000000

## Linux
PREFIX  = /opt/avr8-gnu-toolchain-linux_x86_64
AVRDUDE = avrdude
RM      = rm -f
## See "dmesg" command output
USBPORT = /dev/ttyUSB3

## Windows
#PREFIX  = C:\APPZ\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain
#AVRDUDE = C:\APPZ\avrdude\avrdude.exe
#RM      = del
# See USB-SERIAL CH340 port in Device Manager
#USBPORT = COM3



##
## You should not have to change anything below here
##

# Define variables used as names of programs in built-in rules
export LC_ALL=C
BINDIR  = $(PREFIX)/bin
CC      = $(BINDIR)/avr-gcc -fdiagnostics-color=always
AS      = $(BINDIR)/avr-gcc -fdiagnostics-color=always -x assembler-with-cpp
OBJDUMP = $(BINDIR)/avr-objdump
OBJCOPY = $(BINDIR)/avr-objcopy
SIZE    = $(BINDIR)/avr-size
RMFILES = *.o *.map *.elf *.lss

# Extra flags to give to the C compiler
# https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
CFLAGS  = -std=c99 -Wall -Wextra -Werror -g -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -I. -I$(LIBRARY_DIR)/include
# Extra flags to give to the assembler
ASFLAGS  = -mmcu=$(MCU) -Wall -I$(LIBRARY_DIR)/include
# Extra flags to give to compilers when they are supposed to invoke the linker
LDFLAGS = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map -Wl,--cref

# We define all the targets that are not files
.PHONY = all list size flash clean