#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <memory.h>
#include <pthread.h>
//#include <arpa/inet.h>

#include "pear_pcap.h"


static void *pr_pcap_run(void *arg);

pr_pcap_t *pr_new_pcap(char *device, char *cmd, int snaplen, int promisc, int timeout, pr_pcap_handler_t *handler)
{
    pr_pcap_t *pt = NULL;

    pt = (pr_pcap_t *) malloc(sizeof(*pt));
    if (pt == NULL)
    {
        printf("malloc pr_pcap_t error\n");
        return NULL;
    }

    memset(pt, 0x00, sizeof(*pt));

    pt->cmd     = cmd;
    pt->device  = device;
    pt->snaplen = snaplen;
    pt->promisc = promisc;
    pt->timeout = timeout;
    pt->handler = handler;

    return pt;
}


int pr_pcap_start(pr_pcap_t *pt)
{
    int       err;
    sigset_t  mask;
    sigset_t  oldmask;
    pthread_t ntid;

    char str1[16] = {0x00};
    char str2[16] = {0x00};

    struct bpf_program fcode;

    if (pt->device == NULL)
    {
        if ((pt->device = pcap_lookupdev(pt->errbuf)) == NULL)
        {
            PEAR_LOG("pcap_lookup error: %s\n", pt->errbuf);
            return -1;
        }
    }

    PEAR_LOG("device = %s\n", pt->device);

    //if ((pt->pd = pcap_open_live(pt->device, 10, 0, 5000, pt->errbuf)) == NULL)
    if ((pt->pd = pcap_open_live(pt->device, pt->snaplen, pt->promisc, pt->timeout, pt->errbuf)) == NULL)
    {
        PEAR_LOG("pcap_open_live error: %s\n", pt->errbuf);
        return -1;
    }

    if (pcap_lookupnet(pt->device, &pt->localnet, &pt->netmask, pt->errbuf) < 0)
    {
        PEAR_LOG("pcap_lookupnet error: %s\n", pt->errbuf);
        return -1;
    }

/*
    PEAR_LOG("localnet = %s, netmask = %s\n",
            inet_ntop(AF_INET, &pt->localnet, str1, sizeof(str1)),
            inet_ntop(AF_INET, &pt->netmask, str2, sizeof(str2)));
*/

    if (pcap_compile(pt->pd, &fcode, pt->cmd, 0, pt->netmask) < 0)
    {
        PEAR_LOG("pcap_compile error : %s\n", pcap_geterr(pt->pd));
        return -1;
    }

    if (pcap_setfilter(pt->pd, &fcode) < 0)
    {
        PEAR_LOG("pcap_setfilter error: %s\n", pcap_geterr(pt->pd));
    }

    if (pt->datalink = pcap_datalink(pt->pd) < 0)
    {
        PEAR_LOG("pcap_datalink error: %s\n", pcap_geterr(pt->pd));
    }

    PEAR_LOG("datalink = %d\n", pt->datalink);

    sigemptyset(&mask);
    sigfillset(&mask);
    sigdelset(&mask, SIGINT);

    if (pthread_sigmask(SIG_BLOCK, &mask, &oldmask) != 0)
    {
        return -1;
    }

    err = pthread_create(&ntid, NULL, pr_pcap_run, (void *)pt);

    pthread_sigmask(SIG_BLOCK, &oldmask, NULL);

    if (err != 0)
    {
        PEAR_LOG("ptherad_create error\n");
        return -1;
    }

    return 0;
}


static void *pr_pcap_run(void *arg)
{
    char      *ptr = NULL;
    pr_pcap_t *pt  = NULL;

    struct pcap_pkthdr hdr;

    pt = (pr_pcap_t *) arg;

    for ( ; ; )
    {
        ptr = (char *) pcap_next(pt->pd, &hdr);

        if (ptr == NULL)
        {
            hdr.len = 0;
            gettimeofday(&hdr.ts, NULL);
        }

        //PEAR_LOG("catch: len: %d\n", hdr.len);

        if (pt->handler != NULL && pt->handler->handle != NULL)
        {
            pt->handler->handle(pt->handler, &hdr, ptr);
        }

    }
}



