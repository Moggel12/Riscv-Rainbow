CROSS_PREFIX ?= riscv64-unknown-elf
CC            = $(CROSS_PREFIX)-gcc
LD            = $(CROSS_PREFIX)-gcc
OBJCOPY       = $(CROSS_PREFIX)-objcopy

PQRISCV_DIR ?= ../pqriscv

VEXRISCV_PLATFORM ?= pqvexriscvsim

objs = $(notdir $(patsubst %.c,%.o,$(patsubst %.S,%.o,$(1))))
upper = $(shell echo '$(1)' | tr '[:lower:]' '[:upper:]')

CFLAGS  += -O3 -g3
CFLAGS  += \
				-Wall -Wextra -Wimplicit-function-declaration \
				-Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes \
				-Wundef -Wshadow \
				-fno-common -MD $(DEFINES) \
				-DPQRISCV_PLATFORM=$(PLATFORM) \
				-I${PQRISCV_DIR}/mupq/common \
				-DPROFILE_HASHING \
				-ffunction-sections \
				-fdata-sections \
				--specs=nano.specs \
				$(PLATFORM_CFLAGS) \
				$(EXTRAFLAGS)
LDFLAGS += \
				--specs=nano.specs \
				-Wl,--gc-sections \
				$(PLATFORM_LDFLAGS)

include $(PQRISCV_DIR)/common/vexriscv.mk

LIBHAL_SRC = \
	$(PQRISCV_DIR)/common/hal-vexriscv.c \
	$(PQRISCV_DIR)/mupq/common/fips202.c \
	$(PQRISCV_DIR)/common/keccakf1600.c \
	$(PQRISCV_DIR)/common/keccakf1600_asm.S \
	nonrandombytes.c

LIBHAL_OBJ = $(call objs,$(LIBHAL_SRC))

vpath %.c ./src
vpath %.S ./src
vpath %.c $(PQRISCV_DIR)/common
vpath %.S $(PQRISCV_DIR)/common
vpath %.c $(PQRISCV_DIR)/mupq/common

%.elf: $(LIBHAL_OBJ) $(PLATFORM_LINKDEP)
	@echo -e "  LD\t\t  $@"
	@$(LD) -o $@ $(LDFLAGS) $($(call upper,$(@:.elf=))_OBJ) $(LIBHAL_OBJ)

%.bin: %.elf
	@echo -e "  OBJCOPY\t  $@"
	@$(OBJCOPY) -Obinary $^ $@

%.o: %.c
	@echo -e "  CC\t  $@"
	@$(CC) -c -o $@ $(CFLAGS) $<

%.o: %.S
	@echo -e "  CC\t  $@"
	@$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f *.o *.d *.elf *.bin .vexriscvplatform.mk

.SECONDARY:

-include .vexriscvplatform.mk

.vexriscvplatform.mk:
	@echo "LAST_VEXRISCV_PLATFORM := $(VEXRISCV_PLATFORM)" > .vexriscvplatform.mk

ifdef LAST_VEXRISCV_PLATFORM
ifneq ($(VEXRISCV_PLATFORM),$(LAST_VEXRISCV_PLATFORM))
$(error "You changed your VEXRISCV_PLATFORM, you must run make clean!")
endif
endif
