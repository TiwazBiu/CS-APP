#include <stdio.h>

#ifndef TABSTOP
#define TABSTOP 4
#endif

void exptab(int tabstop);


int main(int argc, char const *argv[])
{
	int c;
	while ((c = getchar()) != EOF){
		if (c == '\t')
			exptab(TABSTOP);
		else
			putchar(c);
	}
	putchar('\n');
	return 0;
}

void exptab(int tabstop)
{
	for (int i = 0; i < tabstop; ++i)
		putchar(' ');
}