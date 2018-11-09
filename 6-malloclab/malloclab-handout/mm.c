/*
 * mm.c
 * xiezhiwen mail: zhiwenxie1900@outlook.com
 *
 * explict list with good enough fit, performance index very from 56-82
 * based on the change of GOODENOUGH and CHUNKSIZE parameter
 * Best Perf index = 43 (util) & 39 (thru) = 82/100 (G:1,C:1<<10)
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "contracts.h"

#include "mm.h"
#include "memlib.h"


// Create aliases for driver tests
// DO NOT CHANGE THE FOLLOWING!
#ifdef DRIVER
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#endif

/*
 *  Logging Functions
 *  -----------------
 *  - dbg_printf acts like printf, but will not be run in a release build.
 *  - checkheap acts like mm_checkheap, but prints the line it failed on and
 *    exits if it fails.
 */

#ifndef NDEBUG
#define dbg_printf(...) printf(__VA_ARGS__)
#define checkheap(verbose) do {if (mm_checkheap(verbose)) {  \
                             printf("Checkheap failed on line %d\n", __LINE__);\
                             exit(-1);  \
                        }}while(0)
#else
#define dbg_printf(...)
#define checkheap(...)
#endif

/* 
 * constants and macros to get block information
 */

#define WSIZE 4             // Word and header/footer size(bytes)
#define DSIZE 8             // Double word size(bytes)
#define GOOD_ENOUGH 1       // find at most GOOD_ENOUGH fits
#define CHUNKSIZE (1<<10)   // Extend heap by this amount(bytes)

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

// pack a size and allocated bit into a word
#define PACK(size, alloc) ((size) | (alloc))

// Read and write a word at address p
#define GET(p) (*(uint32_t *)(p))
#define PUT(p, val) (*(uint32_t *)(p) = (val))

// Read size and allocated field form header or footer
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

// Given block ptr block, get previous/next block
#define NEXT_BLOCK(block) ((uint32_t *)(block) + (GET_SIZE(block) >> 2))
#define PREV_BLOCK(block) ((uint32_t *)(block) - (GET_SIZE((uint32_t *)(block) - 1) >> 2))

// Given block ptr block, compute address of its footer
#define FTRP(block) ((uint32_t *)(block) + (GET_SIZE(block) >> 2) - 1)

// Given block memory poniter ptr, get pointer pointing header of block;
#define BLOCK(ptr) ((uint32_t *)(ptr) - 1) 


/*
 *  Global varibles
 *  ----------------
 */

// Private global varible
static uint32_t *heap_listp;
static uint32_t *free_listp; // header pointer for explict free block list

// static uint32_t *class1_listp; // ready for segregated list
// static uint32_t *class2_listp;
// static uint32_t *class3_listp;
// static uint32_t *class4_listp;


/*
 *  Helper functions
 *  ----------------
 */

// Align p to a multiple of w bytes
static inline void* align(const void const* p, unsigned char w) {
    return (void*)(((uintptr_t)(p) + (w-1)) & ~(w-1));
}

// Check if the given pointer is 8-byte aligned
static inline int aligned(const void const* p) {
    return align(p, 8) == p;
}

// Return whether the pointer is in the heap.
static inline int in_heap(const void* p) {
    return p <= mem_heap_hi() && p >= mem_heap_lo();
}


/*
 *  Block Functions
 *  ---------------
 *  The functions below act similar to the macros above, 
 *  but also check the validity of pointer passed in.
 */

// Return the size of the given block in bytes
static inline size_t block_size(const uint32_t* block) {
    // REQUIRES(block != NULL);
    // REQUIRES(in_heap(block));

    return GET_SIZE(block);
}

// Return true if the block is allocted, false otherwise
static inline int block_alloc(const uint32_t* block) {
    // REQUIRES(block != NULL);
    // REQUIRES(in_heap(block));

    return GET_ALLOC(block);
}

// Mark the given block as free(0)/alloced(1) by marking the header and footer.
// And set the size if given a non-zero size argument.
static inline void block_mark(uint32_t* block, int alloced, size_t size) {
    size_t next;
    size_t mark;
    // REQUIRES(block != NULL);
    

    if (size == 0){
        size = block_size(block);  
    }
    next = (size - WSIZE);
    mark = alloced ? (size | 0x1) : (size & ~0x7);
    PUT(block, mark);
    PUT((char*)block + next, mark);
}

// Return a pointer to the memory that malloc should return to user
static inline uint32_t* block_mem(uint32_t* block) {
    // REQUIRES(block != NULL);
    // REQUIRES(in_heap(block));
    // REQUIRES(aligned(block + 1));

    return block + 1;
}

// Return the header to the previous block
static inline uint32_t* block_prev(uint32_t* block) {
    uint32_t* bp;
    // REQUIRES(block != NULL);
    // REQUIRES(in_heap(block));
    
    bp = PREV_BLOCK(block);
    // REQUIRES(in_heap(bp));
    return bp;
}

// Return the header to the next block
static inline uint32_t* block_next(uint32_t* block) {
    uint32_t* bp;
    // REQUIRES(block != NULL);
    // REQUIRES(in_heap(block));
    
    bp = NEXT_BLOCK(block);
    // REQUIRES(in_heap(bp));
    return bp;
}

// computer how much block size is needed when given memory size
static inline size_t actual_size(size_t msize) {
    size_t asize;
    if (msize <= DSIZE)
        asize = 2 * DSIZE;
    else // uprounded size plus overhead
        asize = DSIZE * ((msize + DSIZE + DSIZE - 1) / DSIZE); 
    return asize;
}

/*
 *  Double linked list manipulate Functions
 *  ---------------
 *  Taking advantage of the fact that max heap size is 2^32,
 *  so distance btween block and list head can be stored in 32 bit(uint32_t)
 */

// Given the distance between head of the heap and block, 
// return the pointer of specified free block.
static inline uint32_t* get_pointer(size_t distance) {
    if (distance == 0) // prev/next block is set to NULL
        return NULL;
    return heap_listp + distance;
}

// invert of get_pointer function
static inline size_t get_distance(uint32_t* bp) {
    if (bp == NULL) // set distance = 0 if no prev/next free block
        return 0;
    return bp - heap_listp;
}

// set the previous free block pointer field of current block
static inline void set_prev(uint32_t* block, uint32_t* prev_bp) {
    // REQUIRES(in_heap(block));
    size_t distance = get_distance(prev_bp);
    PUT(block + 1, distance);
}

// get the previous free block pointer field of current block
static inline uint32_t* get_prev(uint32_t* block) {
    size_t distance = (size_t)GET(block + 1);
    return get_pointer(distance);
}

// set the next free block pointer field of current block
static inline void set_next(uint32_t* block, uint32_t* next_bp) {
    // REQUIRES(in_heap(block));
    size_t distance = get_distance(next_bp);
    PUT(block + 2, distance);
}

// get the next free block pointer field of current block
static inline uint32_t* get_next(uint32_t* block) {
    size_t distance = (size_t)GET(block + 2);
    return get_pointer(distance);
}


/*
 *  Explict Free List Functions
 *  ---------------
 *  insert or delete free blocks from double linked list
 */

// insert a block to head of free block list, since we need to change 
// the value of listp itself, so we should pass the pointer of listp
static inline void insert(uint32_t* block, uint32_t* *listp) {
    // REQUIRES(in_heap(block));
    if (*listp == NULL) { // empty free list
        *listp = block;
        // it's both head and tail
        set_next(block, NULL);
        set_prev(block, NULL);
    } else {
        uint32_t *bp = *listp;
        *listp = block;
        set_prev(block, NULL); // free block is now head
        set_next(block, bp);
        set_prev(bp, block);
    }
}

// delete a block from free list to use it
static inline void delete(uint32_t* block, uint32_t* *listp) {
    // REQUIRES(in_heap(block));
    // REQUIRES(!block_alloc(block));

    uint32_t* prev_bp = get_prev(block);
    uint32_t* next_bp = get_next(block);
    if (*listp == block) { // block is head
        *listp = NULL;
        if (next_bp != NULL)
            *listp = next_bp;
    }
    if (prev_bp != NULL) // block is not the head
        set_next(prev_bp, next_bp);
    if (next_bp != NULL) // block is not the tail
        set_prev(next_bp, prev_bp);

}
/*
 *  Manipunate Functions
 *  ---------------
 *  The functions below use functions above to implement crucial functionalities
 *  to manipunate block.
 */


// add block to free list, coalesce them if possible
static uint32_t* coalesce(uint32_t *block)
{
    // REQUIRES(block != NULL);
    // REQUIRES(in_heap(block));
    // REQUIRES(aligned(block + 1));

    uint32_t* prev_bp = block_prev(block);
    uint32_t* next_bp = block_next(block);
    
    int prev_alloc = block_alloc(prev_bp);
    int next_alloc = block_alloc(next_bp);
    
    size_t size = block_size(block);
    if (prev_alloc && next_alloc) {
        insert(block, &free_listp);
        return block;
    } 
    // alter the free list accordingly
    else if (prev_alloc && !next_alloc) {
        delete(next_bp, &free_listp);
        
        size += block_size(next_bp);
        PUT(FTRP(next_bp), PACK(size, 0));
        PUT(block, PACK(size, 0));
        
        insert(block, &free_listp);
        return block;
    } 
    // in this case, we don't nedd to change free list
    else if (!prev_alloc && next_alloc) {
        size += block_size(prev_bp);
        PUT(prev_bp, PACK(size, 0));
        PUT(FTRP(block), PACK(size, 0));
        return prev_bp;
    }

    else {
        delete(next_bp, &free_listp);
        size += block_size(prev_bp) + block_size(next_bp);
        PUT(prev_bp, PACK(size, 0));
        PUT(FTRP(next_bp), PACK(size, 0));
        return prev_bp;
    }
}

// extend heap with a new free block
static uint32_t* extend_heap(size_t words)
{
    size_t size;
    uint32_t* block;  // block pointer
    // Allocte an even number of words to maintain alignment
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if ((block = mem_sbrk(size)) == (void *)-1) 
        return NULL;
    REQUIRES(aligned(block));   // check validity of alignment
    block--;                        // get to orignal epologue header
    block_mark(block, 0, size);

    PUT(NEXT_BLOCK(block), PACK(0, 1));   // New epilogue header
    
    return coalesce(block);
}

// find good enough fit in explict free list
static void* find_fit(size_t asize)
{   
    size_t min_remain = -1; // max
    int good = 0; 
    uint32_t *best_bp = NULL;
    for (uint32_t* bp = free_listp; bp != NULL && block_size(bp) > 0; bp = get_next(bp)){
        if (!block_alloc(bp)){
            size_t size = block_size(bp);
            if (size == asize){ // perfect fit
                return bp;
            }
            else if (size > asize && size - asize < min_remain){
                min_remain = size - asize;
                best_bp = bp;
                if (++good == GOOD_ENOUGH) // find at most GOOD_ENOUGH fits
                    break;
            }
        } else {
            checkheap(2);
            dbg_printf("error in find fit, go through free list," 
                        "block %p is not a free block\n", (void*)bp);
            exit(-1);
        }
    }
    return best_bp;
}

// split block when possible
static void split(uint32_t *block, size_t asize) {
    size_t fsize = block_size(block);
    size_t remain = fsize - asize;
    
    if (remain < 4 * WSIZE) { // not enough space to split
        block_mark(block, 1, fsize);
    } else {
        block_mark(block, 1, asize);
        block = block_next(block);
        block_mark(block, 0, remain);
        coalesce(block);
    }
}

// delete block from free list, allocate it, and split if possible 
static void place(uint32_t *block, size_t asize) {
    delete(block, &free_listp);
    split(block, asize);
}


/*
 *  Malloc Implementation
 *  ---------------------
 *  The following functions deal with the user-facing malloc implementation.
 */

/*
 * Initialize: return -1 on error, 0 on success.
 */
int mm_init(void) {
    free_listp = NULL; // no free block available from start

    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1) 
        return -1;
    PUT(heap_listp, 0);                      // Alignment padding
    PUT(heap_listp + 1, PACK(DSIZE, 1));     // Prologue header
    PUT(heap_listp + 2, PACK(DSIZE, 1));     // Prologue footer
    PUT(heap_listp + 3, PACK(0, 1));         // Epilogue header
    heap_listp += 1;

    if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
        return -1;

    return 0;
}

/*
 * malloc
 */

void *malloc (size_t size) {
    size_t asize;      // Adjusted block size
    size_t extendsize; // Amount to extend heap if no fit
    uint32_t *block;
    // checkheap(2);      // Let's make sure the heap is ok!
    
    if (size == 0)
        return NULL;
    asize = actual_size(size); // get actual size needed
    if ((block = find_fit(asize)) == NULL) { // No fit found. Get more memory
        extendsize = MAX(asize, CHUNKSIZE);
        // dbg_printf("extend size : %lu\n", extendsize);
        if ((block = extend_heap(extendsize/WSIZE)) == NULL) // extend heap failed
            return NULL;
    }
    place(block, asize);
    return block_mem(block);
}

/*
 * free
 */
void free (void *ptr) {
    if (ptr == NULL) {
        return;
    }
    uint32_t* block = BLOCK(ptr); // get block pointer from ptr to memory
    block_mark(block, 0, 0); // mark alloc bit to 0, don't change size
    coalesce(block);
}

/*
 * realloc 
 * choose another free block only when original block can't hold new size data.
 */
void *realloc(void *oldptr, size_t size) {
    void *newptr;
    size_t oldsize;// block original size 
    size_t asize; // actual use space
    uint32_t *bp; // corresponding block pointer
    uint32_t *next_bp;// next block pointer

    
    if (size == 0) {
        free(oldptr);
        return NULL;
    }
    if (oldptr == NULL) {
        return malloc(size);
    }
    // up-rounded size to compare with block size
    asize = actual_size(size);
    bp = BLOCK(oldptr); // get block pointer
    oldsize = block_size(bp);
    next_bp = block_next(bp);
    REQUIRES(in_heap(next_bp));

    if (oldsize >= asize) {
        split(bp, asize); // change allocted block size when possible
        return block_mem(bp);
    }
    // if next block is free, add it to see if whole size is larger than requested
    else if (!block_alloc(next_bp) &&
                (oldsize + block_size(next_bp) >= asize)) { 

        delete(next_bp, &free_listp);
        oldsize += block_size(next_bp);
        PUT(FTRP(next_bp), PACK(oldsize, 0));
        PUT(bp, PACK(oldsize, 0));

        split(bp, asize);
        return block_mem(bp);

    }
    // need malloc a new place and copy the data
    else {
        newptr = malloc(size);
        if (!newptr) { // realloc failed
            return NULL;
        }
        oldsize = MIN(oldsize, size);
        memcpy(newptr, oldptr, oldsize);
        free(oldptr);
        return newptr;
    }
}

/*
 * calloc - simple implemetation, just malloc and then memset
 */
void *calloc (size_t nmemb, size_t size) {
    size_t bytes = nmemb * size;
    void *ptr = malloc(bytes);
    if (ptr != NULL) {
        memset(ptr, 0 , bytes);
        return ptr;
    }
    return NULL;
}

// Returns 0 if no errors were found, otherwise returns the error
int mm_checkheap(int verbose) {
    int heap_ok = 0; // check if heap is ok
    int free_list_ok = 0; // check if free list is ok
    int num = 1; // count number of free blocks
    uint32_t* bp = heap_listp;
    size_t size;
    
    size_t sums = 0;
    num = 0;
    while (in_heap(bp)) {
        int alloc = block_alloc(bp);
        if (!alloc)
            num++; // count free block

        size = block_size(bp);
        sums += size;
        if (verbose > 1){
            dbg_printf("checking heap, block %p, size: %lu, state: %s, "
                        "free blocks: %d\n",
                        (void*)bp, size, alloc?"allocted":"free", num);
        }

        if (size == 0){
            dbg_printf("last position: %p, relative position: %lu/%ld\n",
                (void*)bp, sums, (bp - heap_listp)*WSIZE);
            heap_ok = 1;
            break;
        }
        bp = NEXT_BLOCK(bp);
    }
    if (!heap_ok){
        dbg_printf("failed on block pointer %p\n", (void*)bp);
        return -1;
    }

    // checking free list
    bp = free_listp;
    while(bp != NULL && in_heap(bp)) {
        int alloc = block_alloc(bp);
        if (!alloc)
            num--;

        size = block_size(bp);
        if (verbose > 1){
            dbg_printf("checking free list %d, block %p, size: %lu, state: %s\n", 
                        num, (void*)bp, size, alloc?"allocated":"free");
        }
        if (alloc == 1) {
            dbg_printf("Error: block %p is allocated\n", (void*)bp);
            return -1;
        }

        uint32_t* next = get_next(bp);
        uint32_t* prev = get_prev(bp);

        if (next != NULL) {
            uint32_t* block = get_prev(next);
            if (block != bp) {
                dbg_printf("Error: next free block adnormal\n"
                    "prev(bp)=%p, next(bp)=%p, "
                    "prev(next(bp))=%p not equal to bp=%p\n",
                    (void*)prev, (void*)next,(void*)block, (void*)bp);
                return -1;
            }
        } 
        else {// end of free list
            free_list_ok = 1;
            break;
        }

        if (prev != NULL) {
            uint32_t* block = get_next(prev);
            if (block != bp) {
                dbg_printf("Error: prev free block adnormal\n"
                    "prev(bp)=%p, next(bp)=%p, "
                    "prev(next(bp))=%p not equal to bp=%p\n",
                    (void*)prev, (void*)next,(void*)block, (void*)bp);
                return -1;
            }
        }

        bp = get_next(bp);
    }

    if (!free_list_ok){
        dbg_printf("failed on block pointer %p\n", (void*)bp);
        return -1;
    }

    if (num != 0){
        dbg_printf("free block inconsistent\n");
        return -1;
    }

    dbg_printf("heap normal\n\n");
    return 0;
}

