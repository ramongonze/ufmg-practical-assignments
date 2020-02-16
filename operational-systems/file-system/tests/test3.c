#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include "fs.h"

int test(uint64_t fsize, uint64_t blksz);
int fs_check(const struct superblock *sb, uint64_t fsize, uint64_t blksz);

#define NELEMS(x) (sizeof(x)/sizeof(x[0]))

static char *fname = "img";


int main(int argc, char **argv)/*{{{*/
{
	uint64_t fsizes[] = {1 << 19, 1 << 20, 1 << 21, 1<<22};
	uint64_t blkszs[] = {64, 128, 256, 512, 1024};
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


#define ERROR(str) { puts(str); return -1; }
int test(uint64_t fsize, uint64_t blksz)/*{{{*/
{
	int err;
	generate_file(fsize);
	struct superblock *sb = fs_format(fname, blksz);
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

	if(fs_check(sb, fsize, blksz)) ERROR("FAIL fs_check\n");
	if(fs_close(sb)) ERROR("FAIL error on fs_close");

	return 0;
}
/*}}}*/


int fs_check(const struct superblock *sb, uint64_t fsize, uint64_t blksz)/*{{{*/
{
	if(sb->magic != 0xdcc605f5) ERROR("FAIL magic\n");
	if(sb->blks != fsize/blksz) ERROR("FAIL number of blocks\n");
	if(sb->blksz != blksz) ERROR("FAIL block size\n");

	struct inode *inode = malloc(blksz);
	if(!inode) { perror(NULL); exit(EXIT_FAILURE); }

	lseek(sb->fd, sb->root * blksz, SEEK_SET);
	read(sb->fd, inode, blksz);

	if(inode->mode != IMDIR) ERROR("FAIL root IMDIR\n");
	if(inode->next != 0) ERROR("FAIL root next\n");

	struct nodeinfo *info = malloc(blksz);
	assert(info);

	lseek(sb->fd, inode->meta * blksz, SEEK_SET);
	read(sb->fd, info, blksz);

	if(info->size != 0) ERROR("FAIL root size\n");
	if(info->name[0] != '/' || info->name[1] != '\0') ERROR("FAIL root name\n");

	free(info);
	free(inode);
	return 0;
}
/*}}}*/
