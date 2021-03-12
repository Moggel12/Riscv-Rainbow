ifndef _CONFIG
_CONFIG :=


### CUSTOM ENTRIES ###
#ifndef PROJ_DIR
#PROJ_DIR = src/Ia_Classic_Reference
#PROJ_DIR = src/demo
#PROJ_DIR = src/Ia_Classic_Optimize
#endif

#CFLAGS = -O3 -Wall -Wextra -fno-omit-frame-pointer

#ifdef DEBUG
#	CFLAGS =  -D_DEBUG_ -g -O1 -mavx2  -Wall -Wextra -fsanitize=addres    s -fno-omit-frame-pointer
#endif

#CFLAGS += -Isrc/$(PROJ_DIR)

### CUSTOM ENTRIES END ###


RETAINED_VARS :=

-include .config.mk

.config.mk:
	@echo "# These variables are retained and can't be changed without a clean" > $@
	@$(foreach var,$(RETAINED_VARS),echo "$(var) := $($(var))" >> $@; echo "LAST_$(var) := $($(var))" >> $@;)


###############
# Some Macros #
###############
objs = $(addsuffix .o,$(1))
hostobjs = $(addprefix host-objs/,$(addsuffix .o,$(1)))
cursubdir = $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
srcwildcard = $(patsubst $(SRCDIR)/%,%,$(wildcard $(SRCDIR)/$(1)))

#####################
# Compiler settings #
#####################

Q ?= @
CROSS_PREFIX ?= riscv64-unknown-elf
CC := $(CROSS_PREFIX)-gcc
AR := $(CROSS_PREFIX)-gcc-ar
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

OPT_FLAGS = $(if $(DEBUG),-O0,$(if $(OPT_SIZE),-Os,-O3))

ifeq ($(LTO),1)
CFLAGS += -flto
LDFLAGS += -Wl,-flto
endif

################
# Common Flags #
################

CPPFLAGS +=

CFLAGS += \
	$(OPT_FLAGS) \
	-g3 \
	$(RISCV_ARCHFLAGS) \
	-Wall -Wextra -Wshadow \
	-MMD \
	-fno-common \
	-ffunction-sections \
	-fdata-sections \
	--specs=nano.specs \
	$(CPPFLAGS)

LDFLAGS += \
	-L. \
	$(RISCV_ARCHFLAGS) \
	--specs=nano.specs \
	--specs=nosys.specs \
	-nostartfiles \
	-ffreestanding \
	-Wl,--gc-sections

HOST_CPPFLAGS +=

HOST_CFLAGS += \
	-O0 -g3 \
	-Wall -Wextra -Wshadow \
	-MMD \
	$(HOST_CPPFLAGS)

LIBDEPS +=
LINKDEPS += $(LIBDEPS)

include hal/hal.mk

################
# Common rules #
################

HALNAME = $*

define compile =
	@echo "  CC       $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(CC) -c -o $@ $(CFLAGS) $<
endef

_halname_%.o:
	@echo "  GEN      $@"
	$(Q)echo "const char hal_name[] = \"$(HALNAME)\";" | \
		$(CC) -x c -c -o $@ $(filter-out -g3,$(CFLAGS)) $(CPPFLAGS) -

%.elf: _halname_%.elf.o $(LINKDEPS)
	@echo "  LD       $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(LD) $(LDFLAGS) -o $@ $(filter %.o,$^) -Wl,--start-group $(LDLIBS) -Wl,--end-group

%.a:
	@echo "  AR       $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(AR) rcs $@ $(filter %.o,$^)

%.bin: %.elf
	@echo "  OBJCOPY  $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(OBJCOPY) -Obinary $^ $@

%.c.o: %.c
	$(compile)

%.S.o: %.S
	@echo "  AS       $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(CC) -c -o $@ $(CFLAGS) $<

#### CUSTOM ENTRIES ###
#%.o: %.c
	#$(CC) $(CFLAGS) $(INCPATH) -c $<

#%.o: $(PROJ_DIR)/%.c
	#$(CC) $(CFLAGS) $(INCPATH) -c $<
#### CUSTOM ENTRIES END ###

##############
# Host Rules #
##############

host-objs/%.c.o: %.c
	@echo "  HOST_CC  $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(HOST_CC) -c -o $@ $(HOST_CFLAGS) $<

define host_link =
	@echo "  HOST_LD  $@"
	$(Q)[ -d $(@D) ] || mkdir -p $(@D)
	$(Q)$(HOST_LD) $(HOST_LDFLAGS) -o $@ $(filter host-%.o,$^) $(HOST_LDLIBS)
endef

.SECONDARY:

endif
