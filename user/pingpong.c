#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p[2];
    int buf[1];
    int status;
    pipe(p);
    if (fork() == 0) {
        close(p[1]);
        if (read(p[0], buf, sizeof(buf)) != 1) {
            fprintf(2, "child read fail");
            exit(1);
        }
        close(p[0]);
        fprintf(1, "%d: received ping\n", getpid());
        exit(0);
    }else{
        close(p[0]);
        if (write(p[1], "a", 1) != 1) {
            fprintf(2, "parent write fail");
            exit(1);
        }
        close(p[1]);
        wait(&status);
        fprintf(1, "%d: received pong\n", getpid());
        exit(0);
    }
}