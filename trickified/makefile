ifndef TRICK_HOME
    $(error TRICK_HOME must be set)
endif

TRICKIFY := $(TRICK_HOME)/share/trick/makefiles/trickify.mk
export TRICK_HOST_CPU := $(shell TRICK_FORCE_32BIT=$(TRICK_FORCE_32BIT) $(TRICK_HOME)/bin/trick-gte TRICK_HOST_CPU)
export JEOD_HOME := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)

ifeq ($(wildcard $(TRICKIFY)),)
    $(error This makefile requires at least Trick 17.1)
endif

export TRICKIFY_PYTHON_DIR := python_${TRICK_HOST_CPU}
export TRICKIFY_OBJECT_NAME := trickified_jeod_${TRICK_HOST_CPU}.o
export TRICKIFY_CXX_FLAGS := \
    -I${JEOD_HOME}/models \
    -I${TRICK_HOME}/trick_source \
    -std=c++11

ifdef JEOD_SPICE_DIR
    TRICKIFY_CXX_FLAGS += -DJEOD_SPICE_DIR=${JEOD_SPICE_DIR}
endif

export USE_ER7_UTILS = 1

all:
	@$(MAKE) -s -f $(TRICKIFY)

clean:
	@rm -rf build ${TRICKIFY_PYTHON_DIR} $(TRICKIFY_OBJECT_NAME)

foo:
	echo TRICK_CFLAGS=${TRICK_CFLAGS}
	echo TRICK_CXXFLAGS=${TRICK_CXXFLAGS}
