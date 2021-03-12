ifdef SRCDIR

VPATH = $(SRCDIR)

# Add your targets here (or later with TARGET += ...)
TARGETS =

all: targets

include config.mk

ifndef PROJ_DIR
PROJ_DIR = Ia_Classic_Reference
#PROJ_DIR = Ia_Compressed_Reference
#PROJ_DIR = demo
endif 

CFLAGS += -I$(SRCDIR)/src/$(PROJ_DIR) -I$(SRCDIR)/src/openssl

# For each target define a TARGETNAME_SRC, TARGETNAME_OBJ and define any
# additional dependencies for your the target TARGETNAME.elf file (just
# define the dependencies, a generic rule for .elf target exists in
# config.mk).
#DEMO_SRC = src/demo.c $(wildcard $(SRCDIR)/src/$(PROJ_DIR)/*.c)
#DEMO_OBJ = $(call objs,$(DEMO_SRC))
#demo.elf: $(DEMO_OBJ)
#TARGETS += demo.bin

#IODEMO_SRC = src/iodemo.c
#IODEMO_OBJ = $(call objs,$(IODEMO_SRC))
#iodemo.elf: $(IODEMO_OBJ)
#TARGETS += iodemo.bin

#CUSTOMDEMO_SRC = src/customdemo.c
#CUSTOMDEMO_OBJ = $(call objs,$(CUSTOMDEMO_SRC))
#customdemo.elf: $(CUSTOMDEMO_OBJ)
#TARGETS += customdemo.bin

GENKEY_SRC = src/rainbow-genkey.c $(wildcard $(SRCDIR)/src/$(PROJ_DIR)/*.c)
GENKEY_OBJ = $(call objs,$(GENKEY_SRC))
rainbow-genkey.elf: $(GENKEY_OBJ)
TARGETS += rainbow-genkey.bin

#SIGNKEY_SRC = src/rainbow-sign.c
#SIGNKEY_OBJ = $(call objs,$(SIGNKEY_SRC))
#TARGETS += rainbow-signkey.bin

#VERIFY_SRC = src/rainbow-verify.c 
#VERIFY_OBJ = $(call objs,$(VERIFY_SRC))
#TARGETS += rainbox-verify.bin

#SRCS = $(wildcard $(PROJ_DIR)/*.c)
#SRCS = src/test.c

# Don't forget to add all objects to the OBJ variable
#OBJ += $(DEMO_OBJ)

OBJ += $(DEMO_OBJ) $(GENKEY_OBJ) $(SIGNKEY_OBJ) $(VERIFY_OBJ)

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
	rm -rf $(OBJDIR)

endif
