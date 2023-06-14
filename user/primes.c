#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


void primes(int pipe1[2]) {
    int i;
    if (read(pipe1[0], &i, sizeof(i)) != sizeof(i)) {
        fprintf(2, "read pipe failed");
        exit(1);
    }

    fprintf(1, "prime %d\n", i);
    int pipe2[2];
    pipe(pipe2);
    int j;
    if (read(pipe1[0], &j, sizeof(j))) {
        if (fork() == 0) {
            close(pipe2[1]);
            primes(pipe2);
            exit(0);
        }else{
            close(pipe2[0]);
            do {
                if (j % i != 0) {
                    write(pipe2[1], &j, sizeof(j));
                }
            }while(read(pipe1[0], &j, sizeof(j)));
            close(pipe2[1]);
            close(pipe1[0]);
            wait(0);
        }

        exit(0);
    }
}

int main(int argc, char **argv) {
    int pipe1[2];
    pipe(pipe1);

    if (fork() == 0) {
        // 这里如果不关闭，配合父进程关闭管道的写入，则子进程永远不会退出
        close(pipe1[1]);
        primes(pipe1);
        exit(0);
    }else{
        close(pipe1[0]);
        int i;
        for (i = 2; i <= 35; i++) {
            write(pipe1[1], &i, sizeof(i));
        }

        // 此时管道的写入端口都被关闭了，子进程得以退出
        close(pipe1[1]);
        // 这里如果没有wait(0)，输出会异常
        wait(0);
        exit(0);
    }
}
