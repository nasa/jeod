 run_test: read_trk_csv.o quat.o quat_from_mat.o main.o
	g++ read_trk_csv.o quat.o quat_from_mat.o main.o -o run_test

read_trk_csv.o : read_trk_csv.cc read_trk_csv.hh
	g++ -c -g read_trk_csv.cc

quat.o : $(JEOD_HOME)/models/utils/quaternion/src/quat.cc $(JEOD_HOME)/models/utils/quaternion/include/quat.hh
	g++ -c -g -I$(JEOD_HOME)/models/ $(JEOD_HOME)/models/utils/quaternion/src/quat.cc

quat_from_mat.o : $(JEOD_HOME)/models/utils/quaternion/src/quat_from_mat.cc $(JEOD_HOME)/models/utils/quaternion/include/quat.hh
	g++ -c -g -I$(JEOD_HOME)/models/ $(JEOD_HOME)/models/utils/quaternion/src/quat_from_mat.cc

main.o : main.cc read_trk_csv.hh $(JEOD_HOME)/models/utils/quaternion/include/quat.hh
	g++ -c -g -I$(JEOD_HOME)/models/ main.cc

clean :
	rm run_test *.o
