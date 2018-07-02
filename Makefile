all: clean main

clean:
	rm -f *.o
	rm -f *.out
	rm -f main

main: algs.o
	g++ main.cpp -o main algs.o -g -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

algs.o:
	g++ -c algs.cpp -g -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic