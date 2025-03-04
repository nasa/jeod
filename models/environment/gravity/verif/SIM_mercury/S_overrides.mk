JEOD_HOME ?= $(realpath $(CURDIR)/../../../../../)

# Set additional Trick compilation flags
TRICK_CFLAGS += -I${JEOD_HOME}/models ${OPTLEVEL}
TRICK_CXXFLAGS += -I${JEOD_HOME}/models ${OPTLEVEL}

# Generalized S_override.mk file
# Sets trick compilation flags and builds ephemeris binary files
include $(JEOD_HOME)/bin/jeod/generic_S_overrides.mk
