#include <stdio.h>
#include <ctype.h>
long atoi(char s[]);

int main(int argc, char const *argv[])
{
    char *line = NULL;
    size_t bytes = 0;
    ssize_t n;
    while ((n = getline(&line, &bytes, stdin)) > 0){
        printf("line:%s", line);
        printf("line size: %ld\n", n);
        printf("int:%ld\n", atoi(line));
    }
    return 0;
}


long atoi(char s[])
{
    long n = 0;
    for (int i = 0; isdigit(s[i]); ++i)
        n = 10*n + s[i] - '0';
    return n;
}