#include <stdio.h>
#include <string.h>

int strindex(char const s[], char const t[]);
int rightindex(char const s[], char const t[]);

int main(int argc, char const *argv[])
{
    char s[] = "world hello really hello and bye!!!!";
    char t[] = "hello";
    printf("%s in left position: %d\n", t, strindex(s,t));
    printf("%s in right position: %d\n", t, rightindex(s,t));

    return 0;
}


int strindex(char const s[], char const t[])
{
    int j,k;
    for (int i = 0; s[i] != '\0'; ++i){
        for (j = 0, k = i; t[j] != '\0' && s[k] == t[j]; ++k,++j)
            ;
        if (t[j] == '\0' && j > 0)
            return i;
    }
    return -1;
}


int rightindex(char const s[], char const t[])
{
    int i,j,k;

    for (i = strlen(s)-strlen(t); i > 0; --i){
        for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; ++j,++k)
            ;
        if (t[k] == '\0' && k > 0)
            return i;
    }

    return -1;
}