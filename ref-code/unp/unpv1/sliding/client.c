#include "miscellaneous.h"
// 隐式地采用错误处理(调用开头大写的库函数名表示用eroor handler包装)


static void sig_alrm(int signo);

int main(int argc, char const *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;

    if(argc != 4)
        err_quit("usage: %s <host> <port> <file>\n", argv[0]);

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    // 将点分十进制格式转换为网络地址格式
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    unsigned short port_num = atoi(argv[2]);
    servaddr.sin_port = htons(port_num);

    // 设置需要连接的服务端以及本地客户端的套接字对
    Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
    

    DF dgframe;
    dgframe.seqnum = 0;
    // 打开指定读取文件
    int fd = Open(argv[3], O_RDONLY, 0);

    // 设置定时器回调函数
    Signal(SIGALRM, sig_alrm);
    
    // 用于格式控制
    int bound = 60;
    while(Read(fd, dgframe.data, sizeof(dgframe.data))){
        printf("%s\n", dgframe.data);
        // 分界线,控制格式
        for(int i = 0; i < bound; ++i) 
            printf("-");
        printf("\n");
        printf("Send %u bytes\n", (unsigned int)(sizeof(dgframe)));
        for(int i = 0; i < bound; ++i) 
            printf("*");
        printf("\n");
        
        
        // 模拟信道出错, 向服务器发送带序号的数据帧
        dgframe.flag = random_corrupt(0.5);
        Write(sockfd, &dgframe, sizeof(dgframe));
        ++dgframe.seqnum;
        
        
        // 接收服务器的回应
        AF ackframe;
        // 设置定时器, 服务器在一定时间不响应则调用回调函数中断read
        alarm(1);
        int n;
        if( (n = read(sockfd, &ackframe, sizeof(ackframe)) < 0)){
            if(errno == EINTR)
                fprintf(stderr, "Server response timeout\n");
            else
                err_sys("read error");
        } else {
            // 收到回应,解除定时器
            alarm(0);
            //解析收到的ackframe
            printf("Get response from server:\n");
            printf("RECEIVED: %d bytes\n",ackframe.recvbytes);
            printf("Sequence number: %d\n", ackframe.seqnum);
        }

        for(int i = 0; i < bound; ++i) 
            printf("-");
            getchar();
        
    }
    return 0;
}


static void sig_alrm(int signo)
{
    return;
}