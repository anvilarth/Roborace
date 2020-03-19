all: roborace

roborace.o: roborace.cpp 
	g++ -c roborace.cpp  `wx-config --cflags`

roborace: roborace.o
	g++ -o roborace roborace.o `wx-config --libs`

clean:
	rm roborace.o


