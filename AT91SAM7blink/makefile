# ***************************************************************
# *     Makefile for Atmel AT91SAM7S256 - flash execution       *
# *                                                             *
# *               Designed for use in Vagrant                   *
# ***************************************************************

NAME = demo_at91sam7_h256_blink_flash

TOOLCHAIN_PATH ?= /usr/local/arm-4.8.3/

# PGM_INTERFACE ?= stlink-v2
# PGM_INTERFACE ?= ftdi/olimex-arm-jtag-swd
# PGM_INTERFACE ?= ftdi/olimex-arm-usb-ocd-h
PGM_INTERFACE ?= ftdi/olimex-jtag-tiny

ifeq ($(PGM_INTERFACE),ftdi/olimex-arm-jtag-swd)
OPENOCD_INTERFACE_SCRIPT ?= interface/ftdi/olimex-arm-usb-ocd-h.cfg -f interface/ftdi/olimex-arm-jtag-swd.cfg
else
OPENOCD_INTERFACE_SCRIPT ?= interface/$(PGM_INTERFACE).cfg
endif

OPENOCD_SCRIPT_PATH ?= /opt/local/share/openocd/scripts/
OPENOCD_TARGET = at91sam7sx

PGM_SERIAL_PORT ?= /dev/ftdi-usbserial
PGM_SERIAL_BAUD_RATE ?= 115200
PGM_SERIAL_VERIFY ?= -v

# --- You should not have to modify these --------------------------------------

TOOLCHAIN_BIN  = $(TOOLCHAIN_PATH)bin/


# --- Do not modify anything below this point ----------------------------------

BUILD_ROOT     = build/
BUILD_DIR      = $(BUILD_ROOT)$(TARGET)/

CC             = $(TOOLCHAIN_BIN)arm-none-eabi-gcc
CXX            = $(TOOLCHAIN_BIN)arm-none-eabi-g++
AS             = $(TOOLCHAIN_BIN)arm-none-eabi-as
OBJCOPY        = $(TOOLCHAIN_BIN)arm-none-eabi-objcopy
DB             = $(TOOLCHAIN_BIN)arm-none-eabi-gdb
OBJDUMP        = $(TOOLCHAIN_BIN)arm-none-eabi-objdump
AR             = $(TOOLCHAIN_BIN)arm-none-eabi-ar
SIZE           = $(TOOLCHAIN_BIN)arm-none-eabi-size
NM             = $(TOOLCHAIN_BIN)arm-none-eabi-nm
LD             = $(TOOLCHAIN_BIN)arm-none-eabi-ld
CP             = $(TOOLCHAIN_BIN)arm-none-eabi-objcopy
OD		       = $(TOOLCHAIN_BIN)arm-none-eabi-objdump
REMOVE         = rm -f
CAT            = cat
FORMAT_SIZE    = | figlet | cowsay -n -f moose
FORMAT_RAMSIZE = | figlet | cowsay -n -f small

OPENOCD_SCRIPTS_PATH = ./
OPENOCD_TARGET_SCRIPT ?= target/$(OPENOCD_TARGET).cfg

# ------------------------------------------------------------------------------
# Files and directories for the user code
# ------------------------------------------------------------------------------

TARGET_BIN     = main.bin

CPPFLAGS = -fno-exceptions -fno-rtti
CFLAGS  = -I./ -c -fno-common -O0 -g
AFLAGS  = -ahls -mapcs-32 -o crt.o
LFLAGS  =  -Map main.map -Tdemo_at91sam7_h256_blink_flash.cmd
CPFLAGS = --output-target=binary
ODFLAGS	= -x --syms

OBJECTS = crt.o main.o timerisr.o timersetup.o isrsupport.o lowlevelinit.o blinker.o


clean:
	-rm $(OBJECTS) crt.lst main.out main.bin main.map main.dmp 

         
build:  main.out
	@ echo "...copying"
	$(CP) $(CPFLAGS) main.out main.bin
	$(OD) $(ODFLAGS) main.out > main.dmp

main.out: $(OBJECTS) demo_at91sam7_h256_blink_flash.cmd 
	@ echo "..linking"
	$(LD) $(LFLAGS) -o main.out $(OBJECTS) libgcc.a

crt.o: crt.s
	@ echo ".assembling crt.s"
	$(AS) $(AFLAGS) crt.s > crt.lst

main.o: main.c  AT91SAM7S256.h board.h
	@ echo ".compiling main.c"
	$(CC) $(CFLAGS) main.c
	
timerisr.o: timerisr.c  AT91SAM7S256.h board.h
	@ echo ".compiling timerisr.c"
	$(CC) $(CFLAGS) timerisr.c
	
lowlevelinit.o: lowlevelinit.c AT91SAM7S256.h board.h
	@ echo ".compiling lowlevelinit.c"
	$(CC) $(CFLAGS) lowlevelinit.c
	
timersetup.o: timersetup.c AT91SAM7S256.h board.h
	@ echo ".compiling timersetup.c"
	$(CC) $(CFLAGS) timersetup.c
	
isrsupport.o: isrsupport.c
	@ echo ".compiling isrsupport.c"
	$(CC) $(CFLAGS) isrsupport.c

blinker.o: blinker.c AT91SAM7S256.h board.h
	@ echo ".compiling blinker.c"
	$(CC) $(CFLAGS) blinker.c
	

# ------------------------------------------------------------------------------
# Firmware flashing
# ------------------------------------------------------------------------------

BASE_JTAG_CMD = $(OPENOCD_BIN_PATH)openocd \
			-s $(OPENOCD_SCRIPT_PATH) \
			-f $(OPENOCD_INTERFACE_SCRIPT) \
			-f $(OPENOCD_TARGET_SCRIPT) \
			-c "init" -c "halt" -c "sleep 200"

POSTLUDE = -c "sleep 200" -c "reset run" -c "shutdown"


upload_jtag: main.out

	$(BASE_JTAG_CMD) \
				-f $(OPENOCD_SCRIPTS_PATH)sam7flash.script \
				-c "flash write_image erase $(TARGET_BIN) 0x00100000" \
				-c "verify_image $(TARGET_BIN) 0x00100000" \
				$(POSTLUDE)

