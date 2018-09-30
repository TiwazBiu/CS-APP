
prog:     file format elf64-x86-64


Disassembly of section .init:

0000000000400448 <_init>:
  400448:	48 83 ec 08          	sub    $0x8,%rsp
  40044c:	48 8b 05 a5 0b 20 00 	mov    0x200ba5(%rip),%rax        # 600ff8 <_DYNAMIC+0x1d0>
  400453:	48 85 c0             	test   %rax,%rax
  400456:	74 05                	je     40045d <_init+0x15>
  400458:	e8 53 00 00 00       	callq  4004b0 <__printf_chk@plt+0x10>
  40045d:	48 83 c4 08          	add    $0x8,%rsp
  400461:	c3                   	retq   

Disassembly of section .plt:

0000000000400470 <__stack_chk_fail@plt-0x10>:
  400470:	ff 35 92 0b 20 00    	pushq  0x200b92(%rip)        # 601008 <_GLOBAL_OFFSET_TABLE_+0x8>
  400476:	ff 25 94 0b 20 00    	jmpq   *0x200b94(%rip)        # 601010 <_GLOBAL_OFFSET_TABLE_+0x10>
  40047c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400480 <__stack_chk_fail@plt>:
  400480:	ff 25 92 0b 20 00    	jmpq   *0x200b92(%rip)        # 601018 <_GLOBAL_OFFSET_TABLE_+0x18>
  400486:	68 00 00 00 00       	pushq  $0x0
  40048b:	e9 e0 ff ff ff       	jmpq   400470 <_init+0x28>

0000000000400490 <__libc_start_main@plt>:
  400490:	ff 25 8a 0b 20 00    	jmpq   *0x200b8a(%rip)        # 601020 <_GLOBAL_OFFSET_TABLE_+0x20>
  400496:	68 01 00 00 00       	pushq  $0x1
  40049b:	e9 d0 ff ff ff       	jmpq   400470 <_init+0x28>

00000000004004a0 <__printf_chk@plt>:
  4004a0:	ff 25 82 0b 20 00    	jmpq   *0x200b82(%rip)        # 601028 <_GLOBAL_OFFSET_TABLE_+0x28>
  4004a6:	68 02 00 00 00       	pushq  $0x2
  4004ab:	e9 c0 ff ff ff       	jmpq   400470 <_init+0x28>

Disassembly of section .plt.got:

00000000004004b0 <.plt.got>:
  4004b0:	ff 25 42 0b 20 00    	jmpq   *0x200b42(%rip)        # 600ff8 <_DYNAMIC+0x1d0>
  4004b6:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

00000000004004c0 <_start>:
  4004c0:	31 ed                	xor    %ebp,%ebp
  4004c2:	49 89 d1             	mov    %rdx,%r9
  4004c5:	5e                   	pop    %rsi
  4004c6:	48 89 e2             	mov    %rsp,%rdx
  4004c9:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  4004cd:	50                   	push   %rax
  4004ce:	54                   	push   %rsp
  4004cf:	49 c7 c0 a0 06 40 00 	mov    $0x4006a0,%r8
  4004d6:	48 c7 c1 30 06 40 00 	mov    $0x400630,%rcx
  4004dd:	48 c7 c7 b6 05 40 00 	mov    $0x4005b6,%rdi
  4004e4:	e8 a7 ff ff ff       	callq  400490 <__libc_start_main@plt>
  4004e9:	f4                   	hlt    
  4004ea:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000004004f0 <deregister_tm_clones>:
  4004f0:	b8 47 10 60 00       	mov    $0x601047,%eax
  4004f5:	55                   	push   %rbp
  4004f6:	48 2d 40 10 60 00    	sub    $0x601040,%rax
  4004fc:	48 83 f8 0e          	cmp    $0xe,%rax
  400500:	48 89 e5             	mov    %rsp,%rbp
  400503:	76 1b                	jbe    400520 <deregister_tm_clones+0x30>
  400505:	b8 00 00 00 00       	mov    $0x0,%eax
  40050a:	48 85 c0             	test   %rax,%rax
  40050d:	74 11                	je     400520 <deregister_tm_clones+0x30>
  40050f:	5d                   	pop    %rbp
  400510:	bf 40 10 60 00       	mov    $0x601040,%edi
  400515:	ff e0                	jmpq   *%rax
  400517:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
  40051e:	00 00 
  400520:	5d                   	pop    %rbp
  400521:	c3                   	retq   
  400522:	0f 1f 40 00          	nopl   0x0(%rax)
  400526:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  40052d:	00 00 00 

0000000000400530 <register_tm_clones>:
  400530:	be 40 10 60 00       	mov    $0x601040,%esi
  400535:	55                   	push   %rbp
  400536:	48 81 ee 40 10 60 00 	sub    $0x601040,%rsi
  40053d:	48 c1 fe 03          	sar    $0x3,%rsi
  400541:	48 89 e5             	mov    %rsp,%rbp
  400544:	48 89 f0             	mov    %rsi,%rax
  400547:	48 c1 e8 3f          	shr    $0x3f,%rax
  40054b:	48 01 c6             	add    %rax,%rsi
  40054e:	48 d1 fe             	sar    %rsi
  400551:	74 15                	je     400568 <register_tm_clones+0x38>
  400553:	b8 00 00 00 00       	mov    $0x0,%eax
  400558:	48 85 c0             	test   %rax,%rax
  40055b:	74 0b                	je     400568 <register_tm_clones+0x38>
  40055d:	5d                   	pop    %rbp
  40055e:	bf 40 10 60 00       	mov    $0x601040,%edi
  400563:	ff e0                	jmpq   *%rax
  400565:	0f 1f 00             	nopl   (%rax)
  400568:	5d                   	pop    %rbp
  400569:	c3                   	retq   
  40056a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000400570 <__do_global_dtors_aux>:
  400570:	80 3d c9 0a 20 00 00 	cmpb   $0x0,0x200ac9(%rip)        # 601040 <__TMC_END__>
  400577:	75 11                	jne    40058a <__do_global_dtors_aux+0x1a>
  400579:	55                   	push   %rbp
  40057a:	48 89 e5             	mov    %rsp,%rbp
  40057d:	e8 6e ff ff ff       	callq  4004f0 <deregister_tm_clones>
  400582:	5d                   	pop    %rbp
  400583:	c6 05 b6 0a 20 00 01 	movb   $0x1,0x200ab6(%rip)        # 601040 <__TMC_END__>
  40058a:	f3 c3                	repz retq 
  40058c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400590 <frame_dummy>:
  400590:	bf 20 0e 60 00       	mov    $0x600e20,%edi
  400595:	48 83 3f 00          	cmpq   $0x0,(%rdi)
  400599:	75 05                	jne    4005a0 <frame_dummy+0x10>
  40059b:	eb 93                	jmp    400530 <register_tm_clones>
  40059d:	0f 1f 00             	nopl   (%rax)
  4005a0:	b8 00 00 00 00       	mov    $0x0,%eax
  4005a5:	48 85 c0             	test   %rax,%rax
  4005a8:	74 f1                	je     40059b <frame_dummy+0xb>
  4005aa:	55                   	push   %rbp
  4005ab:	48 89 e5             	mov    %rsp,%rbp
  4005ae:	ff d0                	callq  *%rax
  4005b0:	5d                   	pop    %rbp
  4005b1:	e9 7a ff ff ff       	jmpq   400530 <register_tm_clones>

00000000004005b6 <main>:
  4005b6:	48 83 ec 18          	sub    $0x18,%rsp
  4005ba:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  4005c1:	00 00 
  4005c3:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
  4005c8:	31 c0                	xor    %eax,%eax
  4005ca:	48 89 e2             	mov    %rsp,%rdx
  4005cd:	be 03 00 00 00       	mov    $0x3,%esi
  4005d2:	bf 02 00 00 00       	mov    $0x2,%edi
  4005d7:	e8 3f 00 00 00       	callq  40061b <multstore>
  4005dc:	48 8b 14 24          	mov    (%rsp),%rdx
  4005e0:	be b4 06 40 00       	mov    $0x4006b4,%esi
  4005e5:	bf 01 00 00 00       	mov    $0x1,%edi
  4005ea:	b8 00 00 00 00       	mov    $0x0,%eax
  4005ef:	e8 ac fe ff ff       	callq  4004a0 <__printf_chk@plt>
  4005f4:	48 8b 4c 24 08       	mov    0x8(%rsp),%rcx
  4005f9:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
  400600:	00 00 
  400602:	74 05                	je     400609 <main+0x53>
  400604:	e8 77 fe ff ff       	callq  400480 <__stack_chk_fail@plt>
  400609:	b8 00 00 00 00       	mov    $0x0,%eax
  40060e:	48 83 c4 18          	add    $0x18,%rsp
  400612:	c3                   	retq   

0000000000400613 <mult2>:
  400613:	48 89 f8             	mov    %rdi,%rax
  400616:	48 0f af c6          	imul   %rsi,%rax
  40061a:	c3                   	retq   

000000000040061b <multstore>:
  40061b:	53                   	push   %rbx
  40061c:	48 89 d3             	mov    %rdx,%rbx
  40061f:	e8 ef ff ff ff       	callq  400613 <mult2>
  400624:	48 89 03             	mov    %rax,(%rbx)
  400627:	5b                   	pop    %rbx
  400628:	c3                   	retq   
  400629:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000400630 <__libc_csu_init>:
  400630:	41 57                	push   %r15
  400632:	41 56                	push   %r14
  400634:	41 89 ff             	mov    %edi,%r15d
  400637:	41 55                	push   %r13
  400639:	41 54                	push   %r12
  40063b:	4c 8d 25 ce 07 20 00 	lea    0x2007ce(%rip),%r12        # 600e10 <__frame_dummy_init_array_entry>
  400642:	55                   	push   %rbp
  400643:	48 8d 2d ce 07 20 00 	lea    0x2007ce(%rip),%rbp        # 600e18 <__init_array_end>
  40064a:	53                   	push   %rbx
  40064b:	49 89 f6             	mov    %rsi,%r14
  40064e:	49 89 d5             	mov    %rdx,%r13
  400651:	4c 29 e5             	sub    %r12,%rbp
  400654:	48 83 ec 08          	sub    $0x8,%rsp
  400658:	48 c1 fd 03          	sar    $0x3,%rbp
  40065c:	e8 e7 fd ff ff       	callq  400448 <_init>
  400661:	48 85 ed             	test   %rbp,%rbp
  400664:	74 20                	je     400686 <__libc_csu_init+0x56>
  400666:	31 db                	xor    %ebx,%ebx
  400668:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  40066f:	00 
  400670:	4c 89 ea             	mov    %r13,%rdx
  400673:	4c 89 f6             	mov    %r14,%rsi
  400676:	44 89 ff             	mov    %r15d,%edi
  400679:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  40067d:	48 83 c3 01          	add    $0x1,%rbx
  400681:	48 39 eb             	cmp    %rbp,%rbx
  400684:	75 ea                	jne    400670 <__libc_csu_init+0x40>
  400686:	48 83 c4 08          	add    $0x8,%rsp
  40068a:	5b                   	pop    %rbx
  40068b:	5d                   	pop    %rbp
  40068c:	41 5c                	pop    %r12
  40068e:	41 5d                	pop    %r13
  400690:	41 5e                	pop    %r14
  400692:	41 5f                	pop    %r15
  400694:	c3                   	retq   
  400695:	90                   	nop
  400696:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  40069d:	00 00 00 

00000000004006a0 <__libc_csu_fini>:
  4006a0:	f3 c3                	repz retq 

Disassembly of section .fini:

00000000004006a4 <_fini>:
  4006a4:	48 83 ec 08          	sub    $0x8,%rsp
  4006a8:	48 83 c4 08          	add    $0x8,%rsp
  4006ac:	c3                   	retq   
