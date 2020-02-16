#include "funcoes.h"

void explorar(Jogador *player, int **mapa, int TAM_MATRIZ, int *vizinhanca){
	int i;
	
	for(i=0; i<CAMPOS; i++){
		switch(i){
			case 0:
			if(player->coluna > 0 && player->linha > 0){
				vizinhanca[i] = mapa[player->coluna-1][player->linha-1];
			}else{
				vizinhanca[i] = CAMPO_INVALIDO;
			}
				break;
			case 1:
				if(player->linha > 0){
					vizinhanca[i] = mapa[player->coluna][player->linha-1];
				}else{
					vizinhanca[i] = CAMPO_INVALIDO;
				}
				break;
			case 2:
				if(player->linha > 0 && player->coluna < TAM_MATRIZ-1){
					vizinhanca[i] = mapa[player->coluna+1][player->linha-1];
				}else{
					vizinhanca[i] = CAMPO_INVALIDO;
				}
				break;
			case 3:
				if(player->coluna < TAM_MATRIZ-1){
					vizinhanca[i] = mapa[player->coluna+1][player->linha];
				}else{
					vizinhanca[i] = CAMPO_INVALIDO;
				}
				break;
			case 4:
				if(player->coluna < TAM_MATRIZ-1 && player->linha < TAM_MATRIZ-1){
					vizinhanca[i] = mapa[player->coluna+1][player->linha+1];
				}else{
					vizinhanca[i] = CAMPO_INVALIDO;
				}
				break;
			case 5:
				if(player->linha < TAM_MATRIZ-1){
					vizinhanca[i] = mapa[player->coluna][player->linha+1];
				}else{
					vizinhanca[i] = CAMPO_INVALIDO;
				}
				break;
			case 6:
				if(player->coluna > 0 && player->linha < TAM_MATRIZ-1){
					vizinhanca[i] = mapa[player->coluna-1][player->linha+1];
				}else{
					vizinhanca[i] = CAMPO_INVALIDO;
				}
				break;
			case 7:
				if(player->coluna > 0){
					vizinhanca[i] = mapa[player->coluna-1][player->linha];
				}else{
					vizinhanca[i] = CAMPO_INVALIDO;
				}
		}
	}
}

//Função que move o jogador ao redor dele
void mover_jogador(Jogador *player, int prox_posicao){
	switch(prox_posicao){
					case 0:
						player->coluna -= 1;
						player->linha -= 1;
						break;
					case 1:
						player->linha -= 1;
						break;
					case 2:
						player->coluna += 1;
						player->linha -= 1;;
						break;
					case 3:
						player->coluna += 1;
						break;
					case 4:
						player->coluna += 1;
						player->linha += 1;
						break;
					case 5:
						player->linha += 1;
						break;
					case 6:
						player->coluna -= 1;
						player->linha += 1;
						break;
					case 7:
						player->coluna -= 1;
						break;
	}
}

int andar(Jogador *player, int **mapa, int *vizinhanca, t_lista *lista){
	int i, achou_pokestop = 0, maior, posicao;
	if(player->passos){
		if(!(player->pokebolas)){ //Se o player estiver sem pokébolas, ele procurará por um campo que contenha um pokéstop
			for(i=0; i<CAMPOS; i++){
				if(vizinhanca[i]==6){
					caminhoPercorrido(lista, *player);
					mapa[player->coluna][player->linha] = CAMPO_INVALIDO;
					mover_jogador(player, i);
					player->passos--;
					player->pokebolas++;
					return 1;
				}
			}
		}else{ //Se o player possuir pokébolas, procurará por um campo que tenho o pokémon mais forte
			//O código abaixo coloca na variável "maior" o campo que posui o maior valor (Pokémon de maior CP)
			for(i=0; i<CAMPOS; i++){
				if(vizinhanca[i]!=7 && vizinhanca[i]!=6){
					maior = vizinhanca[i];
					posicao = i;
					break;
				}
			}

			for(i=0; i<CAMPOS; i++){
				if(vizinhanca[i]> maior && vizinhanca[i]!= CAMPO_INVALIDO && vizinhanca[i]!=6){
					maior = vizinhanca[i];
					posicao = i;
				}	
			}
			caminhoPercorrido(lista, *player);
			player->score += maior;
			if(0 < maior && maior <= 5){
				player->pokemons[maior-1]++;
			}
			mapa[player->coluna][player->linha] = CAMPO_INVALIDO;
			mover_jogador(player, posicao);
			player->passos--;
			player->pokebolas--;
			return 1;
		}
		//Se o jogador estiver sem pokébolas e não houver pokéstops perto, ele procura um campo vazio para andar
		if(!achou_pokestop){
			for(i=0; i<CAMPOS; i++){
				if(!vizinhanca[i]){
					caminhoPercorrido(lista, *player);
					mapa[player->coluna][player->linha] = CAMPO_INVALIDO;
					mover_jogador(player, i);
					player->passos--;
					return 1;
				}
			}
		}
		
		//Se não houver espaços vazios, ele anda para um campo aleatório
		for(i=0; i<CAMPOS; i++){
			if(vizinhanca[i]!=7){
				caminhoPercorrido(lista, *player);
				mapa[player->coluna][player->linha] = CAMPO_INVALIDO;
				mover_jogador(player, i);
				player->passos--;
				return 1;
			}
		}
		
		//Se todos os campos em volta do jogador forem inválidos, a função retorna 0
		return 0;
	}else{ //Se a quantidade de passos restantes do jogador for 0, a função retorna 0
		return 0;
	}
}

void iniciaJogador(Jogador *player, int **mapa, int TAM_MATRIZ){
	int i;
	player->score = mapa[player->coluna][player->linha];
	player->passos = 3*TAM_MATRIZ-1;
	player->pokebolas = 3;
	for(i=0; i<QTD_POKEMONS; i++){
		player->pokemons[i] = 0;
	}
}

//FUNÇÕES PARA A LISTA DO CAMINHO PERCORRIDO

void criaLista(t_lista *lista, Jogador player){
	t_celula *novo;
	novo = (t_celula *)malloc(sizeof(t_celula));
	lista->primeiro = novo;
	lista->ultimo = novo;
	lista->primeiro->coluna = player.coluna;
	lista->primeiro->linha = player.linha;
	novo->prox = NULL;
}

void caminhoPercorrido(t_lista *lista, Jogador player){
	t_celula *novo;
	novo = (t_celula *)malloc(sizeof(t_celula));
	novo->coluna = player.coluna;
	novo->linha = player.linha;
	novo->prox = NULL;
	lista->ultimo->prox = novo;
	lista->ultimo = novo;
}

int maior_valor(int *vetor, int tam){
	int maior, i;
	maior = vetor[0];
	
	for(i=1; i<tam; i++){
		if(vetor[i]>maior){
			maior = vetor[i];
		}
	}
	return maior;
}
