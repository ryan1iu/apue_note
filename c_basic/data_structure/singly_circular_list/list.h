#ifndef LIST_H__
#define LIST_H__

/* defining the data structure of linked list node */
typedef struct node_st{
    int data;
    struct node_st *next; 
}link_list;

/* initializing the linked list */
link_list *linked_list_create();

/* destorying the linked list */
int linked_list_destory(link_list *);

/* inserting a node at the beginning of the linked list */
int linked_list_insert_head(link_list *, int);

/* inserting a node at the end of the linked list */
int linked_list_insert_tail(link_list *, int);

/* deleting the head node */
int linked_list_delete_head(link_list *);

/* deleting the tail node */
int linked_list_delete_tail(link_list *);

/* deleting a specified node */
int linked_list_delete_specified(link_list *, int data);

/* searching a node by index */
link_list *linked_list_find_index(link_list *, int index);

/* searching a node by value and returning the pointer to the first matching node */
link_list *linked_list_find_value(link_list *, int value);

/* traversing the linked list */
void linked_list_traverse(link_list *);

/* returning length of linked list */
unsigned linked_list_length(link_list *);

/* reversing the linked list */
void linked_list_reverse(link_list *);


#endif
