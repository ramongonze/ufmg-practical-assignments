all: tp0

tp0: pilha.o
	gcc tp0.c -o tp0 pilha.o -Wall -Wextra -Werror -std=c99 -pedantic

pilha.o: pilha.h
	gcc -c pilha.c -Wall -Wextra -Werror -std=c99 -pedantic

clean:
	rm *.o *.gch *.out
	rm tp0