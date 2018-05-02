#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "pear_pcap.h"
#include "rate.h"

void *rate_handle(pr_pcap_handler_t *, struct pcap_pkthdr *, char *);

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

    hlr->size_cur = 0;
    hlr->size_ttl = 0;

    hlr->rate_02.rate     = 0;
    hlr->rate_10.rate_max = 0;
    hlr->rate_10.rate     = 0;
    hlr->rate_40.rate_max = 0;
    hlr->rate_40.rate     = 0;

    gettimeofday(&hlr->ts, NULL);

    return hlr;
}


void *rate_handle(pr_pcap_handler_t *phl, struct pcap_pkthdr *hdr, char *ptr)
{
    rate_node_t *temp   = NULL;
    rate_node_t *popn   = NULL;
    rate_handler_t *rhl = NULL;

    rhl = (rate_handler_t *) phl;

    rhl->size_cur += hdr->len;
    rhl->size_ttl += hdr->len;

    printf("####total: %d, cur: %d\n"
           " hdr-sec: %ld \t rhl-sec: %ld\n", rhl->size_ttl, rhl->size_cur, hdr->ts.tv_sec, rhl->ts.tv_sec);
    if (hdr->ts.tv_sec - rhl->ts.tv_sec + (hdr->ts.tv_usec - rhl->ts.tv_usec) * 0.001 * 0.001 < 2)
    {
        return NULL;
    }

    temp = (rate_node_t *) malloc(sizeof(*temp));

    /* record current node data */
    temp->size = rhl->size_cur;
    temp->ts.tv_sec  = hdr->ts.tv_sec;
    temp->ts.tv_usec = hdr->ts.tv_usec;

    /* record last node timeval */
    rhl->ts.tv_sec   = hdr->ts.tv_sec;
    rhl->ts.tv_usec  = hdr->ts.tv_usec;

    printf("total: %d, cur: %d\n", rhl->size_ttl, rhl->size_cur);

    /* reset next node data */
    rhl->size_cur = 0;

    if (rhl->q->capacity -1 <= rhl->q->size)
    {
        popn = pop(rhl->q);
        put(rhl->q, temp);
        free(popn);
    }
    else
    {
        put(rhl->q, temp);
    }


    return NULL;   
}




