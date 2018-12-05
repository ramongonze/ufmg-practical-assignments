#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include <math.h>
#include "fs.h"

#define MAX_NAME 100 // Max files' name
#define MAX_SUBFOLDERS 100 // Max number of subfolders in a path
#define MAX_FILE_SIZE 5000 // Max file size (in blocks)
#define MAX_PATH_NAME 4000// Max length of a path's name

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
	int i, j, k, current_allocated_blocks;
	int blocks[MAX_FILE_SIZE];
	int num_elements_in_path, num_new_blocks, new_file;
	char files[MAX_SUBFOLDERS][MAX_NAME];
	char *token;
	char *name;
	struct inode in, in2;
	struct nodeinfo info, info2;

	name = (char*) malloc(MAX_PATH_NAME*sizeof(char));
	strcpy(name, fname);

	// Separate the subfolders in a vector of strings
	i = 0;
	token = strtok(name, "/"); // Root
	while(token != NULL){
		strcpy(files[i], token);
		token = strtok(NULL, "/");
		i++;
	}
	num_elements_in_path = i;

	// Root iNode
	lseek(sb->fd, sb->root*sb->blksz, SEEK_SET);
	read(sb->fd, &in, sb->blksz);

	// Root nodeinfo
	lseek(sb->fd, sb->blksz, SEEK_SET);
	read(sb->fd, &info, sb->blksz);

	// Go trought every folder in the path, until reach the file, if it exists
	for(j = 0; j < num_elements_in_path; j++){
		// Check every element inside the current directory
		while(1){
			// Check if the element is in the current inode
			for(k = 0; k < info.size; k++){
				// Inode of a file
				lseek(sb->fd, in.links[k]*sb->blksz, SEEK_SET);
				read(sb->fd, &in2, sb->blksz);
			
				// Check if we are in a child inode
				if(in2.mode == IMCHILD){
					// Jump to the first inode
					lseek(sb->fd, in2.parent*sb->blksz, SEEK_SET);
					read(sb->fd, &in2, sb->blksz);
				}
				
				// Get the file's nodeinfo
				lseek(sb->fd, in2.meta*sb->blksz, SEEK_SET);
				read(sb->fd, &info2, sb->blksz);

				if(strcmp(info2.name, files[j]) == 0){
					break;
				}
			}

			if(strcmp(info2.name, files[j]) == 0){
				// The subfolder or file has been found in the current inode
				if(j == (num_elements_in_path-1)){
					// The file has been found
					blocks[0] = in2.meta;
					blocks[1] = in.links[k];
					new_file = 0;
				}

				break;
			}else{
				// The subfolder or file has not been found in the current inode
				
				if(j == (num_elements_in_path-1)){
					// The file does not exists. Create a new file
				
					// New nodeinfo
					blocks[0] = fs_get_block(sb);
					strcpy(info2.name, files[j]);
					info2.size = sb->blksz - 20;

					// New inode
					blocks[1] = fs_get_block(sb);
					in2.meta = blocks[0];
					in2.next = 0;

					new_file = 1;
					break;
				}else if(in.next == 0){
					// No such directory
					errno = ENOENT;
					return -1;
				}
			}

			// Jump to the next inode
			lseek(sb->fd, in.next*sb->blksz, SEEK_SET);
			read(sb->fd, &in, sb->blksz);
		}

		// Jump to the next directory
		info = info2;
		in = in2;
	}

	// Write the nodeinfo
	lseek(sb->fd, blocks[0]*sb->blksz, SEEK_SET);
	write(sb->fd, &info, sb->blksz);			


	// blocks[0] = Contains the block of the nodeinfo
	// blocks[1] = Contains the block of the first inode

	if(new_file){
		// Number of blocks required to write =buf
		num_new_blocks = ceil(((float)cnt)/(sb->blksz-20.0));

		// Get new blocks, if necessary
		for(i = 0; i < num_new_blocks-1; i++){
			blocks[i+2] = fs_get_block(sb);
		}

	}else{
		current_allocated_blocks = ceil(((float)info.size)/(sb->blksz-20.0));
		num_new_blocks = ceil(((float)cnt)/(sb->blksz-20.0));
		
		if(current_allocated_blocks < num_new_blocks || current_allocated_blocks == num_new_blocks){
			i = 2;
			in2 = in;
			while(in2.next != 0){
				blocks[i] = in.next;
				lseek(sb->fd, blocks[i]*sb->blksz, SEEK_SET);
				read(sb->fd, &in2, sb->blksz);
				i++;
			}

			// Get the rest of blocks
			for(i = current_allocated_blocks+1; i <= num_new_blocks; i++){
				blocks[i] = fs_get_block(sb);
			}

		}else{
			// There are more inodes than the necessary
			i = 2;
			in2 = in;
			while(in2.next != 0){
				blocks[i] = in.next;
				lseek(sb->fd, blocks[i]*sb->blksz, SEEK_SET);
				read(sb->fd, &in2, sb->blksz);
				i++;
			}

			// Free the excess blocks 
			for(i = current_allocated_blocks; i > num_new_blocks; i--){
				if(fs_put_block(sb, blocks[i]) != 0){
					return -1;
				}
			}
		}
	}


	// Write the first inode
	in.mode = IMREG;
	in.parent = blocks[1];
	in.meta = blocks[0];
	if(num_new_blocks == 1){
		in.next = 0;
	}else{
		in.next = blocks[2];
	}

	lseek(sb->fd, blocks[1]*sb->blksz, SEEK_SET);
	write(sb->fd, &in, sb->blksz);

	// Write the inodes children, if there is anyone
	in.mode = IMCHILD;
	in.parent = blocks[1];

	// Write the buffer in all inodes, according to number of blocks in vector =blocks
	for(i = 2; i <= num_new_blocks; i++){
		// Write the nodeinfo
		in.meta = blocks[i-1];
		if(i < num_new_blocks){
			in.next = blocks[i+1];					
		}else{
			in.next = 0;
		}
	
		lseek(sb->fd, blocks[i]*sb->blksz, SEEK_SET);
		write(sb->fd, &in, sb->blksz);			
	}	

	// Update sb
	lseek(sb->fd, 0, SEEK_SET);
	if(write(sb->fd, sb, sb->blksz) < 0){
		return -1;
	}

	return 0;
}

// ssize_t fs_read_file(struct superblock *sb, const char *fname, char *buf,
//                      size_t bufsz);

// int fs_unlink(struct superblock *sb, const char *fname);

int fs_mkdir(struct superblock *sb, const char *dname){
	
	int i, j, k;
	int num_elements_in_path;
	int blocks[MAX_FILE_SIZE];
	uint64_t block_info, block_inode;
	char files[MAX_SUBFOLDERS][MAX_NAME];
	char *token;
	char *name;
	struct inode in, in2;
	struct nodeinfo info, info2;

	name = (char*) malloc(MAX_PATH_NAME*sizeof(char));
	strcpy(name, dname);

	// Separate the subfolders in a vector of strings
	i = 0;
	token = strtok(name, "/"); // Root
	while(token != NULL){
		strcpy(files[i], token);
		token = strtok(NULL, "/");
		i++;
	}
	num_elements_in_path = i;
	free(name);

	// Root iNode
	lseek(sb->fd, sb->root*sb->blksz, SEEK_SET);
	read(sb->fd, &in, sb->blksz);

	// Root nodeinfo
	lseek(sb->fd, sb->blksz, SEEK_SET);
	read(sb->fd, &info, sb->blksz);

	// Go trought every folder in the path, until reach the file, if it exists
	for(j = 0; j < num_elements_in_path-1; j++){
		// Check every element inside the current directory
		while(1){
			// Check if the element is in the current inode
			for(k = 0; k < info.size; k++){
				// Inode of a file
				lseek(sb->fd, in.links[k]*sb->blksz, SEEK_SET);
				read(sb->fd, &in2, sb->blksz);
			
				// Check if we are in a child inode
				if(in2.mode == IMCHILD){
					// Jump to the first inode
					lseek(sb->fd, in2.parent*sb->blksz, SEEK_SET);
					read(sb->fd, &in2, sb->blksz);
				}
				
				// Get the file's nodeinfo
				lseek(sb->fd, in2.meta*sb->blksz, SEEK_SET);
				read(sb->fd, &info2, sb->blksz);

				if(strcmp(info2.name, files[j]) == 0){
					break;
				}
			}

			if(strcmp(info2.name, files[j]) == 0){
				// The subfolder or file has been found in the current inode
				if(j == (num_elements_in_path-2)){
					blocks[0] = in2.meta;
					blocks[1] = in.links[k];
				}
				break;
			}else if(j == (num_elements_in_path-2) || in.next == 0){
				// The directory has not been found
				errno = ENOENT;
				return -1;
			}

			// Jump to the next inode
			lseek(sb->fd, in.next*sb->blksz, SEEK_SET);
			read(sb->fd, &in, sb->blksz);
		}

		// Jump to the next directory
		info = info2;
		in = in2;
	}

	// New nodeinfo
	block_info = fs_get_block(sb);
	info2.size = 0; // The directory starts empty
	strcpy(info2.name, files[num_elements_in_path-1]);

	// Write nodeinfo
	lseek(sb->fd, block_info*sb->blksz, SEEK_SET);
	write(sb->fd, &info2, sb->blksz);
	
	// New inode
	in2.mode = IMDIR;
	in2.parent = fs_get_block(sb);
	in2.meta = block_info;
	in2.next = 0;
	block_inode = fs_get_block(sb);

	// Write inode
	lseek(sb->fd, block_inode*sb->blksz, SEEK_SET);
	write(sb->fd, &in2, sb->blksz);

	// Update directory parent info
	in.links[info.size] = block_inode;
	info.size++;

	// Write directory parent updated nodeinfo
	lseek(sb->fd, blocks[0]*sb->blksz, SEEK_SET);
	write(sb->fd, &info, sb->blksz);	

	// Write directory parent updated first inode
	lseek(sb->fd, blocks[1]*sb->blksz, SEEK_SET);
	write(sb->fd, &in, sb->blksz);	

	// Write superblock updated
	lseek(sb->fd, 0, SEEK_SET);
	write(sb->fd, sb, sb->blksz);	

	return 0;
}

// int fs_rmdir(struct superblock *sb, const char *dname);

char * fs_list_dir(struct superblock *sb, const char *dname){
	
	int i, j, k, pos;
	int num_elements_in_path;
	char files[MAX_SUBFOLDERS][MAX_NAME];
	char *token;
	char *name, *elements;
	struct inode in, in2;
	struct nodeinfo info, info2;

	name = (char*) malloc(MAX_PATH_NAME*sizeof(char));
	strcpy(name, dname);

	// Separate the subfolders in a vector of strings
	i = 0;
	token = strtok(name, "/"); // Root
	while(token != NULL){
		strcpy(files[i], token);
		token = strtok(NULL, "/");
		i++;
	}
	num_elements_in_path = i;
	free(name);

	// Root iNode
	lseek(sb->fd, sb->root*sb->blksz, SEEK_SET);
	read(sb->fd, &in, sb->blksz);

	// Root nodeinfo
	lseek(sb->fd, sb->blksz, SEEK_SET);
	read(sb->fd, &info, sb->blksz);

	// Go trought every folder in the path, until reach the file, if it exists
	for(j = 0; j < num_elements_in_path; j++){
		// Check every element inside the current directory
		while(1){
			// Check if the element is in the current inode
			for(k = 0; k < info.size; k++){
				// Inode of a file
				lseek(sb->fd, in.links[k]*sb->blksz, SEEK_SET);
				read(sb->fd, &in2, sb->blksz);
			
				// Check if we are in a child inode
				if(in2.mode == IMCHILD){
					// Jump to the first inode
					lseek(sb->fd, in2.parent*sb->blksz, SEEK_SET);
					read(sb->fd, &in2, sb->blksz);
				}
				
				// Get the file's nodeinfo
				lseek(sb->fd, in2.meta*sb->blksz, SEEK_SET);
				read(sb->fd, &info2, sb->blksz);

				if(strcmp(info2.name, files[j]) == 0){
					break;
				}
			}

			if(strcmp(info2.name, files[j]) == 0){
				// The subfolder or file has been found in the current inode
				break;
			}else if(j == (num_elements_in_path-1) || in.next == 0){
				// The directory has not been found
				errno = ENOENT;
				elements = (char*) malloc(3*sizeof(char));
				strcpy(elements, "-1");
				return elements;
			}

			// Jump to the next inode
			lseek(sb->fd, in.next*sb->blksz, SEEK_SET);
			read(sb->fd, &in, sb->blksz);
		}

		// Jump to the next directory
		info = info2;
		in = in2;
	}

	elements = (char*) malloc(MAX_PATH_NAME*sizeof(char));
	pos = 0;
	elements[0] = '\0';
	for(i = 0; i < info.size; i++){
		// Inode of a file
		lseek(sb->fd, in.links[i]*sb->blksz, SEEK_SET);
		read(sb->fd, &in2, sb->blksz);
	
		// Check if we are in a child inode
		if(in2.mode == IMCHILD){
			// Jump to the first inode
			lseek(sb->fd, in2.parent*sb->blksz, SEEK_SET);
			read(sb->fd, &in2, sb->blksz);
		}
		
		// Get the file's nodeinfo
		lseek(sb->fd, in2.meta*sb->blksz, SEEK_SET);
		read(sb->fd, &info2, sb->blksz);

		strcpy((elements+pos), info.name);
		pos += strlen(info.name);

		if(in2.mode == IMDIR){
			// It is a directory
			elements[pos] = '/';
			elements[pos+1] = '\0';
			pos++;
		}

		if(i < info.size-1){
			elements[pos] = ' ';
			elements[pos+1] = '\0';
			pos++;
		}
	}

	// elements = realloc(elements, pos*sizeof(char));

	return elements;
}
