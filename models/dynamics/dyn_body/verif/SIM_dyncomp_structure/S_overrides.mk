JEOD_HOME ?= $(realpath $(CURDIR)/../../../../../)

# Generalized S_override.mk file
# Sets trick compilation flags and builds ephemeris binary files
include $(JEOD_HOME)/bin/jeod/generic_S_overrides.mk

all: runs SET_test/common_input.py Modified_data Log_data

runs: SET_test
	@for d in ${JEOD_HOME}/verif/SIM_dyncomp/SET_test/RUN_[1-9]* ; do \
	   ld=$$(basename $$d); \
	   if [ ! -d SET_test/$$ld ] ; then \
	     echo mkdir SET_test/$$ld; \
	     mkdir SET_test/$$ld; \
	   fi; \
	   if [ ! -e SET_test/$$ld/input.py ] ; then \
	      echo ln -s $$d/input.py SET_test/$$ld/input.py; \
	      ln -s $$d/input.py SET_test/$$ld/input.py; \
	   fi; \
	done

SET_test:
	mkdir SET_test

SET_test/common_input.py Modified_data Log_data:
	@if [ ! -e $@ ] ; then \
	   echo ln -s ${JEOD_HOME}/verif/SIM_dyncomp/$@ $@; \
	   ln -s ${JEOD_HOME}/verif/SIM_dyncomp/$@ $@; \
	fi

SET_test/common_input.py : SET_test
