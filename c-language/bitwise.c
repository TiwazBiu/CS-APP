#include <stdio.h>

unsigned getbits(unsigned x, int p, int n);
unsigned setbits(unsigned x, int p, int n, unsigned y);
unsigned invert(unsigned x, int p, int n);
unsigned rightrot(unsigned x, int n);
int bitcount(unsigned x);

int main(int argc, char const *argv[])
{
    unsigned n;
    unsigned t;
    unsigned y;
    int count;

    n = 0b000100011000000010011111;
    t = n&0xff;
    printf("0x%08x\n", n);
    printf("0x%08x\n", t);
    t = n&~0xff;
    printf("0x%08x\n", t);
    
    y = getbits(0x8099, 3, 4);
    printf("0x%08x\n", y);
    y = setbits(0x8034, 7, 4, 0x7);
    printf("0x%08x\n", y);
    y = invert(0x8034, 15,8);
    printf("0x%08x\n", y);
    y = rightrot(0x8034, 8);
    printf("0x%08x\n", y);

    unsigned m = 0x8034;
    count = bitcount(m);
    printf("the 1 bit in %X is %d\n", m, count);
    return 0;

}

int bitcount(unsigned x)
{
    int n;

    n = 0;
    while(x != 0){
        x &= x-1;
        ++n;
    }

    return n;
}
unsigned getbits(unsigned x, int p, int n)
{
   return (x>>(p-n+1)) & ~(~0 << n);
}

unsigned setbits(unsigned x, int p, int n, unsigned y)
{
    int shift = p-n+1;
    x &= ~(~(~0 << n) << shift);
    x |= y<<shift;
    return x;
}


unsigned invert(unsigned x, int p, int n)
{
    x ^= ~(~0 << n) << (p-n+1);
    return x;
}


unsigned rightrot(unsigned x, int n)
{
    int bits = sizeof(unsigned)*8;
    unsigned q = getbits(x, n-1, n);
    x >>= n;
    x = setbits(x, bits-1, n, q);
    return x;
}
