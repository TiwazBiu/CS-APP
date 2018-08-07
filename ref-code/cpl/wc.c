#include <stdio.h>

#define IN 1
#define OUT 0

main()
{
	int c, nc, nw, state;
	
	state = OUT;
	nc = nw = 0;
	while( (c = getchar()) != EOF){
		++nc;
		if(c != ' ' && c != '\n' && c != '\t'){
			if(state == OUT)
				++nw;
			state = IN;
			putchar(c);
		}else if(state == IN){
			state = OUT;
			putchar('\n');
		}
	}
	printf("\nchar count: %d\tword count: %d\n", nc, nw);
}	
