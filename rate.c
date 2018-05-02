#include <stdlib.h>

#include "pear_pcap.h"

typedef struct rate_s rate_t;

struct rate_s {
    float rate;
    float rate_max;
};


typedef struct rate_handler_s rate_handler_t;

struct rate_handler_s {
    pr_pcap_handler_t handler;
    rate_t  rate_02;
    rate_t  rate_10;
    rate_t  rate_40;
    queue_t *q;
};


rate_handler_t *new_rate_handler()
{
    rate_handler_t *hlr = NULL;

    hlr = (rate_handler_t *) malloc(sizeof(rate_handler_t)); 
    if (hlr == NULL)
    {
        return NULL;
    }

    hlr->q = new_queue(61);
    hlr->handler.handle = &rate_handle;

    hlr->rate_02.rate     = 0;
    hlr->rate_10.rate_max = 0;
    hlr->rate_10.rate     = 0;
    hlr->rate_40.rate_max = 0;
    hlr->rate_40.rate     = 0;

    return hlr;
}


void *rate_handle(pr_pcap_handler_t *phl, struct pcap_pkthdr *hdr, char *ptr)
{
    rate_handler_t rhl = (rate_handler_t *) phl;

    
}




