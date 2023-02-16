

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

