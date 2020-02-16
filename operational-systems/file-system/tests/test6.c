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
int fs_free_check(struct superblock **sb, uint64_t fsize, uint64_t blksz);
int fs_ops_test(struct superblock *sb);

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
	if(fs_free_check(&sb, fsize, blksz)) ERROR("FAIL fs_free_check\n");
	if(fs_ops_test(sb)) ERROR("FAIL fs_ops_test\n");
	if(fs_close(sb)) ERROR("FAIL error on fs_close");

	sb = fs_open(fname);
	if(!sb) ERROR("FAIL fs_open (2nd time)\n");
	if(fs_check(sb, fsize, blksz)) ERROR("FAIL fs_check\n");
	if(fs_free_check(&sb, fsize, blksz)) ERROR("FAIL fs_free_check\n");
	if(fs_ops_test(sb)) ERROR("FAIL fs_ops_test\n");

	if(fs_open(fname)) ERROR("FAIL opened FS twice\n");
	if(errno != EBUSY) ERROR("FAIL did not set errno EBUSY on fs reopen\n");

	if(fs_close(sb)) ERROR("FAIL error on fs_close");
	return 0;
}
/*}}}*/


int fs_free_check(struct superblock **sb, uint64_t fsize, uint64_t blksz)/*{{{*/
{
	long long usedblocks = fsize/blksz - (*sb)->freeblks;
	long long numblocks = fsize/blksz;
	unsigned long long freeblks = (*sb)->freeblks;

	if(usedblocks > 6) ERROR("FAIL used more than 6 blocks on empty fs\n");

	char *blkmap = malloc(fsize/blksz);
	assert(blkmap);
	memset(blkmap, 0, fsize/blksz);

	struct freepage *fp = malloc(blksz);
	assert(fp);

	lseek((*sb)->fd, (*sb)->freelist * blksz, SEEK_SET);
	read((*sb)->fd, fp, blksz);

	usedblocks = (*sb)->freeblks;
	uint64_t blknum = fs_get_block(*sb);
	while(blknum != 0 && blknum != ((uint64_t)-1)) {
		if(blknum >= numblocks) {
			ERROR("FAIL blknum >= numblocks\n");
		} else if(blkmap[blknum] != 0) {
			ERROR("FAIL blknum returned twice\n");
		} else {
			blkmap[blknum] = 1;
		}
		fs_close(*sb);
		*sb = fs_open(fname);
		blknum = fs_get_block(*sb);
		usedblocks--;
	}
	if((*sb)->freeblks != 0) ERROR("FAIL sb->freeblks != 0\n");
	if(usedblocks != 0) ERROR("FAIL cheese\n");

	fs_close(*sb);
	*sb = fs_open(fname);
	if((*sb)->freeblks != 0) ERROR("FAIL reopen sb->freeblks != 0\n");

	for(uint64_t i = 0; i < numblocks; i++) {
		if(!blkmap[i]) continue;
		fs_put_block(*sb, i);
	}
	if((*sb)->freeblks != freeblks) ERROR("FAIL sb->freeblks != freeblks\n");
	return 0;
}
/*}}}*/


int fs_ops_test(struct superblock *sb)/*{{{*/
{
	uint64_t freeblks = sb->freeblks;
	// printf("had %llu free blocks\n", (long long unsigned)sb->freeblks);

	if(fs_write_file(sb, "/test.1", "test.1", strlen("test.1")+1) < 0)
		ERROR("FAIL fs_write_file\n");
	if(fs_mkdir(sb, "/dir.1") < 0)
		ERROR("FAIL fs_mkdir\n");
	if(fs_write_file(sb, "/dir.1/test.2", "test.2", strlen("test.2")+1) < 0)
		ERROR("FAIL fs_write_file inside directory\n");

	char *dir = fs_list_dir(sb, "/");
	if(strcmp(dir, "test.1 dir.1/"))
		ERROR("FAIL fs_list_dir /\n");
	free(dir);

	dir = fs_list_dir(sb, "/dir.1");
	if(strcmp(dir, "test.2"))
		ERROR("FAIL fs_list_dir /dir.1\n");
	free(dir);

	// printf("have %llu free blocks\n", (long long unsigned)sb->freeblks);
	if(fs_unlink(sb, "/test.1") < 0)
		ERROR("FAIL fs_unlink /test.1\n");

	dir = fs_list_dir(sb, "/");
	if(strcmp(dir, "dir.1/"))
		ERROR("FAIL fs_list_dir\n");
	free(dir);

	if(fs_unlink(sb, "/dir.1/test.2") < 0)
		ERROR("FAIL fs_unlink /dir.1/test.2\n");
	if(fs_rmdir(sb, "/dir.1") < 0)
		ERROR("FAIL fs_rmdir /dir.1\n");

	dir = fs_list_dir(sb, "/");
	if(strcmp(dir, ""))
		ERROR("FAIL fs_list_dir /\n");
	free(dir);

	uint64_t freeblks2 = sb->freeblks;
	// printf("final %llu free blocks\n", (long long unsigned)sb->freeblks);

	if(freeblks != freeblks2) ERROR("FAIL freeblks after fs_ops\n");
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
