ifdef SRCDIR

VPATH = $(SRCDIR)

SRCS = $(SRCDIR)/src

LIBCRYPTO = $(SRCS)/libcrypto
LIBCRYPTO_INC = $(LIBCRYPTO)/include/riscvcrypto
LIBCRYPTO_A = $(LIBCRYPTO)/lib

# Add your targets here (or later with TARGET += ...)
TARGETS =

all: targets

include config.mk

ifndef PROJ_DIR
PROJ_DIR = Ia_Classic_Reference
#PROJ_DIR = Ia_Compressed_Reference
#PROJ_DIR = demo
endif 

CFLAGS += -I$(SRCS)/$(PROJ_DIR) -I$(LIBCRYPTO_INC)
LDLIBS += -L$(LIBCRYPTO_A) -laes_reference -lsha256_reference

CFLS = $(wildcard $(SRCDIR)/src/$(PROJ_DIR)/*.c) 

# For each target define a TARGETNAME_SRC, TARGETNAME_OBJ and define any
# additional dependencies for your the target TARGETNAME.elf file (just
# define the dependencies, a generic rule for .elf target exists in
# config.mk).

GENKEY_SRC = src/rainbow-genkey.c $(CFLS)
GENKEY_OBJ = $(call objs,$(GENKEY_SRC))
rainbow-genkey.elf: $(GENKEY_OBJ)
TARGETS += rainbow-genkey.bin

KEYSIGN_SRC = src/rainbow-sign.c $(CFLS)
KEYSIGN_OBJ = $(call objs,$(KEYSIGN_SRC))
rainbow-sign.elf: $(KEYSIGN_OBJ)
TARGETS += rainbow-sign.bin

VERIFY_SRC = src/rainbow-verify.c $(CFLS)
VERIFY_OBJ = $(call objs,$(VERIFY_SRC))
rainbow-verify.elf: $(VERIFY_OBJ)
TARGETS += rainbow-verify.bin

# Don't forget to add all objects to the OBJ variable
#OBJ += $(DEMO_OBJ)
OBJ += $(GENKEY_OBJ) $(KEYSIGN_OBJ) $(VERIFY_OBJ) 
targets: $(TARGETS)

# Include generated dependencies
-include $(filter %.d,$(OBJ:.o=.d))

# Check for changed variables
define VAR_CHECK =
ifneq ($$(origin LAST_$(1)),undefined)
ifneq "$$($(1))" "$$(LAST_$(1))"
$$(error "You changed the $(1) variable, you must run make clean!")
endif
endif
endef

$(foreach VAR,$(RETAINED_VARS),$(eval $(call VAR_CHECK,$(VAR))))

else
###############################
# Out-of-tree build mechanism #
###############################
.SUFFIXES:

OBJDIR := build

.PHONY: $(OBJDIR)
$(OBJDIR): %:
	+@[ -d $@ ] || mkdir -p $@
	+@$(MAKE) --no-print-directory -r -I$(CURDIR) -C $@ -f $(CURDIR)/Makefile SRCDIR=$(CURDIR) $(MAKECMDGOALS)

Makefile : ;
%.mk :: ;
% :: $(OBJDIR) ;

.PHONY: clean
clean:
	rm -rf $(OBJDIR) src/Ia_Classic_Reference/*.o src/Ia_Compressed_Reference/*.o src/Ia_Classic_Reference/*.d src/Ia_Compressed_Reference/*.d 
endif
