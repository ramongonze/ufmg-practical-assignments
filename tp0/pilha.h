#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPRESSAO 201
#define ERRO 4

typedef struct Celula{
	char e[MAX_EXPRESSAO];
	struct Celula *ante, *prox;
}Celula;

typedef struct Pilha{
	Celula *primeiro, *ultimo;
}Pilha;

//Operações da Pilha
Pilha* cria_pilha_vazia();

void empilha(Pilha *p, char buffer[]);

void faz_operacao(Pilha *p, char op);

void destroi_pilha(Pilha *p);
//------------------