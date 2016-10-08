#include "functions.h"


void output(const char *stat_file) {
    // read statistics
    struct ip_stat * stat = malloc(65536);
    int n = 0;
    read_stat(stat_file, stat, &n);

    printf("N(IP): %d\n", n);

    // line-by-line output each address and number of packets
    struct in_addr tmp;
    int i;
    for (i = 0; i < n; i++) {
        tmp.s_addr = stat[i].ip_address;
        printf("%s: %d\n", inet_ntoa(tmp), stat[i].counter);
    }
    printf("\n");
}

void swap(unsigned * a, unsigned * b) {
    unsigned tmp = *a;
    *a = *b;
    *b = tmp;
}

// bubble sort for sorting by IP address value
void sort(struct ip_stat * statistic, int n) {
    int i = 0, r;

    for (r = n - 1; r > 0; r--) {
        for (i = 0; i < r; i++) {
            if (statistic[i].ip_address > statistic[i+1].ip_address) {
                swap(&statistic[i].ip_address, &statistic[i+1].ip_address);
                swap(&statistic[i].counter, &statistic[i+1].counter);
            }
        }
    }
}

void write_pid(const char *name, int pid) {
    FILE *f = fopen(name, "wb");

    if (f == NULL) return;

    fwrite(&pid, sizeof(int), 1, f);

    fclose(f);
}

void write_stat(const char *name, struct ip_stat *stat, int *size) {
    FILE * f = fopen(name, "wb");
    int i;

    if (f == NULL) return;

    fwrite(size, sizeof *size, 1, f);

    for (i = 0; i < *size; i++)
        fwrite(&stat[i], sizeof(struct ip_stat), 1, f);

    fclose(f);
}

void write_iface(const char *name, const char *iface) {
    FILE *f = fopen(name, "wb");

    if (f == NULL) return;

    fwrite(iface, sizeof(char), strlen(iface)+1, f);

    fclose(f);
}

int read_pid(const char *name) {
    FILE *f = fopen(name, "rb");

    if (f == NULL)
        return -1;

    int pid = -1;
    fread(&pid, sizeof(int), 1, f);

    fclose(f);
    return pid;
}

void read_stat(const char *name, struct ip_stat *stat, int *size) {
    FILE * f = fopen(name, "rb");

    if (f == NULL) {
        *size = 0;
        return;
    }

    fread(size, sizeof(int), 1, f);

    int i = 0;
    for (i = 0; i < *size; i++)
        fread(&stat[i], sizeof(struct ip_stat), 1, f);

    fclose(f);
}


void read_iface(const char *name, char *iface) {
    FILE *f = fopen(name, "rb");

    if (f == NULL) {
        // Set network interface by default
        strcpy(iface, "wlan0");
        return;
    }

    fread(iface, 1, 20, f);
    fclose(f);
}

// log(N) binary search
int search_ip(struct ip_stat * statistics, int size, unsigned ip_addr) {
    int min = 0, max = size-1, i;

    while (min <= max) {
        i = (min + max) / 2;

        if (ip_addr == statistics[i].ip_address)
            return statistics[i].counter;

        if (ip_addr > statistics[i].ip_address)
            min = i + 1;
        else
            max = i - 1;
    }

    return 0;
}