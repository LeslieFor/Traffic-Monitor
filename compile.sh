#!/bin/sh

gcc -o demo demo.c les_pcap.c queue.c rate.c mac.c -lpcap -lpthread

gcc -o rate_demo rate_demo.c les_pcap.c queue.c rate.c mac.c -lpcap -lpthread
