#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>//主机序与网络序转换
#include<apra/inet.h>//IP地址点分十进制与网络序转换
#include<sys/types.h>
#include<sys/socket.h>//socket函数


void process_trans(int fd);
void read_requesthdrs(rio_t *rp);
int is_static(char *uri);
void parse_static_uri(char *uri, char *filename);
void parse_dynamic_uri(char *uri, char *filename, char *cgiargs);
void feed_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void feed_dynamic(int fd, char *filename, char *cgiargs);
void error_request(int fd, char *cause, char *errnum,
                 char *shortmsg, char *description);

int main(int argc, char **argv)
{
    int listen_sock, conn_sock, port, clientlen;
    struct sockaddr_in clientaddr;

    /*检查命令行和参数 */
    if (argc != 2) {
            fprintf(stderr, "usage: %s <port>\n", argv[0]);
            exit(1);
    }
    port = atoi(argv[1]);

    listen_sock = open_listen_sock(port);//打开监听套接字
    while (1) {//利用循环不断接收链接请求
            clientlen = sizeof(clientaddr);
            conn_sock = accept(listen_sock, (SA *)&clientaddr, &clientlen);
            process_trans(conn_sock);          //处理HTTP事务
            close(conn_sock); //关闭连接
    }
}

//读出并解释请求，判断网页类型，调用函数提供相应网页内容
void process_trans(int fd)
{
    int static_flag;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    /* 读取请求行和标题 */
    rio_readinitb(&rio, fd);//用文件描述符fd初始化rio文件及缓冲区
    rio_readlineb(&rio, buf, MAXLINE);//读出请求行，复制到buf缓冲区
    sscanf(buf, "%s %s %s", method, uri, version);
    if (strcasecmp(method, "GET")) {//判断是否为GET
       error_request(fd, method, "501", "Not Implemented",
                "weblet does not implement this method");
       return;
    }
    read_requesthdrs(&rio);//读取其他请求头

    static_flag=is_static(uri);//判断请求是静态内容还是动态内容
    if(static_flag)
        parse_static_uri(uri, filename);
    else
        parse_dynamic_uri(uri, filename, cgiargs);

    if (stat(filename, &sbuf) < 0) {
            error_request(fd, filename, "404", "Not found",
                    "weblet could not find this file");
            return;
    if (static_flag) { //处理静态内容
            if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {//判断是否有读权限
               error_request(fd, filename, "403", "Forbidden",
                            "weblet is not permtted to read the file");
                return;
            }
            feed_static(fd, filename, sbuf.st_size);//将静态页面发送给客户端
    }
    else { /* 处理动态内容 */
            if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {//判断是否有执行权限
                error_request(fd, filename, "403", "Forbidden",
                        "weblet could not run the CGI program");
                return;
            }
            feed_dynamic(fd, filename, cgiargs);//将动态页面发送给客户端
    }
}


int is_static(char *uri)
{
    if (!strstr(uri, "cgi-bin"))
        return 1;
    else
        return 0;
}
