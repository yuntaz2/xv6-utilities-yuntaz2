// lab-1 activity-2
// switch and error check are inspired by Prof.Klefstad's lecture on April 5th
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define BUF_SIZE 8

int main(int argc, char *argv[])
{
    int p[2];
    char buffer[BUF_SIZE];
    pipe(p);
    int pid = fork();
    switch (pid)
    {
    case -1:
    {
        printf("fork() fail at %d\n", pid);
        exit(-1);
    }
    case 0:
    {
        // child process
        int child_pid = getpid();
        read(p[0], buffer, BUF_SIZE);
        printf("%d: received %s\n", child_pid, buffer);
        write(p[1], "pong", 4);
        exit(0);
    }
    default:
    {
        // parent process
        int parent_pid = getpid();
        write(p[1], "ping", 4);
        wait(0);
        read(p[0], buffer, BUF_SIZE);
        printf("%d: received %s\n", parent_pid, buffer);
        exit(0);
    }
    }
}