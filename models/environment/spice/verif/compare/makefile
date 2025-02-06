CXX=g++
CXXFLAGS=-g -Wall -pedantic
OBJECTS=src/tokenizer.o src/compare.o src/eigen_rotation.o
EXECUTABLE=compare

$(EXECUTABLE) : $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

src/tokenizer.o src/compare.o : include/tokenizer.hh
src/eigen_rotation.o src/compare.o : include/orientation.hh
src/compare.o : include/io_utils.hh

.phony : clean
clean :
	rm $(OBJECTS) $(EXECUTABLE)
