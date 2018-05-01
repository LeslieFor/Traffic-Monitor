#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct node_s node_t;

struct node_s {
    void   *data;
    node_t *prev;
    node_t *next;
};

typedef struct queue_s queue_t;

struct queue_s {
    int     size;
    int     capacity;
    node_t *tail;
    node_t *head;
};

queue_t *new_queue(int);

void *pop(queue_t *);
int   put(queue_t *, void *);

void *get_head(queue_t *q);
void *get_tail(queue_t *q);
void *get_order(queue_t *q, int index)

#endif
