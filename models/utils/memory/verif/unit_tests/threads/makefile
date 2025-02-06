

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


build_jeod_lib:
	cd ${JEOD_HOME};\
	$(MAKE) -f bin/jeod/makefile TRICK_BUILD=0 ENABLE_UNIT_TESTS=1

build:  build_jeod_lib
	mkdir -p build;\
	cd build;\
	$(CMAKE_CMD) -DCMAKE_BUILD_TYPE=Debug ..;\
	$(MAKE) install;\
	ln -snf ${JEOD_HOME}/lib_*/de4xx_lib de4xx_lib;

clean_jeod_lib:
	rm -rf ${JEOD_HOME}/build
	rm -rf ${JEOD_HOME}/lib_*

clean:
	rm -rf test_program;
	rm -rf build;

real_clean: clean clean_jeod_lib

run:
	@echo "\n\nTest 1: Nominal behavior test.\n"
	./test_program 1
	@echo "\n\nTest 2: Corrupted memory test (markers overwritten)\n"
	./test_program -outside 2
	@echo "\n\nTest 3: Leak test (some memory not freed)\n"
	./test_program -leak 3
	@echo "\n\nTest 4: Do both (overwrite and leak)\n";
	./test_program -outside -leak 4

