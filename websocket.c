#ifndef WEBSOCKET
#define GET_TIME_H
#define TIME_BUFFER_SIZE 40
#define BACKLOG 20
#define PORT 8080
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#endif

int init_socket(int *listen_fd,struct socket_in *server_addr)
{
        int i=0;
        int sr=SO_REUSEADDR;
        i=(*listen_fd=socket(AF_INET,SOCK_STREAM,0));
        if(i==-1){
            perror("socket listen error in websocket.c");
            return -1;
        }
        setsockopt(*listen_fd,SOL_SOCKET,SO_REUSEADDR,&sr,sizeof(sr));
        server_addr->sin_family=AF_INET;
        server_addr->sin_port=htons(PORT);
        server_addr->sin_addr.s_addr=htonl(INADDR_ANY);
        bind(*listen_fd,(struct sockaddr *)server_addr,sizeof(struct sockaddr_in));
        listen(*listen_fd,BACKLOG);
        return 0;
}

char *get_time_str(char *time_buf)
{
        time_t ntime;
        struct ti *time_now;
        char *ptr=NULL;
        time(&ntime);
        time_now=gmtime(&ntime);
        ptr=asctime(time_now);
        strcat(time_buf,ptr);
        return timebuf;
}
