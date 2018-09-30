// 传入参数是输入的字符串以及解析字符串得到的数组储存位置
int read_six_numbers(char *sp, int *np);
//传入字符串，格式，储存地址
int sscanf(const char *restrict s, const char *restrict format, ...);

phase_6()
{
    //first read six numbers in rsp,rsp+4,rsp+8,...,rsp+20
    read_six_numbers();
    r13=r14=rsp;
    r12=0;
    //check every number is <= 6, and every number is different.
    while (1){
        rbp = r13;
        rax = *(r13);
        rax -= 1;
        if (rax > 5){
            explode_bomb();
        }else {
            r12++;
            if(r12==6) 
                break;
        }
        ebx = r12;
        do{
            eax = *(rsp + ebx*4);
            if(eax == *rbp) explode_bomb();
            ebx++;
        } while(ebx <= 5);
        r13 += 4;
    }

    // change xi to 7-xi
    rsi = rsp + 24;
    rax = rsp;
    ecx = 7;
    do{
        edx = ecx;
        edx -= *(rax);
        *(rax) = edx;
        rax += 4;
    } while(rax != rsi);
    
    

    // 这一段看起来是根据传入的值与相应的node对应，所以传入的肯定是node的排序
    // 0x6032d0 <node1>:   0x0000014c  0x00000001  0x006032e0  0x00000000
    // 0x6032e0 <node2>:   0x000000a8  0x00000002  0x006032f0  0x00000000
    // 0x6032f0 <node3>:   0x0000039c  0x00000003  0x00603300  0x00000000
    // 0x603300 <node4>:   0x000002b3  0x00000004  0x00603310  0x00000000
    // 0x603310 <node5>:   0x000001dd  0x00000005  0x00603320  0x00000000
    // 0x603320 <node6>:   0x000001bb  0x00000006  0x00000000  0x00000000
    // 00000000006032d0 g     O .data  0000000000000010              node1 332
    // 00000000006032e0 g     O .data  0000000000000010              node2 168
    // 00000000006032f0 g     O .data  0000000000000010              node3 924
    // 0000000000603300 g     O .data  0000000000000010              node4 691
    // 0000000000603310 g     O .data  0000000000000010              node5 477
    // 0000000000603320 g     O .data  0000000000000010              node6 443
    // node struct contain int, num, pointer to next node
    // node1
    rsi = 0;
    while(1){
        ecx = *(rsp + rsi);
        if (ecx <= 1){
            edx=0x6032d0;
        } else {
            eax = 1;
            edx = 0x6032d0;
            do{
                rdx=*(rdx+0x8); //下一个node
                rax += 1;
            } while (rax != ecx);
        }

        *(rsp+0x20+rsi*2) = rdx; //储存node的地址
        rsi += 4;
        if (rsi == 0x24) break;
    }


    //node最开始是node1->node2->node3->node4->node5->node6
    //现在根据密码来设置node的顺序
    rbx = *(rsp+0x20); //储存的第一个node地址
    rax = rsp+0x28;
    rsi = rsp+0x50; //boundary
    rcx = rbx;
    while (1){
        rdx = *rax;
        *(rcx+0x8) = rdx;
        rax += 8;
        if (rax == rsi) break;
        else rcx = rdx;
    }

    ebp = 0x05;
    do{
        rax = *(rbx+0x8);
        eax = *(rax);
        if (*rbx < eax) explode_bomb(); // 降序排列才不会触发炸弹
        rbx = *(rbx+0x8);
        ebp -= 1;
    } while (ebp != 0);

}