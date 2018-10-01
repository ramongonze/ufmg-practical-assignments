#include <stdlib.h>
#include <assert.h>

#include "dlist.h"

struct dlist *dlist_create(void) /* {{{ */
{
	struct dlist *dl = malloc(sizeof(struct dlist));
	assert(dl);
	dl->head = NULL;
	dl->tail = NULL;
	dl->count = 0;
	return dl;
} /* }}} */

void dlist_destroy(struct dlist *dl, dlist_data_func cb) /* {{{ */
{
	while(!dlist_empty(dl)) {
		void *data = dlist_pop_left(dl);
		if(cb) cb(data);
	}
	free(dl);
} /* }}} */

void *dlist_pop_left(struct dlist *dl) /* {{{ */
{
	void *data;
	struct dnode *node;

	if(dlist_empty(dl)) return NULL;

	node = dl->head;

	dl->head = node->next;
	if(dl->head == NULL) dl->tail = NULL;
	if(node->next) node->next->prev = NULL;

	data = node->data;
	free(node);

	dl->count--;
	assert(dl->count >= 0);
	return data;
} /* }}} */

void *dlist_pop_right(struct dlist *dl) /* {{{ */
{
	void *data;
	struct dnode *node;

	if(dlist_empty(dl)) return NULL;

	node = dl->tail;

	dl->tail = node->prev;
	if(dl->tail == NULL) dl->head = NULL;
	if(node->prev) node->prev->next = NULL;

	data = node->data;
	free(node);

	dl->count--;
	assert(dl->count >= 0);
	return data;
} /* }}} */

void *dlist_push_right(struct dlist *dl, void *data) /* {{{ */
{
	struct dnode *node = malloc(sizeof(struct dnode));
	assert(node);

	node->data = data;
	node->prev = dl->tail;
	node->next = NULL;

	if(dl->tail) dl->tail->next = node;
	dl->tail = node;

	if(dl->head == NULL) dl->head = node;

	dl->count++;
	return data;
} /* }}} */

void *dlist_find_remove(struct dlist *dl, void *data, /* {{{ */
		dlist_cmp_func cmp, void *user_data)
{
	struct dnode *curr;
	for(curr = dl->head; curr; curr = curr->next) {
		if(!curr->data) continue;
		if(cmp(curr->data, data, user_data)) continue;
		void *ptr = curr->data;
		if(dl->head == curr) dl->head = curr->next;
		if(dl->tail == curr) dl->tail = curr->prev;
		if(curr->prev) curr->prev->next = curr->next;
		if(curr->next) curr->next->prev = curr->prev;
		dl->count--;
		free(curr);
		return ptr;
	}
	return NULL;
} /* }}} */

int dlist_empty(struct dlist *dl) /* {{{ */
{
	if(dl->head == NULL) {
		assert(dl->tail == NULL);
		assert(dl->count == 0);
		return 1;
	} else {
		assert(dl->tail != NULL);
		assert(dl->count > 0);
		return 0;
	}
} /* }}} */

void * dlist_get_index(const struct dlist *dl, int idx) /* {{{ */
{
	struct dnode *curr;
	if(idx >= 0) {
		curr = dl->head;
		while(curr && idx--) curr = curr->next;
	} else {
		curr = dl->tail;
		while(curr && ++idx) curr = curr->prev;
	}
	if(!curr) return NULL;
	return curr->data;
} /* }}} */

void dlist_set_index(struct dlist *dl, int idx, void *data) /* {{{ */
{
	struct dnode *curr;
	if(idx >= 0) {
		curr = dl->head;
		while(curr && idx--) curr = curr->next;
	} else {
		curr = dl->tail;
		while(curr && ++idx) curr = curr->prev;
	}
	if(!curr) return;
	curr->data = data;
} /* }}} */
