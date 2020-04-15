ifndef _HAL
_HAL :=

###############
# HAL library #
###############

LIBHAL_SRC := \
	hal/hal-vexriscv.c \
	hal/sendfn.c \
	hal/nonrandombytes.c \
	hal/init.c \
	hal/start.S

LIBHAL_OBJ := $(call objs,$(LIBHAL_SRC))
libhal.a: $(LIBHAL_OBJ)

OBJ += $(LIBHAL_OBJ)

CFLAGS += -I$(SRCDIR)/hal

############################
# Platform dependent flags #
############################

PLATFORM ?= pqvexriscvsim

ifeq ($(PLATFORM),murax)
libhal.a: CFLAGS += -DVEXRISCV_VOLATILE
endif

ifeq ($(PLATFORM),pqvexriscvup5k)
libhal.a: CFLAGS += -DVEXRISCV_VOLATILE -DVEXRISCV_RWMTVEC
endif

ifeq ($(PLATFORM),pqvexriscvicoboard)
libhal.a: CFLAGS += -DVEXRISCV_VOLATILE -DVEXRISCV_RWMTVEC
endif

ifeq ($(PLATFORM),pqvexriscvsim)
libhal.a: CFLAGS += -DVEXRISCV_RWMTVEC
endif

ifeq ($(PLATFORM),pqvexriscvsimhuge)
libhal.a: CFLAGS += -DVEXRISCV_RWMTVEC
endif

#################
# Linker script #
#################

LINKERSCRIPT := hal/$(PLATFORM).ld

LDFLAGS += \
	-Wl,-T$(notdir $(LINKERSCRIPT)) \
	-L$(SRCDIR)/hal

LINKDEP += $(LINKERSCRIPT)

RETAINED_VARS += PLATFORM

endif
