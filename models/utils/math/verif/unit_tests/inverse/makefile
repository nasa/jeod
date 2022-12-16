

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
	@echo Running test_program
	./test_program
