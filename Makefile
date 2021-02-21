ifdef SRCDIR

VPATH = $(SRCDIR)

# Add your targets here (or later with TARGET += ...)
TARGETS =

all: targets

include config.mk

# For each target define a TARGETNAME_SRC, TARGETNAME_OBJ and define any
# additional dependencies for your the target TARGETNAME.elf file (just
# define the dependencies, a generic rule for .elf target exists in
# config.mk).
DEMO_SRC = src/demo.c
DEMO_OBJ = $(call objs,$(DEMO_SRC))
demo.elf: $(DEMO_OBJ)
TARGETS += demo.bin

IODEMO_SRC = src/iodemo.c
IODEMO_OBJ = $(call objs,$(IODEMO_SRC))
iodemo.elf: $(IODEMO_OBJ)
TARGETS += iodemo.bin

# Don't forget to add all objects to the OBJ variable
OBJ += $(DEMO_OBJ)

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
