JEOD_HOME ?= $(realpath $(CURDIR)/../../../../../../)

# Generalized S_override.mk file
# Sets trick compilation flags and builds ephemeris binary files
include $(JEOD_HOME)/bin/jeod/generic_S_overrides.mk
#=============================================================================
# Define the verification run directories.
#=============================================================================
RUN_SET_DIR = SET_test

RUN_DIRS = $(wildcard $(RUN_SET_DIR)/RUN_*)

runs: $(RUN_DIRS)

$(RUN_DIRS): %: %/input FORCE
	@echo ----------------------------------------------------------
	@echo Running $@
	@echo " "
	./$(S_MAIN) $<
	@echo " "
	@echo Finished running $@
	@echo ----------------------------------------------------------

FORCE:

#############################################################################
##                         MAINTENANCE TARGETS                             ##
#############################################################################

#spotless: real_clean clean_runs


clean_runs:
	@echo Cleaning the RUN directories.
	@${JEOD_HOME}/dgh_scripts/clean_run $(RUN_DIRS)
