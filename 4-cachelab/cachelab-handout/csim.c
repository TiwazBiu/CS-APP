/*  coded by xiezhiwen
 *  mail: zhiwenxie1900@outlook.com 
 */
#define _GNU_SOURCE
#include <stdlib.h> 
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <assert.h>
#include "cachelab.h"

#define SET(address, sbits, bbits) (((address)>>(bbits))&~(~0<<(sbits)))
#define TAG(address, sbits, bbits) ((address)>>((bbits)+(sbits)))
#define BITS 64

typedef struct block{
    unsigned long tag;
    unsigned long timestamp;
    int valid;
} Block, *Blockptr;

void exit_usage_info();


int main(int argc, char *argv[])
{
    extern char* optarg;
    
    // results
    int hits, misses, evictions;
    
    // arguments
    int verbose, sbits, bbits, numl;
    verbose = sbits = bbits = numl = 0;
    FILE *input_fp = NULL;

    char opt;
    while ((opt = getopt(argc, argv, "vs:E:b:t:")) != -1){
        switch (opt){
            case 'v':
                verbose = 1;
                break;
            case 's':
                sbits = atoi(optarg);
                break;
            case 'E':
                numl = atoi(optarg);
                break;
            case 'b':
                bbits = atoi(optarg);
                break;
            case 't':
                input_fp = fopen(optarg, "r");
                assert(input_fp);
                break;
            default:
                exit_usage_info();
        }
    }
    if (input_fp==NULL || sbits<0 || bbits<0 || numl<=0){
        exit_usage_info();
    }
    if (verbose){
        printf("set bits: %d\tnumber of line per set: %d\tblock bits: %d\n",
                sbits, numl, bbits);
    }

    //allocate memory for cache
    Blockptr cache;
    size_t cache_size = (1<<sbits)*numl;
    cache = calloc(cache_size, sizeof(Block));
    assert(cache);
    
    unsigned long clock = 0;
    // parse trace file
    char *lineptr = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&lineptr, &len, input_fp)) != -1){
        // skip instruction line
        if (lineptr[0] == 'I')
            continue;
        // use clock to identify LRU block
        clock++;

        unsigned long address;
        char bytes;
        char type;
        sscanf(lineptr, " %c %lx,%c\n", &type, &address, &bytes);
        if (verbose) printf("%c %lx,%c\t", type, address, bytes);

        unsigned long set = SET(address, sbits, bbits);
        unsigned long tag = TAG(address, sbits, bbits);
        if (verbose) printf("set: %.*lx\t", (sbits+4)>>2, set);
        if (verbose) printf("tag: %.*lx\n", ((BITS-sbits-bbits)+4)>>2, tag);

        // initialize
        Blockptr match_set = cache + set*numl;
        Blockptr interest; // target block
        unsigned long min_time = match_set[0].timestamp;
        int lru = 0; // least recently used block
        int victim = -1; // empty block
        char has_hit = 0;
        
        for (int i=0; i<numl; i++){
            interest = match_set + i;
            
            // check match block
            if (!has_hit && interest->valid && interest->tag == tag){
                if (verbose) printf("hit\t");
                has_hit = 1;
                interest->timestamp = clock;
                hits++;
            } 
            // find empty block as victim
            else if (victim == -1 && !interest->valid) { 
                victim = i;
            }

            // find LRU block if haven't found match
            if (!has_hit && interest->timestamp < min_time){
                min_time = interest->timestamp;
                lru = i;
            }

        }
        
        if (!has_hit){
            if (verbose) printf("miss\t");
            misses++;
            if (victim != -1){ // empty block exists
                interest = match_set + victim;
            } else { // need eviction of LRU block
                if (verbose) printf("eviction\t");
                evictions++;
                interest = match_set + lru;
            }
            interest->tag = tag;
            interest->valid = 1;
            interest->timestamp = clock;
        }
        
        if (type == 'M'){
            interest->timestamp = ++clock;
            if (verbose) printf("hit");
            hits++;
        }
        if (verbose) printf("\n");
    }
    

    // clean up
    free(lineptr);
    free(cache);
    fclose(input_fp);
    printSummary(hits, misses, evictions);
    return 0;
}


void exit_usage_info()
{
    fprintf(stderr, "usage: ./csim [-v] "
                    "-s <s> -E <E> -b <b> -t <tracefile>\n"
                    "<E> must be positive, "
                    "<s>, <b> should be nonnegtive\n");
    exit(1);
}




























