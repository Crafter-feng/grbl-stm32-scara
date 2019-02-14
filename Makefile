# detect what shell is used

#Running on Windows shell 
DEVNUL := NUL
WHICH := where

#Running on Unix shell
#DEVNUL := /dev/null
#WHICH := which

# Find first cross compilation tools
XTOOLS_DIR = $(abspath $(shell dirname `${WHICH} arm-none-eabi-gcc`)/..)

# Set tool & Lib paths
X_LIBC_DIR = $(XTOOLS_DIR)/arm-none-eabi/lib/armv7e-m/fpu
X_CC = $(XTOOLS_DIR)/bin/arm-none-eabi-gcc
X_OBJCOPY = $(XTOOLS_DIR)/bin/arm-none-eabi-objcopy
X_AR = $(XTOOLS_DIR)/bin/arm-none-eabi-ar
X_LD = $(XTOOLS_DIR)/bin/arm-none-eabi-ld
X_GDB = $(XTOOLS_DIR)/bin/arm-none-eabi-gdb

RM=rm -rf

OUT_DIR = build
OUTPUT = grbl-stm32

GRBL_SRC=./grbl/coolant_control.c \
         ./grbl/eeprom.c \
         ./grbl/gcode.c \
         ./grbl/jog.c \
         ./grbl/limits.c \
         ./grbl/main.c \
         ./grbl/motion_control.c \
         ./grbl/nuts_bolts.c \
         ./grbl/planner.c \
         ./grbl/print.c \
         ./grbl/probe.c \
         ./grbl/protocol.c \
         ./grbl/report.c \
         ./grbl/serial.c \
         ./grbl/settings.c \
         ./grbl/spindle_control.c \
         ./grbl/stepper.c \
         ./grbl/system.c \
         ./grbl/scara.c
         
STM_SRC= ./cmsis_boot/startup/startup_stm32f10x_md.c \
         ./cmsis_boot/system_stm32f10x.c \
         ./stm_lib/src/misc.c \
         ./stm_lib/src/stm32f10x_exti.c \
         ./stm_lib/src/stm32f10x_flash.c \
         ./stm_lib/src/stm32f10x_gpio.c \
         ./stm_lib/src/stm32f10x_rcc.c \
         ./stm_lib/src/stm32f10x_tim.c \
         ./stm_lib/src/stm32f10x_usart.c
         
USB_SRC=./stm_usb_fs_lib/src/usb_core.c \
         ./stm_usb_fs_lib/src/usb_init.c \
         ./stm_usb_fs_lib/src/usb_int.c \
         ./stm_usb_fs_lib/src/usb_mem.c \
         ./stm_usb_fs_lib/src/usb_regs.c \
         ./stm_usb_fs_lib/src/usb_sil.c \
         ./usb/hw_config.c \
         ./usb/usb_desc.c \
         ./usb/usb_endp.c \
         ./usb/usb_istr.c \
         ./usb/usb_prop.c \
         ./usb/usb_pwr.c \
         ./util/stm32f10x_it.c

SRC = $(GRBL_SRC) $(USB_SRC) $(STM_SRC)
OBJ = $(patsubst %.c,%.o,$(SRC))

# include files
INC = .
INC += cmsis
INC += cmsis_boot
INC += cmsis_boot/startup
INC += grbl
INC += stm_lib/inc
INC += stm_lib/src
INC += stm_usb_fs_lib/inc
INC += stm_usb_fs_lib/src
INC += usb
INC += util

INCLUDE = $(addprefix -I,$(INC))

CFLAGS = -Wall
CFLAGS += -Os
CFLAGS += -g2
CFLAGS += -mthumb 
CFLAGS += -mcpu=cortex-m3

# linker flags
LDSCRIPT = stm32_flash.ld
LDFLAGS = -T$(LDSCRIPT) -Wl,-Map,$(OUT_DIR)/$(OUTPUT).map -Wl,--gc-sections

# defines
DEFINE = STM32F10X_MD
DEFINE += USE_STDPERIPH_DRIVER
DEFINE += __ASSEMBLY__
DEFINE += _GRBL_
DEFINE += STM32F103C8
DEFINE += LEDBLINK
DEFINE += USEUSB
DEFINE += SCARA
DEFINE += SERVO_PEN

DEFINES = $(addprefix -D,$(DEFINE))

%.o: %.S
	$(X_CC) $(INCLUDE) $(DEFINES) $(CFLAGS) -c $< -o $@
	
%.o: %.c
	$(X_CC) $(INCLUDE) $(DEFINES) $(CFLAGS) -c $< -o $@

.PHONY: all flash clean

all:  $(OBJ)
	@mkdir -p $(OUT_DIR) 2> NUL
	$(X_CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -lm -o $(OUT_DIR)/$(OUTPUT).elf
	$(X_OBJCOPY) -O binary $(OUT_DIR)/$(OUTPUT).elf $(OUT_DIR)/$(OUTPUT).bin
	
flash:
	make	all
	STM32_Programmer_CLI -c port=SWD -d $(OUT_DIR)/$(OUTPUT).elf -v

clean:
	$(RM) $(OBJ)
	$(RM) $(OUT_DIR)
	
