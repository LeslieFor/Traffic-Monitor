#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "queue.h"

queue_t *new_queue(int capacity)
{
    node_t  *n = NULL;
    node_t  *t = NULL;
    queue_t *q = NULL;

    q = (queue_t *) malloc(sizeof(*q));
    if (q == NULL)
    {
        printf("malloc queue_t error\n");
        return NULL;
    }

    memset(q, 0x00, sizeof(*q));

    q->capacity = capacity;

    for (int i = 0; i < q->capacity + 1; i++)
    {
        t = (node_t *) malloc(sizeof(*t));
        if (t == NULL)
        {
            printf("malloc node_t error\n");
            return NULL;
        }

        memset(t, 0x00, sizeof(*t));

        if (q->tail == NULL && q->head == NULL)
        {
            q->tail = t;
            q->head = t;

            q->head->next = t;
            q->head->prev = t;
        }
        else if (q->head->next == q->head)
        {
            t->prev = q->head;
            t->next = q->head;

            q->head->next = t;
            q->head->prev = t;
        }
        else
        {
            n = q->head->prev;

            t->prev = n;
            t->next = q->head;

            n->next = t;
            q->head->prev = t;
        }
    }

    return q;
}


int put(queue_t *q, void *data)
{
    node_t  *n     = NULL;
    node_t  *new_n = NULL;
    queue_t *new_q = NULL;

    if (q->tail->next == q->head)
    {
        new_q = new_queue(q->capacity);
        if (new_q == NULL)
        {
            return -1;
        }

        n     = q->head->prev;
        new_n = new_q->head->prev;

        new_n->next = q->head;
        q->head->prev = new_n;

        n->next = new_q->head;
        new_q->head->prev = n;

        free(new_q);

        q->capacity += q->capacity + 1;
    }

    q->tail = q->tail->next;
    q->tail->data = data;

    q->size++;

    return 1;
}

void *pop(queue_t *q)
{
    void *data = NULL;

    if (q->head == q->tail)
    {
        return NULL;
    }

    q->head = q->head->next;
    data    = q->head->data;
    q->head->data = NULL;

    q->size--;

    return data;
}


/*
 * From tail to head
 * Notice: First Index is 0
 */
void *get_order_tail(queue_t *q, int index)
{
    int i = 0;
    node_t *temp = NULL;

    if (index + 1 > q->size && index > 0)
    {
        return NULL;
    }

    for (temp = q->tail; i < index; i++, temp = temp->prev){;}

    return temp->data;
}

/*
 * From head to tail
 * Notice: First Index is 0
 */
void *get_order_head(queue_t *q, int index)
{
    int i = 0;
    node_t *temp = NULL;

    if (index + 1 > q->size && index > 0)
    {
        return NULL;
    }

    for (temp = q->head; i <= index; i++, temp = temp->next){;}

    return temp->data;
}

void *get_head(queue_t *q)
{

    if (q->head == q->tail)
    {
        return NULL;
    }

    return q->head->next->data;
}

void *get_tail(queue_t *q)
{
    if (q->head == q->tail)
    {
        return NULL;
    }

    return q->tail->data;
}
