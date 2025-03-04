################################################################################
# Makefile include file for data directories that contain large source files.
#
# Compiling these large source files with optimization on causes all kinds of
# problems. If the compile succeeds it take hours, but it's more likely that
# the compiler will eventually consume all memory and drop core.
################################################################################

ifndef jeod_compile_directory_unoptimized

JEOD_HOME ?= $(dirname $(dirname $(dirname "$0")))

###
# \def jeod_compile_directory_unoptimized (directory)
#   Causes all source files in the src subdirectory of the specified
#   directory to be compiled unoptimized.
# \param $1  Directory to be compiled unoptimized
jeod_compile_directory_unoptimized = \
  $(foreach file,$(wildcard $(JEOD_HOME)/models/$(1)/src/*.cc),\
    $(eval $(call jeod_compile_file_unoptimized,$(file))))

###
# \def jeod_compile_file_unoptimized (source_file)
#   Causes the specified file to be compiled unoptimized.
# \param $1  Full path to the source file to be compiled unoptimized
jeod_compile_file_unoptimized = \
  $(addsuffix .o,$(subst /src/,/object_$(TRICK_HOST_CPU)/,$(basename $1))) : \
    TRICK_CXXFLAGS += -O0


endif
