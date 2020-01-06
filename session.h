#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include <netinet/in.h>

#define RECV_BUFFER_SIZE 1024            //接收缓冲区
#define SEND_BUFFER_SIZE 1050000         //发送缓冲区
#define URI_SIZE 128                     //来自客户端浏览的uri请求的长度

#define TIME_OUT_SEC 10                  //
#define TIME_OUT_USEC 0

#define FILE_OK 200
#define FILE_FORBIDEN 403                //没有权限访问
#define FILE_NOT_FOUND 404               //在服务器上找不到
#define UNALLOW_METHOD 405               
#define FILE_TOO_LARGE 413               //文件太大
#define URI_TOO_LONG 414
#define UNSUPPORT_MIME_TYPE 505
#define FILE_MAX_SIZE 1048576            //从硬盘读取文件的大小

#define ALLOW "Allow:GET"                //服务器允许的请求方法
#define SERVER "Server:Mutu(0.1 Alpha)/Linux"

//处理http协议
int http_session(int *connect_fd,struct sockaddr_in *client_addr);

//判断是否为http协议，是返回1
int is_http_protocol(char *msg_from_client);

//获取请求头的标识符
char *get_uri(char *req_header,char *uri_buf);

//获取标识符状态
int get_uri_status(char *uri);

//
char *get_mime_type(char *uri);

int get_file_disk(char *uri,unsigned char *entity_buf);

int set_rep_status();

int set_error_information(unsigned char *send_buf,int errorno);

int reply_normal_information(unsiged char *send_buf,unsigned char *file_buf,int file_size,char *mime_type);

#endif

