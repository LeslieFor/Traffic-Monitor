#ifndef _PEAR_RATE_H_
#define _PEAR_RATE_H_


#include <time.h>
#include "queue.h"

typedef struct rate_s rate_t;

struct rate_s {
    int   size_cur;
    float rate;
    float rate_max;
};


typedef struct rate_node_s rate_node_t;

struct rate_node_s {
    int size;
    struct timeval ts;
};


typedef struct rate_handler_s rate_handler_t;

struct rate_handler_s {
    pr_pcap_handler_t handler;

    rate_t  rate_02;
    rate_t  rate_10;
    rate_t  rate_40;

    int size_cur;
    int size_ttl;
    struct timeval ts;
    queue_t *q;
};


rate_handler_t *new_rate_handler();

#endif
