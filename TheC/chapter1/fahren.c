#include <stdio.h>
/* print fahrenheit-celsius table
 * for fahr = 0, 20, ..., 300
 */

int main()
{
    int fahr;
    const int lower = 0, upper = 300, step = 20;
    for(fahr=lower;fahr<=upper;fahr+=step){
        printf("%3d %6.1f\n", fahr, (5.0/9.0) * (fahr-32));
    }
    return 0;
}


