#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>

int main(int argc,char *argv[])
{
    int listen_fd;
    int connect_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    bzero(&server_addr, sizeof(struct sockaddr_in));
    bzero(&client_addr, sizeof(struct sockaddr_in));

    if(init_socket(&listen_fd, &server_addr) == -1)
    {
        perror("init's error in server.c")    
        exit(EXIT_FAILURE);
    }

    socklen_t addrlen = sizeof(struct sockaddr_in);
    pid_t pid;

    while(1)
    {
        if((connect_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addrlen)) == -1)
        {
            perror("accept'error in server.c");
            continue;
        }
        if( (pid = fork()) > 0)
        {
            close(connect_fd);
            continue;
        }
        else if(pid == 0)
        {
            close(listen_fd);
            printf("pid %d process session from %s : %d\n", getpid(), inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
            if(http_session(&connect_fd, &client_addr) == -1)
            {
                perror("session error in server.c");
                shutdown(connect_fd, SHUT_RDWR);
                printf("pid %d loss connection to %s\n", getpid(), inet_ntoa(client_addr.sin_addr));
                exit(EXIT_FAILURE);
            }

            printf("pid %d close connection to %s\n", getpid(), inet_ntoa(client_addr.sin_addr));
            shutdown(connect_fd, SHUT_RDWR);
            exit(EXIT_SUCCESS);
        }
        else
        {
            perror("fork error in server.c");
            exit(EXIT_FAILURE);
        }

    }


    shutdown(listen_fd, SHUT_RDWR);
    return 0;

}
