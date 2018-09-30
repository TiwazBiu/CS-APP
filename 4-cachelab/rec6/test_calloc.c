#include <stdlib.h>
#include <stdio.h>
int main()
{
    int *a = calloc(213, sizeof(int));
    int *a = malloc(213 * sizeof(int));
    if (a == NULL) return 0;
    for (int i=0; i<213; ++i){
        printf("%d ",a[i]);
        getchar();

    }
    printf("\n");
    
    free(a);
    return 0;
}
	
