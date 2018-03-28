#include <stdint.h>
#include <inttypes.h> 
#include <stdio.h>
int tmult_ok(int x, int y){
	// int64_t s = (int64_t)(x)*(int64_t)(y);
	// printf("x = %d, y = %d, s = %" PRId64 "(0x%016lx)\n", x,y,s,s);
	// int p = s;
	// int pw = (p>=0)?0:1;
	// int t = (s>>32) + pw;
	// printf("v + p_w-1: 0x%08x\n", t);
	// printf("Not overflow(1:not overflow,0: overflow): %d\n", !t);
	// printf("\n\n");
	// return !t;
	int64_t s = (int64_t) x*y;
	return s == (int) s;
	
}

int div16(int x){
	// return (x<0? (x+(1<<4)-1):x)>>4;
	return (x + ((x>>31)&0x0F))>>4;

}
int main(int argc, char const *argv[])
{
	freopen("input", "r", stdin);
	freopen("output", "w", stdout);

	int x,y;
	while(scanf("%d %d",&x,&y) == 2){
		// tmult_ok(x,y);
		int z = div16(x);
		printf("%d/16 = %d\n", x,z);
	}

	float f = 1.0e20;
	double d = 1.0;
	printf("f+d = %f\n",f+d);
	printf("(f+d)-f = %f\n",(f+d)-f);
	fclose(stdin);
	fclose(stdout);
	return 0;
}