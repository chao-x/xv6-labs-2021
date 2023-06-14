#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"


int read_line(char *args[], int i) {
    char buf[1024];
    int j = 0;
    while(read(0, buf+j, 1)) {
        if (buf[j] == '\n') {
            buf[j] = '\0';
            break;
        }
        j++;
    }

    if (j == 0) {
        return -1;
    }

    int left = 0;
    while(left < j) {
        // 忽略前面的空格
        while((left < j) && buf[left] == ' ') {
            left++;
        }
        int right = left;
        while((right < j) && buf[right] != ' ') {
            right++;
        }

        // 只有两种情况 buf[right] 此时为 ' '或者为 '\0' 
        buf[right] = '\0';

        // 不能直接指定，因为buf执行完后会被回收
        // args[i] = &buf[left]
        args[i] = (char*) malloc((right - left + 1) * sizeof(char));
        strcpy(args[i], &buf[left]);

        i++;
        left = right + 1;
    }
    return i;
}

int
main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "error, args count < 2");
    }
    char *args[MAXARG];
    int i;
    for (i = 1; i < argc; i++) {
        args[i-1] = argv[i];
    }

    int end;
    // 这里注意 read_line传入的参数为i-1, i-1 =- argc-1, 否则exec不会正常执行
    while((end = read_line(args, i-1)) != -1) {
        args[end] = 0;
        if (fork() == 0) {
            exec(args[0], args);
            exit(0);
        }else{
            wait(0);
        }
    }
    exit(0);

}