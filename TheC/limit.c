#include <limits.h>
#include <stdio.h>
typedef enum boolean
{
    False, 
    True
} boolean;

int main(int argc, char const *argv[])
{
    boolean i = True;
    printf("%d\n", i);
    i = !i;
    printf("%d\n", i);
    printf("signed:\n");
    printf("%d\t%d\n", CHAR_MIN, CHAR_MAX);
    printf("%d\t%d\n", -(1<<7), (1<<7) -1);
    
    printf("%d\t%d\n", SHRT_MIN, SHRT_MAX);
    printf("%d\t%d\n", -(1<<15), (1<<15) -1);
    
    printf("%d\t%d\n", INT_MIN, INT_MAX);
    printf("%d\t%d\n", -(1<<31), (1<<31) - 1);
    
    printf("%ld\t%ld\n", LONG_MIN, LONG_MAX);
    printf("%ld\t%ld\n", -(long)1<<63, ((long)1<<63) - 1);

    printf("unsigned:\n");
    printf("%u\n", UCHAR_MAX);
    printf("%u\n", (1<<8) - 1);
    
    printf("%u\n", USHRT_MAX);
    printf("%u\n", (1<<16) - 1);

    printf("%u\n", UINT_MAX);
    printf("%lu\n", ((unsigned long)1<<32) - 1);

    printf("%lu\n", ULONG_MAX);
    printf("%lu\n", ((unsigned long)1<<64) - 1);
    return 0;
}