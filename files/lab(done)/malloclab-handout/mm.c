/*
 *
 * Zhang Zhiyuan
 * ID:1500012772
 *
 * My implement and explanation:
 *
 *   1.I use list 8,16,24,32,40,48,56,64 etc to store small blocks,
 *     listX store blocks with size of X bytes.
 *   2.I use large_list to store blocks larger than small lis, and 
 *     in the large_list, blocks are sorted for small to large.
 *   3.As for other less significant complement, just similar as 
 *     CS:APP textbook or mm-naive.c. I rewrote it to fit my style.
 *
 * Some details of block data structure:
 *
 *   1.8-byte-block: 
 *     HEAD & load (allocated), 
 *     HEAD & NEXT_NODE(next block in link list) (unallocated).
 *     As you can see, 8-byte-block is very special.
 *
 *   2.16,24,32,40,48,56,64 etc byte-block: 
 *     HEAD & load (allocated),
 *     HEAD & NEXT_NODE(next block in link, same as 8-byte block) & 
 *     PREV_NODE(prev block in link) & FOOT (unallocated)
 *
 *   3.other block (same as small byte block):
 *     HEAD & load (allocated),
 *     HEAD & NEXT_NODE(next) & PREV_NODE(prev) & FOOT (unallocated)
 *
 *   HEAD = SIZE | PREV_8_FREE | PREV_ALLOC | ALLOC
 *
 *   As CS:APP textbook suggests, we have PREV_ALLOC and ALLOC 
 *   to judge whether this and prev block is free.
 *
 *   As you can see, since free 8-byte blocks don't have FOOT, 
 *   we should know wether prev block is PREV_8_FREE.
 *
 * Some details of functions:
 *   
 *   1.I implement calloc() and realloc() in the style of mm-naive.c.
 *   2.I implement mm_init() and coalesce() in the style of CS:APP text book.
 *   3.I implement extend_heap(), I try to make use of last block if free.
 *   4.I implement malloc() and free() with helper find_fit() and place().
 *   5.I implement find_fit(), insert_node() and delete_node() to deal with 
 *     date structure small list or large list, according to the size.
 *   6.I implement find_list(), insert_list() and delete_list() for 5..
 *   7.I implement mm_check() for debug mode (when define DEBUG).
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/* If you want debugging output, use the following macro.  When you hand
 * in, remove the #define DEBUG line. */
#define DEBUG
#ifdef DEBUG
# define dbg_printf(...) printf(__VA_ARGS__)
#else
# define dbg_printf(...)
#endif


/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#endif /* def DRIVER */


/* word, 4 bytes */
#define WSIZE 4
#define DSIZE 8

/* header size */
#define HEADSIZE 4

/* alignment request, 8 bytes */
#define ALIGNMENT 8

/* extend heap by chucksize */
#define CHUNKSIZE (1<<8)

/* covert between a 64-bit address and 32-bit */
#define ADDR_MASK 0x800000000

/* 
 * max and min value of 2 values
 * since max or MAX is used by lib
 * so I have to use ugly Max :-(
 */
#define Max(x, y) ((x)>(y) ? (x) : (y))
#define Min(x, y) ((x)<(y) ? (x) : (y))

/* make the block to meet with the standard alignment requirements */
#define ALIGN_SIZE(size) (((size) + (ALIGNMENT-1)) & ((-1)<<3))

/* pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size)|(alloc))

/* read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p)=(val))

/* read the information from address p */
#define SIZE(p) (GET(p) & ((-1)<<3))
#define ALLOC(p) (GET(p) & (1<<0))
#define PREV_ALLOC(p) (GET(p) & (1<<1))
#define PREV_8_FREE(p) (GET(p) & (1<<2))

/* read the information of bp */
#define GET_SIZE(bp) ((GET(HEAD(bp))) & ((-1)<<3))
#define GET_HEAD(bp) (GET(HEAD((bp))))
#define GET_FOOT(bp) (GET(FOOT((bp))))
#define GET_ALLOC(bp) ((GET(HEAD(bp))) & (1<<0))
#define GET_PREV_ALLOC(bp) ((GET(HEAD(bp))) & (1<<1))
#define GET_PREV_8_FREE(bp) ((GET(HEAD(bp))) & (1<<2))

#define SET_PREV_ALLOC(bp) (GET(HEAD(bp)) |= (1<<1))
#define SET_PREV_NOT_ALLOC(bp) (GET(HEAD(bp)) &= ~(1<<1))
#define SET_PREV_8_FREE(bp) (GET(HEAD(bp)) |= (1<<2))
#define SET_PREV_NOT_8_FREE(bp) (GET(HEAD(bp)) &= ~(1<<2))

/* pointers of address p */
#define HEAD(p) ((void *)(p) - WSIZE)
#define NEXT_NODE(p) ((void *)(p))
#define PREV_NODE(p) ((void *)(p) + WSIZE)
#define FOOT(p) ((void *)(p) + SIZE(HEAD(p)) - DSIZE)

/* prev and next block of bp */
#define GET_PREV(bp) (GET_PREV_8_FREE(bp) ? \
((void *)bp - 8) : ((void *)(bp) - SIZE((void *)(bp) - DSIZE)))
#define GET_NEXT(bp) ((void *)(bp) + SIZE(((void *)(bp) - WSIZE)))

/* the address bp points, the address is coverted to 64-bit */
#define GET_NEXT_NODE(bp) ((long)GET(NEXT_NODE(bp))|(ADDR_MASK))
#define GET_PREV_NODE(bp) ((long)GET(PREV_NODE(bp))|(ADDR_MASK))

/* put value to bp */
#define PUT_HEAD(bp, val) (PUT(HEAD(bp), val))
#define PUT_FOOT(bp, val) (PUT(FOOT(bp), val))
#define PUT_NEXT_NODE(bp, val) (PUT(NEXT_NODE(bp), \
((unsigned int)(long)val)))
#define PUT_PREV_NODE(bp, val) (PUT(PREV_NODE(bp), \
((unsigned int)(long)val)))

/* functions required to implement */
int mm_init(void);
void *malloc(size_t size);
void *calloc (size_t nmemb, size_t size);
void free(void *bp);
void *coalesce ( void *bp );

/* helper functions */
void *extend_heap(size_t size);
void place(void *ptr, size_t asize);
void insert_node(void *bp);
void delete_node( void *bp);
void *find_fit(size_t asize);
inline void** L(size_t size);
inline void init_list();

/* functinos deal with data structure */
void *find_list(void* head, size_t asize);
void insert_list(void **ptr_list, void *bp);
void delete_list(void **ptr_list, void *bp);

/* debug functions */
void mm_checkheap(int verbose);
void check_block(void *bp);
void check_list8(void *bp);
void check_list(void *bp);

/* global variables */

/* head block node of all heap blocks */
void *heap_ptr;

/* large_list */
void *large_list;

/* virtual NULL pointer and its const version */
void *HEAP_NULL = (void *) ADDR_MASK;

/* head of the small block list */
/* enum*/
void *list8, *list16, *list24, *list32, *list40,
*list48, *list56, *list64, *list72, *list80,
*list88, *list96, *list104, *list112, *list120,
*list128, *list136, *list144, *list152, *list160,
*list168, *list176, *list184, *list192, *list200,
*list208, *list216, *list224, *list232, *list240,
*list248, *list256, *list264, *list272, *list280,
*list288, *list296, *list304, *list312, *list320,
*list328, *list336, *list344, *list352, *list360,
*list368, *list376, *list384, *list392, *list400,
*list408, *list416, *list424, *list432, *list440,
*list448, *list456, *list464, *list472, *list480,
*list488, *list496, *list504, *list512, *list520,
*list528, *list536, *list544, *list552, *list560,
*list568, *list576, *list584, *list592, *list600,
*list608, *list616, *list624, *list632, *list640,
*list648, *list656, *list664, *list672, *list680,
*list688, *list696, *list704, *list712, *list720,
*list728, *list736, *list744, *list752, *list760,
*list768, *list776, *list784, *list792, *list800
;

/* mm_init() init a heap with prologue and epilogue block */
int mm_init(void)
{
	/* create the initial empty heap, padding for alignment */
  	heap_ptr = mem_sbrk(4 * WSIZE);
	if ((size_t) heap_ptr == (size_t) -1)
		return -1;
  	heap_ptr += 2 * WSIZE;
  	
  	/* prologue header */
  	PUT_HEAD(heap_ptr, PACK(DSIZE, 1));
  	
  	/* prologue footer */
  	PUT_FOOT(heap_ptr, GET_HEAD(heap_ptr));
  	
  	/* epilogue header */
	PUT_HEAD(GET_NEXT(heap_ptr), PACK(0, 3));
	
	/* initialize */
    init_list();
    
    return 0;
}

/* extend_heap() merge the last block and extend heap for NEXT_NODE size */
void *extend_heap(size_t words)
{
    void *bp;
    void *end = mem_heap_hi() + 1 - WSIZE;
	size_t size;
	size = words * WSIZE;
    if (!PREV_ALLOC(end)) /* the last block is free, we can make use of it */
    {
        if (PREV_8_FREE(end))
        	size -= 8;
        else
        	size -= SIZE(end - WSIZE);
        
       	assert(size > 0);
    	size = Max(CHUNKSIZE, size);
    	bp = mem_sbrk(size);
    	if ((size_t) bp == (size_t) -1)
        	return NULL;
    
    	/* initialize free block header/footer and the epilogue header */
    	size_t sign = 0 | GET_PREV_ALLOC(bp) | GET_PREV_8_FREE(bp);
    	PUT_HEAD(bp, PACK(size,sign)); /* free block header */
    	PUT_FOOT(bp, PACK(size,sign)); /* free block footer */
    	PUT_HEAD(GET_NEXT(bp), PACK(0,1)); /* new epilogue header */
    	void *new_bp;
    	new_bp=coalesce(bp);
    	return new_bp;
    }
    else /* the last block is not free, just ask for new size bytes */
    {
   	 	assert(size > 0);
   	 	size = Max(CHUNKSIZE, size);
    	bp = mem_sbrk(size);
    	if ((size_t) bp == (size_t) -1)
    		return NULL;
        	
    	/* initialize free block header/footer and the epilogue header */
    	size_t sign = 0 | GET_PREV_ALLOC(bp) | GET_PREV_8_FREE(bp);
    	PUT_HEAD(bp, PACK(size,sign)); /* free block header */
    	PUT_FOOT(bp, PACK(size,sign)); /* free block footer */
    	PUT_HEAD(GET_NEXT(bp), PACK(0,1)); /* new epilogue header */
  		void *new_bp;
    	new_bp=coalesce(bp);
    	return new_bp;
    }
}

/* malloc(size) malloc an aligned block to a legal request */
void *malloc(size_t size)
{
    size_t asize; /* aligned block size */
    void *bp;
    
    /* ignore illegal requests */
    if (size <= 0)
        return NULL;
    
    /* adjust block size to include 8 and alignment requirements. */
    asize = ALIGN_SIZE(size + HEADSIZE);
    
    /* search the free list for a fit */
    bp = find_fit(asize);
    
    /* extend heap if heap used out */
    if (bp == HEAP_NULL)
    	bp = extend_heap(asize / WSIZE);
    
    /* place in the fit size*/
    place(bp, asize);
    return bp;
}

/* free(bp) free a block if neccessary and coalesce it */
void free(void *bp)
{
	/* no need to free */
    if (bp == HEAP_NULL || bp == NULL || !GET_ALLOC(bp))
    	return;
    	
    /* set new free block and coalesce it */
    size_t sign = 0 | GET_PREV_ALLOC(bp) | GET_PREV_8_FREE(bp);
    PUT_HEAD(bp, PACK(GET_SIZE(bp), sign));
    PUT_FOOT(bp, GET_HEAD(bp));
    coalesce(bp);
    
    return;
}

/*
 * realloc(oldptr,size)
 * change the size of the block by mallocing a new block,
 * copying its data, and freeing the old block,
 * just written in the style of mm-naive.c
 */
void *realloc(void *oldptr, size_t size)
{
	size_t oldsize;
	void *newptr;
	
	/* if size == 0 then this is just free, and we return NULL. */
	if (size == 0)
	{
		free(oldptr);
		return NULL;
	}
  	/* if oldptr is NULL, then this is just malloc. */
  	else if (oldptr == NULL)
    	return malloc(size);
	else
	{
		newptr = malloc(size);
  		/* if realloc() fails the original block is NEXT_NODE untouched  */
  		if (!newptr) 
  	 		return NULL;
  	 			
  		/* copy the old data. */
  		oldsize = GET_SIZE(oldptr);
  		memcpy(newptr, oldptr, Min(size,oldsize));
  	
		/* free the old block. */
		free(oldptr);	
		return newptr;
	}
}

/* 
 * calloc(nmemb, size) 
 * allocate the block and set it to zero, 
 * just written in the style of mm-naive.c
 */
void *calloc (size_t nmemb, size_t size)
{
	size_t bytes = nmemb * size;
	void *newptr;
	newptr = malloc(bytes);
	memset(newptr, 0, bytes);
	return newptr;
}

/*
 * coalesce(bp)
 * just merge a new allocated block (newly allocated block is not 
 * in heap_ptr now, because after free() or heap_extend(), block 
 * is usually just allocted and not in heap_ptr, so we can insert 
 * it after merging), with prev and next block and also delete 
 * merged blocks. After merging if possible, insert the new allocatd
 * node after merging in the heap_ptr. There are 4 cases, and we deal
 * with them one by one, just as the style of CS:APP textbook.
 */
void *coalesce(void *bp)
{	
	void *prev, *next;
	size_t prev_alloc = GET_PREV_ALLOC(bp);
	size_t next_alloc = GET_ALLOC( GET_NEXT(bp) );
	size_t size = GET_SIZE(bp);
	size_t sign;
	size_t case_num = ((!prev_alloc)<<1) | ((!next_alloc)<<0);
	
	switch (case_num) /* 4 cases as in CS:APP text book */
	{
        case 0:
        insert_node(bp);
        return bp;		
        
		case 1:
	    size += GET_SIZE(GET_NEXT(bp));
	    delete_node(GET_NEXT(bp));
	    
		sign = 0 | GET_PREV_ALLOC(bp) | GET_PREV_8_FREE(bp);
		PUT_HEAD(bp, PACK(size,sign));
		PUT_FOOT(bp, PACK(size,sign));
		insert_node(bp);
		return bp;
	
		case 2:
	   	prev = (void *) GET_PREV(bp);
	    
		size += GET_SIZE(prev);
	    delete_node(prev);
	    
		sign = 0 | GET_PREV_ALLOC(prev) | GET_PREV_8_FREE(prev);
	    PUT_HEAD(prev, PACK(size, sign));
		PUT_FOOT(prev, PACK(size, sign));
		insert_node(prev);
		return prev;
	
		case 3:
	    prev = (void *) GET_PREV(bp);
	    next = (void *) GET_NEXT(bp);
	    
	    size += GET_SIZE(prev) + GET_SIZE(next);
		delete_node(prev);
		delete_node(next);
		
		sign = 0 | GET_PREV_ALLOC(prev) | GET_PREV_8_FREE(prev);
        PUT_HEAD(prev, PACK(size,sign));
		PUT_FOOT(prev, PACK(size,sign));
		insert_node(prev);		
		return prev;
		
		default:
		/* correct program will never reach this */
		printf("Correct program will never reach there.\n");
		printf("You are kidding me!\n");
		assert(0 == 1);
	}
}

/*
 * place(bp, aszie) place asize block in place bp,
 * split block bp points if neccessary.
 */
void place(void *bp, size_t asize)
{
	if ((bp == HEAP_NULL) || (bp == NULL))
		return;
	size_t size = GET_SIZE(bp);
	assert(size >= asize);
	delete_node(bp);
	if((size-asize) >= 8)
	/* split this block into two blocks */
	{
	    size_t sign = 1 | GET_PREV_ALLOC(bp) | GET_PREV_8_FREE(bp);
		PUT_HEAD(bp,PACK(asize,sign));

		bp = GET_NEXT(bp);
		PUT_HEAD(bp, PACK(size-asize,2));
		PUT_FOOT(bp, GET_HEAD(bp));
		
		/* after spliting, coalesce new free blocks */
		coalesce(bp);
	}
	/* too small to worth spliting */
	else
	{
	    size_t sign = 1 | GET_PREV_ALLOC(bp) | GET_PREV_8_FREE(bp);
		PUT_HEAD(bp, PACK(size, sign));
	}
	return;
}

/* insert_node() insert node */
void insert_node(void *bp)
{
    SET_PREV_NOT_ALLOC(GET_NEXT(bp));
    size_t bpsize = GET_SIZE(bp);
   	if (bpsize == 8)
   	{
   		SET_PREV_8_FREE(GET_NEXT(bp));
        PUT_NEXT_NODE(bp, list8);
        list8 = bp;
    }
    else
    	insert_list(L(bpsize) ,bp);
   	return;
}

/* delete_node() delete node */
void delete_node(void *bp)
{
    SET_PREV_ALLOC(GET_NEXT(bp));
    size_t bpsize = GET_SIZE(bp);
   	if (bpsize == 8)
   	{
    	/*
    	 * we don't know prev block in list because an
    	 * 8-byte-block is too small to store prev block
    	 * information, so we have to search in the list.
    	 */
        SET_PREV_NOT_8_FREE(GET_NEXT(bp));
        void * temp = list8;
        if (temp == bp) {
            list8 = (void *) GET_NEXT_NODE(bp);
            return;
        }
        while (temp != HEAP_NULL) {
            if ((void *) GET_NEXT_NODE(temp) == bp) break;
            temp = (void *) GET_NEXT_NODE(temp); 
        }
        PUT_NEXT_NODE(temp, (void *) GET_NEXT_NODE(bp));
    }
    else
    	delete_list(L(bpsize), bp);
	return;
}

/* find_list() find node in list */
void *find_list(void* head, size_t asize)
{
	while (head != HEAP_NULL)
	{
		if (GET_SIZE(head) >= asize)
			return head;
		else
			head = (void *) GET_NEXT_NODE(head);
	}
	return HEAP_NULL;
}

/* insert_list() insert node in list */
void insert_list(void **ptr_list, void* bp)
{
	if (*ptr_list == HEAP_NULL)
	{
		PUT_NEXT_NODE(bp, HEAP_NULL);
		PUT_PREV_NODE(bp, HEAP_NULL);
		*ptr_list = bp;
		return;
	}
	if (GET_SIZE(*ptr_list) >= GET_SIZE(bp))
	{
		PUT_NEXT_NODE(bp, *ptr_list);
		PUT_PREV_NODE(*ptr_list, bp);
		PUT_PREV_NODE(bp, HEAP_NULL);
		*ptr_list = bp;
		return;
	}
	void * t = *ptr_list;
	while ((void *) GET_NEXT_NODE(t) != HEAP_NULL)
	{
		if (GET_SIZE(GET_NEXT_NODE(t)) >= GET_SIZE(bp))
			break;
		t = (void *) GET_NEXT_NODE(t);
	}
	void * p = (void *) GET_NEXT_NODE(t);
	PUT_NEXT_NODE(bp, p);
	if (p != HEAP_NULL)
		PUT_PREV_NODE(p, bp);
	PUT_PREV_NODE(bp, t);
	PUT_NEXT_NODE(t, bp);
	return;
}

/* delete_list() delete node in list */
void delete_list(void **ptr_list, void* bp)
{
	if (bp == *ptr_list)
	{
		*ptr_list = (void *) GET_NEXT_NODE(bp);
		PUT_PREV_NODE(*ptr_list, HEAP_NULL);
		return;
	}
	void *l = (void *) GET_NEXT_NODE(bp), *r = (void *)  GET_PREV_NODE(bp);
	PUT_NEXT_NODE(r, l);
	if (l != HEAP_NULL)
		PUT_PREV_NODE(l, r);
	return;
}

/* mm_checkheap() print information of all data structure */
void mm_checkheap(int verbose) 
{
    char *bp = heap_ptr;
	
	verbose = verbose;
	/* keep GCC be quite, just as the writeup */
	
	printf("Heap:\n");
   	printf("Checking heap...\n");
    if ((SIZE(heap_ptr) != DSIZE) || !ALLOC(heap_ptr))
        printf("Prologue header error!\n");
    while (GET_SIZE(bp) > 0)
    {
        check_block(bp);
		bp = GET_NEXT(bp);
    }
   	check_block(bp);
    if ((GET_SIZE(bp) != 0) || !(GET_ALLOC(bp)))
        printf("Epilogue header error!\n");
    printf("List8:\n");
    check_list8(list8);
    
    /* I only check some simple case of them */
    printf("List16, 24, 32, 40 :\n");
    check_list(list16);
    check_list(list24);
    check_list(list32);
    check_list(list40);
    printf("Large_list:\n");
    check_list(large_list);
    return;
}

/* check_block() check if the block is doubleword aligned*/
void check_block(void *bp) 
{
    if ((size_t) bp & 7)
    	printf("Block %lx aligned error!\n", (size_t) bp);
    return;
}

/* check_list() check a free block in list */
void check_list(void * temp)
{
	printf("Checking list...\n");
    if (temp != HEAP_NULL)
    {
        check_block(temp);
        check_list((void *) GET_NEXT_NODE(temp));
    }
    return;
}

/* check_list8() check 8-byte block in list8 */
void check_list8(void * bp)
{
	printf("Checking list8...\n");
    if (bp != HEAP_NULL)
    {
        check_block(bp);
        check_list8((void *) GET_NEXT_NODE(bp));
    }
    return;
}


/* warning: ugly code below! */

/* init_list(): init the list */
inline void init_list()
{

	/* initialize */
	/* enum */
	list8 = HEAP_NULL;
    list16 = HEAP_NULL;
    list24 = HEAP_NULL;
    list32 = HEAP_NULL;
    list40 = HEAP_NULL;
    list48 = HEAP_NULL;
    list56 = HEAP_NULL;
    list64 = HEAP_NULL;
    list72 = HEAP_NULL;
    list80 = HEAP_NULL;
    list88 = HEAP_NULL;
    list96 = HEAP_NULL;
    list104 = HEAP_NULL;
    list112 = HEAP_NULL;
    list120 = HEAP_NULL;
    list128 = HEAP_NULL;
    list136 = HEAP_NULL;
    list144 = HEAP_NULL;
    list152 = HEAP_NULL;
    list160 = HEAP_NULL;
    list168 = HEAP_NULL;
    list176 = HEAP_NULL;
    list184 = HEAP_NULL;
    list192 = HEAP_NULL;
    list200 = HEAP_NULL;
    list208 = HEAP_NULL;
    list216 = HEAP_NULL;
    list224 = HEAP_NULL;
    list232 = HEAP_NULL;
    list240 = HEAP_NULL;
    list248 = HEAP_NULL;
    list256 = HEAP_NULL;
    list264 = HEAP_NULL;
    list272 = HEAP_NULL;
    list280 = HEAP_NULL;
    list288 = HEAP_NULL;
    list296 = HEAP_NULL;
    list304 = HEAP_NULL;
    list312 = HEAP_NULL;
    list320 = HEAP_NULL;
    list328 = HEAP_NULL;
    list336 = HEAP_NULL;
    list344 = HEAP_NULL;
    list352 = HEAP_NULL;
    list360 = HEAP_NULL;
    list368 = HEAP_NULL;
    list376 = HEAP_NULL;
    list384 = HEAP_NULL;
    list392 = HEAP_NULL;
    list400 = HEAP_NULL;
    list408 = HEAP_NULL;
    list416 = HEAP_NULL;
    list424 = HEAP_NULL;
    list432 = HEAP_NULL;
    list440 = HEAP_NULL;
    list448 = HEAP_NULL;
    list456 = HEAP_NULL;
    list464 = HEAP_NULL;
    list472 = HEAP_NULL;
    list480 = HEAP_NULL;
    list488 = HEAP_NULL;
    list496 = HEAP_NULL;
    list504 = HEAP_NULL;
    list512 = HEAP_NULL;
    list520 = HEAP_NULL;
    list528 = HEAP_NULL;
    list536 = HEAP_NULL;
    list544 = HEAP_NULL;
    list552 = HEAP_NULL;
    list560 = HEAP_NULL;
    list568 = HEAP_NULL;
    list576 = HEAP_NULL;
    list584 = HEAP_NULL;
    list592 = HEAP_NULL;
    list600 = HEAP_NULL;
    list608 = HEAP_NULL;
    list616 = HEAP_NULL;
    list624 = HEAP_NULL;
    list632 = HEAP_NULL;
    list640 = HEAP_NULL;
    list648 = HEAP_NULL;
    list656 = HEAP_NULL;
    list664 = HEAP_NULL;
    list672 = HEAP_NULL;
    list680 = HEAP_NULL;
    list688 = HEAP_NULL;
    list696 = HEAP_NULL;
    list704 = HEAP_NULL;
    list712 = HEAP_NULL;
    list720 = HEAP_NULL;
    list728 = HEAP_NULL;
    list736 = HEAP_NULL;
    list744 = HEAP_NULL;
    list752 = HEAP_NULL;
    list760 = HEAP_NULL;
    list768 = HEAP_NULL;
    list776 = HEAP_NULL;
    list784 = HEAP_NULL;
    list792 = HEAP_NULL;
    list800 = HEAP_NULL;    
    large_list = HEAP_NULL;
    return;
}

/* L(size): a inline function return &list, list fits the size */
inline void** L(size_t size)
{
	/* enum */
	switch (size)
	{
		case 16: return &list16;
		case 24: return &list24;
		case 32: return &list32;
		case 40: return &list40;
		case 48: return &list48;
		case 56: return &list56;
		case 64: return &list64;
		case 72: return &list72;
		case 80: return &list80;
		case 88: return &list88;
		case 96: return &list96;
		case 104: return &list104;
		case 112: return &list112;
		case 120: return &list120;
		case 128: return &list128;
		case 136: return &list136;
		case 144: return &list144;
		case 152: return &list152;
		case 160: return &list160;
		case 168: return &list168;
		case 176: return &list176;
		case 184: return &list184;
		case 192: return &list192;
		case 200: return &list200;
		case 208: return &list208;
		case 216: return &list216;
		case 224: return &list224;
		case 232: return &list232;
		case 240: return &list240;
		case 248: return &list248;
		case 256: return &list256;
		case 264: return &list264;
		case 272: return &list272;
		case 280: return &list280;
		case 288: return &list288;
		case 296: return &list296;
		case 304: return &list304;
		case 312: return &list312;
		case 320: return &list320;
		case 328: return &list328;
		case 336: return &list336;
		case 344: return &list344;
		case 352: return &list352;
		case 360: return &list360;
		case 368: return &list368;
		case 376: return &list376;
		case 384: return &list384;
		case 392: return &list392;
		case 400: return &list400;
		case 408: return &list408;
		case 416: return &list416;
		case 424: return &list424;
		case 432: return &list432;
		case 440: return &list440;
		case 448: return &list448;
		case 456: return &list456;
		case 464: return &list464;
		case 472: return &list472;
		case 480: return &list480;
		case 488: return &list488;
		case 496: return &list496;
		case 504: return &list504;
		case 512: return &list512;
		case 520: return &list520;
		case 528: return &list528;
		case 536: return &list536;
		case 544: return &list544;
		case 552: return &list552;
		case 560: return &list560;
		case 568: return &list568;
		case 576: return &list576;
		case 584: return &list584;
		case 592: return &list592;
		case 600: return &list600;
		case 608: return &list608;
		case 616: return &list616;
		case 624: return &list624;
		case 632: return &list632;
		case 640: return &list640;
		case 648: return &list648;
		case 656: return &list656;
		case 664: return &list664;
		case 672: return &list672;
		case 680: return &list680;
		case 688: return &list688;
		case 696: return &list696;
		case 704: return &list704;
		case 712: return &list712;
		case 720: return &list720;
		case 728: return &list728;
		case 736: return &list736;
		case 744: return &list744;
		case 752: return &list752;
		case 760: return &list760;
		case 768: return &list768;
		case 776: return &list776;
		case 784: return &list784;
		case 792: return &list792;
		case 800: return &list800;
		default: return &large_list;
	}
}

/* find_fit(asize) find the fit block */
void *find_fit(size_t asize)
{
	/* find in link list */
    /* enum */
    
    if ((asize == 8) && (list8 != HEAP_NULL)) return list8;
    else if ((asize <= 16) && (list16 != HEAP_NULL)) return list16;
    else if ((asize <= 24) && (list24 != HEAP_NULL)) return list24;
	else if ((asize <= 32) && (list32 != HEAP_NULL)) return list32;
    else if ((asize <= 40) && (list40 != HEAP_NULL)) return list40;
    else if ((asize <= 48) && (list48 != HEAP_NULL)) return list48;
    else if ((asize <= 56) && (list56 != HEAP_NULL)) return list56;
	else if ((asize <= 64) && (list64 != HEAP_NULL)) return list64;
    else if ((asize <= 72) && (list72 != HEAP_NULL)) return list72;
    else if ((asize <= 80) && (list80 != HEAP_NULL)) return list80;
    else if ((asize <= 88) && (list88 != HEAP_NULL)) return list88;
	else if ((asize <= 96) && (list96 != HEAP_NULL)) return list96;
	else if ((asize <= 104) && (list104 != HEAP_NULL)) return list104;
    else if ((asize <= 112) && (list112 != HEAP_NULL)) return list112;
    else if ((asize <= 120) && (list120 != HEAP_NULL)) return list120;
	else if ((asize <= 128) && (list128 != HEAP_NULL)) return list128;
	else if ((asize <= 136) && (list136 != HEAP_NULL)) return list136;
    else if ((asize <= 144) && (list144 != HEAP_NULL)) return list144;
    else if ((asize <= 152) && (list152 != HEAP_NULL)) return list152;
    else if ((asize <= 160) && (list160 != HEAP_NULL)) return list160;
	else if ((asize <= 168) && (list168 != HEAP_NULL)) return list168;
	else if ((asize <= 176) && (list176 != HEAP_NULL)) return list176;
    else if ((asize <= 184) && (list184 != HEAP_NULL)) return list184;
    else if ((asize <= 192) && (list192 != HEAP_NULL)) return list192;
	else if ((asize <= 200) && (list200 != HEAP_NULL)) return list200;
    else if ((asize <= 208) && (list208 != HEAP_NULL)) return list208;
    else if ((asize <= 216) && (list216 != HEAP_NULL)) return list216;
    else if ((asize <= 224) && (list224 != HEAP_NULL)) return list224;
	else if ((asize <= 232) && (list232 != HEAP_NULL)) return list232;
    else if ((asize <= 240) && (list240 != HEAP_NULL)) return list240;
    else if ((asize <= 248) && (list248 != HEAP_NULL)) return list248;
    else if ((asize <= 256) && (list256 != HEAP_NULL)) return list256;
	else if ((asize <= 264) && (list264 != HEAP_NULL)) return list264;
    else if ((asize <= 272) && (list272 != HEAP_NULL)) return list272;
    else if ((asize <= 280) && (list280 != HEAP_NULL)) return list280;
    else if ((asize <= 288) && (list288 != HEAP_NULL)) return list288;
	else if ((asize <= 296) && (list296 != HEAP_NULL)) return list296;
	else if ((asize <= 304) && (list304 != HEAP_NULL)) return list304;
    else if ((asize <= 312) && (list312 != HEAP_NULL)) return list312;
    else if ((asize <= 320) && (list320 != HEAP_NULL)) return list320;
	else if ((asize <= 328) && (list328 != HEAP_NULL)) return list328;
	else if ((asize <= 336) && (list336 != HEAP_NULL)) return list336;
    else if ((asize <= 344) && (list344 != HEAP_NULL)) return list344;
    else if ((asize <= 352) && (list352 != HEAP_NULL)) return list352;
    else if ((asize <= 360) && (list360 != HEAP_NULL)) return list360;
	else if ((asize <= 368) && (list368 != HEAP_NULL)) return list368;
	else if ((asize <= 376) && (list376 != HEAP_NULL)) return list376;
    else if ((asize <= 384) && (list384 != HEAP_NULL)) return list384;
    else if ((asize <= 392) && (list392 != HEAP_NULL)) return list392;
	else if ((asize <= 400) && (list400 != HEAP_NULL)) return list400;
    else if ((asize <= 408) && (list408 != HEAP_NULL)) return list408;
    else if ((asize <= 416) && (list416 != HEAP_NULL)) return list416;
    else if ((asize <= 424) && (list424 != HEAP_NULL)) return list424;
	else if ((asize <= 432) && (list432 != HEAP_NULL)) return list432;
    else if ((asize <= 440) && (list440 != HEAP_NULL)) return list440;
    else if ((asize <= 448) && (list448 != HEAP_NULL)) return list448;
    else if ((asize <= 456) && (list456 != HEAP_NULL)) return list456;
	else if ((asize <= 464) && (list464 != HEAP_NULL)) return list464;
    else if ((asize <= 472) && (list472 != HEAP_NULL)) return list472;
    else if ((asize <= 480) && (list480 != HEAP_NULL)) return list480;
    else if ((asize <= 488) && (list488 != HEAP_NULL)) return list488;
	else if ((asize <= 496) && (list496 != HEAP_NULL)) return list496;
	else if ((asize <= 504) && (list504 != HEAP_NULL)) return list504;
    else if ((asize <= 512) && (list512 != HEAP_NULL)) return list512;
    else if ((asize <= 520) && (list520 != HEAP_NULL)) return list520;
	else if ((asize <= 528) && (list528 != HEAP_NULL)) return list528;
	else if ((asize <= 536) && (list536 != HEAP_NULL)) return list536;
    else if ((asize <= 544) && (list544 != HEAP_NULL)) return list544;
    else if ((asize <= 552) && (list552 != HEAP_NULL)) return list552;
    else if ((asize <= 560) && (list560 != HEAP_NULL)) return list560;
	else if ((asize <= 568) && (list568 != HEAP_NULL)) return list568;
	else if ((asize <= 576) && (list576 != HEAP_NULL)) return list576;
    else if ((asize <= 584) && (list584 != HEAP_NULL)) return list584;
    else if ((asize <= 592) && (list592 != HEAP_NULL)) return list592;
	else if ((asize <= 500) && (list600 != HEAP_NULL)) return list600;
    else if ((asize <= 608) && (list608 != HEAP_NULL)) return list608;
    else if ((asize <= 616) && (list616 != HEAP_NULL)) return list616;
    else if ((asize <= 624) && (list624 != HEAP_NULL)) return list624;
	else if ((asize <= 632) && (list632 != HEAP_NULL)) return list632;
    else if ((asize <= 640) && (list640 != HEAP_NULL)) return list640;
    else if ((asize <= 648) && (list648 != HEAP_NULL)) return list648;
    else if ((asize <= 656) && (list656 != HEAP_NULL)) return list656;
	else if ((asize <= 664) && (list664 != HEAP_NULL)) return list664;
    else if ((asize <= 672) && (list672 != HEAP_NULL)) return list672;
    else if ((asize <= 680) && (list680 != HEAP_NULL)) return list680;
    else if ((asize <= 688) && (list688 != HEAP_NULL)) return list688;
	else if ((asize <= 696) && (list696 != HEAP_NULL)) return list696;
	else if ((asize <= 704) && (list704 != HEAP_NULL)) return list704;
    else if ((asize <= 712) && (list712 != HEAP_NULL)) return list712;
    else if ((asize <= 720) && (list720 != HEAP_NULL)) return list720;
	else if ((asize <= 728) && (list728 != HEAP_NULL)) return list728;
	else if ((asize <= 736) && (list736 != HEAP_NULL)) return list736;
    else if ((asize <= 744) && (list744 != HEAP_NULL)) return list744;
    else if ((asize <= 752) && (list752 != HEAP_NULL)) return list752;
    else if ((asize <= 760) && (list760 != HEAP_NULL)) return list760;
	else if ((asize <= 768) && (list768 != HEAP_NULL)) return list768;
	else if ((asize <= 776) && (list776 != HEAP_NULL)) return list776;
    else if ((asize <= 784) && (list784 != HEAP_NULL)) return list784;
    else if ((asize <= 792) && (list792 != HEAP_NULL)) return list792;
	else if ((asize <= 800) && (list800 != HEAP_NULL)) return list800;
	else return find_list(large_list, asize);
}
