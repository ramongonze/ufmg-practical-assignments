all: clean tp1
	
clean:
	-rm -f *.o *.gch *.out
	-rm -f tp0

tp1: graph.o queue.o
	gcc tp1.c -o tp1 graph.o queue.o -Wall -Wextra -Werror -std=c99 -pedantic

graph.o: graph.h
	gcc -c graph.c -Wall -Wextra -Werror -std=c99 -pedantic

queue.o: queue.h
	gcc -c queue.c -Wall -Wextra -Werror -std=c99 -pedantic