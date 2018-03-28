/*
	Zhiyuan Zhang
	ID:1500012772
        Here is my implement and some explantion.
*/
/*
Explantion:

1.About my thinking and stragety:
  Dealing with 32*32, I note that a block consists of 8 integers, so I
divide the matrix in 8*8 size block, and it works.
  Dealing with 64*64, 8*8 size block will cause a lot of conflicts, so I
tried 4*4 size block then, but it fails. Then after trying and trying, I
make my best to generate the first version: dealing with every 8*8 size 
block specailly, and divide each in 4 parts, which seems like black art,
and nearly makes it, but fails again. Then I find it I can use the memory
in B[][] which won't cause conflict! Then I finally make it in the second 
version.
  Dealing with the last task, I try to use the stragety used in 32*32 and
64*64 version, but it has a great effect, and then I try times and times 
again, and make it, though the complement seems strange.

2.About the code style
  Now that even 8-element small array is forbidden, I have to write some
code repeatly, and I coed them in a line, though ugly, but if I don't do 
so, the code will be too long to read.
*/

/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 

#include <stdio.h>
#include "cachelab.h"
#include "contracts.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i0,i,j,k,t0,t1,t2,t3,t4,t5,t6,t7;
    
    REQUIRES(M > 0);
    REQUIRES(N > 0);

    if (M==32 && N==32) {
    //Now that, B=32=8*sizeof(int), S=32, N=32, M=32=4*8, so divide the matrix into small 8*8 size blocks
      for (i0=0;i0<N;i0+=8)
        for (j=0;j<M;j+=8)
          for (i=i0;i<i0+8;i++) {
            t0=A[i+0][j+0]; t1=A[i+0][j+1]; t2=A[i+0][j+2]; t3=A[i+0][j+3]; t4=A[i+0][j+4]; t5=A[i+0][j+5]; t6=A[i+0][j+6]; t7=A[i+0][j+7];
            B[j+0][i+0]=t0; B[j+1][i+0]=t1; B[j+2][i+0]=t2; B[j+3][i+0]=t3; B[j+4][i+0]=t4; B[j+5][i+0]=t5; B[j+6][i+0]=t6; B[j+7][i+0]=t7;
	  }
    } else if (M==64 && N==64) {
    //Divide the matrix into 8*8 size blocks, and divide each blocks into 4*4 small blocks specially
    //This is my first version, I deal with them in a paticular order
    //which seems like black art, isn't it?  :-) But sadly, it can only reaches 6.5 :-(
    /*for (i0=0;i0<N;i0+=8)
        for (k=0;k<M;k+=8){
          for (i=i0,j=k;i<i0+4;i+=2) {
            t0=A[i+0][j+0]; t1=A[i+0][j+1]; t2=A[i+0][j+2]; t3=A[i+0][j+3]; t4=A[i+1][j+0]; t5=A[i+1][j+1]; t6=A[i+1][j+2]; t7=A[i+1][j+3];
            B[j+0][i+0]=t0; B[j+1][i+0]=t1; B[j+2][i+0]=t2; B[j+3][i+0]=t3; B[j+0][i+1]=t4; B[j+1][i+1]=t5; B[j+2][i+1]=t6; B[j+3][i+1]=t7;
          }   
          for (i=i0,j=k+4;i<i0+4;i+=2) {
            t0=A[i+0][j+0]; t1=A[i+0][j+1]; t2=A[i+0][j+2]; t3=A[i+0][j+3]; t4=A[i+1][j+0]; t5=A[i+1][j+1]; t6=A[i+1][j+2]; t7=A[i+1][j+3];
            B[j+0][i+0]=t0; B[j+1][i+0]=t1; B[j+2][i+0]=t2; B[j+3][i+0]=t3; B[j+0][i+1]=t4; B[j+1][i+1]=t5; B[j+2][i+1]=t6; B[j+3][i+1]=t7;
          } 
          for (i=i0+4,j=k+4;i<i0+8;i+=2) {
            t0=A[i+0][j+0]; t1=A[i+0][j+1]; t2=A[i+0][j+2]; t3=A[i+0][j+3]; t4=A[i+1][j+0]; t5=A[i+1][j+1]; t6=A[i+1][j+2]; t7=A[i+1][j+3];
            B[j+0][i+0]=t0; B[j+1][i+0]=t1; B[j+2][i+0]=t2; B[j+3][i+0]=t3; B[j+0][i+1]=t4; B[j+1][i+1]=t5; B[j+2][i+1]=t6; B[j+3][i+1]=t7;
          }
          for (i=i0+4,j=k;i<i0+8;i+=2) {
            t0=A[i+0][j+0]; t1=A[i+0][j+1]; t2=A[i+0][j+2]; t3=A[i+0][j+3]; t4=A[i+1][j+0]; t5=A[i+1][j+1]; t6=A[i+1][j+2]; t7=A[i+1][j+3];
            B[j+0][i+0]=t0; B[j+1][i+0]=t1; B[j+2][i+0]=t2; B[j+3][i+0]=t3; B[j+0][i+1]=t4; B[j+1][i+1]=t5; B[j+2][i+1]=t6; B[j+3][i+1]=t7;
          } 
       }*/
    //This is my second version, modified from the first version
    //Instead of write buffer immediately in the correct position in B[][], we store it in somewhere other in B[][] as buffer to substitute the stragety in the first version
      for (i=0;i<N;i+=8)
	for (j=0;j<M;j+=8) {
          for (k=j;k<j+4;k++) {
	    t0=A[k][i+0]; t1=A[k][i+1]; t2=A[k][i+2]; t3=A[k][i+3]; t4=A[k][i+4]; t5=A[k][i+5]; t6=A[k][i+6]; t7=A[k][i+7];
	    B[i+0][k]=t0; B[i][k+4]=t4; B[i+1][k]=t1;B[i+1][k+4]=t5;B[i+2][k]=t2;B[i+2][k+4]=t6;B[i+3][k]=t3;B[i+3][k+4]=t7;								
          }
	  for (k=i;k<i+4;k++) {
	    t0=B[k][j+4]; t1=B[k][j+5]; t2=B[k][j+6]; t3=B[k][j+7]; t4=A[j+4][k]; t5=A[j+5][k]; t6=A[j+6][k]; t7=A[j+7][k];
	    B[k][j+4]=t4; B[k][j+5]=t5; B[k][j+6]=t6; B[k][j+7]=t7; B[k+4][j]=t0;B[k+4][j+1]=t1;B[k+4][j+2]=t2;B[k+4][j+3]=t3;
          }
	  for (k=i+4;k<i+8;k++) {
	    t0=A[j+4][k]; t1=A[j+5][k]; t2=A[j+6][k]; t3=A[j+7][k];
	    B[k][j+4]=t0; B[k][j+5]=t1; B[k][j+6]=t2; B[k][j+7]=t3;
          }
        }
    } else if (M==61 && N==67) {
    //Devide the matrix into small 16*8 size blocks, and deal with the remaining elements specially, though it may seems strange but it really works
      for (i0=0;i0<N;i0+=16)
        for (j=0;j<M;j+=8)
          for (i=i0;i<i0+16 && i<N;i++) {
            if (j==56) {
              t0 = A[i][j+0];t1 = A[i][j+1];t2 = A[i][j+2];t3 = A[i][j+3];t4 = A[i][j+4];t5 = A[i][j+5];
              B[j+0][i] = t0;B[j+1][i] = t1;B[j+2][i] = t2;B[j+3][i] = t3;B[j+4][i] = t4;B[j+5][i] = t5;
            } else
            {
              t0 = A[i][j+0];t1 = A[i][j+1];t2 = A[i][j+2];t3 = A[i][j+3];t4 = A[i][j+4];t5 = A[i][j+5];t6 = A[i][j+6];t7 = A[i][j+7];
              B[j+0][i] = t0;B[j+1][i] = t1;B[j+2][i] = t2;B[j+3][i] = t3;B[j+4][i] = t4;B[j+5][i] = t5;B[j+6][i] = t6;B[j+7][i] = t7;
            }
	  }
    }

    ENSURES(is_transpose(M, N, A, B));

    return;
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    REQUIRES(M > 0);
    REQUIRES(N > 0);

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

    ENSURES(is_transpose(M, N, A, B));

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

