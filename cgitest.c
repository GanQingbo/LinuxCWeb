#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "weblet.h"
int main(void)
{
        char *buf,*ptr;
        char content[MAXLINE];
        int i,n=0;
        double sum=1;
        scanf("%d",&n);
        for(i=1;i<n;i++){
            sum=sum*i;
        }
        sprintf(content,"CGI TEST:");
        sprintf(content,"%s%d的阶乘是:%lf\r\n",content,n,sum);
        printf("ContentLength:%d\r\n",strlen(content));
        printf("ContentType:text/html\r\n\r\n");
        printf("%s",content);
        fflush(stdout);
        exit(0);

}
