#include "functions.h"

/*
 * File is created as an interface for daemon.c
 */

void set_paths(char **p_daem, char **p_pid, char **p_stat, char **p_iface);



int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Use --help\n");
        return -1;
    }

    // Set current directory path
    char *path_daemon = NULL, *path_pid = NULL, *path_stat = NULL, *path_iface = NULL;
    set_paths(&path_daemon, &path_pid, &path_stat, &path_iface);


    int status = -1;
    if (!strcmp(argv[1], "start")) {
        int pid = read_pid(path_pid);   // create background worker

        if (pid > 0) {
            printf("Daemon already started\n");
            exit(-1);
        }

        // Reset statistics file
        FILE * f = fopen(path_stat, "wb");
        fclose(f);

        printf("Status: %d\n", system(path_daemon));

    } else if (!strcmp(argv[1], "stop")) {
        int pid = read_pid(path_pid);

        if (pid > 0) {
            // Forming command: kill [PID]
            char command[20], c_pid[10];
            sprintf(c_pid, "%d", pid);
            strcpy(command, "kill ");
            strcat(command, c_pid);

            status = system(command);

            // Reset pid for no stopping same process in future
            write_pid(path_pid, -1);
        }

        printf("Status: %d\n", status);

    } else if (argc >= 3 && !strcmp(argv[1], "show")) {
        // read statistics from file
        struct ip_stat * stat = malloc(65536);
        int stat_size = 0;
        read_stat(path_stat, stat, &stat_size);

        // read passed argument of IP address
        unsigned ip = inet_addr(argv[2]);

        printf("Count: %d\n", search_ip(stat, stat_size, ip));

    } else if (argc == 4 && !strcmp(argv[1], "select") && !strcmp(argv[2], "iface")) {
        write_iface(path_iface, argv[3]);
        printf("Reboot for using interface %s\n", argv[3]);

    } else if (!strcmp(argv[1], "--help")) {
        printf("Use '[PATH]/cli start' packets are being sniffed from now on\n");
        printf("Use '[PATH]/cli stop' packets are not sniffed\n");
        printf("Use '[PATH]/cli show [IP] count' printf number of packets received from ip address\n");
        printf("Use '[PATH]/cli select iface [IFACE]' to select interface from sniffing\n");
        printf("Use '[PATH]/cli stat' to look at full statistics\n");

    } else if (argc == 2 && !strcmp(argv[1], "stat")) {
        output(path_stat);
        
    } else
        printf("No command found\n");
    
    return 0;
}

// Set files paths for accessing daemon's data
void set_paths(char **p_daem, char **p_pid, char **p_stat, char **p_iface) {
    *p_pid = getcwd(NULL, 1024);
    *p_daem = getcwd(NULL, 1024);
    *p_stat = getcwd(NULL, 1024);
    *p_iface = getcwd(NULL, 1024);

    strcat(*p_pid, "/data/pid.dat");
    strcat(*p_daem, "/daemon");
    strcat(*p_stat, "/data/stat.dat");
    strcat(*p_iface, "/data/iface.dat");
}
