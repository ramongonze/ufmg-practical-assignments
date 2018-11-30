#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include "fs.h"

struct superblock * fs_format(const char *fname, uint64_t blocksize){
	long int size;
	int i;
	struct superblock *sb;
	struct freepage fp;
	struct nodeinfo *nodei;
	struct inode in;
	FILE* f;

	// Verfify wheter the block size is less than MIN_BLOCK_SIZE
	if(blocksize < MIN_BLOCK_SIZE){
		errno = EINVAL;
		return NULL;
	}

	// Gets the file size
	f = fopen(fname, "r");
	if(f == NULL) return NULL;
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fclose(f);

	// Verify wheter there is enough space in =fname
	if(size/blocksize < MIN_BLOCK_COUNT){
		errno = ENOSPC;
		return NULL;
	}

	// Build the superblock
	sb = (struct superblock*) malloc(blocksize);

	sb->magic = 0xdcc605f5;
	sb->blks = size/blocksize;
	sb->blksz = blocksize;
	sb->freeblks = sb->blks - 3; // The empty fs has the sb, inode and nodeinfo of root
	sb->root = 2; // Block where root is keep
	sb->freelist = 3; // First free block (4º position)
	sb->fd = open(fname, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	if(sb->fd < 0){
		// There was an error opening the file.
		free(sb);
		errno = EBADF;
		return NULL;
	}

	// // Check wheter the fs is already opened
	// if(flock(sb->fd, LOCK_NB | LOCK_EX) == -1){
	// 	errno = EBUSY;
	// 	return NULL;
	// }

	// Writes the superblock in the =fname
	if(write(sb->fd, sb, sb->blksz) < 0){
		// If the return of write() is negative, there was an error writing
		close(sb->fd);
		free(sb);
		return NULL;
	}

	// Set the root directory (we are writing in the second block)
	nodei = (struct nodeinfo*) malloc(sb->blksz);
	nodei->size = 0;
	strcpy(nodei->name, "/\0");
	write(sb->fd, nodei, sb->blksz);
	free(nodei);

	// Fill attributes of root iNode (we are writing in the third block of fs)
	in.mode = IMDIR;
	in.parent = 0;
	in.next = 0;
	in.meta = 1;
	write(sb->fd, &in, sb->blksz);

	// Initialize the list of free blocks
	for(i = 3; i < sb->blks-1; i++){
		fp.next = i+1;
		write(sb->fd, &fp, sb->blksz);
	}

	// The last block has its 'next' equals to 0.
	fp.next = 0;
	write(sb->fd, &fp, sb->blksz);
	
	return sb;
}

struct superblock * fs_open(const char *fname){
	int fd;
	struct superblock *sb;

	fd = open(fname, O_RDWR);
	
	if(fd < 0){
		// Some error happened while opening the file
		return NULL;
	}

	if(flock(fd, LOCK_NB | LOCK_EX) < 0){
		close(fd);
		errno = EBUSY;
		return NULL;
	}


	sb = (struct superblock*) malloc(sizeof(struct superblock));
	lseek(fd, 0, SEEK_SET);
	if(read(fd, sb, sizeof(struct superblock)) < 0){
		free(sb);
		close(fd);
		return NULL;
	}

	if(sb->magic != 0xdcc605f5){
		flock(fd, LOCK_NB | LOCK_UN);
		close(fd);
		errno = EBADF;
		free(sb);
		return NULL;
	}

	return sb;
}

int fs_close(struct superblock *sb){

	if(sb == NULL){
		return -1;
	}

	if(sb->magic != 0xdcc605f5){
	    errno = EBADF;
	    return -1;
	}

	// Remove the lock from file
	if(flock(sb->fd, LOCK_NB | LOCK_UN) < 0){
		errno = EBUSY;
		return -1;
	}

	if(close(sb->fd) < 0){
		return -1;
	}

	free(sb);

	return 0;
}

uint64_t fs_get_block(struct superblock *sb){
	uint64_t freeb;
	struct freepage *firstfp;
	
	if(sb->freeblks == 0){
		// There are no free blocks
		errno = ENOSPC;
		return 0;
	}

	// Puts the pointer in the position of the first free block
	lseek(sb->fd, (sb->freelist * sb->blksz), SEEK_SET);
	firstfp = (struct freepage*) malloc(sb->blksz);
	if(read(sb->fd, firstfp, sb->blksz) < 0){
		free(firstfp);
		return ((uint64_t)0)-1;
	}

	// The number of the first free block
	freeb = sb->freelist;

	// freelist is now pointing to the next free block from the list
	sb->freelist = firstfp->next;
	sb->freeblks--;

	free(firstfp);
	
	lseek(sb->fd, 0, SEEK_SET);
	if(write(sb->fd, sb, sb->blksz) < 0){
		return 0;
	}

	return freeb;
}

// /* Put =block back into the filesystem as a free block.  Returns zero on
//  * success or a negative value on error.  If there is an error, errno is set
//  * accordingly. */
int fs_put_block(struct superblock *sb, uint64_t block){
	struct freepage fp;

	// The block =block will be the first free block in the list of free blocks
	fp.next = sb->freelist;
	sb->freelist = block;
	sb->freeblks++;

	// Update the sb attributes
	lseek(sb->fd, 0, SEEK_SET);
	if(write(sb->fd, sb, sb->blksz) < 0){
		return -1;
	}

	// Write =block in the fs
	lseek(sb->fd, (sb->freelist * sb->blksz), SEEK_SET);
	if(write(sb->fd, &fp, sb->blksz) < 0){
		return -1;
	}

	return 0;
}

int fs_write_file(struct superblock *sb, const char *fname, char *buf, size_t cnt){

}

// ssize_t fs_read_file(struct superblock *sb, const char *fname, char *buf,
//                      size_t bufsz);

// int fs_unlink(struct superblock *sb, const char *fname);

// int fs_mkdir(struct superblock *sb, const char *dname);

// int fs_rmdir(struct superblock *sb, const char *dname);

// char * fs_list_dir(struct superblock *sb, const char *dname);
