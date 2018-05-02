#!/bin/sh

gcc -o demo demo.c pear_pcap.c queue.c rate.c -lpcap -lpthread
