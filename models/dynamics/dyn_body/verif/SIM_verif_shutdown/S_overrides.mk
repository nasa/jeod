# Generalized S_override.mk file
# Sets trick compilation flags and builds ephemeris binary files
include $(JEOD_HOME)/bin/jeod/generic_S_overrides.mk
TRICK_CFLAGS   += -O0
TRICK_CXXFLAGS += -O0
