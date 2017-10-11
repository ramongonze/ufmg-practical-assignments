#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TAM_PARAM_ALG 3
#define TAM_NOME_ALG 10
#define TAM_NOME_TIPO_VETOR 50

typedef long TipoChave;
typedef long TipoIndice;

typedef struct TipoItem{
	TipoChave chave;
}TipoItem;

void criaVetor(TipoItem *vetor, TipoIndice tam_vetor, char tipo_ordenacao);

void Selecao(TipoItem *A, TipoIndice n, long *comparacoes, long *movimentacoes);

void Insercao(TipoItem *A, TipoIndice n, long *comparacoes, long *movimentacoes);

void Bolha(TipoItem *A, TipoIndice n, long *comparacoes, long *movimentacoes);

void Shellsort(TipoItem *A, TipoIndice n, long *comparacoes, long *movimentacoes);

void Quicksort(TipoItem *A, TipoIndice n, long *comparacoes, long *movimentacoes);
void Ordena(TipoIndice Esq, TipoIndice Dir, TipoItem *A, long *comparacoes, long *movimentacoes);
void Particao(TipoIndice Esq, TipoIndice Dir,TipoIndice *i, TipoIndice *j, TipoItem *A, long *comparacoes, long *movimentacoes);

void Heapsort(TipoItem *A, TipoIndice n, long *comparacoes, long *movimentacoes);
void Constroi(TipoItem *A, TipoIndice n, long *comparacoes, long *movimentacoes);
void Refaz(TipoIndice Esq, TipoIndice Dir, TipoItem *A, long *comparacoes, long *movimentacoes);

void Mergesort(TipoItem *A, TipoIndice inicio, TipoIndice fim, long *comparacoes, long *movimentacoes);
void merge(TipoItem *A, TipoIndice inicio, TipoIndice meio, TipoIndice fim, long *comparacoes, long *movimentacoes);

void Radixsort(TipoItem * A, TipoIndice tamanho, long *comparacoes, long *movimentacoes);
TipoIndice MaiorNumero(TipoItem * A, TipoIndice tamanho);
