

.PHONY: build

default: build

CMAKE_CMD:=cmake
ifeq (, $(shell which cmake3))
   ifeq (0, $(shell cmake --version | grep "version 3" -c))
      $(error "No cmake version 3 in $(PATH), consider doing yum install cmake3")
   endif
else
   CMAKE_CMD:=cmake3
endif

export JEOD_HOME := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../../../../../)
ifneq (, ${TRICK_HOME})
export ER7_UTILS_HOME := ${TRICK_HOME}/trick_source
endif

JEOD_BUILD_DIR=${JEOD_HOME}/build_unit_test
JEOD_INSTALL_DIR=${JEOD_HOME}/lib_jeod_unit_test

ifneq (, ${GTEST_HOME})
  GTEST_OPTS:=GTEST_HOME=${GTEST_HOME}
endif

ifneq (, ${SKIP_JEODLIB_BUILD})
build_jeod_lib:
	@echo "Skipping JEOD lib build"
else
build_jeod_lib:
	cd ${JEOD_HOME};\
	$(MAKE) -f bin/jeod/makefile BUILD_DIR=${JEOD_BUILD_DIR} INSTALL_DIR=${JEOD_INSTALL_DIR} ${GTEST_OPTS} TRICK_BUILD=0 ENABLE_UNIT_TESTS=1
endif

build:  build_jeod_lib
	$(CMAKE_CMD) -B build -DCMAKE_BUILD_TYPE=Debug -S .
	$(MAKE) -C build install
	cd build && ln -snf ${JEOD_INSTALL_DIR}/de4xx_lib de4xx_lib

clean_jeod_lib:
	-rm -rf ${JEOD_BUILD_DIR}
	-rm -rf ${JEOD_INSTALL_DIR}

clean:
	-rm -rf test_program;
	-rm -rf build;

real_clean: clean clean_jeod_lib

run:
	@echo Running test_program with variations on the command line options.
	@echo The first test doesn\'t set any arguments.
	@echo All options should be at their default settings.
	@echo The next several tests work with one parameter at a time.
	@echo The final test sets all of the command line options.
	@echo ""
	./test_program                                                  0
	./test_program -switch                                          1
	./test_program -toggle                                          2
	./test_program -notoggle                                        3
	./test_program -counter                                         4
	./test_program -counter -counter                                5
	./test_program -string hello                                    6
	./test_program -int 42                                          7
	./test_program -double 3.141592653589793                        8
	./test_program -double 3.141592653589793 -switch -int 42 \
	   -counter -notoggle -string hello -counter        9

