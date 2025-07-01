CC = g++
CFLAGS = -Wall -g

all: main.o globals.o simpleGates.o toffoli.o qft.o grover.o adder.o
	$(CC) $(CFLAGS) -o main main.o globals.o simpleGates.o toffoli.o qft.o grover.o addition.o
 
main.o: src/main.cpp src/globals.h src/simpleGates.h src/toffoli.h
	$(CC) $(CFLAGS) -c src/main.cpp
 
globals.o: src/globals.h
	$(CC) $(CFLAGS) -c src/globals.cpp

simpleGates.o: src/simpleGates.h src/globals.h
	$(CC) $(CFLAGS) -c src/simpleGates.cpp

toffoli.o: src/toffoli.h src/simpleGates.h src/globals.h
	$(CC) $(CFLAGS) -c src/toffoli.cpp

qft.o: src/qft.h src/simpleGates.h src/globals.h
	$(CC) $(CFLAGS) -c src/qft.cpp

grover.o: src/grover.h src/simpleGates.h src/globals.h src/toffoli.h
	$(CC) $(CFLAGS) -c src/grover.cpp

adder.o: src/addition.h src/simpleGates.h src/globals.h src/toffoli.h
	$(CC) $(CFLAGS) -c src/addition.cpp

clean:
	${RM} ${PROGS} *.o *~.

