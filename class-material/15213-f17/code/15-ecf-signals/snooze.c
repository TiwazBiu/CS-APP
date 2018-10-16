#include "csapp.h"

unsigned int snooze(unsigned int secs);
void sigint_handler(int sig);

int main(int argc, char **argv)
{
    if (argc != 2){
        fprintf(stderr, "usage: %s <secs>\n", argv[0]);
        exit(-1);
    }
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
        unix_error("signal error");
    snooze(atoi(argv[1]));
    exit(0);
}

unsigned int snooze(unsigned int secs)
{
    unsigned int left;
    left = sleep(secs);
    printf("Slept for %u of %u secs.\n", secs-left, secs);
    return left;
}

void sigint_handler(int sig)
{
    printf("\n");
}

