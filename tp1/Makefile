all: main

main: funcoes.o
	gcc main.c -o main funcoes.o

funcoes.o: funcoes.c
	gcc -c funcoes.c

clean:
	-rm -f main
	-rm -f funcoes.o