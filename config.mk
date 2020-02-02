ifndef _CONFIG
_CONFIG :=

###############
# Some Macros #
###############
objs = $(addsuffix .o,$(1))
cursubdir = $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))

#####################
# Compiler settings #
#####################

Q ?= @
CROSS_PREFIX ?= riscv64-unknown-elf
CC := $(CROSS_PREFIX)-gcc
AR := $(CROSS_PREFIX)-ar
LD := $(CC)
OBJCOPY := $(CROSS_PREFIX)-objcopy
SIZE := $(CROSS_PREFIX)-size

SYSROOT ?= $(shell $(CC) --print-sysroot)

CFLAGS += \
	--sysroot=$(SYSROOT)

################
# RISC-V Flags #
################

RISCV_ARCH ?= rv32im
RISCV_ABI ?= ilp32
RISCV_CMODEL ?= medany

RISCV_ARCHFLAGS := \
	-march=$(RISCV_ARCH) \
	-mabi=$(RISCV_ABI) \
	-mcmodel=$(RISCV_CMODEL)

################
# Optimization #
################

DEBUG ?=
OPT_SIZE ?=
LTO ?=

ifeq ($(DEBUG),1)
CFLAGS += -Og -g3
else ifeq ($(OPT_SIZE),1)
CFLAGS += -Os -g3
else
CFLAGS += -O3 -g3
endif

ifeq ($(LTO),1)
CFLAGS += -flto
LDFLAGS += -flto
endif

################
# Common Flags #
################

CFLAGS += \
	$(RISCV_ARCHFLAGS) \
	-Wall -Wextra -Wshadow \
	-MMD \
	-fno-common \
	-ffunction-sections \
	-fdata-sections \
	-DPROFILE_HASHING \
	-fstrict-volatile-bitfields \
	--specs=nano.specs

LDFLAGS += \
	$(RISCV_ARCHFLAGS) \
	--specs=nano.specs \
	--specs=nosys.specs \
	-nostartfiles \
	-ffreestanding \
	-Wl,--gc-sections

################
# Common rules #
################

%.elf: $(LINKDEP)
	@echo "  LD       $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(LD) -o $@ $(filter %.o %.a -l%,$^) $(LDFLAGS)

%.a:
	@echo "  AR       $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(AR) rcs $@ $(filter %.o,$^)

%.bin: %.elf
	@echo "  OBJCOPY  $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(OBJCOPY) -Obinary $^ $@

%.c.o: %.c
	@echo "  CC       $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(CC) -c -o $@ $(CFLAGS) $<

%.S.o: %.S
	@echo "  AS       $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(CC) -c -o $@ $(CFLAGS) $<

include hal/hal.mk

.SECONDARY:

endif
