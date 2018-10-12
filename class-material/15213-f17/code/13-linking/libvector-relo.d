
libvector.so:     file format elf64-x86-64
libvector.so
architecture: i386:x86-64, flags 0x00000150:
HAS_SYMS, DYNAMIC, D_PAGED
start address 0x00000000000005f0

Program Header:
    LOAD off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**21
         filesz 0x00000000000007f4 memsz 0x00000000000007f4 flags r-x
    LOAD off    0x0000000000000e20 vaddr 0x0000000000200e20 paddr 0x0000000000200e20 align 2**21
         filesz 0x0000000000000200 memsz 0x0000000000000210 flags rw-
 DYNAMIC off    0x0000000000000e38 vaddr 0x0000000000200e38 paddr 0x0000000000200e38 align 2**3
         filesz 0x0000000000000190 memsz 0x0000000000000190 flags rw-
    NOTE off    0x00000000000001c8 vaddr 0x00000000000001c8 paddr 0x00000000000001c8 align 2**2
         filesz 0x0000000000000024 memsz 0x0000000000000024 flags r--
EH_FRAME off    0x000000000000075c vaddr 0x000000000000075c paddr 0x000000000000075c align 2**2
         filesz 0x0000000000000024 memsz 0x0000000000000024 flags r--
   STACK off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**4
         filesz 0x0000000000000000 memsz 0x0000000000000000 flags rw-
   RELRO off    0x0000000000000e20 vaddr 0x0000000000200e20 paddr 0x0000000000200e20 align 2**0
         filesz 0x00000000000001e0 memsz 0x00000000000001e0 flags r--

Dynamic Section:
  NEEDED               libc.so.6
  INIT                 0x00000000000005b0
  FINI                 0x0000000000000750
  INIT_ARRAY           0x0000000000200e20
  INIT_ARRAYSZ         0x0000000000000008
  FINI_ARRAY           0x0000000000200e28
  FINI_ARRAYSZ         0x0000000000000008
  GNU_HASH             0x00000000000001f0
  STRTAB               0x00000000000003b8
  SYMTAB               0x0000000000000238
  STRSZ                0x00000000000000c1
  SYMENT               0x0000000000000018
  PLTGOT               0x0000000000201000
  RELA                 0x00000000000004c0
  RELASZ               0x00000000000000f0
  RELAENT              0x0000000000000018
  VERNEED              0x00000000000004a0
  VERNEEDNUM           0x0000000000000001
  VERSYM               0x000000000000047a
  RELACOUNT            0x0000000000000003

Version References:
  required from libc.so.6:
    0x09691a75 0x00 02 GLIBC_2.2.5

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .note.gnu.build-id 00000024  00000000000001c8  00000000000001c8  000001c8  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  1 .gnu.hash     00000048  00000000000001f0  00000000000001f0  000001f0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  2 .dynsym       00000180  0000000000000238  0000000000000238  00000238  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  3 .dynstr       000000c1  00000000000003b8  00000000000003b8  000003b8  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .gnu.version  00000020  000000000000047a  000000000000047a  0000047a  2**1
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  5 .gnu.version_r 00000020  00000000000004a0  00000000000004a0  000004a0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  6 .rela.dyn     000000f0  00000000000004c0  00000000000004c0  000004c0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  7 .init         0000001a  00000000000005b0  00000000000005b0  000005b0  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  8 .plt          00000010  00000000000005d0  00000000000005d0  000005d0  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  9 .plt.got      00000010  00000000000005e0  00000000000005e0  000005e0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 10 .text         0000015f  00000000000005f0  00000000000005f0  000005f0  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 11 .fini         00000009  0000000000000750  0000000000000750  00000750  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 12 .eh_frame_hdr 00000024  000000000000075c  000000000000075c  0000075c  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 13 .eh_frame     00000074  0000000000000780  0000000000000780  00000780  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 14 .init_array   00000008  0000000000200e20  0000000000200e20  00000e20  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 15 .fini_array   00000008  0000000000200e28  0000000000200e28  00000e28  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 16 .jcr          00000008  0000000000200e30  0000000000200e30  00000e30  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 17 .dynamic      00000190  0000000000200e38  0000000000200e38  00000e38  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 18 .got          00000038  0000000000200fc8  0000000000200fc8  00000fc8  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 19 .got.plt      00000018  0000000000201000  0000000000201000  00001000  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 20 .data         00000008  0000000000201018  0000000000201018  00001018  2**3
                  CONTENTS, ALLOC, LOAD, DATA
 21 .bss          00000010  0000000000201020  0000000000201020  00001020  2**2
                  ALLOC
 22 .comment      00000035  0000000000000000  0000000000000000  00001020  2**0
                  CONTENTS, READONLY
SYMBOL TABLE:
00000000000001c8 l    d  .note.gnu.build-id	0000000000000000              .note.gnu.build-id
00000000000001f0 l    d  .gnu.hash	0000000000000000              .gnu.hash
0000000000000238 l    d  .dynsym	0000000000000000              .dynsym
00000000000003b8 l    d  .dynstr	0000000000000000              .dynstr
000000000000047a l    d  .gnu.version	0000000000000000              .gnu.version
00000000000004a0 l    d  .gnu.version_r	0000000000000000              .gnu.version_r
00000000000004c0 l    d  .rela.dyn	0000000000000000              .rela.dyn
00000000000005b0 l    d  .init	0000000000000000              .init
00000000000005d0 l    d  .plt	0000000000000000              .plt
00000000000005e0 l    d  .plt.got	0000000000000000              .plt.got
00000000000005f0 l    d  .text	0000000000000000              .text
0000000000000750 l    d  .fini	0000000000000000              .fini
000000000000075c l    d  .eh_frame_hdr	0000000000000000              .eh_frame_hdr
0000000000000780 l    d  .eh_frame	0000000000000000              .eh_frame
0000000000200e20 l    d  .init_array	0000000000000000              .init_array
0000000000200e28 l    d  .fini_array	0000000000000000              .fini_array
0000000000200e30 l    d  .jcr	0000000000000000              .jcr
0000000000200e38 l    d  .dynamic	0000000000000000              .dynamic
0000000000200fc8 l    d  .got	0000000000000000              .got
0000000000201000 l    d  .got.plt	0000000000000000              .got.plt
0000000000201018 l    d  .data	0000000000000000              .data
0000000000201020 l    d  .bss	0000000000000000              .bss
0000000000000000 l    d  .comment	0000000000000000              .comment
0000000000000000 l    df *ABS*	0000000000000000              crtstuff.c
0000000000200e30 l     O .jcr	0000000000000000              __JCR_LIST__
00000000000005f0 l     F .text	0000000000000000              deregister_tm_clones
0000000000000630 l     F .text	0000000000000000              register_tm_clones
0000000000000680 l     F .text	0000000000000000              __do_global_dtors_aux
0000000000201020 l     O .bss	0000000000000001              completed.7594
0000000000200e28 l     O .fini_array	0000000000000000              __do_global_dtors_aux_fini_array_entry
00000000000006c0 l     F .text	0000000000000000              frame_dummy
0000000000200e20 l     O .init_array	0000000000000000              __frame_dummy_init_array_entry
0000000000000000 l    df *ABS*	0000000000000000              addvec.c
0000000000000000 l    df *ABS*	0000000000000000              multvec.c
0000000000000000 l    df *ABS*	0000000000000000              crtstuff.c
00000000000007f0 l     O .eh_frame	0000000000000000              __FRAME_END__
0000000000200e30 l     O .jcr	0000000000000000              __JCR_END__
0000000000000000 l    df *ABS*	0000000000000000              
0000000000201018 l     O .data	0000000000000000              __dso_handle
0000000000200e38 l     O .dynamic	0000000000000000              _DYNAMIC
000000000000075c l       .eh_frame_hdr	0000000000000000              __GNU_EH_FRAME_HDR
0000000000201020 l     O .data	0000000000000000              __TMC_END__
0000000000201000 l     O .got.plt	0000000000000000              _GLOBAL_OFFSET_TABLE_
0000000000000000  w      *UND*	0000000000000000              _ITM_deregisterTMCloneTable
0000000000201028 g     O .bss	0000000000000004              multcnt
0000000000201020 g       .data	0000000000000000              _edata
0000000000000750 g     F .fini	0000000000000000              _fini
000000000000071f g     F .text	0000000000000030              multvec
00000000000006f0 g     F .text	000000000000002f              addvec
0000000000000000  w      *UND*	0000000000000000              __gmon_start__
0000000000201030 g       .bss	0000000000000000              _end
0000000000201020 g       .bss	0000000000000000              __bss_start
0000000000201024 g     O .bss	0000000000000004              addcnt
0000000000000000  w      *UND*	0000000000000000              _Jv_RegisterClasses
0000000000000000  w      *UND*	0000000000000000              _ITM_registerTMCloneTable
0000000000000000  w    F *UND*	0000000000000000              __cxa_finalize@@GLIBC_2.2.5
00000000000005b0 g     F .init	0000000000000000              _init



Disassembly of section .init:

00000000000005b0 <_init>:
 5b0:	48 83 ec 08          	sub    $0x8,%rsp
 5b4:	48 8b 05 1d 0a 20 00 	mov    0x200a1d(%rip),%rax        # 200fd8 <_DYNAMIC+0x1a0>
 5bb:	48 85 c0             	test   %rax,%rax
 5be:	74 05                	je     5c5 <_init+0x15>
 5c0:	e8 1b 00 00 00       	callq  5e0 <_init+0x30>
 5c5:	48 83 c4 08          	add    $0x8,%rsp
 5c9:	c3                   	retq   

Disassembly of section .plt:

00000000000005d0 <.plt>:
 5d0:	ff 35 32 0a 20 00    	pushq  0x200a32(%rip)        # 201008 <_GLOBAL_OFFSET_TABLE_+0x8>
 5d6:	ff 25 34 0a 20 00    	jmpq   *0x200a34(%rip)        # 201010 <_GLOBAL_OFFSET_TABLE_+0x10>
 5dc:	0f 1f 40 00          	nopl   0x0(%rax)

Disassembly of section .plt.got:

00000000000005e0 <.plt.got>:
 5e0:	ff 25 f2 09 20 00    	jmpq   *0x2009f2(%rip)        # 200fd8 <_DYNAMIC+0x1a0>
 5e6:	66 90                	xchg   %ax,%ax
 5e8:	ff 25 0a 0a 20 00    	jmpq   *0x200a0a(%rip)        # 200ff8 <_DYNAMIC+0x1c0>
 5ee:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

00000000000005f0 <deregister_tm_clones>:
 5f0:	48 8d 3d 29 0a 20 00 	lea    0x200a29(%rip),%rdi        # 201020 <_edata>
 5f7:	48 8d 05 29 0a 20 00 	lea    0x200a29(%rip),%rax        # 201027 <addcnt+0x3>
 5fe:	55                   	push   %rbp
 5ff:	48 29 f8             	sub    %rdi,%rax
 602:	48 89 e5             	mov    %rsp,%rbp
 605:	48 83 f8 0e          	cmp    $0xe,%rax
 609:	76 15                	jbe    620 <deregister_tm_clones+0x30>
 60b:	48 8b 05 b6 09 20 00 	mov    0x2009b6(%rip),%rax        # 200fc8 <_DYNAMIC+0x190>
 612:	48 85 c0             	test   %rax,%rax
 615:	74 09                	je     620 <deregister_tm_clones+0x30>
 617:	5d                   	pop    %rbp
 618:	ff e0                	jmpq   *%rax
 61a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
 620:	5d                   	pop    %rbp
 621:	c3                   	retq   
 622:	0f 1f 40 00          	nopl   0x0(%rax)
 626:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 62d:	00 00 00 

0000000000000630 <register_tm_clones>:
 630:	48 8d 3d e9 09 20 00 	lea    0x2009e9(%rip),%rdi        # 201020 <_edata>
 637:	48 8d 35 e2 09 20 00 	lea    0x2009e2(%rip),%rsi        # 201020 <_edata>
 63e:	55                   	push   %rbp
 63f:	48 29 fe             	sub    %rdi,%rsi
 642:	48 89 e5             	mov    %rsp,%rbp
 645:	48 c1 fe 03          	sar    $0x3,%rsi
 649:	48 89 f0             	mov    %rsi,%rax
 64c:	48 c1 e8 3f          	shr    $0x3f,%rax
 650:	48 01 c6             	add    %rax,%rsi
 653:	48 d1 fe             	sar    %rsi
 656:	74 18                	je     670 <register_tm_clones+0x40>
 658:	48 8b 05 91 09 20 00 	mov    0x200991(%rip),%rax        # 200ff0 <_DYNAMIC+0x1b8>
 65f:	48 85 c0             	test   %rax,%rax
 662:	74 0c                	je     670 <register_tm_clones+0x40>
 664:	5d                   	pop    %rbp
 665:	ff e0                	jmpq   *%rax
 667:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
 66e:	00 00 
 670:	5d                   	pop    %rbp
 671:	c3                   	retq   
 672:	0f 1f 40 00          	nopl   0x0(%rax)
 676:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 67d:	00 00 00 

0000000000000680 <__do_global_dtors_aux>:
 680:	80 3d 99 09 20 00 00 	cmpb   $0x0,0x200999(%rip)        # 201020 <_edata>
 687:	75 27                	jne    6b0 <__do_global_dtors_aux+0x30>
 689:	48 83 3d 67 09 20 00 	cmpq   $0x0,0x200967(%rip)        # 200ff8 <_DYNAMIC+0x1c0>
 690:	00 
 691:	55                   	push   %rbp
 692:	48 89 e5             	mov    %rsp,%rbp
 695:	74 0c                	je     6a3 <__do_global_dtors_aux+0x23>
 697:	48 8b 3d 7a 09 20 00 	mov    0x20097a(%rip),%rdi        # 201018 <__dso_handle>
 69e:	e8 45 ff ff ff       	callq  5e8 <_init+0x38>
 6a3:	e8 48 ff ff ff       	callq  5f0 <deregister_tm_clones>
 6a8:	5d                   	pop    %rbp
 6a9:	c6 05 70 09 20 00 01 	movb   $0x1,0x200970(%rip)        # 201020 <_edata>
 6b0:	f3 c3                	repz retq 
 6b2:	0f 1f 40 00          	nopl   0x0(%rax)
 6b6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 6bd:	00 00 00 

00000000000006c0 <frame_dummy>:
 6c0:	48 8d 3d 69 07 20 00 	lea    0x200769(%rip),%rdi        # 200e30 <__JCR_END__>
 6c7:	48 83 3f 00          	cmpq   $0x0,(%rdi)
 6cb:	75 0b                	jne    6d8 <frame_dummy+0x18>
 6cd:	e9 5e ff ff ff       	jmpq   630 <register_tm_clones>
 6d2:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
 6d8:	48 8b 05 09 09 20 00 	mov    0x200909(%rip),%rax        # 200fe8 <_DYNAMIC+0x1b0>
 6df:	48 85 c0             	test   %rax,%rax
 6e2:	74 e9                	je     6cd <frame_dummy+0xd>
 6e4:	55                   	push   %rbp
 6e5:	48 89 e5             	mov    %rsp,%rbp
 6e8:	ff d0                	callq  *%rax
 6ea:	5d                   	pop    %rbp
 6eb:	e9 40 ff ff ff       	jmpq   630 <register_tm_clones>

00000000000006f0 <addvec>:
 6f0:	4c 8b 05 e9 08 20 00 	mov    0x2008e9(%rip),%r8        # 200fe0 <_DYNAMIC+0x1a8>
 6f7:	41 8b 00             	mov    (%r8),%eax
 6fa:	83 c0 01             	add    $0x1,%eax
 6fd:	41 89 00             	mov    %eax,(%r8)
 700:	b8 00 00 00 00       	mov    $0x0,%eax
 705:	eb 12                	jmp    719 <addvec+0x29>
 707:	4c 63 c0             	movslq %eax,%r8
 70a:	46 8b 0c 86          	mov    (%rsi,%r8,4),%r9d
 70e:	46 03 0c 87          	add    (%rdi,%r8,4),%r9d
 712:	46 89 0c 82          	mov    %r9d,(%rdx,%r8,4)
 716:	83 c0 01             	add    $0x1,%eax
 719:	39 c8                	cmp    %ecx,%eax
 71b:	7c ea                	jl     707 <addvec+0x17>
 71d:	f3 c3                	repz retq 

000000000000071f <multvec>:
 71f:	4c 8b 05 aa 08 20 00 	mov    0x2008aa(%rip),%r8        # 200fd0 <_DYNAMIC+0x198>
 726:	41 8b 00             	mov    (%r8),%eax
 729:	83 c0 01             	add    $0x1,%eax
 72c:	41 89 00             	mov    %eax,(%r8)
 72f:	b8 00 00 00 00       	mov    $0x0,%eax
 734:	eb 13                	jmp    749 <multvec+0x2a>
 736:	4c 63 c0             	movslq %eax,%r8
 739:	46 8b 0c 86          	mov    (%rsi,%r8,4),%r9d
 73d:	46 0f af 0c 87       	imul   (%rdi,%r8,4),%r9d
 742:	46 89 0c 82          	mov    %r9d,(%rdx,%r8,4)
 746:	83 c0 01             	add    $0x1,%eax
 749:	39 c8                	cmp    %ecx,%eax
 74b:	7c e9                	jl     736 <multvec+0x17>
 74d:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000000750 <_fini>:
 750:	48 83 ec 08          	sub    $0x8,%rsp
 754:	48 83 c4 08          	add    $0x8,%rsp
 758:	c3                   	retq   
