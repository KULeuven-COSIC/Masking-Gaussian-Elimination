.PHONY: all clean

PREFIX	?= arm-none-eabi
CC		= $(PREFIX)-gcc
LD		= $(PREFIX)-gcc
OBJCOPY	= $(PREFIX)-objcopy
OBJDUMP	= $(PREFIX)-objdump
GDB		= $(PREFIX)-gdb

OPENCM3DIR = ../libopencm3
ARMNONEEABIDIR = /usr/bin
COMMONDIR = ../common

CRYPTO_SRC = \
	B2M.c \
	blas.c \
	bool_util.c \
	gf256_util.c \
	mask_util.c \
	refresh.c

all: mGE_m4.bin

mGE_m4.%: ARCH_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
mGE_m4.o: CFLAGS += -DSTM32F4
$(COMMONDIR)/stm32f4_wrapper.o: CFLAGS += -DSTM32F4
mGE_m4.elf: LDSCRIPT = $(COMMONDIR)/stm32f407x6.ld
mGE_m4.elf: LDFLAGS += -lopencm3_stm32f4
mGE_m4.elf: OBJS += $(COMMONDIR)/stm32f4_wrapper.o
mGE_m4.elf: $(COMMONDIR)/stm32f4_wrapper.o

CFLAGS		+= -O3 -g3 \
		   -Wall -Wextra -Wimplicit-function-declaration \
		   -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes \
		   -Wundef -Wshadow \
		   -I$(ARMNONEEABIDIR)/include -I$(OPENCM3DIR)/include \
		   -fno-common $(ARCH_FLAGS) -MD
LDFLAGS		+= --static -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group \
		   -T$(LDSCRIPT) -nostartfiles -Wl,--gc-sections,--no-print-gc-sections \
		   $(ARCH_FLAGS) -L$(OPENCM3DIR)/lib

OBJS		+= $(CRYPTO_SRC:.c=.o) 

%.bin: %.elf
	$(OBJCOPY) -Obinary $^ $@

%.elf: %.o $(OBJS) $(LDSCRIPT)
	$(LD) -o $@ $< $(OBJS) $(LDFLAGS)

mGE%.o: main.c 
	$(CC) $(CFLAGS) -o $@ -c $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o *.d *.elf *.bin
