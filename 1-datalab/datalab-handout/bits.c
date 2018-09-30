/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * xiezhiwen
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  // DeMorgan's laws 
  return ~(~x | ~y);
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  return (x>>(n<<3))&0xFF;

}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  /* 
   * use property of &
   * and expresion of opposite number in two's complement 
   */
  return (x>>n)&~(~0<<(31+(~n+1))<<1);
  // return (unsigned)x >> n;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {

  // int mask1 = 0x55555555; // 0b01...
  // int mask2 = 0x33333333; // 0b0011...
  // int mask3 = 0x0F0F0F0F; // 0b00001111...
  // int mask4 = 0x00FF00FF; // 0b0000000011111111...
  // int mask5 = 0x0000FFFF; // 0b00000000000000001111111111111111...
  // to fit the requirement of the problem, generate mask by shift
  int mask1 = 0x55;
  int mask2 = 0x33;
  int mask3 = 0x0F;
  int mask4 = 0xFF;
  int mask5 = 0xFF;
  // get 0x55555555
  mask1 = mask1|(mask1<<8);
  mask1 = mask1|(mask1<<16);
  // get 0x33333333
  mask2 = mask2|(mask2<<8);
  mask2 = mask2|(mask2<<16);
  // get 0x0F0F0F0F
  mask3 = mask3|(mask3<<8);
  mask3 = mask3|(mask3<<16);
  // get 0x00FF00FF
  mask4 = mask4|(mask4<<16);
  // get 0x0000FFFF
  mask5 = mask5|(mask5<<8);

  x = (x&mask1) + ((x>>1)&mask1); // 2/(2^1-1)
  x = (x&mask2) + ((x>>2)&mask2); // 4/(2^2-1)
  // x = (x&mask3) + ((x>>4)&mask3); // 8/(2^4-1)
  // x = (x&mask4) + ((x>>8)&mask4); // 16/(2^8-1)
  // x = (x&mask5) + ((x>>16)&mask5); // 32/(2^16-1)
  // optimize to use less operators
  x = (x+(x>>4))&mask3; // 8/(2^4-1)
  x = (x+(x>>8))&mask4; // 16/(2^8-1)
  x = (x+(x>>16))&mask5; // 32/(2^16-1)
  return x;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  return (((~x+1)|x)>>31) + 1;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1<<31;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
/*
 * first convert positive number to negative number minus 1
 * and keep negative number unchanged
 * then shift right 32-n and shift back to check
 * whether or not the shifted and the original are the same
 */
  int sign = x>>31;
  int op = ~sign;
  int shift = 33 + (~n);
  x = x^op;
  return !(x^(x<<shift>>shift));
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
  /*
   * first round down, then if it's negative and
   * last n bits are not zero, add 1 to the result.
   */
    int sign = x>>31;
    int lastn = x&~(~0<<n);
    return (x>>n) + (0x01&sign&!!lastn);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x+1;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
  // check it's non-negative and the opposite number is negotive 
  // return (!(x>>31))&(!(((~x+1)>>31)+1));

  // check it's non-negative, then check it's not zero
  return (!(x>>31))&(!!x);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
/* there are two cases, one is when the sign of y is positive
 * and sign of x is negative, y>x sure enough.
 * another one is when sign of y-x is positive, we need to exclude
 * the overflow case where y is negative and x is positive, and y-x overflow
 * to become positive.
 */
  int signx = x>>31;
  int signy = y>>31;
  // -1 means y negative, x positive
  // 0 means same sign
  // 1 means y positive, x negative
  int signd = signy+~signx+1;
  // while exclude the case where y is negative and x is positive,
  // 0 means y >= x, -1 means y < x
  int diff = (y+~x+1)>>31;
  return (!((signd+1)^0x02))|(!diff&!(signd>>1));
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
  int n = 0;
  // if(x>>16 == 0) {n += 16; x <<= 16;}
  // if(x>>24 == 0) {n += 8; x <<= 8;}
  // if(x>>28 == 0) {n += 4; x <<= 4;}
  // if(x>>30 == 0) {n += 2; x <<= 2;}
  // if(x>>31 == 0) {n += 1; x <<= 1;}
  int mask1,mask2,mask3,mask4,mask5;
  mask1 = (~(!(x>>16)+(~0)))&16;
  n += mask1; x <<= mask1;
  mask2 = (~(!(x>>24)+(~0)))&8;
  n += mask2; x <<= mask2;
  mask3 = (~(!(x>>28)+(~0)))&4;
  n += mask3; x <<= mask3;
  mask4 = (~(!(x>>30)+(~0)))&2;
  n += mask4; x <<= mask4;
  mask5 = (~(!(x>>31)+(~0)))&1;
  n += mask5;
  return 32+(~n);
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
 /* if the frac part is all-zero or
  * exponential part is not all-ones,
  * it won't be NaN, so turn its sign.
  */
  // ignore sign
  unsigned v = uf<<1;
  int a = !(v<<8);
  int b = !!(v>>24^0xFF);
  return uf^((a|b)<<31);
 }
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
/*
 * first change the negative number to the positive counter part,
 * notice 0x80000000 is 2147483648 if we see it as unsigned
 * and -2147483648 when interpreted as integer, so the only special
 * case is only 0, exclude that first.
 * then get the length of the integer expression, calculate the exponential
 * part, based on that, we get frac part in two different cases, one is length
 * less than 24, so no need for rounding, we get the result directly, another is
 * when rounding may be needed, round up when round part is bigger than half, or 
 * when round part is equal to half and the last part of frac is 1(odd), this is 
 * round to even method.
 */
  unsigned sign;
  unsigned tmp;
  unsigned len;
  unsigned frac;
  unsigned expo;
  unsigned rounding;
  unsigned round_part;
  unsigned shift;
  unsigned mid;
  if (!x){
    return x;
  } else if (x>0) {
    sign = 0;
  } else {
    sign = 1;
    x = -x;
  }
  tmp = x;len = 0;
  while (tmp) {++len; tmp>>=1;}
  // n = 0;
  // if (!(tmp>>32)) {n+=32;tmp<<=32;}
  // if (!(tmp>>48)) {n+=16;tmp<<=16;}
  // if (!(tmp>>56))  {n+=8;tmp<<=8;}
  // if (!(tmp>>60))  {n+=4;tmp<<=4;}
  // if (!(tmp>>62))  {n+=2;tmp<<=2;}
  // if (!(tmp>>63))  {n+=1;}
  // len = 64-n;
  expo = 126 + len;
  rounding = 0;
  shift = 24-len;
  if (len <= 24) {
    frac = x<<shift;
  } else {
    shift = -shift;
    frac = x>>shift;
    round_part = x&~(~0<<shift);
    mid = 1<<(shift-1);
    if (round_part > mid || 
      (round_part == mid && (frac&1))){
      rounding = 1;
    }
  }
  frac = frac&0x7fffff;
  return (sign<<31|expo<<23|frac) + rounding;

}
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  /* if uf is infinity or NaN, return directly,
 * non-normalized is surprisingly easy case to 
 * deal with, just shift frac left 1 bit, it will
 * work either less than 0.5 or bigger than that,
 * and deal with overflow case while expo is 0xFF-1,
 * at last, it's the most common case where you just add
 * exponential part and finish.
 */
  unsigned expo = uf<<1>>24;
  unsigned mask = ~0<<23;
  unsigned frac = uf&~mask;
  if (expo == 0xFF){
    return uf;
  } else if (expo == 0xFE){
    return (uf|(0xFF<<23))&mask;
  } else if (!expo){
    // non-normalized
    frac <<= 1;
    return (uf&mask)|frac;
  }  else {
    // normalized case, most common
    expo += 1;
    return (uf&~(0xFF<<23))|(expo<<23);
  }
}


















