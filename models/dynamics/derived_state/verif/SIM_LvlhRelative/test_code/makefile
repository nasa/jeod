 run_test: read_trk_csv.o main.o
	g++ read_trk_csv.o main.o -o run_test

read_trk_csv.o : read_trk_csv.cc read_trk_csv.hh
	g++ -c -g read_trk_csv.cc

main.o : main.cc read_trk_csv.hh
	g++ -c -g main.cc

clean :
	rm run_test *.o
