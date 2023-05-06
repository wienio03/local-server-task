#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

#define CLIENT_AMOUNT 1
int main(int argc, char* argv[]){
    const char* prefix = "cl_fifo_";
    char full_names[CLIENT_AMOUNT][100];
    char fifo_buffers[CLIENT_AMOUNT][1000];
    int fifo_fds[CLIENT_AMOUNT];
    for(int i = 0; i < CLIENT_AMOUNT; i++){
        sprintf(full_names[i], "%s%d", prefix, i);
        mkfifo(full_names[i], 0666);
    }
    while(1){
        for(int i = 0; i < CLIENT_AMOUNT; i++){
            fifo_fds[i] = open(full_names[i], O_RDONLY);
            read(fifo_fds[i], fifo_buffers[i], 1000);
            if(fifo_buffers[i][0] == '\n'){
                close(fifo_fds[i]);
                unlink(full_names[i]);
                exit(EXIT_SUCCESS);
            }
            else{
                close(fifo_fds[i]);
                int fd[2];
                if(pipe(fd) == -1){
                    perror("pipe failed");
                    exit(EXIT_FAILURE);
                }
                dup2(fd[1], 1);
                system(fifo_buffers[i]);
                close(fd[1]);
                fifo_fds[i] = open(full_names[i], O_WRONLY);
                read(fd[0], fifo_buffers[i], 1000);
                write(fifo_fds[i], fifo_buffers[i], 1000);
                close(fifo_fds[i]);
                close(fd[0]);
                memset(fifo_buffers[i], 0, 1000);
            }
        }
    }

    return 0;
}