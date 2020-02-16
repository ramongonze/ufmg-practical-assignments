#ifndef __FS_HEADER__
#define __FS_HEADER__

/* Relevant error codes:
 *
 * EFBIG (file too large)
 * EEXIST (file exists)
 * EISDIR (is a directory)
 * EBUSY (device is busy)
 * EBADF (bad file descriptor)
 * ENAMETOOLONG (filename too long)
 * ENOENT (no such file or directory)
 * ENOSPC (no space left on device)
 * ENOTDIR (not a directory)
 * ENOTEMPTY (directory not empty)
 * ETXTBUSY (text file busy)
 * EPERM (operation not permitted)
 * EACCES (permission denied)
 */

#include <inttypes.h>

#define IMREG 1   /* regular inode */
#define IMDIR 2   /* directory inode */
#define IMCHILD 4 /* child inode */

struct superblock {
	uint64_t magic; /* 0xdcc605f5 */
	uint64_t blks; /* number of blocks in the filesystem */
	uint64_t blksz; /* block size (bytes) */
	uint64_t freeblks; /* number of free blocks in the filesystem */
	uint64_t freelist; /* pointer to free block list */
	uint64_t root; /* pointer to root directory's inode */
	int fd; /* file descriptor for the filesystem image */
};

struct inode {
	uint64_t mode;
	uint64_t parent;
	/* if =mode does not contain IMCHILD, then =parent points to the
	 * directory that contains this inode.  if =mode contains IMCHILD,
	 * then =parent points to the first inode (i.e., the inode without
	 * IMCHILD) for the entity represented by this inode. */
	uint64_t meta;
	/* if =mode does not contain IMCHILD, then meta points to this inode's
	 * metadata (struct iinfo).  if =mode contains IMCHILD, then meta
	 * points to the previous inode for this inode's entity. */
	uint64_t next;
	/* if this file's date block do not fit in this inode, =next points to
	 * the next inode for this entity; otherwise =next should be zero. */
	uint64_t links[];
	/* if =mode contains IMDIR, then entries in =links point to inode's
	 * for each entity in the directory.  otherwise, if =mode contains
	 * IMREG, then entries in =links point to this file's data blocks. */
};

struct nodeinfo {
	uint64_t size;
	/* for files (mode IMREG), =size should contain the size of the file in 
	 * bytes.  for directories (mode IMDIR), =size should contain the
	 * number of files in the directory. */
	uint64_t reserved[7];
	/* reserving some space to implement security and ownership in the
	 * future. */
	char name[];
	/* remainder of block used to store this entity's name. */
};

struct freepage {
	uint64_t next;
	/* link to next freepage; or zero if this is the last freepage */
	uint64_t count;
	uint64_t links[];
	/* remainder of block used to store links to free blocks.  =count
	 * counts the number of elements in links, stored from links[0] to
	 * links[counts-1]. */
};

#define MIN_BLOCK_SIZE 128
#define MIN_BLOCK_COUNT 32

/* Build a new filesystem image in =fname (the file =fname should be present
 * in the OS's filesystem).  The new filesystem should use =blocksize as its
 * block size; the number of blocks in the filesystem will be automatically
 * computed from the file size.  The filesystem will be initialized with an
 * empty root directory.  This function returns NULL on error and sets errno
 * to the appropriate error code.  If the block size is smaller than
 * MIN_BLOCK_SIZE bytes, then the format fails and the function sets errno to
 * EINVAL.  If there is insufficient space to store MIN_BLOCK_COUNT blocks in
 * =fname, then the function fails and sets errno to ENOSPC. */
struct superblock * fs_format(const char *fname, uint64_t blocksize);

/* Open the filesystem in =fname and return its superblock.  Returns NULL on
 * error, and sets errno accordingly.  If =fname does not contain a
 * 0xdcc605fs, then errno is set to EBADF. */
struct superblock * fs_open(const char *fname);

/* Close the filesystem pointed to by =sb.  Returns zero on success and a
 * negative number on error.  If there is an error, all resources are freed
 * and errno is set appropriately. */
int fs_close(struct superblock *sb);

/* Get a free block in the filesystem.  This block shall be removed from the
 * list of free blocks in the filesystem.  If there are no free blocks, zero
 * is returned.  If an error occurs, (uint64_t)-1 is returned and errno is set
 * appropriately. */
uint64_t fs_get_block(struct superblock *sb);

/* Put =block back into the filesystem as a free block.  Returns zero on
 * success or a negative value on error.  If there is an error, errno is set
 * accordingly. */
int fs_put_block(struct superblock *sb, uint64_t block);

int fs_write_file(struct superblock *sb, const char *fname, char *buf,
                  size_t cnt);

ssize_t fs_read_file(struct superblock *sb, const char *fname, char *buf,
                     size_t bufsz);

int fs_unlink(struct superblock *sb, const char *fname);

int fs_mkdir(struct superblock *sb, const char *dname);

int fs_rmdir(struct superblock *sb, const char *dname);

char * fs_list_dir(struct superblock *sb, const char *dname);

#endif
