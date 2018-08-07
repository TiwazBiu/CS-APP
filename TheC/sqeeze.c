#include <stdio.h>

void squeeze(char s[], char const c);
int contain(char const c[], char const t);
void squeeze2(char s[], char const c[]);
int any(char const s[], char const c[]);
void _strcat(char *s, char *t);
int main(int argc, char const *argv[])
{
    char *line = NULL;
    size_t n = 0;
    while (getline(&line, &n, stdin) > 0){
        squeeze2(line, "abc");
        printf("%s\n", line);
        printf("any x or y in line:%d\n", any(line, "xy"));
        _strcat(line, "End with me");
        printf("%s\n", line);
    }
    return 0;
}

void _strcat(char *s, char *t)
{
    int i,j;

    i = j = 0;
    while (s[i] != '\0')
        ++i;
    while ((s[i++] = t[j++]) != '\0')
        ;
}



void squeeze(char s[], char const c)
{
    int i,j;

    for (i = j = 0; s[i] != '\0'; ++i)
        if (s[i] != c)
            s[j++] = s[i];
    s[j] = '\0';
}


void squeeze2(char s[], char const c[])
{
    int i,j;

    for (i = j = 0; s[i] != '\0'; ++i)
        if (!contain(c, s[i]))
            s[j++] = s[i];
    s[j] = '\0';
}

int any(char const s[], char const c[])
{
    for (int i = 0; s[i] != '\0'; ++i)
        if (contain(c, s[i]))
            return i;
    return -1; 
}


int contain(char const c[], char const t)
{
    for (int i = 0; c[i] != '\0'; ++i)
        if (c[i] == t)
            return 1;
    return 0;
}