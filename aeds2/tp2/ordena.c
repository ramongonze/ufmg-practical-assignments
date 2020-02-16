#include "algoritmos.h"

int main(int argc, char *argv[]){

	int printa_vetores = 0;
	long i, j, tam_vetor, aux, comparacoes, movimentacoes, media_comp, media_mov;
	char *alg_escolhido, tipo_vetor[TAM_NOME_TIPO_VETOR], nome_alg[TAM_NOME_ALG];
	TipoItem *vetor;
	clock_t t_exe, media_exe;

	alg_escolhido = argv[1];
	tam_vetor = atoi(argv[2]);
	if(argc == 5 && *argv[4] == 'p'){
		printa_vetores = 1;
	}

//Aloca memória para o vetor a ser ordenado
	vetor = (TipoItem *) malloc((tam_vetor + 1) * sizeof(TipoItem));

//Atribui o nome do tipo de vetor escolhido
	switch(*argv[3]){
		case 'c':
			strcpy(tipo_vetor, "Ordenado");
			break;
		case 'd':
			strcpy(tipo_vetor, "Inversamente ordenado");
			break;
		case 'q':
			strcpy(tipo_vetor, "Quase ordenado");
			break;
		case 'a':
			strcpy(tipo_vetor, "Aleatorio");
	}

	media_exe = 0;
	media_comp = 0;
	media_mov = 0;

//Seleciona o algoritmo escolhido

//SELEÇÃO
	if(alg_escolhido[0] == 's' && alg_escolhido[1] == 'e' && alg_escolhido[2] == 'l'){
		strcpy(nome_alg, "Selecao");
		if(*argv[3] == 'a') aux = 10;
		else aux = 1;
		for(i = 0; i < aux; i++){
			criaVetor(vetor, tam_vetor, *argv[3]);
			if(printa_vetores){
				printf("VETOR NO ESTADO INICIAL:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
			}
			t_exe = clock();
			Selecao(vetor, tam_vetor, &comparacoes, &movimentacoes);
			t_exe = clock() - t_exe;
			media_exe += t_exe;
			media_comp += comparacoes;
			media_mov += movimentacoes;
			if(printa_vetores){
				printf("VETOR ORDENADO:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
				printa_vetores = 0;
			}
		}
	}

//INSERÇÃO
	else if(alg_escolhido[0] == 'i' && alg_escolhido[1] == 'n' && alg_escolhido[2] == 's'){
		strcpy(nome_alg, "Insercao");
		if(*argv[3] == 'a') aux = 10;
		else aux = 1;
		for(i = 0; i < aux; i++){
			criaVetor(vetor, tam_vetor, *argv[3]);
			if(printa_vetores){
				printf("VETOR NO ESTADO INICIAL:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
			}
			t_exe = clock();
			Insercao(vetor, tam_vetor, &comparacoes, &movimentacoes);
			t_exe = clock() - t_exe;
			media_exe += t_exe;
			media_comp += comparacoes;
			media_mov += movimentacoes;
			if(printa_vetores){
				printf("VETOR ORDENADO:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
				printa_vetores = 0;
			}
		}
	}

//BOLHA
	else if(alg_escolhido[0] == 'b' && alg_escolhido[1] == 'o' && alg_escolhido[2] == 'l'){
		strcpy(nome_alg, "Bolha");
		if(*argv[3] == 'a') aux = 10;
		else aux = 1;
		for(i = 0; i < aux; i++){
			criaVetor(vetor, tam_vetor, *argv[3]);
			if(printa_vetores){
				printf("VETOR NO ESTADO INICIAL:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
			}
			t_exe = clock();
			Bolha(vetor, tam_vetor, &comparacoes, &movimentacoes);
			t_exe = clock() - t_exe;
			media_exe += t_exe;
			media_comp += comparacoes;
			media_mov += movimentacoes;
			if(printa_vetores){
				printf("VETOR ORDENADO:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
			printa_vetores = 0;
			}
		}
	}

//SHELLSORT
	else if(alg_escolhido[0] == 's' && alg_escolhido[1] == 'h' && alg_escolhido[2] == 'e'){
		strcpy(nome_alg, "Shellsort");
		if(*argv[3] == 'a') aux = 10;
		else aux = 1;
		for(i = 0; i < aux; i++){
			criaVetor(vetor, tam_vetor, *argv[3]);
			if(printa_vetores){
				printf("VETOR NO ESTADO INICIAL:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
			}
			t_exe = clock();
			Shellsort(vetor, tam_vetor, &comparacoes, &movimentacoes);
			t_exe = clock() - t_exe;
			media_exe += t_exe;
			media_comp += comparacoes;
			media_mov += movimentacoes;
			if(printa_vetores){
				printf("VETOR ORDENADO:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
				printa_vetores = 0;
			}
		}
	}

//QUICKSORT
	else if(alg_escolhido[0] == 'q' && alg_escolhido[1] == 'u' && alg_escolhido[2] == 'i'){
		strcpy(nome_alg, "Quicksort");
		if(*argv[3] == 'a') aux = 10;
		else aux = 1;;
		for(i = 0; i < aux; i++){
			criaVetor(vetor, tam_vetor, *argv[3]);
			if(printa_vetores){
				printf("VETOR NO ESTADO INICIAL:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
			}
			t_exe = clock();
			Quicksort(vetor, tam_vetor, &comparacoes, &movimentacoes);
			t_exe = clock() - t_exe;
			media_exe += t_exe;
			media_comp += comparacoes;
			media_mov += movimentacoes;
			if(printa_vetores){
				printf("VETOR ORDENADO:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
				printa_vetores = 0;
			}
		}
	}

//HEAPSORT
	else if(alg_escolhido[0] == 'h' && alg_escolhido[1] == 'e' && alg_escolhido[2] == 'a'){
		strcpy(nome_alg, "Heapsort");
		if(*argv[3] == 'a') aux = 10;
		else aux = 1;
		for(i = 0; i < aux; i++){
			criaVetor(vetor, tam_vetor, *argv[3]);
			if(printa_vetores){
				printf("VETOR NO ESTADO INICIAL:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
			}
			t_exe = clock();
			Heapsort(vetor, tam_vetor, &comparacoes, &movimentacoes);
			t_exe = clock() - t_exe;
			media_exe += t_exe;
			media_comp += comparacoes;
			media_mov += movimentacoes;
			if(printa_vetores){
				printf("VETOR ORDENADO:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
				printa_vetores = 0;
			}
		}
	}

//MERGESORT
	else if(alg_escolhido[0] == 'm' && alg_escolhido[1] == 'e' && alg_escolhido[2] == 'r'){
		strcpy(nome_alg, "Mergesort");
		if(*argv[3] == 'a') aux = 10;
		else aux = 1;
		for(i = 0; i < aux; i++){
			criaVetor(vetor, tam_vetor, *argv[3]);
			if(printa_vetores){
				printf("VETOR NO ESTADO INICIAL:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
			}
			t_exe = clock();
			comparacoes = 0;
			movimentacoes = 0;
			Mergesort(vetor, 1, tam_vetor, &comparacoes, &movimentacoes);
			t_exe = clock() - t_exe;
			media_exe += t_exe;
			media_comp += comparacoes;
			media_mov += movimentacoes;
			if(printa_vetores){
				printf("VETOR ORDENADO:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
				printa_vetores = 0;
			}
		}
	}

//RADIXSORT
	else if(alg_escolhido[0] == 'r' && alg_escolhido[1] == 'a' && alg_escolhido[2] == 'd'){
		strcpy(nome_alg, "Radixsort");
		if(*argv[3] == 'a') aux = 10;
		else aux = 1;
		for(i = 0; i < aux; i++){
			criaVetor(vetor, tam_vetor, *argv[3]);
			if(printa_vetores){
				printf("VETOR NO ESTADO INICIAL:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
			}
			t_exe = clock();
			Radixsort(vetor, tam_vetor, &comparacoes, &movimentacoes);
			t_exe = clock() - t_exe;
			media_exe += t_exe;
			media_comp += comparacoes;
			media_mov += movimentacoes;
			if(printa_vetores){
				printf("VETOR ORDENADO:\n");
				for(j = 1; j <= tam_vetor; j++) printf("%ld ", vetor[j].chave);
				printf("\n\n");
				printa_vetores = 0;
			}
		}
	}

//Saídas

	media_exe /= aux;
	media_comp /= aux;
	media_mov /= aux;

	printf("Algoritmo de ordenacao escolhido: %s.\n", nome_alg);
	printf("Tamanho do vetor: %ld elementos.\n", tam_vetor);
	printf("Estado inicial do vetor: %s.\n", tipo_vetor);
	printf("Tempo de execucao (s): %lf\n", ((double)media_exe)/CLOCKS_PER_SEC);
	printf("Numero de comparacoes feitas: %ld\n", media_comp);
	printf("Numero de movimentacoes feitas: %ld\n", media_mov);

	free(vetor);

	return 0;
}
