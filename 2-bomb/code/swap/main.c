#include <stdio.h>

void swap(long*, long*);

int main(int argc, char const *argv[])
{
    long a,b;
    scanf("%ld %ld", &a, &b);
    printf("before swap, a: %ld, b: %ld\n", a, b);
    swap(&a, &b);
    printf("after swap, a: %ld, b: %ld\n", a, b);
    return 0;
}