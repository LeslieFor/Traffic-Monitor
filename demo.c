#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pear_pcap.h"
#include "rate.h"

int main(int argc, char *argv[])
{
    pr_pcap_t *pt = NULL;

    rate_handler_t *rhl = new_rate_handler();

    pt = pr_new_pcap(NULL, CMD_DOWNLOAD, 100, 0, 5000, (pr_pcap_handler_t *)rhl);
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
    }
}
