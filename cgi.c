#include "weblet.h"
int main(void){
    char *buf,*p;
    char content[MAXLINE];
    int n1=0,n2=0;
    scanf("%d&%d",&n1,&n2);
    sprintf(content,"%s%d+%d=%d\r\n",content,n1,n2,n1+n2);
    printf("Content-length:%d\r\n",strlen(content));
    printf("Content-type:text/html\r\n\r\n");
    printf("%s",content);
    fflush(stdout);
    exit(0);
}
