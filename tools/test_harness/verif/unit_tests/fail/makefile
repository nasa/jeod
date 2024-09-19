

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

run::
	@echo Test 1 should call MessageHandler::fail but should return zero.
	./test_program 1

run::
	@echo ""
	@echo ""
	@echo Test 2 tests the ability to intercept allowed codes and should pass.
	./test_program 2

run::
	@echo ""
	@echo ""
	@echo Test 3 should fail with non-zero status.
	./test_program 3

run::
	@echo ""
	@echo ""
	@echo Test 4 should fail with non-zero status.
	./test_program 4

run::
	@echo ""
	@echo ""
	@echo Test 5 should fail with non-zero status.
	./test_program 5

