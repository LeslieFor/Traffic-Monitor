#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mac.h"
#include "rate.h"
//#include "pear_pcap.h"

int main(int argc, char *argv[])
{
    rate_ctx_t *rate_tx = NULL;
    rate_ctx_t *rate_rx = NULL;

    rate_init_tx(&rate_tx, NULL);
    rate_init_rx(&rate_rx, NULL);

    for ( ; ; )
    {
        sleep(1);

	printf("========================\n");

        printf("02s RateTx: %f\tMax-Rate:%f\n", rate_tx->rate_02.rate, rate_tx->rate_02.rate_max);
        printf("10s RateTx: %f\tMax-Rate:%f\n", rate_tx->rate_10.rate, rate_tx->rate_10.rate_max);
        printf("40s RateTx: %f\tMax-Rate:%f\n", rate_tx->rate_40.rate, rate_tx->rate_40.rate_max);

	printf("========================\n");

        printf("02s RateRx: %f\tMax-Rate:%f\n", rate_rx->rate_02.rate, rate_rx->rate_02.rate_max);
        printf("10s RateRx: %f\tMax-Rate:%f\n", rate_rx->rate_10.rate, rate_rx->rate_10.rate_max);
        printf("40s RateRx: %f\tMax-Rate:%f\n", rate_rx->rate_40.rate, rate_rx->rate_40.rate_max);

        //fflush(stdin);
    }
}
