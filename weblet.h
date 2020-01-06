#ifndef MYWEB_H
#define MYWEB_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netdb.h>
#include <limits.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <fcntl.h>

#define RIO_BUFSIZE 8192
#define MAXLINE 8192
#define MAXBUF 8192
#define LISTENQ 1024

typedef struct sockaddr SA;

typedef struct{
    int rio_fd;
    int rio_cnt;
    char *rio_bufptr;
    char rio_buf[RIO_BUFSIZE];
}rio_t;

void process_trans(int fd);
int is_static(char *uri);
void error_request(int fd,char *cause,char *errnum,char *shortmsg,char *cescription);
void read_requesthdrs(rio_t *rp);
void parse_static_uri(char *uri,char *filename);
void parse_dynamic_uri(char *uri,char *filename,char *cg);
void feed_static(int fd,char *filename,int filesize);
void get_filetype(char *filename,char *filetype);
void feed_dynamic(int fd,char *filename,char *cg);

ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
ssize_t	rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t	rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

int open_client_sock(char *hostname, int portno);
int open_listen_sock(int portno);

void *mmap(void *start,size_t length,int prot,int flags,int fd,off_t offsize);
#endif

