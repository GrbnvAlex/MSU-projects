all: clear test

.PHONY: all clear clean test

test:  main.o string_list.o
	g++ main.o string_list.o -o list

main.o: main.cpp string_list.h
	g++ -c main.cpp -o main.o

string_list.o: string_list.cpp string_list.h
	g++ -c string_list.cpp -o string_list.o

clear:
	clear

clean:
	rm -f *.o
