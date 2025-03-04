JEOD_HOME ?= $(realpath $(CURDIR)/../../../../../)

ifndef JEOD_HOME
    $(Error: JEOD_HOME environment variable is undefined.)
endif

ifndef JEOD_SPICE_DIR
    $(error JEOD_SPICE_DIR environment variable must be defined.)
endif

# Generalized S_override.mk file
# Sets trick compilation flags and builds ephemeris binary files
include $(JEOD_HOME)/bin/jeod/generic_S_overrides.mk

