#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mac.h"
#include "rate.h"
#include "les_pcap.h"

int main(int argc, char *argv[])
{
    char cmd[512] = {0x00};
    char mac[128] = {0x00};
    pr_pcap_t *pt =  NULL;

    if (get_mac(mac, 128) < 0)
    {
        printf("get_mac error\n");
        exit(-1);
    }

    sprintf(cmd, "tcp and ether dst %s", mac);
    printf("cmd: %s\n", cmd);

    rate_ctx_t *rhl = new_rate_ctx();

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

    for ( ; ; )
    {
        sleep(1);
        printf("02s Rate: %f\tMax-Rate:%f\n", rhl->rate_02.rate, rhl->rate_02.rate_max);
        printf("10s Rate: %f\tMax-Rate:%f\n", rhl->rate_10.rate, rhl->rate_10.rate_max);
        //fflush(stdin);
    }
}
