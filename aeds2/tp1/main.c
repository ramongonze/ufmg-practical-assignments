#include "funcoes.h"

int main(int argc, char *argv[]){
	int num_usuarios, tempo, acao, id1_acao, id2_acao, id_mensagem, tamanho_nome;
	int i, j, k;
	char buffer_principal[MAX_ACAO], *mensagem, *mensagem_final, *nome_arquivo_entrada, *nome_arquivo_saida;
	char *nomes, buffer[MAX_ACAO], *amigo;
	TUsuario *usuario;
	TTimeline *timeline;
	TMensagem *fim1, *fim2;
	TAmigo *fim3, *fim4;
	FILE *arq_entrada, *arq_saida;
	
	nome_arquivo_entrada = (char *) malloc(MAX_NOME_ARQUIVO * sizeof(char));
	nome_arquivo_saida = (char *) malloc(MAX_NOME_ARQUIVO * sizeof(char));

//Leitura dos usuários -------------------------
	strcpy(nome_arquivo_entrada, argv[1]);
	sprintf(nome_arquivo_saida, "log.");

	for(i = 0; i < strlen(argv[1]); i++){
		*(nome_arquivo_saida + 4 + i) = *(argv[1] + i);
		*(nome_arquivo_saida + 5 + i) = '\0';
	}

	arq_entrada = fopen(nome_arquivo_entrada, "r");
	if(arq_entrada == NULL){
		fprintf(stderr, "Nao foi possivel abrir o arquivo especificado");
		exit(-1);
	}

	fscanf(arq_entrada, "%d", &num_usuarios);
	fseek(arq_entrada, 2, SEEK_CUR);

	usuario = (TUsuario *) malloc(num_usuarios * sizeof(TUsuario));

	for(i = 0; i < num_usuarios; i++){
		fgets(buffer, MAX_ACAO, arq_entrada);
		usuario[i].id = atoi(strtok(buffer, ";"));
		nomes = strtok(NULL, ";");
			
		tamanho_nome = strlen(nomes);
		usuario[i].nome = (char *) malloc(tamanho_nome * sizeof(char));
		strcpy(usuario[i].nome, nomes);
		usuario[i].amigos = criaListaAmigos();

	//Adiciona os amigos (se houver algum) na lista de amigos de cada usuário
		amigo = strtok(NULL, ";");
		while(amigo != NULL){
			adicionaAmigo(usuario[i].amigos, atoi(amigo));
			amigo = strtok(NULL, ";");
		}
	}
		
//Cria a timeline e abre o arquivo de saida ----
	timeline = criaTimeline();
	arq_saida = fopen(nome_arquivo_saida, "w");
	if(arq_saida == NULL){
		fprintf(stderr, "Erro. Nao foi possivel criar o arquivo de saida\n");
		exit(-1);
	}
		
//Laço onde serão lidas as ações dos usuários --
	while(!feof(arq_entrada)){
		fgets(buffer_principal, MAX_ACAO, arq_entrada);
		tempo = atoi(strtok(buffer_principal, ";"));
		acao = atoi(strtok(NULL, ";"));
		switch(acao){
			case POSTAR_MENSAGEM:
				id1_acao = atoi(strtok(NULL, ";"));
				id_mensagem = atoi(strtok(NULL, ";"));
				mensagem = strtok(NULL, "\n");
				
				mensagem_final = (char *)malloc((strlen(mensagem) + 1) * sizeof(char));
				strcpy(mensagem_final, mensagem);
				mensagem_final[strlen(mensagem) - 1] = '\0';

				postarMensagem(&usuario, timeline, num_usuarios, id1_acao, id_mensagem, mensagem_final, tempo);
				free(mensagem_final);
				break;
			case INICIAR_AMIZADE:
				id1_acao = atoi(strtok(NULL, ";"));
				id2_acao = atoi(strtok(NULL, "\n"));
				iniciarAmizade(&usuario, num_usuarios, id1_acao, id2_acao);
				break;
			case CANCELAR_AMIZADE:
				id1_acao = atoi(strtok(NULL, ";"));
				id2_acao = atoi(strtok(NULL, "\n"));
				cancelarAmizade(&usuario, num_usuarios, id1_acao, id2_acao);
				break;
			case CURTIR_MENSAGEM:
				id1_acao = atoi(strtok(NULL, ";"));
				id2_acao = atoi(strtok(NULL, "\n"));
				curtirMensagem(&usuario, timeline, id1_acao, id2_acao, num_usuarios);
				break;
			case EXIBIR_TIMELINE:
				id1_acao = idUsuario(&usuario, num_usuarios, atoi(strtok(NULL, "\n")));
				exibirTimeline(&usuario, &usuario[id1_acao], timeline, &arq_saida, num_usuarios); 
				break;
			default:
				fprintf(stderr, "Erro. Acao do tempo %d desconhecida\n", acao);
		}
	}

//Finalização ----------------------------
	//Timeline
	fim1 = timeline->fundo->prox;
	fim2 = fim1->prox;
	while(fim2 != timeline->topo){
		free(fim1->mensagem);
		free(fim1->visualizada);
		free(fim1);
		fim1 = fim2;
		fim2 = fim2->prox;
	} 
	free(timeline->fundo);
	free(fim1->mensagem);
	free(fim1->visualizada);
	free(fim1);
	free(fim2);
	
	//Usuários
	for(i = 0; i < num_usuarios; i++){
		free(usuario[i].nome);
		fim3 = usuario[i].amigos->primeiro;
		fim4 = fim3->prox;
		while(fim4 != NULL){
			free(fim3);
			fim3 = fim4;
			fim4 = fim4->prox;
		}
		free(fim3);
		free(fim4);
		free(usuario[i].amigos);
	}
	free(usuario);
	fclose(arq_entrada);
	fclose(arq_saida);

	free(nome_arquivo_entrada);
	free(nome_arquivo_saida);

	return 0;
}
