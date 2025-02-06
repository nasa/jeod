
################################################################################
# JEOD library makefile
# Usage: cd $JEOD_HOME && make -f bin/jeod/makefile [var=val ...] [goal]
# Supported variable settings:
# ER7_UTILS_HOME  Directory that contains the toplevel er7_utils directory
# TRICK_HOME      Alternative way to specify the er7_utils directory
# JEOD_SPICE_DIR  Directory that contains the SPICE toolkit
# TRICK_BUILD     Non-empty value => Trick as opposed to unit test build
# HIGHLIGHT       Any non-empty value highlights compiler warnings
# WARNINGS        Optional warnings (-Wall is always in effect).
# AT              Specify AT= to see all recipe commands
# OPTLEVEL        Specify OPTLEVEL=-O0 to compile unoptimized
# GCOV            Any non-empty value enables gcov compilation
#                 (Implies OPTLEVEL=-O0)
################################################################################

.PHONY: all clean clean_obj clean_dep

export JEOD_HOME := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../..)

CMAKE_CMD:=cmake
ifeq (, $(shell which cmake3))
   ifeq (0, $(shell cmake --version | grep "version 3" -c))
      $(error "No cmake version 3 in $(PATH), consider doing yum install cmake3")
   endif
else
   CMAKE_CMD:=cmake3
endif

ifndef TRICK_BUILD
TRICK_BUILD:=1
endif

ifndef ENABLE_UNIT_TESTS
ENABLE_UNIT_TESTS:=0
endif

ifndef DE4XX_ONLY
DE4XX_ONLY:=0
endif

ifndef REGEN_DE4XX_DATA
REGEN_DE4XX_DATA:=0
endif

ifndef BUILD_DIR
BUILD_DIR:=${JEOD_HOME}/build
endif

ifndef BUILD_TYPE
BUILD_TYPE:=Debug
endif

ifndef INSTALL_DIR
INSTALL_DIR:=$(JEOD_HOME)
endif

ifneq (,$(wildcard $(BUILD_DIR)/CMakeCache.txt))
  REDO_CMAKE:=0
  $(eval PREVIOUS_TRICK_BUILD:=$(shell grep TRICK_BUILD $(BUILD_DIR)/CMakeCache.txt | grep -q 0; echo $$?))
  $(eval PREVIOUS_ENABLE_UNIT_TESTS:=$(shell grep ENABLE_UNIT_TESTS $(BUILD_DIR)/CMakeCache.txt | grep -q 0; echo $$?))
  $(eval PREVIOUS_REGEN_DE4XX_DATA:=$(shell grep REGEN_DE4XX_DATA $(BUILD_DIR)/CMakeCache.txt | grep -q 0; echo $$?))
  $(eval PREVIOUS_DE4XX_ONLY:=$(shell grep DE4XX_ONLY $(BUILD_DIR)/CMakeCache.txt | grep -q 0; echo $$?))
  $(eval PREVIOUS_BUILD_TYPE:=$(shell grep BUILD_TYPE $(BUILD_DIR)/CMakeCache.txt | cut -d"=" -f2))
  ifneq (${PREVIOUS_TRICK_BUILD},${TRICK_BUILD})
     REDO_CMAKE:=1
     $(info TRICK_BUILD = ${TRICK_BUILD}, PREVIOUS_TRICK_BUILD = ${PREVIOUS_TRICK_BUILD})
  endif
  ifneq (${PREVIOUS_ENABLE_UNIT_TESTS},${ENABLE_UNIT_TESTS})
     REDO_CMAKE:=1
     $(info ENABLE_UNIT_TESTS = ${ENABLE_UNIT_TESTS}, PREVIOUS_ENABLE_UNIT_TESTS = ${PREVIOUS_ENABLE_UNIT_TESTS})
  endif
  ifneq (${PREVIOUS_REGEN_DE4XX_DATA},${REGEN_DE4XX_DATA})
     REDO_CMAKE:=1
     $(info REGEN_DE4XX_DATA = ${REGEN_DE4XX_DATA}, PREVIOUS_REGEN_DE4XX_DATA = ${PREVIOUS_REGEN_DE4XX_DATA})
  endif
  ifneq (${PREVIOUS_DE4XX_ONLY},${DE4XX_ONLY})
     REDO_CMAKE:=1
     $(info DE4XX_ONLY = ${DE4XX_ONLY}, PREVIOUS_DE4XX_ONLY = ${PREVIOUS_DE4XX_ONLY})
  endif
  ifneq (${PREVIOUS_BUILD_TYPE},${BUILD_TYPE})
     REDO_CMAKE:=1
     $(info BUILD_TYPE = ${BUILD_TYPE}, PREVIOUS_BUILD_TYPE = ${PREVIOUS_BUILD_TYPE})
  endif
else
  REDO_CMAKE:=1
endif


ifeq (${REDO_CMAKE},1)
ifneq "$(MAKECMDGOALS)" "help" 
ifneq (,$(wildcard $(BUILD_DIR)/CMakeCache.txt))
$(warning Make option for JEOD $(BUILD_DIR) has changed. Re-configuring using options)
$(warning "TRICK_BUILD=${TRICK_BUILD} ENABLE_UNIT_TESTS=${ENABLE_UNIT_TESTS} REGEN_DE4XX_DATA=${REGEN_DE4XX_DATA} DE4XX_ONLY=${DE4XX_ONLY} CMAKE_BUILD_TYPE=${BUILD_TYPE}")
endif
endif
all:
	@echo "Building JEOD Library"
	rm -r $(BUILD_DIR); \
	mkdir -p $(BUILD_DIR);\
	cd $(BUILD_DIR);\
	$(CMAKE_CMD) -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DINSTALL_DIR=${INSTALL_DIR} -DTRICK_BUILD=${TRICK_BUILD}  \
           -DENABLE_UNIT_TESTS=${ENABLE_UNIT_TESTS} -DREGEN_DE4XX_DATA=${REGEN_DE4XX_DATA} \
           -DDE4XX_ONLY=${DE4XX_ONLY} ..;\
	$(MAKE) install;
else
all:
	@echo "Building JEOD Library"
	-mkdir -p $(BUILD_DIR);\
	cd $(BUILD_DIR);\
	$(MAKE) install;
endif

help:
	@echo -e "To build the JEOD library, execute:";
	@echo -e "   make -f ${JEOD_HOME}/bin/jeod/makefile [options]\n";
	@echo -e "The JEOD library can be build with the following options (defaults are shown as the assigned value):\n";
	@echo -e "   BUILD_TYPE=RelWithDebInfo\n\tSpecify optimization and debug symbol build type. 4 options:";
	@echo -e "\t\tDebug (-g)\n\t\tRelease (-O3)\n\t\tRelWithDebInfo (-g -O2)\n\t\tMinSizeRel (-Os)\n";
	@echo -e "   BUILD_DIR=${JEOD_HOME}/build\n\tSpecify build directory for the cmake and object files.\n"
	@echo -e "   INSTALL_DIR=${JEOD_HOME}\n\tSpecify root directory for host-specific library directory (i.e. lib_jeod_${TRICK_HOST_CPU}).\n"
	@echo -e "   TRICK_BUILD=1 [or 0]\n\tBuild for a Trick simulation or standalone.\n"
	@echo -e "   ENABLE_UNIT_TESTS=0 [or 1]\n\tBuild for unit testing.\n\t\tThis option adds coverage flags.\n"
	@echo -e "   REGEN_DE4XX_DATA=0 [or 1]\n\tRegenerate the de4xx c++ source files from the ASCII data.\n\tOnly needed when new or current data sets are introduced.\n"
	@echo -e "   DE4XX_ONLY=1 [or 0]\n\tConfigures build to only compile the de4xx ephemeris shared libraries.\n"
