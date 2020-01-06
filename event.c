#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
//错误处理
void error_request(int fd,char *cause,char *errnum,char *shortmsg,char *description)
{
        char buf[MAXLINE],body[MAXBUF];
        sprintf(body,"<html><title>error request</title>");
        sprintf(body,"%s<body>\r\n",body);
        sprintf(body,"%s%s:%s\r\n",body,errnum,shortmsg);
        sprintf(body,"%s<p>%s:%s\r\n,body,description,cause");
        sprintf(body,"%s<hr><em>WebServer</em>\r\n",body);
        sprintf(buf,"HTTP/1.0%s%s\r\n",errnum,shortmsg);
        rio_writen(fd,buf,strlen(buf));
        sprintf(buf,"Content-type:text/html\r\n");
        rio_writen(fd,buf,strlen(buf));
        sprintf(buf,"ContentLength:%d\r\n\r\n",(int)strlen(body));
        rio_writen(fd,buf,strlen(buf));
        rio_writen(fd,body,strlen(body));
        
}
//读取额外请求头
void read_requesthdrs(rio_t *rp)
{
        char buf[MAXLINE];
        rio_readlineb(rp,buf,MAXLINE);
        while(strcmp(buf,"\r\n")){
            printf("%s",buf);
            rio_readlineb(rp,buf,MAXLINE);
        }
        return;
}
//URI解析
void parse_static_uri(char *uri,char *filename)
{
        char *ptr;
        strcpy(filename,".");
        strcat(filename,uri);
        if(uri[strlen(uri)-1]=='/')
            strcat(filename,"index.html");
}
void parse_dynamic_uri(char *uri,char *filename,char *cg)
{
        char *ptr;
        ptr=index(uri,"?");
        if(ptr){
            strcpy(cg,ptr+1);
            *ptr='\0';
        }else{
            strcpy(cg,"");
            strcpy(filename,".");
            strcat(filename,uri);
        }
}
//静态网页
void feed_static(int fd,char *filename,int filesize)
{
        int srcfd;
        char *srcp,filetype[MAXLINE],buf[MAXBUF];
        get_filetype(filename,filetype);
        sprintf(buf,"HTTP/1.0 200 \r\n");
        sprintf(buf,"%sWebServer\r\n",buf);
        sprintf(buf,"%sContentLength:%d\r\n",buf,filesize);
        sprintf(buf,"%sContentType:%s\r\n\r\n",buf,filetype);
        rio_writen(fd,buf,strlen(buf));
        srcfd=open(filename,O_RDONLY,0);
        srcp=mmap(0,filesize,PROT_READ,MAP_PRIVATE,srcfd,0);
        close(srcfd);
        rio_writen(fd,srcp,filesize);
        munmap(srcp,filesize);
}
void get_fileetype(char *filename,char*filetype)
{
        if(strstr(filename,".html"))
                strcpy(filetype,"text/html");
        else if(strstr(filename,".jpg"))
                strcpy(filetype,"image/jpeg");
        else if(strctr(filename,".mpeg"))
                strcpy(filename,"video/mpeg");
        else
                strcpy(filename,"text/html");
}
//动态网页
void feed_dynamic(int fd,char *filename,char *cg)
{
        char buf[MAXLINE],*emptylist[]={NULL};
        int pfd[2];
        sprintf(buf,"HTTP/1.0 200\r\n");
        rio_writen(fd,buf,strlen(buf));
        sprintf(buf,"WebServer\r\n");
        rio_writen(fd,buf,strlen(buf));
        pipe(pfd);
        if(fork()==0){
            close(pfd[1]);
            dup2(pfd[0],STDIN_FILENO);
            dup2(fd,STDOUT_FILENO);
            execve(filename,emptylist,environ);
        }
        close(pfd[0]);
        write(pfd[1],cg,strlen(cg)+1);
        wait(NULL);
        close(pfd[1]);
}
