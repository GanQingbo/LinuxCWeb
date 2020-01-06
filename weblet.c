#include "weblet.h"
int main(int argc, char **argv)
{
    int listen_sock, conn_sock, port, clientlen;
    struct sockaddr_in clientaddr;
    pthread_t tid;
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
            pthread_create(&tid,NULL,(void*(*)(void*))process_trans,(void*)&conn_sock);//创建处理线程
            close(conn_sock); //关闭连接
    }
}
