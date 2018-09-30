
instructions.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:	bf fa 97 b9 59       	mov    $0x59b997fa,%edi
   5:	68 ec 17 40 00       	pushq  $0x4017ec
   a:	c3                   	retq   

00000000004019c3 <setval_426>:
  4019c3:   c7 07 48 89 c7 90       movl   $0x90c78948,(%rdi)
  4019c9:   c3                      retq   

00000000004019ca <getval_280>:
  4019ca:   b8 29 58 90 c3          mov    $0xc3905829,%eax
  4019cf:   c3                      retq   

r->s->r
s:move 0x59b997fa to %rdi, can use pop instruction
pop %rax 58 90 c3
mov %rax, %rdi 48 89 c7 90 c3
ret


stack 1: 4019ca+2 4019cc
      2: 59b997fa 59b997fa
      3: 4019c3+2 4019c5


