#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

char msg[256]="aisdjfieijcidjafijeijaisdhfisahdfdshfkcukeuhfe234283hr8efh9whf3uhueuehfjh";

int main(){
    int cfd;

    struct sockaddr_in caddr;
    memset(&caddr, 0, sizeof(struct sockaddr_in));

    cfd = socket(AF_INET, SOCK_STREAM, 0);

    caddr.sin_family = AF_INET;
    caddr.sin_port = htons(7777);
    inet_aton("127.0.0.1", &caddr.sin_addr);

    connect(cfd, (struct sockaddr *) &caddr, sizeof(struct sockaddr));

    for(int i = 1; i <= 1e7; ++i){
        send(cfd, msg, strlen(msg), 0);

        char str[300];
        memset(str, 0, sizeof(str));
        int len = recv(cfd, str, 299, 0);
    }

    return 0;
}
