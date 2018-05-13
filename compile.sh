#!/bin/sh

gcc -o demo demo.c pear_pcap.c queue.c rate.c mac.c -lpcap -lpthread

gcc -o rate_demo rate_demo.c pear_pcap.c queue.c rate.c mac.c -lpcap -lpthread
