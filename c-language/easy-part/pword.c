#include <stdio.h>

#define IN 1
#define OUT 0
int main()
{
    int c, state;
    state = OUT;
    while ((c = getchar()) != EOF){
        if (c != '\n' && c != ' ' && c != '\t'){
            state = IN;
            putchar(c);
        }else if (state == IN){
            state = OUT;
            putchar('\n');
        }
    }
    return 0;
}
