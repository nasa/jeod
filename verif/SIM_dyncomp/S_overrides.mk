# Generalized S_override.mk file
# Sets trick compilation flags and builds ephemeris binary files
TRICK_CFLAGS += -g
TRICK_CXXFLAGS += -g
include $(JEOD_HOME)/bin/jeod/generic_S_overrides.mk
