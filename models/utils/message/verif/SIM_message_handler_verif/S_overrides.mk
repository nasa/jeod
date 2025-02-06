# Generalized S_override.mk file
# Sets trick compilation flags and builds ephemeris binary files
include $(JEOD_HOME)/bin/jeod/generic_S_overrides.mk

FORCE:

clean_runs:
	@echo Cleaning the RUN directory.
	@${JEOD_HOME}/dgh_scripts/clean_run SET_test/RUN
