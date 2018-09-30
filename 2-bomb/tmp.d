00000000004010f4 <phase_6>:
  4010f4:   41 56                   push   %r14
  4010f6:   41 55                   push   %r13
  4010f8:   41 54                   push   %r12
  4010fa:   55                      push   %rbp
  4010fb:   53                      push   %rbx
  4010fc:   48 83 ec 50             sub    $0x50,%rsp
  
  //read six numbers
  //check every number is <= 6, and every number is different.
  401100:   49 89 e5                mov    %rsp,%r13
  401103:   48 89 e6                mov    %rsp,%rsi
  401106:   e8 51 03 00 00          callq  40145c <read_six_numbers>
  40110b:   49 89 e6                mov    %rsp,%r14 
  40110e:   41 bc 00 00 00 00       mov    $0x0,%r12d//r12=0
  401114:   4c 89 ed                mov    %r13,%rbp //rbp=r13,initialize
  401117:   41 8b 45 00             mov    0x0(%r13),%eax
  40111b:   83 e8 01                sub    $0x1,%eax 
  40111e:   83 f8 05                cmp    $0x5,%eax //test xi-1 < 5
  401121:   76 05                   jbe    401128 <phase_6+0x34>
  401123:   e8 12 03 00 00          callq  40143a <explode_bomb>
  401128:   41 83 c4 01             add    $0x1,%r12d
  40112c:   41 83 fc 06             cmp    $0x6,%r12d
  401130:   74 21                   je     401153 <phase_6+0x5f>
  401132:   44 89 e3                mov    %r12d,%ebx
  401135:   48 63 c3                movslq %ebx,%rax
  401138:   8b 04 84                mov    (%rsp,%rax,4),%eax
  40113b:   39 45 00                cmp    %eax,0x0(%rbp)
  40113e:   75 05                   jne    401145 <phase_6+0x51>
  401140:   e8 f5 02 00 00          callq  40143a <explode_bomb>
  401145:   83 c3 01                add    $0x1,%ebx
  401148:   83 fb 05                cmp    $0x5,%ebx
  40114b:   7e e8                   jle    401135 <phase_6+0x41>
  40114d:   49 83 c5 04             add    $0x4,%r13
  401151:   eb c1                   jmp    401114 <phase_6+0x20>
  


  // change xi to 7-xi
  401153:   48 8d 74 24 18          lea    0x18(%rsp),%rsi //rsi=rsp+0x18,boundary
  401158:   4c 89 f0                mov    %r14,%rax //rax=rsp
  40115b:   b9 07 00 00 00          mov    $0x7,%ecx //ecx=0x7
  401160:   89 ca                   mov    %ecx,%edx //edx=0x7
  401162:   2b 10                   sub    (%rax),%edx //edx-=x1
  401164:   89 10                   mov    %edx,(%rax) //x1=edx
  401166:   48 83 c0 04             add    $0x4,%rax
  40116a:   48 39 f0                cmp    %rsi,%rax
  40116d:   75 f1                   jne    401160 <phase_6+0x6c> //change xs to 7-xs
  

3 4 5 6 1 2
00000000006032d0 g     O .data  0000000000000010              node1 332
00000000006032e0 g     O .data  0000000000000010              node2 168
00000000006032f0 g     O .data  0000000000000010              node3 924
0000000000603300 g     O .data  0000000000000010              node4 691
0000000000603310 g     O .data  0000000000000010              node5 477
0000000000603320 g     O .data  0000000000000010              node6 443
  

  40116f:   be 00 00 00 00          mov    $0x0,%esi //esi=0
  401174:   eb 21                   jmp    .L1, 401197 <phase_6+0xa3> //jump to 401197

.L2:
  401176:   48 8b 52 08             mov    0x8(%rdx),%rdx //rdx=*(0x6032d0+0x8)
  40117a:   83 c0 01                add    $0x1,%eax //eax+=1
  40117d:   39 c8                   cmp    %ecx,%eax 
  40117f:   75 f5                   jne    .L2, 401176 <phase_6+0x82> //if !=, jump
  401181:   eb 05                   jmp    .L3, 401188 <phase_6+0x94> // eles, jump

.L5:
  401183:   ba d0 32 60 00          mov    $0x6032d0,%edx //edx=0x6032d0

.L3:
  401188:   48 89 54 74 20          mov    %rdx,0x20(%rsp,%rsi,2) //*(rsp+32+rsi*2)=rdx
  40118d:   48 83 c6 04             add    $0x4,%rsi
  401191:   48 83 fe 18             cmp    $0x18,%rsi
  401195:   74 14                   je     .L4, 4011ab <phase_6+0xb7> //jump to 4011ab
  
.L1:
  401197:   8b 0c 34                mov    (%rsp,%rsi,1),%ecx //ecx=x1
  40119a:   83 f9 01                cmp    $0x1,%ecx //ecx:0x1
  40119d:   7e e4                   jle    .L5, 401183 <phase_6+0x8f> //if <=, then to L5
  40119f:   b8 01 00 00 00          mov    $0x1,%eax //else, eax=1
  4011a4:   ba d0 32 60 00          mov    $0x6032d0,%edx //edx = 0x6032d0
  4011a9:   eb cb                   jmp    .L2, 401176 <phase_6+0x82>
  


.L4:
  4011ab:   48 8b 5c 24 20          mov    0x20(%rsp),%rbx //rbx=*(rsp+0x20)
  4011b0:   48 8d 44 24 28          lea    0x28(%rsp),%rax //rax=rsp+0x28
  4011b5:   48 8d 74 24 50          lea    0x50(%rsp),%rsi //rsi=rsp+0x50
  4011ba:   48 89 d9                mov    %rbx,%rcx       //rcx=rbx
  
  4011bd:   48 8b 10                mov    (%rax),%rdx     //rdx=*rax
  4011c0:   48 89 51 08             mov    %rdx,0x8(%rcx)  //*(rcx+0x8)=rdx
  4011c4:   48 83 c0 08             add    $0x8,%rax       //rax+=0x8
  4011c8:   48 39 f0                cmp    %rsi,%rax       //rax:rsi
  4011cb:   74 05                   je     4011d2 <phase_6+0xde> 
  4011cd:   48 89 d1                mov    %rdx,%rcx
  4011d0:   eb eb                   jmp    4011bd <phase_6+0xc9>
  


  4011d2:   48 c7 42 08 00 00 00    movq   $0x0,0x8(%rdx) //*(rdx+0x8)=0, set null pointer
  4011d9:   00 
  4011da:   bd 05 00 00 00          mov    $0x5,%ebp      //ebp=0x5
  4011df:   48 8b 43 08             mov    0x8(%rbx),%rax //rax=*(rbx+0x8)
  4011e3:   8b 00                   mov    (%rax),%eax    //eax=*(rax)
  4011e5:   39 03                   cmp    %eax,(%rbx)    //*(rbx):eax
  4011e7:   7d 05                   jge    4011ee <phase_6+0xfa>
  4011e9:   e8 4c 02 00 00          callq  40143a <explode_bomb>
  4011ee:   48 8b 5b 08             mov    0x8(%rbx),%rbx //rbx=*(rbx+0x8)
  4011f2:   83 ed 01                sub    $0x1,%ebp      //ebp-=1
  4011f5:   75 e8                   jne    4011df <phase_6+0xeb> //if ebp!=0, jump 4011df
  


  4011f7:   48 83 c4 50             add    $0x50,%rsp
  4011fb:   5b                      pop    %rbx
  4011fc:   5d                      pop    %rbp
  4011fd:   41 5c                   pop    %r12
  4011ff:   41 5d                   pop    %r13
  401201:   41 5e                   pop    %r14
  401203:   c3                      retq   