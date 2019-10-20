CROSS_PREFIX ?= riscv64-unknown-elf
CC            = $(CROSS_PREFIX)-gcc
AR            = $(CROSS_PREFIX)-ar
LD            = $(CROSS_PREFIX)-gcc
OBJCOPY       = $(CROSS_PREFIX)-objcopy

PQRISCV_DIR ?= ../pqriscv

VEXRISCV_PLATFORM ?= pqvexriscvsim

objs = $(notdir $(patsubst %.c,%.c.o,$(patsubst %.S,%.S.o,$(1))))

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
	$(PQRISCV_DIR)/mupq/common/keccakf1600.c \
	nonrandombytes.c

LIBHAL_OBJ = $(call objs,$(LIBHAL_SRC))

vpath %.c ./src
vpath %.S ./src
vpath %.c $(PQRISCV_DIR)/common
vpath %.S $(PQRISCV_DIR)/common
vpath %.c $(PQRISCV_DIR)/mupq/common

%.a:
	@echo -e "  AR\t\t  $@"
	@$(AR) rcs $@ $(filter %.o,$^)

libhal.a: $(LIBHAL_OBJ)

%.elf: libhal.a $(PLATFORM_LINKDEP)
	@echo -e "  LD\t\t  $@"
	@$(LD) -o $@ $(LDFLAGS) $(filter %.o,$^) $(filter %.a,$^)

%.bin: %.elf
	@echo -e "  OBJCOPY\t  $@"
	@$(OBJCOPY) -Obinary $^ $@

%.c.o: %.c
	@echo -e "  CC\t\t  $@"
	@$(CC) -c -o $@ $(CFLAGS) $<

%.S.o: %.S
	@echo -e "  CC\t\t  $@"
	@$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f *.o *.d *.elf *.bin *.a .vexriscvplatform.mk

.SECONDARY:

-include .vexriscvplatform.mk

.vexriscvplatform.mk:
	@echo "LAST_VEXRISCV_PLATFORM := $(VEXRISCV_PLATFORM)" > .vexriscvplatform.mk

ifdef LAST_VEXRISCV_PLATFORM
ifneq ($(VEXRISCV_PLATFORM),$(LAST_VEXRISCV_PLATFORM))
$(error "You changed your VEXRISCV_PLATFORM, you must run make clean!")
endif
endif
