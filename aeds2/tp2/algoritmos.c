#include "algoritmos.h"

void criaVetor(TipoItem *vetor, TipoIndice tam_vetor, char tipo_ordenacao){
	int i;
	srand((int)clock());
	switch(tipo_ordenacao){
		case 'a':
			for(i = 1; i <= tam_vetor; i++){
				vetor[i].chave = rand() % tam_vetor;
			}
			break;
		case 'c':
			for(i = 1; i <= tam_vetor; i++){
				vetor[i].chave = i;
			}
			break;
		case 'd':
			for(i = 1; i <= tam_vetor; i++){
				vetor[i].chave = tam_vetor - i + 1;
			}
			break;
		case 'q':
		//Cria-se primeiramente 90% do vetor, ordenado
			for(i = 1; i <= (9*tam_vetor)/10; i++){
				vetor[i].chave = i;
			}
		//Desordena 10% dele
			for(i = (9*tam_vetor)/10 + 1; i <= tam_vetor; i++){
				vetor[i].chave = rand() % tam_vetor;
			}
			break;
		default: 
			fprintf(stderr, "ERRO: Comando invalido para ordenacao de vetores.\n");
			exit(-1);
	}
}

void Selecao(TipoItem *A, TipoIndice n, long *comparacoes, long *movimentacoes){
	TipoIndice i, j, Min;
	TipoItem x;

	*comparacoes = 0;
	*movimentacoes = 0;
	for (i = 1; i <= n - 1; i++){
		Min = i;
		for (j = i + 1; j <= n; j++){
			(*comparacoes)++;
			if (A[j].chave < A[Min].chave){
				Min = j;
			}
		}
		x = A[Min];
		A[Min] = A[i];
		A[i] = x;
		(*movimentacoes) += 3;
	}
}

void Insercao(TipoItem *A, TipoIndice n, long *comparacoes, long *movimentacoes){
	TipoIndice i, j;
	TipoItem x;

	*comparacoes = 0;
	*movimentacoes = 0;

	for (i = 2; i <= n; i++){
		x = A[i];
		j = i - 1;
		A[0] = x; /* sentinela */
		while (x.chave < A[j].chave){
			(*comparacoes)++;
			A[j+1] = A[j];
			(*movimentacoes)++;
			j--;
		}
		(*comparacoes)++;
		A[j+1] = x;
		(*movimentacoes) += 3;
	}
}

void Bolha(TipoItem *A, TipoIndice n, long *comparacoes, long *movimentacoes){
	TipoIndice i, j;
	TipoItem x;

	*comparacoes = 0;
	*movimentacoes = 0;

	for (i = 1; i <= n; i++){
		for (j = 1; j <= (n-i); j++){
			(*comparacoes)++;
			if (A[j].chave > A[j+1].chave){
				x = A[j];
				A[j] = A[j+1];
				A[j+1] = x;
				(*movimentacoes) += 3;
			}
		}
	}
}

void Shellsort(TipoItem *A, TipoIndice n, long *comparacoes, long *movimentacoes){
	TipoIndice i, j, h = 1;
	TipoItem x;

	*comparacoes = 0;
	*movimentacoes = 0;

	do h = h * 3 + 1; while (h < n);
	do{
		h /= 3;
		for (i = h + 1; i <= n; i++){
			x = A[i]; j = i;
			while (A[j - h].chave > x.chave){
				(*comparacoes)++;
				A[j] = A[j - h]; j -= h;
				(*movimentacoes)++;
				if (j <= h) break;
			}
			(*comparacoes)++;
			A[j] = x;
			(*movimentacoes) += 2;
		}
	} while (h != 1);
}

//QUICKSORT-------------------
void Quicksort(TipoItem *A, TipoIndice n, long *comparacoes, long *movimentacoes){
	*comparacoes = 0;
	*movimentacoes = 0;
	Ordena(1, n, A, comparacoes, movimentacoes);
}

void Ordena(TipoIndice Esq, TipoIndice Dir, TipoItem *A, long *comparacoes, long *movimentacoes){
	TipoIndice i, j;
	Particao(Esq, Dir, &i, &j, A, comparacoes, movimentacoes);
	if (Esq < j) Ordena(Esq, j, A, comparacoes, movimentacoes);
	if (i < Dir) Ordena(i, Dir, A, comparacoes, movimentacoes);
}

void Particao(TipoIndice Esq, TipoIndice Dir, TipoIndice *i, TipoIndice *j, TipoItem *A, long *comparacoes, long *movimentacoes){
	TipoItem x, w;
	TipoIndice k;
	*i = Esq;
	*j = Dir;
	k = (*i + *j) / 2;

	//Obtém o pivô, sendo esse a mediana entre o primeiro, o último e o elemento central do vetor
	(*comparacoes)++;
	if(A[*i].chave <= A[*j].chave){
		(*comparacoes)++;
		if(A[*j].chave <= A[k].chave){
			(*movimentacoes)++;
			x = A[*j];
		}else if(A[k].chave <= A[*i].chave){
			(*comparacoes)++;
			(*movimentacoes)++;
			x = A[*i];
		}else{
			(*comparacoes)++;
			(*movimentacoes)++;
			x = A[k];
		}
	}else if(A[*j].chave <= A[k].chave){
		*(comparacoes) += 2;
		if(A[k].chave <= A[*i].chave){
			(*movimentacoes)++;
			x = A[k];
		}else{
			(*movimentacoes)++;
			x = A[*i];
		}
	}else{
		(*comparacoes)++;
		(*movimentacoes)++;
		x = A[*j];
	}

	do{
		while (x.chave > A[*i].chave){
			(*comparacoes)++;
			(*i)++;
		}
		(*comparacoes)++;
		while (x.chave < A[*j].chave){
			(*comparacoes)++;
			(*j)--;
		}
		(*comparacoes)++;
		if (*i <= *j){
			w = A[*i];
			A[*i] = A[*j];
			A[*j] = w;
			(*movimentacoes) += 3;
			(*i)++; (*j)--;
		}
	}while (*i <= *j);
}
//----------------------------

//HEAPSORT--------------------
void Heapsort(TipoItem *A, TipoIndice n, long *comparacoes, long *movimentacoes){
   TipoIndice Esq, Dir; 
   TipoItem x;

   *comparacoes = 0;
   *movimentacoes = 0;

   Constroi(A, n, comparacoes, movimentacoes);
   Esq = 1; 
   Dir = n; 
   while (Dir > 1) {
      x = A[1]; 
      A[1] = A[Dir]; 
      A[Dir] = x; 
      (*movimentacoes) += 3;
      Dir--; 
      Refaz(Esq, Dir, A, comparacoes, movimentacoes); 
   } 
}

void Constroi(TipoItem *A, TipoIndice n, long *comparacoes, long *movimentacoes){
   TipoIndice Esq; 
   Esq = n / 2 + 1; 
   while (Esq > 1) { 
      Esq--; 
      Refaz(Esq, n, A, comparacoes, movimentacoes); 
   }
}

void Refaz(TipoIndice Esq, TipoIndice Dir, TipoItem *A, long *comparacoes, long *movimentacoes){
   TipoIndice i = Esq, j;
   TipoItem x;
   j = i * 2; 
   x = A[i]; 
   while (j <= Dir) { 
      if (j < Dir) {
      	(*comparacoes)++; 
         if (A[j].chave < A[j+1].chave) j++; 
      } 
      (*comparacoes)++;
      if (x.chave >= A[j].chave) break; 
      A[i] = A[j]; 
      (*movimentacoes)++;
      i = j; 
      j = i * 2; 
   } 
   A[i] = x; 
   (*movimentacoes)++;
}

//----------------------------

//MERGESORT-------------------
void Mergesort(TipoItem *A, TipoIndice inicio, TipoIndice fim, long *comparacoes, long *movimentacoes){
	TipoIndice meio;
	if(inicio < fim){
		meio = floor((inicio + fim)/2);
		Mergesort(A, inicio, meio, comparacoes, movimentacoes);
		Mergesort(A, meio+1, fim, comparacoes, movimentacoes);
		merge(A, inicio, meio, fim, comparacoes, movimentacoes);
	}
}

void merge(TipoItem *A, TipoIndice inicio, TipoIndice meio, TipoIndice fim, long *comparacoes, long *movimentacoes){
	TipoIndice p1, p2, tamanho, i, j, k;
	TipoItem *temp;
	int fim1 = 0, fim2 = 0;
	
	tamanho = fim-inicio+1;
	p1 = inicio;
	p2 = meio+1;
	temp = (TipoItem *) malloc(tamanho * sizeof(TipoItem));
	if(temp != NULL){
		for(i = 0; i < tamanho; i++){
			if(!fim1 && !fim2){
				if(A[p1].chave < A[p2].chave)
					temp[i] = A[p1++];
				else
					temp[i] = A[p2++];

				if(p1 > meio) fim1 = 1;
				if(p2 > fim) fim2 = 1;
				(*comparacoes) += 3;
				(*movimentacoes)++;
			}else{
				if(!fim1)
					temp[i] = A[p1++];
				else 
					temp[i] = A[p2++];
				(*movimentacoes)++;
			}
		}
		for(j = 0, k = inicio; j < tamanho; j++, k++){
			A[k] = temp[j];
			(*movimentacoes)++;
		}
	}
	free(temp);

}
//----------------------------

//RADIXSORT-------------------
void Radixsort(TipoItem * A, TipoIndice tamanho, long *comparacoes, long *movimentacoes){
  TipoIndice i, *semi_ordenado, digito_significante = 1;
  TipoIndice maior_numero = MaiorNumero(A, tamanho);
 
  semi_ordenado = (TipoIndice *) malloc(tamanho * sizeof(TipoIndice));
  
  *comparacoes = 0;
  *movimentacoes = 0;

  while (maior_numero / digito_significante > 0){
    int bucket[10] = { 0 };
    
    for (i = 0; i < tamanho; i++)
      bucket[(A[i+1].chave / digito_significante) % 10]++;
    
    for (i = 1; i < 10; i++)
      bucket[i] += bucket[i - 1];
    
    for (i = tamanho - 1; i >= 0; i--){
        semi_ordenado[--bucket[(A[i+1].chave / digito_significante) % 10]] = A[i+1].chave;
        (*movimentacoes)++;
    }
       
    for (i = 0; i < tamanho; i++){
        A[i+1].chave = semi_ordenado[i];
        (*movimentacoes)++;
    }
    
    digito_significante *= 10;
  }

  free(semi_ordenado);
}

TipoIndice MaiorNumero(TipoItem *A, TipoIndice tamanho){
  
  TipoIndice i;
  TipoIndice maior = -1;
  
  for(i = 1; i <= tamanho; i++){
    if(A[i].chave > maior)
      maior = A[i].chave;
  }
  
  return maior;
}
//---------------------------
