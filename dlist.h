#ifndef __DLIST_H__
#define __DLIST_H__

struct dlist {
	struct dnode *head;
	struct dnode *tail;
	int count;
};

struct dnode {
	struct dnode *prev;
	struct dnode *next;
	void *data;
};

typedef void (*dlist_data_func)(void *data);
typedef int (*dlist_cmp_func)(const void *e1, const void *e2, void *userdata); 

struct dlist *dlist_create(void);
void dlist_destroy(struct dlist *dl, dlist_data_func);

void *dlist_pop_left(struct dlist *dl);
void *dlist_pop_right(struct dlist *dl);
void *dlist_push_right(struct dlist *dl, void *data);

/* this function calls =cmp to compare =data and each value in =dl.  if a
 * match is found, it is removed from the list and its pointer is returned.
 * returns NULL if no match is found. */
void *dlist_find_remove(struct dlist *dl, void *data, dlist_cmp_func cmp,
			void *userdata);

int dlist_empty(struct dlist *dl);

/* gets the data at index =idx.  =idx can be negative. */
void * dlist_get_index(const struct dlist *dl, int idx);
/* changes the data at index =idx.  does nothing if =idx does not exist. */
void dlist_set_index(struct dlist *dl, int idx, void *data);

#endif
