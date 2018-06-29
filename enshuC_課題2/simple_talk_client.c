#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define PORT 10130

int main(int argc, char **argv) {
	int sock, nbytes;
	char sbuf[1024], *name, cbuf[1024];
	struct sockaddr_in svr;
	struct hostent *hp;
	fd_set rfds;
	struct timeval tv;

	if(argc != 2) {
		fprintf(stderr, "Usage: %s hostname \n", argv[0]);
		exit(1);
	}else{
	  name = *(argv+1);
	}

	if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
	  perror("socket");
	  exit(1);
	}

	bzero(&svr, sizeof(svr));
	svr.sin_family = AF_INET;
	svr.sin_port = htons(PORT);
	if((hp = gethostbyname(name)) == NULL){
	  perror("gethostbyname");
	  exit(1);
	}
	bcopy(hp->h_addr, &svr.sin_addr, hp->h_length);

	if((connect(sock, (struct sockaddr *)&svr, sizeof(svr))) < 0){
	    perror("connect");
	    exit(1);
	}
	else {
		printf("connected\n" );
	}

	do{
	  FD_ZERO(&rfds);
	  FD_SET(0, &rfds);
	  FD_SET(sock, &rfds);
	  tv.tv_sec = 1;
	  tv.tv_usec = 0;
	  memset(sbuf, '\0', sizeof(sbuf));
		memset(cbuf, '\0', sizeof(cbuf));

	  if(select(sock+1, &rfds, NULL, NULL, &tv) > 0){
		  if(FD_ISSET(sock, &rfds)){
		    if((nbytes = read(sock, cbuf, sizeof(cbuf))) > 0){
		      write(1, cbuf, nbytes);
		    }
		  }
		  if(FD_ISSET(0, &rfds)){
		    if((nbytes = read(0, sbuf, sizeof(sbuf))) > 0){
		      write(sock, sbuf, nbytes);
		    }
		  }
	  }
	} while(nbytes != 0);
	close(sock);
	printf("closed\n");
}
