#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef MAXCAP
#define MAXCAP 1000
#endif

#ifndef NORMAL
#define NORMAL 0
#endif

typedef struct Stack
{
    int top;
    unsigned int capacity;
    int *array;
} Stack;

Stack *initStack(unsigned int capacity);
void destoryStack(Stack *stack);
void showStackContent(Stack *stack);

int isFull(Stack *stack);
int isEmpty(Stack *stack);
void push(Stack *stack, int item);
int pop(Stack *stack);

int chartype(int c);
int isquote(int c);

// rudimentary syntax check
int main(int argc, char const *argv[])
{
    Stack *stack = NULL;
    int c;
    int legal;
    int quoteEncounter;
    int ignoreNext;
    int commentPhase;
    stack = initStack(MAXCAP);
    legal = 1;
    quoteEncounter = 0;
    ignoreNext = 0;
    commentPhase = 0;
    while ((c = getchar()) != EOF && legal){
        int tmp;
        int type;

        if (commentPhase == 1){
            if (c == '/'){
                printf("comment: ");
                while((c = getchar()) != '\n')
                    putchar(c);
                putchar('\n');
                commentPhase = 0;
                continue;
            } else {
                commentPhase = 0;
            }
        }

        if (ignoreNext){
            ignoreNext = 0;
            continue;
        } else if (quoteEncounter && !isquote(c) && c != '\\'){
            printf("ignored %c\n", c);
            continue;
        }
        
        type = chartype(c);
        switch(type){
            case NORMAL:
                break;
            case '/':
                commentPhase = 1;
                break;
            case '\\':
                ignoreNext = 1;
                break;
            case '\"':
                if (!quoteEncounter){
                    quoteEncounter = 1;
                    push(stack, '\"');
                } else if ((tmp=pop(stack)) != '\"'){
                    printf("poped %c, but need \"\n", tmp);
                    legal = 0;
                } else {
                    quoteEncounter = 0;
                }
                break;

            case '\'':
                if (!quoteEncounter){
                    quoteEncounter = 1;
                    push(stack, '\'');
                } else if ((tmp=pop(stack)) != '\''){
                    printf("poped %c, but need \'\n", tmp);
                    legal = 0;
                } else {
                    quoteEncounter = 0;
                }
                break;
            case '{':
                push(stack, '{');
                break;
            
            case '}':
                printf("get }, ");
                if ((tmp=pop(stack)) != '{'){
                    printf("poped %c, but need {\n", tmp);
                    legal = 0;
                }
                break;
            
            case '(':
                push(stack, '(');
                break;
            case ')':
                printf("get ), ");
                if ((tmp=pop(stack)) != '('){
                    printf("poped %c, but need (\n", tmp);
                    legal = 0;
                }
                break;
            
            case '[':
                push(stack, '[');
                break;
            case ']':
                printf("get ], ");
                if ((tmp=pop(stack)) != '['){
                    printf("poped %c, but need [\n", tmp);
                    legal = 0;
                }
                break;
            default:
                break;
        }
    }
    if (legal && isEmpty(stack)){
        printf("pass check\n");
    }
    else{
        printf("Not legal\n");
        showStackContent(stack); 
    }
    destoryStack(stack);
    return 0;
}

Stack *initStack(unsigned int capacity)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->top = -1;
    stack->capacity = capacity;
    stack->array = (int *)malloc(sizeof(int)*stack->capacity);
    return stack;
}
void destoryStack(Stack *stack)
{
    free(stack->array);
    free(stack);
}

int isEmpty(Stack *stack)
{
    return stack->top == -1;
}

int isFull(Stack *stack)
{
    return stack->top == stack->capacity - 1;
}


void push(Stack *stack, int item)
{
    if (!isFull(stack)){
        stack->array[++stack->top] = item;
        printf("pushed in %c\n", (char)stack->array[stack->top]);
    } else {
        printf("Stack is Full\n");
    }
}

int pop(Stack *stack)
{
    if(!isEmpty(stack)){
        int c = stack->array[stack->top--];
        printf("poped out %c\n", (char)c);
        return c;
    } else {
        printf("Stack is Empty\n");
        return -1;
    }
}

void showStackContent(Stack *stack)
{
    printf("stack remain:\n");
    if(!isEmpty(stack)){
        for (int i = 0; i <= stack->top; ++i)
            printf("%c",(char)stack->array[i]);
        printf("\n");
    }
}
int chartype(int c)
{
    if (isalnum(c) || isblank(c))
        return NORMAL;
    else
        return c;
}

int isquote(int c)
{
    if (c != '\'' && c != '\"')
        return 0;
    else
        return 1;
}








