#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include "fs.h"

int test(uint64_t fsize, uint64_t blksz);

#define NELEMS(x) (sizeof(x)/sizeof(x[0]))

static char *fname = "img";


int main(int argc, char **argv)/*{{{*/
{
	uint64_t fsizes[] = {1 << 20};
	uint64_t blkszs[] = {256};
	int i, j;

	for(i = 0; i < NELEMS(blkszs); i++) {
	for(j = 0; j < NELEMS(fsizes); j++) {
		printf("fsize %d blksz %d\n", (int)fsizes[j], (int)blkszs[i]);
		if(test(fsizes[j], blkszs[i])) exit(EXIT_FAILURE);
	}
	}

	exit(EXIT_SUCCESS);
}
/*}}}*/


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


int test(uint64_t fsize, uint64_t blksz)/*{{{*/
{
	int err;
	generate_file(fsize);
	struct superblock *sb = fs_format(fname, blksz);
	err = errno;
	if(blksz < MIN_BLOCK_SIZE) {
		if(err != EINVAL) printf("FAIL did not set errno\n");
		if(sb != NULL) printf("FAIL formatted too small blocks\n");
		return -1;
	}
	if(fsize/blksz < MIN_BLOCK_COUNT) {
		if(err != ENOSPC) printf("FAIL did not set errno\n");
		if(sb != NULL) printf("FAIL formatted too small volume\n");
		return -1;
	}
	if(sb == NULL) {
		printf("FAIL no sb\n");
		return -1;
	}
	if(fs_close(sb)) {
		printf("FAIL error on fs_close");
		return -1;
	}
	return 0;
}
/*}}}*/

