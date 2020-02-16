#define POSTAR_MENSAGEM 1
#define INICIAR_AMIZADE 2
#define CANCELAR_AMIZADE 3
#define CURTIR_MENSAGEM 4
#define EXIBIR_TIMELINE 5
#define MAX_NOME 100
#define MAX_ACAO 1000
#define MAX_MENSAGEM 140
#define MAX_NOME_ARQUIVO 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct amigo{
	int id;
	struct amigo *prox;
}TAmigo;

typedef struct{
	TAmigo *primeiro, *ultimo;
}TListaAmigos;

typedef struct mensagem{
	int id, id_postador;
	int curtidas;
	int *visualizada; 
	int tempo;
	char *mensagem;
	struct mensagem *prox;
}TMensagem;

typedef struct {
	TMensagem *fundo, *topo;
}TTimeline;

typedef struct{
	int id;
	char *nome;	
	TListaAmigos *amigos;
}TUsuario;


TListaAmigos* criaListaAmigos();

TTimeline* criaTimeline();

void adicionaAmigo(TListaAmigos *list, int id);

void postarMensagem(TUsuario **usuarios, TTimeline *timeline,int num_usuarios, int id_usuario, int id_mensagem, char *msg, int tempo);

int idUsuario(TUsuario **usuarios, int num_usuarios, int id);

void iniciarAmizade(TUsuario **usuarios, int num_usuarios, int id1, int id2);

void cancelarAmizade(TUsuario **usuarios, int num_usuarios, int id1, int id2);

TListaAmigos *verAmigos(TUsuario *usuario);

void exibirTimeline(TUsuario **usuarios, TUsuario *usuario, TTimeline *timeline,FILE **arq_saida, int num_usuarios);

void curtirMensagem(TUsuario **usuarios, TTimeline *timeline, int id_usuario, int id_mensagem, int num_usuarios);
