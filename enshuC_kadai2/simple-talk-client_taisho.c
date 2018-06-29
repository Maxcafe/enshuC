#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define PORT 10130

int main(int argc,char **argv) {
int sock;
struct sockaddr_in host;
struct hostent *hp;
char str[1024],receive[1024];
int csock;
fd_set rfds;
struct timeval tv;

if ((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0) {
perror("socket");
exit(1);
}

bzero(&host,sizeof(host));
//構造体全体を初期化するためのコマンド
host.sin_family=AF_INET;//アドレスファミっリ
host.sin_port=htons(PORT);//ポート番号
//gethostbyname()ライブラリコールは引数で与えられたホスト名に関するhostent構造体へのポインタを返す
if ( ( hp = gethostbyname(argv[1]) ) == NULL ) {
fprintf(stderr,"unknown host %s\n",argv[1]);
exit(1);
}



bcopy(hp->h_addr,&host.sin_addr,hp->h_length);
//hostのsin_addrにｈｐ取得したアドレスである
//connectをどうやって使うか？
connect(sock , (struct sockaddr*)&host, sizeof(host) );


do{
  memset(str,'\0',sizeof(str));
  FD_ZERO(&rfds);
  FD_SET(0,&rfds);
  FD_SET(sock,&rfds);
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  if(select(sock+1,&rfds,NULL,NULL,&tv)>0) {
    if(FD_ISSET(0,&rfds)) {
      //scanf("%s", str);
      read(0,str,sizeof(str));
      printf("str = %s\n",str );
      write(sock, str, sizeof(str));
    }

    if(FD_ISSET(sock,&rfds)) {
      read(sock, receive, sizeof(receive));
      printf("%s\n", receive);
    }
  }
} while(1);



close(sock);
printf("closed\n");
}
