PROJECT = hello_world

TOOLCHAIN = arm-none-eabi

CC = $(TOOLCHAIN)-gcc
AR = $(TOOLCHAIN)-ar
SIZE = $(TOOLCHAIN)-size
OBJCOPY = $(TOOLCHAIN)-objcopy

BUILDDIR = build
BINDIR = bin
LIBDIR = lib

DIRS = $(BUILDDIR) $(BINDIR) $(LIBDIR)

LIBMAINDIR = src
LIBMAIN = main
LIBMAINPATH = $(LIBDIR)/lib$(LIBMAIN).a
LIBMAINSRCS = $(wildcard $(LIBMAINDIR)/*.c $(LIBMAINDIR)/*.s $(LIBMAINDIR)/hw/*.c $(LIBMAINDIR)/system/*.c $(LIBMAINDIR)/utils/*.c)

CFLAGS  = -g -Wall -std=c99
CFLAGS += -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -DSTM32F40_41xxx
CFLAGS += -I$(LIBMAINDIR) -Icmsis/core -Icmsis/device

LDFLAGS  = -Wl,--gc-sections,-Map,$(BUILDDIR)/$(PROJECT).map
LDFLAGS += -g -Tstm32f4_flash.ld
LDFLAGS += -L$(LIBDIR) -l$(LIBMAIN)

ARFLAGS = rcs

LIBMAINOBJS := $(LIBMAINSRCS:.c=.o)
LIBMAINOBJS := $(LIBMAINOBJS:.s=.o)
LIBMAINOBJS := $(addprefix $(BUILDDIR)/,$(LIBMAINOBJS))

OBJ_DIRS_TREE = $(dir $(LIBMAINOBJS))

TARGET = $(BINDIR)/$(PROJECT)

.phony: all clean

all: post-build

pre-build:
	@mkdir -p $(DIRS)
	@mkdir -p $(OBJ_DIRS_TREE)

post-build: main-build
	@$(SIZE) $(TARGET).elf

main-build: pre-build
	@make --no-print-directory target

target: $(LIBMAINPATH) \
        $(TARGET).elf \
        $(TARGET).hex \
        $(TARGET).bin

%.hex: %.elf
	@echo "(HEX) $@"
	@$(OBJCOPY) -O ihex $^ $@

%.bin: %.elf
	@echo "(BIN) $@"
	@$(OBJCOPY) -O binary $^ $@

$(TARGET).elf: $(LIBMAINPATH)
	@echo "(LD) $^"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ 

$(LIBMAINPATH): $(LIBMAINOBJS)
	@echo "(AR) $^"
	@$(AR) rcs $@ $^

$(BUILDDIR)/%.o: %.c
	@echo "(CC) $^"
	@$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILDDIR)/%.o: %.s
	@echo "(ASM) $^"
	@$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	@rm -Rf $(DIRS)



