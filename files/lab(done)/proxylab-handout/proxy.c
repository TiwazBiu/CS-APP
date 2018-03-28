/*
 * ID:1500012772
 * Zhang Zhiyuan
 *
 * Explanation:
 *
 *   1. Basic and Concurrency:
 *      1.1. I implement function main() to accept a connfd for every client;
 *      1.2. I implement doit() to deal with every connfd with a single thread;
 *      1.3. I use Posix thread to support concurrency, a thread every connect,
 *           to avoid race, every thread has its own space to store connfd;
 *      1.4. I implement connect_server() to handle connection for client;
 *      1.5. I implement prase_url() to prase a url into a request_t type;
 *      1.6. I rewrite(or say, nearly copy) helper functions clienterror() and
 *           read_requesthdrs() in Tiny.c, because these functions are less
 *           interesting and the only point about them is format.
 *
 *   2. Cache:
 * 		2.1. I implement a naive LRU cache (Unix time LRU, all way associated);
 *      2.2. Reading and writing cache is a readr-writer puzzle. With P() & V()
 *           to implement a reader-writer lock, my solution is same with the
 *           solution from textbook CS:APP, which may cause starvation.
 *
 */

#include "csapp.h"
#include <time.h>

/* my constants */
/* the recommend max object size */
#define MAX_OBJECT_SIZE 102400
/* 4 KB buffer size*/
#define LEN (1<<12)
/* max thread number */
#define THREADS 16
/* max num of cache object */
#define CACHE_OBJS 128

/* request struct type */
typedef struct request_t
{
    char hostname[LEN];
    char port[LEN];
    char uri[LEN];
}
request_t;

/* cache struct type */
typedef struct cache_t
{
	request_t r_info;
    char content[MAX_OBJECT_SIZE];
    time_t t; /* Unix time to implement LRU*/
}
cache_t;

/* cache variable */
static cache_t cache[CACHE_OBJS];
static int cache_objs;

/* a sulution to reader-writer problem and to read-write lock the cache */
static int read_count;
static sem_t mutex, w;
static void RH();
static void RT();
static void WH();
static void WT();

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

/* basic functions, resort to head comment for detials */
void *doit(void* pconnfd);
void connect_server(int connfd, struct request_t *r_info);
void parse_url(char *url, struct request_t *r_info);

/* basic helper functions, generally same as Tiny.c */
void clienterror(int fd, char *cause, char *errnum, char *stmsg, char *lgmsg);
void read_requesthdrs(rio_t *rp);

/* cache function */
int find_cache(int connfd, struct request_t* r_info);

int main(int argc, char **argv)
{
    int listenfd, connfd;
    int connfd_storage[THREADS], storages;
    pthread_t tid;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* check the argv */
    if (argc != 2)
    {
        printf("usage: %s <port>\n", argv[0]);
        exit(1);
    }
    
    /* init and start */
    printf("%s",user_agent_hdr);
   	Signal(SIGPIPE, SIG_IGN);
   	storages = 0;

	/* init cache and its lock */
	cache_objs = 0;
	read_count = 0;
    Sem_init(&mutex, 0, 1);
    Sem_init(&w, 0, 1);
	
    /* open listen fd */
    listenfd = Open_listenfd(argv[1]);
    /* deal with every connect in individual threads */
    while(1)
    {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);

		/* store connfd in different places in storages to avoid rave*/
        connfd_storage[storages] = connfd;
        Pthread_create(&tid, NULL, doit, connfd_storage+storages);
        storages++;
    }
    exit(0);
}

/* doit(pconnfd): deal with a HTTP1.0/1.1 request stored in pconnfd */
void *doit(void *pconnfd)
{
	int connfd;
    char buf[LEN], method[LEN], url[LEN], version[LEN];
    rio_t rio;
    request_t r_info;

    /* fetch connfd in pconnfd */
    connfd = *(int *) pconnfd;
    
    /* detach itself to avoid memory leak */
    Pthread_detach(Pthread_self());
    
    /* init rio */
    Rio_readinitb(&rio, connfd);
    
    /*read and prase request */
    Rio_readlineb(&rio, buf, LEN);
    read_requesthdrs(&rio);
	sscanf(buf, "%s %s %s", method, url, version);
	
    /* check request and prase url */
    if (strcmp(method, "GET"))
    {
        clienterror(connfd, method, "501", "Not Supported method"
        , "Proxy does not support this method");
        exit(1);
    }
    if (strcmp(version, "HTTP/1.0") && strcmp(version, "HTTP/1.1"))
    {
        clienterror(connfd, version, "501", "Not Supported version",
        "Proxy only support HTTP/1.0 or 1.1 version");
        exit(1);
    }
    parse_url(url, &r_info);
    
    /* handle connection */
    if (!find_cache(connfd, &r_info))
    	connect_server(connfd, &r_info);
    
    Close(connfd);
    return NULL;
}

/* connect_server(): connect server for request r_info for connfd and deal */
void connect_server(int connfd, struct request_t *r_info)
{
    int clientfd;    
    rio_t rio;
    char buf[LEN+1], head_buf[LEN+1], cache_buf[MAX_OBJECT_SIZE+1];

	/* open clientfd to server */
    clientfd = Open_clientfd(r_info->hostname, r_info->port);
    Rio_readinitb(&rio, clientfd);

    /* write request to server */
    #if 1
    sprintf(buf, "%s %s\r\n\r\n", "GET", r_info->uri);
    #else
    /* it maybe better, but for server tiny, it works without version in head */
    #ifdef HTTP_1_0
    sprintf(buf, "%s %s %s\r\n\r\n", "GET", r_info->uri, "HTTP/1.0"); 
    #else
    sprintf(buf, "%s %s %s\r\n\r\n", "GET", r_info->uri, "HTTP/1.1"); 
    #endif
    #endif
    
    Rio_writen(clientfd, buf, strlen(buf));
    
    /* read header info from server */
    Rio_readlineb(&rio, buf, LEN);
    while (strcmp(buf, "\r\n"))
    {
        sprintf(head_buf, "%s%s", head_buf, buf);
        Rio_readlineb(&rio, buf, LEN);
    }
    sprintf(head_buf, "%s\r\n", head_buf);
	
    /* write header back to client and store in cache buf */
    Rio_writen(connfd, head_buf, strlen(head_buf));     
    strcpy(cache_buf, head_buf);  
    
    /* read content from server and write it back to client */
   	while (Rio_readnb(&rio, buf, LEN))
   	{
    	/* write header back to client and store in cache buf */
        Rio_writen(connfd, buf, LEN);
    	strcat(cache_buf, buf);  
    }

    /* refresh cache */
    
    RH();
    int objs = cache_objs;
    RT();
    
    if (strlen(cache_buf) > MAX_OBJECT_SIZE)
    	return;
    
    if (objs < CACHE_OBJS)
    {
    	WH();
    	/* copy content and r_info into cache */
    	strcpy(cache[objs].content, cache_buf);
    	strcpy(cache[objs].r_info.uri, r_info->uri);
    	strcpy(cache[objs].r_info.port, r_info->port);
    	strcpy(cache[objs].r_info.hostname, r_info->hostname);
    	cache[objs].t = time(NULL);
    	cache_objs++;
    	WT();
    }
    else
    {
    	RH();
    	/* read cache to find a evict with the LRU stragety */
    	int evict = 0, i;
 		for (i = 1; i < cache_objs; i++)
 			if (cache[i].t < cache[evict].t)
 				evict = i;
 		RT();
 		
 		WH();
    	/* copy content and r_info into cache */
		strcpy(cache[evict].content, cache_buf);
    	strcpy(cache[evict].r_info.uri, r_info->uri);
    	strcpy(cache[evict].r_info.port, r_info->port);
    	strcpy(cache[evict].r_info.hostname, r_info->hostname);
    	cache[evict].t = time(NULL);
    	WT();
    }
	
    Close(clientfd);
    return;
}

/* parse_url(): parse url into info saved in pointer r_info */    
void parse_url(char *url, struct request_t *r_info)
{
    char *p1, *p2;
    /* url: "http://hostname:post/uri_content ", while uri = "/uri_content" */
    
    /* get url now: "hostname:post/uri_content", while uri = "/uri_content" */
    sscanf(url, "http://%s", url); 
    
    /* get uri out of url, url now: "hostname:post" */
    p2 = strchr(url, '/');
	strcpy(r_info->uri, p2);
    *p2 = '\0';
    
    /* get port out of url, url now: "hostname" */
    p1 = strchr(url, ':');
   	strcpy(r_info->port, p1+1);
    *p1 = '\0';
	
	/* get hostname in url, and clean url to empty string */
    strcpy(r_info->hostname, url);
    *url = '\0';
    
    return;
}

/* find_cache(): find whether r_info is in cache and deal with it if exsit */
int find_cache(int connfd, struct request_t* r_info)
{
	struct request_t *r_cache_info;
	int i, objs, same;
	
	RH();
	objs = cache_objs;
	RT();
	
	for (i=0; i < objs; i++)
	{
		RH();
		r_cache_info = &cache[i].r_info;
		same = (strcmp(r_info->uri, r_cache_info->uri) == 0) &&
		       (strcmp(r_info->hostname, r_cache_info->hostname) == 0) &&
		       (strcmp(r_info->port, r_cache_info->port) == 0);
		RT();
		/* if same, it means finding it in cache, deal with it then */
		if (same)
		{
			WH();
			cache[i].t = time(NULL);
			WT();
			RH();
			Rio_writen(connfd, cache[i].content, strlen(cache[i].content));
			RT();
			return 1;
		}
	}
	
	/* unfound in cache */
	return 0;
}

/* clienterror(): generally same as tiny server */
void clienterror(int fd, char *cause, char *errnum, char *stmsg, char *lgmsg)
{
	char buf[LEN], body[LEN];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, stmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, lgmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, stmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int) strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
    
    return;
}

/* read_requesthdrs(): generally same as tiny server */
void read_requesthdrs(rio_t *rp)
{
	char buf[LEN];
	Rio_readlineb(rp, buf, LEN);
	while (strcmp(buf, "\r\n")) /* line:netp:readhdrs:checkterm */
		Rio_readlineb(rp, buf, LEN);
    return;
}

/* the solution to readr-writer puzzle */

/* reader head */
static void RH()
{
	P(&mutex);
	read_count++;
	if (read_count == 1)
		P(&w);
	V(&mutex);
}

/* reader tail */
static void RT()
{
	P(&mutex);
	read_count--;
	if (read_count == 0)
		V(&w);
	V(&mutex);
}

/* writer head */
static void WH()
{
	P(&w);
}

/* writer tail */
static void WT()
{
	V(&w);
}

