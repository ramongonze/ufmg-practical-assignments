#include "pilha.h"

//Operações da Pilha
Pilha* cria_pilha_vazia(){
	Pilha *p = (Pilha *) malloc(sizeof(Pilha));
	p->primeiro = (Celula *) malloc(sizeof(Celula));
	p->ultimo = p->primeiro;
	p->ultimo->prox = NULL;
	p->ultimo->ante = NULL;
	return p;
}

void empilha(Pilha *p, char buffer[]){
	Celula *nova = (Celula *) malloc(sizeof(Celula));
	strcpy(nova->e, buffer);
	nova->prox = NULL;
	nova->ante = p->ultimo;
	p->ultimo->prox = nova;
	p->ultimo = nova;
}

void faz_operacao(Pilha *p, char op){ // op == 0: efetua soma, op == 1: efetua uma multiplicação
	Celula *aux, *aux2, *aux3;
	long x1, x2;
	aux3 = p->ultimo;
	aux2 = aux3->ante;
	aux = aux2->ante;
	
	x2 = atol(aux2->e);
	x1 = atol(aux->e);

	if(op == '0') sprintf(aux->e, "%ld", x1 + x2);
	else if(op == '1') sprintf(aux->e, "%ld", x1 * x2);

	free(aux3);
	free(aux2);
	p->ultimo = aux;
	aux->prox = NULL;

}

void destroi_pilha(Pilha *p){
	Celula *aux = p->primeiro;
	Celula *aux2 = p->primeiro->prox;

	while(aux2 != NULL){
		free(aux);
		aux = aux2;
		aux2 = aux2->prox;
	}
	free(aux);
	free(p);
}
//-----------------