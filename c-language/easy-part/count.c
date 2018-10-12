#include <stdio.h>

#define IN 1
#define OUT 0

int main()
{
    double nc, lc, bc, wc;
    int c, state;
    nc = lc = bc = wc = 0;
    state = OUT;
    while ((c = getchar()) != EOF){
        ++nc;
        if (c == '\n')
            ++lc;
        if (c == ' ' || c == '\t' || c == '\n'){
            ++bc;
            state = OUT;
        }else if (state == OUT){
            ++wc;
            state = IN;
        }
    }
    bc -= lc;
    printf("EOF: 0x%X\n",c);
    printf("line count: %6.0f\nword count: %6.0f\n"
            "char count: %6.0f\nblank count:%6.0f\n",
             lc, wc, nc, bc);
    return 0;
}
