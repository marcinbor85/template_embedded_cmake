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

SRCS = src/main.c

LIBSTM32DIR = libstm32
LIBSTM32 = stm32f4stdperiph
LIBSTM32PATH = $(LIBDIR)/lib$(LIBSTM32).a
LIBSTM32SRCS = $(wildcard $(LIBSTM32DIR)/src/*.c)

LIBMAINDIR = src
LIBMAIN = main
LIBMAINPATH = $(LIBDIR)/lib$(LIBMAIN).a
LIBMAINSRCS = $(LIBMAINDIR)/system_stm32f4xx.c \
              $(LIBMAINDIR)/startup_stm32f40xx.s

CFLAGS  = -g -Wall -std=c99
CFLAGS += -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER
CFLAGS += -I$(LIBMAINDIR) -I$(LIBSTM32DIR)/inc -Icmsis/core -Icmsis/device

LDFLAGS  = -Wl,--gc-sections,-Map,$(BUILDDIR)/$(PROJECT).map
LDFLAGS += -g -Tstm32f4_flash.ld
LDFLAGS += -L$(LIBDIR) -l$(LIBSTM32) -l$(LIBMAIN)

ARFLAGS = rcs

OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)
OBJS := $(addprefix $(BUILDDIR)/,$(OBJS))

LIBSTM32OBJS := $(LIBSTM32SRCS:.c=.o)
LIBSTM32OBJS := $(LIBSTM32OBJS:.s=.o)
LIBSTM32OBJS := $(addprefix $(BUILDDIR)/,$(LIBSTM32OBJS))

LIBMAINOBJS := $(LIBMAINSRCS:.c=.o)
LIBMAINOBJS := $(LIBMAINOBJS:.s=.o)
LIBMAINOBJS := $(addprefix $(BUILDDIR)/,$(LIBMAINOBJS))

OBJ_DIRS_TREE = $(dir $(OBJS) $(LIBSTM32OBJS) $(LIBMAINOBJS) )

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

target: $(LIBSTM32PATH) \
        $(LIBMAINPATH) \
        $(TARGET).elf \
        $(TARGET).hex \
        $(TARGET).bin

%.hex: %.elf
	@echo "(HEX) $@"
	@$(OBJCOPY) -O ihex $^ $@

%.bin: %.elf
	@echo "(BIN) $@"
	@$(OBJCOPY) -O binary $^ $@

$(TARGET).elf: $(OBJS)
	@echo "(LD) $^"
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(LIBSTM32PATH): $(LIBSTM32OBJS)
	@echo "(AR) $^"
	@$(AR) $(ARFLAGS) $@ $^

$(LIBMAINPATH): $(LIBMAINOBJS)
	@echo "(AR) $^"
	@$(AR) rcs $@ $^

$(BUILDDIR)/%.o: %.c
	@echo "(CC) $^"
	@$(CC) -c $(CFLAGS) -o $@ $^

$(BUILDDIR)/%.o: %.s
	@echo "(ASM) $^"
	@$(CC) -c $(CFLAGS) -o $@ $^

clean:
	@rm -Rf $(DIRS)



