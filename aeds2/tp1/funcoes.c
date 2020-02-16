#include "funcoes.h"

TListaAmigos *criaListaAmigos(){
	TAmigo *cabeca;
	cabeca = (TAmigo *) malloc(sizeof(TAmigo));
	cabeca->prox = NULL;

	TListaAmigos *novo;

	novo = (TListaAmigos *) malloc(sizeof(TListaAmigos));
	novo->primeiro = cabeca;
	novo->ultimo = cabeca;
	return novo;
}

TTimeline* criaTimeline(){
	TMensagem *fundo;
	fundo = (TMensagem *) malloc(sizeof(TMensagem));
	fundo->prox = NULL;

	TTimeline *novo;

	novo = (TTimeline *) malloc(sizeof(TTimeline));
	novo->fundo = fundo;
	novo->topo = fundo;
	return novo;
}

void adicionaAmigo(TListaAmigos *lista, int id){
	TAmigo *novo;

	novo = (TAmigo *) malloc(sizeof(TAmigo));
	novo->prox = NULL;
	novo->id = id;

	lista->ultimo->prox = novo;
	lista->ultimo = novo;
}

TListaAmigos *verAmigos(TUsuario *usuario){
	return usuario->amigos;
}

void postarMensagem(TUsuario **usuarios, TTimeline *timeline, int num_usuarios, int id_usuario, int id_mensagem, char *msg, int tempo){
	int i, eh_amigo, usuario;
	TAmigo *aux;
	TListaAmigos *amigos;

	usuario = idUsuario(usuarios, num_usuarios, id_usuario);

	TMensagem *nova;
	nova = (TMensagem *) malloc(sizeof(TMensagem));
	nova->id = id_mensagem;
	nova->id_postador = id_usuario;
	nova->curtidas = 0;
	nova->visualizada = (int *)malloc(num_usuarios * sizeof(int));

	for(i = 0; i < num_usuarios; i++){
		amigos = verAmigos(&(usuarios[0][i]));
		eh_amigo = 0;
		aux = amigos->primeiro->prox;
		//Esse laço verifica se o usuário que está exibindo a timeline é amigo do postador da mensagem
		while(aux != NULL){ 
			if(id_usuario == aux->id){
				eh_amigo = 1;
				break;
			}
			aux = aux->prox;
		}

		if(eh_amigo || i == usuario){
			nova->visualizada[i] = 0;
		}else{
			nova->visualizada[i] = 1;
		}

	}

	nova->tempo = tempo;
	nova->mensagem = (char *) malloc(MAX_MENSAGEM * sizeof(char));

	strcpy(nova->mensagem, msg);
	nova->prox = NULL;

	timeline->topo->prox = nova;
	timeline->topo = nova;
}

int idUsuario(TUsuario **usuarios, int num_usuarios, int id){
	int i;

	for(i = 0; i , num_usuarios; i++){
		if(usuarios[0][i].id == id){
			return i;
		}
	}

	return -1;
}

void iniciarAmizade(TUsuario **usuarios, int num_usuarios, int id1, int id2){

	int usuario1, usuario2, achou_amigo;
	TAmigo *aux;

	usuario1 = idUsuario(usuarios, num_usuarios, id1);
	usuario2 = idUsuario(usuarios, num_usuarios, id2);

	achou_amigo = 0;
	aux = usuarios[0][usuario1].amigos->primeiro->prox;
	while(aux != NULL){
		if(aux->id == id2){
			achou_amigo = 1;
			break;
		}
		aux = aux->prox;
	}

	if(!achou_amigo){
		adicionaAmigo(usuarios[0][usuario1].amigos, id2);
	}else{
		fprintf(stderr, "O usuario de id '%d' ja eh amigo de '%d'\n", id1, id2);
	}

	achou_amigo = 0;
	aux = usuarios[0][usuario2].amigos->primeiro->prox;
	while(aux != NULL){
		if(aux->id == id1){
			achou_amigo = 1;
			break;
		}
		aux = aux->prox;
	}

	if(!achou_amigo){
		adicionaAmigo(usuarios[0][usuario2].amigos, id1);
	}else{
		fprintf(stderr, "O usuario de id '%d' ja eh amigo de '%d'\n", id2, id1);
	}
}

void cancelarAmizade(TUsuario **usuarios, int num_usuarios, int id1, int id2){
	int usuario1, usuario2, achou_amigo;
	TAmigo *aux, *aux2;

	usuario1 = idUsuario(usuarios, num_usuarios, id1);
	usuario2 = idUsuario(usuarios, num_usuarios, id2);
	
	achou_amigo = 0;
	aux = usuarios[0][usuario1].amigos->primeiro->prox;
	aux2 = usuarios[0][usuario1].amigos->primeiro;
	while(aux != NULL){
		if(aux->id == id2){
			achou_amigo = 1;
			break;
		}
		aux = aux->prox;
		aux2 = aux2->prox;
	}

	if(!achou_amigo){
		fprintf(stderr, "Nao foi possivel cancelar a amizade. O usuario de id '%d' nao eh amigo de '%d'\n", id1, id2);
	}else{
		aux2->prox = aux->prox;
		if(aux2->prox == NULL){
			usuarios[0][usuario1].amigos->ultimo = aux2;
		}
		if(aux != NULL){
			free(aux);
		}
	}

	achou_amigo = 0;
	aux = usuarios[0][usuario2].amigos->primeiro->prox;
	aux2 = usuarios[0][usuario2].amigos->primeiro;
	while(aux != NULL){
		if(aux->id == id1){
			achou_amigo = 1;
			break;
		}
		aux = aux->prox;
		aux2 = aux2->prox;
	}

	if(!achou_amigo){
		fprintf(stderr, "Nao foi possivel cancelar a amizade. O usuario de id '%d' nao eh amigo de '%d'\n", id2, id1);
	}else{
		aux2->prox = aux->prox;
		if(aux2->prox == NULL){
			usuarios[0][usuario2].amigos->ultimo = aux2;
		}
		if(aux != NULL){
			free(aux);
		}
	}
}

void exibirTimeline(TUsuario **usuarios, TUsuario *usuario, TTimeline *timeline, FILE **arq_saida, int num_usuarios){
	int i, id_usuario;
	TMensagem *aux, *aux3;

	id_usuario = idUsuario(usuarios, num_usuarios, usuario->id);

	//Escreve no arquivo o id e o nome do usuário que exibiu a timeline
	fprintf(*arq_saida, "%d %s\n", usuario->id, usuario->nome);

	aux3 = timeline->topo;

	do{
		aux = timeline->fundo;
		if(aux != aux3){ //Se aux == aux3, a timeline está vazia
			while(aux->prox != aux3){
				aux = aux->prox;
			}

			if(aux->prox->visualizada[id_usuario] == 0){
				fprintf(*arq_saida, "%d %s %d\n", aux->prox->id, aux->prox->mensagem, aux->prox->curtidas);
				aux->prox->visualizada[id_usuario] = 1;
			}
			aux3 = aux;
		}
	}while(aux3 != timeline->fundo);
}

void curtirMensagem(TUsuario **usuarios, TTimeline *timeline, int id_usuario, int id_mensagem, int num_usuarios){
	int i, eh_amigo, usuario;
	TMensagem *aux, *aux2;
	TAmigo *aux3;
	TListaAmigos *amigos;

	//Curte a mensagem
	aux = timeline->fundo;
	while(aux->prox != NULL){
		if(id_mensagem == aux->prox->id){
			aux->prox->curtidas++;
			usuario = idUsuario(usuarios, num_usuarios, aux->prox->id_postador);

			for(i = 0; i < num_usuarios; i++){
				amigos = verAmigos(&(usuarios[0][i]));
				eh_amigo = 0;
				aux3 = amigos->primeiro->prox;
				//Esse laço verifica se o usuário que está exibindo a timeline é amigo do postador da mensagem
				while(aux3 != NULL){ 
					if(aux->prox->id_postador == aux3->id){
						eh_amigo = 1;
						break;
					}
					aux3 = aux3->prox;
				}
				if(eh_amigo || i == usuario){
					aux->prox->visualizada[i] = 0;
				}else{
					aux->prox->visualizada[i] = 1;
				}
			}
			break;
		}
		aux = aux->prox;
	}

	//Atualiza a timeline
	if(!(aux->prox == timeline->topo)){
		aux2 = aux->prox;
		aux->prox = aux->prox->prox;
		timeline->topo->prox = aux2;
		aux2->prox = NULL;
		timeline->topo = aux2;
	}
}
