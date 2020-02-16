#include "tapes.h"

int isSmaller(Record *A, Record *B){
	int i = 0;
	int n;

	if(strlen(A->word) > strlen(B->word)) n = strlen(B->word);
	else n = strlen(A->word);

	while(i < n && A->word[i] == B->word[i]){ i++; }
	
	if(i == n){
		if(strlen(A->word) == strlen(B->word)){ //If the words are the same
			i = A->d; n = B->d; 
			if(i < n){ return 1; }
			else if(i > n){ return 0; }
			else{
				i = A->p; n = B->p; //A.p is ALWAYS different of B.p
				if(i < n){ return 1; }
				else if(i > n){ return 0; }
				else{ return 2; }
			}
		}else if(strlen(A->word) < strlen(B->word)){
			return 1;
		}else{
			return 0;
		}
	}else if(A->word[i] < B->word[i]){ return 1; }
	else{ return 0; }
	
	printf("ERROR!\n");
	return -1;
}

void copyRecord(Record *A, Record *B){ // Record A <- Record B
	if(A != B){
		strcpy(A->word, B->word);
		A->d = B->d;
		A->f = B->f;
		A->p = B->p;
	}
}

void buildUnsortedIndex(int D, char E[], char S[]){
	char buffer[BUFFER_SIZE];
	FILE *file1, *file2;
	int i, j, k;

	sprintf(buffer, "%sindex", S);
	file1 = fopen(buffer, "w");

	for(i = 0; i < D; i++){ //Document number
		j = 0; //Byte position
		
		sprintf(buffer, "%s%d", E, i+1);
		file2 = fopen(buffer, "r");

		while(fscanf(file2, "%s", buffer) != EOF){
			fprintf(file1, "%s %d %d %d\n", buffer, i+1, 1, j);
			j += strlen(buffer) + 1;
			for(k = 0; k < MAX_WORD_SIZE; k++) buffer[k] = '\0';
		}
		fclose(file2);
	}
	fclose(file1);
}

int buildTapes(char S[], int tapes_amount){
	int i, j, tapes_count;
	char buffer[BUFFER_SIZE], test;
	FILE *file1, *file2;
	Record *V;

	V = (Record *) malloc(tapes_amount * sizeof(Record));

	sprintf(buffer, "%sindex", S);
	file1 = fopen(buffer, "r");

	tapes_count = 0; test = '0';
	while(!feof(file1)){ // Take records from the index and put in V
		for(i = j = 0; i < tapes_amount; i++){ // Write records in the RAM to order after
			if(!feof(file1)){
				if(fscanf(file1, "%s %d %d %d", V[i].word, &(V[i].d), &(V[i].f), &(V[i].p)) != EOF){
					j++;
				}
			}
		}


		Quicksort(V, j); // Order V

		sprintf(buffer, "%s%d", S, tapes_count);
		file2 = fopen(buffer, "a");

		for(i = 0; i < j; i++){
			fprintf(file2, "%s %d %d %d", V[i].word, V[i].d, V[i].f, V[i].p);
		}
		
		fprintf(file2, "$");
		fclose(file2);
		
		if((tapes_count + 1) == tapes_amount){ test = '1'; }
		tapes_count = (tapes_count + 1) % tapes_amount;
	}

	fclose(file1);

	sprintf(buffer, "%sindex", S);
	remove(buffer);

	free(V);

	if(test == '1'){ return tapes_amount; }
	else{ return tapes_count; }
}

void intercalate(int tapes_amount, char S[]){
	/*
	*  V_status[x] - Is the status of the tapes:
	*  x = 1: block didin't finished yet
	*  x = 2: finished the block
	*  x = 3: end of blocks
	*/

	int i, j, ind_i, new_tape_number, old_tape_number;
	char c, *V_status, tests[3], buffer[BUFFER_SIZE], buffer2[BUFFER_SIZE];
	Record *V,  small;
	FILE *file1, **files;

	files = (FILE **) malloc(tapes_amount * sizeof(FILE *));

	V = (Record *) malloc(tapes_amount * sizeof(Record));
	V_status = (char *) malloc(tapes_amount * sizeof(char));
	for(i = 0; i < tapes_amount; i++) V_status[i] = '1';

	tests[0] = tests[1] = tests[2] = '1';
	while(tests[0] == '1'){ // There is more than one tape
		
	// Open the tapes and clean the memory array -------------------------------------------------
		for(i = 0; i < tapes_amount; i++){
			sprintf(buffer, "%s%d", S, i);
			files[i] = fopen(buffer, "r");
			V[i].word[0] = '-';
		}
	//--------------------------------------------------------------------------------------------
	// Select the new empty tape number to be written --------------------------------------------
		new_tape_number = tapes_amount; // Number of existent tapes to be intercalated
		old_tape_number = new_tape_number;
	//--------------------------------------------------------------------------------------------
		
		while(tests[1] == '1'){ // Didn't finished yet the blocks of the analyzes tapes
		
		// Put the first record from the Nth block of each tape in the memory --------------------
			for(i = 0; i < tapes_amount; i++){ // i = Analyed tape
				V_status[i] = '1';
				if(fscanf(files[i], "%s", V[i].word) != EOF){
					fscanf(files[i], "%d %d %d", &(V[i].d), &(V[i].f), &(V[i].p));
					fscanf(files[i], "%c", &c);
					if(c == '$'){
						V_status[i] = '2';
						if(fscanf(files[i], "%c", &c) != EOF){
							fseek(files[i], -1, SEEK_CUR);
						}else{
							V_status[i] = '3';
							fclose(files[i]);
							sprintf(buffer, "%s%d", S, i);
							remove(buffer);
						}
					}else{
						fseek(files[i], -1, SEEK_CUR);
					}
				}else{
					V_status[i] = '3';
					V[i].word[0] = '-';
					fclose(files[i]);
				}
			}
		//----------------------------------------------------------------------------------------
			
			while(tests[2] == '1'){ // Intercalation of the Nth block
			
			// Get the smallest record in the memory ---------------------------------------------
				i = 0;
				while(V[i].word[0] == '-'){ i++; }
				copyRecord(&small, &(V[i]));
				ind_i = i;
				while(i < tapes_amount){
					if(V[i].word[0] != '-' && isSmaller(&(V[i]), &small)){
						copyRecord(&small, &(V[i]));
						ind_i = i;
					}
					i++;
				}
			//------------------------------------------------------------------------------------
			// Write it in the tape (new_tape_number) --------------------------------------------
				sprintf(buffer, "%s%d", S, new_tape_number);
				file1 = fopen(buffer, "a");
				fprintf(file1, "%s %d %d %d", V[ind_i].word, V[ind_i].d, V[ind_i].f, V[ind_i].p);
				fclose(file1);
			//------------------------------------------------------------------------------------
			// Replace the record that was removed with another one from the same tape -----------
			//	-> If there isn't no more tapes in the Nth block, update the tapes status to '2' -
				if(V_status[ind_i] == '1'){
					fscanf(files[ind_i], "%s %d %d %d", V[ind_i].word, &(V[ind_i].d), &(V[ind_i].f), &(V[ind_i].p));
					fscanf(files[ind_i], "%c", &c);
					if(c == '$'){
						V_status[ind_i] = '2';
						if(fscanf(files[ind_i], "%c", &c) != EOF){
							fseek(files[ind_i], -1, SEEK_CUR);
						}else{
							V_status[ind_i] = '3';
							fclose(files[ind_i]);
							sprintf(buffer, "%s%d", S, ind_i);
							remove(buffer);
						}
					}else{
						fseek(files[ind_i], -1, SEEK_CUR);
					}
				}else{
					V[ind_i].word[0] = '-';
				}				
			//------------------------------------------------------------------------------------
			// Loop test -------------------------------------------------------------------------
				for(j = i = 0; j < tapes_amount; j++){
					if(V_status[j] == '1') i++;
				}
				if(i == 0){ tests[2] = '0'; }
			//------------------------------------------------------------------------------------
			
			}// There is a tape with status '1'
		
		// Write the rest records of the memory in the tape --------------------------------------
			sprintf(buffer, "%s%d", S, new_tape_number); 
			file1 = fopen(buffer, "a");

			// Select the smallest record of the memory
			j = 0;
			while(j < tapes_amount){
				i = j = 0;
				while(i < tapes_amount && V[i].word[0] == '-'){ i++; j++; }
				if(j < tapes_amount){
					copyRecord(&small, &(V[i]));
					ind_i = i;
					i++;
					while(i < tapes_amount){
						if(V[i].word[0] != '-' && isSmaller(&(V[i]), &small)){
							copyRecord(&small, &(V[i]));
							ind_i = i;
						}
						i++;
					}
					fprintf(file1, "%s %d %d %d", V[ind_i].word, V[ind_i].d, V[ind_i].f, V[ind_i].p);
					V[ind_i].word[0] = '-';
				}
			}
			fprintf(file1, "$");
			fclose(file1);
			tests[2] = '1';
		//----------------------------------------------------------------------------------------
		// Increase the new empty tape number-----------------------------------------------------
			new_tape_number++;
			if(new_tape_number == (2 * old_tape_number)){ new_tape_number = old_tape_number; }
		//----------------------------------------------------------------------------------------
		// Loop test -----------------------------------------------------------------------------
			for(i = j = 0; i < tapes_amount; i++){
				if(V_status[i] == '2') j++;
			}
			if(j == 0){
				tests[1] = '0';
			}else{
				tapes_amount = j;
			}
		//----------------------------------------------------------------------------------------
		
		}// Didn't finished yet the blocks of the analyzes tapes
	
	// Rename the new tapes to 0, 1, 2... --------------------------------------------------------
		for(i = old_tape_number, j = 0; i < new_tape_number; i++, j++){
			sprintf(buffer, "%s%d", S, i);
			sprintf(buffer2, "%s%d", S, j);
			rename(buffer, buffer2);
			V_status[j] = '1';
		}
		tapes_amount = j;
		tests[1] = '1';
	//--------------------------------------------------------------------------------------------
	// Loop test ---------------------------------------------------------------------------------
		for(i = j = 0; i < tapes_amount; i++){
			if(V_status[i] == '1' || V_status[i] == '2') j++;
		}
		if(j <= 1){ tests[0] = '0'; }
	//--------------------------------------------------------------------------------------------
	
	}// There is more than one tape
	
	free(V);
	free(V_status);
	free(files);
}

void putFrequency(char S[]){
	/*
	* This function gets the index ordered by name, document number ant position of the records, and has the goal
	* to put the frequency in the records. The index is already ordered, so the frequency won't change  this order,
	* because it's the third element in the sequency order (word, document number, frequency, position).
	*/

	int i, f, d1, d2, tmp;
	char w1[MAX_WORD_SIZE], w2[MAX_WORD_SIZE], buffer[BUFFER_SIZE];
	Record aux;
	FILE *file1, *file2, *file3;
	
	sprintf(buffer, "%s0", S);
	file1 = fopen(buffer, "r");
	file2 = fopen(buffer, "r");
	sprintf(buffer, "%sindex", S);
	file3 = fopen(buffer, "w");
 	
	f = 1; w2[0] = '-'; d2 = -1;
	while(!feof(file1)){
		fscanf(file1, "%s", w1);
		if(fscanf(file1, "%d %d %d", &d1, &tmp, &tmp) != EOF){			
			if(w2[0] != '-'){
				if(!strcmp(w1, w2) && d1 == d2){
					f++;
				}else{
					for(i = 0; i < f; i++){
						fscanf(file2, "%s %d %d %d", aux.word, &(aux.d), &(aux.f), (&aux.p));
						fprintf(file3, "%s,%d,%d,%d\n", aux.word, aux.d, f, aux.p);
					}
					f = 1;
				}
			}
			strcpy(w2, w1);
			d2 = d1;				
		}
	}

	while(!feof(file2)){
		fscanf(file2, "%s %d %d %d", aux.word, &(aux.d), &(aux.f), (&aux.p));
		if(aux.word[0] == '$') break;
		fprintf(file3, "%s,%d,%d,%d\n", aux.word, aux.d, f, aux.p);
	}

	fclose(file1);
	fclose(file2);
	fclose(file3);
	
	sprintf(buffer, "%s0", S);
	remove(buffer);
}