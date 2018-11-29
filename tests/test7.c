/*
 * DCC605F5: Filesystem implementation programming assignment
 * Test to inserting, extract and test diff between files
 * 
 * Created by:
 *  Yuri Diego Santos Niitsuma <ignitzhjfk@gmail.com>
 *  Cassios Kley Martins Marques <cassios.kmm@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include "fs.h"

struct superblock * sb;

// Command
void generate_file(uint64_t fsize)/*{{{*/
{
	char *buf = malloc(fsize);
	if(!buf) { perror(NULL); exit(EXIT_FAILURE); }
	memset(buf, 0, fsize);
	unlink("img");
	FILE *fd = fopen("img", "w");
	fwrite(buf, 1, fsize, fd);
	fclose(fd);
}
/*}}}*/

// Get size of file inside files/
// NOT inside IMG file
long get_size_of_file(char * file_name)/*{{{*/
{
	char * file_path;
	long r;

	file_path = malloc((strlen(file_name) + strlen("files"))*sizeof(char));
	strcpy(file_path, "files");

	if (file_name[0] == '.')
		strcat(file_path, file_name + 1);
	else
		strcat(file_path, file_name);

	FILE * fp = fopen(file_path, "rb");
	if (fp == 0) exit(EXIT_FAILURE);

	fseek(fp, 0, SEEK_END);
	r = ftell(fp);
	fclose(fp);

	return r;
}
/*}}}*/

// insert file in img file
int insert_files()/*{{{*/
{
	printf("DEBUG: Inserting files\n");

	char * file_name = NULL;
	char * file_path ;
	char *buffer;
	size_t len = 0;
	long file_len = 0;

	FILE *fp;


	// Creating dirs
	FILE *fList = fopen("file_list.txt", "r");
	if (fList == NULL)
		exit(EXIT_FAILURE);

	getline(&file_name, &len, fList);
	if (strcmp("DIRS:\n", file_name) != 0)
		exit(EXIT_FAILURE);

	while (getline(&file_name, &len, fList) != -1) {
		if (strcmp("FILES:\n", file_name) == 0)
			break;

		if (strcmp(".\n", file_name) != 0)
		{
			file_name[strlen(file_name) - 1] = '\0';
			fs_mkdir(sb, file_name + 1); // remove initial .
		}
	}

	// inserting files

	while(getline(&file_name, &len, fList) != -1) {
		file_name[strlen(file_name) - 1] = '\0';
		file_name++;

		file_path = malloc((strlen(file_name) + strlen("files"))*sizeof(char));
		strcpy(file_path, "files");
		strcat(file_path, file_name);

		fp = fopen(file_path, "rb");
		if (fp == NULL) exit(EXIT_FAILURE);
		free(file_path);
		
		fseek(fp, 0, SEEK_END);
		file_len = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		if (file_len == -1) {
			fprintf(stderr, "DEBUG: ERROR in file size");
			return -1;
		}

		buffer = malloc(file_len);
		fread(buffer, 1, file_len, fp);
		fs_write_file(sb, file_name, buffer, file_len);

		file_name--; // hotfix if will free memory
		fclose(fp);
	}

	fclose(fList);
	return 0;
}
/*}}}*/

// insert file in img file
int extract_files(struct superblock * sb)/*{{{*/
{
	printf("DEBUG: Extracting files \"extract_files\"\n");

	char * file_name = NULL;
	size_t len = 0;
	char * file_path;

	char * buffer;
	long file_len = 0;

	FILE *fList = fopen("file_list.txt", "r");
	FILE *fp;

	getline(&file_name, &len, fList);
	if (strcmp("DIRS:\n", file_name) != 0)
		exit(EXIT_FAILURE);
	while (getline(&file_name, &len, fList) != -1)
		if (strcmp("FILES:\n", file_name) == 0)
			break;
	
	while(getline(&file_name, &len, fList) != -1) {
		printf("-------------------------\n");
		printf("DEBUG: Extracting %s\n", file_name);

		file_name[strlen(file_name) - 1] = '\0';

		file_path = malloc((strlen(file_name + 1) + strlen("extract"))*sizeof(char));
		strcpy(file_path, "extract");
		strcat(file_path, file_name + 1);
		
		file_len = get_size_of_file(file_name);
		
		if (file_len == -1) {
			fprintf(stderr, "ERROR: file %s does not exist", file_name);
			return -1;
		}

		buffer = (char*) malloc(file_len * sizeof(char));

		file_len = fs_read_file(sb, file_name + 1, buffer, file_len);
		if (file_len == -1) {
			fprintf(stderr, "ERROR: ERROR in file size");
			return -1;
		}


		fp = fopen(file_path, "wb");
		if (fp == NULL)
			exit(EXIT_FAILURE);

		fwrite(buffer, 1, file_len, fp);
		fclose(fp);
	    
		free(file_path);
		// Logo abaixo da erro se o nome do arquivo tiver extensão "ex: .zip"
		free(buffer);
	}

	fclose(fList);
	return 0;
}
/*}}}*/

#define ERROR(str) { puts(str); return -1; }

int test1(uint64_t fsize, uint64_t blksz)/*{{{*/
{
	int err;
	generate_file(fsize);

	sb = fs_open("img");
	sb = fs_format("img", blksz);

	err = errno;

	if(blksz < MIN_BLOCK_SIZE) {
		if(err != EINVAL) ERROR("FAIL did not set errno\n");
		if(sb != NULL) ERROR("FAIL formatted too small blocks\n");
		return 0;
	}
	if(fsize/blksz < MIN_BLOCK_COUNT) {
		if(err != ENOSPC) ERROR("FAIL did not set errno\n");
		if(sb != NULL) ERROR("FAIL formatted too small volume\n");
		return 0;
	}
	if(sb == NULL) ERROR("FAIL no sb\n");

	fs_close(sb);

	return 0;
}
/*}}}*/

int test2()/*{{{*/
{
	sb = fs_open("img");

	if (insert_files(sb) != 0 && sb == 0) {
		fprintf(stderr, "ERROR: Unknow ERROR\n");
		fprintf(stderr, "DEBUG: errno = %d\n", errno);
		return EXIT_FAILURE;
	}
	
	fs_close(sb);
	return 0;
}
/*}}}*/

int test3()/*{{{*/
{
	sb = fs_open("img");

	if (extract_files(sb) != 0 && sb == 0) {
		fprintf(stderr, "ERROR: Unknow ERROR\n");
		fprintf(stderr, "DEBUG: errno = %d\n", errno);
		return EXIT_FAILURE;
	}

	fs_close(sb);
	return 0;
}
/*}}}*/


int main(int argc, char const *argv[])/*{{{*/
{
	uint64_t fsize;
	uint64_t blksz;

	if (argc != 3) {
		fprintf(stderr, "give 2 args\n");
		return EXIT_FAILURE;
	}

	fsize = 1 << strtoul(argv[1], NULL, 0);
	blksz = strtoul(argv[2], NULL, 0);

	// Generate img file
	test1(fsize, blksz);
	// Inserting files
	test2();
	// Extracting the same files
	test3();

	return 0;
}
/*}}}*/