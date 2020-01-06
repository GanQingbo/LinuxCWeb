#ifndef MYWEB_H
#define MYWEB_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <apra/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h>

#define DEF_MODE   S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
#define DEF_UMASK  S_IWGRP|S_IWOTH
#define RIO_BUFSIZE 8192
#define MAXLINE 8192
#define MAXBUF 8192
#define LISTENQ 1024

typedef struct{
    int rio_fd;
    int rio_cnt;
    char *rio_bufptr;
    char rio_buf[RIO_BUFSIZE];
}rio_t;
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};
void process_trans(int fd);
int is_static(char *uri);
void error_request(int fd,char *cause,char *errnum,char *shortmsg,char *cescription);
void read_requesthdrs(rio_t *rp);
void parse_static_uri(char *uri,char *filename);
void parse_dynamic_uri(char *uri,char *filename,char *cg);
void feed_static(int fd,char *filename,int filesize);
void get_filetype(char *filename,char *filetype);
void feed_dynamic(int fd,char *filename,char *cg);


#endif

