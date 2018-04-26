#include "miscellaneous.h"

int main(int argc, char const *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;

    if(argc != 2){
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    unsigned short port_num = atoi(argv[1]);
    servaddr.sin_port = htons(port_num);
    // 服务器绑定网卡所有IP地址
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 最后得到在本地所有地址的port端口监听的套接字
    Bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

    while (1){
        struct sockaddr_in clientaddr;
        SA *pcliaddr = (SA *)&clientaddr;
        socklen_t salen = sizeof(clientaddr);
        DF recvframe;
        
        bzero(&recvframe, sizeof(recvframe));
        // 服务端由recvfrom接收信息并得知客户端地址,并不需要事先知道
        int n = Recvfrom(sockfd, &recvframe, sizeof(recvframe), 0, 
                         pcliaddr, &salen);
        printf("%d: %s\n", recvframe.seqnum, recvframe.data);

        // 服务器对发送信息的客户端进行回应
        // 如果接收到的帧flag设置为IS_CORRUPTED,则不应答
        if(recvframe.flag == IS_CORRUPTED) 
            continue;
        AF ackframe;
        ackframe.seqnum = recvframe.seqnum;
        ackframe.flag = recvframe.flag;
        ackframe.recvbytes = n;
        Sendto(sockfd, &ackframe, sizeof(ackframe), 0, pcliaddr, salen);
        
    }
    return 0;
}