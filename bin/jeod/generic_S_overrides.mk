# Generic S_overrides makefile for all sims
# This sets the trick compilation flags with C++11 and builds the JEOD libs
# as needed or utilizes the 'trickified' simulation option.
# To invoke this option, build with
#   trick-CP TRICKIFIED=1 [other options]
#
# Caveat user:
# - When rebuilding an existing trickified simulation, you must use
#     make TRICKIFIED=1 [other options]
# - Do not mix and match TRICKIFIED values across builds.



# Set Trick compilation flags
TRICK_CFLAGS += -Wall -I${JEOD_HOME}/models
TRICK_CXXFLAGS += -std=c++11 -Wall -I${JEOD_HOME}/models
TRICK_SFLAGS +=-I${JEOD_HOME}/lib/jeod/JEOD_S_modules
TRICK_SFLAGS +=-I${JEOD_HOME}/lib/jeod

ifdef ENABLE_UNIT_TESTS
TRICK_LDFLAGS += -pg --coverage
endif

ifdef JEOD_SPICE_DIR
TRICK_CFLAGS += -I${JEOD_SPICE_DIR}/include
TRICK_CXXFLAGS += -I${JEOD_SPICE_DIR}/include
TRICK_USER_LINK_LIBS = ${JEOD_SPICE_DIR}/lib/cspice.a

ifeq (1, $(TRICKIFIED))
   ifeq (0, $(shell grep "spice" $(JEOD_HOME)/trickified/trickified_jeod.o -cs))
      $(error Trickified library was built without spice, but it is linked here.)
   endif
endif

else

ifeq (1, $(TRICKIFIED))
   ifneq (0, $(shell grep "spice" $(JEOD_HOME)/trickified/trickified_jeod.o -cs))
      $(error Trickified library was built with spice, but it is not linked here.)
   endif
endif

endif

#=============================================================================
# Build ephemeris binary files
#=============================================================================

CMAKE_CMD:=cmake
ifeq (, $(shell which cmake3))
   ifeq (0, $(shell cmake --version | grep "version 3" -c))
      $(error "No cmake version 3 in $(PATH), consider doing yum install cmake3")
   endif
else
   CMAKE_CMD:=cmake3
endif

PYTHON_CMD:=python
ifeq (, $(shell which python3))
   ifeq (0, $(shell python -V | grep "Python 3." -c))
      $(error "No python version 3 in $(PATH), consider doing yum install python3")
   endif
else
   PYTHON_CMD:=python3
endif

DE4XX_DEST:=build/de4xx_lib
DE4XX_SRC:=${JEOD_HOME}/lib_jeod_${TRICK_HOST_CPU}/de4xx_lib

$(S_MAIN): $(DE4XX_DEST)

$(DE4XX_DEST): $(DE4XX_SRC)
	ln -snf $(DE4XX_SRC) $(DE4XX_DEST);
	cd build;\
	lib_rel_path=`${PYTHON_CMD} -c 'import os.path, sys; print(os.path.relpath("$(DE4XX_SRC)", "."))'`;\
	ln -snf $${lib_rel_path} de4xx_lib

JEOD_TRICKIFIED = ${JEOD_HOME}/trickified
TRICKIFIED_JEOD_LIB = $(JEOD_TRICKIFIED)/trickified_jeod.o
JEOD_TRICKBUILD_LIB = ${JEOD_HOME}/lib_jeod_$(TRICK_HOST_CPU)/libjeod.a
DE4XX_LIB := $(DE4XX_DEST)

# If JEOD_BUILD_TYPE isn't specified, try to deduce from the TRICK_CXXFLAGS variable
# If no option specified and no flags in the TRICK_CXXFLAGS, assume Debug
ifeq (${JEOD_BUILD_TYPE},)
   # Get debug/optimization options. Relatively simple parsing.
   GETDEBUGFLAG:=$(if $(findstring -g, ${TRICK_CXXFLAGS}),1,)
   GETOPTIMMINFLAG:=$(if $(findstring -Os, ${TRICK_CXXFLAGS}),1,)
   GETOPTIMOFFFLAG:=$(if $(findstring -O0, ${TRICK_CXXFLAGS}),1,)
   GETOPTIMLVL1FLAG:=$(if $(findstring -O1, ${TRICK_CXXFLAGS}),1,)
   GETOPTIMLVL2FLAG:=$(if $(findstring -O2, ${TRICK_CXXFLAGS}),1,)
   GETOPTIMLVL3FLAG:=$(if $(findstring -O3, ${TRICK_CXXFLAGS}),1,)
   # Combine -O1, -O2, -O3 into a single "optimized" flag
   GETOPTIMLVLSFLAG:=$(findstring 1, ${GETOPTIMLVL1FLAG} ${GETOPTIMLVL2FLAG} ${GETOPTIMLVL3FLAG})
   ifeq (${GETOPTIMMINFLAG},1)
      JEOD_BUILD_TYPE:=MinSizeRel
   else
      ifeq (${GETDEBUGFLAG},1)
         ifeq (${GETOPTIMLVLSFLAG},1)
            JEOD_BUILD_TYPE:=RelWithDebInfo
         else
            JEOD_BUILD_TYPE:=Debug
         endif
      else
         ifeq (${GETOPTIMLVLSFLAG},1)
            JEOD_BUILD_TYPE:=Release
         else
            JEOD_BUILD_TYPE:=Debug
         endif
      endif
   endif
endif


# Process TRICKIFIED option
ifeq ($(TRICKIFIED),1)
  #Check if build directory exists and is writeable
  TRICKIFIED_EXISTS:=$(shell if [ -e ${TRICKIFIED_JEOD_LIB} ]; then echo 1; else echo 0; fi)
  TRICKIFIED_WRITEABLE:=$(shell if [ -w ${JEOD_TRICKIFIED} ]; then echo 1; else echo 0; fi)

  # Find all model header files except those in verif directories,
  # as a sorted, colon-separated list.
  JEOD_INC_DIRS := \
    $(shell \
      find ${JEOD_HOME}/models -name verif -prune -o -name include -print \
      | sort |  tr '\n' ':' | sed 's/\(.*\):/\1 /')
  JEOD_SRC_DIRS := \
    $(shell \
      find ${JEOD_HOME}/models -name verif -prune -o -name src -print \
      | sort |  tr '\n' ':' | sed 's/\(.*\):/\1 /')

  # Make JEOD appear as an external library to trick.
  TRICK_EXT_LIB_DIRS += :$(JEOD_INC_DIRS):$(JEOD_SRC_DIRS)
  TRICK_GTE_EXT += :${JEOD_HOME}/models

  # Make swig look in the trickified directory.
  TRICK_PYTHON_PATH += :$(JEOD_TRICKIFIED)/python
  TRICK_SWIG_FLAGS += -I$(JEOD_TRICKIFIED)

  # Add the libraries as items to be dynamically linked / loaded.
  TRICK_LDFLAGS += $(TRICKIFIED_JEOD_LIB) $(JEOD_TRICKBUILD_LIB)

ifeq (${TRICKIFIED_EXISTS},0)
   ifeq (${TRICKIFIED_WRITEABLE},1)
     $(DE4XX_SRC): $(JEOD_TRICKBUILD_LIB)

     # Add dependencies for the trickified JEOD and the JEOD library.
     $(SWIG_SRC): $(TRICKIFIED_JEOD_LIB) $(JEOD_TRICKBUILD_LIB)
   else
     $(error Trickified JEOD library doesn't exist and the source directory isn't writeable...)
   endif
else
   ifeq (${TRICKIFIED_WRITEABLE},1)
     # Add dependencies for the trickified JEOD and the JEOD library.
     $(SWIG_SRC): $(TRICKIFIED_JEOD_LIB) $(JEOD_TRICKBUILD_LIB)
   else
     $(info Trickified JEOD library is read-only. Skipping any attempt to update the libs...)
   endif
endif

else
  # Process non-TRICKIFIED build which only compiles the ephemeris data libs
  .PHONY: $(DE4XX_SRC)

  $(DE4XX_SRC):
	@ echo "Building JPL DE4XX ephemeris files" ;\
	cd ${JEOD_HOME};\
	$(MAKE) -f bin/jeod/makefile DE4XX_ONLY=1 TRICK_BUILD=1
endif

.PHONY: $(JEOD_TRICKBUILD_LIB) $(TRICKIFIED_JEOD_LIB)

# Build the trickified JEOD library and JEOD model library if needed.
$(JEOD_TRICKBUILD_LIB):
	$(MAKE) -C ${JEOD_HOME} -f bin/jeod/makefile BUILD_TYPE=${JEOD_BUILD_TYPE} TRICK_BUILD=1

$(TRICKIFIED_JEOD_LIB):
	$(MAKE) -s -C $(JEOD_TRICKIFIED)
