# Add your targets here
TARGETS = demo.bin

all: $(TARGETS)

include config.mk

# For each target define a TARGETNAME_SRC, TARGETNAME_OBJ and define any
# additional dependencies for your the target TARGETNAME.elf file (just
# define the dependencies, a generic rule for .elf target exists in
# config.mk).
DEMO_SRC = ./src/demo.c
DEMO_OBJ = $(call objs,$(DEMO_SRC))
demo.elf: $(DEMO_OBJ)

OBJ := $(DEMO_OBJ) $(LIBHAL_OBJ)
-include $(OBJ:.o=.d)
