all: clean tp2

clean:
	-rm -f *.o *.gch *.out
	-rm -f tp2
        
tp2: qsort.o tapes.o
	gcc tp2.c -o tp2 qsort.o tapes.o -Wall -Wextra -Werror -std=c99 -pedantic

qsort.o:
	gcc -c qsort.c -Wall -Wextra -Werror -std=c99 -pedantic
tapes.o:
	gcc -c tapes.c -Wall -Wextra -Werror -std=c99 -pedantic

