/* 
 * tsh - A tiny shell program with job control
 * 
 * Zhang Zhiyuan
 * ID:1500012772
 *  
 *
 *  1. I implement eval() and three signal handlers as required in writeup.
 *  2. I implement functions wrappers with error check in Unix style,
 * 	   so in my code, system functions foo() are wrapped as wrapper Foo(),
 *     I won't do error check after call system functions wrappers any more,
 * 	   which can reduce the code length, while doing error check meantime.
 *     And since unix_error() will terminate the shell, using my_unix_error()
 *     instead to ensure tsh won't terminate when the file donesn't exsit.
 *  3. I implement helper functions in eval() to make eval() readable, and 
 *     decrese code dupliction, just as the style writeup requires. Especially
 *     the helper functions builtin_cmd() and dojob(), just as the standard 
 *     style of the code in CS:APP textbook. So, my code of eval() is short 
 *     and simple, if you ignore all the comments :-)
 *  4. I use sio_ function to ensure concurrency safety in the signal handler. 
 *  5. I restore errorno in signal handler to make error information correct.
 *  6. I block and unblock around builtins and child processes to avoid race.
 */
 
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

/* Job states */
#define UNDEF         0   /* undefined */
#define FG            1   /* running in foreground */
#define BG            2   /* running in background */
#define ST            3   /* stopped */

/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Parsing states */
#define ST_NORMAL   0x0   /* next token is an argument */
#define ST_INFILE   0x1   /* next token is the input file */
#define ST_OUTFILE  0x2   /* next token is the output file */

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */

struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};
struct job_t job_list[MAXJOBS]; /* The job list */

struct cmdline_tokens {
    int argc;               /* Number of arguments */
    char *argv[MAXARGS];    /* The arguments list */
    char *infile;           /* The input file */
    char *outfile;          /* The output file */
    enum builtins_t {       /* Indicates if argv[0] is a builtin command */
        BUILTIN_NONE,
        BUILTIN_QUIT,
        BUILTIN_JOBS,
        BUILTIN_BG,
        BUILTIN_FG} builtins;
};

/* End global variables */

/* Function prototypes */
void eval(char *cmdline);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, struct cmdline_tokens *tok); 
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *job_list);
int maxjid(struct job_t *job_list); 
int addjob(struct job_t *job_list, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *job_list, pid_t pid); 
pid_t fgpid(struct job_t *job_list);
struct job_t *getjobpid(struct job_t *job_list, pid_t pid);
struct job_t *getjobjid(struct job_t *job_list, int jid); 
int pid2jid(pid_t pid); 
void listjobs(struct job_t *job_list, int output_fd);

void usage(void);
void my_unix_error(char *msg);
void app_error(char *msg);
ssize_t sio_puts(char s[]);
ssize_t sio_putl(long v);
void sio_error(char s[]);

typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/* helper functions in eval() */

/* 
 * open_file(ptr_tok, ptr_infg, ptr_outfg)
 * open the file according to ptr_tok,
 * and store file figure in ptr_infg and ptr_outfig 
 *
 */
void open_file(struct cmdline_tokens* ptr_tok, int* ptr_infg, int* ptr_outfg);

/* close_file(ptr_infg, ptr_ourfg) close I/O file ptr_fg points */
void close_file(int* ptr_infg, int* ptr_outfg);

/* set_signal() sets signal SIGCHLD, SIGTSTP, SIGINT in ptr_sig_set */
void set_signal(sigset_t* ptr_sig_set);

/* getjid(ptr_tok) get jid in cmdline_tokena ptr_tok points */
int getjid(struct cmdline_tokens* ptr_tok);

/* dojob() do job fg or bg */		            
void dojob(struct job_t* job, int bg);

/* builtim_cmd() check if cmd is built in, and execute builtin command */
int builtin_cmd(struct cmdline_tokens* ptr_tok, int* ptr_infg, int* ptr_outfg);

/* functions wrappers with error check (omit the head comment) */

pid_t Fork(void);
void Execve(const char *filename, char *const argv[], char *const envp[]);
pid_t Wait(int *status);
pid_t Waitpid(pid_t pid, int *iptr, int options);
void Kill(pid_t pid, int signum);
void Setpgid(pid_t pid, pid_t pgid);
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
void Sigemptyset(sigset_t *set);
void Sigaddset(sigset_t *set, int signum);
int Open(const char *pathname, int flags, mode_t mode);
void Close(int fd);
int Dup(int fd);
int Dup2(int fd1, int fd2);

/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
    char c;
    char cmdline[MAXLINE];    /* cmdline for fgets */
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            usage();
            break;
        case 'v':             /* em+it additional diagnostic info */
            verbose = 1;
            break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
            break;
        default:
            usage();
        }
    }

    /* Install the signal handlers */

    /* These are the ones you will need to implement */
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */
    Signal(SIGTTIN, SIG_IGN);
    Signal(SIGTTOU, SIG_IGN);

    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler); 

    /* Initialize the job list */
    initjobs(job_list);

    /* Execute the shell's read/eval loop */
    while (1) {

        if (emit_prompt) {
            printf("%s", prompt);
            fflush(stdout);
        }
        if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
            app_error("fgets error");
        if (feof(stdin)) { 
            /* End of file (ctrl-d) */
            printf ("\n");
            fflush(stdout);
            fflush(stderr);
            exit(0);
        }
        
        /* Remove the trailing newline */
        cmdline[strlen(cmdline)-1] = '\0';
        
        /* Evaluate the command line */
        eval(cmdline);
        
        fflush(stdout);
        fflush(stdout);
    } 
    
    exit(0); /* control never reaches here */
}

/* 
 * open_file(ptr_tok, ptr_infg, ptr_outfg)
 * open the file according to ptr_tok,
 * and store file figure in ptr_infg and ptr_outfig 
 *
 */
void open_file(struct cmdline_tokens* ptr_tok, int* ptr_infg, int* ptr_outfg)
{
	if (ptr_tok->infile != NULL) {
        *ptr_infg = Open(ptr_tok->infile, O_RDONLY, 0);
        Dup2(*ptr_infg, STDIN_FILENO);
    }
    if (ptr_tok->outfile != NULL) {
        *ptr_outfg = Open(ptr_tok->outfile, O_RDWR, 0);
        Dup2(*ptr_outfg, STDOUT_FILENO);
    }
    return;
}

/* close_file(ptr_infg, ptr_ourfg) close I/O file ptr_fg points */
void close_file(int* ptr_infg, int* ptr_outfg)
{
	if (*ptr_infg!=-1)
        Close(*ptr_infg);
    if (*ptr_outfg!=-1)
        Close(*ptr_outfg);
	return;
}

/* set_signal() sets signal SIGCHLD, SIGTSTP, SIGINT in ptr_sig_set */
void set_signal(sigset_t* ptr_sig_set)
{
	Sigemptyset(ptr_sig_set);
	Sigaddset(ptr_sig_set, SIGCHLD);
	Sigaddset(ptr_sig_set, SIGINT);
	Sigaddset(ptr_sig_set, SIGTSTP);
	return;	
}


/* 
 * getjid(ptr_tok) get jid in cmdline_tokena ptr_tok points
 *
 * Get the number val in the string, maybe including the leading '%' 
 * What a pity that I don't know how to deal with leading '%' using
 * atoi() in <string.h>, so I have to implement it by my own. :(
 */
int getjid(struct cmdline_tokens* ptr_tok)
{
	long long val = 0, leading_val = '%' - '0'; /* the val of argv[] */
	int i, j, jid;
	for (i = 0; ptr_tok->argv[1][i] != '\0' && ptr_tok->argv[1][i] != ' '; i++)
		val = val * 10 + (ptr_tok->argv[1][i] - '0');
	for (j = 1; j < i; j++)
		leading_val *= 10; /* calculate the weight of leading '%', if there exists one */
	if (ptr_tok->argv[1][0]=='%')
        jid = val - leading_val; /* sub val of leading '%' from val */
    else
        jid = pid2jid(val);
    return jid;  
}

/* dojob() do job fg or bg */		            
void dojob(struct job_t* job, int bg)
{
	if (bg) {
    	printf("[%d] (%d) %s\n", job->jid, job->pid, job->cmdline);
        job->state = BG;
        Kill(-(job->pid), SIGCONT);
    }
    else {
    	job->state = FG;        
    	Kill(-(job->pid), SIGCONT);
    }
	return;
}

/* builtim_cmd() check if cmd is built in, and execute builtin command */
int builtin_cmd(struct cmdline_tokens* ptr_tok, int* ptr_infg, int* ptr_outfg)
{
	struct job_t *job;
	enum builtins_t cmd = ptr_tok->builtins;
	/* deal with built in command */
	if (cmd == BUILTIN_QUIT) { /* close the file and exit */
   		/* before quit tsh, dup close file  */
		close_file(ptr_infg, ptr_outfg);
		exit(0);
	}
	else if (cmd == BUILTIN_JOBS) { /* list the job */
		listjobs(job_list, STDOUT_FILENO);
		return 1;
	}
	else if (cmd == BUILTIN_BG) {
		int jid;
		jid = getjid(ptr_tok);
        job = getjobjid(job_list, jid);
        /* do fg or bg job */
        dojob(job, 1);
        return 1;
	} else if (cmd == BUILTIN_FG) {
		int jid;
		jid = getjid(ptr_tok);
        job = getjobjid(job_list, jid);
        /* do fg or bg job */
        dojob(job, 0);
        return 1;
	}
	return 0;
}

/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
 */
void eval(char *cmdline)
{
    int bg;              /* should the job run in bg or fg? */
    struct cmdline_tokens tok;

    /* Parse command line */
    bg = parseline(cmdline, &tok); 

    if (bg == -1) /* parsing error */
        return;
    if (tok.argv[0] == NULL) /* ignore empty lines */
        return;
	
	/* My complement begin */
	
	/* deal with I/O file */	    
	int fstdin, fstdout; /* for store STDIN_FILENO and STDOUT_FILENO to recover before quiting */
	int infg = -1, outfg = -1;
	struct job_t *job;
	pid_t pid,child_pid;
	sigset_t sig_set,empty_sig_set;
	
	/* store STDIN_FILENO and STDOUT_FILENO to recover before quiting */
    fstdin=Dup(STDIN_FILENO);
    fstdout=Dup(STDOUT_FILENO);
	
	/* open file */
	open_file(&tok, &infg, &outfg);
	
	/* set signal set */
	Sigemptyset(&empty_sig_set);
	set_signal(&sig_set);
	
	if (!builtin_cmd(&tok, &infg, &outfg))
	{
		/* block signal set */
		Sigprocmask(SIG_BLOCK, &sig_set, NULL);
		
		if ((pid = Fork()) == 0) /* child */
		{
			/* unblock signal set in child */
			Sigprocmask(SIG_UNBLOCK, &sig_set, NULL);
			
			/* set child another pidgroup */
			child_pid = getpid();
			Setpgid(child_pid, child_pid);
			
			Execve(tok.argv[0], tok.argv, environ);
			
		}
		else /* parent */
		{
			/* add job in the job list */
			/* I use signals to ensure that addjob be executed before CHLD to avoid race */
			addjob(job_list, pid, bg ? BG : FG, cmdline);
            job = getjobpid(job_list, pid);
			
			/* after add job to job list, unblock signal set in parent */
			Sigprocmask(SIG_UNBLOCK, &sig_set, NULL);
			
			if (!bg) /* fg work */
				while (pid == fgpid(job_list))
					sigsuspend(&empty_sig_set);
				/*
				 * Using functions Waitpid(pid, NULL, 0) is the wrong answer!
				 * It cost me much time to learn that there exists a race, when
				 * child process terminates before parent process, it will 
				 * cause a waitpid error! And it's difficult to avoid the very
				 * situation where fg job only changes its job state, too. 
				 * So I use (pid == fgpid(job_list)) instead.
				 */
			else /* bg work */
				printf("[%d] (%d) %s\n", job->jid, pid, job->cmdline);
				
		}
	}
	
	/* before quit tsh, dup fstd back to STD_FILENO  */
    Dup2(fstdin, STDIN_FILENO);
    Dup2(fstdout, STDOUT_FILENO);
    
	/* close I/O file */    
	close_file(&infg, &outfg);
	
	/* My complement end */

    return;
}

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Parameters:
 *   cmdline:  The command line, in the form:
 *
 *                command [arguments...] [< infile] [> oufile] [&]
 *
 *   tok:      Pointer to a cmdline_tokens structure. The elements of this
 *             structure will be populated with the parsed tokens. Characters 
 *             enclosed in single or double quotes are treated as a single
 *             argument. 
 * Returns:
 *   1:        if the user has requested a BG job
 *   0:        if the user has requested a FG job  
 *  -1:        if cmdline is incorrectly formatted
 * 
 * Note:       The string elements of tok (e.g., argv[], infile, outfile) 
 *             are statically allocated inside parseline() and will be 
 *             overwritten the next time this function is invoked.
 */
int parseline(const char *cmdline, struct cmdline_tokens *tok) 
{

    static char array[MAXLINE];          /* holds local copy of command line */
    const char delims[10] = " \t\r\n";   /* argument delimiters (white-space) */
    char *buf = array;                   /* ptr that traverses command line */
    char *next;                          /* ptr to the end of the current arg */
    char *endbuf;                        /* ptr to end of cmdline string */
    int is_bg;                           /* background job? */

    int parsing_state;                   /* indicates if the next token is the
                                            input or output file */

    if (cmdline == NULL) {
        (void) fprintf(stderr, "Error: command line is NULL\n");
        return -1;
    }

    (void) strncpy(buf, cmdline, MAXLINE);
    endbuf = buf + strlen(buf);

    tok->infile = NULL;
    tok->outfile = NULL;

    /* Build the argv list */
    parsing_state = ST_NORMAL;
    tok->argc = 0;

    while (buf < endbuf) {
        /* Skip the white-spaces */
        buf += strspn (buf, delims);
        if (buf >= endbuf) break;

        /* Check for I/O redirection specifiers */
        if (*buf == '<') {
            if (tok->infile) {
                (void) fprintf(stderr, "Error: Ambiguous I/O redirection\n");
                return -1;
            }
            parsing_state |= ST_INFILE;
            buf++;
            continue;
        }
        if (*buf == '>') {
            if (tok->outfile) {
                (void) fprintf(stderr, "Error: Ambiguous I/O redirection\n");
                return -1;
            }
            parsing_state |= ST_OUTFILE;
            buf ++;
            continue;
        }

        if (*buf == '\'' || *buf == '\"') {
            /* Detect quoted tokens */
            buf++;
            next = strchr (buf, *(buf-1));
        } else {
            /* Find next delimiter */
            next = buf + strcspn (buf, delims);
        }
        
        if (next == NULL) {
            /* Returned by strchr(); this means that the closing
               quote was not found. */
            (void) fprintf (stderr, "Error: unmatched %c.\n", *(buf-1));
            return -1;
        }

        /* Terminate the token */
        *next = '\0';

        /* Record the token as either the next argument or the i/o file */
        switch (parsing_state) {
        case ST_NORMAL:
            tok->argv[tok->argc++] = buf;
            break;
        case ST_INFILE:
            tok->infile = buf;
            break;
        case ST_OUTFILE:
            tok->outfile = buf;
            break;
        default:
            (void) fprintf(stderr, "Error: Ambiguous I/O redirection\n");
            return -1;
        }
        parsing_state = ST_NORMAL;

        /* Check if argv is full */
        if (tok->argc >= MAXARGS-1) break;

        buf = next + 1;
    }

    if (parsing_state != ST_NORMAL) {
        (void) fprintf(stderr,"Error: must provide file name for redirection\n");
        return -1;
    }

    /* The argument list must end with a NULL pointer */
    tok->argv[tok->argc] = NULL;

    if (tok->argc == 0)  /* ignore blank line */
        return 1;

    if (!strcmp(tok->argv[0], "quit")) {                 /* quit command */
        tok->builtins = BUILTIN_QUIT;
    } else if (!strcmp(tok->argv[0], "jobs")) {          /* jobs command */
        tok->builtins = BUILTIN_JOBS;
    } else if (!strcmp(tok->argv[0], "bg")) {            /* bg command */
        tok->builtins = BUILTIN_BG;
    } else if (!strcmp(tok->argv[0], "fg")) {            /* fg command */
        tok->builtins = BUILTIN_FG;
    } else {
        tok->builtins = BUILTIN_NONE;
    }

    /* Should the job run in the background? */
    if ((is_bg = (*tok->argv[tok->argc-1] == '&')) != 0)
        tok->argv[--tok->argc] = NULL;

    return is_bg;
}


/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP, SIGTSTP, SIGTTIN or SIGTTOU signal. The 
 *     handler reaps all available zombie children, but doesn't wait 
 *     for any other currently running children to terminate.  
 */
void sigchld_handler(int sig) 
{
    pid_t pid;
    int jid;
    int status;
    struct job_t* job;
    while ((pid = waitpid(-1, &status, WUNTRACED | WNOHANG)) > 0) /* reap as much child processes as possible */
    {
        if (WIFSTOPPED(status)) /* SIGTSTP */
        {
            jid = pid2jid(pid);
            if (jid != 0)
            {
            	/* print some information */
            	sio_puts("Job [");
            	sio_putl(jid);
            	sio_puts("] (");
            	sio_putl(pid);
            	sio_puts(") stopped by signal ");
            	sio_putl(WSTOPSIG(status));
            	sio_puts("\n");
            	/* 
            	 * This is the the sio version of printf, for printf is not concurrency safe,
            	 * printf("Job [%d] (%d) stopped by signal %d\n",jid, pid, WSTOPSIG(status));
            	 * It is dull and ugly, isn't it? I hope I had a function sio_printf() :(
            	 */
            	 
                job = getjobpid(job_list, pid);
                job->state = ST;
            }
        }
        else if (WIFSIGNALED(status)) /* SIGINT */
        {
            if (WTERMSIG(status) == SIGINT)
            {
                jid = pid2jid(pid);
                if (jid != 0)
                {
                	/* print some information */
                	sio_puts("Job [");
           	 		sio_putl(jid);
            		sio_puts("] (");
            		sio_putl(pid);
            		sio_puts(") terminated by signal ");
            		sio_putl(SIGINT);
            		sio_puts("\n");
            		/* 
            		 * This is the the sio version of printf, for printf is not concurrency safe,
            		 * printf("Job [%d] (%d) terminated by signal %d\n", jid, pid, SIGINT);
            	 	 * It is dull and ugly, isn't it? I hope I had a function sio_printf() :(
            	 	 */
            	 	 
                    deletejob(job_list, pid);
                }
            }
        }
        else /* exit normally */
            deletejob(job_list, pid);
    }
    return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) 
{
	int old_errorno = errno;
	int fg_pid = fgpid(job_list); /* get frontground process pid */
	if (fg_pid > 0)
		Kill(-fg_pid, sig);
	errno = old_errorno;
    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) 
{
	int old_errorno = errno;
	int fg_pid = fgpid(job_list); /* get frontground process pid */
	if (fg_pid > 0)
		Kill(-fg_pid, sig);
	errno = old_errorno;
    return;
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) 
{
    sio_error("Terminating after receipt of SIGQUIT signal\n");
}



/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *job_list) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
        clearjob(&job_list[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *job_list) 
{
    int i, max=0;

    for (i = 0; i < MAXJOBS; i++)
        if (job_list[i].jid > max)
            max = job_list[i].jid;
    return max;
}

/* addjob - Add a job to the job list */
int addjob(struct job_t *job_list, pid_t pid, int state, char *cmdline) 
{
    int i;

    if (pid < 1)
        return 0;

    for (i = 0; i < MAXJOBS; i++) {
        if (job_list[i].pid == 0) {
            job_list[i].pid = pid;
            job_list[i].state = state;
            job_list[i].jid = nextjid++;
            if (nextjid > MAXJOBS)
                nextjid = 1;
            strcpy(job_list[i].cmdline, cmdline);
            if(verbose){
                printf("Added job [%d] %d %s\n",
                       job_list[i].jid,
                       job_list[i].pid,
                       job_list[i].cmdline);
            }
            return 1;
        }
    }
    printf("Tried to create too many jobs\n");
    return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *job_list, pid_t pid) 
{
    int i;

    if (pid < 1)
        return 0;

    for (i = 0; i < MAXJOBS; i++) {
        if (job_list[i].pid == pid) {
            clearjob(&job_list[i]);
            nextjid = maxjid(job_list)+1;
            return 1;
        }
    }
    return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *job_list) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
        if (job_list[i].state == FG)
            return job_list[i].pid;
    return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *job_list, pid_t pid) {
    int i;

    if (pid < 1)
        return NULL;
    for (i = 0; i < MAXJOBS; i++)
        if (job_list[i].pid == pid)
            return &job_list[i];
    return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *job_list, int jid) 
{
    int i;

    if (jid < 1)
        return NULL;
    for (i = 0; i < MAXJOBS; i++)
        if (job_list[i].jid == jid)
            return &job_list[i];
    return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) 
{
    int i;

    if (pid < 1)
        return 0;
    for (i = 0; i < MAXJOBS; i++)
        if (job_list[i].pid == pid) {
            return job_list[i].jid;
        }
    return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *job_list, int output_fd) 
{
    int i;
    char buf[MAXLINE];

    for (i = 0; i < MAXJOBS; i++) {
        memset(buf, '\0', MAXLINE);
        if (job_list[i].pid != 0) {
            sprintf(buf, "[%d] (%d) ", job_list[i].jid, job_list[i].pid);
            if(write(output_fd, buf, strlen(buf)) < 0) {
                fprintf(stderr, "Error writing to output file\n");
                exit(1);
            }
            memset(buf, '\0', MAXLINE);
            switch (job_list[i].state) {
            case BG:
                sprintf(buf, "Running    ");
                break;
            case FG:
                sprintf(buf, "Foreground ");
                break;
            case ST:
                sprintf(buf, "Stopped    ");
                break;
            default:
                sprintf(buf, "listjobs: Internal error: job[%d].state=%d ",
                        i, job_list[i].state);
            }
            if(write(output_fd, buf, strlen(buf)) < 0) {
                fprintf(stderr, "Error writing to output file\n");
                exit(1);
            }
            memset(buf, '\0', MAXLINE);
            sprintf(buf, "%s\n", job_list[i].cmdline);
            if(write(output_fd, buf, strlen(buf)) < 0) {
                fprintf(stderr, "Error writing to output file\n");
                exit(1);
            }
        }
    }
}
/******************************
 * end job list helper routines
 ******************************/


/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) 
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * my_unix_error - unix-style error routine, but do not exit terminal
 */
void my_unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    return;
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

/* Private sio_functions */
/* sio_reverse - Reverse a string (from K&R) */
static void sio_reverse(char s[])
{
    int c, i, j;

    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* sio_ltoa - Convert long to base b string (from K&R) */
static void sio_ltoa(long v, char s[], int b) 
{
    int c, i = 0;
    
    do {  
        s[i++] = ((c = (v % b)) < 10)  ?  c + '0' : c - 10 + 'a';
    } while ((v /= b) > 0);
    s[i] = '\0';
    sio_reverse(s);
}

/* sio_strlen - Return length of string (from K&R) */
static size_t sio_strlen(char s[])
{
    int i = 0;

    while (s[i] != '\0')
        ++i;
    return i;
}

/* Public Sio functions */
ssize_t sio_puts(char s[]) /* Put string */
{
    return write(STDOUT_FILENO, s, sio_strlen(s));
}

ssize_t sio_putl(long v) /* Put long */
{
    char s[128];
    
    sio_ltoa(v, s, 10); /* Based on K&R itoa() */ 
    return sio_puts(s);
}

void sio_error(char s[]) /* Put error message and exit */
{
    sio_puts(s);
    _exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        my_unix_error("Signal error");
    return (old_action.sa_handler);
}

/* functions wrappers with error check*/

/* fork() wrapper */
pid_t Fork(void) 
{
	pid_t pid;
	
	if ((pid = fork()) < 0)
		my_unix_error("Fork error");
	return pid;
}

/* execve() wrapper */
void Execve(const char *filename, char *const argv[], char *const envp[]) 
{
	if (execve(filename, argv, envp) < 0)
		my_unix_error("Execve error");
	return;
}

/* wait() wrapper */
pid_t Wait(int *status) 
{
	pid_t pid;
	
	if ((pid  = wait(status)) < 0)
		my_unix_error("Wait error");
	return pid;
}

/* waitpid() wrapper */
pid_t Waitpid(pid_t pid, int *iptr, int options) 
{
	pid_t retpid;
	
	if ((retpid  = waitpid(pid, iptr, options)) < 0) 
		my_unix_error("Waitpid error");
	return retpid;
}

/* kill() wrapper */
void Kill(pid_t pid, int signum) 
{
	int retval;

	if ((retval = kill(pid, signum)) < 0)
		my_unix_error("Kill error");
}

/* setpgid() wrapper */
void Setpgid(pid_t pid, pid_t pgid) {
	if (setpgid(pid, pgid) < 0)
		my_unix_error("Setpgid error");
	return;
}

/* sigprocmask() wrapper */
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
	if (sigprocmask(how, set, oldset) < 0)
		my_unix_error("Sigprocmask error");
	return;
}

/* sigemptyset() wrapper */
void Sigemptyset(sigset_t *set)
{
	if (sigemptyset(set) < 0)
		my_unix_error("Sigemptyset error");
	return;
}

/* sigaddset() wrapper */
void Sigaddset(sigset_t *set, int signum)
{
	if (sigaddset(set, signum) < 0)
		my_unix_error("Sigaddset error");
	return;
}

/* open() wrapper */
int Open(const char *pathname, int flags, mode_t mode) 
{
	int retval;

	if ((retval = open(pathname, flags, mode))  < 0)
		my_unix_error("Open error");
	return retval;
}

/* close() wrapper */
void Close(int fd) 
{
	if (close(fd) < 0)
		my_unix_error("Close error");
	return;
}

/* dup2() wrapper */
int Dup2(int fd1, int fd2) 
{
	int retval;

	if ((retval = dup2(fd1, fd2)) < 0)
		my_unix_error("Dup2 error");
	return retval;
}

/* dup() wrapper */
int Dup(int fd) 
{
	int retval;

	if ((retval = dup(fd)) < 0)
		my_unix_error("Dup error");
	return retval;
}
