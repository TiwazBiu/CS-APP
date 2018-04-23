#include "miscellaneous.h"
// 隐式地采用错误处理(调用开头大写的库函数名表示用eroor handler包装)
int main(int argc, char const *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;

    if(argc != 4){
        fprintf(stderr, "usage: %s <host> <port> <file>\n", argv[0]);
        exit(0);
    }

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    // 将点分十进制格式转换为网络地址格式
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    unsigned short port_num = atoi(argv[2]);
    servaddr.sin_port = htons(port_num);


    DF frame;
    frame.seqnum = 0;
    // 每次读取文件最多MAXBUF个字节
    FILE *fp = Fopen(argv[3],"r");
    while(Fgets(frame.data, MAXBUF, fp) != NULL){
        printf("%s\n", frame.data);
        // 没啥用的分界线
        for(int i = 0; i < 20; ++i) 
            printf("*");
        printf("\n");
        // 模拟信道出错, 向服务器发送带序号的数据帧
        frame.flag = random_corrupt(0.5);
        Sendto(sockfd, &frame, sizeof(frame), 0, 
                (SA *)&servaddr, sizeof(servaddr));
        ++frame.seqnum;
        
        // 接收服务器的回应
        // 设置地址指针以及长度指针为NULL表示不在乎到底是谁向我发送回应
        AF ackframe;
        Recvfrom(sockfd, &ackframe, sizeof(ackframe), 0, NULL, NULL);
        
        printf("Get response from server:\n");
        printf("RECEIVED: %d bytes\n",ackframe.recvbytes);
        printf("Sequence number: %d\n", ackframe.seqnum);
        if(ackframe.flag == NOT_CORRUPTED)
            printf("not corrupted\n");
        else
            printf("but is corrupted\n");
        // 分界线
        for(int i = 0; i < 20; ++i) 
            printf("-");
        getchar();
    }
    return 0;
}