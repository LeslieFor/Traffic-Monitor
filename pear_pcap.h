#ifndef _PEAR_PCAP_H_
#define _PEAR_PCAP_H_

#include <stdio.h>
#include <stdint.h>
#include <pcap.h>

#define PEAR_LOG(format, ...) printf(format, ##__VA_ARGS__)


#define CMD_TCP      "tcp"
#define CMD_UPLOAD   "tcp and src 192.168.99.181"
//#define CMD_DOWNLOAD "tcp and dst 192.168.99.181"
#define CMD_DOWNLOAD "tcp and dst 172.31.190.206"


/*
 * Handler for msg receive from datalink
 *
 */

typedef struct pr_pcap_handler_s pr_pcap_handler_t;

typedef void *(*pr_pcap_handler_p)(pr_pcap_handler_t *, struct pcap_pkthdr *, char *);

struct pr_pcap_handler_s {
    pr_pcap_handler_p handle;
};


typedef struct pr_pcap_s pr_pcap_t;

struct pr_pcap_s {
    pcap_t *pd;

    char    *cmd;
    char    *device;
    int      promisc;
    int      snaplen;
    int      timeout;
    int      datalink;

    uint32_t netmask;
    uint32_t localnet;

    char errbuf[PCAP_ERRBUF_SIZE];

    pr_pcap_handler_t *handler;
};

int pr_pcap_start(pr_pcap_t *);
pr_pcap_t *pr_new_pcap(char *, char *, int, int, int, pr_pcap_handler_t *);

#endif
