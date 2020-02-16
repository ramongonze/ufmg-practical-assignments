all: ordena

ordena: algoritmos.o
	gcc -Wall ordena.c -o ordena algoritmos.o -lm

algoritmos.o: algoritmos.c
	gcc -c -Wall algoritmos.c -lm

clean:
	-rm -f ordena
	-rm -f algoritmos.o