#include "functions.h"



int main(int argc, char* argv[])
{
    pid_t pid = 0, sid = 0;

    pid = fork();

    // Indication of fork() failure
    if (pid < 0)
    {
        printf("fork failed!\n");
        exit(1);
    }

    // Parent process. Need to kill it.
    if (pid > 0)
    {
        // Write pid to file
        char * path = getcwd(NULL, 1024);
        strcat(path, "/data/pid.dat");

        write_pid(path, pid);

        printf("PID: %d\n", pid);
        exit(0);
    }

    // Set new session
    sid = setsid();
    if(sid < 0)
        exit(1);

    // Close stdin. stdout and stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Start sniffing
    snif();

    return (0);
}