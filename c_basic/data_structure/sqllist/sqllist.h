#ifndef SQLLIST_H__
#define SQLLIST_H__

#define DATASIZE 9

typedef int data_type;

typedef struct{
    data_type data[DATASIZE];
    unsigned int next_index;
} sqlist;

/* create a sqlist */
sqlist * create(void);

/* create a sqlist */
void create1(sqlist **);

void destory(sqlist *);

int insert_index(sqlist *, unsigned index, data_type data);

int insert(sqlist *, data_type);

int sql_remove(sqlist *, unsigned index);

int change(sqlist *, unsigned index, data_type data);

int find(sqlist *, data_type data);

void display(sqlist *);

#endif
