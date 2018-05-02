#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pear_pcap.h"
#include "rate.h"

int main(int argc, char *argv[])
{
    pr_pcap_t *pt = NULL;

    rate_ctx_t *rhl = new_rate_ctx();

    pt = pr_new_pcap(NULL, CMD_DOWNLOAD, 100, 0, 500, (pr_pcap_handler_t *)rhl);
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

    for ( ; ; )
    {
        sleep(1);
        printf("02s Rate: %f\tMax-Rate:%f\n", rhl->rate_02.rate, rhl->rate_02.rate_max);
        printf("10s Rate: %f\tMax-Rate:%f\n", rhl->rate_10.rate, rhl->rate_10.rate_max);
    }
}
