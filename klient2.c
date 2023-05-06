#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    int cl_fd;
    const char* fifo_name = "cl_fifo_2";
    char fifo_buf[1000];

    //otwieramy kolejke fifo tworzona przez serwer    
    do{
        cl_fd = open(fifo_name, O_WRONLY);

        if(cl_fd == -1){
            perror("client wr_open failed");
            exit(EXIT_FAILURE);
        }

        //wczytujemy polecenie do wykonania
        printf("Podaj polecenie: ");
        
        fgets(fifo_buf, 1000, stdin);

        write(cl_fd, fifo_buf, 1000);
        
        memset(fifo_buf, 0, 1000);
        
        close(cl_fd);
        
        printf("\n");

        cl_fd = open(fifo_name, O_RDONLY);

        read(cl_fd, fifo_buf, 1000);

        printf("Wynik polecenia:\n%s\n", fifo_buf);

        memset(fifo_buf, 0, 1000);

        close(cl_fd);
    }
    while(1);
    return 0;
}