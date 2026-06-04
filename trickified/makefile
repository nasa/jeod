ifeq (, ${TRICK_HOME})
export TRICK_HOME := $(shell trick-config --prefix)
endif

ifeq (, ${TRICK_HOME})
    $(error TRICK_HOME must be set)
endif

TRICKIFY := $(TRICK_HOME)/share/trick/makefiles/trickify.mk
export TRICK_HOST_CPU := $(shell TRICK_FORCE_32BIT=$(TRICK_FORCE_32BIT) $(TRICK_HOME)/bin/trick-gte TRICK_HOST_CPU)

ifeq (, ${JEOD_HOME})
export JEOD_HOME := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)
endif

ifeq ($(wildcard $(TRICKIFY)),)
    $(error This makefile requires at least Trick 17.1)
endif

export TRICKIFY_PYTHON_DIR := python_${TRICK_HOST_CPU}
export TRICKIFY_OBJECT_NAME := trickified_jeod_${TRICK_HOST_CPU}.o
export TRICKIFY_CXX_FLAGS := \
    -I${JEOD_HOME}/models \
    -I${TRICK_HOME}/trick_source

ifdef JEOD_SPICE_DIR
    TRICKIFY_CXX_FLAGS += -DJEOD_SPICE_DIR=${JEOD_SPICE_DIR}
endif

export USE_ER7_UTILS = 1


JEOD_TRICKIFIED := ${JEOD_HOME}/trickified
TRICKIFIED_JEOD_LIB := $(JEOD_TRICKIFIED)/trickified_jeod_${TRICK_HOST_CPU}.o
JEOD_TRICKBUILD_LIB := ${JEOD_INSTALL_DIR}/libjeod.a
TRICKIFIED_JEOD_PYLIB := $(JEOD_TRICKIFIED)/python_${TRICK_HOST_CPU}
TRICKIFIED_EXISTS := 0

JEOD_INSTALL_DIR ?= ${JEOD_HOME}/lib_jeod_${TRICK_HOST_CPU}
JEOD_BUILD_EXISTS:=$(shell if [ -e ${JEOD_INSTALL_DIR} ]; then echo 1; else echo 0; fi)
ifeq ($(JEOD_BUILD_EXISTS),1)
JEOD_LIB_WRITEABLE:=$(shell if [ -w ${JEOD_INSTALL_DIR} ]; then echo 1; else echo 0; fi)
else
PARENT_DIR:=$(dir ${JEOD_INSTALL_DIR})
JEOD_LIB_WRITEABLE:=$(shell if [ -w ${PARENT_DIR} ]; then echo 1; else echo 0; fi)
endif


TRICKIFIED_EXISTS:=$(shell if [ -e ${TRICKIFIED_JEOD_LIB} ]; then echo 1; else echo 0; fi)

ifeq (${TRICKIFIED_EXISTS},0)
   ifeq (${JEOD_LIB_WRITEABLE},1)
all:
	@$(MAKE) -s -f $(TRICKIFY)

   else
     $(error Trickified JEOD library doesn't exist and the source directory isn't writeable...)
   endif
else
   ifeq (${JEOD_LIB_WRITEABLE},1)
all:
	@$(MAKE) -s -f $(TRICKIFY)

   else
     $(info Trickified JEOD library is read-only. Skipping any attempt to update the libs...)
all:
	@echo "Do nothing for Trickified JEOD library build"

   endif
endif



clean:
	@rm -rf build ${TRICKIFY_PYTHON_DIR} $(TRICKIFY_OBJECT_NAME)

foo:
	echo TRICK_CFLAGS=${TRICK_CFLAGS}
	echo TRICK_CXXFLAGS=${TRICK_CXXFLAGS}
