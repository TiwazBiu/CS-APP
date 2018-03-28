/*
	Zhiyuan Zhang
	ID:1500012772
*/

#include "cachelab.h"
#include "stdio.h"
#include "stdlib.h"

//line_type:type of a line, last_used:last time this line is used
typedef struct
{
    int valid;
    int last_used;
    long long tag;
}line_type;

//set_type:type of a set, used:the number of used line in this set
typedef struct
{
    line_type* line;
    int used;
}set_type;

//cache_type:type of a cache
typedef struct
{
    set_type* set;
}cache_type;

//time:the clock to record time when access cache 
int time=0;
//display:whether to print details of every instruction, for the convience of test
int display=0;
//the information of file
char* file_name;
FILE* file;

int hits=0,misses=0,evictions=0;
int s,E,b;
cache_type cache;

//functions used, read the code and head comment in each for details
void access_cache(long long addr,int* p_miss_flag,int* p_eviction_flag,int* p_hit_flag);
void deal_a_line(char op,long long addr);
void read_cmd(int argc,char** argv);
void malloc_cache();
void free_cache();

//function fot error checking
void error(int status);
/*
	status 0: Opening file failed.
	status 1: Command line incorrect.
	status 2: Memory allocation failed.
	status 3: Data in the file incorrect.
*/


int main(int argc,char** argv)
{
    //op: the operation such as 'M',L','I','S', addr:address, size:size
    char op;
    int size;
    long long addr;
	
    read_cmd(argc,argv);
    malloc_cache();
	file=fopen(file_name,"r");
	if (file==NULL)
		error(0);
		
    //read line by line in the file
    while (fscanf(file,"%c",&op)==1)
    {
		while (op==' ')
	    fscanf(file,"%c",&op);
        //if the op is 'I', do nothing, else deal it with deal_a_line()
        if (op!='I')
		{
            if (fscanf(file,"%llx,%d\n",&addr,&size)!=2)
            	error(3);
           	if (display)
	       		printf(" %c %llx,%d",op,addr,size);
    	    deal_a_line(op,addr);
            printf("\n");
		}
		else
		{
    	    if (fscanf(file,"%llx,%d\n",&addr,&size)!=2)
    	    	error(3);
		}
    }

    printSummary(hits,misses,evictions);
    fclose(file);
    return 0;
}

//to simulate accessing cache with addr(address), and changes the state in *p_flag 
void access_cache(long long addr,int* p_miss_flag,int* p_eviction_flag,int* p_hit_flag)
{
    //use this mask and bit level skills to calculate tag and id of set in address
    long long tag_mask=(((-1)<<(s+b))),block_mask=~((-1)<<b),set_mask=~(tag_mask|block_mask);
    //the tag and the id of set
    long long tag_num=addr&tag_mask,set_num=(addr&set_mask)>>b; 
    //p_set:points to the set now accessing
    set_type* p_set=&cache.set[set_num];
    //update the time
    time++;

    //check whether hit
    for (int i=0;i<E;i++)
        if (p_set->line[i].valid==1 && p_set->line[i].tag==tag_num)
        {
            (*p_hit_flag)++;
            p_set->line[i].last_used=time;
            return;
        }

    //unhit
    (*p_miss_flag)++;        
    //check whether evictim
    if (p_set->used==E)
    {
        //evictim
        (*p_eviction_flag)++;
        //find the line whose last used time is earliset and evictim it 
	int i0=0;
        for (int i=1;i<E;i++)
            if (p_set->line[i].last_used<p_set->line[i0].last_used)
                i0=i;
        p_set->line[i0].last_used=time;
		p_set->line[i0].tag=tag_num;
    }
    else
    {
	//not evictim
        p_set->used++;
        for (int i=0;i<E;i++)
            if (p_set->line[i].valid==0)
            {
	        p_set->line[i].valid=1;
			p_set->line[i].last_used=time;
			p_set->line[i].tag=tag_num;
                return;
	    }
    }
    return;
}

//allocate memory to cache
void malloc_cache()
{
    cache.set=malloc(sizeof(set_type)*(1<<s));
    if (cache.set==NULL)
    	error(2);
    //(1<<s) sets
    for (int i=0;i<(1<<s);i++)
    {
        //E lines per set
		cache.set[i].line=malloc(sizeof(line_type)*E);
		if (cache.set[i].line==NULL)
    		error(2);
        //initialize every line
        cache.set[i].used=0;
        for (int j=0;j<E;j++)
	    cache.set[i].line[j].valid=0;
    }
    return;
}

//free the memory allocated to cache
void free_cache()
{
    for (int i=0;i<(1<<s);i++)
    	free(cache.set[i].line);  
    free(cache.set);
    return;
}

//to read the information included in the command
void read_cmd(int argc,char** argv)
{
    //k:now dealing with argv[k]
    int k=1;
    while (k<argc)
    {
	//now that argv[k][0]=='-', we deal with argv[k][1] directly
	switch (argv[k][1])
        {
	    case 'v':
		display=1;
		k++;
		break;
	    case 's':
		s=atoi(argv[k+1]);
		k+=2;
		break;
	    case 'E':
		E=atoi(argv[k+1]);
		k+=2;
		break;
	    case 'b':
		b=atoi(argv[k+1]);
		k+=2;
		break;
	    case 't':
		file_name=argv[k+1];
		k+=2;
		break;
        }
    }
    return;
}

//to deal a iine, and count the misses, evictions, hits in this instruction
void deal_a_line(char op,long long addr)
{
    int miss_flag=0,eviction_flag=0,hit_flag=0;
    //every instruction will access cache at least once
    access_cache(addr,&miss_flag,&eviction_flag,&hit_flag);
    //modify the memory will access cache twice
    if (op=='M')
    	access_cache(addr,&miss_flag,&eviction_flag,&hit_flag);
    if (display)
    {
        if (miss_flag)
            printf(" miss");
        if (eviction_flag)
            printf(" eviction");
        if (hit_flag)
            printf(" hit");
        if (hit_flag==2)
	    printf(" hit");
    }
    //count changes in the cache in this line
    misses+=miss_flag;
    evictions+=eviction_flag;
    hits+=hit_flag;
    return;
}

//function for error checking
void error(int status)
{
	switch (status)
	{
		case 0:
			printf("Error: Opening file failed!\n");
			break;
		case 1:
			printf("Error: Command line incorrect!\n");
			break;
		case 2:
			printf("Error: Memory allocation failed!\n");
			break;
		case 3:
			printf("Error: Data in %s incorrect!\n",file_name);
	}
	printf("An error occurs! The program will terminate!\n");
	scanf("\n");
	//terminate the program immediately
	exit(0);
}
