file.out: main.o
	g++ main.o -o file.out

main.o: main.cpp
	g++ -std=c++0x -c main.cpp -o main.o
