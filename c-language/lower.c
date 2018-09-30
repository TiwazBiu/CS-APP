#include <stdio.h>
#include <ctype.h>
c
void lower(char s[]);
int main(int argc, char const *argv[])
{
    char s[] = "HELLO world";
    lower(s);
    printf("%s\n", s);
    return 0;
}


void lower(char s[])
{
    for (int i = 0; s[i] != '\0'; ++i)
        if(isupper(s[i]))
            s[i] += 'a'-'A';
}