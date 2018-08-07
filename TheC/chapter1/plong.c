#include <stdio.h>

#ifndef MAXLINE
#define MAXLINE 1000
#endif

int _getline(char line[], int lim);
void copy(char *to, char const *from);
void swap(char *p, char *q);
void reverse(char *s);


int main(int argc, char const *argv[])
{
    char line[MAXLINE], longest[MAXLINE];
    int max,len;

    max = 0;
    while ((len = _getline(line, MAXLINE)) > 0){
        if (len > max){
            copy(longest, line);
            max = len;
        }
        if (len > 80)
            printf("%s\n", line);
    }   
    printf("max len:%d\nbegin-\n%s\n-end\n", max, longest); 
    reverse(longest);
    printf("reversed:\nbegin-\n%s\n-end\n", longest);
    return 0;
}

int _getline(char line[], int lim)
{
    int c, i;
    
    i = 0;
    while (--lim && (c=getchar()) != EOF){
        line[i++] = (char)c;
        if (c == '\n')
            break;
    }
    line[i] = '\0';
    return i;
}

void copy(char *to, char const *from)
{
    for(int i = 0; (to[i]=from[i]) != '\0'; ++i);
}

void reverse(char *s)
{
    int len;
    for(len = 0; s[len] != '\0'; ++len);
    len -= 1;
    for(int i = 0; i <= len/2; ++i){
        swap(s+i, s+len-i);
    }
}


void swap(char *p, char *q)
{
    char tmp;
    tmp = *p;
    *p = *q;
    *q = tmp;
}











