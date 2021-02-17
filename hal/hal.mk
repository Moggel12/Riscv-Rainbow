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

LIBDEPS += libhal.a
LDLIBS += -lhal

CPPFLAGS += -I$(SRCDIR)/hal

############################
# Platform dependent flags #
############################

PLATFORM ?= pqvexriscvup5k

ifeq ($(PLATFORM),murax)
libhal.a: CPPFLAGS += -DVEXRISCV_VOLATILE
endif

ifeq ($(PLATFORM),pqvexriscvup5k)
libhal.a: CPPFLAGS += -DVEXRISCV_VOLATILE -DVEXRISCV_RWMTVEC
endif

ifeq ($(PLATFORM),pqvexriscvicoboard)
libhal.a: CPPFLAGS += -DVEXRISCV_VOLATILE -DVEXRISCV_RWMTVEC
endif

ifeq ($(PLATFORM),pqvexriscvsim)
libhal.a: CPPFLAGS += -DVEXRISCV_RWMTVEC
endif

ifeq ($(PLATFORM),pqvexriscvsimhuge)
libhal.a: CPPFLAGS += -DVEXRISCV_RWMTVEC
endif

#################
# Linker script #
#################

LINKERSCRIPT := hal/$(PLATFORM).ld

LDFLAGS += \
	-Wl,-T$(notdir $(LINKERSCRIPT)) \
	-L$(SRCDIR)/hal

LINKDEPS += $(LINKERSCRIPT)

RETAINED_VARS += PLATFORM

endif
