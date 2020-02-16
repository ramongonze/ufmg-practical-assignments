#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CAMPO_INVALIDO 7
#define CAMPOS 8
#define MAX_DESC_JOGADORES 500
#define QTD_POKEMONS 5

typedef struct jogador{
	char *nome;
	int score, passos, coluna, linha, pokebolas, pokemons[QTD_POKEMONS];
}Jogador;

typedef struct celula{
	int coluna, linha;
	struct celula *prox;
}t_celula;

typedef struct{
	t_celula *primeiro, *ultimo;
}t_lista;

void iniciaJogador(Jogador *player, int **mapa, int TAM_MATRIZ);

void explorar(Jogador *player, int **mapa, int TAM_MATRIZ, int *vizinhanca);

void mover_jogador(Jogador *player, int prox_posicao);

int andar(Jogador *player, int **mapa, int *vizinhanca, t_lista *lista);

void criaLista(t_lista *lista, Jogador player);

int maior_valor(int *vetor, int tam);

void caminhoPercorrido(t_lista *lista, Jogador player);
