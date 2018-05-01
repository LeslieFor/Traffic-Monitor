#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pear_pcap.h"

int main(int argc, char *argv[])
{
    pr_pcap_t *pt = NULL;

    pt = pr_new_pcap(NULL, CMD_DOWNLOAD, 10, 0, 500, NULL);
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
