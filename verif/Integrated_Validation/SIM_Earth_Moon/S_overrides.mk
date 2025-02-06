# Set additional Trick compile flags
TRICK_SFLAGS +=-I${JEOD_HOME}/lib/jeod

# Generalized S_override.mk file
# Sets trick compilation flags and builds ephemeris binary files
include $(JEOD_HOME)/bin/jeod/generic_S_overrides.mk
