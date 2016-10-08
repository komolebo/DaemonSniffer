#ifndef SNIFFER_FUNCTIONS_H
#define SNIFFER_FUNCTIONS_H

#include <stdlib.h>         // malloc
#include <memory.h>         // strcat
#include <stdio.h>          // printf
#include <zconf.h>          // getcwd
#include <arpa/inet.h>      // converting ip to number
#include <netinet/ip_icmp.h>// provides declarations for icmp header


// For collecting info
struct ip_stat {
    unsigned ip_address;
    unsigned counter;
};


/*
 * Read-write functions.
 * As daemon should be stable for rebooting it must use
 * file system for saving data
 */
void write_pid(const char *name, int pid);

void read_iface(const char *name, char *iface);

void write_iface(const char *name, const char *iface);


void read_stat(const char *name, struct ip_stat *stat, int *size);

void write_stat(const char *name, struct ip_stat *stat, int *size);

int read_pid(const char *name);



// Debug function for checking statistics
void output(const char *stat_file);

// IP addresses and counters shall be sorted For implementing binary search
void sort(struct ip_stat *statistic, int n);

// Binary search of number of packets by chosen IP address
int search_ip(struct ip_stat *statistics, int size, unsigned ip_addr);

// Starts sniffing by running the sockets
void snif();


#endif //SNIFFER_FUNCTIONS_H
