#ifndef LIST_H__
#define LIST_H__

enum mode {
   HEAD,
   TAIL
};

typedef struct list_node_st {
    void *data;
    struct list_node_st *prev;
    struct list_node_st *next;
}dlist_node;

typedef struct{
    struct list_node_st head;
    int size;
}dlist;

typedef int (*cmp) (const void *key, const void *data);
typedef void (*show) (const void *data);

dlist * llist_create(int init_size);

int llist_insert(dlist *list, const void *data, enum mode);

dlist* llist_find(dlist *list, const void *data, void *cmp);

int llist_delete(dlist *list, void *data, void *cmp);

dlist* llist_fetch(dlist *list, const void *data, const void *key, void *cmp);

void llist_travel(dlist *list, show show);

int llist_destroy(dlist *list);
#endif
