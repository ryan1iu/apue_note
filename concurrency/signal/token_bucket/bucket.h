#ifndef BUCKET_H__
#define BUCKET_H__

#define BUCKET_MAX_SIZE 1024
typedef void bucket_t;

int bucket_init(bucket_t **bucket, int rate, int size); // 传二级指针！！

int bucket_fetch(bucket_t *bucket, int token);

int bucket_return(bucket_t *bucket, int token);

int bucket_destory(bucket_t *bucket);

#endif
