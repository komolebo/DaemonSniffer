#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "functions.h"


int main() {
    struct ip_stat * stat = malloc(65536);
    int size = 0;

    read_stat("/root/ClionProjects/Sniffer/stat.dat", stat, &size);

    char * ip = "74.125.232.255";
    unsigned addr = inet_addr(ip);

    output("/root/ClionProjects/Sniffer/data/stat.dat");

    int pid = read_pid("/root/ClionProjects/Sniffer/data/pid.dat");
    printf("pid = %d\n", pid);
    printf("N = %d\n", search_ip(stat, size, addr));

    return 0;
}
