#include "pilha.h"

int main(){

	char expressao[MAX_EXPRESSAO], expressao_aux[MAX_EXPRESSAO], *buffer;
	char resultado[MAX_EXPRESSAO], *cont_caso;
	int qtd_interrogacoes;
	long long int qtd_testes, i, k, l, i_cont;
	Pilha *p;

//Inicialização dos vetores
	for(i = 0; i < MAX_EXPRESSAO; i++){
		expressao[i] = '\0';
		expressao_aux[i] = '\0';
	}
//------------------------

//Leitura da entrada
	fgets(expressao, MAX_EXPRESSAO, stdin);
	expressao[strlen(expressao) - 1] = '\0';
	scanf("%s", resultado);
//------------------
	qtd_interrogacoes = 0;
	for(i = 0; i < MAX_EXPRESSAO; i++) if(expressao[i] == '?') qtd_interrogacoes++;
	qtd_testes = 1;
	for(i = 0; i < qtd_interrogacoes; i++) qtd_testes *= 2;
	cont_caso = (char *) malloc(qtd_interrogacoes * sizeof(char));

//Faz todos os casos de teste possíveis (Força Bruta)
	for(i = 0; i < qtd_testes; i++){
		strcpy(expressao_aux, expressao);
		p = cria_pilha_vazia();

	//Constroi o vetor de teste
		for(k = qtd_interrogacoes - 1, l = i; k >= 0; k--){
			cont_caso[k] = (l - ((l/2)*2)) + 48;
			l /= 2;
		}

	//Empilha cada elemento da fila se houver uma operação, a faz com os dois ultimos elementos da pilha
		i_cont = 0;
		buffer = strtok(expressao_aux, " ");
		while(buffer != NULL){
			if(buffer[0] == '?'){
				empilha(p, buffer);
				faz_operacao(p, cont_caso[i_cont]);
				if(atol(p->ultimo->e) > atol(resultado)) break;
				i_cont++;
			}else{
				empilha(p, buffer);
			}
			buffer = strtok(NULL, " ");
		}

	//Testa se o elemento que sobrou na pilha é igual ao resultado
		if(strcmp(p->ultimo->e, resultado) == 0){
			for(k = 0; k < qtd_interrogacoes; k++){
				if(cont_caso[k] == '0') printf("+");
				else printf("*");
			}
			printf("\n");
		}

		destroi_pilha(p);
		
	}//Final do loop principal de Força Bruta
	free(cont_caso);
//---------------------------------------------------

	return 0;
}