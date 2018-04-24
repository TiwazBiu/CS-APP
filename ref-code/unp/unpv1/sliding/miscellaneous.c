#include "miscellaneous.h"


// 以probability的概率返回IS_CORRUPTED
// 用来模拟信道出错
bool random_corrupt(double probability)
{
    return rand() < probability * ((double)RAND_MAX + 1);
}