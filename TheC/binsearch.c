#include <stdio.h>


int binsearch(int x, int v[], int n);
int binsearch2(int x, int v[], int n);
void shellsort(int v[], int n);
void showArray(int m[], int len);

int main(int argc, char const *argv[])
{
    int v[] = {1,2,3,4,5,6,7,8,9};
    int m[] = {2,4,5,7,1,-1,-4,9,1,0};
    int len = 10;
    int find = binsearch2(5,v,1);
    printf("find:%d\n", find);
    showArray(m, len);
    shellsort(m, len);
    return 0;
}
void swap(int *p, int *q)
{
    int tmp;
    tmp = *p;
    *p = *q;
    *q = tmp;
}
void showArray(int m[], int len)
{
    for (int i = 0; i < len; ++i)
        printf("%5d%s", m[i], (i == len -1)? "\n":"\t");   
}

void shellsort(int v[], int n)
{
    for (int gap = n/2; gap > 0; gap /= 2)
        for (int i = gap; i < n; ++i)
            for (int j = i-gap; j>=0 && v[j]>v[j+gap]; j -= gap){
                swap(v+j, v+j+gap);
                showArray(v, n);
            }
}
int binsearch(int x, int v[], int n)
{
    int low, high, mid;

    low = 0;
    high = n-1;
    while (low <= high){
        mid = (low+high)/2;
        if (x < v[mid])
            high = mid-1;
        else if (x > v[mid])
            low = mid+1;
        else
            return mid;
    }
    return -1;
}


// 没啥用的改进（2 test to 1 test in loop)
int binsearch2(int x, int v[], int n)
{
    int low, high, mid;

    low = 0;
    high = n-1;
    while (low <= high && x < v[high]){
        mid = (low+high)/2;
        if (x < v[mid])
            high = mid-1;
        else
            low = mid;
    }
    if (v[high] == x)
        return high;
    return -1;
}

