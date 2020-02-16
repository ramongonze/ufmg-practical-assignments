all: clean tp3

clean:
	-rm -f *.o *.gch *.out
	-rm -f tp3
        
tp3: qsort.o algorithms.o
	gcc tp3.c -o tp3 algorithms.o qsort.o -Wall -Wextra -Werror -std=c99 -pedantic

qsort.o:
	gcc -c qsort.c -Wall -Wextra -Werror -std=c99 -pedantic

algorithms.o:
	gcc -c algorithms.c -Wall -Wextra -Werror -std=c99 -pedantic