#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "mac.h"
#include "rate.h"
#include "pear_pcap.h"

void *rate_handle(pr_pcap_handler_t *, struct pcap_pkthdr *, char *);
void *rate_handle_timestamp(pr_pcap_handler_t *, struct pcap_pkthdr *, char *);

rate_ctx_t *new_rate_ctx()
{
    rate_ctx_t *hlr = NULL;

    hlr = (rate_ctx_t *) malloc(sizeof(rate_ctx_t)); 
    if (hlr == NULL)
    {
        return NULL;
    }

    hlr->q = new_queue(61);
    hlr->handler.handle = &rate_handle_timestamp;

    hlr->size_cur = 0;
    hlr->size_ttl = 0;

    hlr->rate_01.rate     = 0;
    hlr->rate_01.rate_max = 0;
    hlr->rate_01.size_cur = 0;

    hlr->rate_02.rate     = 0;
    hlr->rate_02.rate_max = 0;
    hlr->rate_02.size_cur = 0;

    hlr->rate_10.rate     = 0;
    hlr->rate_10.rate_max = 0;
    hlr->rate_10.size_cur = 0;

    hlr->rate_40.rate     = 0;
    hlr->rate_40.rate_max = 0;
    hlr->rate_40.size_cur = 0;

    gettimeofday(&hlr->ts, NULL);

    return hlr;
}


void *rate_handle_timestamp(pr_pcap_handler_t *phl, struct pcap_pkthdr *hdr, char *ptr)
{
    int   i    = 0;
    int   size = 0;
    float rate = 0;
    rate_node_t *temp = NULL;
    rate_node_t *temp1 = NULL;
    rate_node_t *temp2 = NULL;
    rate_node_t *tail = NULL;
    rate_node_t *npop = NULL;
    rate_ctx_t  *rhl  = NULL;

    rhl = (rate_ctx_t *) phl;

    /*
    rhl->size_ttl += hdr->len;
    printf("rhl->size_ttl: %d\n", rhl->size_ttl);
    */

    /* Do not get any data, Only store data length */
    if (ptr != NULL)
    {
        //free(ptr);
    }

    /* Store Data Length */
    tail = get_tail(rhl->q);

    /* Per-Secode Pre-Block */
    if (tail != NULL && hdr->ts.tv_sec <= tail->ts.tv_sec)
    {
        for (i = 0; i < rhl->q->size; i++)
        {
            temp = get_order_tail(rhl->q, i);
            if (temp->ts.tv_sec == hdr->ts.tv_sec)
            {
                temp->size += hdr->len;
                break;
            }
        }

        //printf("##### sec: %ld \t len: %d\t total_len: %d #####\n", hdr->ts.tv_sec, hdr->len, temp->size);
    }
    else
    {
        temp = (rate_node_t *) malloc(sizeof(*temp));

        temp->size       = hdr->len;
        temp->ts.tv_sec  = hdr->ts.tv_sec;
        temp->ts.tv_usec = hdr->ts.tv_usec;

        if (rhl->q->capacity < rhl->q->size + 1)
        {
            npop = (rate_node_t *) pop(rhl->q);
        }

        put(rhl->q, temp);

        //sleep(1);
    }


    /* Sum Rate 2s */
    if (rhl->q->size < 3)
    {
        return NULL;
    }

    tail = get_tail(rhl->q);
    temp = (rate_node_t *) get_order_tail(rhl->q, 1);

    temp1 = (rate_node_t *) get_order_tail(rhl->q, 1);
    temp2 = (rate_node_t *) get_order_tail(rhl->q, 2);

        /* Sum Rate 1s */
    rate = temp2->size;
    rhl->rate_01.rate = rate;
    rhl->rate_01.rate_max = rate > rhl->rate_01.rate_max ? rate : rhl->rate_01.rate_max;

    rate = (temp2->size + temp1->size) / 2;

    rhl->rate_02.rate = rate;
    rhl->rate_02.rate_max = rate > rhl->rate_02.rate_max ? rate : rhl->rate_02.rate_max;

    //printf("### 02s Rate: %f rate_max :%f  sec: %ld\n", rate, rhl->rate_02.rate_max, tail->ts.tv_sec - temp->ts.tv_sec + 1);

    /* Rate 10s */
    if (rhl->q->size < 12)
    {
        return NULL;
    }

    for (i = 1; i < 11; i ++)
    {
        temp = get_order_tail(rhl->q, i);
        size += temp->size;
    }

    rate = size / 10;

    rhl->rate_10.rate = rate;
    rhl->rate_10.rate_max = rate > rhl->rate_10.rate_max ? rate : rhl->rate_10.rate_max;

    /* Rate 10s */
    if (rhl->q->size < 42)
    {
        return NULL;
    }

    size = 0;

    for (i = 1; i < 41; i ++)
    {
        temp = get_order_tail(rhl->q, i);
        size += temp->size;
    }

    rate = size / 40;

    rhl->rate_40.rate = rate;
    rhl->rate_40.rate_max = rate > rhl->rate_40.rate_max ? rate : rhl->rate_40.rate_max;


    //printf("### 10s Rate: %f rate_max :%f  sec: %ld\n", rate, rhl->rate_10.rate_max, tail->ts.tv_sec - temp->ts.tv_sec + 1);
    return NULL;
}

int rate_init_rx(rate_ctx_t **p, char *mac)
{
    char cmd[512]     = {0x00};
    char def_mac[128] = {0x00};
    pr_pcap_t *pt     =  NULL;

    if (mac == NULL)
    {
        printf("Use Default Mac\n");

        if (get_mac(def_mac, 128) < 0)
        {
            printf("get_mac error\n");
            exit(-1);
        }

        printf("Default Mac: %s\n", def_mac);
    }
    else
    {
        printf("Use Give Mac: %s\n", mac);
    }

    sprintf(cmd, "tcp and ether dst %s", mac == NULL ? def_mac : mac);
    printf("cmd: %s\n", cmd);

    rate_ctx_t *rhl = new_rate_ctx();
    *p = rhl;

    pt = pr_new_pcap(NULL, cmd, 100, 0, 500, (pr_pcap_handler_t *)rhl);
    if (pt == NULL)
    {
        printf("pr_new_pcap error\n");
        exit(-1);
    }

    if (pr_pcap_start(pt) < 0)
    {
        printf("pr_pcap_start error\n");
        exit(-1);
    }


/*
    for ( ; ; )
    {
        sleep(1);
        printf("02s RateRx: %f\tMax-Rate:%f\n", rhl->rate_02.rate, rhl->rate_02.rate_max);
        printf("10s RateRx: %f\tMax-Rate:%f\n", rhl->rate_10.rate, rhl->rate_10.rate_max);
        printf("40s RateRx: %f\tMax-Rate:%f\n", rhl->rate_40.rate, rhl->rate_40.rate_max);
    }
*/

    return 0;
}

int rate_init_tx(rate_ctx_t **p, char *mac)
{
    char cmd[512]     = {0x00};
    char def_mac[128] = {0x00};
    pr_pcap_t *pt     =  NULL;

    if (mac == NULL)
    {
        printf("Use Default Mac\n");

        if (get_mac(def_mac, 128) < 0)
        {
            printf("get_mac error\n");
            exit(-1);
        }

        printf("Default Mac: %s\n", def_mac);
    }
    else
    {
        printf("Use Give Mac: %s\n", mac);
    }

    sprintf(cmd, "tcp and ether src %s", mac == NULL ? def_mac : mac);
    printf("cmd: %s\n", cmd);

    rate_ctx_t *rhl = new_rate_ctx();
    *p = rhl;

    pt = pr_new_pcap(NULL, cmd, 100, 0, 500, (pr_pcap_handler_t *)rhl);
    if (pt == NULL)
    {
        printf("pr_new_pcap error\n");
        exit(-1);
    }

    if (pr_pcap_start(pt) < 0)
    {
        printf("pr_pcap_start error\n");
        exit(-1);
    }


/*
    for ( ; ; )
    {
        sleep(1);
        printf("02s RateTx: %f\tMax-Rate:%f\n", rhl->rate_02.rate, rhl->rate_02.rate_max);
        printf("10s RateTx: %f\tMax-Rate:%f\n", rhl->rate_10.rate, rhl->rate_10.rate_max);
        printf("40s RateTx: %f\tMax-Rate:%f\n", rhl->rate_40.rate, rhl->rate_40.rate_max);
    }
*/

    return 0;
}
