#include <stdbool.h>
#include "unp.h"

#define DGLEN     2000
#define DF         struct DataFrame
#define AF        struct AckFrame
#define IS_CORRUPTED  1
#define NOT_CORRUPTED 0

struct DataFrame
{
    unsigned int seqnum;
    bool flag;
    char data[DGLEN];
};

struct AckFrame
{
    unsigned int seqnum;
    bool flag;
    int recvbytes;
};

bool random_corrupt(double probability);