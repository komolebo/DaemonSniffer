#include "functions.h"

void process_packet(unsigned char *buffer, struct ip_stat *stat, int *n);


void snif() {
    // For sockets managing
    int saddr_size, data_size, sock_raw;
    struct sockaddr saddr;
    unsigned char *buffer = (unsigned char *) malloc(65536);

    // For collecting info from network
    struct ip_stat * stat = malloc(65536);
    int stat_size = 0;

    // For managing file names
    char *PATH_STAT = getcwd(NULL, 1024), *PATH_IFACE = getcwd(NULL, 1024);
    char *iface = malloc(20);
    strcat(PATH_STAT, "/data/stat.dat");
    strcat(PATH_IFACE, "/data/iface.dat");

    // Read data from files
    read_stat(PATH_STAT, stat, &stat_size);
    read_iface(PATH_IFACE, iface);

    //Create a raw socket that shall sniff
    sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

    // Set specific interface
    setsockopt(sock_raw, SOL_SOCKET, SO_BINDTODEVICE, iface, 4);

    if (sock_raw < 0) {
        printf("Socket Error\n");
        return;
    }

    // Background working
    while (1) {
        saddr_size = sizeof saddr;

        //Receive a packet
        data_size = recvfrom(sock_raw, buffer, 65536, 0, &saddr, &saddr_size);

        if (data_size < 0) {
            printf("Recvfrom error, failed to get packets\n");
            close(sock_raw);
            return;
        }

        process_packet(buffer, stat, &stat_size);

        write_stat(PATH_STAT, stat, &stat_size);
    }
}

void process_packet(unsigned char *buffer, struct ip_stat *stat, int *n) {
    struct iphdr *iph = (struct iphdr *) buffer;
    int i;

    for (i = 0; i < *n; i++)
        if (stat[i].ip_address == iph->saddr) {
            stat[i].counter++;
            return;
        }

    // If we're here, then we got package from new IP-address
    stat[i].ip_address = iph->saddr;
    stat[i].counter = 1;
    (*n)++;

    // For binary search the data should be sorted
    sort(stat, *n);
}