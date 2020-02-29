ifndef _CONFIG
_CONFIG :=

RETAINED_VARS :=

-include .config.mk

.config.mk:
	@echo "# These variables are retained and can't be changed without a clean" > $@
	@$(foreach var,$(RETAINED_VARS),echo "$(var) := $($(var))" >> $@; echo "LAST_$(var) := $($(var))" >> $@;)


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

HOST_CC := cc
HOST_AR := ar
HOST_LD := $(HOST_CC)

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

RETAINED_VARS += DEBUG OPT_SIZE LTO

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
	--specs=nano.specs

LDFLAGS += \
	$(RISCV_ARCHFLAGS) \
	--specs=nano.specs \
	--specs=nosys.specs \
	-nostartfiles \
	-ffreestanding \
	-Wl,--gc-sections

HOST_CFLAGS += \
	-Wall -Wextra -Wshadow \
	-MMD

################
# Common rules #
################

HALNAME = $*

_halname_%.o:
	@echo "  GEN      $@"
	$(Q)echo "const char hal_name[] = \"$(HALNAME)\";" | \
		$(CC) -x c -c -o $@ $(filter-out -g3,$(CFLAGS)) -

%.elf: $(LINKDEP) _halname_%.elf.o
	@echo "  LD       $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(LD) -o $@ $(filter %.o %.a -l%,$(filter-out _halname_%.elf.o,$^)) _halname_$@.o $(LDFLAGS)

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

%.c.S: %.c
	@echo "  CC       $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(CC) -S -o $@ $(CFLAGS) $<

%.c.i: %.c
	@echo "  CC       $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(CC) -E -o $@ $(CFLAGS) $<

%.S.o: %.S
	@echo "  AS       $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(CC) -c -o $@ $(CFLAGS) $<

##############
# Host Rules #
##############

host-%.c.o: %.c
	@echo "  HOST_CC  $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(HOST_CC) -c -o $@ $(HOST_CFLAGS) $<

define host_link =
	@echo "  HOST_LD  $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(HOST_LD) -o $@ $(filter host-%.o -l%,$^) $(HOST_LDFLAGS)
endef

include hal/hal.mk

.SECONDARY:

define VAR_CHECK =
ifneq ($$(origin LAST_$(1)),undefined)
ifneq "$$($(1))" "$$(LAST_$(1))"
$$(error "You changed the $(1) variable, you must run make clean!")
endif
endif
endef

$(foreach VAR,$(RETAINED_VARS),$(eval $(call VAR_CHECK,$(VAR))))

endif
