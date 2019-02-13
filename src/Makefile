# Makefile for SHMOODO

TARGET  = shmoodo


GIT_VERSION = $(shell git describe --abbrev=4 --dirty --always --tags)

ifndef BOARD
$(error BOARD is not set)
endif

ifndef LOGLEVEL
	LOGLEVEL = 0
endif

CFLAGS   = -Wall -Wextra -pedantic -std=c99 -Wunused-result -Os
CFLAGS  += -nostdlib -nostartfiles
CFLAGS  += -ffunction-sections 
CFLAGS  += -fdata-sections 
CFLAGS  += -fno-omit-frame-pointer
CFLAGS  += -Wimplicit-fallthrough=0
CFLAGS  += -I. -I include/pb
CFLAGS  += -I include/pb/libc
CFLAGS  += -DVERSION=\"$(GIT_VERSION)\" 
CFLAGS  += -DLOGLEVEL=$(LOGLEVEL)
CFLAGS  += -DTIMING_REPORT=$(TIMING_REPORT)
CFLAGS  += -fno-common -fno-builtin
CFLAGS  += -ffreestanding -fno-exceptions

LIBS     = 

LDFLAGS = 

ASM_SRCS = 
ARCH_ASM_SRCS =
ARCH_C_SRCS =
PLAT_ASM_SRCS =
PLAT_C_SRCS =
BLOB_INPUT  =

C_SRCS   = lib/string.c
C_SRCS  += lib/memmove.c
C_SRCS  += lib/memchr.c
C_SRCS  += lib/memcmp.c
C_SRCS  += lib/strcmp.c
C_SRCS  += lib/uuid.c
C_SRCS  += lib/memset.c
C_SRCS  += lib/strlen.c
C_SRCS  += lib/printf.c
C_SRCS  += lib/snprintf.c
C_SRCS  += lib/putchar.c
C_SRCS  += lib/puts.c


FINAL_IMAGE = 

include board/$(BOARD)/makefile.mk
include plat/$(PB_PLAT_NAME)/makefile.mk
include arch/$(PB_ARCH_NAME)/makefile.mk


CROSS_COMPILE ?= arm-eabi-

CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
AR=$(CROSS_COMPILE)ar
SIZE=$(CROSS_COMPILE)size
STRIP=$(CROSS_COMPILE)strip
OBJCOPY=$(CROSS_COMPILE)objcopy

LDFLAGS += --defsym=SHMOODO_ENTRY=$(PB_ENTRY) 
LDFLAGS += -Tarch/$(PB_ARCH_NAME)/link.lds
LDFLAGS += -Tplat/$(PB_PLAT_NAME)/link.lds
LDFLAGS += -Tlink.lds  --build-id=none 

ARCH_OBJS     = $(ARCH_ASM_SRCS:.S=.o) $(ARCH_C_SRCS:.c=.o) 
PLAT_OBJS     = $(PLAT_ASM_SRCS:.S=.o) $(PLAT_C_SRCS:.c=.o) 
BOARD_OBJS    = $(BOARD_ASM_SRCS:.S=.o) $(BOARD_C_SRCS:.c=.o) 

OBJS	  = $(ARCH_OBJS) $(PLAT_OBJS) $(BOARD_OBJS)  
OBJS     += $(ASM_SRCS:.S=.o) $(C_SRCS:.c=.o) 
OBJS	 += $(RSA_KEYS_SRCS:.der=.dero)
OBJS     += $(BLOB_INPUT:.bin=.bino)


all: plat_early $(TARGET).bin board_final plat_final
	$(info Summary:)
	$(info )
	$(info BOARD:     [${PB_BOARD_NAME}])
	$(info PLAT:      [${PB_PLAT_NAME}])
	$(info ARCH:      [${PB_ARCH_NAME}])
	$(info LOGLEVEL:  [${LOGLEVEL}])
	@echo "VERSION = $(GIT_VERSION)"
	$(info )
	@$(SIZE) -x -B $(TARGET)

todo:
	@grep -lR "TODO:" *

$(TARGET).bin: $(TARGET)
	@echo OBJCOPY $< $@
	@$(OBJCOPY) -O binary -R .comment $< $@

$(TARGET): $(OBJS) main.o
	@echo LD $@
	@$(LD) $(LDFLAGS) $(OBJS) main.o $(LIBS) -o $@

%.o: %.S
	@echo AS $<
	@$(CC) -D__ASSEMBLY__ -c $(CFLAGS) $< -o $@

%.o: %.c
	@echo CC $<
	@$(CC) -c $(CFLAGS) $< -o $@


.PHONY: board_clean arch_clean plat_clean plat_early plat_final

clean: board_clean arch_clean plat_clean
	@-rm -rf *.o $(TARGET) $(TARGET).bin *.map out
	@-rm -f $(OBJS)

.DEFAULT_GOAL := all

