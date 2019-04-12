struct stat;
struct rtcdate;

// system calls
int fork(void);
//CS153 Lab1 Part(a)
//int exit(void) __attribute__((noreturn));
int exit(int status) __attribute__((noreturn));
//CS153 Lab1 Part(b)
//int wait(void);
int wait(int *status);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
//CS153 Lab1 Part(c)
int waitpid(int pid, int *status, int options);
//CS153 Lab2
int setpriority(int priority);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);