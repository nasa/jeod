JEOD_HOME ?= $(realpath $(CURDIR)/../../../../../)

ifeq (, ${TRICK_HOME})
export TRICK_HOME := $(shell trick-config --prefix)
endif

# Generalized S_override.mk file
# Sets trick compilation flags and builds ephemeris binary files
include $(JEOD_HOME)/bin/jeod/generic_S_overrides.mk

# Set additional Trick compile flags
TRICK_CFLAGS   += -Wextra -I${TRICK_HOME}/trick_source
TRICK_CXXFLAGS += -Wextra -I${TRICK_HOME}/trick_source
