#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

int main(){
	int sfd;

	struct sockaddr_in saddr, caddr;
	memset(&saddr, 0, sizeof(struct sockaddr_in));
	memset(&caddr, 0, sizeof(struct sockaddr_in));

	sfd = socket(AF_INET, SOCK_STREAM, 0);

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(7777);
	inet_aton("127.0.0.1", &saddr.sin_addr);
	bind(sfd, (struct sockaddr *) &saddr, sizeof(struct sockaddr));

	listen(sfd, 1);

	int clen = sizeof(struct sockaddr_in);
	int cfd = accept(sfd, (struct sockaddr *) &caddr, &clen);

	while(1) {
		char buf[300];
		memset(buf, 0, sizeof(buf));
		int len = recv(cfd, buf, 255, 0);
		strcat(buf, " accept");
		send(cfd, buf, strlen(buf), 0);
	}

	return 0;
}

