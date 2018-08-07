#include <stdio.h>

int power(int base, int n);
//
int main(int argc, char const *argv[])
{
    for(int i = 0; i < argc; ++i)
        printf("%s\n", argv[i]);

    for (int i = 0; i < 10; ++i){
        printf("%d %d %d\n", i, power(2,i), power(-2,i));
    }
    return 0;
}

int power(int base, int n)
{
    if (n == 0)
        return 1;
    else if (n == 1)
       return base;
    else
       return base*power(base, n-1);
}

