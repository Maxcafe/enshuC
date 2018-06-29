#include <sys/types.h> /* socket() を使うために必要*/
#include <sys/socket.h> /* 同上 */
#include <netinet/in.h> /* INET ドメインのソケットを使うために必要*/
#include <arpa/inet.h> /* 同上 */
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define PORT 10120

int main(int argc, char **argv) {
  int sock;
  char str[1024], receive[1024];
  struct sockaddr_in host;
  struct hostent *hp;

  /* コマンド引数が1つであることを確認 */
  if(argc != 2) {
    fprintf(stderr, "usage: %s host\n", argv[0]);
    exit(1);
  }

  /* ソケットの生成 */
  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("client: socket");
    exit(1);
  }

  /*
   * host構造体に接続するサーバーのアドレス・ポート番号を設定
   */

  bzero((char *)&host, sizeof(host));
  host.sin_family = AF_INET;
  host.sin_port = htons(PORT);

  if((hp = gethostbyname(argv[1])) == NULL) {
    fprintf(stderr, "unkown host %s\n", argv[1]);
    close(sock);
    exit(1);
  }
  bcopy(hp->h_addr, &host.sin_addr, hp->h_length);

  /* ソケットをサーバに接続 */
  if (connect(sock, (struct sockaddr *)&host, sizeof(host)) > 0) {
    perror("client: connect");
    close(sock);
    exit(1);
  }

memset(str, '\0', sizeof(str));

while (read(0, str, sizeof(str)) > 0) {
  memset(receive, '\0', sizeof(receive));
  if(write(sock, str, sizeof(str)) < 0) {
    perror("write");
    exit(1);
  }

  while (read(sock, receive, sizeof(receive)) < 0) {
    perror("read");
  }
    write(1, receive, sizeof(receive));
  // printf("%s\n", receive);
}

  /* ソケットをクローズ */
  close(sock);
}
