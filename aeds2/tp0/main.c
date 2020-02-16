#include "funcoes.h"

int main(){
	int i, j, TAM_MATRIZ, **mapa, qtd_jogadores, jogada, *vizinhanca, maior_score, finaliza_jogada;
	t_lista *lista;
	FILE *arq_entrada, *arq_saida;
	Jogador *player;
	char buffer_jogadores[MAX_DESC_JOGADORES];
	
	arq_entrada = fopen("entrada.txt", "r");
	arq_saida = fopen("saida.txt", "w");
	fscanf(arq_entrada, "%d", &TAM_MATRIZ);	

	//Faz o alocamento de memória para a matriz 'mapa'
	mapa = (int **) malloc(TAM_MATRIZ * sizeof(int*));
	for(i=0; i<TAM_MATRIZ; i++){
		mapa[i] = (int *) malloc(TAM_MATRIZ*sizeof(int));
	}
	//Lê todos os dados do arquivo de entrada e os coloca em cada célula da matriz
	for(i=0; i<TAM_MATRIZ;  i++){
		for(j=0; j<TAM_MATRIZ; j++){
			fscanf(arq_entrada, "%d", &mapa[j][i]);
		}
	}
	
	//Lê o nº de jogadores
	fscanf(arq_entrada, "%d", &qtd_jogadores);
	player = (Jogador *) malloc(qtd_jogadores * sizeof(Jogador));

	fgets(buffer_jogadores, MAX_DESC_JOGADORES, arq_entrada); //Elimina o \n após a leitura da linha anterior
	//Lê o nome do jogador e sua posição
	for(i=0; i<qtd_jogadores; i++){
		fgets(buffer_jogadores, MAX_DESC_JOGADORES, arq_entrada);
		player[i].nome = (char *) malloc(MAX_DESC_JOGADORES * sizeof(char));
		strcpy(player[i].nome, strtok(buffer_jogadores, ":"));
		player[i].coluna = atoi(strtok(NULL, ","));
		player[i].linha = atoi(strtok(NULL, ""));
	}
	
	fclose(arq_entrada);
	
	for(i=0; i<qtd_jogadores; i++){
		iniciaJogador(&player[i], mapa, TAM_MATRIZ);
	}

	//Executa todas as jogadas de todos os jogadores
	for(jogada=0; jogada<qtd_jogadores; jogada++){
		vizinhanca = (int *) malloc(CAMPOS*sizeof(int));
		lista = (t_lista *)malloc(sizeof(t_lista));
		criaLista(lista, player[jogada]);
		finaliza_jogada = 1;
		
		for(i=player[jogada].passos; i>0; i--){
			explorar(&player[jogada], mapa, TAM_MATRIZ, vizinhanca);
			finaliza_jogada = andar(&player[jogada], mapa, vizinhanca, lista);
			// Se a quantidade de passos do jogador acabar, ou ele estiver preso, sua jogada termina
		}

		//Escreve o resultado no arquivo saida.txt
		fprintf(arq_saida, "%s %d ", player[jogada].nome, player[jogada].score);
			
		//Imprime o caminho percorrido no arquivo "saida.txt"
		for(i=0; i< (3*TAM_MATRIZ-1) - player[jogada].passos; i++){
			lista->primeiro = lista->primeiro->prox;
			fprintf(arq_saida, "%d,%d " , lista->primeiro->coluna, lista->primeiro->linha);
		}

		fprintf(arq_saida, "\n");
		//Após escrever todas as jogadas, e o score do jogador, lê-se o mapa novamente
		free(mapa);
		free(vizinhanca);
		free(lista);
		
		arq_entrada = fopen("entrada.txt", "r");
		fscanf(arq_entrada, "%d", &TAM_MATRIZ);	

		//Faz o alocamento de memória para a matriz 'mapa'
		mapa = (int **) malloc(TAM_MATRIZ * sizeof(int*));
		for(i=0; i<TAM_MATRIZ; i++){
			mapa[i] = (int *) malloc(TAM_MATRIZ*sizeof(int));
		}
		//Lê todos os dados do arquivo de entrada e os coloca em cada célula da matriz
		for(i=0; i<TAM_MATRIZ;  i++){
			for(j=0; j<TAM_MATRIZ; j++){
				fscanf(arq_entrada, "%d", &mapa[j][i]);
			}
		}
		fclose(arq_entrada);
	}//FINAL DO FOR JOGADA
	
	//Compara o score dos jogadores e imprime no arquivo saida.txt o(s) vencedor(es)
	int vencedores[qtd_jogadores], qtd_vencedores = 0, scores[qtd_jogadores], jogador_vencedor;
	
	//--------- Inicia vetores ----------------
	for(i=0; i<qtd_jogadores; i++){
		vencedores[i] = 0;
	}
	
	//-----------------------------------------
	
	//Coloca todos os scores no vetor "scores"
	for(i=0; i<qtd_jogadores; i++){
		scores[i] = player[i].score;
	}
	
	maior_score = maior_valor(scores, qtd_jogadores);
	
	//Conta quantos jogadores possui o maior score
	for(i=0; i<qtd_jogadores; i++){
		if(player[i].score == maior_score){
			vencedores[i] = 1;
			jogador_vencedor = i;
			qtd_vencedores++;
		}
	}

	if(qtd_vencedores == 1){
		fprintf(arq_saida, "VENCEDOR %s", player[jogador_vencedor].nome);
		fclose(arq_saida);		
	}else{ 
		//Desempate pela quantidade de pokémons mais fortes, se houver mais de 1 jogador com o maior score
		int desempate[qtd_jogadores], maior_qtd_desempate, teste_vencedor, achou_vencedor = 0;
			
		//Inicialização do vetor desempate
		for(j=0; j<qtd_jogadores; j++){
			desempate[j] = 0;
		}
		
		//Faz a comparação da quantidade de cada pokémon que cada vencedor possui, começando pelo Pikachu
		for(i=QTD_POKEMONS-1; i>=0; i--){
			teste_vencedor = 0;
			for(j=0; j<qtd_jogadores; j++){
				if(vencedores[j]){
					desempate[j] = player[j].pokemons[i];
				}
			}
			
			maior_qtd_desempate = maior_valor(desempate, qtd_vencedores);

			for(j=0; j<qtd_jogadores; j++){
				if(desempate[j] == maior_qtd_desempate){
					teste_vencedor++;
				}
			}

			if(teste_vencedor == 1){
				for(j=0; j<qtd_jogadores; j++){
					if(vencedores[j] && player[j].pokemons[i] == maior_qtd_desempate){
						jogador_vencedor = j;
						achou_vencedor = 1;
						break;
					}
				}
			}
		}
		if(achou_vencedor){
			fprintf(arq_saida, "VENCEDOR %s", player[jogador_vencedor].nome);
			fclose(arq_saida);
		}else{ //Desempate por campos andados
			for(i=0; i<qtd_jogadores; i++){
				if(vencedores[i]){
					desempate[i] = player[i].passos;
				}
			}
			
			maior_qtd_desempate = maior_valor(desempate, qtd_jogadores);
			
			if(!maior_qtd_desempate){ //Houve empate!!
				fprintf(arq_saida, "VENCEDOR ");
				
				for(i=0; i<qtd_jogadores; i++){
					if(vencedores[i]){
						fprintf(arq_saida, "%s ", player[i].nome);
					}
				}
				fclose(arq_saida);
			}else{ 
				for(i=0; i<qtd_jogadores; i++){
					if(player[i].passos == maior_qtd_desempate){
						fprintf(arq_saida, "%s ", player[i].nome);
					}
				}
				fclose(arq_saida);
			}
		}
	}

	return 0;
}
