
       
typedef signed char __int8_t;

typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;
typedef long long __int64_t;
typedef unsigned long long __uint64_t;

typedef long __darwin_intptr_t;
typedef unsigned int __darwin_natural_t;

typedef int __darwin_ct_rune_t;

typedef union {
 char __mbstate8[128];
 long long _mbstateL;
} __mbstate_t;

typedef __mbstate_t __darwin_mbstate_t;
typedef long int __darwin_ptrdiff_t;

typedef long unsigned int __darwin_size_t;

typedef __builtin_va_list __darwin_va_list;

typedef int __darwin_wchar_t;
typedef __darwin_wchar_t __darwin_rune_t;
typedef int __darwin_wint_t;
typedef unsigned long __darwin_clock_t;
typedef __uint32_t __darwin_socklen_t;
typedef long __darwin_ssize_t;
typedef long __darwin_time_t;

struct __darwin_pthread_handler_rec
{
 void (*__routine)(void *);
 void *__arg;
 struct __darwin_pthread_handler_rec *__next;
};
struct _opaque_pthread_attr_t { long __sig; char __opaque[56]; };
struct _opaque_pthread_cond_t { long __sig; char __opaque[40]; };
struct _opaque_pthread_condattr_t { long __sig; char __opaque[8]; };
struct _opaque_pthread_mutex_t { long __sig; char __opaque[56]; };
struct _opaque_pthread_mutexattr_t { long __sig; char __opaque[8]; };
struct _opaque_pthread_once_t { long __sig; char __opaque[8]; };
struct _opaque_pthread_rwlock_t { long __sig; char __opaque[192]; };
struct _opaque_pthread_rwlockattr_t { long __sig; char __opaque[16]; };
struct _opaque_pthread_t { long __sig; struct __darwin_pthread_handler_rec *__cleanup_stack; char __opaque[1168]; };

typedef __int64_t __darwin_blkcnt_t;
typedef __int32_t __darwin_blksize_t;
typedef __int32_t __darwin_dev_t;
typedef unsigned int __darwin_fsblkcnt_t;
typedef unsigned int __darwin_fsfilcnt_t;
typedef __uint32_t __darwin_gid_t;
typedef __uint32_t __darwin_id_t;
typedef __uint64_t __darwin_ino64_t;

typedef __darwin_ino64_t __darwin_ino_t;

typedef __darwin_natural_t __darwin_mach_port_name_t;
typedef __darwin_mach_port_name_t __darwin_mach_port_t;
typedef __uint16_t __darwin_mode_t;
typedef __int64_t __darwin_off_t;
typedef __int32_t __darwin_pid_t;
typedef struct _opaque_pthread_attr_t
   __darwin_pthread_attr_t;
typedef struct _opaque_pthread_cond_t
   __darwin_pthread_cond_t;
typedef struct _opaque_pthread_condattr_t
   __darwin_pthread_condattr_t;
typedef unsigned long __darwin_pthread_key_t;
typedef struct _opaque_pthread_mutex_t
   __darwin_pthread_mutex_t;
typedef struct _opaque_pthread_mutexattr_t
   __darwin_pthread_mutexattr_t;
typedef struct _opaque_pthread_once_t
   __darwin_pthread_once_t;
typedef struct _opaque_pthread_rwlock_t
   __darwin_pthread_rwlock_t;
typedef struct _opaque_pthread_rwlockattr_t
   __darwin_pthread_rwlockattr_t;
typedef struct _opaque_pthread_t
   *__darwin_pthread_t;
typedef __uint32_t __darwin_sigset_t;
typedef __int32_t __darwin_suseconds_t;
typedef __uint32_t __darwin_uid_t;
typedef __uint32_t __darwin_useconds_t;
typedef unsigned char __darwin_uuid_t[16];
typedef char __darwin_uuid_string_t[37];
typedef int __darwin_nl_item;
typedef int __darwin_wctrans_t;

typedef __uint32_t __darwin_wctype_t;

struct accessx_descriptor {
 unsigned int ad_name_offset;
 int ad_flags;
 int ad_pad[2];
};

typedef __darwin_dev_t dev_t;

typedef __darwin_gid_t gid_t;
typedef __darwin_intptr_t intptr_t;

typedef __darwin_mode_t mode_t;

typedef __darwin_off_t off_t;
typedef __darwin_pid_t pid_t;
typedef __darwin_size_t size_t;
typedef __darwin_ssize_t ssize_t;
typedef __darwin_uid_t uid_t;
typedef __darwin_useconds_t useconds_t;

typedef __darwin_uuid_t uuid_t;

extern "C" {

void _exit(int) __attribute__((__noreturn__));
int access(const char *, int);
unsigned int
  alarm(unsigned int);
int chdir(const char *);
int chown(const char *, uid_t, gid_t);
int close(int) __asm("_" "close" );
size_t confstr(int, char *, size_t) __asm("_" "confstr" );
char *crypt(const char *, const char *);
char *ctermid(char *);
int dup(int);
int dup2(int, int);

void encrypt(char *, int) __asm("_" "encrypt" );

int execl(const char *, const char *, ...);
int execle(const char *, const char *, ...);
int execlp(const char *, const char *, ...);
int execv(const char *, char * const *);
int execve(const char *, char * const *, char * const *);
int execvp(const char *, char * const *);
int fchown(int, uid_t, gid_t);
int fchdir(int);
pid_t fork(void);
long fpathconf(int, int);
int fsync(int) __asm("_" "fsync" );
int ftruncate(int, off_t);
char *getcwd(char *, size_t);
gid_t getegid(void);
uid_t geteuid(void);
gid_t getgid(void);

int getgroups(int, gid_t []);

long gethostid(void);
int gethostname(char *, size_t);
char *getlogin(void);
int getlogin_r(char *, size_t);
int getopt(int, char * const [], const char *) __asm("_" "getopt" );
pid_t getpgid(pid_t);
pid_t getpgrp(void);
pid_t getpid(void);
pid_t getppid(void);
pid_t getsid(pid_t);
uid_t getuid(void);
char *getwd(char *);
int isatty(int);
int lchown(const char *, uid_t, gid_t) __asm("_" "lchown" );
int link(const char *, const char *);
int lockf(int, int, off_t) __asm("_" "lockf" );
off_t lseek(int, off_t, int);
int nice(int) __asm("_" "nice" );
long pathconf(const char *, int);
int pause(void) __asm("_" "pause" );
int pipe(int [2]);
ssize_t pread(int, void *, size_t, off_t) __asm("_" "pread" );
ssize_t pwrite(int, const void *, size_t, off_t) __asm("_" "pwrite" );
ssize_t read(int, void *, size_t) __asm("_" "read" );
ssize_t readlink(const char * , char * , size_t);
int rmdir(const char *);
int setegid(gid_t);
int seteuid(uid_t);
int setgid(gid_t);
int setpgid(pid_t, pid_t);

pid_t setpgrp(void) __asm("_" "setpgrp" );

int setregid(gid_t, gid_t) __asm("_" "setregid" );
int setreuid(uid_t, uid_t) __asm("_" "setreuid" );
pid_t setsid(void);
int setuid(uid_t);
unsigned int
  sleep(unsigned int) __asm("_" "sleep" );
void swab(const void * , void * , ssize_t);
int symlink(const char *, const char *);
void sync(void);
long sysconf(int);
pid_t tcgetpgrp(int);
int tcsetpgrp(int, pid_t);
int truncate(const char *, off_t);
char *ttyname(int);

int ttyname_r(int, char *, size_t) __asm("_" "ttyname_r" );

useconds_t
  ualarm(useconds_t, useconds_t);
int unlink(const char *);
int usleep(useconds_t) __asm("_" "usleep" );
pid_t vfork(void);
ssize_t write(int, const void *, size_t) __asm("_" "write" );

extern char *optarg;
extern int optind, opterr, optopt;

struct timespec
{
 __darwin_time_t tv_sec;
 long tv_nsec;
};

struct timeval
{
 __darwin_time_t tv_sec;
 __darwin_suseconds_t tv_usec;
};

extern "C" {
typedef struct fd_set {
 __int32_t fds_bits[((((1024) % ((sizeof(__int32_t) * 8))) == 0) ? ((1024) / ((sizeof(__int32_t) * 8))) : (((1024) / ((sizeof(__int32_t) * 8))) + 1))];
} fd_set;
}
static inline int
__darwin_fd_isset(int _n, const struct fd_set *_p)
{
 return (_p->fds_bits[_n/(sizeof(__int32_t) * 8)] & (1<<(_n % (sizeof(__int32_t) * 8))));
}
typedef __darwin_time_t time_t;
typedef __darwin_suseconds_t suseconds_t;
typedef __darwin_sigset_t sigset_t;

extern "C" {
int pselect(int, fd_set * , fd_set * ,
  fd_set * , const struct timespec * ,
  const sigset_t * )
  __asm("_" "pselect" "$1050")
  ;
int select(int, fd_set * , fd_set * ,
  fd_set * , struct timeval * )
  __asm("_" "select" "$1050")
  ;
}
void _Exit(int) __attribute__((__noreturn__));
int accessx_np(const struct accessx_descriptor *, size_t, int *, uid_t);
int acct(const char *);
int add_profil(char *, size_t, unsigned long, unsigned int);
void *brk(const void *);
int chroot(const char *);
void endusershell(void);
int execvP(const char *, const char *, char * const *);
char *fflagstostr(unsigned long);
int getdtablesize(void);
int getdomainname(char *, int);
int getgrouplist(const char *, int, int *, int *);
int gethostuuid(uuid_t, const struct timespec *);
mode_t getmode(const void *, mode_t);
int getpagesize(void) __attribute__((__const__));
char *getpass(const char *);
int getpeereid(int, uid_t *, gid_t *);
int getpgid(pid_t _pid);
int getsgroups_np(int *, uuid_t);
int getsid(pid_t _pid);
char *getusershell(void);
int getwgroups_np(int *, uuid_t);
int initgroups(const char *, int);
int iruserok(unsigned long, int, const char *, const char *);
int iruserok_sa(const void *, int, int, const char *, const char *);
int issetugid(void);
char *mkdtemp(char *);
int mknod(const char *, mode_t, dev_t);
int mkstemp(char *);
int mkstemps(char *, int);
char *mktemp(char *);
int nfssvc(int, void *);
int profil(char *, size_t, unsigned long, unsigned int);
int pthread_setugid_np(uid_t, gid_t);
int pthread_getugid_np( uid_t *, gid_t *);
int rcmd(char **, int, const char *, const char *, const char *, int *);
int rcmd_af(char **, int, const char *, const char *, const char *, int *,
  int);
int reboot(int);
int revoke(const char *);
int rresvport(int *);
int rresvport_af(int *, int);
int ruserok(const char *, int, const char *, const char *);
void *sbrk(int);
int setdomainname(const char *, int);
int setgroups(int, const gid_t *);
void sethostid(long);
int sethostname(const char *, int);

void setkey(const char *) __asm("_" "setkey" );

int setlogin(const char *);
void *setmode(const char *) __asm("_" "setmode" );
int setrgid(gid_t);
int setruid(uid_t);
int setsgroups_np(int, const uuid_t);
void setusershell(void);
int setwgroups_np(int, const uuid_t);
int strtofflags(char **, unsigned long *, unsigned long *);
int swapon(const char *);
int syscall(int, ...);
int ttyslot(void);
int undelete(const char *);
int unwhiteout(const char *);
void *valloc(size_t);

extern char *suboptarg;
int getsubopt(char **, char * const *, char **);

int fgetattrlist(int,void*,void*,size_t,unsigned int);
int fsetattrlist(int,void*,void*,size_t,unsigned int);
int getattrlist(const char*,void*,void*,size_t,unsigned int) __asm("_" "getattrlist" );
int setattrlist(const char*,void*,void*,size_t,unsigned int) __asm("_" "setattrlist" );
int exchangedata(const char*,const char*,unsigned int);
int getdirentriesattr(int,void*,void*,size_t,unsigned int*,unsigned int*,unsigned int*,unsigned int);

struct fssearchblock;
struct searchstate;

int searchfs(const char *, struct fssearchblock *, unsigned long *, unsigned int, unsigned int, struct searchstate *);
int fsctl(const char *,unsigned long,void*,unsigned int);
int ffsctl(int,unsigned long,void*,unsigned int);

extern int optreset;
}

extern "C" {

extern void __dtrace_probe$cxa_runtime$cxa_exception_rethrow$v1(void);
extern int __dtrace_isenabled$cxa_runtime$cxa_exception_rethrow$v1(void);
extern void __dtrace_probe$cxa_runtime$cxa_exception_throw$v1$766f6964202a(void *);
extern int __dtrace_isenabled$cxa_runtime$cxa_exception_throw$v1(void);
}
namespace std {

}

       

typedef long int ptrdiff_t;
namespace std {

  using ::ptrdiff_t;
  using ::size_t;

}
typedef enum {
 P_ALL,
 P_PID,
 P_PGID
} idtype_t;

typedef __darwin_id_t id_t;

typedef int sig_atomic_t;
struct __darwin_i386_thread_state
{
    unsigned int __eax;
    unsigned int __ebx;
    unsigned int __ecx;
    unsigned int __edx;
    unsigned int __edi;
    unsigned int __esi;
    unsigned int __ebp;
    unsigned int __esp;
    unsigned int __ss;
    unsigned int __eflags;
    unsigned int __eip;
    unsigned int __cs;
    unsigned int __ds;
    unsigned int __es;
    unsigned int __fs;
    unsigned int __gs;
};

struct __darwin_fp_control
{
    unsigned short __invalid :1,
        __denorm :1,
    __zdiv :1,
    __ovrfl :1,
    __undfl :1,
    __precis :1,
      :2,
    __pc :2,

    __rc :2,
             :1,
      :3;
};
typedef struct __darwin_fp_control __darwin_fp_control_t;

struct __darwin_fp_status
{
    unsigned short __invalid :1,
        __denorm :1,
    __zdiv :1,
    __ovrfl :1,
    __undfl :1,
    __precis :1,
    __stkflt :1,
    __errsumm :1,
    __c0 :1,
    __c1 :1,
    __c2 :1,
    __tos :3,
    __c3 :1,
    __busy :1;
};
typedef struct __darwin_fp_status __darwin_fp_status_t;

struct __darwin_mmst_reg
{
 char __mmst_reg[10];
 char __mmst_rsrv[6];
};

struct __darwin_xmm_reg
{
 char __xmm_reg[16];
};

struct __darwin_i386_float_state
{
 int __fpu_reserved[2];
 struct __darwin_fp_control __fpu_fcw;
 struct __darwin_fp_status __fpu_fsw;
 __uint8_t __fpu_ftw;
 __uint8_t __fpu_rsrv1;
 __uint16_t __fpu_fop;
 __uint32_t __fpu_ip;
 __uint16_t __fpu_cs;
 __uint16_t __fpu_rsrv2;
 __uint32_t __fpu_dp;
 __uint16_t __fpu_ds;
 __uint16_t __fpu_rsrv3;
 __uint32_t __fpu_mxcsr;
 __uint32_t __fpu_mxcsrmask;
 struct __darwin_mmst_reg __fpu_stmm0;
 struct __darwin_mmst_reg __fpu_stmm1;
 struct __darwin_mmst_reg __fpu_stmm2;
 struct __darwin_mmst_reg __fpu_stmm3;
 struct __darwin_mmst_reg __fpu_stmm4;
 struct __darwin_mmst_reg __fpu_stmm5;
 struct __darwin_mmst_reg __fpu_stmm6;
 struct __darwin_mmst_reg __fpu_stmm7;
 struct __darwin_xmm_reg __fpu_xmm0;
 struct __darwin_xmm_reg __fpu_xmm1;
 struct __darwin_xmm_reg __fpu_xmm2;
 struct __darwin_xmm_reg __fpu_xmm3;
 struct __darwin_xmm_reg __fpu_xmm4;
 struct __darwin_xmm_reg __fpu_xmm5;
 struct __darwin_xmm_reg __fpu_xmm6;
 struct __darwin_xmm_reg __fpu_xmm7;
 char __fpu_rsrv4[14*16];
 int __fpu_reserved1;
};

struct __darwin_i386_exception_state
{
    unsigned int __trapno;
    unsigned int __err;
    unsigned int __faultvaddr;
};

struct __darwin_x86_debug_state32
{
 unsigned int __dr0;
 unsigned int __dr1;
 unsigned int __dr2;
 unsigned int __dr3;
 unsigned int __dr4;
 unsigned int __dr5;
 unsigned int __dr6;
 unsigned int __dr7;
};

struct __darwin_x86_thread_state64
{
 __uint64_t __rax;
 __uint64_t __rbx;
 __uint64_t __rcx;
 __uint64_t __rdx;
 __uint64_t __rdi;
 __uint64_t __rsi;
 __uint64_t __rbp;
 __uint64_t __rsp;
 __uint64_t __r8;
 __uint64_t __r9;
 __uint64_t __r10;
 __uint64_t __r11;
 __uint64_t __r12;
 __uint64_t __r13;
 __uint64_t __r14;
 __uint64_t __r15;
 __uint64_t __rip;
 __uint64_t __rflags;
 __uint64_t __cs;
 __uint64_t __fs;
 __uint64_t __gs;
};

struct __darwin_x86_float_state64
{
 int __fpu_reserved[2];
 struct __darwin_fp_control __fpu_fcw;
 struct __darwin_fp_status __fpu_fsw;
 __uint8_t __fpu_ftw;
 __uint8_t __fpu_rsrv1;
 __uint16_t __fpu_fop;
 __uint32_t __fpu_ip;
 __uint16_t __fpu_cs;

 __uint16_t __fpu_rsrv2;
 __uint32_t __fpu_dp;
 __uint16_t __fpu_ds;

 __uint16_t __fpu_rsrv3;
 __uint32_t __fpu_mxcsr;
 __uint32_t __fpu_mxcsrmask;
 struct __darwin_mmst_reg __fpu_stmm0;
 struct __darwin_mmst_reg __fpu_stmm1;
 struct __darwin_mmst_reg __fpu_stmm2;
 struct __darwin_mmst_reg __fpu_stmm3;
 struct __darwin_mmst_reg __fpu_stmm4;
 struct __darwin_mmst_reg __fpu_stmm5;
 struct __darwin_mmst_reg __fpu_stmm6;
 struct __darwin_mmst_reg __fpu_stmm7;
 struct __darwin_xmm_reg __fpu_xmm0;
 struct __darwin_xmm_reg __fpu_xmm1;
 struct __darwin_xmm_reg __fpu_xmm2;
 struct __darwin_xmm_reg __fpu_xmm3;
 struct __darwin_xmm_reg __fpu_xmm4;
 struct __darwin_xmm_reg __fpu_xmm5;
 struct __darwin_xmm_reg __fpu_xmm6;
 struct __darwin_xmm_reg __fpu_xmm7;
 struct __darwin_xmm_reg __fpu_xmm8;
 struct __darwin_xmm_reg __fpu_xmm9;
 struct __darwin_xmm_reg __fpu_xmm10;
 struct __darwin_xmm_reg __fpu_xmm11;
 struct __darwin_xmm_reg __fpu_xmm12;
 struct __darwin_xmm_reg __fpu_xmm13;
 struct __darwin_xmm_reg __fpu_xmm14;
 struct __darwin_xmm_reg __fpu_xmm15;
 char __fpu_rsrv4[6*16];
 int __fpu_reserved1;
};

struct __darwin_x86_exception_state64
{
    unsigned int __trapno;
    unsigned int __err;
    __uint64_t __faultvaddr;
};

struct __darwin_x86_debug_state64
{
 __uint64_t __dr0;
 __uint64_t __dr1;
 __uint64_t __dr2;
 __uint64_t __dr3;
 __uint64_t __dr4;
 __uint64_t __dr5;
 __uint64_t __dr6;
 __uint64_t __dr7;
};
struct __darwin_mcontext32
{
 struct __darwin_i386_exception_state __es;
 struct __darwin_i386_thread_state __ss;
 struct __darwin_i386_float_state __fs;
};

struct __darwin_mcontext64
{
 struct __darwin_x86_exception_state64 __es;
 struct __darwin_x86_thread_state64 __ss;
 struct __darwin_x86_float_state64 __fs;
};

typedef struct __darwin_mcontext64 *mcontext_t;

struct __darwin_sigaltstack
{
 void *ss_sp;
 __darwin_size_t ss_size;
 int ss_flags;
};

struct __darwin_ucontext
{
 int uc_onstack;
 __darwin_sigset_t uc_sigmask;
 struct __darwin_sigaltstack uc_stack;
 struct __darwin_ucontext *uc_link;
 __darwin_size_t uc_mcsize;
 struct __darwin_mcontext64 *uc_mcontext;

};

typedef struct __darwin_sigaltstack stack_t;

typedef struct __darwin_ucontext ucontext_t;
typedef __darwin_pthread_attr_t pthread_attr_t;

union sigval {

 int sival_int;
 void *sival_ptr;
};

struct sigevent {
 int sigev_notify;
 int sigev_signo;
 union sigval sigev_value;
 void (*sigev_notify_function)(union sigval);
 pthread_attr_t *sigev_notify_attributes;
};
typedef struct __siginfo {
 int si_signo;
 int si_errno;
 int si_code;
 pid_t si_pid;
 uid_t si_uid;
 int si_status;
 void *si_addr;
 union sigval si_value;
 long si_band;
 unsigned long __pad[7];
} siginfo_t;

union __sigaction_u {
 void (*__sa_handler)(int);
 void (*__sa_sigaction)(int, struct __siginfo *,
         void *);
};
struct __sigaction {
 union __sigaction_u __sigaction_u;
 void (*sa_tramp)(void *, int, int, siginfo_t *, void *);
 sigset_t sa_mask;
 int sa_flags;
};
struct sigaction {
 union __sigaction_u __sigaction_u;
 sigset_t sa_mask;
 int sa_flags;
};

typedef void (*sig_t)(int);

struct sigvec {
 void (*sv_handler)(int);
 int sv_mask;
 int sv_flags;
};

struct sigstack {
 char *ss_sp;
 int ss_onstack;
};

extern "C" {
void (*signal(int, void (*)(int)))(int);
}
typedef __uint64_t rlim_t;

struct rusage {
 struct timeval ru_utime;
 struct timeval ru_stime;

 long ru_maxrss;

 long ru_ixrss;
 long ru_idrss;
 long ru_isrss;
 long ru_minflt;
 long ru_majflt;
 long ru_nswap;
 long ru_inblock;
 long ru_oublock;
 long ru_msgsnd;
 long ru_msgrcv;
 long ru_nsignals;
 long ru_nvcsw;
 long ru_nivcsw;
};

struct rlimit {
 rlim_t rlim_cur;
 rlim_t rlim_max;
};

extern "C" {
int getpriority(int, id_t);

int getiopolicy_np(int, int);

int getrlimit(int, struct rlimit *) __asm("_" "getrlimit" );
int getrusage(int, struct rusage *);
int setpriority(int, id_t, int);

int setiopolicy_np(int, int, int);

int setrlimit(int, const struct rlimit *) __asm("_" "setrlimit" );
}
static inline
__uint16_t
_OSSwapInt16(
    __uint16_t _data
)
{
    return ((_data << 8) | (_data >> 8));
}

static inline
__uint32_t
_OSSwapInt32(
    __uint32_t _data
)
{

    return __builtin_bswap32(_data);
}
static inline
__uint64_t
_OSSwapInt64(
    __uint64_t _data
)
{
    return __builtin_bswap64(_data);
}
union wait {
 int w_status;

 struct {

  unsigned int w_Termsig:7,
    w_Coredump:1,
    w_Retcode:8,
    w_Filler:16;

 } w_T;

 struct {

  unsigned int w_Stopval:8,
    w_Stopsig:8,
    w_Filler:16;
 } w_S;
};

extern "C" {
pid_t wait(int *) __asm("_" "wait" );
pid_t waitpid(pid_t, int *, int) __asm("_" "waitpid" );

int waitid(idtype_t, id_t, siginfo_t *, int) __asm("_" "waitid" );
pid_t wait3(int *, int, struct rusage *);
pid_t wait4(pid_t, int *, int, struct rusage *);

}
extern "C" {
void *alloca(size_t);
}
typedef __darwin_ct_rune_t ct_rune_t;
typedef __darwin_rune_t rune_t;

typedef struct {
 int quot;
 int rem;
} div_t;

typedef struct {
 long quot;
 long rem;
} ldiv_t;
typedef struct {
 long long quot;
 long long rem;
} lldiv_t;

extern int __mb_cur_max;

extern "C" {
void abort(void) __attribute__((__noreturn__));
int abs(int) __attribute__((__const__));
int atexit(void (*)(void));
double atof(const char *);
int atoi(const char *);
long atol(const char *);

long long
  atoll(const char *);

void *bsearch(const void *, const void *, size_t,
     size_t, int (*)(const void *, const void *));
void *calloc(size_t, size_t);
div_t div(int, int) __attribute__((__const__));
void exit(int) __attribute__((__noreturn__));
void free(void *);
char *getenv(const char *);
long labs(long) __attribute__((__const__));
ldiv_t ldiv(long, long) __attribute__((__const__));

long long
  llabs(long long);
lldiv_t lldiv(long long, long long);

void *malloc(size_t);
int mblen(const char *, size_t);
size_t mbstowcs(wchar_t * , const char * , size_t);
int mbtowc(wchar_t * , const char * , size_t);
int posix_memalign(void **, size_t, size_t);
void qsort(void *, size_t, size_t,
     int (*)(const void *, const void *));
int rand(void);
void *realloc(void *, size_t);
void srand(unsigned);
double strtod(const char *, char **) __asm("_" "strtod" );
float strtof(const char *, char **) __asm("_" "strtof" );
long strtol(const char *, char **, int);
long double
  strtold(const char *, char **) ;

long long
  strtoll(const char *, char **, int);

unsigned long
  strtoul(const char *, char **, int);

unsigned long long
  strtoull(const char *, char **, int);

int system(const char *) __asm("_" "system" );
size_t wcstombs(char * , const wchar_t * , size_t);
int wctomb(char *, wchar_t);
void _Exit(int) __attribute__((__noreturn__));
long a64l(const char *);
double drand48(void);
char *ecvt(double, int, int *, int *);
double erand48(unsigned short[3]);
char *fcvt(double, int, int *, int *);
char *gcvt(double, int, char *);
int getsubopt(char **, char * const *, char **);
int grantpt(int);

char *initstate(unsigned, char *, size_t);

long jrand48(unsigned short[3]);
char *l64a(long);
void lcong48(unsigned short[7]);
long lrand48(void);
char *mktemp(char *);
int mkstemp(char *);
long mrand48(void);
long nrand48(unsigned short[3]);
int posix_openpt(int);
char *ptsname(int);
int putenv(char *) __asm("_" "putenv" );
long random(void);
int rand_r(unsigned *);

char *realpath(const char * , char * ) __asm("_" "realpath" "$DARWIN_EXTSN");

unsigned short
 *seed48(unsigned short[3]);
int setenv(const char *, const char *, int) __asm("_" "setenv" );

void setkey(const char *) __asm("_" "setkey" );

char *setstate(const char *);
void srand48(long);

void srandom(unsigned);

int unlockpt(int);

int unsetenv(const char *) __asm("_" "unsetenv" );

typedef signed char int8_t;

typedef unsigned char u_int8_t;
typedef short int16_t;

typedef unsigned short u_int16_t;
typedef int int32_t;

typedef unsigned int u_int32_t;
typedef long long int64_t;

typedef unsigned long long u_int64_t;
typedef int64_t register_t;

typedef unsigned long uintptr_t;
typedef u_int64_t user_addr_t;
typedef u_int64_t user_size_t;
typedef int64_t user_ssize_t;
typedef int64_t user_long_t;
typedef u_int64_t user_ulong_t;
typedef int64_t user_time_t;
typedef int64_t user_off_t;

typedef u_int64_t syscall_arg_t;

u_int32_t
  arc4random(void);
void arc4random_addrandom(unsigned char *dat, int datlen);
void arc4random_stir(void);

int atexit_b(void (^)(void));
void *bsearch_b(const void *, const void *, size_t,
     size_t, int (^)(const void *, const void *));

char *cgetcap(char *, const char *, int);
int cgetclose(void);
int cgetent(char **, char **, const char *);
int cgetfirst(char **, char **);
int cgetmatch(const char *, const char *);
int cgetnext(char **, char **);
int cgetnum(char *, const char *, long *);
int cgetset(const char *);
int cgetstr(char *, const char *, char **);
int cgetustr(char *, const char *, char **);

int daemon(int, int) __asm("_" "daemon" "$1050") __attribute__((deprecated,visibility("default")));
char *devname(dev_t, mode_t);
char *devname_r(dev_t, mode_t, char *buf, int len);
char *getbsize(int *, long *);
int getloadavg(double [], int);
const char
 *getprogname(void);

int heapsort(void *, size_t, size_t,
     int (*)(const void *, const void *));

int heapsort_b(void *, size_t, size_t,
     int (^)(const void *, const void *));

int mergesort(void *, size_t, size_t,
     int (*)(const void *, const void *));

int mergesort_b(void *, size_t, size_t,
     int (^)(const void *, const void *));

void psort(void *, size_t, size_t,
     int (*)(const void *, const void *));

void psort_b(void *, size_t, size_t,
     int (^)(const void *, const void *));

void psort_r(void *, size_t, size_t, void *,
     int (*)(void *, const void *, const void *));

void qsort_b(void *, size_t, size_t,
     int (^)(const void *, const void *));

void qsort_r(void *, size_t, size_t, void *,
     int (*)(void *, const void *, const void *));
int radixsort(const unsigned char **, int, const unsigned char *,
     unsigned);
void setprogname(const char *);
int sradixsort(const unsigned char **, int, const unsigned char *,
     unsigned);
void sranddev(void);
void srandomdev(void);
void *reallocf(void *, size_t);

long long
  strtoq(const char *, char **, int);
unsigned long long
  strtouq(const char *, char **, int);

extern char *suboptarg;
void *valloc(size_t);
}
namespace std {

  using ::div_t;
  using ::ldiv_t;

  using ::abort;
  using ::abs;
  using ::atexit;
  using ::atof;
  using ::atoi;
  using ::atol;
  using ::bsearch;
  using ::calloc;
  using ::div;
  using ::exit;
  using ::free;
  using ::getenv;
  using ::labs;
  using ::ldiv;
  using ::malloc;

  using ::mblen;
  using ::mbstowcs;
  using ::mbtowc;

  using ::qsort;
  using ::rand;
  using ::realloc;
  using ::srand;
  using ::strtod;
  using ::strtol;
  using ::strtoul;
  using ::system;

  using ::wcstombs;
  using ::wctomb;
  inline long
  abs(long __i) { return labs(__i); }

  inline ldiv_t
  div(long __i, long __j) { return ldiv(__i, __j); }

}

namespace __gnu_cxx {
  using ::lldiv_t;

  using ::_Exit;
  inline long long
  abs(long long __x) { return __x >= 0 ? __x : -__x; }
  using ::llabs;

  inline lldiv_t
  div(long long __n, long long __d)
  { lldiv_t __q; __q.quot = __n / __d; __q.rem = __n % __d; return __q; }

  using ::lldiv;

  using ::atoll;
  using ::strtoll;
  using ::strtoull;

  using ::strtof;
  using ::strtold;

}

namespace std {
  using ::__gnu_cxx::lldiv_t;

  using ::__gnu_cxx::_Exit;
  using ::__gnu_cxx::abs;

  using ::__gnu_cxx::llabs;
  using ::__gnu_cxx::div;
  using ::__gnu_cxx::lldiv;

  using ::__gnu_cxx::atoll;
  using ::__gnu_cxx::strtof;
  using ::__gnu_cxx::strtoll;
  using ::__gnu_cxx::strtoull;
  using ::__gnu_cxx::strtold;

}
       

       
namespace std {

  template<typename _Alloc>
    class allocator;

  template<class _CharT>
    struct char_traits;

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
           typename _Alloc = allocator<_CharT> >
    class basic_string;

  template<> struct char_traits<char>;

  typedef basic_string<char> string;
  template<> struct char_traits<wchar_t>;

  typedef basic_string<wchar_t> wstring;
}

       
       
extern "C" {
void *memchr(const void *, int, size_t);
int memcmp(const void *, const void *, size_t);
void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);

char *stpcpy(char *, const char *);
char *strcasestr(const char *, const char *);

char *strcat(char *, const char *);
char *strchr(const char *, int);
int strcmp(const char *, const char *);
int strcoll(const char *, const char *);
char *strcpy(char *, const char *);
size_t strcspn(const char *, const char *);
char *strerror(int) __asm("_" "strerror" );
int strerror_r(int, char *, size_t);
size_t strlen(const char *);
char *strncat(char *, const char *, size_t);
int strncmp(const char *, const char *, size_t);
char *strncpy(char *, const char *, size_t);

char *strnstr(const char *, const char *, size_t);

char *strpbrk(const char *, const char *);
char *strrchr(const char *, int);
size_t strspn(const char *, const char *);
char *strstr(const char *, const char *);
char *strtok(char *, const char *);
size_t strxfrm(char *, const char *, size_t);

void *memccpy(void *, const void *, int, size_t);
char *strtok_r(char *, const char *, char **);
char *strdup(const char *);

int bcmp(const void *, const void *, size_t);
void bcopy(const void *, void *, size_t);
void bzero(void *, size_t);
int ffs(int);
int ffsl(long);
int fls(int);
int flsl(long);
char *index(const char *, int);
void memset_pattern4(void *, const void *, size_t);
void memset_pattern8(void *, const void *, size_t);
void memset_pattern16(void *, const void *, size_t);
char *rindex(const char *, int);
int strcasecmp(const char *, const char *);
size_t strlcat(char *, const char *, size_t);
size_t strlcpy(char *, const char *, size_t);
void strmode(int, char *);
int strncasecmp(const char *, const char *, size_t);
char *strsep(char **, const char *);
char *strsignal(int sig);
void swab(const void * , void * , ssize_t);
}
namespace std {

  using ::memcpy;
  using ::memmove;
  using ::strcpy;
  using ::strncpy;
  using ::strcat;
  using ::strncat;
  using ::memcmp;
  using ::strcmp;
  using ::strcoll;
  using ::strncmp;
  using ::strxfrm;
  using ::strcspn;
  using ::strspn;
  using ::strtok;
  using ::memset;
  using ::strerror;
  using ::strlen;

  using ::memchr;

  inline void*
  memchr(void* __p, int __c, size_t __n)
  { return memchr(const_cast<const void*>(__p), __c, __n); }

  using ::strchr;

  inline char*
  strchr(char* __s1, int __n)
  { return __builtin_strchr(const_cast<const char*>(__s1), __n); }

  using ::strpbrk;

  inline char*
  strpbrk(char* __s1, const char* __s2)
  { return __builtin_strpbrk(const_cast<const char*>(__s1), __s2); }

  using ::strrchr;

  inline char*
  strrchr(char* __s1, int __n)
  { return __builtin_strrchr(const_cast<const char*>(__s1), __n); }

  using ::strstr;

  inline char*
  strstr(char* __s1, const char* __s2)
  { return __builtin_strstr(const_cast<const char*>(__s1), __s2); }

}


struct lconv {
 char *decimal_point;
 char *thousands_sep;
 char *grouping;
 char *int_curr_symbol;
 char *currency_symbol;
 char *mon_decimal_point;
 char *mon_thousands_sep;
 char *mon_grouping;
 char *positive_sign;
 char *negative_sign;
 char int_frac_digits;
 char frac_digits;
 char p_cs_precedes;
 char p_sep_by_space;
 char n_cs_precedes;
 char n_sep_by_space;
 char p_sign_posn;
 char n_sign_posn;
 char int_p_cs_precedes;
 char int_n_cs_precedes;
 char int_p_sep_by_space;
 char int_n_sep_by_space;
 char int_p_sign_posn;
 char int_n_sign_posn;
};

extern "C" {
struct lconv *localeconv(void);
}
extern "C" {
char *setlocale(int, const char *);
}
namespace std {

  using ::lconv;
  using ::setlocale;
  using ::localeconv;

}
       

typedef __darwin_va_list va_list;

typedef __darwin_off_t fpos_t;

struct __sbuf {
 unsigned char *_base;
 int _size;
};
struct __sFILEX;

typedef struct __sFILE {
 unsigned char *_p;
 int _r;
 int _w;
 short _flags;
 short _file;
 struct __sbuf _bf;
 int _lbfsize;
 void *_cookie;
 int (*_close)(void *);
 int (*_read) (void *, char *, int);
 fpos_t (*_seek) (void *, fpos_t, int);
 int (*_write)(void *, const char *, int);
 struct __sbuf _ub;
 struct __sFILEX *_extra;
 int _ur;
 unsigned char _ubuf[3];
 unsigned char _nbuf[1];
 struct __sbuf _lb;
 int _blksize;
 fpos_t _offset;
} FILE;

extern "C" {

extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;

}

extern "C" {
void clearerr(FILE *);
int fclose(FILE *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
int fgetc(FILE *);
int fgetpos(FILE * , fpos_t *);
char *fgets(char * , int, FILE *);

FILE *fopen(const char * , const char * ) __asm("_" "fopen" );

int fprintf(FILE * , const char * , ...) ;
int fputc(int, FILE *);
int fputs(const char * , FILE * ) __asm("_" "fputs" );
size_t fread(void * , size_t, size_t, FILE * );
FILE *freopen(const char * , const char * ,
     FILE * ) __asm("_" "freopen" );
int fscanf(FILE * , const char * , ...) ;
int fseek(FILE *, long, int);
int fsetpos(FILE *, const fpos_t *);
long ftell(FILE *);
size_t fwrite(const void * , size_t, size_t, FILE * ) __asm("_" "fwrite" );
int getc(FILE *);
int getchar(void);
char *gets(char *);

extern const int sys_nerr;
extern const char *const sys_errlist[];

void perror(const char *);
int printf(const char * , ...) ;
int putc(int, FILE *);
int putchar(int);
int puts(const char *);
int remove(const char *);
int rename (const char *, const char *);
void rewind(FILE *);
int scanf(const char * , ...) ;
void setbuf(FILE * , char * );
int setvbuf(FILE * , char * , int, size_t);
int sprintf(char * , const char * , ...) ;
int sscanf(const char * , const char * , ...) ;
FILE *tmpfile(void);
char *tmpnam(char *);
int ungetc(int, FILE *);
int vfprintf(FILE * , const char * , va_list) ;
int vprintf(const char * , va_list) ;
int vsprintf(char * , const char * , va_list) ;

int asprintf(char **, const char *, ...) ;
int vasprintf(char **, const char *, va_list) ;

}

extern "C" {
char *ctermid(char *);

char *ctermid_r(char *);
FILE *fdopen(int, const char *) __asm("_" "fdopen" );
char *fgetln(FILE *, size_t *);

int fileno(FILE *);
void flockfile(FILE *);

const char
 *fmtcheck(const char *, const char *);
int fpurge(FILE *);

int fseeko(FILE *, off_t, int);
off_t ftello(FILE *);
int ftrylockfile(FILE *);
void funlockfile(FILE *);
int getc_unlocked(FILE *);
int getchar_unlocked(void);

int getw(FILE *);

int pclose(FILE *);

FILE *popen(const char *, const char *) __asm("_" "popen" );

int putc_unlocked(int, FILE *);
int putchar_unlocked(int);

int putw(int, FILE *);
void setbuffer(FILE *, char *, int);
int setlinebuf(FILE *);

int snprintf(char * , size_t, const char * , ...) ;
char *tempnam(const char *, const char *) __asm("_" "tempnam" );
int vfscanf(FILE * , const char * , va_list) ;
int vscanf(const char * , va_list) ;
int vsnprintf(char * , size_t, const char * , va_list) ;
int vsscanf(const char * , const char * , va_list) ;

FILE *zopen(const char *, const char *, int);

}

extern "C" {
FILE *funopen(const void *,
  int (*)(void *, char *, int),
  int (*)(void *, const char *, int),
  fpos_t (*)(void *, fpos_t, int),
  int (*)(void *));
}

extern "C" {
int __srget(FILE *);
int __svfscanf(FILE *, const char *, va_list) ;
int __swbuf(int, FILE *);
}

static inline int __sputc(int _c, FILE *_p) {
 if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
  return (*_p->_p++ = _c);
 else
  return (__swbuf(_c, _p));
}
namespace std {

  using ::FILE;
  using ::fpos_t;

  using ::clearerr;
  using ::fclose;
  using ::feof;
  using ::ferror;
  using ::fflush;
  using ::fgetc;
  using ::fgetpos;
  using ::fgets;
  using ::fopen;
  using ::fprintf;
  using ::fputc;
  using ::fputs;
  using ::fread;
  using ::freopen;
  using ::fscanf;
  using ::fseek;
  using ::fsetpos;
  using ::ftell;
  using ::fwrite;
  using ::getc;
  using ::getchar;
  using ::gets;
  using ::perror;
  using ::printf;
  using ::putc;
  using ::putchar;
  using ::puts;
  using ::remove;
  using ::rename;
  using ::rewind;
  using ::scanf;
  using ::setbuf;
  using ::setvbuf;
  using ::sprintf;
  using ::sscanf;
  using ::tmpfile;
  using ::tmpnam;
  using ::ungetc;
  using ::vfprintf;
  using ::vprintf;
  using ::vsprintf;

}

namespace __gnu_cxx {

  using ::snprintf;
  using ::vfscanf;
  using ::vscanf;
  using ::vsnprintf;
  using ::vsscanf;
}

namespace std {

  using ::__gnu_cxx::snprintf;
  using ::__gnu_cxx::vfscanf;
  using ::__gnu_cxx::vscanf;
  using ::__gnu_cxx::vsnprintf;
  using ::__gnu_cxx::vsscanf;

}

       

typedef __builtin_va_list __gnuc_va_list;

namespace std {

  using ::va_list;

}
namespace std {

  typedef int* __c_locale;

  inline int
  __convert_from_v(const __c_locale&, char* __out,
     const int __size __attribute__((__unused__)),
     const char* __fmt, ...)
  {
    char* __old = std::setlocale(4, __null);
    char* __sav = __null;
    if (std::strcmp(__old, "C"))
      {
 __sav = new char[std::strlen(__old) + 1];
 std::strcpy(__sav, __old);
 std::setlocale(4, "C");
      }

    va_list __args;
    __builtin_va_start(__args,__fmt);
    const int __ret = std::vsnprintf(__out, __size, __fmt, __args);
    __builtin_va_end(__args);

    if (__sav)
      {
 std::setlocale(4, __sav);
 delete [] __sav;
      }
    return __ret;
  }

}
extern "C" {
struct sched_param { int sched_priority; char __opaque[4]; };
extern int sched_yield(void);
extern int sched_get_priority_min(int);
extern int sched_get_priority_max(int);
}

typedef __darwin_clock_t clock_t;

struct tm {
 int tm_sec;
 int tm_min;
 int tm_hour;
 int tm_mday;
 int tm_mon;
 int tm_year;
 int tm_wday;
 int tm_yday;
 int tm_isdst;
 long tm_gmtoff;
 char *tm_zone;
};

extern char *tzname[];
extern int getdate_err;

extern long timezone __asm("_" "timezone" );

extern int daylight;

extern "C" {
char *asctime(const struct tm *);
clock_t clock(void) __asm("_" "clock" );
char *ctime(const time_t *);
double difftime(time_t, time_t);
struct tm *getdate(const char *);
struct tm *gmtime(const time_t *);
struct tm *localtime(const time_t *);
time_t mktime(struct tm *) __asm("_" "mktime" );
size_t strftime(char * , size_t, const char * , const struct tm * ) __asm("_" "strftime" );
char *strptime(const char * , const char * , struct tm * ) __asm("_" "strptime" );
time_t time(time_t *);
void tzset(void);

char *asctime_r(const struct tm * , char * );
char *ctime_r(const time_t *, char *);
struct tm *gmtime_r(const time_t * , struct tm * );
struct tm *localtime_r(const time_t * , struct tm * );
time_t posix2time(time_t);

void tzsetwall(void);
time_t time2posix(time_t);
time_t timelocal(struct tm * const);
time_t timegm(struct tm * const);

int nanosleep(const struct timespec *, struct timespec *) __asm("_" "nanosleep" );

}
typedef __darwin_pthread_cond_t pthread_cond_t;
typedef __darwin_pthread_condattr_t pthread_condattr_t;
typedef __darwin_pthread_key_t pthread_key_t;
typedef __darwin_pthread_mutex_t pthread_mutex_t;
typedef __darwin_pthread_mutexattr_t pthread_mutexattr_t;
typedef __darwin_pthread_once_t pthread_once_t;
typedef __darwin_pthread_rwlock_t pthread_rwlock_t;
typedef __darwin_pthread_rwlockattr_t pthread_rwlockattr_t;
typedef __darwin_pthread_t pthread_t;
typedef __darwin_mach_port_t mach_port_t;

extern "C" {

int pthread_atfork(void (*)(void), void (*)(void),
                      void (*)(void));
int pthread_attr_destroy(pthread_attr_t *);
int pthread_attr_getdetachstate(const pthread_attr_t *,
          int *);
int pthread_attr_getguardsize(const pthread_attr_t * ,
                                      size_t * );
int pthread_attr_getinheritsched(const pthread_attr_t * ,
           int * );
int pthread_attr_getschedparam(const pthread_attr_t * ,
                                     struct sched_param * );
int pthread_attr_getschedpolicy(const pthread_attr_t * ,
          int * );
int pthread_attr_getscope(const pthread_attr_t * , int * );
int pthread_attr_getstack(const pthread_attr_t * ,
                                      void ** , size_t * );
int pthread_attr_getstackaddr(const pthread_attr_t * ,
                                      void ** );
int pthread_attr_getstacksize(const pthread_attr_t * ,
                                      size_t * );
int pthread_attr_init(pthread_attr_t *);
int pthread_attr_setdetachstate(pthread_attr_t *,
          int );
int pthread_attr_setguardsize(pthread_attr_t *, size_t );
int pthread_attr_setinheritsched(pthread_attr_t *,
           int );
int pthread_attr_setschedparam(pthread_attr_t * ,
                                     const struct sched_param * );
int pthread_attr_setschedpolicy(pthread_attr_t *,
          int );
int pthread_attr_setscope(pthread_attr_t *, int);
int pthread_attr_setstack(pthread_attr_t *,
                                      void *, size_t );
int pthread_attr_setstackaddr(pthread_attr_t *,
                                      void *);
int pthread_attr_setstacksize(pthread_attr_t *, size_t );
int pthread_cancel(pthread_t ) __asm("_" "pthread_cancel" );

int pthread_cond_broadcast(pthread_cond_t *);
int pthread_cond_destroy(pthread_cond_t *);
int pthread_cond_init(pthread_cond_t * ,
                            const pthread_condattr_t * ) __asm("_" "pthread_cond_init" );
int pthread_cond_signal(pthread_cond_t *);
int pthread_cond_timedwait(pthread_cond_t * ,
     pthread_mutex_t * ,
     const struct timespec * ) __asm("_" "pthread_cond_timedwait" );
int pthread_cond_wait(pthread_cond_t * ,
       pthread_mutex_t * ) __asm("_" "pthread_cond_wait" );
int pthread_condattr_destroy(pthread_condattr_t *);
int pthread_condattr_init(pthread_condattr_t *);
int pthread_condattr_getpshared(const pthread_condattr_t * ,
   int * );
int pthread_condattr_setpshared(pthread_condattr_t *,
   int );
int pthread_create(pthread_t * ,
                         const pthread_attr_t * ,
                         void *(*)(void *),
                         void * );
int pthread_detach(pthread_t );
int pthread_equal(pthread_t ,
   pthread_t );
void pthread_exit(void *) __attribute__((__noreturn__));
int pthread_getconcurrency(void);
int pthread_getschedparam(pthread_t , int * , struct sched_param * );
void *pthread_getspecific(pthread_key_t );
int pthread_join(pthread_t , void **) __asm("_" "pthread_join" );
int pthread_key_create(pthread_key_t *, void (*)(void *));
int pthread_key_delete(pthread_key_t );
int pthread_mutex_destroy(pthread_mutex_t *);
int pthread_mutex_getprioceiling(const pthread_mutex_t * , int * );
int pthread_mutex_init(pthread_mutex_t * , const pthread_mutexattr_t * );
int pthread_mutex_lock(pthread_mutex_t *);
int pthread_mutex_setprioceiling(pthread_mutex_t * , int, int * );
int pthread_mutex_trylock(pthread_mutex_t *);
int pthread_mutex_unlock(pthread_mutex_t *);
int pthread_mutexattr_destroy(pthread_mutexattr_t *) __asm("_" "pthread_mutexattr_destroy" );
int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t * , int * );
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t * , int * );
int pthread_mutexattr_getpshared(const pthread_mutexattr_t * , int * );
int pthread_mutexattr_gettype(const pthread_mutexattr_t * , int * );
int pthread_mutexattr_init(pthread_mutexattr_t *);
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *, int);
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *, int);
int pthread_mutexattr_setpshared(pthread_mutexattr_t *, int );
int pthread_mutexattr_settype(pthread_mutexattr_t *, int);
int pthread_once(pthread_once_t *, void (*)(void));
int pthread_rwlock_destroy(pthread_rwlock_t * ) __asm("_" "pthread_rwlock_destroy" );
int pthread_rwlock_init(pthread_rwlock_t * , const pthread_rwlockattr_t * ) __asm("_" "pthread_rwlock_init" );
int pthread_rwlock_rdlock(pthread_rwlock_t *) __asm("_" "pthread_rwlock_rdlock" );
int pthread_rwlock_tryrdlock(pthread_rwlock_t *) __asm("_" "pthread_rwlock_tryrdlock" );
int pthread_rwlock_trywrlock(pthread_rwlock_t *) __asm("_" "pthread_rwlock_trywrlock" );
int pthread_rwlock_wrlock(pthread_rwlock_t *) __asm("_" "pthread_rwlock_wrlock" );
int pthread_rwlock_unlock(pthread_rwlock_t *) __asm("_" "pthread_rwlock_unlock" );
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *);
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t * ,
   int * );
int pthread_rwlockattr_init(pthread_rwlockattr_t *);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *,
   int );
pthread_t pthread_self(void);

int pthread_setcancelstate(int , int *) __asm("_" "pthread_setcancelstate" );
int pthread_setcanceltype(int , int *) __asm("_" "pthread_setcanceltype" );
int pthread_setconcurrency(int);
int pthread_setschedparam(pthread_t ,
    int ,
                                const struct sched_param *);
int pthread_setspecific(pthread_key_t ,
         const void *);
void pthread_testcancel(void) __asm("_" "pthread_testcancel" );

int pthread_is_threaded_np(void);
int pthread_threadid_np(pthread_t,__uint64_t*);

int pthread_getname_np(pthread_t,char*,size_t);
int pthread_setname_np(const char*);

int pthread_main_np(void);
mach_port_t pthread_mach_thread_np(pthread_t);
size_t pthread_get_stacksize_np(pthread_t);
void * pthread_get_stackaddr_np(pthread_t);
int pthread_cond_signal_thread_np(pthread_cond_t *, pthread_t);
int pthread_cond_timedwait_relative_np(pthread_cond_t *,
     pthread_mutex_t *,
     const struct timespec *);
int pthread_create_suspended_np(pthread_t *,
                         const pthread_attr_t *,
                         void *(*)(void *),
                         void *);
int pthread_kill(pthread_t, int);

pthread_t pthread_from_mach_thread_np(mach_port_t);

int pthread_sigmask(int, const sigset_t *, sigset_t *) __asm("_" "pthread_sigmask" );
void pthread_yield_np(void);

}

typedef pthread_key_t __gthread_key_t;
typedef pthread_once_t __gthread_once_t;
typedef pthread_mutex_t __gthread_mutex_t;
typedef pthread_mutex_t __gthread_recursive_mutex_t;
static inline int
__gthread_active_p (void)
{
  return 1;
}

static inline int
__gthread_once (__gthread_once_t *once, void (*func) (void))
{
  if (__gthread_active_p ())
    return pthread_once (once, func);
  else
    return -1;
}

static inline int
__gthread_key_create (__gthread_key_t *key, void (*dtor) (void *))
{
  return pthread_key_create (key, dtor);
}

static inline int
__gthread_key_delete (__gthread_key_t key)
{
  return pthread_key_delete (key);
}

static inline void *
__gthread_getspecific (__gthread_key_t key)
{
  return pthread_getspecific (key);
}

static inline int
__gthread_setspecific (__gthread_key_t key, const void *ptr)
{
  return pthread_setspecific (key, ptr);
}

static inline int
__gthread_mutex_lock (__gthread_mutex_t *mutex)
{
  if (__gthread_active_p ())
    return pthread_mutex_lock (mutex);
  else
    return 0;
}

static inline int
__gthread_mutex_trylock (__gthread_mutex_t *mutex)
{
  if (__gthread_active_p ())
    return pthread_mutex_trylock (mutex);
  else
    return 0;
}

static inline int
__gthread_mutex_unlock (__gthread_mutex_t *mutex)
{
  if (__gthread_active_p ())
    return pthread_mutex_unlock (mutex);
  else
    return 0;
}
static inline int
__gthread_recursive_mutex_init_function (__gthread_recursive_mutex_t *mutex)
{
  if (__gthread_active_p ())
    {
      pthread_mutexattr_t attr;
      int r;

      r = pthread_mutexattr_init (&attr);
      if (!r)
 r = pthread_mutexattr_settype (&attr, 2);
      if (!r)
 r = pthread_mutex_init (mutex, &attr);
      if (!r)
 r = pthread_mutexattr_destroy (&attr);
      return r;
    }
  return 0;
}
static inline int
__gthread_recursive_mutex_lock (__gthread_recursive_mutex_t *mutex)
{
  return __gthread_mutex_lock (mutex);
}

static inline int
__gthread_recursive_mutex_trylock (__gthread_recursive_mutex_t *mutex)
{
  return __gthread_mutex_trylock (mutex);
}

static inline int
__gthread_recursive_mutex_unlock (__gthread_recursive_mutex_t *mutex)
{
  return __gthread_mutex_unlock (mutex);
}

namespace std {

  typedef __gthread_mutex_t __c_lock;
  typedef FILE __c_file;

}

       

typedef __darwin_wint_t wint_t;

typedef struct {
 __darwin_rune_t __min;
 __darwin_rune_t __max;
 __darwin_rune_t __map;
 __uint32_t *__types;
} _RuneEntry;

typedef struct {
 int __nranges;
 _RuneEntry *__ranges;
} _RuneRange;

typedef struct {
 char __name[14];
 __uint32_t __mask;
} _RuneCharClass;

typedef struct {
 char __magic[8];
 char __encoding[32];

 __darwin_rune_t (*__sgetrune)(const char *, __darwin_size_t, char const **);
 int (*__sputrune)(__darwin_rune_t, char *, __darwin_size_t, char **);
 __darwin_rune_t __invalid_rune;

 __uint32_t __runetype[(1 <<8 )];
 __darwin_rune_t __maplower[(1 <<8 )];
 __darwin_rune_t __mapupper[(1 <<8 )];
 _RuneRange __runetype_ext;
 _RuneRange __maplower_ext;
 _RuneRange __mapupper_ext;

 void *__variable;
 int __variable_len;
 int __ncharclasses;
 _RuneCharClass *__charclasses;
} _RuneLocale;

extern "C" {
extern _RuneLocale _DefaultRuneLocale;
extern _RuneLocale *_CurrentRuneLocale;
}
extern "C" {
unsigned long ___runetype(__darwin_ct_rune_t);
__darwin_ct_rune_t ___tolower(__darwin_ct_rune_t);
__darwin_ct_rune_t ___toupper(__darwin_ct_rune_t);
}

static inline int
isascii(int _c)
{
 return ((_c & ~0x7F) == 0);
}

extern "C" {
int __maskrune(__darwin_ct_rune_t, unsigned long);
}
static inline int
__istype(__darwin_ct_rune_t _c, unsigned long _f)
{

 return (isascii(_c) ? !!(_DefaultRuneLocale.__runetype[_c] & _f)
  : !!__maskrune(_c, _f));

}

static inline __darwin_ct_rune_t
__isctype(__darwin_ct_rune_t _c, unsigned long _f)
{

 return (_c < 0 || _c >= (1 <<8 )) ? 0 :
  !!(_DefaultRuneLocale.__runetype[_c] & _f);

}

extern "C" {
__darwin_ct_rune_t __toupper(__darwin_ct_rune_t);
__darwin_ct_rune_t __tolower(__darwin_ct_rune_t);
}
static inline int
__wcwidth(__darwin_ct_rune_t _c)
{
 unsigned int _x;

 if (_c == 0)
  return (0);
 _x = (unsigned int)__maskrune(_c, 0xe0000000L|0x00040000L);
 if ((_x & 0xe0000000L) != 0)
  return ((_x & 0xe0000000L) >> 30);
 return ((_x & 0x00040000L) != 0 ? 1 : -1);
}
static inline int
isalnum(int _c)
{
 return (__istype(_c, 0x00000100L|0x00000400L));
}

static inline int
isalpha(int _c)
{
 return (__istype(_c, 0x00000100L));
}

static inline int
isblank(int _c)
{
 return (__istype(_c, 0x00020000L));
}

static inline int
iscntrl(int _c)
{
 return (__istype(_c, 0x00000200L));
}
static inline int
isdigit(int _c)
{
 return (__isctype(_c, 0x00000400L));
}

static inline int
isgraph(int _c)
{
 return (__istype(_c, 0x00000800L));
}

static inline int
islower(int _c)
{
 return (__istype(_c, 0x00001000L));
}

static inline int
isprint(int _c)
{
 return (__istype(_c, 0x00040000L));
}

static inline int
ispunct(int _c)
{
 return (__istype(_c, 0x00002000L));
}

static inline int
isspace(int _c)
{
 return (__istype(_c, 0x00004000L));
}

static inline int
isupper(int _c)
{
 return (__istype(_c, 0x00008000L));
}
static inline int
isxdigit(int _c)
{
 return (__isctype(_c, 0x00010000L));
}

static inline int
toascii(int _c)
{
 return (_c & 0x7F);
}

static inline int
tolower(int _c)
{
        return (__tolower(_c));
}

static inline int
toupper(int _c)
{
        return (__toupper(_c));
}
static inline int
digittoint(int _c)
{
 return (__maskrune(_c, 0x0F));
}

static inline int
ishexnumber(int _c)
{
 return (__istype(_c, 0x00010000L));
}

static inline int
isideogram(int _c)
{
 return (__istype(_c, 0x00080000L));
}

static inline int
isnumber(int _c)
{
 return (__istype(_c, 0x00000400L));
}

static inline int
isphonogram(int _c)
{
 return (__istype(_c, 0x00200000L));
}

static inline int
isrune(int _c)
{
 return (__istype(_c, 0xFFFFFFF0L));
}

static inline int
isspecial(int _c)
{
 return (__istype(_c, 0x00100000L));
}
namespace std {

  using ::isalnum;
  using ::isalpha;
  using ::iscntrl;
  using ::isdigit;
  using ::isgraph;
  using ::islower;
  using ::isprint;
  using ::ispunct;
  using ::isspace;
  using ::isupper;
  using ::isxdigit;
  using ::tolower;
  using ::toupper;

}
       
       
       
namespace std {

  using ::clock_t;
  using ::time_t;
  using ::tm;

  using ::clock;
  using ::difftime;
  using ::mktime;
  using ::time;
  using ::asctime;
  using ::ctime;
  using ::gmtime;
  using ::localtime;
  using ::strftime;

}

typedef __darwin_mbstate_t mbstate_t;

typedef __darwin_wctype_t wctype_t;

static inline int
iswalnum(wint_t _wc)
{
 return (__istype(_wc, 0x00000100L|0x00000400L));
}

static inline int
iswalpha(wint_t _wc)
{
 return (__istype(_wc, 0x00000100L));
}

static inline int
iswcntrl(wint_t _wc)
{
 return (__istype(_wc, 0x00000200L));
}

static inline int
iswctype(wint_t _wc, wctype_t _charclass)
{
 return (__istype(_wc, _charclass));
}

static inline int
iswdigit(wint_t _wc)
{
 return (__isctype(_wc, 0x00000400L));
}

static inline int
iswgraph(wint_t _wc)
{
 return (__istype(_wc, 0x00000800L));
}

static inline int
iswlower(wint_t _wc)
{
 return (__istype(_wc, 0x00001000L));
}

static inline int
iswprint(wint_t _wc)
{
 return (__istype(_wc, 0x00040000L));
}

static inline int
iswpunct(wint_t _wc)
{
 return (__istype(_wc, 0x00002000L));
}

static inline int
iswspace(wint_t _wc)
{
 return (__istype(_wc, 0x00004000L));
}

static inline int
iswupper(wint_t _wc)
{
 return (__istype(_wc, 0x00008000L));
}

static inline int
iswxdigit(wint_t _wc)
{
 return (__isctype(_wc, 0x00010000L));
}

static inline wint_t
towlower(wint_t _wc)
{
        return (__tolower(_wc));
}

static inline wint_t
towupper(wint_t _wc)
{
        return (__toupper(_wc));
}

extern "C" {
wctype_t
 wctype(const char *);
}
extern "C" {
wint_t btowc(int);
wint_t fgetwc(FILE *);
wchar_t *fgetws(wchar_t * , int, FILE * );
wint_t fputwc(wchar_t, FILE *);
int fputws(const wchar_t * , FILE * );
int fwide(FILE *, int);
int fwprintf(FILE * , const wchar_t * , ...) ;
int fwscanf(FILE * , const wchar_t * , ...) ;
wint_t getwc(FILE *);
wint_t getwchar(void);
size_t mbrlen(const char * , size_t, mbstate_t * );
size_t mbrtowc(wchar_t * , const char * , size_t,
     mbstate_t * );
int mbsinit(const mbstate_t *);
size_t mbsrtowcs(wchar_t * , const char ** , size_t,
     mbstate_t * );
wint_t putwc(wchar_t, FILE *);
wint_t putwchar(wchar_t);
int swprintf(wchar_t * , size_t, const wchar_t * ,
     ...) ;
int swscanf(const wchar_t * , const wchar_t * , ...) ;
wint_t ungetwc(wint_t, FILE *);
int vfwprintf(FILE * , const wchar_t * ,
     __darwin_va_list) ;
int vswprintf(wchar_t * , size_t, const wchar_t * ,
     __darwin_va_list) ;
int vwprintf(const wchar_t * , __darwin_va_list) ;
size_t wcrtomb(char * , wchar_t, mbstate_t * );
wchar_t *wcscat(wchar_t * , const wchar_t * );
wchar_t *wcschr(const wchar_t *, wchar_t);
int wcscmp(const wchar_t *, const wchar_t *);
int wcscoll(const wchar_t *, const wchar_t *);
wchar_t *wcscpy(wchar_t * , const wchar_t * );
size_t wcscspn(const wchar_t *, const wchar_t *);
size_t wcsftime(wchar_t * , size_t, const wchar_t * ,
     const struct tm * ) __asm("_" "wcsftime" );
size_t wcslen(const wchar_t *);
wchar_t *wcsncat(wchar_t * , const wchar_t * , size_t);
int wcsncmp(const wchar_t *, const wchar_t *, size_t);
wchar_t *wcsncpy(wchar_t * , const wchar_t * , size_t);
wchar_t *wcspbrk(const wchar_t *, const wchar_t *);
wchar_t *wcsrchr(const wchar_t *, wchar_t);
size_t wcsrtombs(char * , const wchar_t ** , size_t,
     mbstate_t * );
size_t wcsspn(const wchar_t *, const wchar_t *);
wchar_t *wcsstr(const wchar_t * , const wchar_t * );
size_t wcsxfrm(wchar_t * , const wchar_t * , size_t);
int wctob(wint_t);
double wcstod(const wchar_t * , wchar_t ** );
wchar_t *wcstok(wchar_t * , const wchar_t * ,
     wchar_t ** );
long wcstol(const wchar_t * , wchar_t ** , int);
unsigned long
  wcstoul(const wchar_t * , wchar_t ** , int);
wchar_t *wmemchr(const wchar_t *, wchar_t, size_t);
int wmemcmp(const wchar_t *, const wchar_t *, size_t);
wchar_t *wmemcpy(wchar_t * , const wchar_t * , size_t);
wchar_t *wmemmove(wchar_t *, const wchar_t *, size_t);
wchar_t *wmemset(wchar_t *, wchar_t, size_t);
int wprintf(const wchar_t * , ...) ;
int wscanf(const wchar_t * , ...) ;
int vfwscanf(FILE * , const wchar_t * ,
     __darwin_va_list) ;
int vswscanf(const wchar_t * , const wchar_t * ,
     __darwin_va_list) ;
int vwscanf(const wchar_t * , __darwin_va_list) ;
float wcstof(const wchar_t * , wchar_t ** );
long double
 wcstold(const wchar_t * , wchar_t ** ) ;

long long
 wcstoll(const wchar_t * , wchar_t ** , int);
unsigned long long
 wcstoull(const wchar_t * , wchar_t ** , int);

int wcswidth(const wchar_t *, size_t);
int wcwidth(wchar_t);

size_t mbsnrtowcs(wchar_t * , const char ** , size_t,
     size_t, mbstate_t * );
size_t wcslcat(wchar_t *, const wchar_t *, size_t);
size_t wcslcpy(wchar_t *, const wchar_t *, size_t);
size_t wcsnrtombs(char * , const wchar_t ** , size_t,
     size_t, mbstate_t * );
}
namespace std {

  using ::mbstate_t;

}

namespace std {

  using ::wint_t;

  using ::btowc;
  using ::fgetwc;
  using ::fgetws;
  using ::fputwc;
  using ::fputws;
  using ::fwide;
  using ::fwprintf;
  using ::fwscanf;
  using ::getwc;
  using ::getwchar;
  using ::mbrlen;
  using ::mbrtowc;
  using ::mbsinit;
  using ::mbsrtowcs;
  using ::putwc;
  using ::putwchar;
  using ::swprintf;
  using ::swscanf;
  using ::ungetwc;
  using ::vfwprintf;

  using ::vfwscanf;

  using ::vswprintf;

  using ::vswscanf;

  using ::vwprintf;

  using ::vwscanf;

  using ::wcrtomb;
  using ::wcscat;
  using ::wcscmp;
  using ::wcscoll;
  using ::wcscpy;
  using ::wcscspn;
  using ::wcsftime;
  using ::wcslen;
  using ::wcsncat;
  using ::wcsncmp;
  using ::wcsncpy;
  using ::wcsrtombs;
  using ::wcsspn;
  using ::wcstod;

  using ::wcstof;

  using ::wcstok;
  using ::wcstol;
  using ::wcstoul;
  using ::wcsxfrm;
  using ::wctob;
  using ::wmemcmp;
  using ::wmemcpy;
  using ::wmemmove;
  using ::wmemset;
  using ::wprintf;
  using ::wscanf;

  using ::wcschr;

  inline wchar_t*
  wcschr(wchar_t* __p, wchar_t __c)
  { return wcschr(const_cast<const wchar_t*>(__p), __c); }

  using ::wcspbrk;

  inline wchar_t*
  wcspbrk(wchar_t* __s1, const wchar_t* __s2)
  { return wcspbrk(const_cast<const wchar_t*>(__s1), __s2); }

  using ::wcsrchr;

  inline wchar_t*
  wcsrchr(wchar_t* __p, wchar_t __c)
  { return wcsrchr(const_cast<const wchar_t*>(__p), __c); }

  using ::wcsstr;

  inline wchar_t*
  wcsstr(wchar_t* __s1, const wchar_t* __s2)
  { return wcsstr(const_cast<const wchar_t*>(__s1), __s2); }

  using ::wmemchr;

  inline wchar_t*
  wmemchr(wchar_t* __p, wchar_t __c, size_t __n)
  { return wmemchr(const_cast<const wchar_t*>(__p), __c, __n); }

}

namespace __gnu_cxx {
  using ::wcstold;

  using ::wcstoll;
  using ::wcstoull;
}

namespace std {

  using ::__gnu_cxx::wcstold;
  using ::__gnu_cxx::wcstoll;
  using ::__gnu_cxx::wcstoull;

}

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;
typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

typedef int8_t int_fast8_t;
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int64_t int_fast64_t;
typedef uint8_t uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;

typedef long int intmax_t;

typedef long unsigned int uintmax_t;

namespace std {

  typedef int64_t streamoff;

  typedef ptrdiff_t streamsize;

  template<typename _StateT>
    class fpos;

  template<typename _StateT>
    class fpos
    {
    private:
      streamoff _M_off;
      _StateT _M_state;

    public:
      fpos()
      : _M_off(0), _M_state() { }

      fpos(streamoff __off)
      : _M_off(__off), _M_state() { }
      operator streamoff() const { return _M_off; }
      void
      state(_StateT __st)
      { _M_state = __st; }
      _StateT
      state() const
      { return _M_state; }

      fpos&
      operator+=(streamoff __off)
      {
 _M_off += __off;
 return *this;
      }

      fpos&
      operator-=(streamoff __off)
      {
 _M_off -= __off;
 return *this;
      }

      fpos
      operator+(streamoff __off) const
      {
 fpos __pos(*this);
 __pos += __off;
 return __pos;
      }

      fpos
      operator-(streamoff __off) const
      {
 fpos __pos(*this);
 __pos -= __off;
 return __pos;
      }
      streamoff
      operator-(const fpos& __other) const
      { return _M_off - __other._M_off; }
    };
  template<typename _StateT>
    inline bool
    operator==(const fpos<_StateT>& __lhs, const fpos<_StateT>& __rhs)
    { return streamoff(__lhs) == streamoff(__rhs); }

  template<typename _StateT>
    inline bool
    operator!=(const fpos<_StateT>& __lhs, const fpos<_StateT>& __rhs)
    { return streamoff(__lhs) != streamoff(__rhs); }

  typedef fpos<mbstate_t> streampos;

  typedef fpos<mbstate_t> wstreampos;

}
namespace std {
  void
  __throw_bad_exception(void) __attribute__((__noreturn__));
  void
  __throw_bad_alloc(void) __attribute__((__noreturn__));
  void
  __throw_bad_cast(void) __attribute__((__noreturn__));

  void
  __throw_bad_typeid(void) __attribute__((__noreturn__));
  void
  __throw_logic_error(const char*) __attribute__((__noreturn__));

  void
  __throw_domain_error(const char*) __attribute__((__noreturn__));

  void
  __throw_invalid_argument(const char*) __attribute__((__noreturn__));

  void
  __throw_length_error(const char*) __attribute__((__noreturn__));

  void
  __throw_out_of_range(const char*) __attribute__((__noreturn__));

  void
  __throw_runtime_error(const char*) __attribute__((__noreturn__));

  void
  __throw_range_error(const char*) __attribute__((__noreturn__));

  void
  __throw_overflow_error(const char*) __attribute__((__noreturn__));

  void
  __throw_underflow_error(const char*) __attribute__((__noreturn__));
  void
  __throw_ios_failure(const char*) __attribute__((__noreturn__));

}
namespace std {

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_ios;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_streambuf;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_istream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_ostream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_iostream;

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
     typename _Alloc = allocator<_CharT> >
    class basic_stringbuf;

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
    typename _Alloc = allocator<_CharT> >
    class basic_istringstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
    typename _Alloc = allocator<_CharT> >
    class basic_ostringstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
    typename _Alloc = allocator<_CharT> >
    class basic_stringstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_filebuf;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_ifstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_ofstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_fstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class istreambuf_iterator;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class ostreambuf_iterator;

  class ios_base;

  typedef basic_ios<char> ios;
  typedef basic_streambuf<char> streambuf;
  typedef basic_istream<char> istream;
  typedef basic_ostream<char> ostream;
  typedef basic_iostream<char> iostream;
  typedef basic_stringbuf<char> stringbuf;
  typedef basic_istringstream<char> istringstream;
  typedef basic_ostringstream<char> ostringstream;
  typedef basic_stringstream<char> stringstream;
  typedef basic_filebuf<char> filebuf;
  typedef basic_ifstream<char> ifstream;
  typedef basic_ofstream<char> ofstream;
  typedef basic_fstream<char> fstream;
  typedef basic_ios<wchar_t> wios;
  typedef basic_streambuf<wchar_t> wstreambuf;
  typedef basic_istream<wchar_t> wistream;
  typedef basic_ostream<wchar_t> wostream;
  typedef basic_iostream<wchar_t> wiostream;
  typedef basic_stringbuf<wchar_t> wstringbuf;
  typedef basic_istringstream<wchar_t> wistringstream;
  typedef basic_ostringstream<wchar_t> wostringstream;
  typedef basic_stringstream<wchar_t> wstringstream;
  typedef basic_filebuf<wchar_t> wfilebuf;
  typedef basic_ifstream<wchar_t> wifstream;
  typedef basic_ofstream<wchar_t> wofstream;
  typedef basic_fstream<wchar_t> wfstream;

}

namespace std {
  template<class _T1, class _T2>
    struct pair
    {
      typedef _T1 first_type;
      typedef _T2 second_type;

      _T1 first;
      _T2 second;

      pair()
      : first(), second() { }
      pair(const _T1& __a, const _T2& __b)
      : first(__a), second(__b) { }
      template<class _U1, class _U2>
        pair(const pair<_U1, _U2>& __p)
 : first(__p.first), second(__p.second) { }
    };
  template<class _T1, class _T2>
    inline bool
    operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return __x.first == __y.first && __x.second == __y.second; }
  template<class _T1, class _T2>
    inline bool
    operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return __x.first < __y.first
      || (!(__y.first < __x.first) && __x.second < __y.second); }
  template<class _T1, class _T2>
    inline bool
    operator!=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return !(__x == __y); }
  template<class _T1, class _T2>
    inline bool
    operator>(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return __y < __x; }
  template<class _T1, class _T2>
    inline bool
    operator<=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return !(__y < __x); }
  template<class _T1, class _T2>
    inline bool
    operator>=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
    { return !(__x < __y); }

  template<class _T1, class _T2>
    inline pair<_T1, _T2>
    make_pair(_T1 __x, _T2 __y)
    { return pair<_T1, _T2>(__x, __y); }

}

       
namespace __gnu_cxx {

  template<typename _Iterator, typename _Container>
    class __normal_iterator;

}

namespace std {

namespace __detail
{
  typedef char __one;
  typedef char __two[2];

  template<typename _Tp>
  __one __test_type(int _Tp::*);
  template<typename _Tp>
  __two& __test_type(...);
}
  struct __true_type { };
  struct __false_type { };

  template<bool>
    struct __truth_type
    { typedef __false_type __type; };

  template<>
    struct __truth_type<true>
    { typedef __true_type __type; };

  template<class _Sp, class _Tp>
    struct __traitor
    {
      enum { __value = bool(_Sp::__value) || bool(_Tp::__value) };
      typedef typename __truth_type<__value>::__type __type;
    };
  template<typename, typename>
    struct __are_same
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  template<typename _Tp>
    struct __are_same<_Tp, _Tp>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
  template<typename _Tp>
    struct __is_void
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  template<>
    struct __is_void<void>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
  template<typename _Tp>
    struct __is_integer
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };
  template<>
    struct __is_integer<bool>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<char>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<signed char>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<unsigned char>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
  template<>
    struct __is_integer<wchar_t>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
  template<>
    struct __is_integer<short>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<unsigned short>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<int>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<unsigned int>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<long>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<unsigned long>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<long long>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_integer<unsigned long long>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
  template<typename _Tp>
    struct __is_floating
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };
  template<>
    struct __is_floating<float>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_floating<double>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };

  template<>
    struct __is_floating<long double>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
  template<typename _Tp>
    struct __is_pointer
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  template<typename _Tp>
    struct __is_pointer<_Tp*>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
  template<typename _Tp>
    struct __is_normal_iterator
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  template<typename _Iterator, typename _Container>
    struct __is_normal_iterator< __gnu_cxx::__normal_iterator<_Iterator,
             _Container> >
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
  template<typename _Tp>
    struct __is_arithmetic
    : public __traitor<__is_integer<_Tp>, __is_floating<_Tp> >
    { };
  template<typename _Tp>
    struct __is_fundamental
    : public __traitor<__is_void<_Tp>, __is_arithmetic<_Tp> >
    { };
  template<typename _Tp>
    struct __is_scalar
    : public __traitor<__is_arithmetic<_Tp>, __is_pointer<_Tp> >
    { };
  template<typename _Tp>
    struct __is_pod
    {
      enum
 {
   __value = (sizeof(__detail::__test_type<_Tp>(0))
       != sizeof(__detail::__one))
 };
    };
  template<typename _Tp>
    struct __is_empty
    {
    private:
      template<typename>
        struct __first { };
      template<typename _Up>
        struct __second
        : public _Up { };

    public:
      enum
 {
   __value = sizeof(__first<_Tp>) == sizeof(__second<_Tp>)
 };
    };
  template<typename _Tp>
    struct __is_char
    {
      enum { __value = 0 };
      typedef __false_type __type;
    };

  template<>
    struct __is_char<char>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
  template<>
    struct __is_char<wchar_t>
    {
      enum { __value = 1 };
      typedef __true_type __type;
    };
}

       

       

namespace std {

  namespace rel_ops
  {

    template <class _Tp>
      inline bool
      operator!=(const _Tp& __x, const _Tp& __y)
      { return !(__x == __y); }

    template <class _Tp>
      inline bool
      operator>(const _Tp& __x, const _Tp& __y)
      { return __y < __x; }

    template <class _Tp>
      inline bool
      operator<=(const _Tp& __x, const _Tp& __y)
      { return !(__y < __x); }

    template <class _Tp>
      inline bool
      operator>=(const _Tp& __x, const _Tp& __y)
      { return !(__x < __y); }

  }

}
namespace __gnu_cxx {
  template<bool, typename>
    struct __enable_if
    { };

  template<typename _Tp>
    struct __enable_if<true, _Tp>
    { typedef _Tp __type; };

  template<bool _Cond, typename _Iftrue, typename _Iffalse>
    struct __conditional_type
    { typedef _Iftrue __type; };

  template<typename _Iftrue, typename _Iffalse>
    struct __conditional_type<false, _Iftrue, _Iffalse>
    { typedef _Iffalse __type; };

  template<typename _Tp>
    struct __add_unsigned
    {
    private:
      typedef __enable_if<std::__is_integer<_Tp>::__value, _Tp> __if_type;

    public:
      typedef typename __if_type::__type __type;
    };

  template<>
    struct __add_unsigned<char>
    { typedef unsigned char __type; };

  template<>
    struct __add_unsigned<signed char>
    { typedef unsigned char __type; };

  template<>
    struct __add_unsigned<short>
    { typedef unsigned short __type; };

  template<>
    struct __add_unsigned<int>
    { typedef unsigned int __type; };

  template<>
    struct __add_unsigned<long>
    { typedef unsigned long __type; };

  template<>
    struct __add_unsigned<long long>
    { typedef unsigned long long __type; };
  template<>
    struct __add_unsigned<bool>;

  template<>
    struct __add_unsigned<wchar_t>;

  template<typename _Tp>
    struct __remove_unsigned
    {
    private:
      typedef __enable_if<std::__is_integer<_Tp>::__value, _Tp> __if_type;

    public:
      typedef typename __if_type::__type __type;
    };

  template<>
    struct __remove_unsigned<char>
    { typedef signed char __type; };

  template<>
    struct __remove_unsigned<unsigned char>
    { typedef signed char __type; };

  template<>
    struct __remove_unsigned<unsigned short>
    { typedef short __type; };

  template<>
    struct __remove_unsigned<unsigned int>
    { typedef int __type; };

  template<>
    struct __remove_unsigned<unsigned long>
    { typedef long __type; };

  template<>
    struct __remove_unsigned<unsigned long long>
    { typedef long long __type; };
  template<>
    struct __remove_unsigned<bool>;

  template<>
    struct __remove_unsigned<wchar_t>;

}

       
namespace std {

  struct input_iterator_tag {};

  struct output_iterator_tag {};

  struct forward_iterator_tag : public input_iterator_tag {};
  struct bidirectional_iterator_tag : public forward_iterator_tag {};
  struct random_access_iterator_tag : public bidirectional_iterator_tag {};

  template<typename _Category, typename _Tp, typename _Distance = ptrdiff_t,
           typename _Pointer = _Tp*, typename _Reference = _Tp&>
    struct iterator
    {

      typedef _Category iterator_category;

      typedef _Tp value_type;

      typedef _Distance difference_type;

      typedef _Pointer pointer;

      typedef _Reference reference;
    };

  template<typename _Iterator>
    struct iterator_traits
    {
      typedef typename _Iterator::iterator_category iterator_category;
      typedef typename _Iterator::value_type value_type;
      typedef typename _Iterator::difference_type difference_type;
      typedef typename _Iterator::pointer pointer;
      typedef typename _Iterator::reference reference;
    };

  template<typename _Tp>
    struct iterator_traits<_Tp*>
    {
      typedef random_access_iterator_tag iterator_category;
      typedef _Tp value_type;
      typedef ptrdiff_t difference_type;
      typedef _Tp* pointer;
      typedef _Tp& reference;
    };

  template<typename _Tp>
    struct iterator_traits<const _Tp*>
    {
      typedef random_access_iterator_tag iterator_category;
      typedef _Tp value_type;
      typedef ptrdiff_t difference_type;
      typedef const _Tp* pointer;
      typedef const _Tp& reference;
    };

  template<typename _Iter>
    inline typename iterator_traits<_Iter>::iterator_category
    __iterator_category(const _Iter&)
    { return typename iterator_traits<_Iter>::iterator_category(); }

}

       

       

namespace std {

  template<typename _InputIterator>
    inline typename iterator_traits<_InputIterator>::difference_type
    __distance(_InputIterator __first, _InputIterator __last,
               input_iterator_tag)
    {

     

      typename iterator_traits<_InputIterator>::difference_type __n = 0;
      while (__first != __last)
 {
   ++__first;
   ++__n;
 }
      return __n;
    }

  template<typename _RandomAccessIterator>
    inline typename iterator_traits<_RandomAccessIterator>::difference_type
    __distance(_RandomAccessIterator __first, _RandomAccessIterator __last,
               random_access_iterator_tag)
    {

     

      return __last - __first;
    }

  template<typename _InputIterator>
    inline typename iterator_traits<_InputIterator>::difference_type
    distance(_InputIterator __first, _InputIterator __last)
    {

      return std::__distance(__first, __last,
        std::__iterator_category(__first));
    }

  template<typename _InputIterator, typename _Distance>
    inline void
    __advance(_InputIterator& __i, _Distance __n, input_iterator_tag)
    {

     
      while (__n--)
 ++__i;
    }

  template<typename _BidirectionalIterator, typename _Distance>
    inline void
    __advance(_BidirectionalIterator& __i, _Distance __n,
       bidirectional_iterator_tag)
    {

     

      if (__n > 0)
        while (__n--)
   ++__i;
      else
        while (__n++)
   --__i;
    }

  template<typename _RandomAccessIterator, typename _Distance>
    inline void
    __advance(_RandomAccessIterator& __i, _Distance __n,
              random_access_iterator_tag)
    {

     

      __i += __n;
    }

  template<typename _InputIterator, typename _Distance>
    inline void
    advance(_InputIterator& __i, _Distance __n)
    {

      typename iterator_traits<_InputIterator>::difference_type __d = __n;
      std::__advance(__i, __d, std::__iterator_category(__i));
    }

}

namespace std {

  template<typename _Iterator>
    class reverse_iterator
    : public iterator<typename iterator_traits<_Iterator>::iterator_category,
        typename iterator_traits<_Iterator>::value_type,
        typename iterator_traits<_Iterator>::difference_type,
        typename iterator_traits<_Iterator>::pointer,
                      typename iterator_traits<_Iterator>::reference>
    {
    protected:
      _Iterator current;

    public:
      typedef _Iterator iterator_type;
      typedef typename iterator_traits<_Iterator>::difference_type
              difference_type;
      typedef typename iterator_traits<_Iterator>::reference reference;
      typedef typename iterator_traits<_Iterator>::pointer pointer;

    public:
      reverse_iterator() : current() { }
      explicit
      reverse_iterator(iterator_type __x) : current(__x) { }
      reverse_iterator(const reverse_iterator& __x)
      : current(__x.current) { }

      template<typename _Iter>
        reverse_iterator(const reverse_iterator<_Iter>& __x)
 : current(__x.base()) { }
      iterator_type
      base() const
      { return current; }
      reference
      operator*() const
      {
 _Iterator __tmp = current;
 return *--__tmp;
      }
      pointer
      operator->() const
      { return &(operator*()); }
      reverse_iterator&
      operator++()
      {
 --current;
 return *this;
      }
      reverse_iterator
      operator++(int)
      {
 reverse_iterator __tmp = *this;
 --current;
 return __tmp;
      }
      reverse_iterator&
      operator--()
      {
 ++current;
 return *this;
      }
      reverse_iterator
      operator--(int)
      {
 reverse_iterator __tmp = *this;
 ++current;
 return __tmp;
      }
      reverse_iterator
      operator+(difference_type __n) const
      { return reverse_iterator(current - __n); }
      reverse_iterator&
      operator+=(difference_type __n)
      {
 current -= __n;
 return *this;
      }
      reverse_iterator
      operator-(difference_type __n) const
      { return reverse_iterator(current + __n); }
      reverse_iterator&
      operator-=(difference_type __n)
      {
 current += __n;
 return *this;
      }
      reference
      operator[](difference_type __n) const
      { return *(*this + __n); }
    };

  template<typename _Iterator>
    inline bool
    operator==(const reverse_iterator<_Iterator>& __x,
        const reverse_iterator<_Iterator>& __y)
    { return __x.base() == __y.base(); }

  template<typename _Iterator>
    inline bool
    operator<(const reverse_iterator<_Iterator>& __x,
       const reverse_iterator<_Iterator>& __y)
    { return __y.base() < __x.base(); }

  template<typename _Iterator>
    inline bool
    operator!=(const reverse_iterator<_Iterator>& __x,
        const reverse_iterator<_Iterator>& __y)
    { return !(__x == __y); }

  template<typename _Iterator>
    inline bool
    operator>(const reverse_iterator<_Iterator>& __x,
       const reverse_iterator<_Iterator>& __y)
    { return __y < __x; }

  template<typename _Iterator>
    inline bool
    operator<=(const reverse_iterator<_Iterator>& __x,
        const reverse_iterator<_Iterator>& __y)
    { return !(__y < __x); }

  template<typename _Iterator>
    inline bool
    operator>=(const reverse_iterator<_Iterator>& __x,
        const reverse_iterator<_Iterator>& __y)
    { return !(__x < __y); }

  template<typename _Iterator>
    inline typename reverse_iterator<_Iterator>::difference_type
    operator-(const reverse_iterator<_Iterator>& __x,
       const reverse_iterator<_Iterator>& __y)
    { return __y.base() - __x.base(); }

  template<typename _Iterator>
    inline reverse_iterator<_Iterator>
    operator+(typename reverse_iterator<_Iterator>::difference_type __n,
       const reverse_iterator<_Iterator>& __x)
    { return reverse_iterator<_Iterator>(__x.base() - __n); }

  template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator==(const reverse_iterator<_IteratorL>& __x,
        const reverse_iterator<_IteratorR>& __y)
    { return __x.base() == __y.base(); }

  template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator<(const reverse_iterator<_IteratorL>& __x,
       const reverse_iterator<_IteratorR>& __y)
    { return __y.base() < __x.base(); }

  template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator!=(const reverse_iterator<_IteratorL>& __x,
        const reverse_iterator<_IteratorR>& __y)
    { return !(__x == __y); }

  template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator>(const reverse_iterator<_IteratorL>& __x,
       const reverse_iterator<_IteratorR>& __y)
    { return __y < __x; }

  template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator<=(const reverse_iterator<_IteratorL>& __x,
        const reverse_iterator<_IteratorR>& __y)
    { return !(__y < __x); }

  template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator>=(const reverse_iterator<_IteratorL>& __x,
        const reverse_iterator<_IteratorR>& __y)
    { return !(__x < __y); }

  template<typename _IteratorL, typename _IteratorR>
    inline typename reverse_iterator<_IteratorL>::difference_type
    operator-(const reverse_iterator<_IteratorL>& __x,
       const reverse_iterator<_IteratorR>& __y)
    { return __y.base() - __x.base(); }

  template<typename _Container>
    class back_insert_iterator
    : public iterator<output_iterator_tag, void, void, void, void>
    {
    protected:
      _Container* container;

    public:

      typedef _Container container_type;
      explicit
      back_insert_iterator(_Container& __x) : container(&__x) { }

      back_insert_iterator&
      operator=(typename _Container::const_reference __value)
      {
 container->push_back(__value);
 return *this;
      }
      back_insert_iterator&
      operator*()
      { return *this; }
      back_insert_iterator&
      operator++()
      { return *this; }
      back_insert_iterator
      operator++(int)
      { return *this; }
    };

  template<typename _Container>
    inline back_insert_iterator<_Container>
    back_inserter(_Container& __x)
    { return back_insert_iterator<_Container>(__x); }

  template<typename _Container>
    class front_insert_iterator
    : public iterator<output_iterator_tag, void, void, void, void>
    {
    protected:
      _Container* container;

    public:

      typedef _Container container_type;
      explicit front_insert_iterator(_Container& __x) : container(&__x) { }

      front_insert_iterator&
      operator=(typename _Container::const_reference __value)
      {
 container->push_front(__value);
 return *this;
      }
      front_insert_iterator&
      operator*()
      { return *this; }
      front_insert_iterator&
      operator++()
      { return *this; }
      front_insert_iterator
      operator++(int)
      { return *this; }
    };

  template<typename _Container>
    inline front_insert_iterator<_Container>
    front_inserter(_Container& __x)
    { return front_insert_iterator<_Container>(__x); }

  template<typename _Container>
    class insert_iterator
    : public iterator<output_iterator_tag, void, void, void, void>
    {
    protected:
      _Container* container;
      typename _Container::iterator iter;

    public:

      typedef _Container container_type;

      insert_iterator(_Container& __x, typename _Container::iterator __i)
      : container(&__x), iter(__i) {}

      insert_iterator&
      operator=(const typename _Container::const_reference __value)
      {
 iter = container->insert(iter, __value);
 ++iter;
 return *this;
      }
      insert_iterator&
      operator*()
      { return *this; }
      insert_iterator&
      operator++()
      { return *this; }
      insert_iterator&
      operator++(int)
      { return *this; }
    };

  template<typename _Container, typename _Iterator>
    inline insert_iterator<_Container>
    inserter(_Container& __x, _Iterator __i)
    {
      return insert_iterator<_Container>(__x,
      typename _Container::iterator(__i));
    }

}

namespace __gnu_cxx {

  using std::iterator_traits;
  using std::iterator;
  template<typename _Iterator, typename _Container>
    class __normal_iterator
    {
    protected:
      _Iterator _M_current;

    public:
      typedef typename iterator_traits<_Iterator>::iterator_category
                                                             iterator_category;
      typedef typename iterator_traits<_Iterator>::value_type value_type;
      typedef typename iterator_traits<_Iterator>::difference_type
                                                             difference_type;
      typedef typename iterator_traits<_Iterator>::reference reference;
      typedef typename iterator_traits<_Iterator>::pointer pointer;

      __normal_iterator() : _M_current(_Iterator()) { }

      explicit
      __normal_iterator(const _Iterator& __i) : _M_current(__i) { }
      template<typename _Iter>
        __normal_iterator(const __normal_iterator<_Iter,
     typename __enable_if<
              (std::__are_same<_Iter, typename _Container::pointer>::__value),
        _Container>::__type>& __i)
        : _M_current(__i.base()) { }
      reference
      operator*() const
      { return *_M_current; }

      pointer
      operator->() const
      { return _M_current; }

      __normal_iterator&
      operator++()
      {
 ++_M_current;
 return *this;
      }

      __normal_iterator
      operator++(int)
      { return __normal_iterator(_M_current++); }
      __normal_iterator&
      operator--()
      {
 --_M_current;
 return *this;
      }

      __normal_iterator
      operator--(int)
      { return __normal_iterator(_M_current--); }
      reference
      operator[](const difference_type& __n) const
      { return _M_current[__n]; }

      __normal_iterator&
      operator+=(const difference_type& __n)
      { _M_current += __n; return *this; }

      __normal_iterator
      operator+(const difference_type& __n) const
      { return __normal_iterator(_M_current + __n); }

      __normal_iterator&
      operator-=(const difference_type& __n)
      { _M_current -= __n; return *this; }

      __normal_iterator
      operator-(const difference_type& __n) const
      { return __normal_iterator(_M_current - __n); }

      const _Iterator&
      base() const
      { return _M_current; }
    };

  template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator==(const __normal_iterator<_IteratorL, _Container>& __lhs,
        const __normal_iterator<_IteratorR, _Container>& __rhs)
    { return __lhs.base() == __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline bool
    operator==(const __normal_iterator<_Iterator, _Container>& __lhs,
        const __normal_iterator<_Iterator, _Container>& __rhs)
    { return __lhs.base() == __rhs.base(); }

  template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator!=(const __normal_iterator<_IteratorL, _Container>& __lhs,
        const __normal_iterator<_IteratorR, _Container>& __rhs)
    { return __lhs.base() != __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline bool
    operator!=(const __normal_iterator<_Iterator, _Container>& __lhs,
        const __normal_iterator<_Iterator, _Container>& __rhs)
    { return __lhs.base() != __rhs.base(); }
  template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator<(const __normal_iterator<_IteratorL, _Container>& __lhs,
       const __normal_iterator<_IteratorR, _Container>& __rhs)
    { return __lhs.base() < __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline bool
    operator<(const __normal_iterator<_Iterator, _Container>& __lhs,
       const __normal_iterator<_Iterator, _Container>& __rhs)
    { return __lhs.base() < __rhs.base(); }

  template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator>(const __normal_iterator<_IteratorL, _Container>& __lhs,
       const __normal_iterator<_IteratorR, _Container>& __rhs)
    { return __lhs.base() > __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline bool
    operator>(const __normal_iterator<_Iterator, _Container>& __lhs,
       const __normal_iterator<_Iterator, _Container>& __rhs)
    { return __lhs.base() > __rhs.base(); }

  template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator<=(const __normal_iterator<_IteratorL, _Container>& __lhs,
        const __normal_iterator<_IteratorR, _Container>& __rhs)
    { return __lhs.base() <= __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline bool
    operator<=(const __normal_iterator<_Iterator, _Container>& __lhs,
        const __normal_iterator<_Iterator, _Container>& __rhs)
    { return __lhs.base() <= __rhs.base(); }

  template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator>=(const __normal_iterator<_IteratorL, _Container>& __lhs,
        const __normal_iterator<_IteratorR, _Container>& __rhs)
    { return __lhs.base() >= __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline bool
    operator>=(const __normal_iterator<_Iterator, _Container>& __lhs,
        const __normal_iterator<_Iterator, _Container>& __rhs)
    { return __lhs.base() >= __rhs.base(); }

  template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline typename __normal_iterator<_IteratorL, _Container>::difference_type
    operator-(const __normal_iterator<_IteratorL, _Container>& __lhs,
       const __normal_iterator<_IteratorR, _Container>& __rhs)
    { return __lhs.base() - __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline typename __normal_iterator<_Iterator, _Container>::difference_type
    operator-(const __normal_iterator<_Iterator, _Container>& __lhs,
       const __normal_iterator<_Iterator, _Container>& __rhs)
    { return __lhs.base() - __rhs.base(); }

  template<typename _Iterator, typename _Container>
    inline __normal_iterator<_Iterator, _Container>
    operator+(typename __normal_iterator<_Iterator, _Container>::difference_type
       __n, const __normal_iterator<_Iterator, _Container>& __i)
    { return __normal_iterator<_Iterator, _Container>(__i.base() + __n); }

}
namespace std
{
  namespace __debug { }
}

namespace __gnu_cxx
{
  namespace __debug { };
}

namespace __gnu_debug
{
  using namespace std::__debug;
  using namespace __gnu_cxx::__debug;
}
namespace std {

  template<typename _Tp>
    inline void
    swap(_Tp& __a, _Tp& __b)
    {

     

      _Tp __tmp = __a;
      __a = __b;
      __b = __tmp;
    }
  template<bool _BoolType>
    struct __iter_swap
    {
      template<typename _ForwardIterator1, typename _ForwardIterator2>
        static void
        iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b)
        {
          typedef typename iterator_traits<_ForwardIterator1>::value_type
            _ValueType1;
          _ValueType1 __tmp = *__a;
          *__a = *__b;
          *__b = __tmp;
 }
    };

  template<>
    struct __iter_swap<true>
    {
      template<typename _ForwardIterator1, typename _ForwardIterator2>
        static void
        iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b)
        {
          swap(*__a, *__b);
        }
    };

  template<typename _ForwardIterator1, typename _ForwardIterator2>
    inline void
    iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b)
    {
      typedef typename iterator_traits<_ForwardIterator1>::value_type
 _ValueType1;
      typedef typename iterator_traits<_ForwardIterator2>::value_type
 _ValueType2;
     

     

     

     
      typedef typename iterator_traits<_ForwardIterator1>::reference
 _ReferenceType1;
      typedef typename iterator_traits<_ForwardIterator2>::reference
 _ReferenceType2;
      std::__iter_swap<__are_same<_ValueType1, _ValueType2>::__value &&
 __are_same<_ValueType1 &, _ReferenceType1>::__value &&
 __are_same<_ValueType2 &, _ReferenceType2>::__value>::
 iter_swap(__a, __b);
    }

  template<typename _Tp>
    inline const _Tp&
    min(const _Tp& __a, const _Tp& __b)
    {

     

      if (__b < __a)
 return __b;
      return __a;
    }

  template<typename _Tp>
    inline const _Tp&
    max(const _Tp& __a, const _Tp& __b)
    {

     

      if (__a < __b)
 return __b;
      return __a;
    }

  template<typename _Tp, typename _Compare>
    inline const _Tp&
    min(const _Tp& __a, const _Tp& __b, _Compare __comp)
    {

      if (__comp(__b, __a))
 return __b;
      return __a;
    }

  template<typename _Tp, typename _Compare>
    inline const _Tp&
    max(const _Tp& __a, const _Tp& __b, _Compare __comp)
    {

      if (__comp(__a, __b))
 return __b;
      return __a;
    }

  template<bool, typename>
    struct __copy
    {
      template<typename _II, typename _OI>
        static _OI
        copy(_II __first, _II __last, _OI __result)
        {
   for (; __first != __last; ++__result, ++__first)
     *__result = *__first;
   return __result;
 }
    };

  template<bool _BoolType>
    struct __copy<_BoolType, random_access_iterator_tag>
    {
      template<typename _II, typename _OI>
        static _OI
        copy(_II __first, _II __last, _OI __result)
        {
   typedef typename iterator_traits<_II>::difference_type _Distance;
   for(_Distance __n = __last - __first; __n > 0; --__n)
     {
       *__result = *__first;
       ++__first;
       ++__result;
     }
   return __result;
 }
    };

  template<>
    struct __copy<true, random_access_iterator_tag>
    {
      template<typename _Tp>
        static _Tp*
        copy(const _Tp* __first, const _Tp* __last, _Tp* __result)
        {
   std::memmove(__result, __first, sizeof(_Tp) * (__last - __first));
   return __result + (__last - __first);
 }
    };

  template<typename _II, typename _OI>
    inline _OI
    __copy_aux(_II __first, _II __last, _OI __result)
    {
      typedef typename iterator_traits<_II>::value_type _ValueTypeI;
      typedef typename iterator_traits<_OI>::value_type _ValueTypeO;
      typedef typename iterator_traits<_II>::iterator_category _Category;
      const bool __simple = (__is_scalar<_ValueTypeI>::__value
                      && __is_pointer<_II>::__value
                      && __is_pointer<_OI>::__value
        && __are_same<_ValueTypeI, _ValueTypeO>::__value);

      return std::__copy<__simple, _Category>::copy(__first, __last, __result);
    }
  template<typename _CharT>
  typename __gnu_cxx::__enable_if<__is_char<_CharT>::__value,
      ostreambuf_iterator<_CharT> >::__type
    __copy_aux(_CharT*, _CharT*, ostreambuf_iterator<_CharT>);

  template<typename _CharT>
    typename __gnu_cxx::__enable_if<__is_char<_CharT>::__value,
        ostreambuf_iterator<_CharT> >::__type
    __copy_aux(const _CharT*, const _CharT*, ostreambuf_iterator<_CharT>);

  template<typename _CharT>
  typename __gnu_cxx::__enable_if<__is_char<_CharT>::__value, _CharT*>::__type
    __copy_aux(istreambuf_iterator<_CharT>, istreambuf_iterator<_CharT>,
        _CharT*);

  template<bool, bool>
    struct __copy_normal
    {
      template<typename _II, typename _OI>
        static _OI
        __copy_n(_II __first, _II __last, _OI __result)
        { return std::__copy_aux(__first, __last, __result); }
    };

  template<>
    struct __copy_normal<true, false>
    {
      template<typename _II, typename _OI>
        static _OI
        __copy_n(_II __first, _II __last, _OI __result)
        { return std::__copy_aux(__first.base(), __last.base(), __result); }
    };

  template<>
    struct __copy_normal<false, true>
    {
      template<typename _II, typename _OI>
        static _OI
        __copy_n(_II __first, _II __last, _OI __result)
        { return _OI(std::__copy_aux(__first, __last, __result.base())); }
    };

  template<>
    struct __copy_normal<true, true>
    {
      template<typename _II, typename _OI>
        static _OI
        __copy_n(_II __first, _II __last, _OI __result)
        { return _OI(std::__copy_aux(__first.base(), __last.base(),
         __result.base())); }
    };

  template<typename _InputIterator, typename _OutputIterator>
    inline _OutputIterator
    copy(_InputIterator __first, _InputIterator __last,
  _OutputIterator __result)
    {

     
     

      ;

       const bool __in = __is_normal_iterator<_InputIterator>::__value;
       const bool __out = __is_normal_iterator<_OutputIterator>::__value;
       return std::__copy_normal<__in, __out>::__copy_n(__first, __last,
       __result);
    }
  template<typename _CharT>
    typename __gnu_cxx::__enable_if<__is_char<_CharT>::__value,
                 ostreambuf_iterator<_CharT> >::__type
    copy(istreambuf_iterator<_CharT>, istreambuf_iterator<_CharT>,
  ostreambuf_iterator<_CharT>);

  template<bool, typename>
    struct __copy_backward
    {
      template<typename _BI1, typename _BI2>
        static _BI2
        __copy_b(_BI1 __first, _BI1 __last, _BI2 __result)
        {
   while (__first != __last)
     *--__result = *--__last;
   return __result;
 }
    };

  template<bool _BoolType>
    struct __copy_backward<_BoolType, random_access_iterator_tag>
    {
      template<typename _BI1, typename _BI2>
        static _BI2
        __copy_b(_BI1 __first, _BI1 __last, _BI2 __result)
        {
   typename iterator_traits<_BI1>::difference_type __n;
   for (__n = __last - __first; __n > 0; --__n)
     *--__result = *--__last;
   return __result;
 }
    };

  template<>
    struct __copy_backward<true, random_access_iterator_tag>
    {
      template<typename _Tp>
        static _Tp*
        __copy_b(const _Tp* __first, const _Tp* __last, _Tp* __result)
        {
   const ptrdiff_t _Num = __last - __first;
   std::memmove(__result - _Num, __first, sizeof(_Tp) * _Num);
   return __result - _Num;
 }
    };

  template<typename _BI1, typename _BI2>
    inline _BI2
    __copy_backward_aux(_BI1 __first, _BI1 __last, _BI2 __result)
    {
      typedef typename iterator_traits<_BI1>::value_type _ValueType1;
      typedef typename iterator_traits<_BI2>::value_type _ValueType2;
      typedef typename iterator_traits<_BI1>::iterator_category _Category;
      const bool __simple = (__is_scalar<_ValueType1>::__value
                      && __is_pointer<_BI1>::__value
                      && __is_pointer<_BI2>::__value
        && __are_same<_ValueType1, _ValueType2>::__value);

      return std::__copy_backward<__simple, _Category>::__copy_b(__first,
         __last,
         __result);
    }

  template<bool, bool>
    struct __copy_backward_normal
    {
      template<typename _BI1, typename _BI2>
        static _BI2
        __copy_b_n(_BI1 __first, _BI1 __last, _BI2 __result)
        { return std::__copy_backward_aux(__first, __last, __result); }
    };

  template<>
    struct __copy_backward_normal<true, false>
    {
      template<typename _BI1, typename _BI2>
        static _BI2
        __copy_b_n(_BI1 __first, _BI1 __last, _BI2 __result)
        { return std::__copy_backward_aux(__first.base(), __last.base(),
       __result); }
    };

  template<>
    struct __copy_backward_normal<false, true>
    {
      template<typename _BI1, typename _BI2>
        static _BI2
        __copy_b_n(_BI1 __first, _BI1 __last, _BI2 __result)
        { return _BI2(std::__copy_backward_aux(__first, __last,
            __result.base())); }
    };

  template<>
    struct __copy_backward_normal<true, true>
    {
      template<typename _BI1, typename _BI2>
        static _BI2
        __copy_b_n(_BI1 __first, _BI1 __last, _BI2 __result)
        { return _BI2(std::__copy_backward_aux(__first.base(), __last.base(),
            __result.base())); }
    };

  template <typename _BI1, typename _BI2>
    inline _BI2
    copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
    {

     
     
     
      ;

      const bool __bi1 = __is_normal_iterator<_BI1>::__value;
      const bool __bi2 = __is_normal_iterator<_BI2>::__value;
      return std::__copy_backward_normal<__bi1, __bi2>::__copy_b_n(__first,
           __last,
           __result);
    }

  template<bool>
    struct __fill
    {
      template<typename _ForwardIterator, typename _Tp>
        static void
        fill(_ForwardIterator __first, _ForwardIterator __last,
      const _Tp& __value)
        {
   for (; __first != __last; ++__first)
     *__first = __value;
 }
    };

  template<>
    struct __fill<true>
    {
      template<typename _ForwardIterator, typename _Tp>
        static void
        fill(_ForwardIterator __first, _ForwardIterator __last,
      const _Tp& __value)
        {
   const _Tp __tmp = __value;
   for (; __first != __last; ++__first)
     *__first = __tmp;
 }
    };

  template<typename _ForwardIterator, typename _Tp>
    void
    fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value)
    {

     

      ;

      const bool __scalar = __is_scalar<_Tp>::__value;
      std::__fill<__scalar>::fill(__first, __last, __value);
    }
  inline void
  fill(unsigned char* __first, unsigned char* __last, const unsigned char& __c)
  {
    ;
    const unsigned char __tmp = __c;
    std::memset(__first, __tmp, __last - __first);
  }

  inline void
  fill(signed char* __first, signed char* __last, const signed char& __c)
  {
    ;
    const signed char __tmp = __c;
    std::memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
  }

  inline void
  fill(char* __first, char* __last, const char& __c)
  {
    ;
    const char __tmp = __c;
    std::memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
  }

  template<bool>
    struct __fill_n
    {
      template<typename _OutputIterator, typename _Size, typename _Tp>
        static _OutputIterator
        fill_n(_OutputIterator __first, _Size __n, const _Tp& __value)
        {
   for (; __n > 0; --__n, ++__first)
     *__first = __value;
   return __first;
 }
    };

  template<>
    struct __fill_n<true>
    {
      template<typename _OutputIterator, typename _Size, typename _Tp>
        static _OutputIterator
        fill_n(_OutputIterator __first, _Size __n, const _Tp& __value)
        {
   const _Tp __tmp = __value;
   for (; __n > 0; --__n, ++__first)
     *__first = __tmp;
   return __first;
 }
    };

  template<typename _OutputIterator, typename _Size, typename _Tp>
    _OutputIterator
    fill_n(_OutputIterator __first, _Size __n, const _Tp& __value)
    {

     

      const bool __scalar = __is_scalar<_Tp>::__value;
      return std::__fill_n<__scalar>::fill_n(__first, __n, __value);
    }

  template<typename _Size>
    inline unsigned char*
    fill_n(unsigned char* __first, _Size __n, const unsigned char& __c)
    {
      std::fill(__first, __first + __n, __c);
      return __first + __n;
    }

  template<typename _Size>
    inline signed char*
    fill_n(signed char* __first, _Size __n, const signed char& __c)
    {
      std::fill(__first, __first + __n, __c);
      return __first + __n;
    }

  template<typename _Size>
    inline char*
    fill_n(char* __first, _Size __n, const char& __c)
    {
      std::fill(__first, __first + __n, __c);
      return __first + __n;
    }

  template<typename _InputIterator1, typename _InputIterator2>
    pair<_InputIterator1, _InputIterator2>
    mismatch(_InputIterator1 __first1, _InputIterator1 __last1,
      _InputIterator2 __first2)
    {

     
     
     
      ;

      while (__first1 != __last1 && *__first1 == *__first2)
        {
   ++__first1;
   ++__first2;
        }
      return pair<_InputIterator1, _InputIterator2>(__first1, __first2);
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _BinaryPredicate>
    pair<_InputIterator1, _InputIterator2>
    mismatch(_InputIterator1 __first1, _InputIterator1 __last1,
      _InputIterator2 __first2, _BinaryPredicate __binary_pred)
    {

     
     
      ;

      while (__first1 != __last1 && __binary_pred(*__first1, *__first2))
        {
   ++__first1;
   ++__first2;
        }
      return pair<_InputIterator1, _InputIterator2>(__first1, __first2);
    }

  template<typename _InputIterator1, typename _InputIterator2>
    inline bool
    equal(_InputIterator1 __first1, _InputIterator1 __last1,
   _InputIterator2 __first2)
    {

     
     
     
      ;

      for (; __first1 != __last1; ++__first1, ++__first2)
 if (!(*__first1 == *__first2))
   return false;
      return true;
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _BinaryPredicate>
    inline bool
    equal(_InputIterator1 __first1, _InputIterator1 __last1,
   _InputIterator2 __first2,
   _BinaryPredicate __binary_pred)
    {

     
     
      ;

      for (; __first1 != __last1; ++__first1, ++__first2)
 if (!__binary_pred(*__first1, *__first2))
   return false;
      return true;
    }

  template<typename _InputIterator1, typename _InputIterator2>
    bool
    lexicographical_compare(_InputIterator1 __first1, _InputIterator1 __last1,
       _InputIterator2 __first2, _InputIterator2 __last2)
    {

     
     
     
     
      ;
      ;

      for (; __first1 != __last1 && __first2 != __last2;
    ++__first1, ++__first2)
 {
   if (*__first1 < *__first2)
     return true;
   if (*__first2 < *__first1)
     return false;
 }
      return __first1 == __last1 && __first2 != __last2;
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _Compare>
    bool
    lexicographical_compare(_InputIterator1 __first1, _InputIterator1 __last1,
       _InputIterator2 __first2, _InputIterator2 __last2,
       _Compare __comp)
    {

     
     
      ;
      ;

      for (; __first1 != __last1 && __first2 != __last2;
    ++__first1, ++__first2)
 {
   if (__comp(*__first1, *__first2))
     return true;
   if (__comp(*__first2, *__first1))
     return false;
 }
      return __first1 == __last1 && __first2 != __last2;
    }

  inline bool
  lexicographical_compare(const unsigned char* __first1,
     const unsigned char* __last1,
     const unsigned char* __first2,
     const unsigned char* __last2)
  {
    ;
    ;

    const size_t __len1 = __last1 - __first1;
    const size_t __len2 = __last2 - __first2;
    const int __result = std::memcmp(__first1, __first2,
         std::min(__len1, __len2));
    return __result != 0 ? __result < 0 : __len1 < __len2;
  }

  inline bool
  lexicographical_compare(const char* __first1, const char* __last1,
     const char* __first2, const char* __last2)
  {
    ;
    ;
    return std::lexicographical_compare((const signed char*) __first1,
     (const signed char*) __last1,
     (const signed char*) __first2,
     (const signed char*) __last2);
  }

}

namespace __gnu_cxx {

  template <class _CharT>
    struct _Char_types
    {
      typedef unsigned long int_type;
      typedef std::streampos pos_type;
      typedef std::streamoff off_type;
      typedef std::mbstate_t state_type;
    };

  template<typename _CharT>
    struct char_traits
    {
      typedef _CharT char_type;
      typedef typename _Char_types<_CharT>::int_type int_type;
      typedef typename _Char_types<_CharT>::pos_type pos_type;
      typedef typename _Char_types<_CharT>::off_type off_type;
      typedef typename _Char_types<_CharT>::state_type state_type;

      static void
      assign(char_type& __c1, const char_type& __c2)
      { __c1 = __c2; }

      static bool
      eq(const char_type& __c1, const char_type& __c2)
      { return __c1 == __c2; }

      static bool
      lt(const char_type& __c1, const char_type& __c2)
      { return __c1 < __c2; }

      static int
      compare(const char_type* __s1, const char_type* __s2, std::size_t __n);

      static std::size_t
      length(const char_type* __s);

      static const char_type*
      find(const char_type* __s, std::size_t __n, const char_type& __a);

      static char_type*
      move(char_type* __s1, const char_type* __s2, std::size_t __n);

      static char_type*
      copy(char_type* __s1, const char_type* __s2, std::size_t __n);

      static char_type*
      assign(char_type* __s, std::size_t __n, char_type __a);

      static char_type
      to_char_type(const int_type& __c)
      { return static_cast<char_type>(__c); }

      static int_type
      to_int_type(const char_type& __c)
      { return static_cast<int_type>(__c); }

      static bool
      eq_int_type(const int_type& __c1, const int_type& __c2)
      { return __c1 == __c2; }

      static int_type
      eof()
      { return static_cast<int_type>((-1)); }

      static int_type
      not_eof(const int_type& __c)
      { return !eq_int_type(__c, eof()) ? __c : to_int_type(char_type()); }
    };

  template<typename _CharT>
    int
    char_traits<_CharT>::
    compare(const char_type* __s1, const char_type* __s2, std::size_t __n)
    {
      for (std::size_t __i = 0; __i < __n; ++__i)
 if (lt(__s1[__i], __s2[__i]))
   return -1;
 else if (lt(__s2[__i], __s1[__i]))
   return 1;
      return 0;
    }

  template<typename _CharT>
    std::size_t
    char_traits<_CharT>::
    length(const char_type* __p)
    {
      std::size_t __i = 0;
      while (!eq(__p[__i], char_type()))
        ++__i;
      return __i;
    }

  template<typename _CharT>
    const typename char_traits<_CharT>::char_type*
    char_traits<_CharT>::
    find(const char_type* __s, std::size_t __n, const char_type& __a)
    {
      for (std::size_t __i = 0; __i < __n; ++__i)
        if (eq(__s[__i], __a))
          return __s + __i;
      return 0;
    }

  template<typename _CharT>
    typename char_traits<_CharT>::char_type*
    char_traits<_CharT>::
    move(char_type* __s1, const char_type* __s2, std::size_t __n)
    {
      return static_cast<_CharT*>(std::memmove(__s1, __s2,
            __n * sizeof(char_type)));
    }

  template<typename _CharT>
    typename char_traits<_CharT>::char_type*
    char_traits<_CharT>::
    copy(char_type* __s1, const char_type* __s2, std::size_t __n)
    {
      std::copy(__s2, __s2 + __n, __s1);
      return __s1;
    }

  template<typename _CharT>
    typename char_traits<_CharT>::char_type*
    char_traits<_CharT>::
    assign(char_type* __s, std::size_t __n, char_type __a)
    {
      std::fill_n(__s, __n, __a);
      return __s;
    }

}

namespace std {

  template<class _CharT>
    struct char_traits : public __gnu_cxx::char_traits<_CharT>
    { };

  template<>
    struct char_traits<char>
    {
      typedef char char_type;
      typedef int int_type;
      typedef streampos pos_type;
      typedef streamoff off_type;
      typedef mbstate_t state_type;

      static void
      assign(char_type& __c1, const char_type& __c2)
      { __c1 = __c2; }

      static bool
      eq(const char_type& __c1, const char_type& __c2)
      { return __c1 == __c2; }

      static bool
      lt(const char_type& __c1, const char_type& __c2)
      { return __c1 < __c2; }

      static int
      compare(const char_type* __s1, const char_type* __s2, size_t __n)
      { return memcmp(__s1, __s2, __n); }

      static size_t
      length(const char_type* __s)
      { return strlen(__s); }

      static const char_type*
      find(const char_type* __s, size_t __n, const char_type& __a)
      { return static_cast<const char_type*>(memchr(__s, __a, __n)); }

      static char_type*
      move(char_type* __s1, const char_type* __s2, size_t __n)
      { return static_cast<char_type*>(memmove(__s1, __s2, __n)); }

      static char_type*
      copy(char_type* __s1, const char_type* __s2, size_t __n)
      { return static_cast<char_type*>(memcpy(__s1, __s2, __n)); }

      static char_type*
      assign(char_type* __s, size_t __n, char_type __a)
      { return static_cast<char_type*>(memset(__s, __a, __n)); }

      static char_type
      to_char_type(const int_type& __c)
      { return static_cast<char_type>(__c); }

      static int_type
      to_int_type(const char_type& __c)
      { return static_cast<int_type>(static_cast<unsigned char>(__c)); }

      static bool
      eq_int_type(const int_type& __c1, const int_type& __c2)
      { return __c1 == __c2; }

      static int_type
      eof() { return static_cast<int_type>((-1)); }

      static int_type
      not_eof(const int_type& __c)
      { return (__c == eof()) ? 0 : __c; }
  };
  template<>
    struct char_traits<wchar_t>
    {
      typedef wchar_t char_type;
      typedef wint_t int_type;
      typedef streamoff off_type;
      typedef wstreampos pos_type;
      typedef mbstate_t state_type;

      static void
      assign(char_type& __c1, const char_type& __c2)
      { __c1 = __c2; }

      static bool
      eq(const char_type& __c1, const char_type& __c2)
      { return __c1 == __c2; }

      static bool
      lt(const char_type& __c1, const char_type& __c2)
      { return __c1 < __c2; }

      static int
      compare(const char_type* __s1, const char_type* __s2, size_t __n)
      { return wmemcmp(__s1, __s2, __n); }

      static size_t
      length(const char_type* __s)
      { return wcslen(__s); }

      static const char_type*
      find(const char_type* __s, size_t __n, const char_type& __a)
      { return wmemchr(__s, __a, __n); }

      static char_type*
      move(char_type* __s1, const char_type* __s2, size_t __n)
      { return wmemmove(__s1, __s2, __n); }

      static char_type*
      copy(char_type* __s1, const char_type* __s2, size_t __n)
      { return wmemcpy(__s1, __s2, __n); }

      static char_type*
      assign(char_type* __s, size_t __n, char_type __a)
      { return wmemset(__s, __a, __n); }

      static char_type
      to_char_type(const int_type& __c) { return char_type(__c); }

      static int_type
      to_int_type(const char_type& __c) { return int_type(__c); }

      static bool
      eq_int_type(const int_type& __c1, const int_type& __c2)
      { return __c1 == __c2; }

      static int_type
      eof() { return static_cast<int_type>(((__darwin_wint_t)-1)); }

      static int_type
      not_eof(const int_type& __c)
      { return eq_int_type(__c, eof()) ? 0 : __c; }
  };
}

       

extern "C++" {

namespace std
{

  class exception
  {
  public:
    exception() throw() { }
    virtual ~exception() throw();

    virtual const char* what() const throw();
  };

  class bad_exception : public exception
  {
  public:
    bad_exception() throw() { }

    virtual ~bad_exception() throw();
    virtual const char* what() const throw();
  };
  typedef void (*terminate_handler) ();
  typedef void (*unexpected_handler) ();
  terminate_handler set_terminate(terminate_handler) throw();

  void terminate() __attribute__ ((__noreturn__));
  unexpected_handler set_unexpected(unexpected_handler) throw();

  void unexpected() __attribute__ ((__noreturn__));

  bool uncaught_exception() throw();
}

namespace __gnu_cxx {

  void __verbose_terminate_handler ();

}

}
extern "C++" {

namespace std
{

  class bad_alloc : public exception
  {
  public:
    bad_alloc() throw() { }

    virtual ~bad_alloc() throw();
    virtual const char* what() const throw();
  };

  struct nothrow_t { };

  extern const nothrow_t nothrow;

  typedef void (*new_handler)();

  new_handler set_new_handler(new_handler) throw();
}

void* operator new(std::size_t) throw (std::bad_alloc);
void* operator new[](std::size_t) throw (std::bad_alloc);
void operator delete(void*) throw();
void operator delete[](void*) throw();
void* operator new(std::size_t, const std::nothrow_t&) throw();
void* operator new[](std::size_t, const std::nothrow_t&) throw();
void operator delete(void*, const std::nothrow_t&) throw();
void operator delete[](void*, const std::nothrow_t&) throw();
inline void* operator new(std::size_t, void* __p) throw() { return __p; }
inline void* operator new[](std::size_t, void* __p) throw() { return __p; }
inline void operator delete (void*, void*) throw() { }
inline void operator delete[](void*, void*) throw() { }

}

namespace __gnu_cxx {

  using std::size_t;
  using std::ptrdiff_t;

  template<typename _Tp>
    class new_allocator
    {
    public:
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef _Tp* pointer;
      typedef const _Tp* const_pointer;
      typedef _Tp& reference;
      typedef const _Tp& const_reference;
      typedef _Tp value_type;

      template<typename _Tp1>
        struct rebind
        { typedef new_allocator<_Tp1> other; };

      new_allocator() throw() { }

      new_allocator(const new_allocator&) throw() { }

      template<typename _Tp1>
        new_allocator(const new_allocator<_Tp1>&) throw() { }

      ~new_allocator() throw() { }

      pointer
      address(reference __x) const { return &__x; }

      const_pointer
      address(const_reference __x) const { return &__x; }

      pointer
      allocate(size_type __n, const void* = 0)
      {
 if (__builtin_expect(__n > this->max_size(), false))
   std::__throw_bad_alloc();

 return static_cast<_Tp*>(::operator new(__n * sizeof(_Tp)));
      }
      void
      deallocate(pointer __p, size_type)
      { ::operator delete(__p); }

      size_type
      max_size() const throw()
      { return size_t(-1) / sizeof(_Tp); }

      void
      construct(pointer __p, const _Tp& __val)
      { ::new(__p) _Tp(__val); }

      void
      destroy(pointer __p) { __p->~_Tp(); }
    };

  template<typename _Tp>
    inline bool
    operator==(const new_allocator<_Tp>&, const new_allocator<_Tp>&)
    { return true; }

  template<typename _Tp>
    inline bool
    operator!=(const new_allocator<_Tp>&, const new_allocator<_Tp>&)
    { return false; }

}

namespace std {

  template<typename _Tp>
    class allocator;
  template<>
    class allocator<void>
    {
    public:
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef void* pointer;
      typedef const void* const_pointer;
      typedef void value_type;

      template<typename _Tp1>
        struct rebind
        { typedef allocator<_Tp1> other; };
    };

  template<typename _Tp>
    class allocator: public __gnu_cxx::new_allocator<_Tp>
    {
   public:
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef _Tp* pointer;
      typedef const _Tp* const_pointer;
      typedef _Tp& reference;
      typedef const _Tp& const_reference;
      typedef _Tp value_type;

      template<typename _Tp1>
        struct rebind
        { typedef allocator<_Tp1> other; };

      allocator() throw() { }

      allocator(const allocator& __a) throw()
      : __gnu_cxx::new_allocator<_Tp>(__a) { }

      template<typename _Tp1>
        allocator(const allocator<_Tp1>&) throw() { }

      ~allocator() throw() { }
    };

  template<typename _T1, typename _T2>
    inline bool
    operator==(const allocator<_T1>&, const allocator<_T2>&)
    { return true; }

  template<typename _T1, typename _T2>
    inline bool
    operator!=(const allocator<_T1>&, const allocator<_T2>&)
    { return false; }

  extern template class allocator<char>;
  extern template class allocator<wchar_t>;
  template<typename _Alloc, bool = std::__is_empty<_Alloc>::__value>
    struct __alloc_swap
    { static void _S_do_it(_Alloc&, _Alloc&) { } };

  template<typename _Alloc>
    struct __alloc_swap<_Alloc, false>
    {
      static void
      _S_do_it(_Alloc& __one, _Alloc& __two)
      {

 if (__one != __two)
   swap(__one, __two);
      }
    };

}

namespace std {

  template<typename _T1, typename _T2>
    inline void
    _Construct(_T1* __p, const _T2& __value)
    {
      ::new(static_cast<void*>(__p)) _T1(__value);
    }

  template<typename _T1>
    inline void
    _Construct(_T1* __p)
    {
      ::new(static_cast<void*>(__p)) _T1();
    }
  template<typename _Tp>
    inline void
    _Destroy(_Tp* __pointer)
    { __pointer->~_Tp(); }

  template<typename _ForwardIterator>
    inline void
    __destroy_aux(_ForwardIterator __first, _ForwardIterator __last,
    __false_type)
    {
      for (; __first != __last; ++__first)
 std::_Destroy(&*__first);
    }

  template<typename _ForwardIterator>
    inline void
    __destroy_aux(_ForwardIterator, _ForwardIterator, __true_type)
    { }

  template<typename _ForwardIterator>
    inline void
    _Destroy(_ForwardIterator __first, _ForwardIterator __last)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
                       _Value_type;
      typedef typename std::__is_scalar<_Value_type>::__type
                _Has_trivial_destructor;

      std::__destroy_aux(__first, __last, _Has_trivial_destructor());
    }

  template <typename _Tp> class allocator;

  template<typename _ForwardIterator, typename _Allocator>
    void
    _Destroy(_ForwardIterator __first, _ForwardIterator __last,
      _Allocator __alloc)
    {
      for (; __first != __last; ++__first)
 __alloc.destroy(&*__first);
    }

  template<typename _ForwardIterator, typename _Tp>
    inline void
    _Destroy(_ForwardIterator __first, _ForwardIterator __last,
      allocator<_Tp>)
    {
      _Destroy(__first, __last);
    }

}
namespace std {
  template<typename _InputIterator, typename _ForwardIterator>
    inline _ForwardIterator
    __uninitialized_copy_aux(_InputIterator __first, _InputIterator __last,
        _ForwardIterator __result,
        __true_type)
    { return std::copy(__first, __last, __result); }

  template<typename _InputIterator, typename _ForwardIterator>
    inline _ForwardIterator
    __uninitialized_copy_aux(_InputIterator __first, _InputIterator __last,
        _ForwardIterator __result,
        __false_type)
    {
      _ForwardIterator __cur = __result;
      try
 {
   for (; __first != __last; ++__first, ++__cur)
     std::_Construct(&*__cur, *__first);
   return __cur;
 }
      catch(...)
 {
   std::_Destroy(__result, __cur);
   throw;
 }
    }

  template<typename _InputIterator, typename _ForwardIterator>
    inline _ForwardIterator
    uninitialized_copy(_InputIterator __first, _InputIterator __last,
         _ForwardIterator __result)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type _ValueType;
      typedef typename std::__is_scalar<_ValueType>::__type _Is_POD;
      return std::__uninitialized_copy_aux(__first, __last, __result,
        _Is_POD());
    }

  inline char*
  uninitialized_copy(const char* __first, const char* __last, char* __result)
  {
    std::memmove(__result, __first, __last - __first);
    return __result + (__last - __first);
  }

  inline wchar_t*
  uninitialized_copy(const wchar_t* __first, const wchar_t* __last,
       wchar_t* __result)
  {
    std::memmove(__result, __first, sizeof(wchar_t) * (__last - __first));
    return __result + (__last - __first);
  }

  template<typename _ForwardIterator, typename _Tp>
    inline void
    __uninitialized_fill_aux(_ForwardIterator __first,
        _ForwardIterator __last,
        const _Tp& __x, __true_type)
    { std::fill(__first, __last, __x); }

  template<typename _ForwardIterator, typename _Tp>
    void
    __uninitialized_fill_aux(_ForwardIterator __first, _ForwardIterator __last,
        const _Tp& __x, __false_type)
    {
      _ForwardIterator __cur = __first;
      try
 {
   for (; __cur != __last; ++__cur)
     std::_Construct(&*__cur, __x);
 }
      catch(...)
 {
   std::_Destroy(__first, __cur);
   throw;
 }
    }

  template<typename _ForwardIterator, typename _Tp>
    inline void
    uninitialized_fill(_ForwardIterator __first, _ForwardIterator __last,
         const _Tp& __x)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type _ValueType;
      typedef typename std::__is_scalar<_ValueType>::__type _Is_POD;
      std::__uninitialized_fill_aux(__first, __last, __x, _Is_POD());
    }

  template<typename _ForwardIterator, typename _Size, typename _Tp>
    inline void
    __uninitialized_fill_n_aux(_ForwardIterator __first, _Size __n,
          const _Tp& __x, __true_type)
    { std::fill_n(__first, __n, __x); }

  template<typename _ForwardIterator, typename _Size, typename _Tp>
    void
    __uninitialized_fill_n_aux(_ForwardIterator __first, _Size __n,
          const _Tp& __x, __false_type)
    {
      _ForwardIterator __cur = __first;
      try
 {
   for (; __n > 0; --__n, ++__cur)
     std::_Construct(&*__cur, __x);
 }
      catch(...)
 {
   std::_Destroy(__first, __cur);
   throw;
 }
    }

  template<typename _ForwardIterator, typename _Size, typename _Tp>
    inline void
    uninitialized_fill_n(_ForwardIterator __first, _Size __n, const _Tp& __x)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type _ValueType;
      typedef typename std::__is_scalar<_ValueType>::__type _Is_POD;
      std::__uninitialized_fill_n_aux(__first, __n, __x, _Is_POD());
    }

  template<typename _InputIterator, typename _ForwardIterator,
    typename _Allocator>
    _ForwardIterator
    __uninitialized_copy_a(_InputIterator __first, _InputIterator __last,
      _ForwardIterator __result,
      _Allocator __alloc)
    {
      _ForwardIterator __cur = __result;
      try
 {
   for (; __first != __last; ++__first, ++__cur)
     __alloc.construct(&*__cur, *__first);
   return __cur;
 }
      catch(...)
 {
   std::_Destroy(__result, __cur, __alloc);
   throw;
 }
    }

  template<typename _InputIterator, typename _ForwardIterator, typename _Tp>
    inline _ForwardIterator
    __uninitialized_copy_a(_InputIterator __first, _InputIterator __last,
      _ForwardIterator __result,
      allocator<_Tp>)
    { return std::uninitialized_copy(__first, __last, __result); }

  template<typename _ForwardIterator, typename _Tp, typename _Allocator>
    void
    __uninitialized_fill_a(_ForwardIterator __first, _ForwardIterator __last,
      const _Tp& __x, _Allocator __alloc)
    {
      _ForwardIterator __cur = __first;
      try
 {
   for (; __cur != __last; ++__cur)
     __alloc.construct(&*__cur, __x);
 }
      catch(...)
 {
   std::_Destroy(__first, __cur, __alloc);
   throw;
 }
    }

  template<typename _ForwardIterator, typename _Tp, typename _Tp2>
    inline void
    __uninitialized_fill_a(_ForwardIterator __first, _ForwardIterator __last,
      const _Tp& __x, allocator<_Tp2>)
    { std::uninitialized_fill(__first, __last, __x); }

  template<typename _ForwardIterator, typename _Size, typename _Tp,
    typename _Allocator>
    void
    __uninitialized_fill_n_a(_ForwardIterator __first, _Size __n,
        const _Tp& __x,
        _Allocator __alloc)
    {
      _ForwardIterator __cur = __first;
      try
 {
   for (; __n > 0; --__n, ++__cur)
     __alloc.construct(&*__cur, __x);
 }
      catch(...)
 {
   std::_Destroy(__first, __cur, __alloc);
   throw;
 }
    }

  template<typename _ForwardIterator, typename _Size, typename _Tp,
    typename _Tp2>
    inline void
    __uninitialized_fill_n_a(_ForwardIterator __first, _Size __n,
        const _Tp& __x,
        allocator<_Tp2>)
    { std::uninitialized_fill_n(__first, __n, __x); }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _ForwardIterator, typename _Allocator>
    inline _ForwardIterator
    __uninitialized_copy_copy(_InputIterator1 __first1,
         _InputIterator1 __last1,
         _InputIterator2 __first2,
         _InputIterator2 __last2,
         _ForwardIterator __result,
         _Allocator __alloc)
    {
      _ForwardIterator __mid = std::__uninitialized_copy_a(__first1, __last1,
          __result,
          __alloc);
      try
 {
   return std::__uninitialized_copy_a(__first2, __last2, __mid, __alloc);
 }
      catch(...)
 {
   std::_Destroy(__result, __mid, __alloc);
   throw;
 }
    }
  template<typename _ForwardIterator, typename _Tp, typename _InputIterator,
    typename _Allocator>
    inline _ForwardIterator
    __uninitialized_fill_copy(_ForwardIterator __result, _ForwardIterator __mid,
         const _Tp& __x, _InputIterator __first,
         _InputIterator __last,
         _Allocator __alloc)
    {
      std::__uninitialized_fill_a(__result, __mid, __x, __alloc);
      try
 {
   return std::__uninitialized_copy_a(__first, __last, __mid, __alloc);
 }
      catch(...)
 {
   std::_Destroy(__result, __mid, __alloc);
   throw;
 }
    }
  template<typename _InputIterator, typename _ForwardIterator, typename _Tp,
    typename _Allocator>
    inline void
    __uninitialized_copy_fill(_InputIterator __first1, _InputIterator __last1,
         _ForwardIterator __first2,
         _ForwardIterator __last2, const _Tp& __x,
         _Allocator __alloc)
    {
      _ForwardIterator __mid2 = std::__uninitialized_copy_a(__first1, __last1,
           __first2,
           __alloc);
      try
 {
   std::__uninitialized_fill_a(__mid2, __last2, __x, __alloc);
 }
      catch(...)
 {
   std::_Destroy(__first2, __mid2, __alloc);
   throw;
 }
    }

}

namespace std {

  template <class _ForwardIterator, class _Tp>
    class raw_storage_iterator
    : public iterator<output_iterator_tag, void, void, void, void>
    {
    protected:
      _ForwardIterator _M_iter;

    public:
      explicit
      raw_storage_iterator(_ForwardIterator __x)
      : _M_iter(__x) {}

      raw_storage_iterator&
      operator*() { return *this; }

      raw_storage_iterator&
      operator=(const _Tp& __element)
      {
 std::_Construct(&*_M_iter, __element);
 return *this;
      }

      raw_storage_iterator<_ForwardIterator, _Tp>&
      operator++()
      {
 ++_M_iter;
 return *this;
      }

      raw_storage_iterator<_ForwardIterator, _Tp>
      operator++(int)
      {
 raw_storage_iterator<_ForwardIterator, _Tp> __tmp = *this;
 ++_M_iter;
 return __tmp;
      }
    };

}
       
namespace std {
  enum float_round_style
  {
    round_indeterminate = -1,
    round_toward_zero = 0,
    round_to_nearest = 1,
    round_toward_infinity = 2,
    round_toward_neg_infinity = 3
  };

  enum float_denorm_style
  {

    denorm_indeterminate = -1,

    denorm_absent = 0,

    denorm_present = 1
  };

  struct __numeric_limits_base
  {
    static const bool is_specialized = false;
    static const int digits = 0;

    static const int digits10 = 0;

    static const bool is_signed = false;

    static const bool is_integer = false;
    static const bool is_exact = false;
    static const int radix = 0;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;

    static const int max_exponent = 0;
    static const int max_exponent10 = 0;
    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;

    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const bool is_iec559 = false;

    static const bool is_bounded = false;
    static const bool is_modulo = false;
    static const bool traps = false;

    static const bool tinyness_before = false;

    static const float_round_style round_style = round_toward_zero;
  };

  template<typename _Tp>
    struct numeric_limits : public __numeric_limits_base
    {
      static _Tp min() throw() { return static_cast<_Tp>(0); }

      static _Tp max() throw() { return static_cast<_Tp>(0); }
      static _Tp epsilon() throw() { return static_cast<_Tp>(0); }

      static _Tp round_error() throw() { return static_cast<_Tp>(0); }

      static _Tp infinity() throw() { return static_cast<_Tp>(0); }

      static _Tp quiet_NaN() throw() { return static_cast<_Tp>(0); }
      static _Tp signaling_NaN() throw() { return static_cast<_Tp>(0); }

      static _Tp denorm_min() throw() { return static_cast<_Tp>(0); }
    };

  template<>
    struct numeric_limits<bool>
    {
      static const bool is_specialized = true;

      static bool min() throw()
      { return false; }
      static bool max() throw()
      { return true; }

      static const int digits = 1;
      static const int digits10 = 0;
      static const bool is_signed = false;
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static bool epsilon() throw()
      { return false; }
      static bool round_error() throw()
      { return false; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = false;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static bool infinity() throw()
      { return false; }
      static bool quiet_NaN() throw()
      { return false; }
      static bool signaling_NaN() throw()
      { return false; }
      static bool denorm_min() throw()
      { return false; }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = false;
      static const bool traps = true;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
  template<>
    struct numeric_limits<char>
    {
      static const bool is_specialized = true;

      static char min() throw()
      { return (((char)(-1) < 0) ? (char)1 << (sizeof(char) * 8 - ((char)(-1) < 0)) : (char)0); }
      static char max() throw()
      { return (((char)(-1) < 0) ? ((char)1 << (sizeof(char) * 8 - ((char)(-1) < 0))) - 1 : ~(char)0); }

      static const int digits = (sizeof(char) * 8 - ((char)(-1) < 0));
      static const int digits10 = ((sizeof(char) * 8 - ((char)(-1) < 0)) * 643 / 2136);
      static const bool is_signed = ((char)(-1) < 0);
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static char epsilon() throw()
      { return 0; }
      static char round_error() throw()
      { return 0; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = false;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static char infinity() throw()
      { return char(); }
      static char quiet_NaN() throw()
      { return char(); }
      static char signaling_NaN() throw()
      { return char(); }
      static char denorm_min() throw()
      { return static_cast<char>(0); }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = true;

      static const bool traps = true;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
  template<>
    struct numeric_limits<signed char>
    {
      static const bool is_specialized = true;

      static signed char min() throw()
      { return -127 - 1; }
      static signed char max() throw()
      { return 127; }

      static const int digits = (sizeof(signed char) * 8 - ((signed char)(-1) < 0));
      static const int digits10 = ((sizeof(signed char) * 8 - ((signed char)(-1) < 0)) * 643 / 2136);
      static const bool is_signed = true;
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static signed char epsilon() throw()
      { return 0; }
      static signed char round_error() throw()
      { return 0; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = false;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static signed char infinity() throw()
      { return static_cast<signed char>(0); }
      static signed char quiet_NaN() throw()
      { return static_cast<signed char>(0); }
      static signed char signaling_NaN() throw()
      { return static_cast<signed char>(0); }
      static signed char denorm_min() throw()
      { return static_cast<signed char>(0); }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = true;

      static const bool traps = true;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
  template<>
    struct numeric_limits<unsigned char>
    {
      static const bool is_specialized = true;

      static unsigned char min() throw()
      { return 0; }
      static unsigned char max() throw()
      { return 127 * 2U + 1; }

      static const int digits = (sizeof(unsigned char) * 8 - ((unsigned char)(-1) < 0));
      static const int digits10 = ((sizeof(unsigned char) * 8 - ((unsigned char)(-1) < 0)) * 643 / 2136);
      static const bool is_signed = false;
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static unsigned char epsilon() throw()
      { return 0; }
      static unsigned char round_error() throw()
      { return 0; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = false;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static unsigned char infinity() throw()
      { return static_cast<unsigned char>(0); }
      static unsigned char quiet_NaN() throw()
      { return static_cast<unsigned char>(0); }
      static unsigned char signaling_NaN() throw()
      { return static_cast<unsigned char>(0); }
      static unsigned char denorm_min() throw()
      { return static_cast<unsigned char>(0); }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = true;

      static const bool traps = true;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
  template<>
    struct numeric_limits<wchar_t>
    {
      static const bool is_specialized = true;

      static wchar_t min() throw()
      { return (((wchar_t)(-1) < 0) ? (wchar_t)1 << (sizeof(wchar_t) * 8 - ((wchar_t)(-1) < 0)) : (wchar_t)0); }
      static wchar_t max() throw()
      { return (((wchar_t)(-1) < 0) ? ((wchar_t)1 << (sizeof(wchar_t) * 8 - ((wchar_t)(-1) < 0))) - 1 : ~(wchar_t)0); }

      static const int digits = (sizeof(wchar_t) * 8 - ((wchar_t)(-1) < 0));
      static const int digits10 = ((sizeof(wchar_t) * 8 - ((wchar_t)(-1) < 0)) * 643 / 2136);
      static const bool is_signed = ((wchar_t)(-1) < 0);
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static wchar_t epsilon() throw()
      { return 0; }
      static wchar_t round_error() throw()
      { return 0; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = false;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static wchar_t infinity() throw()
      { return wchar_t(); }
      static wchar_t quiet_NaN() throw()
      { return wchar_t(); }
      static wchar_t signaling_NaN() throw()
      { return wchar_t(); }
      static wchar_t denorm_min() throw()
      { return wchar_t(); }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = true;

      static const bool traps = true;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
  template<>
    struct numeric_limits<short>
    {
      static const bool is_specialized = true;

      static short min() throw()
      { return -32767 - 1; }
      static short max() throw()
      { return 32767; }

      static const int digits = (sizeof(short) * 8 - ((short)(-1) < 0));
      static const int digits10 = ((sizeof(short) * 8 - ((short)(-1) < 0)) * 643 / 2136);
      static const bool is_signed = true;
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static short epsilon() throw()
      { return 0; }
      static short round_error() throw()
      { return 0; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = false;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static short infinity() throw()
      { return short(); }
      static short quiet_NaN() throw()
      { return short(); }
      static short signaling_NaN() throw()
      { return short(); }
      static short denorm_min() throw()
      { return short(); }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = true;

      static const bool traps = true;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
  template<>
    struct numeric_limits<unsigned short>
    {
      static const bool is_specialized = true;

      static unsigned short min() throw()
      { return 0; }
      static unsigned short max() throw()
      { return 32767 * 2U + 1; }

      static const int digits = (sizeof(unsigned short) * 8 - ((unsigned short)(-1) < 0));
      static const int digits10 = ((sizeof(unsigned short) * 8 - ((unsigned short)(-1) < 0)) * 643 / 2136);
      static const bool is_signed = false;
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static unsigned short epsilon() throw()
      { return 0; }
      static unsigned short round_error() throw()
      { return 0; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = false;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static unsigned short infinity() throw()
      { return static_cast<unsigned short>(0); }
      static unsigned short quiet_NaN() throw()
      { return static_cast<unsigned short>(0); }
      static unsigned short signaling_NaN() throw()
      { return static_cast<unsigned short>(0); }
      static unsigned short denorm_min() throw()
      { return static_cast<unsigned short>(0); }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = true;

      static const bool traps = true;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
  template<>
    struct numeric_limits<int>
    {
      static const bool is_specialized = true;

      static int min() throw()
      { return -2147483647 - 1; }
      static int max() throw()
      { return 2147483647; }

      static const int digits = (sizeof(int) * 8 - ((int)(-1) < 0));
      static const int digits10 = ((sizeof(int) * 8 - ((int)(-1) < 0)) * 643 / 2136);
      static const bool is_signed = true;
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static int epsilon() throw()
      { return 0; }
      static int round_error() throw()
      { return 0; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = false;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static int infinity() throw()
      { return static_cast<int>(0); }
      static int quiet_NaN() throw()
      { return static_cast<int>(0); }
      static int signaling_NaN() throw()
      { return static_cast<int>(0); }
      static int denorm_min() throw()
      { return static_cast<int>(0); }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = true;

      static const bool traps = true;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
  template<>
    struct numeric_limits<unsigned int>
    {
      static const bool is_specialized = true;

      static unsigned int min() throw()
      { return 0; }
      static unsigned int max() throw()
      { return 2147483647 * 2U + 1; }

      static const int digits = (sizeof(unsigned int) * 8 - ((unsigned int)(-1) < 0));
      static const int digits10 = ((sizeof(unsigned int) * 8 - ((unsigned int)(-1) < 0)) * 643 / 2136);
      static const bool is_signed = false;
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static unsigned int epsilon() throw()
      { return 0; }
      static unsigned int round_error() throw()
      { return 0; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = false;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static unsigned int infinity() throw()
      { return static_cast<unsigned int>(0); }
      static unsigned int quiet_NaN() throw()
      { return static_cast<unsigned int>(0); }
      static unsigned int signaling_NaN() throw()
      { return static_cast<unsigned int>(0); }
      static unsigned int denorm_min() throw()
      { return static_cast<unsigned int>(0); }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = true;

      static const bool traps = true;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
  template<>
    struct numeric_limits<long>
    {
      static const bool is_specialized = true;

      static long min() throw()
      { return -9223372036854775807L - 1; }
      static long max() throw()
      { return 9223372036854775807L; }

      static const int digits = (sizeof(long) * 8 - ((long)(-1) < 0));
      static const int digits10 = ((sizeof(long) * 8 - ((long)(-1) < 0)) * 643 / 2136);
      static const bool is_signed = true;
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static long epsilon() throw()
      { return 0; }
      static long round_error() throw()
      { return 0; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = false;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static long infinity() throw()
      { return static_cast<long>(0); }
      static long quiet_NaN() throw()
      { return static_cast<long>(0); }
      static long signaling_NaN() throw()
      { return static_cast<long>(0); }
      static long denorm_min() throw()
      { return static_cast<long>(0); }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = true;

      static const bool traps = true;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
  template<>
    struct numeric_limits<unsigned long>
    {
      static const bool is_specialized = true;

      static unsigned long min() throw()
      { return 0; }
      static unsigned long max() throw()
      { return 9223372036854775807L * 2UL + 1; }

      static const int digits = (sizeof(unsigned long) * 8 - ((unsigned long)(-1) < 0));
      static const int digits10 = ((sizeof(unsigned long) * 8 - ((unsigned long)(-1) < 0)) * 643 / 2136);
      static const bool is_signed = false;
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static unsigned long epsilon() throw()
      { return 0; }
      static unsigned long round_error() throw()
      { return 0; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = false;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static unsigned long infinity() throw()
      { return static_cast<unsigned long>(0); }
      static unsigned long quiet_NaN() throw()
      { return static_cast<unsigned long>(0); }
      static unsigned long signaling_NaN() throw()
      { return static_cast<unsigned long>(0); }
      static unsigned long denorm_min() throw()
      { return static_cast<unsigned long>(0); }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = true;

      static const bool traps = true;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
  template<>
    struct numeric_limits<long long>
    {
      static const bool is_specialized = true;

      static long long min() throw()
      { return -9223372036854775807LL - 1; }
      static long long max() throw()
      { return 9223372036854775807LL; }

      static const int digits = (sizeof(long long) * 8 - ((long long)(-1) < 0));
      static const int digits10 = ((sizeof(long long) * 8 - ((long long)(-1) < 0)) * 643 / 2136);
      static const bool is_signed = true;
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static long long epsilon() throw()
      { return 0; }
      static long long round_error() throw()
      { return 0; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = false;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static long long infinity() throw()
      { return static_cast<long long>(0); }
      static long long quiet_NaN() throw()
      { return static_cast<long long>(0); }
      static long long signaling_NaN() throw()
      { return static_cast<long long>(0); }
      static long long denorm_min() throw()
      { return static_cast<long long>(0); }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = true;

      static const bool traps = true;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
  template<>
    struct numeric_limits<unsigned long long>
    {
      static const bool is_specialized = true;

      static unsigned long long min() throw()
      { return 0; }
      static unsigned long long max() throw()
      { return 9223372036854775807LL * 2ULL + 1; }

      static const int digits = (sizeof(unsigned long long) * 8 - ((unsigned long long)(-1) < 0));
      static const int digits10 = ((sizeof(unsigned long long) * 8 - ((unsigned long long)(-1) < 0)) * 643 / 2136);
      static const bool is_signed = false;
      static const bool is_integer = true;
      static const bool is_exact = true;
      static const int radix = 2;
      static unsigned long long epsilon() throw()
      { return 0; }
      static unsigned long long round_error() throw()
      { return 0; }

      static const int min_exponent = 0;
      static const int min_exponent10 = 0;
      static const int max_exponent = 0;
      static const int max_exponent10 = 0;

      static const bool has_infinity = false;
      static const bool has_quiet_NaN = false;
      static const bool has_signaling_NaN = false;
      static const float_denorm_style has_denorm = denorm_absent;
      static const bool has_denorm_loss = false;

      static unsigned long long infinity() throw()
      { return static_cast<unsigned long long>(0); }
      static unsigned long long quiet_NaN() throw()
      { return static_cast<unsigned long long>(0); }
      static unsigned long long signaling_NaN() throw()
      { return static_cast<unsigned long long>(0); }
      static unsigned long long denorm_min() throw()
      { return static_cast<unsigned long long>(0); }

      static const bool is_iec559 = false;
      static const bool is_bounded = true;
      static const bool is_modulo = true;

      static const bool traps = true;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_toward_zero;
    };
  template<>
    struct numeric_limits<float>
    {
      static const bool is_specialized = true;

      static float min() throw()
      { return 1.17549435e-38F; }
      static float max() throw()
      { return 3.40282347e+38F; }

      static const int digits = 24;
      static const int digits10 = 6;
      static const bool is_signed = true;
      static const bool is_integer = false;
      static const bool is_exact = false;
      static const int radix = 2;
      static float epsilon() throw()
      { return 1.19209290e-7F; }
      static float round_error() throw()
      { return 0.5F; }

      static const int min_exponent = (-125);
      static const int min_exponent10 = (-37);
      static const int max_exponent = 128;
      static const int max_exponent10 = 38;

      static const bool has_infinity = 1;
      static const bool has_quiet_NaN = 1;
      static const bool has_signaling_NaN = has_quiet_NaN;
      static const float_denorm_style has_denorm
 = bool(1) ? denorm_present : denorm_absent;
      static const bool has_denorm_loss = false;

      static float infinity() throw()
      { return __builtin_huge_valf (); }
      static float quiet_NaN() throw()
      { return __builtin_nanf (""); }
      static float signaling_NaN() throw()
      { return __builtin_nansf (""); }
      static float denorm_min() throw()
      { return 1.40129846e-45F; }

      static const bool is_iec559
 = has_infinity && has_quiet_NaN && has_denorm == denorm_present;
      static const bool is_bounded = true;
      static const bool is_modulo = false;

      static const bool traps = false;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_to_nearest;
    };
  template<>
    struct numeric_limits<double>
    {
      static const bool is_specialized = true;

      static double min() throw()
      { return 2.2250738585072014e-308; }
      static double max() throw()
      { return 1.7976931348623157e+308; }

      static const int digits = 53;
      static const int digits10 = 15;
      static const bool is_signed = true;
      static const bool is_integer = false;
      static const bool is_exact = false;
      static const int radix = 2;
      static double epsilon() throw()
      { return 2.2204460492503131e-16; }
      static double round_error() throw()
      { return 0.5; }

      static const int min_exponent = (-1021);
      static const int min_exponent10 = (-307);
      static const int max_exponent = 1024;
      static const int max_exponent10 = 308;

      static const bool has_infinity = 1;
      static const bool has_quiet_NaN = 1;
      static const bool has_signaling_NaN = has_quiet_NaN;
      static const float_denorm_style has_denorm
 = bool(1) ? denorm_present : denorm_absent;
      static const bool has_denorm_loss = false;

      static double infinity() throw()
      { return __builtin_huge_val(); }
      static double quiet_NaN() throw()
      { return __builtin_nan (""); }
      static double signaling_NaN() throw()
      { return __builtin_nans (""); }
      static double denorm_min() throw()
      { return 4.9406564584124654e-324; }

      static const bool is_iec559
 = has_infinity && has_quiet_NaN && has_denorm == denorm_present;
      static const bool is_bounded = true;
      static const bool is_modulo = false;

      static const bool traps = false;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_to_nearest;
    };
  template<>
    struct numeric_limits<long double>
    {
      static const bool is_specialized = true;

      static long double min() throw()
      { return 3.36210314311209350626e-4932L; }
      static long double max() throw()
      { return 1.18973149535723176502e+4932L; }

      static const int digits = 64;
      static const int digits10 = 18;
      static const bool is_signed = true;
      static const bool is_integer = false;
      static const bool is_exact = false;
      static const int radix = 2;
      static long double epsilon() throw()
      { return 1.08420217248550443401e-19L; }
      static long double round_error() throw()
      { return 0.5L; }

      static const int min_exponent = (-16381);
      static const int min_exponent10 = (-4931);
      static const int max_exponent = 16384;
      static const int max_exponent10 = 4932;

      static const bool has_infinity = 1;
      static const bool has_quiet_NaN = 1;
      static const bool has_signaling_NaN = has_quiet_NaN;
      static const float_denorm_style has_denorm
 = bool(1) ? denorm_present : denorm_absent;
      static const bool has_denorm_loss
 = false;

      static long double infinity() throw()
      { return __builtin_huge_vall (); }
      static long double quiet_NaN() throw()
      { return __builtin_nanl (""); }
      static long double signaling_NaN() throw()
      { return __builtin_nansl (""); }
      static long double denorm_min() throw()
      { return 3.64519953188247460253e-4951L; }

      static const bool is_iec559
 = has_infinity && has_quiet_NaN && has_denorm == denorm_present;
      static const bool is_bounded = true;
      static const bool is_modulo = false;

      static const bool traps = false;
      static const bool tinyness_before = false;
      static const float_round_style round_style = round_to_nearest;
    };

}
namespace std {

  template<typename _Tp>
    pair<_Tp*, ptrdiff_t>
    __get_temporary_buffer(ptrdiff_t __len, _Tp*)
    {
      const ptrdiff_t __max = numeric_limits<ptrdiff_t>::max() / sizeof(_Tp);
      if (__len > __max)
 __len = __max;

      while (__len > 0)
 {
   _Tp* __tmp = static_cast<_Tp*>(::operator new(__len * sizeof(_Tp),
       nothrow));
   if (__tmp != 0)
     return pair<_Tp*, ptrdiff_t>(__tmp, __len);
   __len /= 2;
 }
      return pair<_Tp*, ptrdiff_t>(static_cast<_Tp*>(0), 0);
    }

  template<typename _Tp>
    inline pair<_Tp*, ptrdiff_t>
    get_temporary_buffer(ptrdiff_t __len)
    { return std::__get_temporary_buffer(__len, static_cast<_Tp*>(0)); }

  template<typename _Tp>
    void
    return_temporary_buffer(_Tp* __p)
    { ::operator delete(__p, nothrow); }

  template<typename _Tp1>
    struct auto_ptr_ref
    {
      _Tp1* _M_ptr;

      explicit
      auto_ptr_ref(_Tp1* __p): _M_ptr(__p) { }
    };

  template<typename _Tp>
    class auto_ptr
    {
    private:
      _Tp* _M_ptr;

    public:

      typedef _Tp element_type;

      explicit
      auto_ptr(element_type* __p = 0) throw() : _M_ptr(__p) { }

      auto_ptr(auto_ptr& __a) throw() : _M_ptr(__a.release()) { }

      template<typename _Tp1>
        auto_ptr(auto_ptr<_Tp1>& __a) throw() : _M_ptr(__a.release()) { }

      auto_ptr&
      operator=(auto_ptr& __a) throw()
      {
 reset(__a.release());
 return *this;
      }

      template<typename _Tp1>
        auto_ptr&
        operator=(auto_ptr<_Tp1>& __a) throw()
        {
   reset(__a.release());
   return *this;
 }

      ~auto_ptr() { delete _M_ptr; }

      element_type&
      operator*() const throw()
      {
 ;
 return *_M_ptr;
      }

      element_type*
      operator->() const throw()
      {
 ;
 return _M_ptr;
      }

      element_type*
      get() const throw() { return _M_ptr; }

      element_type*
      release() throw()
      {
 element_type* __tmp = _M_ptr;
 _M_ptr = 0;
 return __tmp;
      }

      void
      reset(element_type* __p = 0) throw()
      {
 if (__p != _M_ptr)
   {
     delete _M_ptr;
     _M_ptr = __p;
   }
      }

      auto_ptr(auto_ptr_ref<element_type> __ref) throw()
      : _M_ptr(__ref._M_ptr) { }

      auto_ptr&
      operator=(auto_ptr_ref<element_type> __ref) throw()
      {
 if (__ref._M_ptr != this->get())
   {
     delete _M_ptr;
     _M_ptr = __ref._M_ptr;
   }
 return *this;
      }

      template<typename _Tp1>
        operator auto_ptr_ref<_Tp1>() throw()
        { return auto_ptr_ref<_Tp1>(this->release()); }

      template<typename _Tp1>
        operator auto_ptr<_Tp1>() throw()
        { return auto_ptr<_Tp1>(this->release()); }
  };

}

       
namespace std {

  template<typename _CharT, typename _Traits>
    inline void
    __ostream_write(basic_ostream<_CharT, _Traits>& __out,
      const _CharT* __s, streamsize __n)
    {
      typedef basic_ostream<_CharT, _Traits> __ostream_type;
      typedef typename __ostream_type::ios_base __ios_base;

      const streamsize __put = __out.rdbuf()->sputn(__s, __n);
      if (__put != __n)
 __out.setstate(__ios_base::badbit);
    }

  template<typename _CharT, typename _Traits>
    inline void
    __ostream_fill(basic_ostream<_CharT, _Traits>& __out, streamsize __n)
    {
      typedef basic_ostream<_CharT, _Traits> __ostream_type;
      typedef typename __ostream_type::ios_base __ios_base;

      const _CharT __c = __out.fill();
      for (; __n > 0; --__n)
 {
   const typename _Traits::int_type __put = __out.rdbuf()->sputc(__c);
   if (_Traits::eq_int_type(__put, _Traits::eof()))
     {
       __out.setstate(__ios_base::badbit);
       break;
     }
 }
    }

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>&
    __ostream_insert(basic_ostream<_CharT, _Traits>& __out,
       const _CharT* __s, streamsize __n)
    {
      typedef basic_ostream<_CharT, _Traits> __ostream_type;
      typedef typename __ostream_type::ios_base __ios_base;

      typename __ostream_type::sentry __cerb(__out);
      if (__cerb)
 {
   try
     {
       const streamsize __w = __out.width();
       if (__w > __n)
  {
    const bool __left = ((__out.flags()
     & __ios_base::adjustfield)
           == __ios_base::left);
    if (!__left)
      __ostream_fill(__out, __w - __n);
    if (__out.good())
      __ostream_write(__out, __s, __n);
    if (__left && __out.good())
      __ostream_fill(__out, __w - __n);
  }
       else
  __ostream_write(__out, __s, __n);
       __out.width(0);
     }
   catch(...)
     { __out._M_setstate(__ios_base::badbit); }
 }
      return __out;
    }

  extern template ostream& __ostream_insert(ostream&, const char*, streamsize);
  extern template wostream& __ostream_insert(wostream&, const wchar_t*,
          streamsize);

}
namespace std {

  template <class _Arg, class _Result>
    struct unary_function
    {
      typedef _Arg argument_type;
      typedef _Result result_type;
    };
  template <class _Arg1, class _Arg2, class _Result>
    struct binary_function
    {
      typedef _Arg1 first_argument_type;
      typedef _Arg2 second_argument_type;
      typedef _Result result_type;
    };

  template <class _Tp>
    struct plus : public binary_function<_Tp, _Tp, _Tp>
    {
      _Tp
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x + __y; }
    };
  template <class _Tp>
    struct minus : public binary_function<_Tp, _Tp, _Tp>
    {
      _Tp
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x - __y; }
    };
  template <class _Tp>
    struct multiplies : public binary_function<_Tp, _Tp, _Tp>
    {
      _Tp
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x * __y; }
    };
  template <class _Tp>
    struct divides : public binary_function<_Tp, _Tp, _Tp>
    {
      _Tp
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x / __y; }
    };
  template <class _Tp>
    struct modulus : public binary_function<_Tp, _Tp, _Tp>
    {
      _Tp
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x % __y; }
    };
  template <class _Tp>
    struct negate : public unary_function<_Tp, _Tp>
    {
      _Tp
      operator()(const _Tp& __x) const
      { return -__x; }
    };

  template <class _Tp>
    struct equal_to : public binary_function<_Tp, _Tp, bool>
    {
      bool
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x == __y; }
    };
  template <class _Tp>
    struct not_equal_to : public binary_function<_Tp, _Tp, bool>
    {
      bool
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x != __y; }
    };
  template <class _Tp>
    struct greater : public binary_function<_Tp, _Tp, bool>
    {
      bool
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x > __y; }
    };
  template <class _Tp>
    struct less : public binary_function<_Tp, _Tp, bool>
    {
      bool
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x < __y; }
    };
  template <class _Tp>
    struct greater_equal : public binary_function<_Tp, _Tp, bool>
    {
      bool
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x >= __y; }
    };
  template <class _Tp>
    struct less_equal : public binary_function<_Tp, _Tp, bool>
    {
      bool
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x <= __y; }
    };

  template <class _Tp>
    struct logical_and : public binary_function<_Tp, _Tp, bool>
    {
      bool
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x && __y; }
    };
  template <class _Tp>
    struct logical_or : public binary_function<_Tp, _Tp, bool>
    {
      bool
      operator()(const _Tp& __x, const _Tp& __y) const
      { return __x || __y; }
    };
  template <class _Tp>
    struct logical_not : public unary_function<_Tp, bool>
    {
      bool
      operator()(const _Tp& __x) const
      { return !__x; }
    };

  template <class _Predicate>
    class unary_negate
    : public unary_function<typename _Predicate::argument_type, bool>
    {
    protected:
      _Predicate _M_pred;
    public:
      explicit
      unary_negate(const _Predicate& __x) : _M_pred(__x) {}

      bool
      operator()(const typename _Predicate::argument_type& __x) const
      { return !_M_pred(__x); }
    };
  template <class _Predicate>
    inline unary_negate<_Predicate>
    not1(const _Predicate& __pred)
    { return unary_negate<_Predicate>(__pred); }
  template <class _Predicate>
    class binary_negate
    : public binary_function<typename _Predicate::first_argument_type,
        typename _Predicate::second_argument_type,
        bool>
    {
    protected:
      _Predicate _M_pred;
    public:
      explicit
      binary_negate(const _Predicate& __x)
      : _M_pred(__x) { }

      bool
      operator()(const typename _Predicate::first_argument_type& __x,
   const typename _Predicate::second_argument_type& __y) const
      { return !_M_pred(__x, __y); }
    };
  template <class _Predicate>
    inline binary_negate<_Predicate>
    not2(const _Predicate& __pred)
    { return binary_negate<_Predicate>(__pred); }

  template <class _Operation>
    class binder1st
    : public unary_function<typename _Operation::second_argument_type,
       typename _Operation::result_type>
    {
    protected:
      _Operation op;
      typename _Operation::first_argument_type value;
    public:
      binder1st(const _Operation& __x,
  const typename _Operation::first_argument_type& __y)
      : op(__x), value(__y) {}

      typename _Operation::result_type
      operator()(const typename _Operation::second_argument_type& __x) const
      { return op(value, __x); }

      typename _Operation::result_type
      operator()(typename _Operation::second_argument_type& __x) const
      { return op(value, __x); }
    };
  template <class _Operation, class _Tp>
    inline binder1st<_Operation>
    bind1st(const _Operation& __fn, const _Tp& __x)
    {
      typedef typename _Operation::first_argument_type _Arg1_type;
      return binder1st<_Operation>(__fn, _Arg1_type(__x));
    }
  template <class _Operation>
    class binder2nd
    : public unary_function<typename _Operation::first_argument_type,
       typename _Operation::result_type>
    {
    protected:
      _Operation op;
      typename _Operation::second_argument_type value;
    public:
      binder2nd(const _Operation& __x,
  const typename _Operation::second_argument_type& __y)
      : op(__x), value(__y) {}

      typename _Operation::result_type
      operator()(const typename _Operation::first_argument_type& __x) const
      { return op(__x, value); }

      typename _Operation::result_type
      operator()(typename _Operation::first_argument_type& __x) const
      { return op(__x, value); }
    };
  template <class _Operation, class _Tp>
    inline binder2nd<_Operation>
    bind2nd(const _Operation& __fn, const _Tp& __x)
    {
      typedef typename _Operation::second_argument_type _Arg2_type;
      return binder2nd<_Operation>(__fn, _Arg2_type(__x));
    }

  template <class _Arg, class _Result>
    class pointer_to_unary_function : public unary_function<_Arg, _Result>
    {
    protected:
      _Result (*_M_ptr)(_Arg);
    public:
      pointer_to_unary_function() {}

      explicit
      pointer_to_unary_function(_Result (*__x)(_Arg))
      : _M_ptr(__x) {}

      _Result
      operator()(_Arg __x) const
      { return _M_ptr(__x); }
    };
  template <class _Arg, class _Result>
    inline pointer_to_unary_function<_Arg, _Result>
    ptr_fun(_Result (*__x)(_Arg))
    { return pointer_to_unary_function<_Arg, _Result>(__x); }
  template <class _Arg1, class _Arg2, class _Result>
    class pointer_to_binary_function
    : public binary_function<_Arg1, _Arg2, _Result>
    {
    protected:
      _Result (*_M_ptr)(_Arg1, _Arg2);
    public:
      pointer_to_binary_function() {}

      explicit
      pointer_to_binary_function(_Result (*__x)(_Arg1, _Arg2))
      : _M_ptr(__x) {}

      _Result
      operator()(_Arg1 __x, _Arg2 __y) const
      { return _M_ptr(__x, __y); }
    };
  template <class _Arg1, class _Arg2, class _Result>
    inline pointer_to_binary_function<_Arg1, _Arg2, _Result>
    ptr_fun(_Result (*__x)(_Arg1, _Arg2))
    { return pointer_to_binary_function<_Arg1, _Arg2, _Result>(__x); }
  template <class _Tp>
    struct _Identity : public unary_function<_Tp,_Tp>
    {
      _Tp&
      operator()(_Tp& __x) const
      { return __x; }

      const _Tp&
      operator()(const _Tp& __x) const
      { return __x; }
    };

  template <class _Pair>
    struct _Select1st : public unary_function<_Pair,
           typename _Pair::first_type>
    {
      typename _Pair::first_type&
      operator()(_Pair& __x) const
      { return __x.first; }

      const typename _Pair::first_type&
      operator()(const _Pair& __x) const
      { return __x.first; }
    };

  template <class _Pair>
    struct _Select2nd : public unary_function<_Pair,
           typename _Pair::second_type>
    {
      typename _Pair::second_type&
      operator()(_Pair& __x) const
      { return __x.second; }

      const typename _Pair::second_type&
      operator()(const _Pair& __x) const
      { return __x.second; }
    };

  template <class _Ret, class _Tp>
    class mem_fun_t : public unary_function<_Tp*, _Ret>
    {
    public:
      explicit
      mem_fun_t(_Ret (_Tp::*__pf)())
      : _M_f(__pf) {}

      _Ret
      operator()(_Tp* __p) const
      { return (__p->*_M_f)(); }
    private:
      _Ret (_Tp::*_M_f)();
    };
  template <class _Ret, class _Tp>
    class const_mem_fun_t : public unary_function<const _Tp*, _Ret>
    {
    public:
      explicit
      const_mem_fun_t(_Ret (_Tp::*__pf)() const)
      : _M_f(__pf) {}

      _Ret
      operator()(const _Tp* __p) const
      { return (__p->*_M_f)(); }
    private:
      _Ret (_Tp::*_M_f)() const;
    };
  template <class _Ret, class _Tp>
    class mem_fun_ref_t : public unary_function<_Tp, _Ret>
    {
    public:
      explicit
      mem_fun_ref_t(_Ret (_Tp::*__pf)())
      : _M_f(__pf) {}

      _Ret
      operator()(_Tp& __r) const
      { return (__r.*_M_f)(); }
    private:
      _Ret (_Tp::*_M_f)();
  };
  template <class _Ret, class _Tp>
    class const_mem_fun_ref_t : public unary_function<_Tp, _Ret>
    {
    public:
      explicit
      const_mem_fun_ref_t(_Ret (_Tp::*__pf)() const)
      : _M_f(__pf) {}

      _Ret
      operator()(const _Tp& __r) const
      { return (__r.*_M_f)(); }
    private:
      _Ret (_Tp::*_M_f)() const;
    };
  template <class _Ret, class _Tp, class _Arg>
    class mem_fun1_t : public binary_function<_Tp*, _Arg, _Ret>
    {
    public:
      explicit
      mem_fun1_t(_Ret (_Tp::*__pf)(_Arg))
      : _M_f(__pf) {}

      _Ret
      operator()(_Tp* __p, _Arg __x) const
      { return (__p->*_M_f)(__x); }
    private:
      _Ret (_Tp::*_M_f)(_Arg);
    };
  template <class _Ret, class _Tp, class _Arg>
    class const_mem_fun1_t : public binary_function<const _Tp*, _Arg, _Ret>
    {
    public:
      explicit
      const_mem_fun1_t(_Ret (_Tp::*__pf)(_Arg) const)
      : _M_f(__pf) {}

      _Ret
      operator()(const _Tp* __p, _Arg __x) const
      { return (__p->*_M_f)(__x); }
    private:
      _Ret (_Tp::*_M_f)(_Arg) const;
    };
  template <class _Ret, class _Tp, class _Arg>
    class mem_fun1_ref_t : public binary_function<_Tp, _Arg, _Ret>
    {
    public:
      explicit
      mem_fun1_ref_t(_Ret (_Tp::*__pf)(_Arg))
      : _M_f(__pf) {}

      _Ret
      operator()(_Tp& __r, _Arg __x) const
      { return (__r.*_M_f)(__x); }
    private:
      _Ret (_Tp::*_M_f)(_Arg);
    };
  template <class _Ret, class _Tp, class _Arg>
    class const_mem_fun1_ref_t : public binary_function<_Tp, _Arg, _Ret>
    {
    public:
      explicit
      const_mem_fun1_ref_t(_Ret (_Tp::*__pf)(_Arg) const)
      : _M_f(__pf) {}

      _Ret
      operator()(const _Tp& __r, _Arg __x) const
      { return (__r.*_M_f)(__x); }
    private:
      _Ret (_Tp::*_M_f)(_Arg) const;
    };

  template <class _Ret, class _Tp>
    inline mem_fun_t<_Ret, _Tp>
    mem_fun(_Ret (_Tp::*__f)())
    { return mem_fun_t<_Ret, _Tp>(__f); }

  template <class _Ret, class _Tp>
    inline const_mem_fun_t<_Ret, _Tp>
    mem_fun(_Ret (_Tp::*__f)() const)
    { return const_mem_fun_t<_Ret, _Tp>(__f); }

  template <class _Ret, class _Tp>
    inline mem_fun_ref_t<_Ret, _Tp>
    mem_fun_ref(_Ret (_Tp::*__f)())
    { return mem_fun_ref_t<_Ret, _Tp>(__f); }

  template <class _Ret, class _Tp>
    inline const_mem_fun_ref_t<_Ret, _Tp>
    mem_fun_ref(_Ret (_Tp::*__f)() const)
    { return const_mem_fun_ref_t<_Ret, _Tp>(__f); }

  template <class _Ret, class _Tp, class _Arg>
    inline mem_fun1_t<_Ret, _Tp, _Arg>
    mem_fun(_Ret (_Tp::*__f)(_Arg))
    { return mem_fun1_t<_Ret, _Tp, _Arg>(__f); }

  template <class _Ret, class _Tp, class _Arg>
    inline const_mem_fun1_t<_Ret, _Tp, _Arg>
    mem_fun(_Ret (_Tp::*__f)(_Arg) const)
    { return const_mem_fun1_t<_Ret, _Tp, _Arg>(__f); }

  template <class _Ret, class _Tp, class _Arg>
    inline mem_fun1_ref_t<_Ret, _Tp, _Arg>
    mem_fun_ref(_Ret (_Tp::*__f)(_Arg))
    { return mem_fun1_ref_t<_Ret, _Tp, _Arg>(__f); }

  template <class _Ret, class _Tp, class _Arg>
    inline const_mem_fun1_ref_t<_Ret, _Tp, _Arg>
    mem_fun_ref(_Ret (_Tp::*__f)(_Arg) const)
    { return const_mem_fun1_ref_t<_Ret, _Tp, _Arg>(__f); }

}

       
typedef int _Atomic_word;
namespace __gnu_cxx {
  static inline _Atomic_word
  __exchange_and_add(volatile _Atomic_word* __mem, int __val)
  { return __sync_fetch_and_add(__mem, __val); }

  static inline void
  __atomic_add(volatile _Atomic_word* __mem, int __val)
  { __sync_fetch_and_add(__mem, __val); }

  static inline _Atomic_word
  __exchange_and_add_single(_Atomic_word* __mem, int __val)
  {
    _Atomic_word __result = *__mem;
    *__mem += __val;
    return __result;
  }

  static inline void
  __atomic_add_single(_Atomic_word* __mem, int __val)
  { *__mem += __val; }

  static inline _Atomic_word
  __attribute__ ((__unused__))
  __exchange_and_add_dispatch(_Atomic_word* __mem, int __val)
  {

    if (__gthread_active_p())
      return __exchange_and_add(__mem, __val);
    else
      return __exchange_and_add_single(__mem, __val);

  }

  static inline void
  __attribute__ ((__unused__))
  __atomic_add_dispatch(_Atomic_word* __mem, int __val)
  {

    if (__gthread_active_p())
      __atomic_add(__mem, __val);
    else
      __atomic_add_single(__mem, __val);

  }

}

namespace std {

  template<typename _CharT, typename _Traits, typename _Alloc>
    class basic_string
    {
      typedef typename _Alloc::template rebind<_CharT>::other _CharT_alloc_type;
    public:
      typedef _Traits traits_type;
      typedef typename _Traits::char_type value_type;
      typedef _Alloc allocator_type;
      typedef typename _CharT_alloc_type::size_type size_type;
      typedef typename _CharT_alloc_type::difference_type difference_type;
      typedef typename _CharT_alloc_type::reference reference;
      typedef typename _CharT_alloc_type::const_reference const_reference;
      typedef typename _CharT_alloc_type::pointer pointer;
      typedef typename _CharT_alloc_type::const_pointer const_pointer;
      typedef __gnu_cxx::__normal_iterator<pointer, basic_string> iterator;
      typedef __gnu_cxx::__normal_iterator<const_pointer, basic_string>
                                                            const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef std::reverse_iterator<iterator> reverse_iterator;

    private:

      struct _Rep_base
      {
 size_type _M_length;
 size_type _M_capacity;
 _Atomic_word _M_refcount;
      };

      struct _Rep : _Rep_base
      {

 typedef typename _Alloc::template rebind<char>::other _Raw_bytes_alloc;

 static const size_type _S_max_size;
 static const _CharT _S_terminal;

        static size_type _S_empty_rep_storage[];

        static _Rep&
        _S_empty_rep()
        {

   void* __p = reinterpret_cast<void*>(&_S_empty_rep_storage);
   return *reinterpret_cast<_Rep*>(__p);
 }

        bool
 _M_is_leaked() const
        { return this->_M_refcount < 0; }

        bool
 _M_is_shared() const
        { return this->_M_refcount > 0; }

        void
 _M_set_leaked()
        { this->_M_refcount = -1; }

        void
 _M_set_sharable()
        { this->_M_refcount = 0; }

 void
 _M_set_length_and_sharable(size_type __n)
 {
   this->_M_set_sharable();
   this->_M_length = __n;
   traits_type::assign(this->_M_refdata()[__n], _S_terminal);
 }

 _CharT*
 _M_refdata() throw()
 { return reinterpret_cast<_CharT*>(this + 1); }

 _CharT*
 _M_grab(const _Alloc& __alloc1, const _Alloc& __alloc2)
 {
   return (!_M_is_leaked() && __alloc1 == __alloc2)
           ? _M_refcopy() : _M_clone(__alloc1);
 }
 static _Rep*
 _S_create(size_type, size_type, const _Alloc&);

 void
 _M_dispose(const _Alloc& __a)
 {

   if (__builtin_expect(this != &_S_empty_rep(), false))

     if (__gnu_cxx::__exchange_and_add_dispatch(&this->_M_refcount,
             -1) <= 0)
       _M_destroy(__a);
 }

 void
 _M_destroy(const _Alloc&) throw();

 _CharT*
 _M_refcopy() throw()
 {

   if (__builtin_expect(this != &_S_empty_rep(), false))

            __gnu_cxx::__atomic_add_dispatch(&this->_M_refcount, 1);
   return _M_refdata();
 }

 _CharT*
 _M_clone(const _Alloc&, size_type __res = 0);
      };
      struct _Alloc_hider : _Alloc
      {
 _Alloc_hider(_CharT* __dat, const _Alloc& __a)
 : _Alloc(__a), _M_p(__dat) { }

 _CharT* _M_p;
      };

    public:
      static const size_type npos = static_cast<size_type>(-1);

    private:

      mutable _Alloc_hider _M_dataplus;

      _CharT*
      _M_data() const
      { return _M_dataplus._M_p; }

      _CharT*
      _M_data(_CharT* __p)
      { return (_M_dataplus._M_p = __p); }

      _Rep*
      _M_rep() const
      { return &((reinterpret_cast<_Rep*> (_M_data()))[-1]); }

      iterator
      _M_ibegin() const
      { return iterator(_M_data()); }

      iterator
      _M_iend() const
      { return iterator(_M_data() + this->size()); }

      void
      _M_leak()
      {
 if (!_M_rep()->_M_is_leaked())
   _M_leak_hard();
      }

      size_type
      _M_check(size_type __pos, const char* __s) const
      {
 if (__pos > this->size())
   __throw_out_of_range((__s));
 return __pos;
      }

      void
      _M_check_length(size_type __n1, size_type __n2, const char* __s) const
      {
 if (this->max_size() - (this->size() - __n1) < __n2)
   __throw_length_error((__s));
      }
      size_type
      _M_limit(size_type __pos, size_type __off) const
      {
 const bool __testoff = __off < this->size() - __pos;
 return __testoff ? __off : this->size() - __pos;
      }
      bool
      _M_disjunct(const _CharT* __s) const
      {
 return (less<const _CharT*>()(__s, _M_data())
  || less<const _CharT*>()(_M_data() + this->size(), __s));
      }

      static void
      _M_copy(_CharT* __d, const _CharT* __s, size_type __n)
      {
 if (__n == 1)
   traits_type::assign(*__d, *__s);
 else
   traits_type::copy(__d, __s, __n);
      }

      static void
      _M_move(_CharT* __d, const _CharT* __s, size_type __n)
      {
 if (__n == 1)
   traits_type::assign(*__d, *__s);
 else
   traits_type::move(__d, __s, __n);
      }

      static void
      _M_assign(_CharT* __d, size_type __n, _CharT __c)
      {
 if (__n == 1)
   traits_type::assign(*__d, __c);
 else
   traits_type::assign(__d, __n, __c);
      }

      template<class _Iterator>
        static void
        _S_copy_chars(_CharT* __p, _Iterator __k1, _Iterator __k2)
        {
   for (; __k1 != __k2; ++__k1, ++__p)
     traits_type::assign(*__p, *__k1);
 }

      static void
      _S_copy_chars(_CharT* __p, iterator __k1, iterator __k2)
      { _S_copy_chars(__p, __k1.base(), __k2.base()); }

      static void
      _S_copy_chars(_CharT* __p, const_iterator __k1, const_iterator __k2)
      { _S_copy_chars(__p, __k1.base(), __k2.base()); }

      static void
      _S_copy_chars(_CharT* __p, _CharT* __k1, _CharT* __k2)
      { _M_copy(__p, __k1, __k2 - __k1); }

      static void
      _S_copy_chars(_CharT* __p, const _CharT* __k1, const _CharT* __k2)
      { _M_copy(__p, __k1, __k2 - __k1); }

      static int
      _S_compare(size_type __x, size_type __y)
      {
         if (__x > __y)
            return 1;
         if (__x < __y)
            return -1;
         return 0;
      }

      void
      _M_mutate(size_type __pos, size_type __len1, size_type __len2);

      void
      _M_leak_hard();

      static _Rep&
      _S_empty_rep()
      { return _Rep::_S_empty_rep(); }

    public:

      inline
      basic_string();
      explicit
      basic_string(const _Alloc& __a);
      basic_string(const basic_string& __str);
      basic_string(const basic_string& __str, size_type __pos,
     size_type __n = npos);

      basic_string(const basic_string& __str, size_type __pos,
     size_type __n, const _Alloc& __a);

      basic_string(const _CharT* __s, size_type __n,
     const _Alloc& __a = _Alloc());

      basic_string(const _CharT* __s, const _Alloc& __a = _Alloc());
      basic_string(size_type __n, _CharT __c, const _Alloc& __a = _Alloc());

      template<class _InputIterator>
        basic_string(_InputIterator __beg, _InputIterator __end,
       const _Alloc& __a = _Alloc());
      ~basic_string()
      { _M_rep()->_M_dispose(this->get_allocator()); }

      basic_string&
      operator=(const basic_string& __str)
      { return this->assign(__str); }

      basic_string&
      operator=(const _CharT* __s)
      { return this->assign(__s); }

      basic_string&
      operator=(_CharT __c)
      {
 this->assign(1, __c);
 return *this;
      }
      iterator
      begin()
      {
 _M_leak();
 return iterator(_M_data());
      }

      const_iterator
      begin() const
      { return const_iterator(_M_data()); }

      iterator
      end()
      {
 _M_leak();
 return iterator(_M_data() + this->size());
      }

      const_iterator
      end() const
      { return const_iterator(_M_data() + this->size()); }
      reverse_iterator
      rbegin()
      { return reverse_iterator(this->end()); }
      const_reverse_iterator
      rbegin() const
      { return const_reverse_iterator(this->end()); }
      reverse_iterator
      rend()
      { return reverse_iterator(this->begin()); }
      const_reverse_iterator
      rend() const
      { return const_reverse_iterator(this->begin()); }

    public:

      size_type
      size() const
      { return _M_rep()->_M_length; }

      size_type
      length() const
      { return _M_rep()->_M_length; }
      size_type
      max_size() const
      { return _Rep::_S_max_size; }

      void
      resize(size_type __n, _CharT __c);

      void
      resize(size_type __n)
      { this->resize(__n, _CharT()); }

      size_type
      capacity() const
      { return _M_rep()->_M_capacity; }

      void
      reserve(size_type __res_arg = 0);
      void
      clear()
      { _M_mutate(0, this->size(), 0); }
      bool
      empty() const
      { return this->size() == 0; }

      const_reference
      operator[] (size_type __pos) const
      {
 ;
 return _M_data()[__pos];
      }

      reference
      operator[](size_type __pos)
      {

 ;

 ;
 _M_leak();
 return _M_data()[__pos];
      }

      const_reference
      at(size_type __n) const
      {
 if (__n >= this->size())
   __throw_out_of_range(("basic_string::at"));
 return _M_data()[__n];
      }

      reference
      at(size_type __n)
      {
 if (__n >= size())
   __throw_out_of_range(("basic_string::at"));
 _M_leak();
 return _M_data()[__n];
      }

      basic_string&
      operator+=(const basic_string& __str)
      { return this->append(__str); }
      basic_string&
      operator+=(const _CharT* __s)
      { return this->append(__s); }
      basic_string&
      operator+=(_CharT __c)
      {
 this->push_back(__c);
 return *this;
      }
      basic_string&
      append(const basic_string& __str);

      basic_string&
      append(const basic_string& __str, size_type __pos, size_type __n);

      basic_string&
      append(const _CharT* __s, size_type __n);
      basic_string&
      append(const _CharT* __s)
      {
 ;
 return this->append(__s, traits_type::length(__s));
      }

      basic_string&
      append(size_type __n, _CharT __c);

      template<class _InputIterator>
        basic_string&
        append(_InputIterator __first, _InputIterator __last)
        { return this->replace(_M_iend(), _M_iend(), __first, __last); }

      void
      push_back(_CharT __c)
      {
 const size_type __len = 1 + this->size();
 if (__len > this->capacity() || _M_rep()->_M_is_shared())
   this->reserve(__len);
 traits_type::assign(_M_data()[this->size()], __c);
 _M_rep()->_M_set_length_and_sharable(__len);
      }
      basic_string&
      assign(const basic_string& __str);

      basic_string&
      assign(const basic_string& __str, size_type __pos, size_type __n)
      { return this->assign(__str._M_data()
       + __str._M_check(__pos, "basic_string::assign"),
       __str._M_limit(__pos, __n)); }

      basic_string&
      assign(const _CharT* __s, size_type __n);

      basic_string&
      assign(const _CharT* __s)
      {
 ;
 return this->assign(__s, traits_type::length(__s));
      }

      basic_string&
      assign(size_type __n, _CharT __c)
      { return _M_replace_aux(size_type(0), this->size(), __n, __c); }

      template<class _InputIterator>
        basic_string&
        assign(_InputIterator __first, _InputIterator __last)
        { return this->replace(_M_ibegin(), _M_iend(), __first, __last); }

      void
      insert(iterator __p, size_type __n, _CharT __c)
      { this->replace(__p, __p, __n, __c); }

      template<class _InputIterator>
        void
        insert(iterator __p, _InputIterator __beg, _InputIterator __end)
        { this->replace(__p, __p, __beg, __end); }

      basic_string&
      insert(size_type __pos1, const basic_string& __str)
      { return this->insert(__pos1, __str, size_type(0), __str.size()); }

      basic_string&
      insert(size_type __pos1, const basic_string& __str,
      size_type __pos2, size_type __n)
      { return this->insert(__pos1, __str._M_data()
       + __str._M_check(__pos2, "basic_string::insert"),
       __str._M_limit(__pos2, __n)); }

      basic_string&
      insert(size_type __pos, const _CharT* __s, size_type __n);

      basic_string&
      insert(size_type __pos, const _CharT* __s)
      {
 ;
 return this->insert(__pos, __s, traits_type::length(__s));
      }

      basic_string&
      insert(size_type __pos, size_type __n, _CharT __c)
      { return _M_replace_aux(_M_check(__pos, "basic_string::insert"),
         size_type(0), __n, __c); }

      iterator
      insert(iterator __p, _CharT __c)
      {
 ;
 const size_type __pos = __p - _M_ibegin();
 _M_replace_aux(__pos, size_type(0), size_type(1), __c);
 _M_rep()->_M_set_leaked();
 return iterator(_M_data() + __pos);
      }

      basic_string&
      erase(size_type __pos = 0, size_type __n = npos)
      {
 _M_mutate(_M_check(__pos, "basic_string::erase"),
    _M_limit(__pos, __n), size_type(0));
 return *this;
      }

      iterator
      erase(iterator __position)
      {
 ;

 const size_type __pos = __position - _M_ibegin();
 _M_mutate(__pos, size_type(1), size_type(0));
 _M_rep()->_M_set_leaked();
 return iterator(_M_data() + __pos);
      }

      iterator
      erase(iterator __first, iterator __last)
      {
 ;

        const size_type __pos = __first - _M_ibegin();
 _M_mutate(__pos, __last - __first, size_type(0));
 _M_rep()->_M_set_leaked();
 return iterator(_M_data() + __pos);
      }

      basic_string&
      replace(size_type __pos, size_type __n, const basic_string& __str)
      { return this->replace(__pos, __n, __str._M_data(), __str.size()); }

      basic_string&
      replace(size_type __pos1, size_type __n1, const basic_string& __str,
       size_type __pos2, size_type __n2)
      { return this->replace(__pos1, __n1, __str._M_data()
        + __str._M_check(__pos2, "basic_string::replace"),
        __str._M_limit(__pos2, __n2)); }

      basic_string&
      replace(size_type __pos, size_type __n1, const _CharT* __s,
       size_type __n2);

      basic_string&
      replace(size_type __pos, size_type __n1, const _CharT* __s)
      {
 ;
 return this->replace(__pos, __n1, __s, traits_type::length(__s));
      }

      basic_string&
      replace(size_type __pos, size_type __n1, size_type __n2, _CharT __c)
      { return _M_replace_aux(_M_check(__pos, "basic_string::replace"),
         _M_limit(__pos, __n1), __n2, __c); }

      basic_string&
      replace(iterator __i1, iterator __i2, const basic_string& __str)
      { return this->replace(__i1, __i2, __str._M_data(), __str.size()); }

      basic_string&
      replace(iterator __i1, iterator __i2, const _CharT* __s, size_type __n)
      {
 ;

 return this->replace(__i1 - _M_ibegin(), __i2 - __i1, __s, __n);
      }

      basic_string&
      replace(iterator __i1, iterator __i2, const _CharT* __s)
      {
 ;
 return this->replace(__i1, __i2, __s, traits_type::length(__s));
      }

      basic_string&
      replace(iterator __i1, iterator __i2, size_type __n, _CharT __c)
      {
 ;

 return _M_replace_aux(__i1 - _M_ibegin(), __i2 - __i1, __n, __c);
      }

      template<class _InputIterator>
        basic_string&
        replace(iterator __i1, iterator __i2,
  _InputIterator __k1, _InputIterator __k2)
        {
   ;

   ;
   typedef typename std::__is_integer<_InputIterator>::__type _Integral;
   return _M_replace_dispatch(__i1, __i2, __k1, __k2, _Integral());
 }

      basic_string&
      replace(iterator __i1, iterator __i2, _CharT* __k1, _CharT* __k2)
      {
 ;

 ;
 return this->replace(__i1 - _M_ibegin(), __i2 - __i1,
        __k1, __k2 - __k1);
      }

      basic_string&
      replace(iterator __i1, iterator __i2,
       const _CharT* __k1, const _CharT* __k2)
      {
 ;

 ;
 return this->replace(__i1 - _M_ibegin(), __i2 - __i1,
        __k1, __k2 - __k1);
      }

      basic_string&
      replace(iterator __i1, iterator __i2, iterator __k1, iterator __k2)
      {
 ;

 ;
 return this->replace(__i1 - _M_ibegin(), __i2 - __i1,
        __k1.base(), __k2 - __k1);
      }

      basic_string&
      replace(iterator __i1, iterator __i2,
       const_iterator __k1, const_iterator __k2)
      {
 ;

 ;
 return this->replace(__i1 - _M_ibegin(), __i2 - __i1,
        __k1.base(), __k2 - __k1);
      }

    private:
      template<class _Integer>
 basic_string&
 _M_replace_dispatch(iterator __i1, iterator __i2, _Integer __n,
       _Integer __val, __true_type)
        { return _M_replace_aux(__i1 - _M_ibegin(), __i2 - __i1, __n, __val); }

      template<class _InputIterator>
 basic_string&
 _M_replace_dispatch(iterator __i1, iterator __i2, _InputIterator __k1,
       _InputIterator __k2, __false_type);

      basic_string&
      _M_replace_aux(size_type __pos1, size_type __n1, size_type __n2,
       _CharT __c);

      basic_string&
      _M_replace_safe(size_type __pos1, size_type __n1, const _CharT* __s,
        size_type __n2);

      template<class _InIterator>
        static _CharT*
        _S_construct_aux(_InIterator __beg, _InIterator __end,
    const _Alloc& __a, __false_type)
 {
          typedef typename iterator_traits<_InIterator>::iterator_category _Tag;
          return _S_construct(__beg, __end, __a, _Tag());
 }

      template<class _InIterator>
        static _CharT*
        _S_construct_aux(_InIterator __beg, _InIterator __end,
    const _Alloc& __a, __true_type)
 { return _S_construct(static_cast<size_type>(__beg),
         static_cast<value_type>(__end), __a); }

      template<class _InIterator>
        static _CharT*
        _S_construct(_InIterator __beg, _InIterator __end, const _Alloc& __a)
 {
   typedef typename std::__is_integer<_InIterator>::__type _Integral;
   return _S_construct_aux(__beg, __end, __a, _Integral());
        }
      template<class _InIterator>
        static _CharT*
         _S_construct(_InIterator __beg, _InIterator __end, const _Alloc& __a,
        input_iterator_tag);

      template<class _FwdIterator>
        static _CharT*
        _S_construct(_FwdIterator __beg, _FwdIterator __end, const _Alloc& __a,
       forward_iterator_tag);

      static _CharT*
      _S_construct(size_type __req, _CharT __c, const _Alloc& __a);

    public:

      size_type
      copy(_CharT* __s, size_type __n, size_type __pos = 0) const;

      void
      swap(basic_string& __s);

      const _CharT*
      c_str() const
      { return _M_data(); }

      const _CharT*
      data() const
      { return _M_data(); }
      allocator_type
      get_allocator() const
      { return _M_dataplus; }

      size_type
      find(const _CharT* __s, size_type __pos, size_type __n) const;

      size_type
      find(const basic_string& __str, size_type __pos = 0) const
      { return this->find(__str.data(), __pos, __str.size()); }

      size_type
      find(const _CharT* __s, size_type __pos = 0) const
      {
 ;
 return this->find(__s, __pos, traits_type::length(__s));
      }

      size_type
      find(_CharT __c, size_type __pos = 0) const;

      size_type
      rfind(const basic_string& __str, size_type __pos = npos) const
      { return this->rfind(__str.data(), __pos, __str.size()); }

      size_type
      rfind(const _CharT* __s, size_type __pos, size_type __n) const;

      size_type
      rfind(const _CharT* __s, size_type __pos = npos) const
      {
 ;
 return this->rfind(__s, __pos, traits_type::length(__s));
      }

      size_type
      rfind(_CharT __c, size_type __pos = npos) const;

      size_type
      find_first_of(const basic_string& __str, size_type __pos = 0) const
      { return this->find_first_of(__str.data(), __pos, __str.size()); }

      size_type
      find_first_of(const _CharT* __s, size_type __pos, size_type __n) const;

      size_type
      find_first_of(const _CharT* __s, size_type __pos = 0) const
      {
 ;
 return this->find_first_of(__s, __pos, traits_type::length(__s));
      }

      size_type
      find_first_of(_CharT __c, size_type __pos = 0) const
      { return this->find(__c, __pos); }

      size_type
      find_last_of(const basic_string& __str, size_type __pos = npos) const
      { return this->find_last_of(__str.data(), __pos, __str.size()); }

      size_type
      find_last_of(const _CharT* __s, size_type __pos, size_type __n) const;

      size_type
      find_last_of(const _CharT* __s, size_type __pos = npos) const
      {
 ;
 return this->find_last_of(__s, __pos, traits_type::length(__s));
      }

      size_type
      find_last_of(_CharT __c, size_type __pos = npos) const
      { return this->rfind(__c, __pos); }

      size_type
      find_first_not_of(const basic_string& __str, size_type __pos = 0) const
      { return this->find_first_not_of(__str.data(), __pos, __str.size()); }

      size_type
      find_first_not_of(const _CharT* __s, size_type __pos,
   size_type __n) const;

      size_type
      find_first_not_of(const _CharT* __s, size_type __pos = 0) const
      {
 ;
 return this->find_first_not_of(__s, __pos, traits_type::length(__s));
      }

      size_type
      find_first_not_of(_CharT __c, size_type __pos = 0) const;

      size_type
      find_last_not_of(const basic_string& __str, size_type __pos = npos) const
      { return this->find_last_not_of(__str.data(), __pos, __str.size()); }

      size_type
      find_last_not_of(const _CharT* __s, size_type __pos,
         size_type __n) const;

      size_type
      find_last_not_of(const _CharT* __s, size_type __pos = npos) const
      {
 ;
 return this->find_last_not_of(__s, __pos, traits_type::length(__s));
      }

      size_type
      find_last_not_of(_CharT __c, size_type __pos = npos) const;

      basic_string
      substr(size_type __pos = 0, size_type __n = npos) const
      { return basic_string(*this,
       _M_check(__pos, "basic_string::substr"), __n); }

      int
      compare(const basic_string& __str) const
      {
 const size_type __size = this->size();
 const size_type __osize = __str.size();
 const size_type __len = std::min(__size, __osize);

 int __r = traits_type::compare(_M_data(), __str.data(), __len);
 if (!__r)
   __r = _S_compare(__size, __osize);
 return __r;
      }

      int
      compare(size_type __pos, size_type __n, const basic_string& __str) const;

      int
      compare(size_type __pos1, size_type __n1, const basic_string& __str,
       size_type __pos2, size_type __n2) const;

      int
      compare(const _CharT* __s) const;

      int
      compare(size_type __pos, size_type __n1, const _CharT* __s) const;

      int
      compare(size_type __pos, size_type __n1, const _CharT* __s,
       size_type __n2) const;
  };

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline basic_string<_CharT, _Traits, _Alloc>::
    basic_string()

    : _M_dataplus(_S_empty_rep()._M_refdata(), _Alloc()) { }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>
    operator+(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
       const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    {
      basic_string<_CharT, _Traits, _Alloc> __str(__lhs);
      __str.append(__rhs);
      return __str;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT,_Traits,_Alloc>
    operator+(const _CharT* __lhs,
       const basic_string<_CharT,_Traits,_Alloc>& __rhs);

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT,_Traits,_Alloc>
    operator+(_CharT __lhs, const basic_string<_CharT,_Traits,_Alloc>& __rhs);

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline basic_string<_CharT, _Traits, _Alloc>
    operator+(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
      const _CharT* __rhs)
    {
      basic_string<_CharT, _Traits, _Alloc> __str(__lhs);
      __str.append(__rhs);
      return __str;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline basic_string<_CharT, _Traits, _Alloc>
    operator+(const basic_string<_CharT, _Traits, _Alloc>& __lhs, _CharT __rhs)
    {
      typedef basic_string<_CharT, _Traits, _Alloc> __string_type;
      typedef typename __string_type::size_type __size_type;
      __string_type __str(__lhs);
      __str.append(__size_type(1), __rhs);
      return __str;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator==(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
        const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    { return __lhs.compare(__rhs) == 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator==(const _CharT* __lhs,
        const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    { return __rhs.compare(__lhs) == 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator==(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
        const _CharT* __rhs)
    { return __lhs.compare(__rhs) == 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator!=(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
        const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    { return __rhs.compare(__lhs) != 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator!=(const _CharT* __lhs,
        const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    { return __rhs.compare(__lhs) != 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator!=(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
        const _CharT* __rhs)
    { return __lhs.compare(__rhs) != 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator<(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
       const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    { return __lhs.compare(__rhs) < 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator<(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
       const _CharT* __rhs)
    { return __lhs.compare(__rhs) < 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator<(const _CharT* __lhs,
       const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    { return __rhs.compare(__lhs) > 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator>(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
       const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    { return __lhs.compare(__rhs) > 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator>(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
       const _CharT* __rhs)
    { return __lhs.compare(__rhs) > 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator>(const _CharT* __lhs,
       const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    { return __rhs.compare(__lhs) < 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator<=(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
        const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    { return __lhs.compare(__rhs) <= 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator<=(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
        const _CharT* __rhs)
    { return __lhs.compare(__rhs) <= 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator<=(const _CharT* __lhs,
        const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    { return __rhs.compare(__lhs) >= 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator>=(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
        const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    { return __lhs.compare(__rhs) >= 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator>=(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
        const _CharT* __rhs)
    { return __lhs.compare(__rhs) >= 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline bool
    operator>=(const _CharT* __lhs,
      const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    { return __rhs.compare(__lhs) <= 0; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline void
    swap(basic_string<_CharT, _Traits, _Alloc>& __lhs,
  basic_string<_CharT, _Traits, _Alloc>& __rhs)
    { __lhs.swap(__rhs); }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_istream<_CharT, _Traits>&
    operator>>(basic_istream<_CharT, _Traits>& __is,
        basic_string<_CharT, _Traits, _Alloc>& __str);

  template<>
    basic_istream<char>&
    operator>>(basic_istream<char>& __is, basic_string<char>& __str);

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline basic_ostream<_CharT, _Traits>&
    operator<<(basic_ostream<_CharT, _Traits>& __os,
        const basic_string<_CharT, _Traits, _Alloc>& __str)
    {
      return __ostream_insert(__os, __str.data(), __str.size());
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_istream<_CharT, _Traits>&
    getline(basic_istream<_CharT, _Traits>& __is,
     basic_string<_CharT, _Traits, _Alloc>& __str, _CharT __delim);

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline basic_istream<_CharT, _Traits>&
    getline(basic_istream<_CharT, _Traits>& __is,
     basic_string<_CharT, _Traits, _Alloc>& __str)
    { return getline(__is, __str, __is.widen('\n')); }

  template<>
    basic_istream<char>&
    getline(basic_istream<char>& __in, basic_string<char>& __str,
     char __delim);
  template<>
    basic_istream<wchar_t>&
    getline(basic_istream<wchar_t>& __in, basic_string<wchar_t>& __str,
     wchar_t __delim);
}

       

namespace std {
  template<typename _RandomAccessIterator, typename _Distance>
    bool
    __is_heap(_RandomAccessIterator __first, _Distance __n)
    {
      _Distance __parent = 0;
      for (_Distance __child = 1; __child < __n; ++__child)
 {
   if (__first[__parent] < __first[__child])
     return false;
   if ((__child & 1) == 0)
     ++__parent;
 }
      return true;
    }

  template<typename _RandomAccessIterator, typename _Distance,
           typename _StrictWeakOrdering>
    bool
    __is_heap(_RandomAccessIterator __first, _StrictWeakOrdering __comp,
       _Distance __n)
    {
      _Distance __parent = 0;
      for (_Distance __child = 1; __child < __n; ++__child)
 {
   if (__comp(__first[__parent], __first[__child]))
     return false;
   if ((__child & 1) == 0)
     ++__parent;
 }
      return true;
    }

  template<typename _RandomAccessIterator>
    bool
    __is_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
    { return std::__is_heap(__first, std::distance(__first, __last)); }

  template<typename _RandomAccessIterator, typename _StrictWeakOrdering>
    bool
    __is_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
     _StrictWeakOrdering __comp)
    { return std::__is_heap(__first, __comp, std::distance(__first, __last)); }

  template<typename _RandomAccessIterator, typename _Distance, typename _Tp>
    void
    __push_heap(_RandomAccessIterator __first,
  _Distance __holeIndex, _Distance __topIndex, _Tp __value)
    {
      _Distance __parent = (__holeIndex - 1) / 2;
      while (__holeIndex > __topIndex && *(__first + __parent) < __value)
 {
   *(__first + __holeIndex) = *(__first + __parent);
   __holeIndex = __parent;
   __parent = (__holeIndex - 1) / 2;
 }
      *(__first + __holeIndex) = __value;
    }

  template<typename _RandomAccessIterator>
    inline void
    push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
   _ValueType;
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type
   _DistanceType;
     

     
      ;
      std::__push_heap(__first, _DistanceType((__last - __first) - 1),
         _DistanceType(0), _ValueType(*(__last - 1)));
    }

  template<typename _RandomAccessIterator, typename _Distance, typename _Tp,
     typename _Compare>
    void
    __push_heap(_RandomAccessIterator __first, _Distance __holeIndex,
  _Distance __topIndex, _Tp __value, _Compare __comp)
    {
      _Distance __parent = (__holeIndex - 1) / 2;
      while (__holeIndex > __topIndex
      && __comp(*(__first + __parent), __value))
 {
   *(__first + __holeIndex) = *(__first + __parent);
   __holeIndex = __parent;
   __parent = (__holeIndex - 1) / 2;
 }
      *(__first + __holeIndex) = __value;
    }

  template<typename _RandomAccessIterator, typename _Compare>
    inline void
    push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
       _Compare __comp)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
   _ValueType;
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type
   _DistanceType;
     

      ;
      ;

      std::__push_heap(__first, _DistanceType((__last - __first) - 1),
         _DistanceType(0), _ValueType(*(__last - 1)), __comp);
    }

  template<typename _RandomAccessIterator, typename _Distance, typename _Tp>
    void
    __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex,
    _Distance __len, _Tp __value)
    {
      const _Distance __topIndex = __holeIndex;
      _Distance __secondChild = 2 * __holeIndex + 2;
      while (__secondChild < __len)
 {
   if (*(__first + __secondChild) < *(__first + (__secondChild - 1)))
     __secondChild--;
   *(__first + __holeIndex) = *(__first + __secondChild);
   __holeIndex = __secondChild;
   __secondChild = 2 * (__secondChild + 1);
 }
      if (__secondChild == __len)
 {
   *(__first + __holeIndex) = *(__first + (__secondChild - 1));
   __holeIndex = __secondChild - 1;
 }
      std::__push_heap(__first, __holeIndex, __topIndex, __value);
    }

  template<typename _RandomAccessIterator, typename _Tp>
    inline void
    __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
        _RandomAccessIterator __result, _Tp __value)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type
 _Distance;
      *__result = *__first;
      std::__adjust_heap(__first, _Distance(0), _Distance(__last - __first),
    __value);
    }

  template<typename _RandomAccessIterator>
    inline void
    pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;
     

     
      ;
      ;

      std::__pop_heap(__first, __last - 1, __last - 1,
        _ValueType(*(__last - 1)));
    }

  template<typename _RandomAccessIterator, typename _Distance,
    typename _Tp, typename _Compare>
    void
    __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex,
    _Distance __len, _Tp __value, _Compare __comp)
    {
      const _Distance __topIndex = __holeIndex;
      _Distance __secondChild = 2 * __holeIndex + 2;
      while (__secondChild < __len)
 {
   if (__comp(*(__first + __secondChild),
       *(__first + (__secondChild - 1))))
     __secondChild--;
   *(__first + __holeIndex) = *(__first + __secondChild);
   __holeIndex = __secondChild;
   __secondChild = 2 * (__secondChild + 1);
 }
      if (__secondChild == __len)
 {
   *(__first + __holeIndex) = *(__first + (__secondChild - 1));
   __holeIndex = __secondChild - 1;
 }
      std::__push_heap(__first, __holeIndex, __topIndex, __value, __comp);
    }

  template<typename _RandomAccessIterator, typename _Tp, typename _Compare>
    inline void
    __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
        _RandomAccessIterator __result, _Tp __value, _Compare __comp)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type
 _Distance;
      *__result = *__first;
      std::__adjust_heap(__first, _Distance(0), _Distance(__last - __first),
    __value, __comp);
    }

  template<typename _RandomAccessIterator, typename _Compare>
    inline void
    pop_heap(_RandomAccessIterator __first,
      _RandomAccessIterator __last, _Compare __comp)
    {

     

      ;
      ;

      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;
      std::__pop_heap(__first, __last - 1, __last - 1,
        _ValueType(*(__last - 1)), __comp);
    }

  template<typename _RandomAccessIterator>
    void
    make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
   _ValueType;
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type
   _DistanceType;
     

     
      ;

      if (__last - __first < 2)
 return;

      const _DistanceType __len = __last - __first;
      _DistanceType __parent = (__len - 2) / 2;
      while (true)
 {
   std::__adjust_heap(__first, __parent, __len,
        _ValueType(*(__first + __parent)));
   if (__parent == 0)
     return;
   __parent--;
 }
    }

  template<typename _RandomAccessIterator, typename _Compare>
    inline void
    make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
       _Compare __comp)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
   _ValueType;
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type
   _DistanceType;
     

      ;

      if (__last - __first < 2)
 return;

      const _DistanceType __len = __last - __first;
      _DistanceType __parent = (__len - 2) / 2;
      while (true)
 {
   std::__adjust_heap(__first, __parent, __len,
        _ValueType(*(__first + __parent)), __comp);
   if (__parent == 0)
     return;
   __parent--;
 }
    }

  template<typename _RandomAccessIterator>
    void
    sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
    {

     

     

      ;
      while (__last - __first > 1)
 std::pop_heap(__first, _RandomAccessIterator(__last--));
    }

  template<typename _RandomAccessIterator, typename _Compare>
    void
    sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
       _Compare __comp)
    {

     

      ;
      ;

      while (__last - __first > 1)
 std::pop_heap(__first, _RandomAccessIterator(__last--), __comp);
    }

}

namespace std {

  template<typename _ForwardIterator, typename _Tp>
    class _Temporary_buffer
    {

     

    public:
      typedef _Tp value_type;
      typedef value_type* pointer;
      typedef pointer iterator;
      typedef ptrdiff_t size_type;

    protected:
      size_type _M_original_len;
      size_type _M_len;
      pointer _M_buffer;

      void
      _M_initialize_buffer(const _Tp&, __true_type) { }

      void
      _M_initialize_buffer(const _Tp& __val, __false_type)
      { std::uninitialized_fill_n(_M_buffer, _M_len, __val); }

    public:

      size_type
      size() const
      { return _M_len; }
      size_type
      requested_size() const
      { return _M_original_len; }
      iterator
      begin()
      { return _M_buffer; }
      iterator
      end()
      { return _M_buffer + _M_len; }

      _Temporary_buffer(_ForwardIterator __first, _ForwardIterator __last);

      ~_Temporary_buffer()
      {
 std::_Destroy(_M_buffer, _M_buffer + _M_len);
 std::return_temporary_buffer(_M_buffer);
      }

    private:

      _Temporary_buffer(const _Temporary_buffer&);

      void
      operator=(const _Temporary_buffer&);
    };
  template<typename _ForwardIterator, typename _Tp>
    _Temporary_buffer<_ForwardIterator, _Tp>::
    _Temporary_buffer(_ForwardIterator __first, _ForwardIterator __last)
    : _M_original_len(std::distance(__first, __last)),
      _M_len(0), _M_buffer(0)
    {

      typedef typename std::__is_scalar<_Tp>::__type _Trivial;

      try
 {
   pair<pointer, size_type> __p(get_temporary_buffer<
           value_type>(_M_original_len));
   _M_buffer = __p.first;
   _M_len = __p.second;
   if (_M_len > 0)
     _M_initialize_buffer(*__first, _Trivial());
 }
      catch(...)
 {
   std::return_temporary_buffer(_M_buffer);
   _M_buffer = 0;
   _M_len = 0;
   throw;
 }
    }

}

namespace std {

  template<typename _Tp>
    inline const _Tp&
    __median(const _Tp& __a, const _Tp& __b, const _Tp& __c)
    {

     
      if (__a < __b)
 if (__b < __c)
   return __b;
 else if (__a < __c)
   return __c;
 else
   return __a;
      else if (__a < __c)
 return __a;
      else if (__b < __c)
 return __c;
      else
 return __b;
    }

  template<typename _Tp, typename _Compare>
    inline const _Tp&
    __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
    {

     
      if (__comp(__a, __b))
 if (__comp(__b, __c))
   return __b;
 else if (__comp(__a, __c))
   return __c;
 else
   return __a;
      else if (__comp(__a, __c))
 return __a;
      else if (__comp(__b, __c))
 return __c;
      else
 return __b;
    }

  template<typename _InputIterator, typename _Function>
    _Function
    for_each(_InputIterator __first, _InputIterator __last, _Function __f)
    {

     
      ;
      for ( ; __first != __last; ++__first)
 __f(*__first);
      return __f;
    }
  template<typename _InputIterator, typename _Tp>
    inline _InputIterator
    __find(_InputIterator __first, _InputIterator __last,
    const _Tp& __val, input_iterator_tag)
    {
      while (__first != __last && !(*__first == __val))
 ++__first;
      return __first;
    }
  template<typename _InputIterator, typename _Predicate>
    inline _InputIterator
    __find_if(_InputIterator __first, _InputIterator __last,
       _Predicate __pred, input_iterator_tag)
    {
      while (__first != __last && !__pred(*__first))
 ++__first;
      return __first;
    }
  template<typename _RandomAccessIterator, typename _Tp>
    _RandomAccessIterator
    __find(_RandomAccessIterator __first, _RandomAccessIterator __last,
    const _Tp& __val, random_access_iterator_tag)
    {
      typename iterator_traits<_RandomAccessIterator>::difference_type
 __trip_count = (__last - __first) >> 2;

      for ( ; __trip_count > 0 ; --__trip_count)
 {
   if (*__first == __val)
     return __first;
   ++__first;

   if (*__first == __val)
     return __first;
   ++__first;

   if (*__first == __val)
     return __first;
   ++__first;

   if (*__first == __val)
     return __first;
   ++__first;
 }

      switch (__last - __first)
 {
 case 3:
   if (*__first == __val)
     return __first;
   ++__first;
 case 2:
   if (*__first == __val)
     return __first;
   ++__first;
 case 1:
   if (*__first == __val)
     return __first;
   ++__first;
 case 0:
 default:
   return __last;
 }
    }
  template<typename _RandomAccessIterator, typename _Predicate>
    _RandomAccessIterator
    __find_if(_RandomAccessIterator __first, _RandomAccessIterator __last,
       _Predicate __pred, random_access_iterator_tag)
    {
      typename iterator_traits<_RandomAccessIterator>::difference_type
 __trip_count = (__last - __first) >> 2;

      for ( ; __trip_count > 0 ; --__trip_count)
 {
   if (__pred(*__first))
     return __first;
   ++__first;

   if (__pred(*__first))
     return __first;
   ++__first;

   if (__pred(*__first))
     return __first;
   ++__first;

   if (__pred(*__first))
     return __first;
   ++__first;
 }

      switch (__last - __first)
 {
 case 3:
   if (__pred(*__first))
     return __first;
   ++__first;
 case 2:
   if (__pred(*__first))
     return __first;
   ++__first;
 case 1:
   if (__pred(*__first))
     return __first;
   ++__first;
 case 0:
 default:
   return __last;
 }
    }
  template<typename _CharT>
    typename __gnu_cxx::__enable_if<__is_char<_CharT>::__value,
        istreambuf_iterator<_CharT> >::__type
    find(istreambuf_iterator<_CharT>, istreambuf_iterator<_CharT>,
  const _CharT&);

  template<typename _InputIterator, typename _Tp>
    inline _InputIterator
    find(_InputIterator __first, _InputIterator __last,
  const _Tp& __val)
    {

     
     

      ;
      return std::__find(__first, __last, __val,
           std::__iterator_category(__first));
    }

  template<typename _InputIterator, typename _Predicate>
    inline _InputIterator
    find_if(_InputIterator __first, _InputIterator __last,
     _Predicate __pred)
    {

     
     

      ;
      return std::__find_if(__first, __last, __pred,
       std::__iterator_category(__first));
    }

  template<typename _ForwardIterator>
    _ForwardIterator
    adjacent_find(_ForwardIterator __first, _ForwardIterator __last)
    {

     
     

      ;
      if (__first == __last)
 return __last;
      _ForwardIterator __next = __first;
      while(++__next != __last)
 {
   if (*__first == *__next)
     return __first;
   __first = __next;
 }
      return __last;
    }

  template<typename _ForwardIterator, typename _BinaryPredicate>
    _ForwardIterator
    adjacent_find(_ForwardIterator __first, _ForwardIterator __last,
    _BinaryPredicate __binary_pred)
    {

     
     
      ;
      if (__first == __last)
 return __last;
      _ForwardIterator __next = __first;
      while(++__next != __last)
 {
   if (__binary_pred(*__first, *__next))
     return __first;
   __first = __next;
 }
      return __last;
    }

  template<typename _InputIterator, typename _Tp>
    typename iterator_traits<_InputIterator>::difference_type
    count(_InputIterator __first, _InputIterator __last, const _Tp& __value)
    {

     
     

      ;
      typename iterator_traits<_InputIterator>::difference_type __n = 0;
      for ( ; __first != __last; ++__first)
 if (*__first == __value)
   ++__n;
      return __n;
    }

  template<typename _InputIterator, typename _Predicate>
    typename iterator_traits<_InputIterator>::difference_type
    count_if(_InputIterator __first, _InputIterator __last, _Predicate __pred)
    {

     
     

      ;
      typename iterator_traits<_InputIterator>::difference_type __n = 0;
      for ( ; __first != __last; ++__first)
 if (__pred(*__first))
   ++__n;
      return __n;
    }

  template<typename _ForwardIterator1, typename _ForwardIterator2>
    _ForwardIterator1
    search(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
    _ForwardIterator2 __first2, _ForwardIterator2 __last2)
    {

     
     
     
      ;
      ;

      if (__first1 == __last1 || __first2 == __last2)
 return __first1;
      _ForwardIterator2 __tmp(__first2);
      ++__tmp;
      if (__tmp == __last2)
 return std::find(__first1, __last1, *__first2);
      _ForwardIterator2 __p1, __p;
      __p1 = __first2; ++__p1;
      _ForwardIterator1 __current = __first1;

      while (__first1 != __last1)
 {
   __first1 = std::find(__first1, __last1, *__first2);
   if (__first1 == __last1)
     return __last1;

   __p = __p1;
   __current = __first1;
   if (++__current == __last1)
     return __last1;

   while (*__current == *__p)
     {
       if (++__p == __last2)
  return __first1;
       if (++__current == __last1)
  return __last1;
     }
   ++__first1;
 }
      return __first1;
    }

  template<typename _ForwardIterator1, typename _ForwardIterator2,
    typename _BinaryPredicate>
    _ForwardIterator1
    search(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
    _ForwardIterator2 __first2, _ForwardIterator2 __last2,
    _BinaryPredicate __predicate)
    {

     
     
     
      ;
      ;
      if (__first1 == __last1 || __first2 == __last2)
 return __first1;
      _ForwardIterator2 __tmp(__first2);
      ++__tmp;
      if (__tmp == __last2)
 {
   while (__first1 != __last1 && !__predicate(*__first1, *__first2))
     ++__first1;
   return __first1;
 }
      _ForwardIterator2 __p1, __p;
      __p1 = __first2; ++__p1;
      _ForwardIterator1 __current = __first1;

      while (__first1 != __last1)
 {
   while (__first1 != __last1)
     {
       if (__predicate(*__first1, *__first2))
  break;
       ++__first1;
     }
   while (__first1 != __last1 && !__predicate(*__first1, *__first2))
     ++__first1;
   if (__first1 == __last1)
     return __last1;

   __p = __p1;
   __current = __first1;
   if (++__current == __last1)
     return __last1;

   while (__predicate(*__current, *__p))
     {
       if (++__p == __last2)
  return __first1;
       if (++__current == __last1)
  return __last1;
     }
   ++__first1;
 }
      return __first1;
    }

  template<typename _ForwardIterator, typename _Integer, typename _Tp>
    _ForwardIterator
    __search_n(_ForwardIterator __first, _ForwardIterator __last,
        _Integer __count, const _Tp& __val,
        std::forward_iterator_tag)
    {
      __first = std::find(__first, __last, __val);
      while (__first != __last)
 {
   typename iterator_traits<_ForwardIterator>::difference_type
     __n = __count;
   _ForwardIterator __i = __first;
   ++__i;
   while (__i != __last && __n != 1 && *__i == __val)
     {
       ++__i;
       --__n;
     }
   if (__n == 1)
     return __first;
   if (__i == __last)
     return __last;
   __first = std::find(++__i, __last, __val);
 }
      return __last;
    }

  template<typename _RandomAccessIter, typename _Integer, typename _Tp>
    _RandomAccessIter
    __search_n(_RandomAccessIter __first, _RandomAccessIter __last,
        _Integer __count, const _Tp& __val,
        std::random_access_iterator_tag)
    {

      typedef typename std::iterator_traits<_RandomAccessIter>::difference_type
 _DistanceType;

      _DistanceType __tailSize = __last - __first;
      const _DistanceType __pattSize = __count;

      if (__tailSize < __pattSize)
        return __last;

      const _DistanceType __skipOffset = __pattSize - 1;
      _RandomAccessIter __lookAhead = __first + __skipOffset;
      __tailSize -= __pattSize;

      while (1)
 {
   while (!(*__lookAhead == __val))
     {
       if (__tailSize < __pattSize)
  return __last;
       __lookAhead += __pattSize;
       __tailSize -= __pattSize;
     }
   _DistanceType __remainder = __skipOffset;
   for (_RandomAccessIter __backTrack = __lookAhead - 1;
        *__backTrack == __val; --__backTrack)
     {
       if (--__remainder == 0)
  return (__lookAhead - __skipOffset);
     }
   if (__remainder > __tailSize)
     return __last;
   __lookAhead += __remainder;
   __tailSize -= __remainder;
 }
    }

  template<typename _ForwardIterator, typename _Integer, typename _Tp>
    _ForwardIterator
    search_n(_ForwardIterator __first, _ForwardIterator __last,
      _Integer __count, const _Tp& __val)
    {

     
     

      ;

      if (__count <= 0)
 return __first;
      if (__count == 1)
 return std::find(__first, __last, __val);
      return std::__search_n(__first, __last, __count, __val,
        std::__iterator_category(__first));
    }

  template<typename _ForwardIterator, typename _Integer, typename _Tp,
           typename _BinaryPredicate>
    _ForwardIterator
    __search_n(_ForwardIterator __first, _ForwardIterator __last,
        _Integer __count, const _Tp& __val,
        _BinaryPredicate __binary_pred, std::forward_iterator_tag)
    {
      while (__first != __last && !__binary_pred(*__first, __val))
        ++__first;

      while (__first != __last)
 {
   typename iterator_traits<_ForwardIterator>::difference_type
     __n = __count;
   _ForwardIterator __i = __first;
   ++__i;
   while (__i != __last && __n != 1 && __binary_pred(*__i, __val))
     {
       ++__i;
       --__n;
     }
   if (__n == 1)
     return __first;
   if (__i == __last)
     return __last;
   __first = ++__i;
   while (__first != __last && !__binary_pred(*__first, __val))
     ++__first;
 }
      return __last;
    }

  template<typename _RandomAccessIter, typename _Integer, typename _Tp,
    typename _BinaryPredicate>
    _RandomAccessIter
    __search_n(_RandomAccessIter __first, _RandomAccessIter __last,
        _Integer __count, const _Tp& __val,
        _BinaryPredicate __binary_pred, std::random_access_iterator_tag)
    {

      typedef typename std::iterator_traits<_RandomAccessIter>::difference_type
 _DistanceType;

      _DistanceType __tailSize = __last - __first;
      const _DistanceType __pattSize = __count;

      if (__tailSize < __pattSize)
        return __last;

      const _DistanceType __skipOffset = __pattSize - 1;
      _RandomAccessIter __lookAhead = __first + __skipOffset;
      __tailSize -= __pattSize;

      while (1)
 {
   while (!__binary_pred(*__lookAhead, __val))
     {
       if (__tailSize < __pattSize)
  return __last;
       __lookAhead += __pattSize;
       __tailSize -= __pattSize;
     }
   _DistanceType __remainder = __skipOffset;
   for (_RandomAccessIter __backTrack = __lookAhead - 1;
        __binary_pred(*__backTrack, __val); --__backTrack)
     {
       if (--__remainder == 0)
  return (__lookAhead - __skipOffset);
     }
   if (__remainder > __tailSize)
     return __last;
   __lookAhead += __remainder;
   __tailSize -= __remainder;
 }
    }

  template<typename _ForwardIterator, typename _Integer, typename _Tp,
           typename _BinaryPredicate>
    _ForwardIterator
    search_n(_ForwardIterator __first, _ForwardIterator __last,
      _Integer __count, const _Tp& __val,
      _BinaryPredicate __binary_pred)
    {

     
     

      ;

      if (__count <= 0)
 return __first;
      if (__count == 1)
 {
   while (__first != __last && !__binary_pred(*__first, __val))
     ++__first;
   return __first;
 }
      return std::__search_n(__first, __last, __count, __val, __binary_pred,
        std::__iterator_category(__first));
    }

  template<typename _ForwardIterator1, typename _ForwardIterator2>
    _ForwardIterator2
    swap_ranges(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
  _ForwardIterator2 __first2)
    {

     

     

     
     
      ;

      for ( ; __first1 != __last1; ++__first1, ++__first2)
 std::iter_swap(__first1, __first2);
      return __first2;
    }

  template<typename _InputIterator, typename _OutputIterator,
    typename _UnaryOperation>
    _OutputIterator
    transform(_InputIterator __first, _InputIterator __last,
       _OutputIterator __result, _UnaryOperation __unary_op)
    {

     
     
      ;

      for ( ; __first != __last; ++__first, ++__result)
 *__result = __unary_op(*__first);
      return __result;
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _OutputIterator, typename _BinaryOperation>
    _OutputIterator
    transform(_InputIterator1 __first1, _InputIterator1 __last1,
       _InputIterator2 __first2, _OutputIterator __result,
       _BinaryOperation __binary_op)
    {

     
     
     
      ;

      for ( ; __first1 != __last1; ++__first1, ++__first2, ++__result)
 *__result = __binary_op(*__first1, *__first2);
      return __result;
    }

  template<typename _ForwardIterator, typename _Tp>
    void
    replace(_ForwardIterator __first, _ForwardIterator __last,
     const _Tp& __old_value, const _Tp& __new_value)
    {

     

     

     

      ;

      for ( ; __first != __last; ++__first)
 if (*__first == __old_value)
   *__first = __new_value;
    }

  template<typename _ForwardIterator, typename _Predicate, typename _Tp>
    void
    replace_if(_ForwardIterator __first, _ForwardIterator __last,
        _Predicate __pred, const _Tp& __new_value)
    {

     

     

     

      ;

      for ( ; __first != __last; ++__first)
 if (__pred(*__first))
   *__first = __new_value;
    }

  template<typename _InputIterator, typename _OutputIterator, typename _Tp>
    _OutputIterator
    replace_copy(_InputIterator __first, _InputIterator __last,
   _OutputIterator __result,
   const _Tp& __old_value, const _Tp& __new_value)
    {

     
     

     

      ;

      for ( ; __first != __last; ++__first, ++__result)
 if (*__first == __old_value)
   *__result = __new_value;
 else
   *__result = *__first;
      return __result;
    }

  template<typename _InputIterator, typename _OutputIterator,
    typename _Predicate, typename _Tp>
    _OutputIterator
    replace_copy_if(_InputIterator __first, _InputIterator __last,
      _OutputIterator __result,
      _Predicate __pred, const _Tp& __new_value)
    {

     
     

     

      ;

      for ( ; __first != __last; ++__first, ++__result)
 if (__pred(*__first))
   *__result = __new_value;
 else
   *__result = *__first;
      return __result;
    }

  template<typename _ForwardIterator, typename _Generator>
    void
    generate(_ForwardIterator __first, _ForwardIterator __last,
      _Generator __gen)
    {

     
     

      ;

      for ( ; __first != __last; ++__first)
 *__first = __gen();
    }

  template<typename _OutputIterator, typename _Size, typename _Generator>
    _OutputIterator
    generate_n(_OutputIterator __first, _Size __n, _Generator __gen)
    {

     

      for ( ; __n > 0; --__n, ++__first)
 *__first = __gen();
      return __first;
    }

  template<typename _InputIterator, typename _OutputIterator, typename _Tp>
    _OutputIterator
    remove_copy(_InputIterator __first, _InputIterator __last,
  _OutputIterator __result, const _Tp& __value)
    {

     
     

     

      ;

      for ( ; __first != __last; ++__first)
 if (!(*__first == __value))
   {
     *__result = *__first;
     ++__result;
   }
      return __result;
    }

  template<typename _InputIterator, typename _OutputIterator,
    typename _Predicate>
    _OutputIterator
    remove_copy_if(_InputIterator __first, _InputIterator __last,
     _OutputIterator __result, _Predicate __pred)
    {

     
     

     

      ;

      for ( ; __first != __last; ++__first)
 if (!__pred(*__first))
   {
     *__result = *__first;
     ++__result;
   }
      return __result;
    }

  template<typename _ForwardIterator, typename _Tp>
    _ForwardIterator
    remove(_ForwardIterator __first, _ForwardIterator __last,
    const _Tp& __value)
    {

     

     

      ;

      __first = std::find(__first, __last, __value);
      _ForwardIterator __i = __first;
      return __first == __last ? __first
          : std::remove_copy(++__i, __last,
        __first, __value);
    }

  template<typename _ForwardIterator, typename _Predicate>
    _ForwardIterator
    remove_if(_ForwardIterator __first, _ForwardIterator __last,
       _Predicate __pred)
    {

     

     

      ;

      __first = std::find_if(__first, __last, __pred);
      _ForwardIterator __i = __first;
      return __first == __last ? __first
          : std::remove_copy_if(++__i, __last,
           __first, __pred);
    }

  template<typename _ForwardIterator, typename _OutputIterator>
    _OutputIterator
    __unique_copy(_ForwardIterator __first, _ForwardIterator __last,
    _OutputIterator __result,
    forward_iterator_tag, output_iterator_tag)
    {

      _ForwardIterator __next = __first;
      *__result = *__first;
      while (++__next != __last)
 if (!(*__first == *__next))
   {
     __first = __next;
     *++__result = *__first;
   }
      return ++__result;
    }

  template<typename _InputIterator, typename _OutputIterator>
    _OutputIterator
    __unique_copy(_InputIterator __first, _InputIterator __last,
    _OutputIterator __result,
    input_iterator_tag, output_iterator_tag)
    {

      typename iterator_traits<_InputIterator>::value_type __value = *__first;
      *__result = __value;
      while (++__first != __last)
 if (!(__value == *__first))
   {
     __value = *__first;
     *++__result = __value;
   }
      return ++__result;
    }

  template<typename _InputIterator, typename _ForwardIterator>
    _ForwardIterator
    __unique_copy(_InputIterator __first, _InputIterator __last,
    _ForwardIterator __result,
    input_iterator_tag, forward_iterator_tag)
    {

      *__result = *__first;
      while (++__first != __last)
 if (!(*__result == *__first))
   *++__result = *__first;
      return ++__result;
    }

  template<typename _ForwardIterator, typename _OutputIterator,
    typename _BinaryPredicate>
    _OutputIterator
    __unique_copy(_ForwardIterator __first, _ForwardIterator __last,
    _OutputIterator __result, _BinaryPredicate __binary_pred,
    forward_iterator_tag, output_iterator_tag)
    {

     

      _ForwardIterator __next = __first;
      *__result = *__first;
      while (++__next != __last)
 if (!__binary_pred(*__first, *__next))
   {
     __first = __next;
     *++__result = *__first;
   }
      return ++__result;
    }

  template<typename _InputIterator, typename _OutputIterator,
    typename _BinaryPredicate>
    _OutputIterator
    __unique_copy(_InputIterator __first, _InputIterator __last,
    _OutputIterator __result, _BinaryPredicate __binary_pred,
    input_iterator_tag, output_iterator_tag)
    {

     

      typename iterator_traits<_InputIterator>::value_type __value = *__first;
      *__result = __value;
      while (++__first != __last)
 if (!__binary_pred(__value, *__first))
   {
     __value = *__first;
     *++__result = __value;
   }
      return ++__result;
    }

  template<typename _InputIterator, typename _ForwardIterator,
    typename _BinaryPredicate>
    _ForwardIterator
    __unique_copy(_InputIterator __first, _InputIterator __last,
    _ForwardIterator __result, _BinaryPredicate __binary_pred,
    input_iterator_tag, forward_iterator_tag)
    {

     

      *__result = *__first;
      while (++__first != __last)
 if (!__binary_pred(*__result, *__first))
   *++__result = *__first;
      return ++__result;
    }

  template<typename _InputIterator, typename _OutputIterator>
    inline _OutputIterator
    unique_copy(_InputIterator __first, _InputIterator __last,
  _OutputIterator __result)
    {

     
     

     

      ;

      if (__first == __last)
 return __result;
      return std::__unique_copy(__first, __last, __result,
    std::__iterator_category(__first),
    std::__iterator_category(__result));
    }

  template<typename _InputIterator, typename _OutputIterator,
    typename _BinaryPredicate>
    inline _OutputIterator
    unique_copy(_InputIterator __first, _InputIterator __last,
  _OutputIterator __result,
  _BinaryPredicate __binary_pred)
    {

     
     

      ;

      if (__first == __last)
 return __result;
      return std::__unique_copy(__first, __last, __result, __binary_pred,
    std::__iterator_category(__first),
    std::__iterator_category(__result));
    }

  template<typename _ForwardIterator>
    _ForwardIterator
    unique(_ForwardIterator __first, _ForwardIterator __last)
    {

     

     

      ;
      __first = std::adjacent_find(__first, __last);
      if (__first == __last)
 return __last;
      _ForwardIterator __dest = __first;
      ++__first;
      while (++__first != __last)
 if (!(*__dest == *__first))
   *++__dest = *__first;
      return ++__dest;
    }

  template<typename _ForwardIterator, typename _BinaryPredicate>
    _ForwardIterator
    unique(_ForwardIterator __first, _ForwardIterator __last,
           _BinaryPredicate __binary_pred)
    {

     

     
      ;
      __first = std::adjacent_find(__first, __last, __binary_pred);
      if (__first == __last)
 return __last;
      _ForwardIterator __dest = __first;
      ++__first;
      while (++__first != __last)
 if (!__binary_pred(*__dest, *__first))
   *++__dest = *__first;
      return ++__dest;
    }

  template<typename _BidirectionalIterator>
    void
    __reverse(_BidirectionalIterator __first, _BidirectionalIterator __last,
       bidirectional_iterator_tag)
    {
      while (true)
 if (__first == __last || __first == --__last)
   return;
 else
   {
     std::iter_swap(__first, __last);
     ++__first;
   }
    }

  template<typename _RandomAccessIterator>
    void
    __reverse(_RandomAccessIterator __first, _RandomAccessIterator __last,
       random_access_iterator_tag)
    {
      if (__first == __last)
 return;
      --__last;
      while (__first < __last)
 {
   std::iter_swap(__first, __last);
   ++__first;
   --__last;
 }
    }

  template<typename _BidirectionalIterator>
    inline void
    reverse(_BidirectionalIterator __first, _BidirectionalIterator __last)
    {

     

      ;
      std::__reverse(__first, __last, std::__iterator_category(__first));
    }

  template<typename _BidirectionalIterator, typename _OutputIterator>
    _OutputIterator
    reverse_copy(_BidirectionalIterator __first, _BidirectionalIterator __last,
        _OutputIterator __result)
    {

     

     

      ;

      while (__first != __last)
 {
   --__last;
   *__result = *__last;
   ++__result;
 }
      return __result;
    }

  template<typename _EuclideanRingElement>
    _EuclideanRingElement
    __gcd(_EuclideanRingElement __m, _EuclideanRingElement __n)
    {
      while (__n != 0)
 {
   _EuclideanRingElement __t = __m % __n;
   __m = __n;
   __n = __t;
 }
      return __m;
    }
  template<typename _ForwardIterator>
    void
    __rotate(_ForwardIterator __first,
      _ForwardIterator __middle,
      _ForwardIterator __last,
      forward_iterator_tag)
    {
      if (__first == __middle || __last == __middle)
 return;

      _ForwardIterator __first2 = __middle;
      do
 {
   swap(*__first, *__first2);
   ++__first;
   ++__first2;
   if (__first == __middle)
     __middle = __first2;
 }
      while (__first2 != __last);

      __first2 = __middle;

      while (__first2 != __last)
 {
   swap(*__first, *__first2);
   ++__first;
   ++__first2;
   if (__first == __middle)
     __middle = __first2;
   else if (__first2 == __last)
     __first2 = __middle;
 }
    }
  template<typename _BidirectionalIterator>
    void
    __rotate(_BidirectionalIterator __first,
      _BidirectionalIterator __middle,
      _BidirectionalIterator __last,
       bidirectional_iterator_tag)
    {

     
      if (__first == __middle || __last == __middle)
 return;

      std::__reverse(__first, __middle, bidirectional_iterator_tag());
      std::__reverse(__middle, __last, bidirectional_iterator_tag());

      while (__first != __middle && __middle != __last)
 {
   swap(*__first, *--__last);
   ++__first;
 }

      if (__first == __middle)
 std::__reverse(__middle, __last, bidirectional_iterator_tag());
      else
 std::__reverse(__first, __middle, bidirectional_iterator_tag());
    }
  template<typename _RandomAccessIterator>
    void
    __rotate(_RandomAccessIterator __first,
      _RandomAccessIterator __middle,
      _RandomAccessIterator __last,
      random_access_iterator_tag)
    {

     
      if (__first == __middle || __last == __middle)
 return;

      typedef typename iterator_traits<_RandomAccessIterator>::difference_type
 _Distance;
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;

      const _Distance __n = __last - __first;
      const _Distance __k = __middle - __first;
      const _Distance __l = __n - __k;

      if (__k == __l)
 {
   std::swap_ranges(__first, __middle, __middle);
   return;
 }

      const _Distance __d = __gcd(__n, __k);

      for (_Distance __i = 0; __i < __d; __i++)
 {
   _ValueType __tmp = *__first;
   _RandomAccessIterator __p = __first;

   if (__k < __l)
     {
       for (_Distance __j = 0; __j < __l / __d; __j++)
  {
    if (__p > __first + __l)
      {
        *__p = *(__p - __l);
        __p -= __l;
      }

    *__p = *(__p + __k);
    __p += __k;
  }
     }
   else
     {
       for (_Distance __j = 0; __j < __k / __d - 1; __j ++)
  {
    if (__p < __last - __k)
      {
        *__p = *(__p + __k);
        __p += __k;
      }
    *__p = * (__p - __l);
    __p -= __l;
  }
     }

   *__p = __tmp;
   ++__first;
 }
    }

  template<typename _ForwardIterator>
    inline void
    rotate(_ForwardIterator __first, _ForwardIterator __middle,
    _ForwardIterator __last)
    {

     

      ;
      ;

      typedef typename iterator_traits<_ForwardIterator>::iterator_category
 _IterType;
      std::__rotate(__first, __middle, __last, _IterType());
    }

  template<typename _ForwardIterator, typename _OutputIterator>
    _OutputIterator
    rotate_copy(_ForwardIterator __first, _ForwardIterator __middle,
                _ForwardIterator __last, _OutputIterator __result)
    {

     
     

      ;
      ;

      return std::copy(__first, __middle,
                       std::copy(__middle, __last, __result));
    }

  template<typename _RandomAccessIterator>
    inline void
    random_shuffle(_RandomAccessIterator __first, _RandomAccessIterator __last)
    {

     

      ;

      if (__first != __last)
 for (_RandomAccessIterator __i = __first + 1; __i != __last; ++__i)
   std::iter_swap(__i, __first + (std::rand() % ((__i - __first) + 1)));
    }

  template<typename _RandomAccessIterator, typename _RandomNumberGenerator>
    void
    random_shuffle(_RandomAccessIterator __first, _RandomAccessIterator __last,
     _RandomNumberGenerator& __rand)
    {

     

      ;

      if (__first == __last)
 return;
      for (_RandomAccessIterator __i = __first + 1; __i != __last; ++__i)
 std::iter_swap(__i, __first + __rand((__i - __first) + 1));
    }

  template<typename _ForwardIterator, typename _Predicate>
    _ForwardIterator
    __partition(_ForwardIterator __first, _ForwardIterator __last,
  _Predicate __pred,
  forward_iterator_tag)
    {
      if (__first == __last)
 return __first;

      while (__pred(*__first))
 if (++__first == __last)
   return __first;

      _ForwardIterator __next = __first;

      while (++__next != __last)
 if (__pred(*__next))
   {
     swap(*__first, *__next);
     ++__first;
   }

      return __first;
    }
  template<typename _BidirectionalIterator, typename _Predicate>
    _BidirectionalIterator
    __partition(_BidirectionalIterator __first, _BidirectionalIterator __last,
  _Predicate __pred,
  bidirectional_iterator_tag)
    {
      while (true)
 {
   while (true)
     if (__first == __last)
       return __first;
     else if (__pred(*__first))
       ++__first;
     else
       break;
   --__last;
   while (true)
     if (__first == __last)
       return __first;
     else if (!__pred(*__last))
       --__last;
     else
       break;
   std::iter_swap(__first, __last);
   ++__first;
 }
    }

  template<typename _ForwardIterator, typename _Predicate>
    inline _ForwardIterator
    partition(_ForwardIterator __first, _ForwardIterator __last,
       _Predicate __pred)
    {

     

     

      ;

      return std::__partition(__first, __last, __pred,
         std::__iterator_category(__first));
    }

  template<typename _ForwardIterator, typename _Predicate, typename _Distance>
    _ForwardIterator
    __inplace_stable_partition(_ForwardIterator __first,
          _ForwardIterator __last,
          _Predicate __pred, _Distance __len)
    {
      if (__len == 1)
 return __pred(*__first) ? __last : __first;
      _ForwardIterator __middle = __first;
      std::advance(__middle, __len / 2);
      _ForwardIterator __begin = std::__inplace_stable_partition(__first,
         __middle,
         __pred,
         __len / 2);
      _ForwardIterator __end = std::__inplace_stable_partition(__middle, __last,
              __pred,
              __len
              - __len / 2);
      std::rotate(__begin, __middle, __end);
      std::advance(__begin, std::distance(__middle, __end));
      return __begin;
    }
  template<typename _ForwardIterator, typename _Pointer, typename _Predicate,
    typename _Distance>
    _ForwardIterator
    __stable_partition_adaptive(_ForwardIterator __first,
    _ForwardIterator __last,
    _Predicate __pred, _Distance __len,
    _Pointer __buffer,
    _Distance __buffer_size)
    {
      if (__len <= __buffer_size)
 {
   _ForwardIterator __result1 = __first;
   _Pointer __result2 = __buffer;
   for ( ; __first != __last ; ++__first)
     if (__pred(*__first))
       {
  *__result1 = *__first;
  ++__result1;
       }
     else
       {
  *__result2 = *__first;
  ++__result2;
       }
   std::copy(__buffer, __result2, __result1);
   return __result1;
 }
      else
 {
   _ForwardIterator __middle = __first;
   std::advance(__middle, __len / 2);
   _ForwardIterator __begin =
     std::__stable_partition_adaptive(__first, __middle, __pred,
          __len / 2, __buffer,
          __buffer_size);
   _ForwardIterator __end =
     std::__stable_partition_adaptive(__middle, __last, __pred,
          __len - __len / 2,
          __buffer, __buffer_size);
   std::rotate(__begin, __middle, __end);
   std::advance(__begin, std::distance(__middle, __end));
   return __begin;
 }
    }

  template<typename _ForwardIterator, typename _Predicate>
    _ForwardIterator
    stable_partition(_ForwardIterator __first, _ForwardIterator __last,
       _Predicate __pred)
    {

     

     

      ;

      if (__first == __last)
 return __first;
      else
 {
   typedef typename iterator_traits<_ForwardIterator>::value_type
     _ValueType;
   typedef typename iterator_traits<_ForwardIterator>::difference_type
     _DistanceType;

   _Temporary_buffer<_ForwardIterator, _ValueType> __buf(__first,
        __last);
 if (__buf.size() > 0)
   return
     std::__stable_partition_adaptive(__first, __last, __pred,
       _DistanceType(__buf.requested_size()),
       __buf.begin(), __buf.size());
 else
   return
     std::__inplace_stable_partition(__first, __last, __pred,
      _DistanceType(__buf.requested_size()));
 }
    }
  template<typename _RandomAccessIterator, typename _Tp>
    _RandomAccessIterator
    __unguarded_partition(_RandomAccessIterator __first,
     _RandomAccessIterator __last, _Tp __pivot)
    {
      while (true)
 {
   while (*__first < __pivot)
     ++__first;
   --__last;
   while (__pivot < *__last)
     --__last;
   if (!(__first < __last))
     return __first;
   std::iter_swap(__first, __last);
   ++__first;
 }
    }
  template<typename _RandomAccessIterator, typename _Tp, typename _Compare>
    _RandomAccessIterator
    __unguarded_partition(_RandomAccessIterator __first,
     _RandomAccessIterator __last,
     _Tp __pivot, _Compare __comp)
    {
      while (true)
 {
   while (__comp(*__first, __pivot))
     ++__first;
   --__last;
   while (__comp(__pivot, *__last))
     --__last;
   if (!(__first < __last))
     return __first;
   std::iter_swap(__first, __last);
   ++__first;
 }
    }

  enum { _S_threshold = 16 };
  template<typename _RandomAccessIterator, typename _Tp>
    void
    __unguarded_linear_insert(_RandomAccessIterator __last, _Tp __val)
    {
      _RandomAccessIterator __next = __last;
      --__next;
      while (__val < *__next)
 {
   *__last = *__next;
   __last = __next;
   --__next;
 }
      *__last = __val;
    }
  template<typename _RandomAccessIterator, typename _Tp, typename _Compare>
    void
    __unguarded_linear_insert(_RandomAccessIterator __last, _Tp __val,
         _Compare __comp)
    {
      _RandomAccessIterator __next = __last;
      --__next;
      while (__comp(__val, *__next))
 {
   *__last = *__next;
   __last = __next;
   --__next;
 }
      *__last = __val;
    }
  template<typename _RandomAccessIterator>
    void
    __insertion_sort(_RandomAccessIterator __first,
       _RandomAccessIterator __last)
    {
      if (__first == __last)
 return;

      for (_RandomAccessIterator __i = __first + 1; __i != __last; ++__i)
 {
   typename iterator_traits<_RandomAccessIterator>::value_type
     __val = *__i;
   if (__val < *__first)
     {
       std::copy_backward(__first, __i, __i + 1);
       *__first = __val;
     }
   else
     std::__unguarded_linear_insert(__i, __val);
 }
    }
  template<typename _RandomAccessIterator, typename _Compare>
    void
    __insertion_sort(_RandomAccessIterator __first,
       _RandomAccessIterator __last, _Compare __comp)
    {
      if (__first == __last) return;

      for (_RandomAccessIterator __i = __first + 1; __i != __last; ++__i)
 {
   typename iterator_traits<_RandomAccessIterator>::value_type
     __val = *__i;
   if (__comp(__val, *__first))
     {
       std::copy_backward(__first, __i, __i + 1);
       *__first = __val;
     }
   else
     std::__unguarded_linear_insert(__i, __val, __comp);
 }
    }
  template<typename _RandomAccessIterator>
    inline void
    __unguarded_insertion_sort(_RandomAccessIterator __first,
          _RandomAccessIterator __last)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;

      for (_RandomAccessIterator __i = __first; __i != __last; ++__i)
 std::__unguarded_linear_insert(__i, _ValueType(*__i));
    }
  template<typename _RandomAccessIterator, typename _Compare>
    inline void
    __unguarded_insertion_sort(_RandomAccessIterator __first,
          _RandomAccessIterator __last, _Compare __comp)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;

      for (_RandomAccessIterator __i = __first; __i != __last; ++__i)
 std::__unguarded_linear_insert(__i, _ValueType(*__i), __comp);
    }
  template<typename _RandomAccessIterator>
    void
    __final_insertion_sort(_RandomAccessIterator __first,
      _RandomAccessIterator __last)
    {
      if (__last - __first > int(_S_threshold))
 {
   std::__insertion_sort(__first, __first + int(_S_threshold));
   std::__unguarded_insertion_sort(__first + int(_S_threshold), __last);
 }
      else
 std::__insertion_sort(__first, __last);
    }
  template<typename _RandomAccessIterator, typename _Compare>
    void
    __final_insertion_sort(_RandomAccessIterator __first,
      _RandomAccessIterator __last, _Compare __comp)
    {
      if (__last - __first > int(_S_threshold))
 {
   std::__insertion_sort(__first, __first + int(_S_threshold), __comp);
   std::__unguarded_insertion_sort(__first + int(_S_threshold), __last,
       __comp);
 }
      else
 std::__insertion_sort(__first, __last, __comp);
    }
  template<typename _RandomAccessIterator>
    void
    __heap_select(_RandomAccessIterator __first,
    _RandomAccessIterator __middle,
    _RandomAccessIterator __last)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;

      std::make_heap(__first, __middle);
      for (_RandomAccessIterator __i = __middle; __i < __last; ++__i)
 if (*__i < *__first)
   std::__pop_heap(__first, __middle, __i, _ValueType(*__i));
    }
  template<typename _RandomAccessIterator, typename _Compare>
    void
    __heap_select(_RandomAccessIterator __first,
    _RandomAccessIterator __middle,
    _RandomAccessIterator __last, _Compare __comp)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;

      std::make_heap(__first, __middle, __comp);
      for (_RandomAccessIterator __i = __middle; __i < __last; ++__i)
 if (__comp(*__i, *__first))
   std::__pop_heap(__first, __middle, __i, _ValueType(*__i), __comp);
    }
  template<typename _Size>
    inline _Size
    __lg(_Size __n)
    {
      _Size __k;
      for (__k = 0; __n != 1; __n >>= 1)
 ++__k;
      return __k;
    }

  template<typename _RandomAccessIterator>
    inline void
    partial_sort(_RandomAccessIterator __first,
   _RandomAccessIterator __middle,
   _RandomAccessIterator __last)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;
     

     
      ;
      ;

      std::__heap_select(__first, __middle, __last);
      std::sort_heap(__first, __middle);
    }

  template<typename _RandomAccessIterator, typename _Compare>
    inline void
    partial_sort(_RandomAccessIterator __first,
   _RandomAccessIterator __middle,
   _RandomAccessIterator __last,
   _Compare __comp)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;
     

     

      ;
      ;

      std::__heap_select(__first, __middle, __last, __comp);
      std::sort_heap(__first, __middle, __comp);
    }

  template<typename _InputIterator, typename _RandomAccessIterator>
    _RandomAccessIterator
    partial_sort_copy(_InputIterator __first, _InputIterator __last,
        _RandomAccessIterator __result_first,
        _RandomAccessIterator __result_last)
    {
      typedef typename iterator_traits<_InputIterator>::value_type
 _InputValueType;
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _OutputValueType;
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type
 _DistanceType;
     
     

     

     
      ;
      ;

      if (__result_first == __result_last)
 return __result_last;
      _RandomAccessIterator __result_real_last = __result_first;
      while(__first != __last && __result_real_last != __result_last)
 {
   *__result_real_last = *__first;
   ++__result_real_last;
   ++__first;
 }
      std::make_heap(__result_first, __result_real_last);
      while (__first != __last)
 {
   if (*__first < *__result_first)
     std::__adjust_heap(__result_first, _DistanceType(0),
          _DistanceType(__result_real_last
          - __result_first),
          _InputValueType(*__first));
   ++__first;
 }
      std::sort_heap(__result_first, __result_real_last);
      return __result_real_last;
    }

  template<typename _InputIterator, typename _RandomAccessIterator, typename _Compare>
    _RandomAccessIterator
    partial_sort_copy(_InputIterator __first, _InputIterator __last,
        _RandomAccessIterator __result_first,
        _RandomAccessIterator __result_last,
        _Compare __comp)
    {
      typedef typename iterator_traits<_InputIterator>::value_type
 _InputValueType;
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _OutputValueType;
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type
 _DistanceType;
     
     

     

     

     

      ;
      ;

      if (__result_first == __result_last)
 return __result_last;
      _RandomAccessIterator __result_real_last = __result_first;
      while(__first != __last && __result_real_last != __result_last)
 {
   *__result_real_last = *__first;
   ++__result_real_last;
   ++__first;
 }
      std::make_heap(__result_first, __result_real_last, __comp);
      while (__first != __last)
 {
   if (__comp(*__first, *__result_first))
     std::__adjust_heap(__result_first, _DistanceType(0),
          _DistanceType(__result_real_last
          - __result_first),
          _InputValueType(*__first),
          __comp);
   ++__first;
 }
      std::sort_heap(__result_first, __result_real_last, __comp);
      return __result_real_last;
    }
  template<typename _RandomAccessIterator, typename _Size>
    void
    __introsort_loop(_RandomAccessIterator __first,
       _RandomAccessIterator __last,
       _Size __depth_limit)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;

      while (__last - __first > int(_S_threshold))
 {
   if (__depth_limit == 0)
     {
       std::partial_sort(__first, __last, __last);
       return;
     }
   --__depth_limit;
   _RandomAccessIterator __cut =
     std::__unguarded_partition(__first, __last,
           _ValueType(std::__median(*__first,
        *(__first
          + (__last
             - __first)
          / 2),
        *(__last
          - 1))));
   std::__introsort_loop(__cut, __last, __depth_limit);
   __last = __cut;
 }
    }
  template<typename _RandomAccessIterator, typename _Size, typename _Compare>
    void
    __introsort_loop(_RandomAccessIterator __first,
       _RandomAccessIterator __last,
       _Size __depth_limit, _Compare __comp)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;

      while (__last - __first > int(_S_threshold))
 {
   if (__depth_limit == 0)
     {
       std::partial_sort(__first, __last, __last, __comp);
       return;
     }
   --__depth_limit;
   _RandomAccessIterator __cut =
     std::__unguarded_partition(__first, __last,
           _ValueType(std::__median(*__first,
        *(__first
          + (__last
             - __first)
          / 2),
        *(__last - 1),
        __comp)),
           __comp);
   std::__introsort_loop(__cut, __last, __depth_limit, __comp);
   __last = __cut;
 }
    }

  template<typename _RandomAccessIterator>
    inline void
    sort(_RandomAccessIterator __first, _RandomAccessIterator __last)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;
     

     
      ;

      if (__first != __last)
 {
   std::__introsort_loop(__first, __last,
    std::__lg(__last - __first) * 2);
   std::__final_insertion_sort(__first, __last);
 }
    }

  template<typename _RandomAccessIterator, typename _Compare>
    inline void
    sort(_RandomAccessIterator __first, _RandomAccessIterator __last,
  _Compare __comp)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;
     

     

      ;

      if (__first != __last)
 {
   std::__introsort_loop(__first, __last,
    std::__lg(__last - __first) * 2, __comp);
   std::__final_insertion_sort(__first, __last, __comp);
 }
    }

  template<typename _ForwardIterator, typename _Tp>
    _ForwardIterator
    lower_bound(_ForwardIterator __first, _ForwardIterator __last,
  const _Tp& __val)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
 _ValueType;
      typedef typename iterator_traits<_ForwardIterator>::difference_type
 _DistanceType;
     
     
      ;

      _DistanceType __len = std::distance(__first, __last);
      _DistanceType __half;
      _ForwardIterator __middle;

      while (__len > 0)
 {
   __half = __len >> 1;
   __middle = __first;
   std::advance(__middle, __half);
   if (*__middle < __val)
     {
       __first = __middle;
       ++__first;
       __len = __len - __half - 1;
     }
   else
     __len = __half;
 }
      return __first;
    }

  template<typename _ForwardIterator, typename _Tp, typename _Compare>
    _ForwardIterator
    lower_bound(_ForwardIterator __first, _ForwardIterator __last,
  const _Tp& __val, _Compare __comp)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
 _ValueType;
      typedef typename iterator_traits<_ForwardIterator>::difference_type
 _DistanceType;
     
     

      ;

      _DistanceType __len = std::distance(__first, __last);
      _DistanceType __half;
      _ForwardIterator __middle;

      while (__len > 0)
 {
   __half = __len >> 1;
   __middle = __first;
   std::advance(__middle, __half);
   if (__comp(*__middle, __val))
     {
       __first = __middle;
       ++__first;
       __len = __len - __half - 1;
     }
   else
     __len = __half;
 }
      return __first;
    }

  template<typename _ForwardIterator, typename _Tp>
    _ForwardIterator
    upper_bound(_ForwardIterator __first, _ForwardIterator __last,
  const _Tp& __val)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
 _ValueType;
      typedef typename iterator_traits<_ForwardIterator>::difference_type
 _DistanceType;
     
     
      ;

      _DistanceType __len = std::distance(__first, __last);
      _DistanceType __half;
      _ForwardIterator __middle;

      while (__len > 0)
 {
   __half = __len >> 1;
   __middle = __first;
   std::advance(__middle, __half);
   if (__val < *__middle)
     __len = __half;
   else
     {
       __first = __middle;
       ++__first;
       __len = __len - __half - 1;
     }
 }
      return __first;
    }

  template<typename _ForwardIterator, typename _Tp, typename _Compare>
    _ForwardIterator
    upper_bound(_ForwardIterator __first, _ForwardIterator __last,
  const _Tp& __val, _Compare __comp)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
 _ValueType;
      typedef typename iterator_traits<_ForwardIterator>::difference_type
 _DistanceType;
     
     

      ;

      _DistanceType __len = std::distance(__first, __last);
      _DistanceType __half;
      _ForwardIterator __middle;

      while (__len > 0)
 {
   __half = __len >> 1;
   __middle = __first;
   std::advance(__middle, __half);
   if (__comp(__val, *__middle))
     __len = __half;
   else
     {
       __first = __middle;
       ++__first;
       __len = __len - __half - 1;
     }
 }
      return __first;
    }
  template<typename _BidirectionalIterator, typename _Distance>
    void
    __merge_without_buffer(_BidirectionalIterator __first,
      _BidirectionalIterator __middle,
      _BidirectionalIterator __last,
      _Distance __len1, _Distance __len2)
    {
      if (__len1 == 0 || __len2 == 0)
 return;
      if (__len1 + __len2 == 2)
 {
   if (*__middle < *__first)
     std::iter_swap(__first, __middle);
   return;
 }
      _BidirectionalIterator __first_cut = __first;
      _BidirectionalIterator __second_cut = __middle;
      _Distance __len11 = 0;
      _Distance __len22 = 0;
      if (__len1 > __len2)
 {
   __len11 = __len1 / 2;
   std::advance(__first_cut, __len11);
   __second_cut = std::lower_bound(__middle, __last, *__first_cut);
   __len22 = std::distance(__middle, __second_cut);
 }
      else
 {
   __len22 = __len2 / 2;
   std::advance(__second_cut, __len22);
   __first_cut = std::upper_bound(__first, __middle, *__second_cut);
   __len11 = std::distance(__first, __first_cut);
 }
      std::rotate(__first_cut, __middle, __second_cut);
      _BidirectionalIterator __new_middle = __first_cut;
      std::advance(__new_middle, std::distance(__middle, __second_cut));
      std::__merge_without_buffer(__first, __first_cut, __new_middle,
      __len11, __len22);
      std::__merge_without_buffer(__new_middle, __second_cut, __last,
      __len1 - __len11, __len2 - __len22);
    }
  template<typename _BidirectionalIterator, typename _Distance,
    typename _Compare>
    void
    __merge_without_buffer(_BidirectionalIterator __first,
                           _BidirectionalIterator __middle,
      _BidirectionalIterator __last,
      _Distance __len1, _Distance __len2,
      _Compare __comp)
    {
      if (__len1 == 0 || __len2 == 0)
 return;
      if (__len1 + __len2 == 2)
 {
   if (__comp(*__middle, *__first))
     std::iter_swap(__first, __middle);
   return;
 }
      _BidirectionalIterator __first_cut = __first;
      _BidirectionalIterator __second_cut = __middle;
      _Distance __len11 = 0;
      _Distance __len22 = 0;
      if (__len1 > __len2)
 {
   __len11 = __len1 / 2;
   std::advance(__first_cut, __len11);
   __second_cut = std::lower_bound(__middle, __last, *__first_cut,
       __comp);
   __len22 = std::distance(__middle, __second_cut);
 }
      else
 {
   __len22 = __len2 / 2;
   std::advance(__second_cut, __len22);
   __first_cut = std::upper_bound(__first, __middle, *__second_cut,
      __comp);
   __len11 = std::distance(__first, __first_cut);
 }
      std::rotate(__first_cut, __middle, __second_cut);
      _BidirectionalIterator __new_middle = __first_cut;
      std::advance(__new_middle, std::distance(__middle, __second_cut));
      std::__merge_without_buffer(__first, __first_cut, __new_middle,
      __len11, __len22, __comp);
      std::__merge_without_buffer(__new_middle, __second_cut, __last,
      __len1 - __len11, __len2 - __len22, __comp);
    }
  template<typename _RandomAccessIterator>
    void
    __inplace_stable_sort(_RandomAccessIterator __first,
     _RandomAccessIterator __last)
    {
      if (__last - __first < 15)
 {
   std::__insertion_sort(__first, __last);
   return;
 }
      _RandomAccessIterator __middle = __first + (__last - __first) / 2;
      std::__inplace_stable_sort(__first, __middle);
      std::__inplace_stable_sort(__middle, __last);
      std::__merge_without_buffer(__first, __middle, __last,
      __middle - __first,
      __last - __middle);
    }
  template<typename _RandomAccessIterator, typename _Compare>
    void
    __inplace_stable_sort(_RandomAccessIterator __first,
     _RandomAccessIterator __last, _Compare __comp)
    {
      if (__last - __first < 15)
 {
   std::__insertion_sort(__first, __last, __comp);
   return;
 }
      _RandomAccessIterator __middle = __first + (__last - __first) / 2;
      std::__inplace_stable_sort(__first, __middle, __comp);
      std::__inplace_stable_sort(__middle, __last, __comp);
      std::__merge_without_buffer(__first, __middle, __last,
      __middle - __first,
      __last - __middle,
      __comp);
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _OutputIterator>
    _OutputIterator
    merge(_InputIterator1 __first1, _InputIterator1 __last1,
   _InputIterator2 __first2, _InputIterator2 __last2,
   _OutputIterator __result)
    {
      typedef typename iterator_traits<_InputIterator1>::value_type
 _ValueType1;
      typedef typename iterator_traits<_InputIterator2>::value_type
 _ValueType2;
     
     
     

     

     
      ;
      ;

      while (__first1 != __last1 && __first2 != __last2)
 {
   if (*__first2 < *__first1)
     {
       *__result = *__first2;
       ++__first2;
     }
   else
     {
       *__result = *__first1;
       ++__first1;
     }
   ++__result;
 }
      return std::copy(__first2, __last2, std::copy(__first1, __last1,
          __result));
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _OutputIterator, typename _Compare>
    _OutputIterator
    merge(_InputIterator1 __first1, _InputIterator1 __last1,
   _InputIterator2 __first2, _InputIterator2 __last2,
   _OutputIterator __result, _Compare __comp)
    {
      typedef typename iterator_traits<_InputIterator1>::value_type
 _ValueType1;
      typedef typename iterator_traits<_InputIterator2>::value_type
 _ValueType2;
     
     
     

     

     

      ;
      ;

      while (__first1 != __last1 && __first2 != __last2)
 {
   if (__comp(*__first2, *__first1))
     {
       *__result = *__first2;
       ++__first2;
     }
   else
     {
       *__result = *__first1;
       ++__first1;
     }
   ++__result;
 }
      return std::copy(__first2, __last2, std::copy(__first1, __last1,
          __result));
    }

  template<typename _RandomAccessIterator1, typename _RandomAccessIterator2,
    typename _Distance>
    void
    __merge_sort_loop(_RandomAccessIterator1 __first,
        _RandomAccessIterator1 __last,
        _RandomAccessIterator2 __result,
        _Distance __step_size)
    {
      const _Distance __two_step = 2 * __step_size;

      while (__last - __first >= __two_step)
 {
   __result = std::merge(__first, __first + __step_size,
    __first + __step_size, __first + __two_step,
    __result);
   __first += __two_step;
 }

      __step_size = std::min(_Distance(__last - __first), __step_size);
      std::merge(__first, __first + __step_size, __first + __step_size, __last,
   __result);
    }

  template<typename _RandomAccessIterator1, typename _RandomAccessIterator2,
    typename _Distance, typename _Compare>
    void
    __merge_sort_loop(_RandomAccessIterator1 __first,
        _RandomAccessIterator1 __last,
        _RandomAccessIterator2 __result, _Distance __step_size,
        _Compare __comp)
    {
      const _Distance __two_step = 2 * __step_size;

      while (__last - __first >= __two_step)
 {
   __result = std::merge(__first, __first + __step_size,
    __first + __step_size, __first + __two_step,
    __result,
    __comp);
   __first += __two_step;
 }
      __step_size = std::min(_Distance(__last - __first), __step_size);

      std::merge(__first, __first + __step_size,
   __first + __step_size, __last,
   __result,
   __comp);
    }

  enum { _S_chunk_size = 7 };

  template<typename _RandomAccessIterator, typename _Distance>
    void
    __chunk_insertion_sort(_RandomAccessIterator __first,
      _RandomAccessIterator __last,
      _Distance __chunk_size)
    {
      while (__last - __first >= __chunk_size)
 {
   std::__insertion_sort(__first, __first + __chunk_size);
   __first += __chunk_size;
 }
      std::__insertion_sort(__first, __last);
    }

  template<typename _RandomAccessIterator, typename _Distance, typename _Compare>
    void
    __chunk_insertion_sort(_RandomAccessIterator __first,
      _RandomAccessIterator __last,
      _Distance __chunk_size, _Compare __comp)
    {
      while (__last - __first >= __chunk_size)
 {
   std::__insertion_sort(__first, __first + __chunk_size, __comp);
   __first += __chunk_size;
 }
      std::__insertion_sort(__first, __last, __comp);
    }

  template<typename _RandomAccessIterator, typename _Pointer>
    void
    __merge_sort_with_buffer(_RandomAccessIterator __first,
        _RandomAccessIterator __last,
                             _Pointer __buffer)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type
 _Distance;

      const _Distance __len = __last - __first;
      const _Pointer __buffer_last = __buffer + __len;

      _Distance __step_size = _S_chunk_size;
      std::__chunk_insertion_sort(__first, __last, __step_size);

      while (__step_size < __len)
 {
   std::__merge_sort_loop(__first, __last, __buffer, __step_size);
   __step_size *= 2;
   std::__merge_sort_loop(__buffer, __buffer_last, __first, __step_size);
   __step_size *= 2;
 }
    }

  template<typename _RandomAccessIterator, typename _Pointer, typename _Compare>
    void
    __merge_sort_with_buffer(_RandomAccessIterator __first,
        _RandomAccessIterator __last,
                             _Pointer __buffer, _Compare __comp)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type
 _Distance;

      const _Distance __len = __last - __first;
      const _Pointer __buffer_last = __buffer + __len;

      _Distance __step_size = _S_chunk_size;
      std::__chunk_insertion_sort(__first, __last, __step_size, __comp);

      while (__step_size < __len)
 {
   std::__merge_sort_loop(__first, __last, __buffer,
     __step_size, __comp);
   __step_size *= 2;
   std::__merge_sort_loop(__buffer, __buffer_last, __first,
     __step_size, __comp);
   __step_size *= 2;
 }
    }
  template<typename _BidirectionalIterator1, typename _BidirectionalIterator2,
    typename _BidirectionalIterator3>
    _BidirectionalIterator3
    __merge_backward(_BidirectionalIterator1 __first1,
       _BidirectionalIterator1 __last1,
       _BidirectionalIterator2 __first2,
       _BidirectionalIterator2 __last2,
       _BidirectionalIterator3 __result)
    {
      if (__first1 == __last1)
 return std::copy_backward(__first2, __last2, __result);
      if (__first2 == __last2)
 return std::copy_backward(__first1, __last1, __result);
      --__last1;
      --__last2;
      while (true)
 {
   if (*__last2 < *__last1)
     {
       *--__result = *__last1;
       if (__first1 == __last1)
  return std::copy_backward(__first2, ++__last2, __result);
       --__last1;
     }
   else
     {
       *--__result = *__last2;
       if (__first2 == __last2)
  return std::copy_backward(__first1, ++__last1, __result);
       --__last2;
     }
 }
    }
  template<typename _BidirectionalIterator1, typename _BidirectionalIterator2,
    typename _BidirectionalIterator3, typename _Compare>
    _BidirectionalIterator3
    __merge_backward(_BidirectionalIterator1 __first1,
       _BidirectionalIterator1 __last1,
       _BidirectionalIterator2 __first2,
       _BidirectionalIterator2 __last2,
       _BidirectionalIterator3 __result,
       _Compare __comp)
    {
      if (__first1 == __last1)
 return std::copy_backward(__first2, __last2, __result);
      if (__first2 == __last2)
 return std::copy_backward(__first1, __last1, __result);
      --__last1;
      --__last2;
      while (true)
 {
   if (__comp(*__last2, *__last1))
     {
       *--__result = *__last1;
       if (__first1 == __last1)
  return std::copy_backward(__first2, ++__last2, __result);
       --__last1;
     }
   else
     {
       *--__result = *__last2;
       if (__first2 == __last2)
  return std::copy_backward(__first1, ++__last1, __result);
       --__last2;
     }
 }
    }
  template<typename _BidirectionalIterator1, typename _BidirectionalIterator2,
    typename _Distance>
    _BidirectionalIterator1
    __rotate_adaptive(_BidirectionalIterator1 __first,
        _BidirectionalIterator1 __middle,
        _BidirectionalIterator1 __last,
        _Distance __len1, _Distance __len2,
        _BidirectionalIterator2 __buffer,
        _Distance __buffer_size)
    {
      _BidirectionalIterator2 __buffer_end;
      if (__len1 > __len2 && __len2 <= __buffer_size)
 {
   __buffer_end = std::copy(__middle, __last, __buffer);
   std::copy_backward(__first, __middle, __last);
   return std::copy(__buffer, __buffer_end, __first);
 }
      else if (__len1 <= __buffer_size)
 {
   __buffer_end = std::copy(__first, __middle, __buffer);
   std::copy(__middle, __last, __first);
   return std::copy_backward(__buffer, __buffer_end, __last);
 }
      else
 {
   std::rotate(__first, __middle, __last);
   std::advance(__first, std::distance(__middle, __last));
   return __first;
 }
    }
  template<typename _BidirectionalIterator, typename _Distance,
    typename _Pointer>
    void
    __merge_adaptive(_BidirectionalIterator __first,
                     _BidirectionalIterator __middle,
       _BidirectionalIterator __last,
       _Distance __len1, _Distance __len2,
       _Pointer __buffer, _Distance __buffer_size)
    {
      if (__len1 <= __len2 && __len1 <= __buffer_size)
 {
   _Pointer __buffer_end = std::copy(__first, __middle, __buffer);
   std::merge(__buffer, __buffer_end, __middle, __last, __first);
 }
      else if (__len2 <= __buffer_size)
 {
   _Pointer __buffer_end = std::copy(__middle, __last, __buffer);
   std::__merge_backward(__first, __middle, __buffer,
    __buffer_end, __last);
 }
      else
 {
   _BidirectionalIterator __first_cut = __first;
   _BidirectionalIterator __second_cut = __middle;
   _Distance __len11 = 0;
   _Distance __len22 = 0;
   if (__len1 > __len2)
     {
       __len11 = __len1 / 2;
       std::advance(__first_cut, __len11);
       __second_cut = std::lower_bound(__middle, __last,
           *__first_cut);
       __len22 = std::distance(__middle, __second_cut);
     }
   else
     {
       __len22 = __len2 / 2;
       std::advance(__second_cut, __len22);
       __first_cut = std::upper_bound(__first, __middle,
          *__second_cut);
       __len11 = std::distance(__first, __first_cut);
     }
   _BidirectionalIterator __new_middle =
     std::__rotate_adaptive(__first_cut, __middle, __second_cut,
       __len1 - __len11, __len22, __buffer,
       __buffer_size);
   std::__merge_adaptive(__first, __first_cut, __new_middle, __len11,
    __len22, __buffer, __buffer_size);
   std::__merge_adaptive(__new_middle, __second_cut, __last,
    __len1 - __len11,
    __len2 - __len22, __buffer, __buffer_size);
 }
    }
  template<typename _BidirectionalIterator, typename _Distance, typename _Pointer,
    typename _Compare>
    void
    __merge_adaptive(_BidirectionalIterator __first,
                     _BidirectionalIterator __middle,
       _BidirectionalIterator __last,
       _Distance __len1, _Distance __len2,
       _Pointer __buffer, _Distance __buffer_size,
       _Compare __comp)
    {
      if (__len1 <= __len2 && __len1 <= __buffer_size)
 {
   _Pointer __buffer_end = std::copy(__first, __middle, __buffer);
   std::merge(__buffer, __buffer_end, __middle, __last, __first, __comp);
 }
      else if (__len2 <= __buffer_size)
 {
   _Pointer __buffer_end = std::copy(__middle, __last, __buffer);
   std::__merge_backward(__first, __middle, __buffer, __buffer_end,
    __last, __comp);
 }
      else
 {
   _BidirectionalIterator __first_cut = __first;
   _BidirectionalIterator __second_cut = __middle;
   _Distance __len11 = 0;
   _Distance __len22 = 0;
   if (__len1 > __len2)
     {
       __len11 = __len1 / 2;
       std::advance(__first_cut, __len11);
       __second_cut = std::lower_bound(__middle, __last, *__first_cut,
           __comp);
       __len22 = std::distance(__middle, __second_cut);
     }
   else
     {
       __len22 = __len2 / 2;
       std::advance(__second_cut, __len22);
       __first_cut = std::upper_bound(__first, __middle, *__second_cut,
          __comp);
       __len11 = std::distance(__first, __first_cut);
     }
   _BidirectionalIterator __new_middle =
     std::__rotate_adaptive(__first_cut, __middle, __second_cut,
       __len1 - __len11, __len22, __buffer,
       __buffer_size);
   std::__merge_adaptive(__first, __first_cut, __new_middle, __len11,
    __len22, __buffer, __buffer_size, __comp);
   std::__merge_adaptive(__new_middle, __second_cut, __last,
    __len1 - __len11,
    __len2 - __len22, __buffer,
    __buffer_size, __comp);
 }
    }

  template<typename _BidirectionalIterator>
    void
    inplace_merge(_BidirectionalIterator __first,
    _BidirectionalIterator __middle,
    _BidirectionalIterator __last)
    {
      typedef typename iterator_traits<_BidirectionalIterator>::value_type
          _ValueType;
      typedef typename iterator_traits<_BidirectionalIterator>::difference_type
          _DistanceType;
     

     
      ;
      ;

      if (__first == __middle || __middle == __last)
 return;

      _DistanceType __len1 = std::distance(__first, __middle);
      _DistanceType __len2 = std::distance(__middle, __last);

      _Temporary_buffer<_BidirectionalIterator, _ValueType> __buf(__first,
          __last);
      if (__buf.begin() == 0)
 std::__merge_without_buffer(__first, __middle, __last, __len1, __len2);
      else
 std::__merge_adaptive(__first, __middle, __last, __len1, __len2,
         __buf.begin(), _DistanceType(__buf.size()));
    }

  template<typename _BidirectionalIterator, typename _Compare>
    void
    inplace_merge(_BidirectionalIterator __first,
    _BidirectionalIterator __middle,
    _BidirectionalIterator __last,
    _Compare __comp)
    {
      typedef typename iterator_traits<_BidirectionalIterator>::value_type
          _ValueType;
      typedef typename iterator_traits<_BidirectionalIterator>::difference_type
          _DistanceType;
     

     

      ;
      ;

      if (__first == __middle || __middle == __last)
 return;

      const _DistanceType __len1 = std::distance(__first, __middle);
      const _DistanceType __len2 = std::distance(__middle, __last);

      _Temporary_buffer<_BidirectionalIterator, _ValueType> __buf(__first,
          __last);
      if (__buf.begin() == 0)
 std::__merge_without_buffer(__first, __middle, __last, __len1,
        __len2, __comp);
      else
 std::__merge_adaptive(__first, __middle, __last, __len1, __len2,
         __buf.begin(), _DistanceType(__buf.size()),
         __comp);
    }

  template<typename _RandomAccessIterator, typename _Pointer,
    typename _Distance>
    void
    __stable_sort_adaptive(_RandomAccessIterator __first,
      _RandomAccessIterator __last,
                           _Pointer __buffer, _Distance __buffer_size)
    {
      const _Distance __len = (__last - __first + 1) / 2;
      const _RandomAccessIterator __middle = __first + __len;
      if (__len > __buffer_size)
 {
   std::__stable_sort_adaptive(__first, __middle,
          __buffer, __buffer_size);
   std::__stable_sort_adaptive(__middle, __last,
          __buffer, __buffer_size);
 }
      else
 {
   std::__merge_sort_with_buffer(__first, __middle, __buffer);
   std::__merge_sort_with_buffer(__middle, __last, __buffer);
 }
      std::__merge_adaptive(__first, __middle, __last,
       _Distance(__middle - __first),
       _Distance(__last - __middle),
       __buffer, __buffer_size);
    }

  template<typename _RandomAccessIterator, typename _Pointer,
    typename _Distance, typename _Compare>
    void
    __stable_sort_adaptive(_RandomAccessIterator __first,
      _RandomAccessIterator __last,
                           _Pointer __buffer, _Distance __buffer_size,
                           _Compare __comp)
    {
      const _Distance __len = (__last - __first + 1) / 2;
      const _RandomAccessIterator __middle = __first + __len;
      if (__len > __buffer_size)
 {
   std::__stable_sort_adaptive(__first, __middle, __buffer,
          __buffer_size, __comp);
   std::__stable_sort_adaptive(__middle, __last, __buffer,
          __buffer_size, __comp);
 }
      else
 {
   std::__merge_sort_with_buffer(__first, __middle, __buffer, __comp);
   std::__merge_sort_with_buffer(__middle, __last, __buffer, __comp);
 }
      std::__merge_adaptive(__first, __middle, __last,
       _Distance(__middle - __first),
       _Distance(__last - __middle),
       __buffer, __buffer_size,
       __comp);
    }

  template<typename _RandomAccessIterator>
    inline void
    stable_sort(_RandomAccessIterator __first, _RandomAccessIterator __last)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type
 _DistanceType;
     

     
      ;

      _Temporary_buffer<_RandomAccessIterator, _ValueType> __buf(__first,
         __last);
      if (__buf.begin() == 0)
 std::__inplace_stable_sort(__first, __last);
      else
 std::__stable_sort_adaptive(__first, __last, __buf.begin(),
        _DistanceType(__buf.size()));
    }

  template<typename _RandomAccessIterator, typename _Compare>
    inline void
    stable_sort(_RandomAccessIterator __first, _RandomAccessIterator __last,
  _Compare __comp)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;
      typedef typename iterator_traits<_RandomAccessIterator>::difference_type
 _DistanceType;
     

     
      ;

      _Temporary_buffer<_RandomAccessIterator, _ValueType> __buf(__first,
         __last);
      if (__buf.begin() == 0)
 std::__inplace_stable_sort(__first, __last, __comp);
      else
 std::__stable_sort_adaptive(__first, __last, __buf.begin(),
        _DistanceType(__buf.size()), __comp);
    }
  template<typename _RandomAccessIterator, typename _Size>
    void
    __introselect(_RandomAccessIterator __first, _RandomAccessIterator __nth,
    _RandomAccessIterator __last, _Size __depth_limit)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;

      while (__last - __first > 3)
 {
   if (__depth_limit == 0)
     {
       std::__heap_select(__first, __nth + 1, __last);

       std::iter_swap(__first, __nth);
       return;
     }
   --__depth_limit;
   _RandomAccessIterator __cut =
     std::__unguarded_partition(__first, __last,
           _ValueType(std::__median(*__first,
        *(__first
          + (__last
             - __first)
          / 2),
        *(__last
          - 1))));
   if (__cut <= __nth)
     __first = __cut;
   else
     __last = __cut;
 }
      std::__insertion_sort(__first, __last);
    }

  template<typename _RandomAccessIterator, typename _Size, typename _Compare>
    void
    __introselect(_RandomAccessIterator __first, _RandomAccessIterator __nth,
    _RandomAccessIterator __last, _Size __depth_limit,
    _Compare __comp)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;

      while (__last - __first > 3)
 {
   if (__depth_limit == 0)
     {
       std::__heap_select(__first, __nth + 1, __last, __comp);

       std::iter_swap(__first, __nth);
       return;
     }
   --__depth_limit;
   _RandomAccessIterator __cut =
     std::__unguarded_partition(__first, __last,
           _ValueType(std::__median(*__first,
        *(__first
          + (__last
             - __first)
          / 2),
        *(__last - 1),
        __comp)),
           __comp);
   if (__cut <= __nth)
     __first = __cut;
   else
     __last = __cut;
 }
      std::__insertion_sort(__first, __last, __comp);
    }

  template<typename _RandomAccessIterator>
    inline void
    nth_element(_RandomAccessIterator __first, _RandomAccessIterator __nth,
  _RandomAccessIterator __last)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;
     

     
      ;
      ;

      if (__first == __last || __nth == __last)
 return;

      std::__introselect(__first, __nth, __last,
    std::__lg(__last - __first) * 2);
    }

  template<typename _RandomAccessIterator, typename _Compare>
    inline void
    nth_element(_RandomAccessIterator __first, _RandomAccessIterator __nth,
  _RandomAccessIterator __last, _Compare __comp)
    {
      typedef typename iterator_traits<_RandomAccessIterator>::value_type
 _ValueType;
     

     

      ;
      ;

      if (__first == __last || __nth == __last)
 return;

      std::__introselect(__first, __nth, __last,
    std::__lg(__last - __first) * 2, __comp);
    }

  template<typename _ForwardIterator, typename _Tp>
    pair<_ForwardIterator, _ForwardIterator>
    equal_range(_ForwardIterator __first, _ForwardIterator __last,
  const _Tp& __val)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
 _ValueType;
      typedef typename iterator_traits<_ForwardIterator>::difference_type
 _DistanceType;
     
     
     
      ;

      _DistanceType __len = std::distance(__first, __last);
      _DistanceType __half;
      _ForwardIterator __middle, __left, __right;

      while (__len > 0)
 {
   __half = __len >> 1;
   __middle = __first;
   std::advance(__middle, __half);
   if (*__middle < __val)
     {
       __first = __middle;
       ++__first;
       __len = __len - __half - 1;
     }
   else if (__val < *__middle)
     __len = __half;
   else
     {
       __left = std::lower_bound(__first, __middle, __val);
       std::advance(__first, __len);
       __right = std::upper_bound(++__middle, __first, __val);
       return pair<_ForwardIterator, _ForwardIterator>(__left, __right);
     }
 }
      return pair<_ForwardIterator, _ForwardIterator>(__first, __first);
    }

  template<typename _ForwardIterator, typename _Tp, typename _Compare>
    pair<_ForwardIterator, _ForwardIterator>
    equal_range(_ForwardIterator __first, _ForwardIterator __last,
  const _Tp& __val,
  _Compare __comp)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
 _ValueType;
      typedef typename iterator_traits<_ForwardIterator>::difference_type
 _DistanceType;
     
     

     

      ;

      _DistanceType __len = std::distance(__first, __last);
      _DistanceType __half;
      _ForwardIterator __middle, __left, __right;

      while (__len > 0)
 {
   __half = __len >> 1;
   __middle = __first;
   std::advance(__middle, __half);
   if (__comp(*__middle, __val))
     {
       __first = __middle;
       ++__first;
       __len = __len - __half - 1;
     }
   else if (__comp(__val, *__middle))
     __len = __half;
   else
     {
       __left = std::lower_bound(__first, __middle, __val, __comp);
       std::advance(__first, __len);
       __right = std::upper_bound(++__middle, __first, __val, __comp);
       return pair<_ForwardIterator, _ForwardIterator>(__left, __right);
     }
 }
      return pair<_ForwardIterator, _ForwardIterator>(__first, __first);
    }

  template<typename _ForwardIterator, typename _Tp>
    bool
    binary_search(_ForwardIterator __first, _ForwardIterator __last,
                  const _Tp& __val)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
 _ValueType;
     
     
      ;

      _ForwardIterator __i = std::lower_bound(__first, __last, __val);
      return __i != __last && !(__val < *__i);
    }

  template<typename _ForwardIterator, typename _Tp, typename _Compare>
    bool
    binary_search(_ForwardIterator __first, _ForwardIterator __last,
                  const _Tp& __val, _Compare __comp)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
 _ValueType;
     
     

      ;

      _ForwardIterator __i = std::lower_bound(__first, __last, __val, __comp);
      return __i != __last && !__comp(__val, *__i);
    }

  template<typename _InputIterator1, typename _InputIterator2>
    bool
    includes(_InputIterator1 __first1, _InputIterator1 __last1,
      _InputIterator2 __first2, _InputIterator2 __last2)
    {
      typedef typename iterator_traits<_InputIterator1>::value_type
 _ValueType1;
      typedef typename iterator_traits<_InputIterator2>::value_type
 _ValueType2;
     
     
     
     
      ;
      ;

      while (__first1 != __last1 && __first2 != __last2)
 if (*__first2 < *__first1)
   return false;
 else if(*__first1 < *__first2)
   ++__first1;
 else
   ++__first1, ++__first2;

      return __first2 == __last2;
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _Compare>
    bool
    includes(_InputIterator1 __first1, _InputIterator1 __last1,
      _InputIterator2 __first2, _InputIterator2 __last2, _Compare __comp)
    {
      typedef typename iterator_traits<_InputIterator1>::value_type
 _ValueType1;
      typedef typename iterator_traits<_InputIterator2>::value_type
 _ValueType2;
     
     
     

     

      ;
      ;

      while (__first1 != __last1 && __first2 != __last2)
 if (__comp(*__first2, *__first1))
   return false;
 else if(__comp(*__first1, *__first2))
   ++__first1;
 else
   ++__first1, ++__first2;

      return __first2 == __last2;
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _OutputIterator>
    _OutputIterator
    set_union(_InputIterator1 __first1, _InputIterator1 __last1,
       _InputIterator2 __first2, _InputIterator2 __last2,
       _OutputIterator __result)
    {
      typedef typename iterator_traits<_InputIterator1>::value_type
 _ValueType1;
      typedef typename iterator_traits<_InputIterator2>::value_type
 _ValueType2;
     
     
     

     

     
     
      ;
      ;

      while (__first1 != __last1 && __first2 != __last2)
 {
   if (*__first1 < *__first2)
     {
       *__result = *__first1;
       ++__first1;
     }
   else if (*__first2 < *__first1)
     {
       *__result = *__first2;
       ++__first2;
     }
   else
     {
       *__result = *__first1;
       ++__first1;
       ++__first2;
     }
   ++__result;
 }
      return std::copy(__first2, __last2, std::copy(__first1, __last1,
          __result));
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _OutputIterator, typename _Compare>
    _OutputIterator
    set_union(_InputIterator1 __first1, _InputIterator1 __last1,
       _InputIterator2 __first2, _InputIterator2 __last2,
       _OutputIterator __result, _Compare __comp)
    {
      typedef typename iterator_traits<_InputIterator1>::value_type
 _ValueType1;
      typedef typename iterator_traits<_InputIterator2>::value_type
 _ValueType2;
     
     
     

     

     

     

      ;
      ;

      while (__first1 != __last1 && __first2 != __last2)
 {
   if (__comp(*__first1, *__first2))
     {
       *__result = *__first1;
       ++__first1;
     }
   else if (__comp(*__first2, *__first1))
     {
       *__result = *__first2;
       ++__first2;
     }
   else
     {
       *__result = *__first1;
       ++__first1;
       ++__first2;
     }
   ++__result;
 }
      return std::copy(__first2, __last2, std::copy(__first1, __last1,
          __result));
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _OutputIterator>
    _OutputIterator
    set_intersection(_InputIterator1 __first1, _InputIterator1 __last1,
       _InputIterator2 __first2, _InputIterator2 __last2,
       _OutputIterator __result)
    {
      typedef typename iterator_traits<_InputIterator1>::value_type
 _ValueType1;
      typedef typename iterator_traits<_InputIterator2>::value_type
 _ValueType2;
     
     
     

     
     
      ;
      ;

      while (__first1 != __last1 && __first2 != __last2)
 if (*__first1 < *__first2)
   ++__first1;
 else if (*__first2 < *__first1)
   ++__first2;
 else
   {
     *__result = *__first1;
     ++__first1;
     ++__first2;
     ++__result;
   }
      return __result;
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _OutputIterator, typename _Compare>
    _OutputIterator
    set_intersection(_InputIterator1 __first1, _InputIterator1 __last1,
       _InputIterator2 __first2, _InputIterator2 __last2,
       _OutputIterator __result, _Compare __comp)
    {
      typedef typename iterator_traits<_InputIterator1>::value_type
 _ValueType1;
      typedef typename iterator_traits<_InputIterator2>::value_type
 _ValueType2;
     
     
     

     

     

      ;
      ;

      while (__first1 != __last1 && __first2 != __last2)
 if (__comp(*__first1, *__first2))
   ++__first1;
 else if (__comp(*__first2, *__first1))
   ++__first2;
 else
   {
     *__result = *__first1;
     ++__first1;
     ++__first2;
     ++__result;
   }
      return __result;
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _OutputIterator>
    _OutputIterator
    set_difference(_InputIterator1 __first1, _InputIterator1 __last1,
     _InputIterator2 __first2, _InputIterator2 __last2,
     _OutputIterator __result)
    {
      typedef typename iterator_traits<_InputIterator1>::value_type
 _ValueType1;
      typedef typename iterator_traits<_InputIterator2>::value_type
 _ValueType2;
     
     
     

     
     
      ;
      ;

      while (__first1 != __last1 && __first2 != __last2)
 if (*__first1 < *__first2)
   {
     *__result = *__first1;
     ++__first1;
     ++__result;
   }
 else if (*__first2 < *__first1)
   ++__first2;
 else
   {
     ++__first1;
     ++__first2;
   }
      return std::copy(__first1, __last1, __result);
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _OutputIterator, typename _Compare>
    _OutputIterator
    set_difference(_InputIterator1 __first1, _InputIterator1 __last1,
     _InputIterator2 __first2, _InputIterator2 __last2,
     _OutputIterator __result, _Compare __comp)
    {
      typedef typename iterator_traits<_InputIterator1>::value_type
 _ValueType1;
      typedef typename iterator_traits<_InputIterator2>::value_type
 _ValueType2;
     
     
     

     

     

      ;
      ;

      while (__first1 != __last1 && __first2 != __last2)
 if (__comp(*__first1, *__first2))
   {
     *__result = *__first1;
     ++__first1;
     ++__result;
   }
 else if (__comp(*__first2, *__first1))
   ++__first2;
 else
   {
     ++__first1;
     ++__first2;
   }
      return std::copy(__first1, __last1, __result);
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _OutputIterator>
    _OutputIterator
    set_symmetric_difference(_InputIterator1 __first1, _InputIterator1 __last1,
        _InputIterator2 __first2, _InputIterator2 __last2,
        _OutputIterator __result)
    {
      typedef typename iterator_traits<_InputIterator1>::value_type
 _ValueType1;
      typedef typename iterator_traits<_InputIterator2>::value_type
 _ValueType2;
     
     
     

     

     
     
      ;
      ;

      while (__first1 != __last1 && __first2 != __last2)
 if (*__first1 < *__first2)
   {
     *__result = *__first1;
     ++__first1;
     ++__result;
   }
 else if (*__first2 < *__first1)
   {
     *__result = *__first2;
     ++__first2;
     ++__result;
   }
 else
   {
     ++__first1;
     ++__first2;
   }
      return std::copy(__first2, __last2, std::copy(__first1,
          __last1, __result));
    }

  template<typename _InputIterator1, typename _InputIterator2,
    typename _OutputIterator, typename _Compare>
    _OutputIterator
    set_symmetric_difference(_InputIterator1 __first1, _InputIterator1 __last1,
        _InputIterator2 __first2, _InputIterator2 __last2,
        _OutputIterator __result,
        _Compare __comp)
    {
      typedef typename iterator_traits<_InputIterator1>::value_type
 _ValueType1;
      typedef typename iterator_traits<_InputIterator2>::value_type
 _ValueType2;
     
     
     

     

     

     

      ;
      ;

      while (__first1 != __last1 && __first2 != __last2)
 if (__comp(*__first1, *__first2))
   {
     *__result = *__first1;
     ++__first1;
     ++__result;
   }
 else if (__comp(*__first2, *__first1))
   {
     *__result = *__first2;
     ++__first2;
     ++__result;
   }
 else
   {
     ++__first1;
     ++__first2;
   }
      return std::copy(__first2, __last2, std::copy(__first1,
          __last1, __result));
    }

  template<typename _ForwardIterator>
    _ForwardIterator
    max_element(_ForwardIterator __first, _ForwardIterator __last)
    {

     
     

      ;

      if (__first == __last)
 return __first;
      _ForwardIterator __result = __first;
      while (++__first != __last)
 if (*__result < *__first)
   __result = __first;
      return __result;
    }

  template<typename _ForwardIterator, typename _Compare>
    _ForwardIterator
    max_element(_ForwardIterator __first, _ForwardIterator __last,
  _Compare __comp)
    {

     
     
      ;

      if (__first == __last) return __first;
      _ForwardIterator __result = __first;
      while (++__first != __last)
 if (__comp(*__result, *__first)) __result = __first;
      return __result;
    }

  template<typename _ForwardIterator>
    _ForwardIterator
    min_element(_ForwardIterator __first, _ForwardIterator __last)
    {

     
     

      ;

      if (__first == __last)
 return __first;
      _ForwardIterator __result = __first;
      while (++__first != __last)
 if (*__first < *__result)
   __result = __first;
      return __result;
    }

  template<typename _ForwardIterator, typename _Compare>
    _ForwardIterator
    min_element(_ForwardIterator __first, _ForwardIterator __last,
  _Compare __comp)
    {

     
     
      ;

      if (__first == __last)
 return __first;
      _ForwardIterator __result = __first;
      while (++__first != __last)
 if (__comp(*__first, *__result))
   __result = __first;
      return __result;
    }

  template<typename _BidirectionalIterator>
    bool
    next_permutation(_BidirectionalIterator __first,
       _BidirectionalIterator __last)
    {

     

     

      ;

      if (__first == __last)
 return false;
      _BidirectionalIterator __i = __first;
      ++__i;
      if (__i == __last)
 return false;
      __i = __last;
      --__i;

      for(;;)
 {
   _BidirectionalIterator __ii = __i;
   --__i;
   if (*__i < *__ii)
     {
       _BidirectionalIterator __j = __last;
       while (!(*__i < *--__j))
  {}
       std::iter_swap(__i, __j);
       std::reverse(__ii, __last);
       return true;
     }
   if (__i == __first)
     {
       std::reverse(__first, __last);
       return false;
     }
 }
    }

  template<typename _BidirectionalIterator, typename _Compare>
    bool
    next_permutation(_BidirectionalIterator __first,
       _BidirectionalIterator __last, _Compare __comp)
    {

     

     
      ;

      if (__first == __last)
 return false;
      _BidirectionalIterator __i = __first;
      ++__i;
      if (__i == __last)
 return false;
      __i = __last;
      --__i;

      for(;;)
 {
   _BidirectionalIterator __ii = __i;
   --__i;
   if (__comp(*__i, *__ii))
     {
       _BidirectionalIterator __j = __last;
       while (!__comp(*__i, *--__j))
  {}
       std::iter_swap(__i, __j);
       std::reverse(__ii, __last);
       return true;
     }
   if (__i == __first)
     {
       std::reverse(__first, __last);
       return false;
     }
 }
    }

  template<typename _BidirectionalIterator>
    bool
    prev_permutation(_BidirectionalIterator __first,
       _BidirectionalIterator __last)
    {

     

     

      ;

      if (__first == __last)
 return false;
      _BidirectionalIterator __i = __first;
      ++__i;
      if (__i == __last)
 return false;
      __i = __last;
      --__i;

      for(;;)
 {
   _BidirectionalIterator __ii = __i;
   --__i;
   if (*__ii < *__i)
     {
       _BidirectionalIterator __j = __last;
       while (!(*--__j < *__i))
  {}
       std::iter_swap(__i, __j);
       std::reverse(__ii, __last);
       return true;
     }
   if (__i == __first)
     {
       std::reverse(__first, __last);
       return false;
     }
 }
    }

  template<typename _BidirectionalIterator, typename _Compare>
    bool
    prev_permutation(_BidirectionalIterator __first,
       _BidirectionalIterator __last, _Compare __comp)
    {

     

     
      ;

      if (__first == __last)
 return false;
      _BidirectionalIterator __i = __first;
      ++__i;
      if (__i == __last)
 return false;
      __i = __last;
      --__i;

      for(;;)
 {
   _BidirectionalIterator __ii = __i;
   --__i;
   if (__comp(*__ii, *__i))
     {
       _BidirectionalIterator __j = __last;
       while (!__comp(*--__j, *__i))
  {}
       std::iter_swap(__i, __j);
       std::reverse(__ii, __last);
       return true;
     }
   if (__i == __first)
     {
       std::reverse(__first, __last);
       return false;
     }
 }
    }

  template<typename _InputIterator, typename _ForwardIterator>
    _InputIterator
    find_first_of(_InputIterator __first1, _InputIterator __last1,
    _ForwardIterator __first2, _ForwardIterator __last2)
    {

     
     
     
      ;
      ;

      for ( ; __first1 != __last1; ++__first1)
 for (_ForwardIterator __iter = __first2; __iter != __last2; ++__iter)
   if (*__first1 == *__iter)
     return __first1;
      return __last1;
    }

  template<typename _InputIterator, typename _ForwardIterator,
    typename _BinaryPredicate>
    _InputIterator
    find_first_of(_InputIterator __first1, _InputIterator __last1,
    _ForwardIterator __first2, _ForwardIterator __last2,
    _BinaryPredicate __comp)
    {

     
     
     
      ;
      ;

      for ( ; __first1 != __last1; ++__first1)
 for (_ForwardIterator __iter = __first2; __iter != __last2; ++__iter)
   if (__comp(*__first1, *__iter))
     return __first1;
      return __last1;
    }

  template<typename _ForwardIterator1, typename _ForwardIterator2>
    _ForwardIterator1
    __find_end(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
        _ForwardIterator2 __first2, _ForwardIterator2 __last2,
        forward_iterator_tag, forward_iterator_tag)
    {
      if (__first2 == __last2)
 return __last1;
      else
 {
   _ForwardIterator1 __result = __last1;
   while (1)
     {
       _ForwardIterator1 __new_result
  = std::search(__first1, __last1, __first2, __last2);
       if (__new_result == __last1)
  return __result;
       else
  {
    __result = __new_result;
    __first1 = __new_result;
    ++__first1;
  }
     }
 }
    }

  template<typename _ForwardIterator1, typename _ForwardIterator2,
    typename _BinaryPredicate>
    _ForwardIterator1
    __find_end(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
        _ForwardIterator2 __first2, _ForwardIterator2 __last2,
        forward_iterator_tag, forward_iterator_tag,
        _BinaryPredicate __comp)
    {
      if (__first2 == __last2)
 return __last1;
      else
 {
   _ForwardIterator1 __result = __last1;
   while (1)
     {
       _ForwardIterator1 __new_result
  = std::search(__first1, __last1, __first2, __last2, __comp);
       if (__new_result == __last1)
  return __result;
       else
  {
    __result = __new_result;
    __first1 = __new_result;
    ++__first1;
  }
     }
 }
    }
  template<typename _BidirectionalIterator1, typename _BidirectionalIterator2>
    _BidirectionalIterator1
    __find_end(_BidirectionalIterator1 __first1,
        _BidirectionalIterator1 __last1,
        _BidirectionalIterator2 __first2,
        _BidirectionalIterator2 __last2,
        bidirectional_iterator_tag, bidirectional_iterator_tag)
    {

     

     
      typedef reverse_iterator<_BidirectionalIterator1> _RevIterator1;
      typedef reverse_iterator<_BidirectionalIterator2> _RevIterator2;

      _RevIterator1 __rlast1(__first1);
      _RevIterator2 __rlast2(__first2);
      _RevIterator1 __rresult = std::search(_RevIterator1(__last1), __rlast1,
         _RevIterator2(__last2), __rlast2);

      if (__rresult == __rlast1)
 return __last1;
      else
 {
   _BidirectionalIterator1 __result = __rresult.base();
   std::advance(__result, -std::distance(__first2, __last2));
   return __result;
 }
    }

  template<typename _BidirectionalIterator1, typename _BidirectionalIterator2,
    typename _BinaryPredicate>
    _BidirectionalIterator1
    __find_end(_BidirectionalIterator1 __first1,
        _BidirectionalIterator1 __last1,
        _BidirectionalIterator2 __first2,
        _BidirectionalIterator2 __last2,
        bidirectional_iterator_tag, bidirectional_iterator_tag,
        _BinaryPredicate __comp)
    {

     

     
      typedef reverse_iterator<_BidirectionalIterator1> _RevIterator1;
      typedef reverse_iterator<_BidirectionalIterator2> _RevIterator2;

      _RevIterator1 __rlast1(__first1);
      _RevIterator2 __rlast2(__first2);
      _RevIterator1 __rresult = std::search(_RevIterator1(__last1), __rlast1,
         _RevIterator2(__last2), __rlast2,
         __comp);

      if (__rresult == __rlast1)
 return __last1;
      else
 {
   _BidirectionalIterator1 __result = __rresult.base();
   std::advance(__result, -std::distance(__first2, __last2));
   return __result;
 }
    }

  template<typename _ForwardIterator1, typename _ForwardIterator2>
    inline _ForwardIterator1
    find_end(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
      _ForwardIterator2 __first2, _ForwardIterator2 __last2)
    {

     
     
     
      ;
      ;

      return std::__find_end(__first1, __last1, __first2, __last2,
        std::__iterator_category(__first1),
        std::__iterator_category(__first2));
    }

  template<typename _ForwardIterator1, typename _ForwardIterator2,
    typename _BinaryPredicate>
    inline _ForwardIterator1
    find_end(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
      _ForwardIterator2 __first2, _ForwardIterator2 __last2,
      _BinaryPredicate __comp)
    {

     
     
     
      ;
      ;

      return std::__find_end(__first1, __last1, __first2, __last2,
        std::__iterator_category(__first1),
        std::__iterator_category(__first2),
        __comp);
    }

}
       
namespace std {

  template<typename _Type>
    inline bool
    __is_null_pointer(_Type* __ptr)
    { return __ptr == 0; }

  template<typename _Type>
    inline bool
    __is_null_pointer(_Type)
    { return false; }

  template<typename _CharT, typename _Traits, typename _Alloc>
    const typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::
    _Rep::_S_max_size = (((npos - sizeof(_Rep_base))/sizeof(_CharT)) - 1) / 4;

  template<typename _CharT, typename _Traits, typename _Alloc>
    const _CharT
    basic_string<_CharT, _Traits, _Alloc>::
    _Rep::_S_terminal = _CharT();

  template<typename _CharT, typename _Traits, typename _Alloc>
    const typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::npos;

  template<typename _CharT, typename _Traits, typename _Alloc>
    typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::_Rep::_S_empty_rep_storage[
    (sizeof(_Rep_base) + sizeof(_CharT) + sizeof(size_type) - 1) /
      sizeof(size_type)];

  template<typename _CharT, typename _Traits, typename _Alloc>
    template<typename _InIterator>
      _CharT*
      basic_string<_CharT, _Traits, _Alloc>::
      _S_construct(_InIterator __beg, _InIterator __end, const _Alloc& __a,
     input_iterator_tag)
      {

 if (__beg == __end && __a == _Alloc())
   return _S_empty_rep()._M_refdata();
 _CharT __buf[128];
 size_type __len = 0;
 while (__beg != __end && __len < sizeof(__buf) / sizeof(_CharT))
   {
     __buf[__len++] = *__beg;
     ++__beg;
   }
 _Rep* __r = _Rep::_S_create(__len, size_type(0), __a);
 _M_copy(__r->_M_refdata(), __buf, __len);
 try
   {
     while (__beg != __end)
       {
  if (__len == __r->_M_capacity)
    {

      _Rep* __another = _Rep::_S_create(__len + 1, __len, __a);
      _M_copy(__another->_M_refdata(), __r->_M_refdata(), __len);
      __r->_M_destroy(__a);
      __r = __another;
    }
  __r->_M_refdata()[__len++] = *__beg;
  ++__beg;
       }
   }
 catch(...)
   {
     __r->_M_destroy(__a);
     throw;
   }
 __r->_M_set_length_and_sharable(__len);
 return __r->_M_refdata();
      }

  template<typename _CharT, typename _Traits, typename _Alloc>
    template <typename _InIterator>
      _CharT*
      basic_string<_CharT, _Traits, _Alloc>::
      _S_construct(_InIterator __beg, _InIterator __end, const _Alloc& __a,
     forward_iterator_tag)
      {

 if (__beg == __end && __a == _Alloc())
   return _S_empty_rep()._M_refdata();
 if (__builtin_expect(__is_null_pointer(__beg) && __beg != __end, 0))
   __throw_logic_error(("basic_string::_S_construct NULL not valid"));

 const size_type __dnew = static_cast<size_type>(std::distance(__beg,
              __end));

 _Rep* __r = _Rep::_S_create(__dnew, size_type(0), __a);
 try
   { _S_copy_chars(__r->_M_refdata(), __beg, __end); }
 catch(...)
   {
     __r->_M_destroy(__a);
     throw;
   }
 __r->_M_set_length_and_sharable(__dnew);
 return __r->_M_refdata();
      }

  template<typename _CharT, typename _Traits, typename _Alloc>
    _CharT*
    basic_string<_CharT, _Traits, _Alloc>::
    _S_construct(size_type __n, _CharT __c, const _Alloc& __a)
    {

      if (__n == 0 && __a == _Alloc())
 return _S_empty_rep()._M_refdata();
      _Rep* __r = _Rep::_S_create(__n, size_type(0), __a);
      if (__n)
 _M_assign(__r->_M_refdata(), __n, __c);

      __r->_M_set_length_and_sharable(__n);
      return __r->_M_refdata();
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>::
    basic_string(const basic_string& __str)
    : _M_dataplus(__str._M_rep()->_M_grab(_Alloc(__str.get_allocator()),
       __str.get_allocator()),
    __str.get_allocator())
    { }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>::
    basic_string(const _Alloc& __a)
    : _M_dataplus(_S_construct(size_type(), _CharT(), __a), __a)
    { }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>::
    basic_string(const basic_string& __str, size_type __pos, size_type __n)
    : _M_dataplus(_S_construct(__str._M_data()
          + __str._M_check(__pos,
      "basic_string::basic_string"),
          __str._M_data() + __str._M_limit(__pos, __n)
          + __pos, _Alloc()), _Alloc())
    { }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>::
    basic_string(const basic_string& __str, size_type __pos,
   size_type __n, const _Alloc& __a)
    : _M_dataplus(_S_construct(__str._M_data()
          + __str._M_check(__pos,
      "basic_string::basic_string"),
          __str._M_data() + __str._M_limit(__pos, __n)
          + __pos, __a), __a)
    { }
  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>::
    basic_string(const _CharT* __s, size_type __n, const _Alloc& __a)
    : _M_dataplus(_S_construct(__s, __s + __n, __a), __a)
    { }
  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>::
    basic_string(const _CharT* __s, const _Alloc& __a)
    : _M_dataplus(_S_construct(__s, __s ? __s + traits_type::length(__s) :
          __s + npos, __a), __a)
    { }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>::
    basic_string(size_type __n, _CharT __c, const _Alloc& __a)
    : _M_dataplus(_S_construct(__n, __c, __a), __a)
    { }
  template<typename _CharT, typename _Traits, typename _Alloc>
    template<typename _InputIterator>
    basic_string<_CharT, _Traits, _Alloc>::
    basic_string(_InputIterator __beg, _InputIterator __end, const _Alloc& __a)
    : _M_dataplus(_S_construct(__beg, __end, __a), __a)
    { }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>&
    basic_string<_CharT, _Traits, _Alloc>::
    assign(const basic_string& __str)
    {
      if (_M_rep() != __str._M_rep())
 {

   const allocator_type __a = this->get_allocator();
   _CharT* __tmp = __str._M_rep()->_M_grab(__a, __str.get_allocator());
   _M_rep()->_M_dispose(__a);
   _M_data(__tmp);
 }
      return *this;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>&
    basic_string<_CharT, _Traits, _Alloc>::
    assign(const _CharT* __s, size_type __n)
    {
      ;
      _M_check_length(this->size(), __n, "basic_string::assign");
      if (_M_disjunct(__s) || _M_rep()->_M_is_shared())
 return _M_replace_safe(size_type(0), this->size(), __s, __n);
      else
 {

   const size_type __pos = __s - _M_data();
   if (__pos >= __n)
     _M_copy(_M_data(), __s, __n);
   else if (__pos)
     _M_move(_M_data(), __s, __n);
   _M_rep()->_M_set_length_and_sharable(__n);
   return *this;
 }
     }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>&
    basic_string<_CharT, _Traits, _Alloc>::
    append(size_type __n, _CharT __c)
    {
      if (__n)
 {
   _M_check_length(size_type(0), __n, "basic_string::append");
   const size_type __len = __n + this->size();
   if (__len > this->capacity() || _M_rep()->_M_is_shared())
     this->reserve(__len);
   _M_assign(_M_data() + this->size(), __n, __c);
   _M_rep()->_M_set_length_and_sharable(__len);
 }
      return *this;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>&
    basic_string<_CharT, _Traits, _Alloc>::
    append(const _CharT* __s, size_type __n)
    {
      ;
      if (__n)
 {
   _M_check_length(size_type(0), __n, "basic_string::append");
   const size_type __len = __n + this->size();
   if (__len > this->capacity() || _M_rep()->_M_is_shared())
     {
       if (_M_disjunct(__s))
  this->reserve(__len);
       else
  {
    const size_type __off = __s - _M_data();
    this->reserve(__len);
    __s = _M_data() + __off;
  }
     }
   _M_copy(_M_data() + this->size(), __s, __n);
   _M_rep()->_M_set_length_and_sharable(__len);
 }
      return *this;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>&
    basic_string<_CharT, _Traits, _Alloc>::
    append(const basic_string& __str)
    {
      const size_type __size = __str.size();
      if (__size)
 {
   const size_type __len = __size + this->size();
   if (__len > this->capacity() || _M_rep()->_M_is_shared())
     this->reserve(__len);
   _M_copy(_M_data() + this->size(), __str._M_data(), __size);
   _M_rep()->_M_set_length_and_sharable(__len);
 }
      return *this;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>&
    basic_string<_CharT, _Traits, _Alloc>::
    append(const basic_string& __str, size_type __pos, size_type __n)
    {
      __str._M_check(__pos, "basic_string::append");
      __n = __str._M_limit(__pos, __n);
      if (__n)
 {
   const size_type __len = __n + this->size();
   if (__len > this->capacity() || _M_rep()->_M_is_shared())
     this->reserve(__len);
   _M_copy(_M_data() + this->size(), __str._M_data() + __pos, __n);
   _M_rep()->_M_set_length_and_sharable(__len);
 }
      return *this;
    }

   template<typename _CharT, typename _Traits, typename _Alloc>
     basic_string<_CharT, _Traits, _Alloc>&
     basic_string<_CharT, _Traits, _Alloc>::
     insert(size_type __pos, const _CharT* __s, size_type __n)
     {
       ;
       _M_check(__pos, "basic_string::insert");
       _M_check_length(size_type(0), __n, "basic_string::insert");
       if (_M_disjunct(__s) || _M_rep()->_M_is_shared())
         return _M_replace_safe(__pos, size_type(0), __s, __n);
       else
         {

           const size_type __off = __s - _M_data();
           _M_mutate(__pos, 0, __n);
           __s = _M_data() + __off;
           _CharT* __p = _M_data() + __pos;
           if (__s + __n <= __p)
             _M_copy(__p, __s, __n);
           else if (__s >= __p)
             _M_copy(__p, __s + __n, __n);
           else
             {
        const size_type __nleft = __p - __s;
               _M_copy(__p, __s, __nleft);
               _M_copy(__p + __nleft, __p + __n, __n - __nleft);
             }
           return *this;
         }
     }

   template<typename _CharT, typename _Traits, typename _Alloc>
     basic_string<_CharT, _Traits, _Alloc>&
     basic_string<_CharT, _Traits, _Alloc>::
     replace(size_type __pos, size_type __n1, const _CharT* __s,
      size_type __n2)
     {
       ;
       _M_check(__pos, "basic_string::replace");
       __n1 = _M_limit(__pos, __n1);
       _M_check_length(__n1, __n2, "basic_string::replace");
       bool __left;
       if (_M_disjunct(__s) || _M_rep()->_M_is_shared())
         return _M_replace_safe(__pos, __n1, __s, __n2);
       else if ((__left = __s + __n2 <= _M_data() + __pos)
  || _M_data() + __pos + __n1 <= __s)
  {

    size_type __off = __s - _M_data();
    __left ? __off : (__off += __n2 - __n1);
    _M_mutate(__pos, __n1, __n2);
    _M_copy(_M_data() + __pos, _M_data() + __off, __n2);
    return *this;
  }
       else
  {

    const basic_string __tmp(__s, __n2);
    return _M_replace_safe(__pos, __n1, __tmp._M_data(), __n2);
  }
     }

  template<typename _CharT, typename _Traits, typename _Alloc>
    void
    basic_string<_CharT, _Traits, _Alloc>::_Rep::
    _M_destroy(const _Alloc& __a) throw ()
    {
      const size_type __size = sizeof(_Rep_base) +
                        (this->_M_capacity + 1) * sizeof(_CharT);
      _Raw_bytes_alloc(__a).deallocate(reinterpret_cast<char*>(this), __size);
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    void
    basic_string<_CharT, _Traits, _Alloc>::
    _M_leak_hard()
    {

      if (_M_rep() == &_S_empty_rep())
 return;

      if (_M_rep()->_M_is_shared())
 _M_mutate(0, 0, 0);
      _M_rep()->_M_set_leaked();
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    void
    basic_string<_CharT, _Traits, _Alloc>::
    _M_mutate(size_type __pos, size_type __len1, size_type __len2)
    {
      const size_type __old_size = this->size();
      const size_type __new_size = __old_size + __len2 - __len1;
      const size_type __how_much = __old_size - __pos - __len1;

      if (__new_size > this->capacity() || _M_rep()->_M_is_shared())
 {

   const allocator_type __a = get_allocator();
   _Rep* __r = _Rep::_S_create(__new_size, this->capacity(), __a);

   if (__pos)
     _M_copy(__r->_M_refdata(), _M_data(), __pos);
   if (__how_much)
     _M_copy(__r->_M_refdata() + __pos + __len2,
      _M_data() + __pos + __len1, __how_much);

   _M_rep()->_M_dispose(__a);
   _M_data(__r->_M_refdata());
 }
      else if (__how_much && __len1 != __len2)
 {

   _M_move(_M_data() + __pos + __len2,
    _M_data() + __pos + __len1, __how_much);
 }
      _M_rep()->_M_set_length_and_sharable(__new_size);
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    void
    basic_string<_CharT, _Traits, _Alloc>::
    reserve(size_type __res)
    {
      if (__res != this->capacity() || _M_rep()->_M_is_shared())
        {

   if (__res < this->size())
     __res = this->size();
   const allocator_type __a = get_allocator();
   _CharT* __tmp = _M_rep()->_M_clone(__a, __res - this->size());
   _M_rep()->_M_dispose(__a);
   _M_data(__tmp);
        }
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    void
    basic_string<_CharT, _Traits, _Alloc>::
    swap(basic_string& __s)
    {
      if (_M_rep()->_M_is_leaked())
 _M_rep()->_M_set_sharable();
      if (__s._M_rep()->_M_is_leaked())
 __s._M_rep()->_M_set_sharable();
      if (this->get_allocator() == __s.get_allocator())
 {
   _CharT* __tmp = _M_data();
   _M_data(__s._M_data());
   __s._M_data(__tmp);
 }

      else
 {
   const basic_string __tmp1(_M_ibegin(), _M_iend(),
        __s.get_allocator());
   const basic_string __tmp2(__s._M_ibegin(), __s._M_iend(),
        this->get_allocator());
   *this = __tmp2;
   __s = __tmp1;
 }
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    typename basic_string<_CharT, _Traits, _Alloc>::_Rep*
    basic_string<_CharT, _Traits, _Alloc>::_Rep::
    _S_create(size_type __capacity, size_type __old_capacity,
       const _Alloc& __alloc)
    {
      if (__capacity > _S_max_size)
 __throw_length_error(("basic_string::_S_create"));

      const size_type __pagesize = 4096;
      const size_type __malloc_header_size = 4 * sizeof(void*);

      if (__capacity > __old_capacity && __capacity < 2 * __old_capacity)
 __capacity = 2 * __old_capacity;
      size_type __size = (__capacity + 1) * sizeof(_CharT) + sizeof(_Rep);

      const size_type __adj_size = __size + __malloc_header_size;
      if (__adj_size > __pagesize && __capacity > __old_capacity)
 {
   const size_type __extra = __pagesize - __adj_size % __pagesize;
   __capacity += __extra / sizeof(_CharT);

   if (__capacity > _S_max_size)
     __capacity = _S_max_size;
   __size = (__capacity + 1) * sizeof(_CharT) + sizeof(_Rep);
 }

      void* __place = _Raw_bytes_alloc(__alloc).allocate(__size);
      _Rep *__p = new (__place) _Rep;
      __p->_M_capacity = __capacity;

      __p->_M_set_sharable();
      return __p;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    _CharT*
    basic_string<_CharT, _Traits, _Alloc>::_Rep::
    _M_clone(const _Alloc& __alloc, size_type __res)
    {

      const size_type __requested_cap = this->_M_length + __res;
      _Rep* __r = _Rep::_S_create(__requested_cap, this->_M_capacity,
      __alloc);
      if (this->_M_length)
 _M_copy(__r->_M_refdata(), _M_refdata(), this->_M_length);

      __r->_M_set_length_and_sharable(this->_M_length);
      return __r->_M_refdata();
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    void
    basic_string<_CharT, _Traits, _Alloc>::
    resize(size_type __n, _CharT __c)
    {
      const size_type __size = this->size();
      _M_check_length(__size, __n, "basic_string::resize");
      if (__size < __n)
 this->append(__n - __size, __c);
      else if (__n < __size)
 this->erase(__n);

    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    template<typename _InputIterator>
      basic_string<_CharT, _Traits, _Alloc>&
      basic_string<_CharT, _Traits, _Alloc>::
      _M_replace_dispatch(iterator __i1, iterator __i2, _InputIterator __k1,
     _InputIterator __k2, __false_type)
      {
 const basic_string __s(__k1, __k2);
 const size_type __n1 = __i2 - __i1;
 _M_check_length(__n1, __s.size(), "basic_string::_M_replace_dispatch");
 return _M_replace_safe(__i1 - _M_ibegin(), __n1, __s._M_data(),
          __s.size());
      }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>&
    basic_string<_CharT, _Traits, _Alloc>::
    _M_replace_aux(size_type __pos1, size_type __n1, size_type __n2,
     _CharT __c)
    {
      _M_check_length(__n1, __n2, "basic_string::_M_replace_aux");
      _M_mutate(__pos1, __n1, __n2);
      if (__n2)
 _M_assign(_M_data() + __pos1, __n2, __c);
      return *this;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>&
    basic_string<_CharT, _Traits, _Alloc>::
    _M_replace_safe(size_type __pos1, size_type __n1, const _CharT* __s,
      size_type __n2)
    {
      _M_mutate(__pos1, __n1, __n2);
      if (__n2)
 _M_copy(_M_data() + __pos1, __s, __n2);
      return *this;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>
    operator+(const _CharT* __lhs,
       const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    {
      ;
      typedef basic_string<_CharT, _Traits, _Alloc> __string_type;
      typedef typename __string_type::size_type __size_type;
      const __size_type __len = _Traits::length(__lhs);
      __string_type __str;
      __str.reserve(__len + __rhs.size());
      __str.append(__lhs, __len);
      __str.append(__rhs);
      return __str;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_string<_CharT, _Traits, _Alloc>
    operator+(_CharT __lhs, const basic_string<_CharT, _Traits, _Alloc>& __rhs)
    {
      typedef basic_string<_CharT, _Traits, _Alloc> __string_type;
      typedef typename __string_type::size_type __size_type;
      __string_type __str;
      const __size_type __len = __rhs.size();
      __str.reserve(__len + 1);
      __str.append(__size_type(1), __lhs);
      __str.append(__rhs);
      return __str;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::
    copy(_CharT* __s, size_type __n, size_type __pos) const
    {
      _M_check(__pos, "basic_string::copy");
      __n = _M_limit(__pos, __n);
      ;
      if (__n)
 _M_copy(__s, _M_data() + __pos, __n);

      return __n;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::
    find(const _CharT* __s, size_type __pos, size_type __n) const
    {
      ;
      const size_type __size = this->size();
      const _CharT* __data = _M_data();

      if (__n == 0)
 return __pos <= __size ? __pos : npos;

      if (__n <= __size)
 {
   for (; __pos <= __size - __n; ++__pos)
     if (traits_type::eq(__data[__pos], __s[0])
  && traits_type::compare(__data + __pos + 1,
     __s + 1, __n - 1) == 0)
       return __pos;
 }
      return npos;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::
    find(_CharT __c, size_type __pos) const
    {
      size_type __ret = npos;
      const size_type __size = this->size();
      if (__pos < __size)
 {
   const _CharT* __data = _M_data();
   const size_type __n = __size - __pos;
   const _CharT* __p = traits_type::find(__data + __pos, __n, __c);
   if (__p)
     __ret = __p - __data;
 }
      return __ret;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::
    rfind(const _CharT* __s, size_type __pos, size_type __n) const
    {
      ;
      const size_type __size = this->size();
      if (__n <= __size)
 {
   __pos = std::min(size_type(__size - __n), __pos);
   const _CharT* __data = _M_data();
   do
     {
       if (traits_type::compare(__data + __pos, __s, __n) == 0)
  return __pos;
     }
   while (__pos-- > 0);
 }
      return npos;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::
    rfind(_CharT __c, size_type __pos) const
    {
      size_type __size = this->size();
      if (__size)
 {
   if (--__size > __pos)
     __size = __pos;
   for (++__size; __size-- > 0; )
     if (traits_type::eq(_M_data()[__size], __c))
       return __size;
 }
      return npos;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::
    find_first_of(const _CharT* __s, size_type __pos, size_type __n) const
    {
      ;
      for (; __n && __pos < this->size(); ++__pos)
 {
   const _CharT* __p = traits_type::find(__s, __n, _M_data()[__pos]);
   if (__p)
     return __pos;
 }
      return npos;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::
    find_last_of(const _CharT* __s, size_type __pos, size_type __n) const
    {
      ;
      size_type __size = this->size();
      if (__size && __n)
 {
   if (--__size > __pos)
     __size = __pos;
   do
     {
       if (traits_type::find(__s, __n, _M_data()[__size]))
  return __size;
     }
   while (__size-- != 0);
 }
      return npos;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::
    find_first_not_of(const _CharT* __s, size_type __pos, size_type __n) const
    {
      ;
      for (; __pos < this->size(); ++__pos)
 if (!traits_type::find(__s, __n, _M_data()[__pos]))
   return __pos;
      return npos;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::
    find_first_not_of(_CharT __c, size_type __pos) const
    {
      for (; __pos < this->size(); ++__pos)
 if (!traits_type::eq(_M_data()[__pos], __c))
   return __pos;
      return npos;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::
    find_last_not_of(const _CharT* __s, size_type __pos, size_type __n) const
    {
      ;
      size_type __size = this->size();
      if (__size)
 {
   if (--__size > __pos)
     __size = __pos;
   do
     {
       if (!traits_type::find(__s, __n, _M_data()[__size]))
  return __size;
     }
   while (__size--);
 }
      return npos;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    typename basic_string<_CharT, _Traits, _Alloc>::size_type
    basic_string<_CharT, _Traits, _Alloc>::
    find_last_not_of(_CharT __c, size_type __pos) const
    {
      size_type __size = this->size();
      if (__size)
 {
   if (--__size > __pos)
     __size = __pos;
   do
     {
       if (!traits_type::eq(_M_data()[__size], __c))
  return __size;
     }
   while (__size--);
 }
      return npos;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    int
    basic_string<_CharT, _Traits, _Alloc>::
    compare(size_type __pos, size_type __n, const basic_string& __str) const
    {
      _M_check(__pos, "basic_string::compare");
      __n = _M_limit(__pos, __n);
      const size_type __osize = __str.size();
      const size_type __len = std::min(__n, __osize);
      int __r = traits_type::compare(_M_data() + __pos, __str.data(), __len);
      if (!__r)
 __r = _S_compare(__n, __osize);
      return __r;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    int
    basic_string<_CharT, _Traits, _Alloc>::
    compare(size_type __pos1, size_type __n1, const basic_string& __str,
     size_type __pos2, size_type __n2) const
    {
      _M_check(__pos1, "basic_string::compare");
      __str._M_check(__pos2, "basic_string::compare");
      __n1 = _M_limit(__pos1, __n1);
      __n2 = __str._M_limit(__pos2, __n2);
      const size_type __len = std::min(__n1, __n2);
      int __r = traits_type::compare(_M_data() + __pos1,
         __str.data() + __pos2, __len);
      if (!__r)
 __r = _S_compare(__n1, __n2);
      return __r;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    int
    basic_string<_CharT, _Traits, _Alloc>::
    compare(const _CharT* __s) const
    {
      ;
      const size_type __size = this->size();
      const size_type __osize = traits_type::length(__s);
      const size_type __len = std::min(__size, __osize);
      int __r = traits_type::compare(_M_data(), __s, __len);
      if (!__r)
 __r = _S_compare(__size, __osize);
      return __r;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    int
    basic_string <_CharT, _Traits, _Alloc>::
    compare(size_type __pos, size_type __n1, const _CharT* __s) const
    {
      ;
      _M_check(__pos, "basic_string::compare");
      __n1 = _M_limit(__pos, __n1);
      const size_type __osize = traits_type::length(__s);
      const size_type __len = std::min(__n1, __osize);
      int __r = traits_type::compare(_M_data() + __pos, __s, __len);
      if (!__r)
 __r = _S_compare(__n1, __osize);
      return __r;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    int
    basic_string <_CharT, _Traits, _Alloc>::
    compare(size_type __pos, size_type __n1, const _CharT* __s,
     size_type __n2) const
    {
      ;
      _M_check(__pos, "basic_string::compare");
      __n1 = _M_limit(__pos, __n1);
      const size_type __len = std::min(__n1, __n2);
      int __r = traits_type::compare(_M_data() + __pos, __s, __len);
      if (!__r)
 __r = _S_compare(__n1, __n2);
      return __r;
    }

  extern template class basic_string<char>;
  extern template
    basic_istream<char>&
    operator>>(basic_istream<char>&, string&);
  extern template
    basic_ostream<char>&
    operator<<(basic_ostream<char>&, const string&);
  extern template
    basic_istream<char>&
    getline(basic_istream<char>&, string&, char);
  extern template
    basic_istream<char>&
    getline(basic_istream<char>&, string&);
  extern template class basic_string<wchar_t>;
  extern template
    basic_istream<wchar_t>&
    operator>>(basic_istream<wchar_t>&, wstring&);
  extern template
    basic_ostream<wchar_t>&
    operator<<(basic_ostream<wchar_t>&, const wstring&);
  extern template
    basic_istream<wchar_t>&
    getline(basic_istream<wchar_t>&, wstring&, wchar_t);
  extern template
    basic_istream<wchar_t>&
    getline(basic_istream<wchar_t>&, wstring&);

}
using std::string;

  namespace enigma { enum var_types { vt_real, vt_tstr }; }
namespace enigma {
  union rvt {
    double d;
    void * p;
    rvt(double x): d(x) {}
  };
}

struct var;

struct variant
{
  enigma::rvt rval;
  string sval;
  int type;

  operator int();
  operator bool();
  operator char();
  operator long();
  operator short();
  operator unsigned();
  operator unsigned char();
  operator unsigned long();
  operator unsigned short();
  operator unsigned long long();
  operator long long();
  operator double();
  operator float();

  operator string();

  operator int() const;
  operator bool() const;
  operator char() const;
  operator long() const;
  operator short() const;
  operator unsigned() const;
  operator unsigned char() const;
  operator unsigned long() const;
  operator unsigned short() const;
  operator unsigned long long() const;
  operator long long() const;
  operator double() const;
  operator float() const;

  operator string() const;

  variant();
  variant (int x) ; variant (long x) ; variant (unsigned long x) ; variant (unsigned x) ; variant (long long x) ; variant (unsigned long long x) ; variant (float x) ; variant (double x) ; variant (const char *x) ; variant (string x) ; variant (const variant &x) ; variant (const var &x) ;

  variant& operator= (int x) ; variant& operator= (long x) ; variant& operator= (unsigned long x) ; variant& operator= (unsigned x) ; variant& operator= (long long x) ; variant& operator= (unsigned long long x) ; variant& operator= (float x) ; variant& operator= (double x) ; variant& operator= (const char *x) ; variant& operator= (string x) ; variant& operator= (const variant x) ; variant& operator= (const var &x) ;
  variant& operator+= (int x) ; variant& operator+= (long x) ; variant& operator+= (unsigned long x) ; variant& operator+= (unsigned x) ; variant& operator+= (long long x) ; variant& operator+= (unsigned long long x) ; variant& operator+= (float x) ; variant& operator+= (double x) ; variant& operator+= (const char *x) ; variant& operator+= (string x) ; variant& operator+= (const variant x) ; variant& operator+= (const var &x) ;
  variant& operator-= (int x) ; variant& operator-= (long x) ; variant& operator-= (unsigned long x) ; variant& operator-= (unsigned x) ; variant& operator-= (long long x) ; variant& operator-= (unsigned long long x) ; variant& operator-= (float x) ; variant& operator-= (double x) ; variant& operator-= (const char *x) ; variant& operator-= (string x) ; variant& operator-= (const variant x) ; variant& operator-= (const var &x) ;
  variant& operator*= (int x) ; variant& operator*= (long x) ; variant& operator*= (unsigned long x) ; variant& operator*= (unsigned x) ; variant& operator*= (long long x) ; variant& operator*= (unsigned long long x) ; variant& operator*= (float x) ; variant& operator*= (double x) ; variant& operator*= (const char *x) ; variant& operator*= (string x) ; variant& operator*= (const variant x) ; variant& operator*= (const var &x) ;
  variant& operator/= (int x) ; variant& operator/= (long x) ; variant& operator/= (unsigned long x) ; variant& operator/= (unsigned x) ; variant& operator/= (long long x) ; variant& operator/= (unsigned long long x) ; variant& operator/= (float x) ; variant& operator/= (double x) ; variant& operator/= (const char *x) ; variant& operator/= (string x) ; variant& operator/= (const variant x) ; variant& operator/= (const var &x) ;
  variant& operator%= (int x) ; variant& operator%= (long x) ; variant& operator%= (unsigned long x) ; variant& operator%= (unsigned x) ; variant& operator%= (long long x) ; variant& operator%= (unsigned long long x) ; variant& operator%= (float x) ; variant& operator%= (double x) ; variant& operator%= (const char *x) ; variant& operator%= (string x) ; variant& operator%= (const variant x) ; variant& operator%= (const var &x) ;

  variant& operator<<= (int x) ; variant& operator<<= (long x) ; variant& operator<<= (unsigned long x) ; variant& operator<<= (unsigned x) ; variant& operator<<= (long long x) ; variant& operator<<= (unsigned long long x) ; variant& operator<<= (float x) ; variant& operator<<= (double x) ; variant& operator<<= (const char *x) ; variant& operator<<= (string x) ; variant& operator<<= (const variant x) ; variant& operator<<= (const var &x) ;
  variant& operator>>= (int x) ; variant& operator>>= (long x) ; variant& operator>>= (unsigned long x) ; variant& operator>>= (unsigned x) ; variant& operator>>= (long long x) ; variant& operator>>= (unsigned long long x) ; variant& operator>>= (float x) ; variant& operator>>= (double x) ; variant& operator>>= (const char *x) ; variant& operator>>= (string x) ; variant& operator>>= (const variant x) ; variant& operator>>= (const var &x) ;
  variant& operator&= (int x) ; variant& operator&= (long x) ; variant& operator&= (unsigned long x) ; variant& operator&= (unsigned x) ; variant& operator&= (long long x) ; variant& operator&= (unsigned long long x) ; variant& operator&= (float x) ; variant& operator&= (double x) ; variant& operator&= (const char *x) ; variant& operator&= (string x) ; variant& operator&= (const variant x) ; variant& operator&= (const var &x) ;
  variant& operator|= (int x) ; variant& operator|= (long x) ; variant& operator|= (unsigned long x) ; variant& operator|= (unsigned x) ; variant& operator|= (long long x) ; variant& operator|= (unsigned long long x) ; variant& operator|= (float x) ; variant& operator|= (double x) ; variant& operator|= (const char *x) ; variant& operator|= (string x) ; variant& operator|= (const variant x) ; variant& operator|= (const var &x) ;
  variant& operator^= (int x) ; variant& operator^= (long x) ; variant& operator^= (unsigned long x) ; variant& operator^= (unsigned x) ; variant& operator^= (long long x) ; variant& operator^= (unsigned long long x) ; variant& operator^= (float x) ; variant& operator^= (double x) ; variant& operator^= (const char *x) ; variant& operator^= (string x) ; variant& operator^= (const variant x) ; variant& operator^= (const var &x) ;

  variant operator+ (int x) const ; variant operator+ (long x) const ; variant operator+ (unsigned long x) const ; variant operator+ (unsigned x) const ; variant operator+ (long long x) const ; variant operator+ (unsigned long long x) const ; variant operator+ (float x) const ; variant operator+ (double x) const ; variant operator+ (const char *x) const ; variant operator+ (string x) const ; variant operator+ (const variant x) const; variant operator+ (const var &x) const;
  double operator- (int x) const ; double operator- (long x) const ; double operator- (unsigned long x) const ; double operator- (unsigned x) const ; double operator- (long long x) const ; double operator- (unsigned long long x) const ; double operator- (float x) const ; double operator- (double x) const ; double operator- (const char *x) const ; double operator- (string x) const ; double operator- (const variant x) const; double operator- (const var &x) const;
  double operator* (int x) const ; double operator* (long x) const ; double operator* (unsigned long x) const ; double operator* (unsigned x) const ; double operator* (long long x) const ; double operator* (unsigned long long x) const ; double operator* (float x) const ; double operator* (double x) const ; double operator* (const char *x) const ; double operator* (string x) const ; double operator* (const variant x) const; double operator* (const var &x) const;
  double operator/ (int x) const ; double operator/ (long x) const ; double operator/ (unsigned long x) const ; double operator/ (unsigned x) const ; double operator/ (long long x) const ; double operator/ (unsigned long long x) const ; double operator/ (float x) const ; double operator/ (double x) const ; double operator/ (const char *x) const ; double operator/ (string x) const ; double operator/ (const variant x) const; double operator/ (const var &x) const;
  double operator% (int x) const ; double operator% (long x) const ; double operator% (unsigned long x) const ; double operator% (unsigned x) const ; double operator% (long long x) const ; double operator% (unsigned long long x) const ; double operator% (float x) const ; double operator% (double x) const ; double operator% (const char *x) const ; double operator% (string x) const ; double operator% (const variant x) const; double operator% (const var &x) const;

  long operator<< (int x) const ; long operator<< (long x) const ; long operator<< (unsigned long x) const ; long operator<< (unsigned x) const ; long operator<< (long long x) const ; long operator<< (unsigned long long x) const ; long operator<< (float x) const ; long operator<< (double x) const ; long operator<< (const char *x) const ; long operator<< (string x) const ; long operator<< (const variant x) const; long operator<< (const var &x) const;
  long operator>> (int x) const ; long operator>> (long x) const ; long operator>> (unsigned long x) const ; long operator>> (unsigned x) const ; long operator>> (long long x) const ; long operator>> (unsigned long long x) const ; long operator>> (float x) const ; long operator>> (double x) const ; long operator>> (const char *x) const ; long operator>> (string x) const ; long operator>> (const variant x) const; long operator>> (const var &x) const;
  long operator& (int x) const ; long operator& (long x) const ; long operator& (unsigned long x) const ; long operator& (unsigned x) const ; long operator& (long long x) const ; long operator& (unsigned long long x) const ; long operator& (float x) const ; long operator& (double x) const ; long operator& (const char *x) const ; long operator& (string x) const ; long operator& (const variant x) const; long operator& (const var &x) const;
  long operator| (int x) const ; long operator| (long x) const ; long operator| (unsigned long x) const ; long operator| (unsigned x) const ; long operator| (long long x) const ; long operator| (unsigned long long x) const ; long operator| (float x) const ; long operator| (double x) const ; long operator| (const char *x) const ; long operator| (string x) const ; long operator| (const variant x) const; long operator| (const var &x) const;
  long operator^ (int x) const ; long operator^ (long x) const ; long operator^ (unsigned long x) const ; long operator^ (unsigned x) const ; long operator^ (long long x) const ; long operator^ (unsigned long long x) const ; long operator^ (float x) const ; long operator^ (double x) const ; long operator^ (const char *x) const ; long operator^ (string x) const ; long operator^ (const variant x) const; long operator^ (const var &x) const;

  bool operator== (int x) const ; bool operator== (long x) const ; bool operator== (unsigned long x) const ; bool operator== (unsigned x) const ; bool operator== (long long x) const ; bool operator== (unsigned long long x) const ; bool operator== (float x) const ; bool operator== (double x) const ; bool operator== (const char *x) const ; bool operator== (string x) const ; bool operator== (const variant &x) const; bool operator== (const var &x) const;
  bool operator!= (int x) const ; bool operator!= (long x) const ; bool operator!= (unsigned long x) const ; bool operator!= (unsigned x) const ; bool operator!= (long long x) const ; bool operator!= (unsigned long long x) const ; bool operator!= (float x) const ; bool operator!= (double x) const ; bool operator!= (const char *x) const ; bool operator!= (string x) const ; bool operator!= (const variant &x) const; bool operator!= (const var &x) const;
  bool operator>= (int x) const ; bool operator>= (long x) const ; bool operator>= (unsigned long x) const ; bool operator>= (unsigned x) const ; bool operator>= (long long x) const ; bool operator>= (unsigned long long x) const ; bool operator>= (float x) const ; bool operator>= (double x) const ; bool operator>= (const char *x) const ; bool operator>= (string x) const ; bool operator>= (const variant &x) const; bool operator>= (const var &x) const;
  bool operator<= (int x) const ; bool operator<= (long x) const ; bool operator<= (unsigned long x) const ; bool operator<= (unsigned x) const ; bool operator<= (long long x) const ; bool operator<= (unsigned long long x) const ; bool operator<= (float x) const ; bool operator<= (double x) const ; bool operator<= (const char *x) const ; bool operator<= (string x) const ; bool operator<= (const variant &x) const; bool operator<= (const var &x) const;
  bool operator> (int x) const ; bool operator> (long x) const ; bool operator> (unsigned long x) const ; bool operator> (unsigned x) const ; bool operator> (long long x) const ; bool operator> (unsigned long long x) const ; bool operator> (float x) const ; bool operator> (double x) const ; bool operator> (const char *x) const ; bool operator> (string x) const ; bool operator> (const variant &x) const; bool operator> (const var &x) const;
  bool operator< (int x) const ; bool operator< (long x) const ; bool operator< (unsigned long x) const ; bool operator< (unsigned x) const ; bool operator< (long long x) const ; bool operator< (unsigned long long x) const ; bool operator< (float x) const ; bool operator< (double x) const ; bool operator< (const char *x) const ; bool operator< (string x) const ; bool operator< (const variant &x) const; bool operator< (const var &x) const;

  char& operator[] (int);
  variant& operator++();
  double operator++(int);
  variant& operator--();
  double operator--(int);
  variant& operator*();

  bool operator!() const;
  long operator~() const;
  double operator-() const;
  double operator+() const;

  ~variant();
};

struct var
{
  void *values;

  private:
    void initialize();
    void cleanup();
  public:

  operator variant&();
  operator const variant&() const;

  operator int();
  operator bool();
  operator char();
  operator long();
  operator short();
  operator unsigned();
  operator unsigned char();
  operator unsigned long();
  operator unsigned short();
  operator unsigned long long();
  operator long long();
  operator double();
  operator float();

  operator string();

  operator int() const;
  operator bool() const;
  operator char() const;
  operator long() const;
  operator short() const;
  operator unsigned() const;
  operator unsigned char() const;
  operator unsigned long() const;
  operator unsigned short() const;
  operator unsigned long long() const;
  operator long long() const;
  operator double() const;
  operator float() const;

  operator string() const;

  var();
  var(const var&);
  var (int x) ; var (long x) ; var (unsigned long x) ; var (unsigned x) ; var (long long x) ; var (unsigned long long x) ; var (float x) ; var (double x) ; var (const char *x) ; var (string x) ; var (variant x);;

  variant& operator= (int x) ; variant& operator= (long x) ; variant& operator= (unsigned long x) ; variant& operator= (unsigned x) ; variant& operator= (long long x) ; variant& operator= (unsigned long long x) ; variant& operator= (float x) ; variant& operator= (double x) ; variant& operator= (const char *x) ; variant& operator= (string x) ; variant& operator= (variant x);
  var& operator= (const var&);

  variant& operator+= (int x) ; variant& operator+= (long x) ; variant& operator+= (unsigned long x) ; variant& operator+= (unsigned x) ; variant& operator+= (long long x) ; variant& operator+= (unsigned long long x) ; variant& operator+= (float x) ; variant& operator+= (double x) ; variant& operator+= (const char *x) ; variant& operator+= (string x) ; variant& operator+= (const var& x) ; variant& operator+= (variant x) ;
  variant& operator-= (int x) ; variant& operator-= (long x) ; variant& operator-= (unsigned long x) ; variant& operator-= (unsigned x) ; variant& operator-= (long long x) ; variant& operator-= (unsigned long long x) ; variant& operator-= (float x) ; variant& operator-= (double x) ; variant& operator-= (const char *x) ; variant& operator-= (string x) ; variant& operator-= (const var& x) ; variant& operator-= (variant x) ;
  variant& operator*= (int x) ; variant& operator*= (long x) ; variant& operator*= (unsigned long x) ; variant& operator*= (unsigned x) ; variant& operator*= (long long x) ; variant& operator*= (unsigned long long x) ; variant& operator*= (float x) ; variant& operator*= (double x) ; variant& operator*= (const char *x) ; variant& operator*= (string x) ; variant& operator*= (const var& x) ; variant& operator*= (variant x) ;
  variant& operator/= (int x) ; variant& operator/= (long x) ; variant& operator/= (unsigned long x) ; variant& operator/= (unsigned x) ; variant& operator/= (long long x) ; variant& operator/= (unsigned long long x) ; variant& operator/= (float x) ; variant& operator/= (double x) ; variant& operator/= (const char *x) ; variant& operator/= (string x) ; variant& operator/= (const var& x) ; variant& operator/= (variant x) ;

  variant& operator<<= (int x) ; variant& operator<<= (long x) ; variant& operator<<= (unsigned long x) ; variant& operator<<= (unsigned x) ; variant& operator<<= (long long x) ; variant& operator<<= (unsigned long long x) ; variant& operator<<= (float x) ; variant& operator<<= (double x) ; variant& operator<<= (const char *x) ; variant& operator<<= (string x) ; variant& operator<<= (const var& x) ; variant& operator<<= (variant x) ;
  variant& operator>>= (int x) ; variant& operator>>= (long x) ; variant& operator>>= (unsigned long x) ; variant& operator>>= (unsigned x) ; variant& operator>>= (long long x) ; variant& operator>>= (unsigned long long x) ; variant& operator>>= (float x) ; variant& operator>>= (double x) ; variant& operator>>= (const char *x) ; variant& operator>>= (string x) ; variant& operator>>= (const var& x) ; variant& operator>>= (variant x) ;
  variant& operator&= (int x) ; variant& operator&= (long x) ; variant& operator&= (unsigned long x) ; variant& operator&= (unsigned x) ; variant& operator&= (long long x) ; variant& operator&= (unsigned long long x) ; variant& operator&= (float x) ; variant& operator&= (double x) ; variant& operator&= (const char *x) ; variant& operator&= (string x) ; variant& operator&= (const var& x) ; variant& operator&= (variant x) ;
  variant& operator|= (int x) ; variant& operator|= (long x) ; variant& operator|= (unsigned long x) ; variant& operator|= (unsigned x) ; variant& operator|= (long long x) ; variant& operator|= (unsigned long long x) ; variant& operator|= (float x) ; variant& operator|= (double x) ; variant& operator|= (const char *x) ; variant& operator|= (string x) ; variant& operator|= (const var& x) ; variant& operator|= (variant x) ;
  variant& operator^= (int x) ; variant& operator^= (long x) ; variant& operator^= (unsigned long x) ; variant& operator^= (unsigned x) ; variant& operator^= (long long x) ; variant& operator^= (unsigned long long x) ; variant& operator^= (float x) ; variant& operator^= (double x) ; variant& operator^= (const char *x) ; variant& operator^= (string x) ; variant& operator^= (const var& x) ; variant& operator^= (variant x) ;

  variant operator+ (int x) const ; variant operator+ (long x) const ; variant operator+ (unsigned long x) const ; variant operator+ (unsigned x) const ; variant operator+ (long long x) const ; variant operator+ (unsigned long long x) const ; variant operator+ (float x) const ; variant operator+ (double x) const ; variant operator+ (const char *x) const ; variant operator+ (string x) const ; variant operator+ (const var& x) const; variant operator+ (variant x) const;
  double operator- (int x) const ; double operator- (long x) const ; double operator- (unsigned long x) const ; double operator- (unsigned x) const ; double operator- (long long x) const ; double operator- (unsigned long long x) const ; double operator- (float x) const ; double operator- (double x) const ; double operator- (const char *x) const ; double operator- (string x) const ; double operator- (const var& x) const; double operator- (variant x) const;
  double operator* (int x) const ; double operator* (long x) const ; double operator* (unsigned long x) const ; double operator* (unsigned x) const ; double operator* (long long x) const ; double operator* (unsigned long long x) const ; double operator* (float x) const ; double operator* (double x) const ; double operator* (const char *x) const ; double operator* (string x) const ; double operator* (const var& x) const; double operator* (variant x) const;
  double operator/ (int x) const ; double operator/ (long x) const ; double operator/ (unsigned long x) const ; double operator/ (unsigned x) const ; double operator/ (long long x) const ; double operator/ (unsigned long long x) const ; double operator/ (float x) const ; double operator/ (double x) const ; double operator/ (const char *x) const ; double operator/ (string x) const ; double operator/ (const var& x) const; double operator/ (variant x) const;
  double operator% (int x) const ; double operator% (long x) const ; double operator% (unsigned long x) const ; double operator% (unsigned x) const ; double operator% (long long x) const ; double operator% (unsigned long long x) const ; double operator% (float x) const ; double operator% (double x) const ; double operator% (const char *x) const ; double operator% (string x) const ; double operator% (const var& x) const; double operator% (variant x) const;

  long operator<< (int x) const ; long operator<< (long x) const ; long operator<< (unsigned long x) const ; long operator<< (unsigned x) const ; long operator<< (long long x) const ; long operator<< (unsigned long long x) const ; long operator<< (float x) const ; long operator<< (double x) const ; long operator<< (const char *x) const ; long operator<< (string x) const ; long operator<< (const var& x) const; long operator<< (variant x) const;
  long operator>> (int x) const ; long operator>> (long x) const ; long operator>> (unsigned long x) const ; long operator>> (unsigned x) const ; long operator>> (long long x) const ; long operator>> (unsigned long long x) const ; long operator>> (float x) const ; long operator>> (double x) const ; long operator>> (const char *x) const ; long operator>> (string x) const ; long operator>> (const var& x) const; long operator>> (variant x) const;
  long operator& (int x) const ; long operator& (long x) const ; long operator& (unsigned long x) const ; long operator& (unsigned x) const ; long operator& (long long x) const ; long operator& (unsigned long long x) const ; long operator& (float x) const ; long operator& (double x) const ; long operator& (const char *x) const ; long operator& (string x) const ; long operator& (const var& x) const; long operator& (variant x) const;
  long operator| (int x) const ; long operator| (long x) const ; long operator| (unsigned long x) const ; long operator| (unsigned x) const ; long operator| (long long x) const ; long operator| (unsigned long long x) const ; long operator| (float x) const ; long operator| (double x) const ; long operator| (const char *x) const ; long operator| (string x) const ; long operator| (const var& x) const; long operator| (variant x) const;
  long operator^ (int x) const ; long operator^ (long x) const ; long operator^ (unsigned long x) const ; long operator^ (unsigned x) const ; long operator^ (long long x) const ; long operator^ (unsigned long long x) const ; long operator^ (float x) const ; long operator^ (double x) const ; long operator^ (const char *x) const ; long operator^ (string x) const ; long operator^ (const var& x) const; long operator^ (variant x) const;

  bool operator== (int x) const ; bool operator== (long x) const ; bool operator== (unsigned long x) const ; bool operator== (unsigned x) const ; bool operator== (long long x) const ; bool operator== (unsigned long long x) const ; bool operator== (float x) const ; bool operator== (double x) const ; bool operator== (const char *x) const ; bool operator== (string x) const ; bool operator== (const variant &x) const; bool operator== (const var& x) const;
  bool operator!= (int x) const ; bool operator!= (long x) const ; bool operator!= (unsigned long x) const ; bool operator!= (unsigned x) const ; bool operator!= (long long x) const ; bool operator!= (unsigned long long x) const ; bool operator!= (float x) const ; bool operator!= (double x) const ; bool operator!= (const char *x) const ; bool operator!= (string x) const ; bool operator!= (const variant &x) const; bool operator!= (const var& x) const;
  bool operator>= (int x) const ; bool operator>= (long x) const ; bool operator>= (unsigned long x) const ; bool operator>= (unsigned x) const ; bool operator>= (long long x) const ; bool operator>= (unsigned long long x) const ; bool operator>= (float x) const ; bool operator>= (double x) const ; bool operator>= (const char *x) const ; bool operator>= (string x) const ; bool operator>= (const variant &x) const; bool operator>= (const var& x) const;
  bool operator<= (int x) const ; bool operator<= (long x) const ; bool operator<= (unsigned long x) const ; bool operator<= (unsigned x) const ; bool operator<= (long long x) const ; bool operator<= (unsigned long long x) const ; bool operator<= (float x) const ; bool operator<= (double x) const ; bool operator<= (const char *x) const ; bool operator<= (string x) const ; bool operator<= (const variant &x) const; bool operator<= (const var& x) const;
  bool operator> (int x) const ; bool operator> (long x) const ; bool operator> (unsigned long x) const ; bool operator> (unsigned x) const ; bool operator> (long long x) const ; bool operator> (unsigned long long x) const ; bool operator> (float x) const ; bool operator> (double x) const ; bool operator> (const char *x) const ; bool operator> (string x) const ; bool operator> (const variant &x) const; bool operator> (const var& x) const;
  bool operator< (int x) const ; bool operator< (long x) const ; bool operator< (unsigned long x) const ; bool operator< (unsigned x) const ; bool operator< (long long x) const ; bool operator< (unsigned long long x) const ; bool operator< (float x) const ; bool operator< (double x) const ; bool operator< (const char *x) const ; bool operator< (string x) const ; bool operator< (const variant &x) const; bool operator< (const var& x) const;

  variant& operator++();
  double operator++(int);
  variant& operator--();
  double operator--(int);

  bool operator!() const;
  long operator~() const;
  double operator-() const;
  double operator+() const;

  variant& operator* ();
  variant& operator[] (int);
  variant& operator() ();
  variant& operator() (int);
  variant& operator() (int,int);

  const variant& operator* () const;
  const variant& operator() () const;
  const variant& operator[] (int) const;
  const variant& operator() (int) const;
  const variant& operator() (int,int) const;

  ~var();
};

int& operator += (int &x,const variant& y) ; long& operator += (long &x,const variant& y) ; unsigned long& operator += (unsigned long &x,const variant& y) ; unsigned& operator += (unsigned &x,const variant& y) ; long long& operator += (long long &x,const variant& y) ; unsigned long long& operator += (unsigned long long &x,const variant& y) ; double& operator += (double &x,const variant& y) ; float& operator += (float &x,const variant& y) ;
int& operator -= (int &x,const variant& y) ; long& operator -= (long &x,const variant& y) ; unsigned long& operator -= (unsigned long &x,const variant& y) ; unsigned& operator -= (unsigned &x,const variant& y) ; long long& operator -= (long long &x,const variant& y) ; unsigned long long& operator -= (unsigned long long &x,const variant& y) ; double& operator -= (double &x,const variant& y) ; float& operator -= (float &x,const variant& y) ;
int& operator *= (int &x,const variant& y) ; long& operator *= (long &x,const variant& y) ; unsigned long& operator *= (unsigned long &x,const variant& y) ; unsigned& operator *= (unsigned &x,const variant& y) ; long long& operator *= (long long &x,const variant& y) ; unsigned long long& operator *= (unsigned long long &x,const variant& y) ; double& operator *= (double &x,const variant& y) ; float& operator *= (float &x,const variant& y) ;
int& operator /= (int &x,const variant& y) ; long& operator /= (long &x,const variant& y) ; unsigned long& operator /= (unsigned long &x,const variant& y) ; unsigned& operator /= (unsigned &x,const variant& y) ; long long& operator /= (long long &x,const variant& y) ; unsigned long long& operator /= (unsigned long long &x,const variant& y) ; double& operator /= (double &x,const variant& y) ; float& operator /= (float &x,const variant& y) ;

int& operator <<= (int &x,const variant& y) ; long& operator <<= (long &x,const variant& y) ; unsigned long& operator <<= (unsigned long &x,const variant& y) ; unsigned& operator <<= (unsigned &x,const variant& y) ; long long& operator <<= (long long &x,const variant& y) ; unsigned long long& operator <<= (unsigned long long &x,const variant& y) ; double& operator <<= (double &x,const variant& y) ; float& operator <<= (float &x,const variant& y) ;
int& operator >>= (int &x,const variant& y) ; long& operator >>= (long &x,const variant& y) ; unsigned long& operator >>= (unsigned long &x,const variant& y) ; unsigned& operator >>= (unsigned &x,const variant& y) ; long long& operator >>= (long long &x,const variant& y) ; unsigned long long& operator >>= (unsigned long long &x,const variant& y) ; double& operator >>= (double &x,const variant& y) ; float& operator >>= (float &x,const variant& y) ;
int& operator &= (int &x,const variant& y) ; long& operator &= (long &x,const variant& y) ; unsigned long& operator &= (unsigned long &x,const variant& y) ; unsigned& operator &= (unsigned &x,const variant& y) ; long long& operator &= (long long &x,const variant& y) ; unsigned long long& operator &= (unsigned long long &x,const variant& y) ; double& operator &= (double &x,const variant& y) ; float& operator &= (float &x,const variant& y) ;
int& operator |= (int &x,const variant& y) ; long& operator |= (long &x,const variant& y) ; unsigned long& operator |= (unsigned long &x,const variant& y) ; unsigned& operator |= (unsigned &x,const variant& y) ; long long& operator |= (long long &x,const variant& y) ; unsigned long long& operator |= (unsigned long long &x,const variant& y) ; double& operator |= (double &x,const variant& y) ; float& operator |= (float &x,const variant& y) ;
int& operator ^= (int &x,const variant& y) ; long& operator ^= (long &x,const variant& y) ; unsigned long& operator ^= (unsigned long &x,const variant& y) ; unsigned& operator ^= (unsigned &x,const variant& y) ; long long& operator ^= (long long &x,const variant& y) ; unsigned long long& operator ^= (unsigned long long &x,const variant& y) ; double& operator ^= (double &x,const variant& y) ; float& operator ^= (float &x,const variant& y) ;
double operator+ (int x,const variant& y) ; double operator+ (long x,const variant& y) ; double operator+ (unsigned long x,const variant& y) ; double operator+ (unsigned x,const variant& y) ; double operator+ (long long x,const variant& y) ; double operator+ (unsigned long long x,const variant& y) ; double operator+ (double x,const variant& y) ; double operator+ (float x,const variant& y) ; string operator+ (const char *x,const variant& y) ; string operator+ (string x,const variant& y) ;
double operator- (int x,const variant& y) ; double operator- (long x,const variant& y) ; double operator- (unsigned long x,const variant& y) ; double operator- (unsigned x,const variant& y) ; double operator- (long long x,const variant& y) ; double operator- (unsigned long long x,const variant& y) ; double operator- (double x,const variant& y) ; double operator- (float x,const variant& y) ; string operator- (const char *x,const variant& y) ; string operator- (string x,const variant& y) ;
double operator* (int x,const variant& y) ; double operator* (long x,const variant& y) ; double operator* (unsigned long x,const variant& y) ; double operator* (unsigned x,const variant& y) ; double operator* (long long x,const variant& y) ; double operator* (unsigned long long x,const variant& y) ; double operator* (double x,const variant& y) ; double operator* (float x,const variant& y) ; string operator* (const char *x,const variant& y) ; string operator* (string x,const variant& y) ;
double operator/ (int x,const variant& y) ; double operator/ (long x,const variant& y) ; double operator/ (unsigned long x,const variant& y) ; double operator/ (unsigned x,const variant& y) ; double operator/ (long long x,const variant& y) ; double operator/ (unsigned long long x,const variant& y) ; double operator/ (double x,const variant& y) ; double operator/ (float x,const variant& y) ; string operator/ (const char *x,const variant& y) ; string operator/ (string x,const variant& y) ;
double operator% (int x,const variant& y) ; double operator% (long x,const variant& y) ; double operator% (unsigned long x,const variant& y) ; double operator% (unsigned x,const variant& y) ; double operator% (long long x,const variant& y) ; double operator% (unsigned long long x,const variant& y) ; double operator% (double x,const variant& y) ; double operator% (float x,const variant& y) ; string operator% (const char *x,const variant& y) ; string operator% (string x,const variant& y) ;

int operator<<(int x,const variant& y) ; long operator<<(long x,const variant& y) ; unsigned operator<<(unsigned x,const variant& y) ; unsigned long operator<<(unsigned long x,const variant& y) ; long long operator<<(long long x,const variant& y) ; unsigned long long operator<<(unsigned long long x,const variant& y) ; long operator<<(double x,const variant& y) ; long operator<<(float x,const variant& y) ;
int operator>>(int x,const variant& y) ; long operator>>(long x,const variant& y) ; unsigned operator>>(unsigned x,const variant& y) ; unsigned long operator>>(unsigned long x,const variant& y) ; long long operator>>(long long x,const variant& y) ; unsigned long long operator>>(unsigned long long x,const variant& y) ; long operator>>(double x,const variant& y) ; long operator>>(float x,const variant& y) ;
int operator&(int x,const variant& y) ; long operator&(long x,const variant& y) ; unsigned operator&(unsigned x,const variant& y) ; unsigned long operator&(unsigned long x,const variant& y) ; long long operator&(long long x,const variant& y) ; unsigned long long operator&(unsigned long long x,const variant& y) ; long operator&(double x,const variant& y) ; long operator&(float x,const variant& y) ;
int operator|(int x,const variant& y) ; long operator|(long x,const variant& y) ; unsigned operator|(unsigned x,const variant& y) ; unsigned long operator|(unsigned long x,const variant& y) ; long long operator|(long long x,const variant& y) ; unsigned long long operator|(unsigned long long x,const variant& y) ; long operator|(double x,const variant& y) ; long operator|(float x,const variant& y) ;
int operator^(int x,const variant& y) ; long operator^(long x,const variant& y) ; unsigned operator^(unsigned x,const variant& y) ; unsigned long operator^(unsigned long x,const variant& y) ; long long operator^(long long x,const variant& y) ; unsigned long long operator^(unsigned long long x,const variant& y) ; long operator^(double x,const variant& y) ; long operator^(float x,const variant& y) ;

bool operator== (int x,const variant& y) ; bool operator== (long x,const variant& y) ; bool operator== (unsigned long x,const variant& y) ; bool operator== (unsigned x,const variant& y) ; bool operator== (long long x,const variant& y) ; bool operator== (unsigned long long x,const variant& y) ; bool operator== (double x,const variant& y) ; bool operator== (float x,const variant& y) ; bool operator== (const char *x,const variant& y) ; bool operator== (string x,const variant& y) ;
bool operator!= (int x,const variant& y) ; bool operator!= (long x,const variant& y) ; bool operator!= (unsigned long x,const variant& y) ; bool operator!= (unsigned x,const variant& y) ; bool operator!= (long long x,const variant& y) ; bool operator!= (unsigned long long x,const variant& y) ; bool operator!= (double x,const variant& y) ; bool operator!= (float x,const variant& y) ; bool operator!= (const char *x,const variant& y) ; bool operator!= (string x,const variant& y) ;
bool operator>= (int x,const variant& y) ; bool operator>= (long x,const variant& y) ; bool operator>= (unsigned long x,const variant& y) ; bool operator>= (unsigned x,const variant& y) ; bool operator>= (long long x,const variant& y) ; bool operator>= (unsigned long long x,const variant& y) ; bool operator>= (double x,const variant& y) ; bool operator>= (float x,const variant& y) ; bool operator>= (const char *x,const variant& y) ; bool operator>= (string x,const variant& y) ;
bool operator<= (int x,const variant& y) ; bool operator<= (long x,const variant& y) ; bool operator<= (unsigned long x,const variant& y) ; bool operator<= (unsigned x,const variant& y) ; bool operator<= (long long x,const variant& y) ; bool operator<= (unsigned long long x,const variant& y) ; bool operator<= (double x,const variant& y) ; bool operator<= (float x,const variant& y) ; bool operator<= (const char *x,const variant& y) ; bool operator<= (string x,const variant& y) ;
bool operator> (int x,const variant& y) ; bool operator> (long x,const variant& y) ; bool operator> (unsigned long x,const variant& y) ; bool operator> (unsigned x,const variant& y) ; bool operator> (long long x,const variant& y) ; bool operator> (unsigned long long x,const variant& y) ; bool operator> (double x,const variant& y) ; bool operator> (float x,const variant& y) ; bool operator> (const char *x,const variant& y) ; bool operator> (string x,const variant& y) ;
bool operator< (int x,const variant& y) ; bool operator< (long x,const variant& y) ; bool operator< (unsigned long x,const variant& y) ; bool operator< (unsigned x,const variant& y) ; bool operator< (long long x,const variant& y) ; bool operator< (unsigned long long x,const variant& y) ; bool operator< (double x,const variant& y) ; bool operator< (float x,const variant& y) ; bool operator< (const char *x,const variant& y) ; bool operator< (string x,const variant& y) ;

int& operator += (int &x,const var& y) ; long& operator += (long &x,const var& y) ; unsigned long& operator += (unsigned long &x,const var& y) ; unsigned& operator += (unsigned &x,const var& y) ; long long& operator += (long long &x,const var& y) ; unsigned long long& operator += (unsigned long long &x,const var& y) ; double& operator += (double &x,const var& y) ; float& operator += (float &x,const var& y) ;
int& operator -= (int &x,const var& y) ; long& operator -= (long &x,const var& y) ; unsigned long& operator -= (unsigned long &x,const var& y) ; unsigned& operator -= (unsigned &x,const var& y) ; long long& operator -= (long long &x,const var& y) ; unsigned long long& operator -= (unsigned long long &x,const var& y) ; double& operator -= (double &x,const var& y) ; float& operator -= (float &x,const var& y) ;
int& operator *= (int &x,const var& y) ; long& operator *= (long &x,const var& y) ; unsigned long& operator *= (unsigned long &x,const var& y) ; unsigned& operator *= (unsigned &x,const var& y) ; long long& operator *= (long long &x,const var& y) ; unsigned long long& operator *= (unsigned long long &x,const var& y) ; double& operator *= (double &x,const var& y) ; float& operator *= (float &x,const var& y) ;
int& operator /= (int &x,const var& y) ; long& operator /= (long &x,const var& y) ; unsigned long& operator /= (unsigned long &x,const var& y) ; unsigned& operator /= (unsigned &x,const var& y) ; long long& operator /= (long long &x,const var& y) ; unsigned long long& operator /= (unsigned long long &x,const var& y) ; double& operator /= (double &x,const var& y) ; float& operator /= (float &x,const var& y) ;

int& operator <<= (int &x,const var& y) ; long& operator <<= (long &x,const var& y) ; unsigned long& operator <<= (unsigned long &x,const var& y) ; unsigned& operator <<= (unsigned &x,const var& y) ; long long& operator <<= (long long &x,const var& y) ; unsigned long long& operator <<= (unsigned long long &x,const var& y) ; double& operator <<= (double &x,const var& y) ; float& operator <<= (float &x,const var& y) ;
int& operator >>= (int &x,const var& y) ; long& operator >>= (long &x,const var& y) ; unsigned long& operator >>= (unsigned long &x,const var& y) ; unsigned& operator >>= (unsigned &x,const var& y) ; long long& operator >>= (long long &x,const var& y) ; unsigned long long& operator >>= (unsigned long long &x,const var& y) ; double& operator >>= (double &x,const var& y) ; float& operator >>= (float &x,const var& y) ;
int& operator &= (int &x,const var& y) ; long& operator &= (long &x,const var& y) ; unsigned long& operator &= (unsigned long &x,const var& y) ; unsigned& operator &= (unsigned &x,const var& y) ; long long& operator &= (long long &x,const var& y) ; unsigned long long& operator &= (unsigned long long &x,const var& y) ; double& operator &= (double &x,const var& y) ; float& operator &= (float &x,const var& y) ;
int& operator |= (int &x,const var& y) ; long& operator |= (long &x,const var& y) ; unsigned long& operator |= (unsigned long &x,const var& y) ; unsigned& operator |= (unsigned &x,const var& y) ; long long& operator |= (long long &x,const var& y) ; unsigned long long& operator |= (unsigned long long &x,const var& y) ; double& operator |= (double &x,const var& y) ; float& operator |= (float &x,const var& y) ;
int& operator ^= (int &x,const var& y) ; long& operator ^= (long &x,const var& y) ; unsigned long& operator ^= (unsigned long &x,const var& y) ; unsigned& operator ^= (unsigned &x,const var& y) ; long long& operator ^= (long long &x,const var& y) ; unsigned long long& operator ^= (unsigned long long &x,const var& y) ; double& operator ^= (double &x,const var& y) ; float& operator ^= (float &x,const var& y) ;
double operator+ (int x,const var& y) ; double operator+ (long x,const var& y) ; double operator+ (unsigned long x,const var& y) ; double operator+ (unsigned x,const var& y) ; double operator+ (long long x,const var& y) ; double operator+ (unsigned long long x,const var& y) ; double operator+ (double x,const var& y) ; double operator+ (float x,const var& y) ; string operator+ (const char *x,const var& y) ; string operator+ (string x,const var& y) ;
double operator- (int x,const var& y) ; double operator- (long x,const var& y) ; double operator- (unsigned long x,const var& y) ; double operator- (unsigned x,const var& y) ; double operator- (long long x,const var& y) ; double operator- (unsigned long long x,const var& y) ; double operator- (double x,const var& y) ; double operator- (float x,const var& y) ; string operator- (const char *x,const var& y) ; string operator- (string x,const var& y) ;
double operator* (int x,const var& y) ; double operator* (long x,const var& y) ; double operator* (unsigned long x,const var& y) ; double operator* (unsigned x,const var& y) ; double operator* (long long x,const var& y) ; double operator* (unsigned long long x,const var& y) ; double operator* (double x,const var& y) ; double operator* (float x,const var& y) ; string operator* (const char *x,const var& y) ; string operator* (string x,const var& y) ;
double operator/ (int x,const var& y) ; double operator/ (long x,const var& y) ; double operator/ (unsigned long x,const var& y) ; double operator/ (unsigned x,const var& y) ; double operator/ (long long x,const var& y) ; double operator/ (unsigned long long x,const var& y) ; double operator/ (double x,const var& y) ; double operator/ (float x,const var& y) ; string operator/ (const char *x,const var& y) ; string operator/ (string x,const var& y) ;
double operator% (int x,const var& y) ; double operator% (long x,const var& y) ; double operator% (unsigned long x,const var& y) ; double operator% (unsigned x,const var& y) ; double operator% (long long x,const var& y) ; double operator% (unsigned long long x,const var& y) ; double operator% (double x,const var& y) ; double operator% (float x,const var& y) ; string operator% (const char *x,const var& y) ; string operator% (string x,const var& y) ;

int operator<<(int x,const var& y) ; long operator<<(long x,const var& y) ; unsigned operator<<(unsigned x,const var& y) ; unsigned long operator<<(unsigned long x,const var& y) ; long long operator<<(long long x,const var& y) ; unsigned long long operator<<(unsigned long long x,const var& y) ; long operator<<(double x,const var& y) ; long operator<<(float x,const var& y) ;
int operator>>(int x,const var& y) ; long operator>>(long x,const var& y) ; unsigned operator>>(unsigned x,const var& y) ; unsigned long operator>>(unsigned long x,const var& y) ; long long operator>>(long long x,const var& y) ; unsigned long long operator>>(unsigned long long x,const var& y) ; long operator>>(double x,const var& y) ; long operator>>(float x,const var& y) ;
int operator&(int x,const var& y) ; long operator&(long x,const var& y) ; unsigned operator&(unsigned x,const var& y) ; unsigned long operator&(unsigned long x,const var& y) ; long long operator&(long long x,const var& y) ; unsigned long long operator&(unsigned long long x,const var& y) ; long operator&(double x,const var& y) ; long operator&(float x,const var& y) ;
int operator|(int x,const var& y) ; long operator|(long x,const var& y) ; unsigned operator|(unsigned x,const var& y) ; unsigned long operator|(unsigned long x,const var& y) ; long long operator|(long long x,const var& y) ; unsigned long long operator|(unsigned long long x,const var& y) ; long operator|(double x,const var& y) ; long operator|(float x,const var& y) ;
int operator^(int x,const var& y) ; long operator^(long x,const var& y) ; unsigned operator^(unsigned x,const var& y) ; unsigned long operator^(unsigned long x,const var& y) ; long long operator^(long long x,const var& y) ; unsigned long long operator^(unsigned long long x,const var& y) ; long operator^(double x,const var& y) ; long operator^(float x,const var& y) ;

bool operator== (int x,const var& y) ; bool operator== (long x,const var& y) ; bool operator== (unsigned long x,const var& y) ; bool operator== (unsigned x,const var& y) ; bool operator== (long long x,const var& y) ; bool operator== (unsigned long long x,const var& y) ; bool operator== (double x,const var& y) ; bool operator== (float x,const var& y) ; bool operator== (const char *x,const var& y) ; bool operator== (string x,const var& y) ;
bool operator!= (int x,const var& y) ; bool operator!= (long x,const var& y) ; bool operator!= (unsigned long x,const var& y) ; bool operator!= (unsigned x,const var& y) ; bool operator!= (long long x,const var& y) ; bool operator!= (unsigned long long x,const var& y) ; bool operator!= (double x,const var& y) ; bool operator!= (float x,const var& y) ; bool operator!= (const char *x,const var& y) ; bool operator!= (string x,const var& y) ;
bool operator>= (int x,const var& y) ; bool operator>= (long x,const var& y) ; bool operator>= (unsigned long x,const var& y) ; bool operator>= (unsigned x,const var& y) ; bool operator>= (long long x,const var& y) ; bool operator>= (unsigned long long x,const var& y) ; bool operator>= (double x,const var& y) ; bool operator>= (float x,const var& y) ; bool operator>= (const char *x,const var& y) ; bool operator>= (string x,const var& y) ;
bool operator<= (int x,const var& y) ; bool operator<= (long x,const var& y) ; bool operator<= (unsigned long x,const var& y) ; bool operator<= (unsigned x,const var& y) ; bool operator<= (long long x,const var& y) ; bool operator<= (unsigned long long x,const var& y) ; bool operator<= (double x,const var& y) ; bool operator<= (float x,const var& y) ; bool operator<= (const char *x,const var& y) ; bool operator<= (string x,const var& y) ;
bool operator> (int x,const var& y) ; bool operator> (long x,const var& y) ; bool operator> (unsigned long x,const var& y) ; bool operator> (unsigned x,const var& y) ; bool operator> (long long x,const var& y) ; bool operator> (unsigned long long x,const var& y) ; bool operator> (double x,const var& y) ; bool operator> (float x,const var& y) ; bool operator> (const char *x,const var& y) ; bool operator> (string x,const var& y) ;
bool operator< (int x,const var& y) ; bool operator< (long x,const var& y) ; bool operator< (unsigned long x,const var& y) ; bool operator< (unsigned x,const var& y) ; bool operator< (long long x,const var& y) ; bool operator< (unsigned long long x,const var& y) ; bool operator< (double x,const var& y) ; bool operator< (float x,const var& y) ; bool operator< (const char *x,const var& y) ; bool operator< (string x,const var& y) ;

namespace enigma {
  struct varargs
  {
    int argc;
    void *argv;

  };
}
       
extern "C" {

 typedef float float_t;
 typedef double double_t;

extern unsigned int __math_errhandling ( void );

extern int __fpclassifyf(float );
extern int __fpclassifyd(double );
extern int __fpclassify (long double);

 static __inline__ int __inline_isfinitef (float ) __attribute__ ((always_inline));
 static __inline__ int __inline_isfinited (double ) __attribute__ ((always_inline));
 static __inline__ int __inline_isfinite (long double) __attribute__ ((always_inline));
 static __inline__ int __inline_isinff (float ) __attribute__ ((always_inline));
 static __inline__ int __inline_isinfd (double ) __attribute__ ((always_inline));
 static __inline__ int __inline_isinf (long double) __attribute__ ((always_inline));
 static __inline__ int __inline_isnanf (float ) __attribute__ ((always_inline));
 static __inline__ int __inline_isnand (double ) __attribute__ ((always_inline));
 static __inline__ int __inline_isnan (long double) __attribute__ ((always_inline));
 static __inline__ int __inline_isnormalf (float ) __attribute__ ((always_inline));
 static __inline__ int __inline_isnormald (double ) __attribute__ ((always_inline));
 static __inline__ int __inline_isnormal (long double) __attribute__ ((always_inline));
 static __inline__ int __inline_signbitf (float ) __attribute__ ((always_inline));
 static __inline__ int __inline_signbitd (double ) __attribute__ ((always_inline));
 static __inline__ int __inline_signbit (long double) __attribute__ ((always_inline));

 static __inline__ int __inline_isinff( float __x ) { return __builtin_fabsf(__x) == __builtin_inff(); }
 static __inline__ int __inline_isinfd( double __x ) { return __builtin_fabs(__x) == __builtin_inf(); }
 static __inline__ int __inline_isinf( long double __x ) { return __builtin_fabsl(__x) == __builtin_infl(); }
 static __inline__ int __inline_isfinitef( float __x ) { return __x == __x && __builtin_fabsf(__x) != __builtin_inff(); }
 static __inline__ int __inline_isfinited( double __x ) { return __x == __x && __builtin_fabs(__x) != __builtin_inf(); }
 static __inline__ int __inline_isfinite( long double __x ) { return __x == __x && __builtin_fabsl(__x) != __builtin_infl(); }
 static __inline__ int __inline_isnanf( float __x ) { return __x != __x; }
 static __inline__ int __inline_isnand( double __x ) { return __x != __x; }
 static __inline__ int __inline_isnan( long double __x ) { return __x != __x; }
 static __inline__ int __inline_signbitf( float __x ) { union{ float __f; unsigned int __u; }__u; __u.__f = __x; return (int)(__u.__u >> 31); }
 static __inline__ int __inline_signbitd( double __x ) { union{ double __f; unsigned int __u[2]; }__u; __u.__f = __x; return (int)(__u.__u[1] >> 31); }
 static __inline__ int __inline_signbit( long double __x ){ union{ long double __ld; struct{ unsigned int __m[2]; short __sexp; }__p; }__u; __u.__ld = __x; return (int) (((unsigned short) __u.__p.__sexp) >> 15); }
 static __inline__ int __inline_isnormalf( float __x ) { float fabsf = __builtin_fabsf(__x); if( __x != __x ) return 0; return fabsf < __builtin_inff() && fabsf >= 1.17549435e-38F; }
 static __inline__ int __inline_isnormald( double __x ) { double fabsf = __builtin_fabs(__x); if( __x != __x ) return 0; return fabsf < __builtin_inf() && fabsf >= 2.2250738585072014e-308; }
 static __inline__ int __inline_isnormal( long double __x ) { long double fabsf = __builtin_fabsl(__x); if( __x != __x ) return 0; return fabsf < __builtin_infl() && fabsf >= 3.36210314311209350626e-4932L; }

extern double acos( double );
extern float acosf( float );

extern double asin( double );
extern float asinf( float );

extern double atan( double );
extern float atanf( float );

extern double atan2( double, double );
extern float atan2f( float, float );

extern double cos( double );
extern float cosf( float );

extern double sin( double );
extern float sinf( float );

extern double tan( double );
extern float tanf( float );

extern double acosh( double );
extern float acoshf( float );

extern double asinh( double );
extern float asinhf( float );

extern double atanh( double );
extern float atanhf( float );

extern double cosh( double );
extern float coshf( float );

extern double sinh( double );
extern float sinhf( float );

extern double tanh( double );
extern float tanhf( float );

extern double exp ( double );
extern float expf ( float );

extern double exp2 ( double );
extern float exp2f ( float );

extern double expm1 ( double );
extern float expm1f ( float );

extern double log ( double );
extern float logf ( float );

extern double log10 ( double );
extern float log10f ( float );

extern double log2 ( double );
extern float log2f ( float );

extern double log1p ( double );
extern float log1pf ( float );

extern double logb ( double );
extern float logbf ( float );

extern double modf ( double, double * );
extern float modff ( float, float * );

extern double ldexp ( double, int );
extern float ldexpf ( float, int );

extern double frexp ( double, int * );
extern float frexpf ( float, int * );

extern int ilogb ( double );
extern int ilogbf ( float );

extern double scalbn ( double, int );
extern float scalbnf ( float, int );

extern double scalbln ( double, long int );
extern float scalblnf ( float, long int );

extern double fabs( double );
extern float fabsf( float );

extern double cbrt( double );
extern float cbrtf( float );

extern double hypot ( double, double );
extern float hypotf ( float, float );

extern double pow ( double, double );
extern float powf ( float, float );

extern double sqrt( double );
extern float sqrtf( float );

extern double erf( double );
extern float erff( float );

extern double erfc( double );
extern float erfcf( float );
extern double lgamma( double );
extern float lgammaf( float );

extern double tgamma( double );
extern float tgammaf( float );

extern double ceil ( double );
extern float ceilf ( float );

extern double floor ( double );
extern float floorf ( float );

extern double nearbyint ( double );
extern float nearbyintf ( float );

extern double rint ( double );
extern float rintf ( float );

extern long int lrint ( double );
extern long int lrintf ( float );

extern double round ( double );
extern float roundf ( float );

extern long int lround ( double );
extern long int lroundf ( float );
    extern long long int llrint ( double );
    extern long long int llrintf ( float );

    extern long long int llround ( double );
    extern long long int llroundf ( float );

extern double trunc ( double );
extern float truncf ( float );

extern double fmod ( double, double );
extern float fmodf ( float, float );

extern double remainder ( double, double );
extern float remainderf ( float, float );

extern double remquo ( double, double, int * );
extern float remquof ( float, float, int * );

extern double copysign ( double, double );
extern float copysignf ( float, float );

extern double nan( const char * );
extern float nanf( const char * );

extern double nextafter ( double, double );
extern float nextafterf ( float, float );

extern double fdim ( double, double );
extern float fdimf ( float, float );

extern double fmax ( double, double );
extern float fmaxf ( float, float );

extern double fmin ( double, double );
extern float fminf ( float, float );

extern double fma ( double, double, double );
extern float fmaf ( float, float, float );

extern long double acosl(long double);
extern long double asinl(long double);
extern long double atanl(long double);
extern long double atan2l(long double, long double);
extern long double cosl(long double);
extern long double sinl(long double);
extern long double tanl(long double);
extern long double acoshl(long double);
extern long double asinhl(long double);
extern long double atanhl(long double);
extern long double coshl(long double);
extern long double sinhl(long double);
extern long double tanhl(long double);
extern long double expl(long double);
extern long double exp2l(long double);
extern long double expm1l(long double);
extern long double logl(long double);
extern long double log10l(long double);
extern long double log2l(long double);
extern long double log1pl(long double);
extern long double logbl(long double);
extern long double modfl(long double, long double *);
extern long double ldexpl(long double, int);
extern long double frexpl(long double, int *);
extern int ilogbl(long double);
extern long double scalbnl(long double, int);
extern long double scalblnl(long double, long int);
extern long double fabsl(long double);
extern long double cbrtl(long double);
extern long double hypotl(long double, long double);
extern long double powl(long double, long double);
extern long double sqrtl(long double);
extern long double erfl(long double);
extern long double erfcl(long double);
extern long double lgammal(long double);

extern long double tgammal(long double);
extern long double ceill(long double);
extern long double floorl(long double);
extern long double nearbyintl(long double);
extern long double rintl(long double);
extern long int lrintl(long double);
extern long double roundl(long double);
extern long int lroundl(long double);

    extern long long int llrintl(long double);
    extern long long int llroundl(long double);
extern long double truncl(long double);
extern long double fmodl(long double, long double);
extern long double remainderl(long double, long double);
extern long double remquol(long double, long double, int *);
extern long double copysignl(long double, long double);
extern long double nanl(const char *);
extern long double nextafterl(long double, long double);
extern double nexttoward(double, long double);
extern float nexttowardf(float, long double);
extern long double nexttowardl(long double, long double);
extern long double fdiml(long double, long double);
extern long double fmaxl(long double, long double);
extern long double fminl(long double, long double);
extern long double fmal(long double, long double, long double);

extern double __inf( void );
extern float __inff( void );
extern long double __infl( void );
extern float __nan( void );
extern double bessel_j0 ( double );

extern double bessel_j1 ( double );

extern double bessel_jn ( int, double );

extern double bessel_y0 ( double );

extern double bessel_y1 ( double );

extern double bessel_yn ( int, double );

extern double scalb ( double, double );

extern int signgam;

extern long int rinttol ( double );

extern long int roundtol ( double );

extern int finite ( double );

extern double gamma ( double );

extern double significand ( double );
extern double drem ( double, double );

}

namespace std {

  template<typename _Tp> _Tp __cmath_power(_Tp, unsigned int);

  inline double
  abs(double __x)
  { return __builtin_fabs(__x); }

  inline float
  abs(float __x)
  { return __builtin_fabsf(__x); }

  inline long double
  abs(long double __x)
  { return __builtin_fabsl(__x); }

  using ::acos;

  inline float
  acos(float __x)
  { return __builtin_acosf(__x); }

  inline long double
  acos(long double __x)
  { return __builtin_acosl(__x); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    acos(_Tp __x)
    { return __builtin_acos(__x); }

  using ::asin;

  inline float
  asin(float __x)
  { return __builtin_asinf(__x); }

  inline long double
  asin(long double __x)
  { return __builtin_asinl(__x); }

  template<typename _Tp>
  inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
      double>::__type
    asin(_Tp __x)
    { return __builtin_asin(__x); }

  using ::atan;

  inline float
  atan(float __x)
  { return __builtin_atanf(__x); }

  inline long double
  atan(long double __x)
  { return __builtin_atanl(__x); }

  template<typename _Tp>
  inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
      double>::__type
    atan(_Tp __x)
    { return __builtin_atan(__x); }

  using ::atan2;

  inline float
  atan2(float __y, float __x)
  { return __builtin_atan2f(__y, __x); }

  inline long double
  atan2(long double __y, long double __x)
  { return __builtin_atan2l(__y, __x); }

  template<typename _Tp, typename _Up>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value
            && __is_integer<_Up>::__value,
        double>::__type
    atan2(_Tp __y, _Up __x)
    { return __builtin_atan2(__y, __x); }

  using ::ceil;

  inline float
  ceil(float __x)
  { return __builtin_ceilf(__x); }

  inline long double
  ceil(long double __x)
  { return __builtin_ceill(__x); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    ceil(_Tp __x)
    { return __builtin_ceil(__x); }

  using ::cos;

  inline float
  cos(float __x)
  { return __builtin_cosf(__x); }

  inline long double
  cos(long double __x)
  { return __builtin_cosl(__x); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    cos(_Tp __x)
    { return __builtin_cos(__x); }

  using ::cosh;

  inline float
  cosh(float __x)
  { return __builtin_coshf(__x); }

  inline long double
  cosh(long double __x)
  { return __builtin_coshl(__x); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    cosh(_Tp __x)
    { return __builtin_cosh(__x); }

  using ::exp;

  inline float
  exp(float __x)
  { return __builtin_expf(__x); }

  inline long double
  exp(long double __x)
  { return __builtin_expl(__x); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    exp(_Tp __x)
    { return __builtin_exp(__x); }

  using ::fabs;

  inline float
  fabs(float __x)
  { return __builtin_fabsf(__x); }

  inline long double
  fabs(long double __x)
  { return __builtin_fabsl(__x); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    fabs(_Tp __x)
    { return __builtin_fabs(__x); }

  using ::floor;

  inline float
  floor(float __x)
  { return __builtin_floorf(__x); }

  inline long double
  floor(long double __x)
  { return __builtin_floorl(__x); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    floor(_Tp __x)
    { return __builtin_floor(__x); }

  using ::fmod;

  inline float
  fmod(float __x, float __y)
  { return __builtin_fmodf(__x, __y); }

  inline long double
  fmod(long double __x, long double __y)
  { return __builtin_fmodl(__x, __y); }

  using ::frexp;

  inline float
  frexp(float __x, int* __exp)
  { return __builtin_frexpf(__x, __exp); }

  inline long double
  frexp(long double __x, int* __exp)
  { return __builtin_frexpl(__x, __exp); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    frexp(_Tp __x, int* __exp)
    { return __builtin_frexp(__x, __exp); }

  using ::ldexp;

  inline float
  ldexp(float __x, int __exp)
  { return __builtin_ldexpf(__x, __exp); }

  inline long double
  ldexp(long double __x, int __exp)
  { return __builtin_ldexpl(__x, __exp); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
  ldexp(_Tp __x, int __exp)
  { return __builtin_ldexp(__x, __exp); }

  using ::log;

  inline float
  log(float __x)
  { return __builtin_logf(__x); }

  inline long double
  log(long double __x)
  { return __builtin_logl(__x); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    log(_Tp __x)
    { return __builtin_log(__x); }

  using ::log10;

  inline float
  log10(float __x)
  { return __builtin_log10f(__x); }

  inline long double
  log10(long double __x)
  { return __builtin_log10l(__x); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    log10(_Tp __x)
    { return __builtin_log10(__x); }

  using ::modf;

  inline float
  modf(float __x, float* __iptr)
  { return __builtin_modff(__x, __iptr); }

  inline long double
  modf(long double __x, long double* __iptr)
  { return __builtin_modfl(__x, __iptr); }

  template<typename _Tp>
    inline _Tp
    __pow_helper(_Tp __x, int __n)
    {
      return __n < 0
        ? _Tp(1)/__cmath_power(__x, -__n)
        : __cmath_power(__x, __n);
    }

  using ::pow;

  inline float
  pow(float __x, float __y)
  { return __builtin_powf(__x, __y); }

  inline long double
  pow(long double __x, long double __y)
  { return __builtin_powl(__x, __y); }

  inline double
  pow(double __x, int __i)
  { return __builtin_powi(__x, __i); }

  inline float
  pow(float __x, int __n)
  { return __builtin_powif(__x, __n); }

  inline long double
  pow(long double __x, int __n)
  { return __builtin_powil(__x, __n); }

  using ::sin;

  inline float
  sin(float __x)
  { return __builtin_sinf(__x); }

  inline long double
  sin(long double __x)
  { return __builtin_sinl(__x); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    sin(_Tp __x)
    { return __builtin_sin(__x); }

  using ::sinh;

  inline float
  sinh(float __x)
  { return __builtin_sinhf(__x); }

  inline long double
  sinh(long double __x)
  { return __builtin_sinhl(__x); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    sinh(_Tp __x)
    { return __builtin_sinh(__x); }

  using ::sqrt;

  inline float
  sqrt(float __x)
  { return __builtin_sqrtf(__x); }

  inline long double
  sqrt(long double __x)
  { return __builtin_sqrtl(__x); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    sqrt(_Tp __x)
    { return __builtin_sqrt(__x); }

  using ::tan;

  inline float
  tan(float __x)
  { return __builtin_tanf(__x); }

  inline long double
  tan(long double __x)
  { return __builtin_tanl(__x); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    tan(_Tp __x)
    { return __builtin_tan(__x); }

  using ::tanh;

  inline float
  tanh(float __x)
  { return __builtin_tanhf(__x); }

  inline long double
  tanh(long double __x)
  { return __builtin_tanhl(__x); }

  template<typename _Tp>
    inline typename __gnu_cxx::__enable_if<__is_integer<_Tp>::__value,
        double>::__type
    tanh(_Tp __x)
    { return __builtin_tanh(__x); }

}

namespace __gnu_cxx {

  template<typename _Tp>
    inline int
    __capture_fpclassify(_Tp __f) { return ( sizeof (__f) == sizeof(float ) ? __fpclassifyf((float)(__f)) : sizeof (__f) == sizeof(double) ? __fpclassifyd((double)(__f)) : __fpclassify ((long double)(__f))); }

  template<typename _Tp>
    inline int
    __capture_isfinite(_Tp __f) { return ( sizeof (__f) == sizeof(float ) ? __inline_isfinitef((float)(__f)) : sizeof (__f) == sizeof(double) ? __inline_isfinited((double)(__f)) : __inline_isfinite ((long double)(__f))); }

  template<typename _Tp>
    inline int
    __capture_isinf(_Tp __f) { return ( sizeof (__f) == sizeof(float ) ? __inline_isinff((float)(__f)) : sizeof (__f) == sizeof(double) ? __inline_isinfd((double)(__f)) : __inline_isinf ((long double)(__f))); }

  template<typename _Tp>
    inline int
    __capture_isnan(_Tp __f) { return ( sizeof (__f) == sizeof(float ) ? __inline_isnanf((float)(__f)) : sizeof (__f) == sizeof(double) ? __inline_isnand((double)(__f)) : __inline_isnan ((long double)(__f))); }

  template<typename _Tp>
    inline int
    __capture_isnormal(_Tp __f) { return ( sizeof (__f) == sizeof(float ) ? __inline_isnormalf((float)(__f)) : sizeof (__f) == sizeof(double) ? __inline_isnormald((double)(__f)) : __inline_isnormal ((long double)(__f))); }

  template<typename _Tp>
    inline int
    __capture_signbit(_Tp __f) { return ( sizeof (__f) == sizeof(float ) ? __inline_signbitf((float)(__f)) : sizeof (__f) == sizeof(double) ? __inline_signbitd((double)(__f)) : __inline_signbit((long double)(__f))); }

  template<typename _Tp>
    inline int
    __capture_isgreater(_Tp __f1, _Tp __f2)
    { return __builtin_isgreater ((__f1),(__f2)); }

  template<typename _Tp>
    inline int
    __capture_isgreaterequal(_Tp __f1, _Tp __f2)
    { return __builtin_isgreaterequal ((__f1),(__f2)); }

  template<typename _Tp>
    inline int
    __capture_isless(_Tp __f1, _Tp __f2) { return __builtin_isless ((__f1),(__f2)); }

  template<typename _Tp>
    inline int
    __capture_islessequal(_Tp __f1, _Tp __f2)
    { return __builtin_islessequal ((__f1),(__f2)); }

  template<typename _Tp>
    inline int
    __capture_islessgreater(_Tp __f1, _Tp __f2)
    { return __builtin_islessgreater ((__f1),(__f2)); }

  template<typename _Tp>
    inline int
    __capture_isunordered(_Tp __f1, _Tp __f2)
    { return __builtin_isunordered ((__f1),(__f2)); }

}

namespace std {

  template<typename _Tp>
    inline int
    fpclassify(_Tp __f) { return ::__gnu_cxx::__capture_fpclassify(__f); }

  template<typename _Tp>
    inline int
    isfinite(_Tp __f) { return ::__gnu_cxx::__capture_isfinite(__f); }

  template<typename _Tp>
    inline int
    isinf(_Tp __f) { return ::__gnu_cxx::__capture_isinf(__f); }

  template<typename _Tp>
    inline int
    isnan(_Tp __f) { return ::__gnu_cxx::__capture_isnan(__f); }

  template<typename _Tp>
    inline int
    isnormal(_Tp __f) { return ::__gnu_cxx::__capture_isnormal(__f); }

  template<typename _Tp>
    inline int
    signbit(_Tp __f) { return ::__gnu_cxx::__capture_signbit(__f); }

  template<typename _Tp>
    inline int
    isgreater(_Tp __f1, _Tp __f2)
    { return ::__gnu_cxx::__capture_isgreater(__f1, __f2); }

  template<typename _Tp>
    inline int
    isgreaterequal(_Tp __f1, _Tp __f2)
    { return ::__gnu_cxx::__capture_isgreaterequal(__f1, __f2); }

  template<typename _Tp>
    inline int
    isless(_Tp __f1, _Tp __f2)
    { return ::__gnu_cxx::__capture_isless(__f1, __f2); }

  template<typename _Tp>
    inline int
    islessequal(_Tp __f1, _Tp __f2)
    { return ::__gnu_cxx::__capture_islessequal(__f1, __f2); }

  template<typename _Tp>
    inline int
    islessgreater(_Tp __f1, _Tp __f2)
    { return ::__gnu_cxx::__capture_islessgreater(__f1, __f2); }

  template<typename _Tp>
    inline int
    isunordered(_Tp __f1, _Tp __f2)
    { return ::__gnu_cxx::__capture_isunordered(__f1, __f2); }

}

namespace std {

  template<typename _Tp>
    inline _Tp
    __cmath_power(_Tp __x, unsigned int __n)
    {
      _Tp __y = __n % 2 ? __x : 1;

      while (__n >>= 1)
        {
          __x = __x * __x;
          if (__n % 2)
            __y = __y * __x;
        }

      return __y;
    }

}

const double pi = 3.14159265358979323846264338327950288;

extern double random(double n);
int random_set_seed(int seed);
int random_get_seed();
int randomize();
int random_integer(int x);
static inline int irandom(int x) {return int(random(x + 1));}
static inline double random_range(double x1, double x2) {return x1 + random(x2 - x1);}
static inline double irandom_range(int x1, int x2) {return x1 + irandom(x2 - x1);}
unsigned int random32();
double mtrandom();
int mtrandom_seed(int x);
double abs(const variant& x);
double ceil(const variant& x);
double floor(const variant& x);
double exp(const variant& x);
double sqrt(const variant& x);
double log10(const variant& x);
double sin(const variant& x);
double cos(const variant& x);
double tan(const variant& x);

double abs(const var& x);
double ceil(const var& x);
double floor(const var& x);
double exp(const var& x);
double sqrt(const var& x);
double log10(const var& x);
double sin(const var& x);
double cos(const var& x);
double tan(const var& x);

double sqr(double x);
double power(double x,double power);
double ln(double x);
double logn(double n,double x);

double arcsin(double x);
double arccos(double x);
double arctan(double x);
double arctan2(double y,double x);

double sind(double x);
double cosd(double x);
double tand(double x);

double asind(double x);
double acosd(double x);
double atand(double x);
double atand2(double y,double x);
double arcsind(double x);
double arccosd(double x);
double arctand(double x);
double arctand2(double y,double x);

int sign(double x);
int cmp(double x,double y);
double frac(double x);

double degtorad(double x);
double radtodeg(double x);

double lerp(double x, double y, double a);

double lengthdir_x(double len,double dir);
double lengthdir_y(double len,double dir);
double direction_difference(double dir1,double dir2);
double point_direction(double x1,double y1,double x2,double y2);
double point_distance(double x1, double y1, double x2, double y2);

double max(const enigma::varargs &t);
double min(const enigma::varargs &t);
double min(double x, double y);
double max(double x, double y);
double median(enigma::varargs t);
double mean(const enigma::varargs &t);
variant choose(const enigma::varargs& args);

bool is_string(variant val);
bool is_real(variant val);

string chr(char val);
int ord(string str);

double real(variant str);

size_t string_length(string str);
size_t string_length(const char* str);
int string_pos(string substr,string str);

string string_format(double val, unsigned tot, unsigned dec);
string string_copy(string str,int index,int count);
string string_char_at(string str,int index);
string string_delete(string str,int index,int count);
string string_insert(string substr,string str,int index);
string string_replace(string str,string substr,string newstr);
string string_replace_all(string str,string substr,string newstr);
int string_count(string substr,string str);

string string_lower(string str);
string string_upper(string str);

string string_repeat(string str,int count);

string string_letters(string str);
string string_digits(string str);
string string_lettersdigits(string str);

bool string_isletters(string str);
bool string_isdigits(string str);
bool string_islettersdigits(string str);

string filename_name(string fname);
string filename_path(string fname);
string filename_dir(string fname);
string filename_drive(string fname);
string filename_ext(string fname);
string filename_change_ext(string fname, string newext);

int file_text_open_read(string fname);
int file_text_open_write(string fname);
int file_text_open_append(string fname);
void file_text_close(int fileid);
void file_text_write_string(int fileid, string str);
void file_text_write_real(int fileid, double x);
void file_text_writeln(int fileid);
string file_text_read_string(int fileid);
double file_text_read_real(int fileid);
void file_text_readln(int fileid);
bool file_text_eof(int fileid);
bool file_text_eoln(int fileid);

int file_bin_open(string fname,int mode);
bool file_bin_rewrite(int fileid);
void file_bin_close(int fileid);
size_t file_bin_size(int fileid);
size_t file_bin_position(int fileid);
void file_bin_seek(int fileid,size_t pos);
void file_bin_write_byte(int fileid,unsigned char byte);
int file_bin_read_byte(int fileid);

void cons_show_message(string message);
void cons_print_overwritable(string message);
void show_debug_message(string message);
char cons_get_byte();
string cons_get_char();
string cons_get_string();
struct bbox_rect_t {
  int bottom, left, right, top;
};
namespace enigma
{
  struct sprite
  {
    int width,height,subcount,xoffset,yoffset,id;
    unsigned int *texturearray;
    void **colldata;
    double texbordx, texbordy;
    bbox_rect_t bbox, bbox_relative;
    bool where,smooth;

    sprite();
    sprite(unsigned int);
  };
  extern sprite** spritestructarray;
  extern size_t sprite_idmax;
}

namespace enigma
{

  int sprite_new_empty(unsigned sprid, unsigned subc, int w, int h, int x, int y, int bbt, int bbb, int bbl, int bbr, bool pl, bool sm);
  void sprite_set_subimage(int sprid, int imgindex, int x, int y, unsigned int w,unsigned int h,unsigned char*chunk);

  void spritestructarray_reallocate();
}

extern int sprite_get_width (int sprite);
extern int sprite_get_height (int sprite);

extern int sprite_get_bbox_bottom (int sprite);
extern int sprite_get_bbox_left (int sprite);
extern int sprite_get_bbox_right (int sprite);
extern int sprite_get_bbox_top (int sprite);
extern int sprite_get_bbox_mode (int sprite);

extern int sprite_get_bbox_bottom_relative (int sprite);
extern int sprite_get_bbox_left_relative (int sprite);
extern int sprite_get_bbox_right_relative (int sprite);
extern int sprite_get_bbox_top_relative (int sprite);

extern const bbox_rect_t &sprite_get_bbox(int sprite);
extern const bbox_rect_t &sprite_get_bbox_relative(int sprite);

extern int sprite_get_number (int sprite);
extern int sprite_get_texture (int sprite, int subimage);
extern int sprite_get_xoffset (int sprite);
extern int sprite_get_yoffset (int sprite);
int sprite_add(std::string filename, int imgnumb, bool precise, bool transparent, bool smooth, bool preload, int x_offset, int y_offset);
int sprite_add(std::string filename, int imgnumb, bool transparent, bool smooth, int x_offset, int y_offset);

namespace enigma
{
  struct fontglyph
  {
    int x, y, x2, y2;
    float tx, ty, tx2, ty2;
    float xs;
  };
  struct font
  {

    string name, fontname;
    int fontsize; bool bold, italic;
    unsigned char glyphstart, glyphcount;
    fontglyph *glyphs;
    unsigned int height, yoffset;
    unsigned int texture;
    int twid, thgt;
  };
  struct rawfont {
    string name;
    int id;

    string fontname;
    int fontsize; bool bold, italic;
    unsigned char glyphstart, glyphcount;
  };
  extern rawfont rawfontdata[];
  extern font **fontstructarray;

  extern int rawfontcount, rawfontmaxid;
  int font_new(unsigned char gs, unsigned char gc);
}

int font_add_sprite(int spr, unsigned char first, bool prop, int sep);

extern void ABORT_ON_ALL_ERRORS();

void ABORT_ON_ALL_ERRORS() { }
const int self = -1;
const int other = -2;
const int all = -3;
const int noone = -4;
const int global = -5;
const int local = -7;
namespace enigma
{
    extern int maxid;
    extern int instancecount;
    extern int id_current;

    extern double newinst_x, newinst_y;
    extern int newinst_obj, newinst_id;

    struct object_basic
    {
      const unsigned id;
      const int object_index;

      virtual void unlink();
      virtual void deactivate();
      virtual void activate();
      virtual variant myevent_create();
      virtual variant myevent_gamestart();
      virtual variant myevent_draw();
      virtual variant myevent_roomend();
      virtual variant myevent_destroy();

      object_basic();
      object_basic(int uid, int uoid);
      virtual ~object_basic();
    };
}

namespace enigma
{
    void constructor(object_basic* instance);
}

namespace enigma
{
  struct multifunction_variant: variant
  {

    virtual void function(variant oldvalue);
    multifunction_variant& operator=(multifunction_variant&);
    multifunction_variant& operator= (int x) ; multifunction_variant& operator= (long x) ; multifunction_variant& operator= (unsigned long x) ; multifunction_variant& operator= (unsigned x) ; multifunction_variant& operator= (long long x) ; multifunction_variant& operator= (unsigned long long x) ; multifunction_variant& operator= (float x) ; multifunction_variant& operator= (double x) ; multifunction_variant& operator= (const char *x) ; multifunction_variant& operator= (string x) ; multifunction_variant& operator= (const variant &x); multifunction_variant& operator= (const var &x);;
    multifunction_variant& operator+= (int x) ; multifunction_variant& operator+= (long x) ; multifunction_variant& operator+= (unsigned long x) ; multifunction_variant& operator+= (unsigned x) ; multifunction_variant& operator+= (long long x) ; multifunction_variant& operator+= (unsigned long long x) ; multifunction_variant& operator+= (float x) ; multifunction_variant& operator+= (double x) ; multifunction_variant& operator+= (const char *x) ; multifunction_variant& operator+= (string x) ; multifunction_variant& operator+= (const variant &x); multifunction_variant& operator+= (const var &x);;
    multifunction_variant& operator-= (int x) ; multifunction_variant& operator-= (long x) ; multifunction_variant& operator-= (unsigned long x) ; multifunction_variant& operator-= (unsigned x) ; multifunction_variant& operator-= (long long x) ; multifunction_variant& operator-= (unsigned long long x) ; multifunction_variant& operator-= (float x) ; multifunction_variant& operator-= (double x) ; multifunction_variant& operator-= (const char *x) ; multifunction_variant& operator-= (string x) ; multifunction_variant& operator-= (const variant &x); multifunction_variant& operator-= (const var &x);;
    multifunction_variant& operator*= (int x) ; multifunction_variant& operator*= (long x) ; multifunction_variant& operator*= (unsigned long x) ; multifunction_variant& operator*= (unsigned x) ; multifunction_variant& operator*= (long long x) ; multifunction_variant& operator*= (unsigned long long x) ; multifunction_variant& operator*= (float x) ; multifunction_variant& operator*= (double x) ; multifunction_variant& operator*= (const char *x) ; multifunction_variant& operator*= (string x) ; multifunction_variant& operator*= (const variant &x); multifunction_variant& operator*= (const var &x);;
    multifunction_variant& operator/= (int x) ; multifunction_variant& operator/= (long x) ; multifunction_variant& operator/= (unsigned long x) ; multifunction_variant& operator/= (unsigned x) ; multifunction_variant& operator/= (long long x) ; multifunction_variant& operator/= (unsigned long long x) ; multifunction_variant& operator/= (float x) ; multifunction_variant& operator/= (double x) ; multifunction_variant& operator/= (const char *x) ; multifunction_variant& operator/= (string x) ; multifunction_variant& operator/= (const variant &x); multifunction_variant& operator/= (const var &x);;
    multifunction_variant& operator%= (int x) ; multifunction_variant& operator%= (long x) ; multifunction_variant& operator%= (unsigned long x) ; multifunction_variant& operator%= (unsigned x) ; multifunction_variant& operator%= (long long x) ; multifunction_variant& operator%= (unsigned long long x) ; multifunction_variant& operator%= (float x) ; multifunction_variant& operator%= (double x) ; multifunction_variant& operator%= (const char *x) ; multifunction_variant& operator%= (string x) ; multifunction_variant& operator%= (const variant &x); multifunction_variant& operator%= (const var &x);;
    multifunction_variant& operator<<= (int x) ; multifunction_variant& operator<<= (long x) ; multifunction_variant& operator<<= (unsigned long x) ; multifunction_variant& operator<<= (unsigned x) ; multifunction_variant& operator<<= (long long x) ; multifunction_variant& operator<<= (unsigned long long x) ; multifunction_variant& operator<<= (float x) ; multifunction_variant& operator<<= (double x) ; multifunction_variant& operator<<= (const char *x) ; multifunction_variant& operator<<= (string x) ; multifunction_variant& operator<<= (const variant &x); multifunction_variant& operator<<= (const var &x);;
    multifunction_variant& operator>>= (int x) ; multifunction_variant& operator>>= (long x) ; multifunction_variant& operator>>= (unsigned long x) ; multifunction_variant& operator>>= (unsigned x) ; multifunction_variant& operator>>= (long long x) ; multifunction_variant& operator>>= (unsigned long long x) ; multifunction_variant& operator>>= (float x) ; multifunction_variant& operator>>= (double x) ; multifunction_variant& operator>>= (const char *x) ; multifunction_variant& operator>>= (string x) ; multifunction_variant& operator>>= (const variant &x); multifunction_variant& operator>>= (const var &x);;
    multifunction_variant& operator&= (int x) ; multifunction_variant& operator&= (long x) ; multifunction_variant& operator&= (unsigned long x) ; multifunction_variant& operator&= (unsigned x) ; multifunction_variant& operator&= (long long x) ; multifunction_variant& operator&= (unsigned long long x) ; multifunction_variant& operator&= (float x) ; multifunction_variant& operator&= (double x) ; multifunction_variant& operator&= (const char *x) ; multifunction_variant& operator&= (string x) ; multifunction_variant& operator&= (const variant &x); multifunction_variant& operator&= (const var &x);;
    multifunction_variant& operator|= (int x) ; multifunction_variant& operator|= (long x) ; multifunction_variant& operator|= (unsigned long x) ; multifunction_variant& operator|= (unsigned x) ; multifunction_variant& operator|= (long long x) ; multifunction_variant& operator|= (unsigned long long x) ; multifunction_variant& operator|= (float x) ; multifunction_variant& operator|= (double x) ; multifunction_variant& operator|= (const char *x) ; multifunction_variant& operator|= (string x) ; multifunction_variant& operator|= (const variant &x); multifunction_variant& operator|= (const var &x);;
    multifunction_variant& operator^= (int x) ; multifunction_variant& operator^= (long x) ; multifunction_variant& operator^= (unsigned long x) ; multifunction_variant& operator^= (unsigned x) ; multifunction_variant& operator^= (long long x) ; multifunction_variant& operator^= (unsigned long long x) ; multifunction_variant& operator^= (float x) ; multifunction_variant& operator^= (double x) ; multifunction_variant& operator^= (const char *x) ; multifunction_variant& operator^= (string x) ; multifunction_variant& operator^= (const variant &x); multifunction_variant& operator^= (const var &x);;

    multifunction_variant();
    multifunction_variant (int x) ; multifunction_variant (long x) ; multifunction_variant (unsigned long x) ; multifunction_variant (unsigned x) ; multifunction_variant (long long x) ; multifunction_variant (unsigned long long x) ; multifunction_variant (float x) ; multifunction_variant (double x) ; multifunction_variant (const char *x) ; multifunction_variant (string x) ; multifunction_variant (const variant &x) ; multifunction_variant (const var &x) ;;
    virtual ~multifunction_variant();

  };
}
namespace enigma {
  struct directionv: multifunction_variant {
    using multifunction_variant::operator=; directionv &operator=(directionv&);;
    double *spd, *hspd, *vspd;
    void function(variant oldval);
  };

  struct speedv: multifunction_variant {
    using multifunction_variant::operator=; speedv &operator=(speedv&);;
    double *dir, *hspd, *vspd;
    void function(variant oldval);
  };

  struct hspeedv: multifunction_variant {
    using multifunction_variant::operator=; hspeedv &operator=(hspeedv&);;
    double *vspd, *dir, *spd;
    void function(variant oldval);
  };

  struct vspeedv: multifunction_variant {
    using multifunction_variant::operator=; vspeedv &operator=(vspeedv&);;
    double *hspd, *dir, *spd;
    void function(variant oldval);
  };
}
namespace enigma
{
  struct object_planar: object_basic
  {

      double x, y;
      double xprevious, yprevious;
      double xstart, ystart;

      bool persistent;

      directionv direction;
      speedv speed;
      hspeedv hspeed;
      vspeedv vspeed;
      double gravity;
      double gravity_direction;
      double friction;
      object_planar();
      object_planar(unsigned, int);
      virtual ~object_planar();
  };

  void propagate_locals(object_planar*);
}

namespace enigma
{
  struct depthv: multifunction_variant {
    using multifunction_variant::operator=; depthv &operator=(depthv&);;
    struct inst_iter *myiter;
    void function(variant oldval);
    void init(double depth, object_basic* who);
    void remove();
    ~depthv();
  };
  struct object_graphics: object_planar
  {

      int sprite_index;
      double image_index;
      double image_speed;

      int image_number;
      enigma::depthv depth;
      bool visible;
      double image_xscale;
      double image_yscale;
      double image_angle;

        int sprite_width, sprite_height;
        int sprite_xoffset, sprite_yoffset;

      object_graphics();
      object_graphics(unsigned x, int y);
      virtual ~object_graphics();
  };
}
namespace enigma
{
  struct object_transform: object_graphics
  {

        double image_alpha;
        int image_blend;
        int image_single;

      object_transform();
      object_transform(unsigned x, int y);
      virtual ~object_transform();
  };
}
struct bbox_rect_t;
namespace enigma
{
  struct object_collisions: object_transform
  {

      int mask_index;
      bool solid;

        int bbox_left, bbox_right, bbox_top, bbox_bottom;

      object_collisions();
      object_collisions(unsigned, int);
      virtual ~object_collisions();
  };
}
namespace enigma
{

  void *collisionsystem_sprite_data_create(char*,int,int,int,int);

    object_basic *place_meeting_inst(double x, double y, int object);

}

namespace enigma
{

  void graphicssystem_initialize();
  void sprite_safety_override();

  void sprites_init();
  unsigned graphics_create_texture(int fullwidth, int fullheight, void* pxdata);
  unsigned char* graphics_get_texture_rgba(unsigned texture);
}
std::string draw_get_graphics_error();
void screen_refresh();
void screen_redraw();

void screen_init();

namespace enigma
{

  bool widget_system_initialize();
}

void show_error(std::string msg, const bool fatal);

namespace enigma
{

  void windowsystem_write_exename(char* exenamehere);
  void sleep_for_framerate(int framerate);
  void input_initialize();
}

void window_set_caption(std::string caption);
void window_set_size(unsigned int width, unsigned int height);
int window_mouse_get_x();
int window_mouse_get_y();
int window_get_width();
int window_get_height();

int file_exists(std::string fname);
int file_delete(std::string fname);
int file_rename(std::string oldname,std::string newname);
int file_copy(std::string fname,std::string newname);
int directory_exists(std::string dname);
int directory_create(std::string dname);

std::string file_find_first(std::string mask,int attr);

enum {
  fa_readonly = 1,
  fa_hidden = 2,
  fa_sysfile = 4,
  fa_volumeid = 8,
  fa_directory = 16,
  fa_archive = 32
};

std::string file_find_next();
void file_find_close();
bool file_attributes(std::string fname,int attr);

void export_include_file(std::string fname);
void export_include_file_location(std::string fname,std::string location);
void discard_include_file(std::string fname);

extern unsigned game_id;
extern std::string working_directory;
extern std::string program_directory;
extern std::string temp_directory;
int parameter_count();
std::string parameter_string(int n);

std::string environment_get_variable(std::string name);
using std::string;

void gmw_init();

void Sleep(int ms);

void window_set_position(int x,int y);

int window_set_visible(int visible);
int window_get_visible();
void window_set_caption(std::string caption);
char *window_get_caption();
int display_mouse_get_x();
int display_mouse_get_y();
int window_mouse_get_x();
int window_mouse_get_y();

int window_mouse_set(double x,double y);
int display_mouse_set(double x,double y);

int window_get_x();
int window_get_y();
int window_get_width();
int window_get_height();
void window_set_size(unsigned int w,unsigned int h);
int window_set_rectangle(double x,double y,double w,double h);
int window_center();
int window_set_fullscreen(const bool full);
int window_get_fullscreen();

int window_set_cursor(double c);

void window_set_showicons(bool show);
void window_set_color(int color);

int window_view_mouse_get_x(int wid);
int window_view_mouse_get_y(int wid);
void window_view_mouse_set(int wid, int x, int y);
int window_views_mouse_get_x();
int window_views_mouse_get_y();
void window_views_mouse_set(int x, int y);

int window_get_region_width();
int window_get_region_height();

int show_message(string str);

void io_handle();
void io_clear();
void keyboard_wait();

namespace enigma {
 extern char** parameters;

    long int current_time();
}

void game_end();
void action_end_game();

namespace enigma
{
  extern unsigned bound_texture;
  extern unsigned char currentcolor[4];
  extern bool glew_isgo;
  extern bool pbo_isgo;
}

enum {
  c_aqua = 0xFFFF00,
  c_black = 0x000000,
  c_blue = 0xFF0000,
  c_dkgray = 0x404040,
  c_fuchsia = 0xFF00FF,
  c_gray = 0x808080,
  c_green = 0x008000,
  c_lime = 0x00FF00,
  c_ltgray = 0xC0C0C0,
  c_maroon = 0x000080,
  c_navy = 0x800000,
  c_olive = 0x008080,
  c_orange = 0x0080FF,
  c_purple = 0x800080,
  c_red = 0x0000FF,
  c_silver = 0xC0C0C0,
  c_teal = 0x808000,
  c_white = 0xFFFFFF,
  c_yellow = 0x00FFFF
};

void draw_clear(int color);
void draw_clear_alpha(int color, float alpha);

int merge_color(int col1, int col2, double amount);

void draw_set_color(int col);
void draw_set_color_rgb(unsigned char red, unsigned char green, unsigned char blue);
void draw_set_alpha(float alpha);
void draw_set_color_rgba(unsigned char red, unsigned char green, unsigned char blue, float alpha);
int draw_get_color();
int draw_get_red();
int draw_get_green();
int draw_get_blue();
float draw_get_alpha();
int make_color_rgb(unsigned char red, unsigned char green, unsigned char blue);
int (*const make_color)(unsigned char,unsigned char,unsigned char) = make_color_rgb;

int color_get_red(int color);
int color_get_green(int color);
int color_get_blue(int color);

int color_get_hue(int color);
int color_get_value(int color);
int color_get_saturation(int color);

int make_color_hsv(int hue,int saturation,int value);

void draw_unbind_all();

const int pr_pointlist =1;
const int pr_linelist =2;
const int pr_linestrip =3;
const int pr_trianglelist =4;
const int pr_trianglestrip =5;
const int pr_trianglefan =6;

const int pr_lineloop =7;
const int pr_quadlist =8;
const int pr_quadstrip =9;
const int pr_polygon =10;

const int pr_aa_nicest =4354;
const int pr_aa_fastest =4353;
const int pr_aa_dontcare =4352;

void draw_set_primitive_aa(bool enable, int quality);
int draw_primitive_begin(int kind);
int draw_primitive_begin_texture(int dink,unsigned tex);
int draw_vertex(double x, double y);
int draw_vertex_color(float x, float y, int color, float alpha);
int draw_vertex_texture(float x, float y, float tx, float ty);
int draw_vertex_texture_color(float x, float y, float tx, float ty, int col, float alpha);

int draw_primitive_end();

extern bool d3dMode;
extern double projection_matrix[16];
extern double transformation_matrix[16];

void d3d_start();
void d3d_end();
bool d3d_get_mode();
void d3d_set_perspective(bool enable);
void d3d_set_hidden(bool enable);
void d3d_set_lighting(bool enable);
void d3d_set_culling(bool enable);
void d3d_set_fog(bool enable, int color, double start, double end);
void d3d_set_depth(double dep);
void d3d_set_shading(bool smooth);

void d3d_primitive_begin(int kind);
void d3d_primitive_begin_texture(int kind, int texId);
void d3d_primitive_end();
void d3d_vertex(double x, double y, double z);
void d3d_vertex_color(double x, double y, double z, int color, double alpha);
void d3d_vertex_texture(double x, double y, double z, double tx, double ty);
void d3d_vertex_texture_color(double x, double y, double z, double tx, double ty, int color, double alpha);
void d3d_vertex_normal(double x, double y, double z, double nx, double ny, double nz);
void d3d_vertex_normal_color(double x, double y, double z, double nx, double ny, double nz, int color, double alpha);
void d3d_vertex_normal_texture(double x, double y, double z, double nx, double ny, double nz, double tx, double ty);
void d3d_vertex_normal_texture_color(double x, double y, double z, double nx, double ny, double nz, double tx, double ty, int color, double alpha);
void d3d_set_projection(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup);
void d3d_set_projection_ext(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup,double angle,double aspect,double znear,double zfar);
void d3d_set_projection_ortho(double x, double y, double width, double height, double angle);
void d3d_set_projection_perspective(double x, double y, double width, double height, double angle);

void d3d_draw_wall(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep);
void d3d_draw_floor(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep);
void d3d_draw_block(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, bool closed = true);
void d3d_draw_cylinder(double x1, double y1, double z1, double x2, double y2, double z2, int texid, int hrep, int vrep, bool closed, int steps);
void d3d_draw_cone(double x1, double y1, double z1, double x2, double y2, double z2, int texid, int hrep, int vrep, bool closed, int steps);
void d3d_draw_ellipsoid(double x1, double y1, double z1, double x2, double y2, double z2, int texid, int hrep, int vrep, int steps);

void d3d_transform_set_identity();
void d3d_transform_add_translation(double xt,double yt,double zt);
void d3d_transform_add_scaling(double xs,double ys,double zs);
void d3d_transform_add_rotation_x(double angle);
void d3d_transform_add_rotation_y(double angle);
void d3d_transform_add_rotation_z(double angle);
void d3d_transform_add_rotation_axis(double x, double y, double z, double angle);
void d3d_transform_set_translation(double xt,double yt,double zt);
void d3d_transform_set_scaling(double xs,double ys,double zs);
void d3d_transform_set_rotation_x(double angle);
void d3d_transform_set_rotation_y(double angle);
void d3d_transform_set_rotation_z(double angle);
void d3d_transform_set_rotation_axis(double x, double y, double z, double angle);
bool d3d_transform_stack_push();
bool d3d_transform_stack_pop();
void d3d_transform_stack_clear();
bool d3d_transform_stack_empty();
bool d3d_transform_stack_top();
bool d3d_transform_stack_disgard();

bool d3d_light_define_direction(int id, double dx, double dy, double dz, int col);
bool d3d_light_define_point(int id, double x, double y, double z, double range, int col);
bool d3d_light_enable(int id, bool enable);

unsigned int d3d_model_create();
void d3d_model_destroy(const unsigned int id);
void d3d_model_copy(const unsigned int id, const unsigned int source);
unsigned int d3d_model_duplicate(const unsigned int source);
bool d3d_model_exists(const unsigned int id);
void d3d_model_clear(const unsigned int id);
void d3d_model_save(const unsigned int id, std::string fname);
bool d3d_model_load(const unsigned int id, std::string fname);
void d3d_model_draw(const unsigned int id, double x, double y, double z, int texId);
void d3d_model_primitive_begin(const unsigned int id, int kind);
void d3d_model_primitive_end(const unsigned int id);
void d3d_model_vertex(const unsigned int id, double x, double y, double z);
void d3d_model_vertex_color(const unsigned int id, double x, double y, double z, int col, double alpha);
void d3d_model_vertex_texture(const unsigned int id, double x, double y, double z, double tx, double ty);
void d3d_model_vertex_texture_color(const unsigned int id, double x, double y, double z, double tx, double ty, int col, double alpha);
void d3d_model_vertex_normal(const unsigned int id, double x, double y, double z, double nx, double ny, double nz);
void d3d_model_vertex_normal_color(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, int col, double alpha);
void d3d_model_vertex_normal_texture(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, double tx, double ty);
void d3d_model_vertex_normal_texture_color(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, double tx, double ty, int col, double alpha);
void d3d_model_block(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed = true);
void d3d_model_cylinder(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps);
void d3d_model_cone(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps);
void d3d_model_ellipsoid(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, int steps);
void d3d_model_wall(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep);
void d3d_model_floor(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep);

void draw_set_line_pattern(unsigned short pattern, int scale);
void draw_point(float x, float y);
void draw_point_color(float x, float y,int color);
void draw_line(float x1,float y1,float x2,float y2);
void draw_line_width(float x1,float y1,float x2,float y2,float width);
void draw_line_color(float x1,float y1,float x2,float y2,int color1,int color2);
void draw_line_width_color(float x1,float y1,float x2,float y2,float width,int color1,int color2);
void draw_rectangle(float x1,float y1,float x2,float y2,bool outline);
void draw_rectangle_angle(float x1,float y1,float x2,float y2,float angle,bool outline);
void draw_rectangle_color(float x1,float y1,float x2,float y2,int color1,int color2,int color3,int color4,bool outline = false);
void draw_set_circle_precision(float precision);
float draw_get_circle_precision(void);
void draw_circle(float x, float y, float radius, bool outline);
void draw_circle_color(float x, float y, float radius, int color1, int color2, bool outline = false);
void draw_circle_perfect(float x, float y, float radius, bool outline);
void draw_circle_color_perfect(float x, float y, float radius, int color1, int color2, bool outline = false);
void draw_ellipse(float x1,float y1,float x2,float y2,bool outline);
void draw_ellipse_color(float x1, float y1, float x2, float y2, int col1, int col2, bool outline = false);
void draw_ellipse_perfect(float x1,float y1,float x2,float y2,bool outline);
void draw_triangle(float x1,float y1,float x2,float y2,float x3,float y3,bool outline);
void draw_triangle_color(float x1,float y1,float x2,float y2,float x3,float y3,int col1,int col2,int col3,bool outline = false);
void draw_roundrect(float x1,float y1,float x2,float y2, float r, bool outline);
void draw_roundrect_color(float x1, float y1, float x2, float y2, float r, int col1, int col2, bool outline = false);
void draw_healthbar(float x1, float y1, float x2, float y2, float amount, int backcol, int mincol, int maxcol, int direction, bool showback, bool showborder);
void draw_mandelbrot(float x, float y, float w, double Zx, double Zy, double Zw, unsigned iter);
void draw_arrow(float x1, float y1, float x2, float y2, float arrow_size, float line_size = 1, bool outline = false);
void draw_button(float x1, float y1, float x2, float y2, float border_width, bool up);
int draw_mandelbrot(int x,int y,float w,double Zx,double Zy,double Zw,unsigned iter);
int draw_getpixel(int,int);

enum {
  bm_normal = 0,
  bm_add = 1,
  bm_max = 2,
  bm_subtract = 3
};
enum {
  bm_zero = 1,
  bm_one = 2,
  bm_src_color = 3,
  bm_inv_src_color = 4,
  bm_src_alpha = 5,
  bm_inv_src_alpha = 6,
  bm_dest_alpha = 7,
  bm_inv_dest_alpha = 8,
  bm_dest_color = 9,
  bm_inv_dest_color = 10,
  bm_src_alpha_sat = 11
};

int draw_set_blend_mode(int mode);
int draw_set_blend_mode_ext(double src,double dest);
extern "C" {

typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

typedef int64_t GLint64EXT;
typedef uint64_t GLuint64EXT;

typedef GLint64EXT GLint64;
typedef GLuint64EXT GLuint64;
typedef struct __GLsync *GLsync;

typedef char GLchar;

extern void glAccum (GLenum op, GLfloat value);
extern void glAlphaFunc (GLenum func, GLclampf ref);
extern GLboolean glAreTexturesResident (GLsizei n, const GLuint *textures, GLboolean *residences);
extern void glArrayElement (GLint i);
extern void glBegin (GLenum mode);
extern void glBindTexture (GLenum target, GLuint texture);
extern void glBitmap (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
extern void glBlendFunc (GLenum sfactor, GLenum dfactor);
extern void glCallList (GLuint list);
extern void glCallLists (GLsizei n, GLenum type, const GLvoid *lists);
extern void glClear (GLbitfield mask);
extern void glClearAccum (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern void glClearDepth (GLclampd depth);
extern void glClearIndex (GLfloat c);
extern void glClearStencil (GLint s);
extern void glClipPlane (GLenum plane, const GLdouble *equation);
extern void glColor3b (GLbyte red, GLbyte green, GLbyte blue);
extern void glColor3bv (const GLbyte *v);
extern void glColor3d (GLdouble red, GLdouble green, GLdouble blue);
extern void glColor3dv (const GLdouble *v);
extern void glColor3f (GLfloat red, GLfloat green, GLfloat blue);
extern void glColor3fv (const GLfloat *v);
extern void glColor3i (GLint red, GLint green, GLint blue);
extern void glColor3iv (const GLint *v);
extern void glColor3s (GLshort red, GLshort green, GLshort blue);
extern void glColor3sv (const GLshort *v);
extern void glColor3ub (GLubyte red, GLubyte green, GLubyte blue);
extern void glColor3ubv (const GLubyte *v);
extern void glColor3ui (GLuint red, GLuint green, GLuint blue);
extern void glColor3uiv (const GLuint *v);
extern void glColor3us (GLushort red, GLushort green, GLushort blue);
extern void glColor3usv (const GLushort *v);
extern void glColor4b (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
extern void glColor4bv (const GLbyte *v);
extern void glColor4d (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
extern void glColor4dv (const GLdouble *v);
extern void glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void glColor4fv (const GLfloat *v);
extern void glColor4i (GLint red, GLint green, GLint blue, GLint alpha);
extern void glColor4iv (const GLint *v);
extern void glColor4s (GLshort red, GLshort green, GLshort blue, GLshort alpha);
extern void glColor4sv (const GLshort *v);
extern void glColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
extern void glColor4ubv (const GLubyte *v);
extern void glColor4ui (GLuint red, GLuint green, GLuint blue, GLuint alpha);
extern void glColor4uiv (const GLuint *v);
extern void glColor4us (GLushort red, GLushort green, GLushort blue, GLushort alpha);
extern void glColor4usv (const GLushort *v);
extern void glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
extern void glColorMaterial (GLenum face, GLenum mode);
extern void glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glCopyPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
extern void glCopyTexImage1D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
extern void glCopyTexImage2D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern void glCopyTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern void glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern void glCullFace (GLenum mode);
extern void glDeleteLists (GLuint list, GLsizei range);
extern void glDeleteTextures (GLsizei n, const GLuint *textures);
extern void glDepthFunc (GLenum func);
extern void glDepthMask (GLboolean flag);
extern void glDepthRange (GLclampd zNear, GLclampd zFar);
extern void glDisable (GLenum cap);
extern void glDisableClientState (GLenum array);
extern void glDrawArrays (GLenum mode, GLint first, GLsizei count);
extern void glDrawBuffer (GLenum mode);
extern void glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
extern void glDrawPixels (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
extern void glEdgeFlag (GLboolean flag);
extern void glEdgeFlagPointer (GLsizei stride, const GLvoid *pointer);
extern void glEdgeFlagv (const GLboolean *flag);
extern void glEnable (GLenum cap);
extern void glEnableClientState (GLenum array);
extern void glEnd (void);
extern void glEndList (void);
extern void glEvalCoord1d (GLdouble u);
extern void glEvalCoord1dv (const GLdouble *u);
extern void glEvalCoord1f (GLfloat u);
extern void glEvalCoord1fv (const GLfloat *u);
extern void glEvalCoord2d (GLdouble u, GLdouble v);
extern void glEvalCoord2dv (const GLdouble *u);
extern void glEvalCoord2f (GLfloat u, GLfloat v);
extern void glEvalCoord2fv (const GLfloat *u);
extern void glEvalMesh1 (GLenum mode, GLint i1, GLint i2);
extern void glEvalMesh2 (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
extern void glEvalPoint1 (GLint i);
extern void glEvalPoint2 (GLint i, GLint j);
extern void glFeedbackBuffer (GLsizei size, GLenum type, GLfloat *buffer);
extern void glFinish (void);
extern void glFlush (void);
extern void glFogf (GLenum pname, GLfloat param);
extern void glFogfv (GLenum pname, const GLfloat *params);
extern void glFogi (GLenum pname, GLint param);
extern void glFogiv (GLenum pname, const GLint *params);
extern void glFrontFace (GLenum mode);
extern void glFrustum (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern GLuint glGenLists (GLsizei range);
extern void glGenTextures (GLsizei n, GLuint *textures);
extern void glGetBooleanv (GLenum pname, GLboolean *params);
extern void glGetClipPlane (GLenum plane, GLdouble *equation);
extern void glGetDoublev (GLenum pname, GLdouble *params);
extern GLenum glGetError (void);
extern void glGetFloatv (GLenum pname, GLfloat *params);
extern void glGetIntegerv (GLenum pname, GLint *params);
extern void glGetLightfv (GLenum light, GLenum pname, GLfloat *params);
extern void glGetLightiv (GLenum light, GLenum pname, GLint *params);
extern void glGetMapdv (GLenum target, GLenum query, GLdouble *v);
extern void glGetMapfv (GLenum target, GLenum query, GLfloat *v);
extern void glGetMapiv (GLenum target, GLenum query, GLint *v);
extern void glGetMaterialfv (GLenum face, GLenum pname, GLfloat *params);
extern void glGetMaterialiv (GLenum face, GLenum pname, GLint *params);
extern void glGetPixelMapfv (GLenum map, GLfloat *values);
extern void glGetPixelMapuiv (GLenum map, GLuint *values);
extern void glGetPixelMapusv (GLenum map, GLushort *values);
extern void glGetPointerv (GLenum pname, GLvoid* *params);
extern void glGetPolygonStipple (GLubyte *mask);
extern const GLubyte * glGetString (GLenum name);
extern void glGetTexEnvfv (GLenum target, GLenum pname, GLfloat *params);
extern void glGetTexEnviv (GLenum target, GLenum pname, GLint *params);
extern void glGetTexGendv (GLenum coord, GLenum pname, GLdouble *params);
extern void glGetTexGenfv (GLenum coord, GLenum pname, GLfloat *params);
extern void glGetTexGeniv (GLenum coord, GLenum pname, GLint *params);
extern void glGetTexImage (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
extern void glGetTexLevelParameterfv (GLenum target, GLint level, GLenum pname, GLfloat *params);
extern void glGetTexLevelParameteriv (GLenum target, GLint level, GLenum pname, GLint *params);
extern void glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params);
extern void glGetTexParameteriv (GLenum target, GLenum pname, GLint *params);
extern void glHint (GLenum target, GLenum mode);
extern void glIndexMask (GLuint mask);
extern void glIndexPointer (GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glIndexd (GLdouble c);
extern void glIndexdv (const GLdouble *c);
extern void glIndexf (GLfloat c);
extern void glIndexfv (const GLfloat *c);
extern void glIndexi (GLint c);
extern void glIndexiv (const GLint *c);
extern void glIndexs (GLshort c);
extern void glIndexsv (const GLshort *c);
extern void glIndexub (GLubyte c);
extern void glIndexubv (const GLubyte *c);
extern void glInitNames (void);
extern void glInterleavedArrays (GLenum format, GLsizei stride, const GLvoid *pointer);
extern GLboolean glIsEnabled (GLenum cap);
extern GLboolean glIsList (GLuint list);
extern GLboolean glIsTexture (GLuint texture);
extern void glLightModelf (GLenum pname, GLfloat param);
extern void glLightModelfv (GLenum pname, const GLfloat *params);
extern void glLightModeli (GLenum pname, GLint param);
extern void glLightModeliv (GLenum pname, const GLint *params);
extern void glLightf (GLenum light, GLenum pname, GLfloat param);
extern void glLightfv (GLenum light, GLenum pname, const GLfloat *params);
extern void glLighti (GLenum light, GLenum pname, GLint param);
extern void glLightiv (GLenum light, GLenum pname, const GLint *params);
extern void glLineStipple (GLint factor, GLushort pattern);
extern void glLineWidth (GLfloat width);
extern void glListBase (GLuint base);
extern void glLoadIdentity (void);
extern void glLoadMatrixd (const GLdouble *m);
extern void glLoadMatrixf (const GLfloat *m);
extern void glLoadName (GLuint name);
extern void glLogicOp (GLenum opcode);
extern void glMap1d (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
extern void glMap1f (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
extern void glMap2d (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
extern void glMap2f (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
extern void glMapGrid1d (GLint un, GLdouble u1, GLdouble u2);
extern void glMapGrid1f (GLint un, GLfloat u1, GLfloat u2);
extern void glMapGrid2d (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
extern void glMapGrid2f (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
extern void glMaterialf (GLenum face, GLenum pname, GLfloat param);
extern void glMaterialfv (GLenum face, GLenum pname, const GLfloat *params);
extern void glMateriali (GLenum face, GLenum pname, GLint param);
extern void glMaterialiv (GLenum face, GLenum pname, const GLint *params);
extern void glMatrixMode (GLenum mode);
extern void glMultMatrixd (const GLdouble *m);
extern void glMultMatrixf (const GLfloat *m);
extern void glNewList (GLuint list, GLenum mode);
extern void glNormal3b (GLbyte nx, GLbyte ny, GLbyte nz);
extern void glNormal3bv (const GLbyte *v);
extern void glNormal3d (GLdouble nx, GLdouble ny, GLdouble nz);
extern void glNormal3dv (const GLdouble *v);
extern void glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz);
extern void glNormal3fv (const GLfloat *v);
extern void glNormal3i (GLint nx, GLint ny, GLint nz);
extern void glNormal3iv (const GLint *v);
extern void glNormal3s (GLshort nx, GLshort ny, GLshort nz);
extern void glNormal3sv (const GLshort *v);
extern void glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern void glPassThrough (GLfloat token);
extern void glPixelMapfv (GLenum map, GLsizei mapsize, const GLfloat *values);
extern void glPixelMapuiv (GLenum map, GLsizei mapsize, const GLuint *values);
extern void glPixelMapusv (GLenum map, GLsizei mapsize, const GLushort *values);
extern void glPixelStoref (GLenum pname, GLfloat param);
extern void glPixelStorei (GLenum pname, GLint param);
extern void glPixelTransferf (GLenum pname, GLfloat param);
extern void glPixelTransferi (GLenum pname, GLint param);
extern void glPixelZoom (GLfloat xfactor, GLfloat yfactor);
extern void glPointSize (GLfloat size);
extern void glPolygonMode (GLenum face, GLenum mode);
extern void glPolygonOffset (GLfloat factor, GLfloat units);
extern void glPolygonStipple (const GLubyte *mask);
extern void glPopAttrib (void);
extern void glPopClientAttrib (void);
extern void glPopMatrix (void);
extern void glPopName (void);
extern void glPrioritizeTextures (GLsizei n, const GLuint *textures, const GLclampf *priorities);
extern void glPushAttrib (GLbitfield mask);
extern void glPushClientAttrib (GLbitfield mask);
extern void glPushMatrix (void);
extern void glPushName (GLuint name);
extern void glRasterPos2d (GLdouble x, GLdouble y);
extern void glRasterPos2dv (const GLdouble *v);
extern void glRasterPos2f (GLfloat x, GLfloat y);
extern void glRasterPos2fv (const GLfloat *v);
extern void glRasterPos2i (GLint x, GLint y);
extern void glRasterPos2iv (const GLint *v);
extern void glRasterPos2s (GLshort x, GLshort y);
extern void glRasterPos2sv (const GLshort *v);
extern void glRasterPos3d (GLdouble x, GLdouble y, GLdouble z);
extern void glRasterPos3dv (const GLdouble *v);
extern void glRasterPos3f (GLfloat x, GLfloat y, GLfloat z);
extern void glRasterPos3fv (const GLfloat *v);
extern void glRasterPos3i (GLint x, GLint y, GLint z);
extern void glRasterPos3iv (const GLint *v);
extern void glRasterPos3s (GLshort x, GLshort y, GLshort z);
extern void glRasterPos3sv (const GLshort *v);
extern void glRasterPos4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void glRasterPos4dv (const GLdouble *v);
extern void glRasterPos4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void glRasterPos4fv (const GLfloat *v);
extern void glRasterPos4i (GLint x, GLint y, GLint z, GLint w);
extern void glRasterPos4iv (const GLint *v);
extern void glRasterPos4s (GLshort x, GLshort y, GLshort z, GLshort w);
extern void glRasterPos4sv (const GLshort *v);
extern void glReadBuffer (GLenum mode);
extern void glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
extern void glRectd (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
extern void glRectdv (const GLdouble *v1, const GLdouble *v2);
extern void glRectf (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
extern void glRectfv (const GLfloat *v1, const GLfloat *v2);
extern void glRecti (GLint x1, GLint y1, GLint x2, GLint y2);
extern void glRectiv (const GLint *v1, const GLint *v2);
extern void glRects (GLshort x1, GLshort y1, GLshort x2, GLshort y2);
extern void glRectsv (const GLshort *v1, const GLshort *v2);
extern GLint glRenderMode (GLenum mode);
extern void glRotated (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
extern void glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern void glScaled (GLdouble x, GLdouble y, GLdouble z);
extern void glScalef (GLfloat x, GLfloat y, GLfloat z);
extern void glScissor (GLint x, GLint y, GLsizei width, GLsizei height);
extern void glSelectBuffer (GLsizei size, GLuint *buffer);
extern void glShadeModel (GLenum mode);
extern void glStencilFunc (GLenum func, GLint ref, GLuint mask);
extern void glStencilMask (GLuint mask);
extern void glStencilOp (GLenum fail, GLenum zfail, GLenum zpass);
extern void glTexCoord1d (GLdouble s);
extern void glTexCoord1dv (const GLdouble *v);
extern void glTexCoord1f (GLfloat s);
extern void glTexCoord1fv (const GLfloat *v);
extern void glTexCoord1i (GLint s);
extern void glTexCoord1iv (const GLint *v);
extern void glTexCoord1s (GLshort s);
extern void glTexCoord1sv (const GLshort *v);
extern void glTexCoord2d (GLdouble s, GLdouble t);
extern void glTexCoord2dv (const GLdouble *v);
extern void glTexCoord2f (GLfloat s, GLfloat t);
extern void glTexCoord2fv (const GLfloat *v);
extern void glTexCoord2i (GLint s, GLint t);
extern void glTexCoord2iv (const GLint *v);
extern void glTexCoord2s (GLshort s, GLshort t);
extern void glTexCoord2sv (const GLshort *v);
extern void glTexCoord3d (GLdouble s, GLdouble t, GLdouble r);
extern void glTexCoord3dv (const GLdouble *v);
extern void glTexCoord3f (GLfloat s, GLfloat t, GLfloat r);
extern void glTexCoord3fv (const GLfloat *v);
extern void glTexCoord3i (GLint s, GLint t, GLint r);
extern void glTexCoord3iv (const GLint *v);
extern void glTexCoord3s (GLshort s, GLshort t, GLshort r);
extern void glTexCoord3sv (const GLshort *v);
extern void glTexCoord4d (GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern void glTexCoord4dv (const GLdouble *v);
extern void glTexCoord4f (GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern void glTexCoord4fv (const GLfloat *v);
extern void glTexCoord4i (GLint s, GLint t, GLint r, GLint q);
extern void glTexCoord4iv (const GLint *v);
extern void glTexCoord4s (GLshort s, GLshort t, GLshort r, GLshort q);
extern void glTexCoord4sv (const GLshort *v);
extern void glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glTexEnvf (GLenum target, GLenum pname, GLfloat param);
extern void glTexEnvfv (GLenum target, GLenum pname, const GLfloat *params);
extern void glTexEnvi (GLenum target, GLenum pname, GLint param);
extern void glTexEnviv (GLenum target, GLenum pname, const GLint *params);
extern void glTexGend (GLenum coord, GLenum pname, GLdouble param);
extern void glTexGendv (GLenum coord, GLenum pname, const GLdouble *params);
extern void glTexGenf (GLenum coord, GLenum pname, GLfloat param);
extern void glTexGenfv (GLenum coord, GLenum pname, const GLfloat *params);
extern void glTexGeni (GLenum coord, GLenum pname, GLint param);
extern void glTexGeniv (GLenum coord, GLenum pname, const GLint *params);
extern void glTexImage1D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void glTexParameterf (GLenum target, GLenum pname, GLfloat param);
extern void glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params);
extern void glTexParameteri (GLenum target, GLenum pname, GLint param);
extern void glTexParameteriv (GLenum target, GLenum pname, const GLint *params);
extern void glTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
extern void glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
extern void glTranslated (GLdouble x, GLdouble y, GLdouble z);
extern void glTranslatef (GLfloat x, GLfloat y, GLfloat z);
extern void glVertex2d (GLdouble x, GLdouble y);
extern void glVertex2dv (const GLdouble *v);
extern void glVertex2f (GLfloat x, GLfloat y);
extern void glVertex2fv (const GLfloat *v);
extern void glVertex2i (GLint x, GLint y);
extern void glVertex2iv (const GLint *v);
extern void glVertex2s (GLshort x, GLshort y);
extern void glVertex2sv (const GLshort *v);
extern void glVertex3d (GLdouble x, GLdouble y, GLdouble z);
extern void glVertex3dv (const GLdouble *v);
extern void glVertex3f (GLfloat x, GLfloat y, GLfloat z);
extern void glVertex3fv (const GLfloat *v);
extern void glVertex3i (GLint x, GLint y, GLint z);
extern void glVertex3iv (const GLint *v);
extern void glVertex3s (GLshort x, GLshort y, GLshort z);
extern void glVertex3sv (const GLshort *v);
extern void glVertex4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void glVertex4dv (const GLdouble *v);
extern void glVertex4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void glVertex4fv (const GLfloat *v);
extern void glVertex4i (GLint x, GLint y, GLint z, GLint w);
extern void glVertex4iv (const GLint *v);
extern void glVertex4s (GLshort x, GLshort y, GLshort z, GLshort w);
extern void glVertex4sv (const GLshort *v);
extern void glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glViewport (GLint x, GLint y, GLsizei width, GLsizei height);

extern "C" {

class GLUnurbs;
class GLUquadric;
class GLUtesselator;

typedef class GLUnurbs GLUnurbsObj;
typedef class GLUquadric GLUquadricObj;
typedef class GLUtesselator GLUtesselatorObj;
typedef class GLUtesselator GLUtriangulatorObj;

extern void gluBeginCurve (GLUnurbs* nurb);
extern void gluBeginPolygon (GLUtesselator* tess);
extern void gluBeginSurface (GLUnurbs* nurb);
extern void gluBeginTrim (GLUnurbs* nurb);
extern GLint gluBuild1DMipmapLevels (GLenum target, GLint internalFormat, GLsizei width, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data);
extern GLint gluBuild1DMipmaps (GLenum target, GLint internalFormat, GLsizei width, GLenum format, GLenum type, const void *data);
extern GLint gluBuild2DMipmapLevels (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data);
extern GLint gluBuild2DMipmaps (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *data);
extern GLint gluBuild3DMipmapLevels (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLint level, GLint base, GLint max, const void *data);
extern GLint gluBuild3DMipmaps (GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
extern GLboolean gluCheckExtension (const GLubyte *extName, const GLubyte *extString);
extern void gluCylinder (GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks);
extern void gluDeleteNurbsRenderer (GLUnurbs* nurb);
extern void gluDeleteQuadric (GLUquadric* quad);
extern void gluDeleteTess (GLUtesselator* tess);
extern void gluDisk (GLUquadric* quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops);
extern void gluEndCurve (GLUnurbs* nurb);
extern void gluEndPolygon (GLUtesselator* tess);
extern void gluEndSurface (GLUnurbs* nurb);
extern void gluEndTrim (GLUnurbs* nurb);
extern const GLubyte * gluErrorString (GLenum error);
extern void gluGetNurbsProperty (GLUnurbs* nurb, GLenum property, GLfloat* data);
extern const GLubyte * gluGetString (GLenum name);
extern void gluGetTessProperty (GLUtesselator* tess, GLenum which, GLdouble* data);
extern void gluLoadSamplingMatrices (GLUnurbs* nurb, const GLfloat *model, const GLfloat *perspective, const GLint *view);
extern void gluLookAt (GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ);
extern GLUnurbs* gluNewNurbsRenderer (void);
extern GLUquadric* gluNewQuadric (void);
extern GLUtesselator* gluNewTess (void);
extern void gluNextContour (GLUtesselator* tess, GLenum type);
extern void gluNurbsCallback (GLUnurbs* nurb, GLenum which, GLvoid (*CallBackFunc)());
extern void gluNurbsCallbackData (GLUnurbs* nurb, GLvoid* userData);
extern void gluNurbsCallbackDataEXT (GLUnurbs* nurb, GLvoid* userData);
extern void gluNurbsCurve (GLUnurbs* nurb, GLint knotCount, GLfloat *knots, GLint stride, GLfloat *control, GLint order, GLenum type);
extern void gluNurbsProperty (GLUnurbs* nurb, GLenum property, GLfloat value);
extern void gluNurbsSurface (GLUnurbs* nurb, GLint sKnotCount, GLfloat* sKnots, GLint tKnotCount, GLfloat* tKnots, GLint sStride, GLint tStride, GLfloat* control, GLint sOrder, GLint tOrder, GLenum type);
extern void gluOrtho2D (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
extern void gluPartialDisk (GLUquadric* quad, GLdouble inner, GLdouble outer, GLint slices, GLint loops, GLdouble start, GLdouble sweep);
extern void gluPerspective (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
extern void gluPickMatrix (GLdouble x, GLdouble y, GLdouble delX, GLdouble delY, GLint *viewport);
extern GLint gluProject (GLdouble objX, GLdouble objY, GLdouble objZ, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble* winX, GLdouble* winY, GLdouble* winZ);
extern void gluPwlCurve (GLUnurbs* nurb, GLint count, GLfloat* data, GLint stride, GLenum type);
extern void gluQuadricCallback (GLUquadric* quad, GLenum which, GLvoid (*CallBackFunc)());
extern void gluQuadricDrawStyle (GLUquadric* quad, GLenum draw);
extern void gluQuadricNormals (GLUquadric* quad, GLenum normal);
extern void gluQuadricOrientation (GLUquadric* quad, GLenum orientation);
extern void gluQuadricTexture (GLUquadric* quad, GLboolean texture);
extern GLint gluScaleImage (GLenum format, GLsizei wIn, GLsizei hIn, GLenum typeIn, const void *dataIn, GLsizei wOut, GLsizei hOut, GLenum typeOut, GLvoid* dataOut);
extern void gluSphere (GLUquadric* quad, GLdouble radius, GLint slices, GLint stacks);
extern void gluTessBeginContour (GLUtesselator* tess);
extern void gluTessBeginPolygon (GLUtesselator* tess, GLvoid* data);
extern void gluTessCallback (GLUtesselator* tess, GLenum which, GLvoid (*CallBackFunc)());
extern void gluTessEndContour (GLUtesselator* tess);
extern void gluTessEndPolygon (GLUtesselator* tess);
extern void gluTessNormal (GLUtesselator* tess, GLdouble valueX, GLdouble valueY, GLdouble valueZ);
extern void gluTessProperty (GLUtesselator* tess, GLenum which, GLdouble data);
extern void gluTessVertex (GLUtesselator* tess, GLdouble *location, GLvoid* data);
extern GLint gluUnProject (GLdouble winX, GLdouble winY, GLdouble winZ, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble* objX, GLdouble* objY, GLdouble* objZ);
extern GLint gluUnProject4 (GLdouble winX, GLdouble winY, GLdouble winZ, GLdouble clipW, const GLdouble *model, const GLdouble *proj, const GLint *view, GLdouble nearPlane, GLdouble farPlane, GLdouble* objX, GLdouble* objY, GLdouble* objZ, GLdouble* objW);
}
typedef void ( * PFNGLCOPYTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void ( * PFNGLDRAWRANGEELEMENTSPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
typedef void ( * PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void ( * PFNGLTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);

typedef void ( * PFNGLACTIVETEXTUREPROC) (GLenum texture);
typedef void ( * PFNGLCLIENTACTIVETEXTUREPROC) (GLenum texture);
typedef void ( * PFNGLCOMPRESSEDTEXIMAGE1DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void ( * PFNGLCOMPRESSEDTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void ( * PFNGLCOMPRESSEDTEXIMAGE3DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void ( * PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void ( * PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void ( * PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void ( * PFNGLGETCOMPRESSEDTEXIMAGEPROC) (GLenum target, GLint lod, GLvoid *img);
typedef void ( * PFNGLLOADTRANSPOSEMATRIXDPROC) (const GLdouble m[16]);
typedef void ( * PFNGLLOADTRANSPOSEMATRIXFPROC) (const GLfloat m[16]);
typedef void ( * PFNGLMULTTRANSPOSEMATRIXDPROC) (const GLdouble m[16]);
typedef void ( * PFNGLMULTTRANSPOSEMATRIXFPROC) (const GLfloat m[16]);
typedef void ( * PFNGLMULTITEXCOORD1DPROC) (GLenum target, GLdouble s);
typedef void ( * PFNGLMULTITEXCOORD1DVPROC) (GLenum target, const GLdouble *v);
typedef void ( * PFNGLMULTITEXCOORD1FPROC) (GLenum target, GLfloat s);
typedef void ( * PFNGLMULTITEXCOORD1FVPROC) (GLenum target, const GLfloat *v);
typedef void ( * PFNGLMULTITEXCOORD1IPROC) (GLenum target, GLint s);
typedef void ( * PFNGLMULTITEXCOORD1IVPROC) (GLenum target, const GLint *v);
typedef void ( * PFNGLMULTITEXCOORD1SPROC) (GLenum target, GLshort s);
typedef void ( * PFNGLMULTITEXCOORD1SVPROC) (GLenum target, const GLshort *v);
typedef void ( * PFNGLMULTITEXCOORD2DPROC) (GLenum target, GLdouble s, GLdouble t);
typedef void ( * PFNGLMULTITEXCOORD2DVPROC) (GLenum target, const GLdouble *v);
typedef void ( * PFNGLMULTITEXCOORD2FPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void ( * PFNGLMULTITEXCOORD2FVPROC) (GLenum target, const GLfloat *v);
typedef void ( * PFNGLMULTITEXCOORD2IPROC) (GLenum target, GLint s, GLint t);
typedef void ( * PFNGLMULTITEXCOORD2IVPROC) (GLenum target, const GLint *v);
typedef void ( * PFNGLMULTITEXCOORD2SPROC) (GLenum target, GLshort s, GLshort t);
typedef void ( * PFNGLMULTITEXCOORD2SVPROC) (GLenum target, const GLshort *v);
typedef void ( * PFNGLMULTITEXCOORD3DPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void ( * PFNGLMULTITEXCOORD3DVPROC) (GLenum target, const GLdouble *v);
typedef void ( * PFNGLMULTITEXCOORD3FPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void ( * PFNGLMULTITEXCOORD3FVPROC) (GLenum target, const GLfloat *v);
typedef void ( * PFNGLMULTITEXCOORD3IPROC) (GLenum target, GLint s, GLint t, GLint r);
typedef void ( * PFNGLMULTITEXCOORD3IVPROC) (GLenum target, const GLint *v);
typedef void ( * PFNGLMULTITEXCOORD3SPROC) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void ( * PFNGLMULTITEXCOORD3SVPROC) (GLenum target, const GLshort *v);
typedef void ( * PFNGLMULTITEXCOORD4DPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void ( * PFNGLMULTITEXCOORD4DVPROC) (GLenum target, const GLdouble *v);
typedef void ( * PFNGLMULTITEXCOORD4FPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void ( * PFNGLMULTITEXCOORD4FVPROC) (GLenum target, const GLfloat *v);
typedef void ( * PFNGLMULTITEXCOORD4IPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void ( * PFNGLMULTITEXCOORD4IVPROC) (GLenum target, const GLint *v);
typedef void ( * PFNGLMULTITEXCOORD4SPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void ( * PFNGLMULTITEXCOORD4SVPROC) (GLenum target, const GLshort *v);
typedef void ( * PFNGLSAMPLECOVERAGEPROC) (GLclampf value, GLboolean invert);

typedef void ( * PFNGLBLENDCOLORPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void ( * PFNGLBLENDEQUATIONPROC) (GLenum mode);
typedef void ( * PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void ( * PFNGLFOGCOORDPOINTERPROC) (GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void ( * PFNGLFOGCOORDDPROC) (GLdouble coord);
typedef void ( * PFNGLFOGCOORDDVPROC) (const GLdouble *coord);
typedef void ( * PFNGLFOGCOORDFPROC) (GLfloat coord);
typedef void ( * PFNGLFOGCOORDFVPROC) (const GLfloat *coord);
typedef void ( * PFNGLMULTIDRAWARRAYSPROC) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
typedef void ( * PFNGLMULTIDRAWELEMENTSPROC) (GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount);
typedef void ( * PFNGLPOINTPARAMETERFPROC) (GLenum pname, GLfloat param);
typedef void ( * PFNGLPOINTPARAMETERFVPROC) (GLenum pname, const GLfloat *params);
typedef void ( * PFNGLPOINTPARAMETERIPROC) (GLenum pname, GLint param);
typedef void ( * PFNGLPOINTPARAMETERIVPROC) (GLenum pname, const GLint *params);
typedef void ( * PFNGLSECONDARYCOLOR3BPROC) (GLbyte red, GLbyte green, GLbyte blue);
typedef void ( * PFNGLSECONDARYCOLOR3BVPROC) (const GLbyte *v);
typedef void ( * PFNGLSECONDARYCOLOR3DPROC) (GLdouble red, GLdouble green, GLdouble blue);
typedef void ( * PFNGLSECONDARYCOLOR3DVPROC) (const GLdouble *v);
typedef void ( * PFNGLSECONDARYCOLOR3FPROC) (GLfloat red, GLfloat green, GLfloat blue);
typedef void ( * PFNGLSECONDARYCOLOR3FVPROC) (const GLfloat *v);
typedef void ( * PFNGLSECONDARYCOLOR3IPROC) (GLint red, GLint green, GLint blue);
typedef void ( * PFNGLSECONDARYCOLOR3IVPROC) (const GLint *v);
typedef void ( * PFNGLSECONDARYCOLOR3SPROC) (GLshort red, GLshort green, GLshort blue);
typedef void ( * PFNGLSECONDARYCOLOR3SVPROC) (const GLshort *v);
typedef void ( * PFNGLSECONDARYCOLOR3UBPROC) (GLubyte red, GLubyte green, GLubyte blue);
typedef void ( * PFNGLSECONDARYCOLOR3UBVPROC) (const GLubyte *v);
typedef void ( * PFNGLSECONDARYCOLOR3UIPROC) (GLuint red, GLuint green, GLuint blue);
typedef void ( * PFNGLSECONDARYCOLOR3UIVPROC) (const GLuint *v);
typedef void ( * PFNGLSECONDARYCOLOR3USPROC) (GLushort red, GLushort green, GLushort blue);
typedef void ( * PFNGLSECONDARYCOLOR3USVPROC) (const GLushort *v);
typedef void ( * PFNGLSECONDARYCOLORPOINTERPROC) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void ( * PFNGLWINDOWPOS2DPROC) (GLdouble x, GLdouble y);
typedef void ( * PFNGLWINDOWPOS2DVPROC) (const GLdouble *p);
typedef void ( * PFNGLWINDOWPOS2FPROC) (GLfloat x, GLfloat y);
typedef void ( * PFNGLWINDOWPOS2FVPROC) (const GLfloat *p);
typedef void ( * PFNGLWINDOWPOS2IPROC) (GLint x, GLint y);
typedef void ( * PFNGLWINDOWPOS2IVPROC) (const GLint *p);
typedef void ( * PFNGLWINDOWPOS2SPROC) (GLshort x, GLshort y);
typedef void ( * PFNGLWINDOWPOS2SVPROC) (const GLshort *p);
typedef void ( * PFNGLWINDOWPOS3DPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLWINDOWPOS3DVPROC) (const GLdouble *p);
typedef void ( * PFNGLWINDOWPOS3FPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLWINDOWPOS3FVPROC) (const GLfloat *p);
typedef void ( * PFNGLWINDOWPOS3IPROC) (GLint x, GLint y, GLint z);
typedef void ( * PFNGLWINDOWPOS3IVPROC) (const GLint *p);
typedef void ( * PFNGLWINDOWPOS3SPROC) (GLshort x, GLshort y, GLshort z);
typedef void ( * PFNGLWINDOWPOS3SVPROC) (const GLshort *p);

typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

typedef void ( * PFNGLBEGINQUERYPROC) (GLenum target, GLuint id);
typedef void ( * PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void ( * PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
typedef void ( * PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
typedef void ( * PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);
typedef void ( * PFNGLDELETEQUERIESPROC) (GLsizei n, const GLuint* ids);
typedef void ( * PFNGLENDQUERYPROC) (GLenum target);
typedef void ( * PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void ( * PFNGLGENQUERIESPROC) (GLsizei n, GLuint* ids);
typedef void ( * PFNGLGETBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef void ( * PFNGLGETBUFFERPOINTERVPROC) (GLenum target, GLenum pname, GLvoid** params);
typedef void ( * PFNGLGETBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid* data);
typedef void ( * PFNGLGETQUERYOBJECTIVPROC) (GLuint id, GLenum pname, GLint* params);
typedef void ( * PFNGLGETQUERYOBJECTUIVPROC) (GLuint id, GLenum pname, GLuint* params);
typedef void ( * PFNGLGETQUERYIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean ( * PFNGLISBUFFERPROC) (GLuint buffer);
typedef GLboolean ( * PFNGLISQUERYPROC) (GLuint id);
typedef GLvoid* ( * PFNGLMAPBUFFERPROC) (GLenum target, GLenum access);
typedef GLboolean ( * PFNGLUNMAPBUFFERPROC) (GLenum target);

typedef void ( * PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void ( * PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const GLchar* name);
typedef void ( * PFNGLBLENDEQUATIONSEPARATEPROC) (GLenum, GLenum);
typedef void ( * PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef GLuint ( * PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint ( * PFNGLCREATESHADERPROC) (GLenum type);
typedef void ( * PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void ( * PFNGLDELETESHADERPROC) (GLuint shader);
typedef void ( * PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void ( * PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint);
typedef void ( * PFNGLDRAWBUFFERSPROC) (GLsizei n, const GLenum* bufs);
typedef void ( * PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint);
typedef void ( * PFNGLGETACTIVEATTRIBPROC) (GLuint program, GLuint index, GLsizei maxLength, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef void ( * PFNGLGETACTIVEUNIFORMPROC) (GLuint program, GLuint index, GLsizei maxLength, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef void ( * PFNGLGETATTACHEDSHADERSPROC) (GLuint program, GLsizei maxCount, GLsizei* count, GLuint* shaders);
typedef GLint ( * PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void ( * PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void ( * PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint* param);
typedef void ( * PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void ( * PFNGLGETSHADERSOURCEPROC) (GLuint obj, GLsizei maxLength, GLsizei* length, GLchar* source);
typedef void ( * PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* param);
typedef GLint ( * PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void ( * PFNGLGETUNIFORMFVPROC) (GLuint program, GLint location, GLfloat* params);
typedef void ( * PFNGLGETUNIFORMIVPROC) (GLuint program, GLint location, GLint* params);
typedef void ( * PFNGLGETVERTEXATTRIBPOINTERVPROC) (GLuint, GLenum, GLvoid**);
typedef void ( * PFNGLGETVERTEXATTRIBDVPROC) (GLuint, GLenum, GLdouble*);
typedef void ( * PFNGLGETVERTEXATTRIBFVPROC) (GLuint, GLenum, GLfloat*);
typedef void ( * PFNGLGETVERTEXATTRIBIVPROC) (GLuint, GLenum, GLint*);
typedef GLboolean ( * PFNGLISPROGRAMPROC) (GLuint program);
typedef GLboolean ( * PFNGLISSHADERPROC) (GLuint shader);
typedef void ( * PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void ( * PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);
typedef void ( * PFNGLSTENCILFUNCSEPARATEPROC) (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
typedef void ( * PFNGLSTENCILMASKSEPARATEPROC) (GLenum, GLuint);
typedef void ( * PFNGLSTENCILOPSEPARATEPROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void ( * PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void ( * PFNGLUNIFORM1FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void ( * PFNGLUNIFORM1IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLUNIFORM2FPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void ( * PFNGLUNIFORM2FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLUNIFORM2IPROC) (GLint location, GLint v0, GLint v1);
typedef void ( * PFNGLUNIFORM2IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void ( * PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLUNIFORM3IPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void ( * PFNGLUNIFORM3IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void ( * PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLUNIFORM4IPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void ( * PFNGLUNIFORM4IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLUNIFORMMATRIX2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLUNIFORMMATRIX3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void ( * PFNGLVALIDATEPROGRAMPROC) (GLuint program);
typedef void ( * PFNGLVERTEXATTRIB1DPROC) (GLuint index, GLdouble x);
typedef void ( * PFNGLVERTEXATTRIB1DVPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIB1FPROC) (GLuint index, GLfloat x);
typedef void ( * PFNGLVERTEXATTRIB1FVPROC) (GLuint index, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIB1SPROC) (GLuint index, GLshort x);
typedef void ( * PFNGLVERTEXATTRIB1SVPROC) (GLuint index, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIB2DPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void ( * PFNGLVERTEXATTRIB2DVPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIB2FPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void ( * PFNGLVERTEXATTRIB2FVPROC) (GLuint index, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIB2SPROC) (GLuint index, GLshort x, GLshort y);
typedef void ( * PFNGLVERTEXATTRIB2SVPROC) (GLuint index, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIB3DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLVERTEXATTRIB3DVPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIB3FPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLVERTEXATTRIB3FVPROC) (GLuint index, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIB3SPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void ( * PFNGLVERTEXATTRIB3SVPROC) (GLuint index, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIB4NBVPROC) (GLuint index, const GLbyte* v);
typedef void ( * PFNGLVERTEXATTRIB4NIVPROC) (GLuint index, const GLint* v);
typedef void ( * PFNGLVERTEXATTRIB4NSVPROC) (GLuint index, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIB4NUBPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void ( * PFNGLVERTEXATTRIB4NUBVPROC) (GLuint index, const GLubyte* v);
typedef void ( * PFNGLVERTEXATTRIB4NUIVPROC) (GLuint index, const GLuint* v);
typedef void ( * PFNGLVERTEXATTRIB4NUSVPROC) (GLuint index, const GLushort* v);
typedef void ( * PFNGLVERTEXATTRIB4BVPROC) (GLuint index, const GLbyte* v);
typedef void ( * PFNGLVERTEXATTRIB4DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void ( * PFNGLVERTEXATTRIB4DVPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIB4FPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void ( * PFNGLVERTEXATTRIB4FVPROC) (GLuint index, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIB4IVPROC) (GLuint index, const GLint* v);
typedef void ( * PFNGLVERTEXATTRIB4SPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void ( * PFNGLVERTEXATTRIB4SVPROC) (GLuint index, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIB4UBVPROC) (GLuint index, const GLubyte* v);
typedef void ( * PFNGLVERTEXATTRIB4UIVPROC) (GLuint index, const GLuint* v);
typedef void ( * PFNGLVERTEXATTRIB4USVPROC) (GLuint index, const GLushort* v);
typedef void ( * PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);

typedef void ( * PFNGLUNIFORMMATRIX2X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void ( * PFNGLUNIFORMMATRIX2X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void ( * PFNGLUNIFORMMATRIX3X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void ( * PFNGLUNIFORMMATRIX3X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void ( * PFNGLUNIFORMMATRIX4X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void ( * PFNGLUNIFORMMATRIX4X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

typedef void ( * PFNGLBEGINCONDITIONALRENDERPROC) (GLuint, GLenum);
typedef void ( * PFNGLBEGINTRANSFORMFEEDBACKPROC) (GLenum);
typedef void ( * PFNGLBINDFRAGDATALOCATIONPROC) (GLuint, GLuint, const GLchar*);
typedef void ( * PFNGLCLAMPCOLORPROC) (GLenum, GLenum);
typedef void ( * PFNGLCLEARBUFFERFIPROC) (GLenum, GLint, GLfloat, GLint);
typedef void ( * PFNGLCLEARBUFFERFVPROC) (GLenum, GLint, const GLfloat*);
typedef void ( * PFNGLCLEARBUFFERIVPROC) (GLenum, GLint, const GLint*);
typedef void ( * PFNGLCLEARBUFFERUIVPROC) (GLenum, GLint, const GLuint*);
typedef void ( * PFNGLCOLORMASKIPROC) (GLuint, GLboolean, GLboolean, GLboolean, GLboolean);
typedef void ( * PFNGLDISABLEIPROC) (GLenum, GLuint);
typedef void ( * PFNGLENABLEIPROC) (GLenum, GLuint);
typedef void ( * PFNGLENDCONDITIONALRENDERPROC) (void);
typedef void ( * PFNGLENDTRANSFORMFEEDBACKPROC) (void);
typedef void ( * PFNGLGETBOOLEANI_VPROC) (GLenum, GLuint, GLboolean*);
typedef GLint ( * PFNGLGETFRAGDATALOCATIONPROC) (GLuint, const GLchar*);
typedef const GLubyte* ( * PFNGLGETSTRINGIPROC) (GLenum, GLuint);
typedef void ( * PFNGLGETTEXPARAMETERIIVPROC) (GLenum, GLenum, GLint*);
typedef void ( * PFNGLGETTEXPARAMETERIUIVPROC) (GLenum, GLenum, GLuint*);
typedef void ( * PFNGLGETTRANSFORMFEEDBACKVARYINGPROC) (GLuint, GLuint, GLsizei, GLsizei *, GLsizei *, GLenum *, GLchar *);
typedef void ( * PFNGLGETUNIFORMUIVPROC) (GLuint, GLint, GLuint*);
typedef void ( * PFNGLGETVERTEXATTRIBIIVPROC) (GLuint, GLenum, GLint*);
typedef void ( * PFNGLGETVERTEXATTRIBIUIVPROC) (GLuint, GLenum, GLuint*);
typedef GLboolean ( * PFNGLISENABLEDIPROC) (GLenum, GLuint);
typedef void ( * PFNGLTEXPARAMETERIIVPROC) (GLenum, GLenum, const GLint*);
typedef void ( * PFNGLTEXPARAMETERIUIVPROC) (GLenum, GLenum, const GLuint*);
typedef void ( * PFNGLTRANSFORMFEEDBACKVARYINGSPROC) (GLuint, GLsizei, const GLchar **, GLenum);
typedef void ( * PFNGLUNIFORM1UIPROC) (GLint, GLuint);
typedef void ( * PFNGLUNIFORM1UIVPROC) (GLint, GLsizei, const GLuint*);
typedef void ( * PFNGLUNIFORM2UIPROC) (GLint, GLuint, GLuint);
typedef void ( * PFNGLUNIFORM2UIVPROC) (GLint, GLsizei, const GLuint*);
typedef void ( * PFNGLUNIFORM3UIPROC) (GLint, GLuint, GLuint, GLuint);
typedef void ( * PFNGLUNIFORM3UIVPROC) (GLint, GLsizei, const GLuint*);
typedef void ( * PFNGLUNIFORM4UIPROC) (GLint, GLuint, GLuint, GLuint, GLuint);
typedef void ( * PFNGLUNIFORM4UIVPROC) (GLint, GLsizei, const GLuint*);
typedef void ( * PFNGLVERTEXATTRIBI1IPROC) (GLuint, GLint);
typedef void ( * PFNGLVERTEXATTRIBI1IVPROC) (GLuint, const GLint*);
typedef void ( * PFNGLVERTEXATTRIBI1UIPROC) (GLuint, GLuint);
typedef void ( * PFNGLVERTEXATTRIBI1UIVPROC) (GLuint, const GLuint*);
typedef void ( * PFNGLVERTEXATTRIBI2IPROC) (GLuint, GLint, GLint);
typedef void ( * PFNGLVERTEXATTRIBI2IVPROC) (GLuint, const GLint*);
typedef void ( * PFNGLVERTEXATTRIBI2UIPROC) (GLuint, GLuint, GLuint);
typedef void ( * PFNGLVERTEXATTRIBI2UIVPROC) (GLuint, const GLuint*);
typedef void ( * PFNGLVERTEXATTRIBI3IPROC) (GLuint, GLint, GLint, GLint);
typedef void ( * PFNGLVERTEXATTRIBI3IVPROC) (GLuint, const GLint*);
typedef void ( * PFNGLVERTEXATTRIBI3UIPROC) (GLuint, GLuint, GLuint, GLuint);
typedef void ( * PFNGLVERTEXATTRIBI3UIVPROC) (GLuint, const GLuint*);
typedef void ( * PFNGLVERTEXATTRIBI4BVPROC) (GLuint, const GLbyte*);
typedef void ( * PFNGLVERTEXATTRIBI4IPROC) (GLuint, GLint, GLint, GLint, GLint);
typedef void ( * PFNGLVERTEXATTRIBI4IVPROC) (GLuint, const GLint*);
typedef void ( * PFNGLVERTEXATTRIBI4SVPROC) (GLuint, const GLshort*);
typedef void ( * PFNGLVERTEXATTRIBI4UBVPROC) (GLuint, const GLubyte*);
typedef void ( * PFNGLVERTEXATTRIBI4UIPROC) (GLuint, GLuint, GLuint, GLuint, GLuint);
typedef void ( * PFNGLVERTEXATTRIBI4UIVPROC) (GLuint, const GLuint*);
typedef void ( * PFNGLVERTEXATTRIBI4USVPROC) (GLuint, const GLushort*);
typedef void ( * PFNGLVERTEXATTRIBIPOINTERPROC) (GLuint, GLint, GLenum, GLsizei, const GLvoid*);

typedef void ( * PFNGLDRAWARRAYSINSTANCEDPROC) (GLenum, GLint, GLsizei, GLsizei);
typedef void ( * PFNGLDRAWELEMENTSINSTANCEDPROC) (GLenum, GLsizei, GLenum, const GLvoid*, GLsizei);
typedef void ( * PFNGLPRIMITIVERESTARTINDEXPROC) (GLuint);
typedef void ( * PFNGLTEXBUFFERPROC) (GLenum, GLenum, GLuint);

typedef void ( * PFNGLFRAMEBUFFERTEXTUREPROC) (GLenum, GLenum, GLuint, GLint);
typedef void ( * PFNGLGETBUFFERPARAMETERI64VPROC) (GLenum, GLenum, GLint64 *);
typedef void ( * PFNGLGETINTEGER64I_VPROC) (GLenum, GLuint, GLint64 *);

typedef void ( * PFNGLVERTEXATTRIBDIVISORPROC) (GLuint index, GLuint divisor);

typedef void ( * PFNGLBLENDEQUATIONSEPARATEIPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void ( * PFNGLBLENDEQUATIONIPROC) (GLuint buf, GLenum mode);
typedef void ( * PFNGLBLENDFUNCSEPARATEIPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void ( * PFNGLBLENDFUNCIPROC) (GLuint buf, GLenum src, GLenum dst);
typedef void ( * PFNGLMINSAMPLESHADINGPROC) (GLclampf value);

typedef void ( * PFNGLTBUFFERMASK3DFXPROC) (GLuint mask);

typedef void ( *GLDEBUGPROCAMD)(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);

typedef void ( * PFNGLDEBUGMESSAGECALLBACKAMDPROC) (GLDEBUGPROCAMD callback, void* userParam);
typedef void ( * PFNGLDEBUGMESSAGEENABLEAMDPROC) (GLenum category, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
typedef void ( * PFNGLDEBUGMESSAGEINSERTAMDPROC) (GLenum category, GLenum severity, GLuint id, GLsizei length, const char* buf);
typedef GLuint ( * PFNGLGETDEBUGMESSAGELOGAMDPROC) (GLuint count, GLsizei bufsize, GLenum* categories, GLuint* severities, GLuint* ids, GLsizei* lengths, char* message);

typedef void ( * PFNGLBLENDEQUATIONINDEXEDAMDPROC) (GLuint buf, GLenum mode);
typedef void ( * PFNGLBLENDEQUATIONSEPARATEINDEXEDAMDPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void ( * PFNGLBLENDFUNCINDEXEDAMDPROC) (GLuint buf, GLenum src, GLenum dst);
typedef void ( * PFNGLBLENDFUNCSEPARATEINDEXEDAMDPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

typedef void ( * PFNGLMULTIDRAWARRAYSINDIRECTAMDPROC) (GLenum mode, const void* indirect, GLsizei primcount, GLsizei stride);
typedef void ( * PFNGLMULTIDRAWELEMENTSINDIRECTAMDPROC) (GLenum mode, GLenum type, const void* indirect, GLsizei primcount, GLsizei stride);

typedef void ( * PFNGLDELETENAMESAMDPROC) (GLenum identifier, GLuint num, const GLuint* names);
typedef void ( * PFNGLGENNAMESAMDPROC) (GLenum identifier, GLuint num, GLuint* names);
typedef GLboolean ( * PFNGLISNAMEAMDPROC) (GLenum identifier, GLuint name);

typedef void ( * PFNGLBEGINPERFMONITORAMDPROC) (GLuint monitor);
typedef void ( * PFNGLDELETEPERFMONITORSAMDPROC) (GLsizei n, GLuint* monitors);
typedef void ( * PFNGLENDPERFMONITORAMDPROC) (GLuint monitor);
typedef void ( * PFNGLGENPERFMONITORSAMDPROC) (GLsizei n, GLuint* monitors);
typedef void ( * PFNGLGETPERFMONITORCOUNTERDATAAMDPROC) (GLuint monitor, GLenum pname, GLsizei dataSize, GLuint* data, GLint *bytesWritten);
typedef void ( * PFNGLGETPERFMONITORCOUNTERINFOAMDPROC) (GLuint group, GLuint counter, GLenum pname, void* data);
typedef void ( * PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC) (GLuint group, GLuint counter, GLsizei bufSize, GLsizei* length, char *counterString);
typedef void ( * PFNGLGETPERFMONITORCOUNTERSAMDPROC) (GLuint group, GLint* numCounters, GLint *maxActiveCounters, GLsizei countersSize, GLuint *counters);
typedef void ( * PFNGLGETPERFMONITORGROUPSTRINGAMDPROC) (GLuint group, GLsizei bufSize, GLsizei* length, char *groupString);
typedef void ( * PFNGLGETPERFMONITORGROUPSAMDPROC) (GLint* numGroups, GLsizei groupsSize, GLuint *groups);
typedef void ( * PFNGLSELECTPERFMONITORCOUNTERSAMDPROC) (GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint* counterList);

typedef void ( * PFNGLSETMULTISAMPLEFVAMDPROC) (GLenum pname, GLuint index, const GLfloat* val);

typedef void ( * PFNGLTESSELLATIONFACTORAMDPROC) (GLfloat factor);
typedef void ( * PFNGLTESSELLATIONMODEAMDPROC) (GLenum mode);

typedef void ( * PFNGLDRAWELEMENTARRAYAPPLEPROC) (GLenum mode, GLint first, GLsizei count);
typedef void ( * PFNGLDRAWRANGEELEMENTARRAYAPPLEPROC) (GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count);
typedef void ( * PFNGLELEMENTPOINTERAPPLEPROC) (GLenum type, const void* pointer);
typedef void ( * PFNGLMULTIDRAWELEMENTARRAYAPPLEPROC) (GLenum mode, const GLint* first, const GLsizei *count, GLsizei primcount);
typedef void ( * PFNGLMULTIDRAWRANGEELEMENTARRAYAPPLEPROC) (GLenum mode, GLuint start, GLuint end, const GLint* first, const GLsizei *count, GLsizei primcount);

typedef void ( * PFNGLDELETEFENCESAPPLEPROC) (GLsizei n, const GLuint* fences);
typedef void ( * PFNGLFINISHFENCEAPPLEPROC) (GLuint fence);
typedef void ( * PFNGLFINISHOBJECTAPPLEPROC) (GLenum object, GLint name);
typedef void ( * PFNGLGENFENCESAPPLEPROC) (GLsizei n, GLuint* fences);
typedef GLboolean ( * PFNGLISFENCEAPPLEPROC) (GLuint fence);
typedef void ( * PFNGLSETFENCEAPPLEPROC) (GLuint fence);
typedef GLboolean ( * PFNGLTESTFENCEAPPLEPROC) (GLuint fence);
typedef GLboolean ( * PFNGLTESTOBJECTAPPLEPROC) (GLenum object, GLuint name);

typedef void ( * PFNGLBUFFERPARAMETERIAPPLEPROC) (GLenum target, GLenum pname, GLint param);
typedef void ( * PFNGLFLUSHMAPPEDBUFFERRANGEAPPLEPROC) (GLenum target, GLintptr offset, GLsizeiptr size);

typedef void ( * PFNGLGETOBJECTPARAMETERIVAPPLEPROC) (GLenum objectType, GLuint name, GLenum pname, GLint* params);
typedef GLenum ( * PFNGLOBJECTPURGEABLEAPPLEPROC) (GLenum objectType, GLuint name, GLenum option);
typedef GLenum ( * PFNGLOBJECTUNPURGEABLEAPPLEPROC) (GLenum objectType, GLuint name, GLenum option);

typedef void ( * PFNGLGETTEXPARAMETERPOINTERVAPPLEPROC) (GLenum target, GLenum pname, GLvoid **params);
typedef void ( * PFNGLTEXTURERANGEAPPLEPROC) (GLenum target, GLsizei length, GLvoid *pointer);

typedef void ( * PFNGLBINDVERTEXARRAYAPPLEPROC) (GLuint array);
typedef void ( * PFNGLDELETEVERTEXARRAYSAPPLEPROC) (GLsizei n, const GLuint* arrays);
typedef void ( * PFNGLGENVERTEXARRAYSAPPLEPROC) (GLsizei n, const GLuint* arrays);
typedef GLboolean ( * PFNGLISVERTEXARRAYAPPLEPROC) (GLuint array);

typedef void ( * PFNGLFLUSHVERTEXARRAYRANGEAPPLEPROC) (GLsizei length, void* pointer);
typedef void ( * PFNGLVERTEXARRAYPARAMETERIAPPLEPROC) (GLenum pname, GLint param);
typedef void ( * PFNGLVERTEXARRAYRANGEAPPLEPROC) (GLsizei length, void* pointer);

typedef void ( * PFNGLDISABLEVERTEXATTRIBAPPLEPROC) (GLuint index, GLenum pname);
typedef void ( * PFNGLENABLEVERTEXATTRIBAPPLEPROC) (GLuint index, GLenum pname);
typedef GLboolean ( * PFNGLISVERTEXATTRIBENABLEDAPPLEPROC) (GLuint index, GLenum pname);
typedef void ( * PFNGLMAPVERTEXATTRIB1DAPPLEPROC) (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble* points);
typedef void ( * PFNGLMAPVERTEXATTRIB1FAPPLEPROC) (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat* points);
typedef void ( * PFNGLMAPVERTEXATTRIB2DAPPLEPROC) (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble* points);
typedef void ( * PFNGLMAPVERTEXATTRIB2FAPPLEPROC) (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat* points);

typedef void ( * PFNGLCLEARDEPTHFPROC) (GLclampf d);
typedef void ( * PFNGLDEPTHRANGEFPROC) (GLclampf n, GLclampf f);
typedef void ( * PFNGLGETSHADERPRECISIONFORMATPROC) (GLenum shadertype, GLenum precisiontype, GLint* range, GLint *precision);
typedef void ( * PFNGLRELEASESHADERCOMPILERPROC) (void);
typedef void ( * PFNGLSHADERBINARYPROC) (GLsizei count, const GLuint* shaders, GLenum binaryformat, const GLvoid*binary, GLsizei length);

typedef void ( * PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC) (GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance);
typedef void ( * PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount, GLuint baseinstance);
typedef void ( * PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount, GLint basevertex, GLuint baseinstance);

typedef void ( * PFNGLBINDFRAGDATALOCATIONINDEXEDPROC) (GLuint program, GLuint colorNumber, GLuint index, const char * name);
typedef GLint ( * PFNGLGETFRAGDATAINDEXPROC) (GLuint program, const char * name);

typedef struct _cl_context *cl_context;
typedef struct _cl_event *cl_event;

typedef GLsync ( * PFNGLCREATESYNCFROMCLEVENTARBPROC) (cl_context context, cl_event event, GLbitfield flags);

typedef void ( * PFNGLCLAMPCOLORARBPROC) (GLenum target, GLenum clamp);

typedef void ( * PFNGLCOPYBUFFERSUBDATAPROC) (GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);

typedef void ( *GLDEBUGPROCARB)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);

typedef void ( * PFNGLDEBUGMESSAGECALLBACKARBPROC) (GLDEBUGPROCARB callback, void* userParam);
typedef void ( * PFNGLDEBUGMESSAGECONTROLARBPROC) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
typedef void ( * PFNGLDEBUGMESSAGEINSERTARBPROC) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* buf);
typedef GLuint ( * PFNGLGETDEBUGMESSAGELOGARBPROC) (GLuint count, GLsizei bufsize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, char* messageLog);

typedef void ( * PFNGLDRAWBUFFERSARBPROC) (GLsizei n, const GLenum* bufs);

typedef void ( * PFNGLBLENDEQUATIONSEPARATEIARBPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void ( * PFNGLBLENDEQUATIONIARBPROC) (GLuint buf, GLenum mode);
typedef void ( * PFNGLBLENDFUNCSEPARATEIARBPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void ( * PFNGLBLENDFUNCIARBPROC) (GLuint buf, GLenum src, GLenum dst);

typedef void ( * PFNGLDRAWELEMENTSBASEVERTEXPROC) (GLenum mode, GLsizei count, GLenum type, void* indices, GLint basevertex);
typedef void ( * PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount, GLint basevertex);
typedef void ( * PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, void* indices, GLint basevertex);
typedef void ( * PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC) (GLenum mode, GLsizei* count, GLenum type, GLvoid**indices, GLsizei primcount, GLint *basevertex);

typedef void ( * PFNGLDRAWARRAYSINDIRECTPROC) (GLenum mode, const void* indirect);
typedef void ( * PFNGLDRAWELEMENTSINDIRECTPROC) (GLenum mode, GLenum type, const void* indirect);

typedef void ( * PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef void ( * PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
typedef void ( * PFNGLBLITFRAMEBUFFERPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef GLenum ( * PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef void ( * PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint* framebuffers);
typedef void ( * PFNGLDELETERENDERBUFFERSPROC) (GLsizei n, const GLuint* renderbuffers);
typedef void ( * PFNGLFRAMEBUFFERRENDERBUFFERPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void ( * PFNGLFRAMEBUFFERTEXTURE1DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void ( * PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void ( * PFNGLFRAMEBUFFERTEXTURE3DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer);
typedef void ( * PFNGLFRAMEBUFFERTEXTURELAYERPROC) (GLenum target,GLenum attachment, GLuint texture,GLint level,GLint layer);
typedef void ( * PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint* framebuffers);
typedef void ( * PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint* renderbuffers);
typedef void ( * PFNGLGENERATEMIPMAPPROC) (GLenum target);
typedef void ( * PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
typedef void ( * PFNGLGETRENDERBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean ( * PFNGLISFRAMEBUFFERPROC) (GLuint framebuffer);
typedef GLboolean ( * PFNGLISRENDERBUFFERPROC) (GLuint renderbuffer);
typedef void ( * PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void ( * PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

typedef void ( * PFNGLFRAMEBUFFERTEXTUREARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void ( * PFNGLFRAMEBUFFERTEXTUREFACEARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void ( * PFNGLFRAMEBUFFERTEXTURELAYERARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void ( * PFNGLPROGRAMPARAMETERIARBPROC) (GLuint program, GLenum pname, GLint value);

typedef void ( * PFNGLGETPROGRAMBINARYPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLenum *binaryFormat, GLvoid*binary);
typedef void ( * PFNGLPROGRAMBINARYPROC) (GLuint program, GLenum binaryFormat, const void* binary, GLsizei length);
typedef void ( * PFNGLPROGRAMPARAMETERIPROC) (GLuint program, GLenum pname, GLint value);

typedef void ( * PFNGLGETUNIFORMDVPROC) (GLuint program, GLint location, GLdouble* params);
typedef void ( * PFNGLPROGRAMUNIFORM1DEXTPROC) (GLuint program, GLint location, GLdouble x);
typedef void ( * PFNGLPROGRAMUNIFORM1DVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORM2DEXTPROC) (GLuint program, GLint location, GLdouble x, GLdouble y);
typedef void ( * PFNGLPROGRAMUNIFORM2DVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORM3DEXTPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLPROGRAMUNIFORM3DVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORM4DEXTPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void ( * PFNGLPROGRAMUNIFORM4DVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX2DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX2X3DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX2X4DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX3DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX3X2DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX3X4DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX4DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX4X2DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX4X3DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLUNIFORM1DPROC) (GLint location, GLdouble x);
typedef void ( * PFNGLUNIFORM1DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void ( * PFNGLUNIFORM2DPROC) (GLint location, GLdouble x, GLdouble y);
typedef void ( * PFNGLUNIFORM2DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void ( * PFNGLUNIFORM3DPROC) (GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLUNIFORM3DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void ( * PFNGLUNIFORM4DPROC) (GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void ( * PFNGLUNIFORM4DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void ( * PFNGLUNIFORMMATRIX2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLUNIFORMMATRIX2X3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLUNIFORMMATRIX2X4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLUNIFORMMATRIX3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLUNIFORMMATRIX3X2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLUNIFORMMATRIX3X4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLUNIFORMMATRIX4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLUNIFORMMATRIX4X2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLUNIFORMMATRIX4X3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);

typedef void ( * PFNGLCOLORSUBTABLEPROC) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
typedef void ( * PFNGLCOLORTABLEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
typedef void ( * PFNGLCOLORTABLEPARAMETERFVPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void ( * PFNGLCOLORTABLEPARAMETERIVPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void ( * PFNGLCONVOLUTIONFILTER1DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
typedef void ( * PFNGLCONVOLUTIONFILTER2DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
typedef void ( * PFNGLCONVOLUTIONPARAMETERFPROC) (GLenum target, GLenum pname, GLfloat params);
typedef void ( * PFNGLCONVOLUTIONPARAMETERFVPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void ( * PFNGLCONVOLUTIONPARAMETERIPROC) (GLenum target, GLenum pname, GLint params);
typedef void ( * PFNGLCONVOLUTIONPARAMETERIVPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void ( * PFNGLCOPYCOLORSUBTABLEPROC) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
typedef void ( * PFNGLCOPYCOLORTABLEPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void ( * PFNGLCOPYCONVOLUTIONFILTER1DPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void ( * PFNGLCOPYCONVOLUTIONFILTER2DPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void ( * PFNGLGETCOLORTABLEPROC) (GLenum target, GLenum format, GLenum type, GLvoid *table);
typedef void ( * PFNGLGETCOLORTABLEPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void ( * PFNGLGETCOLORTABLEPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void ( * PFNGLGETCONVOLUTIONFILTERPROC) (GLenum target, GLenum format, GLenum type, GLvoid *image);
typedef void ( * PFNGLGETCONVOLUTIONPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void ( * PFNGLGETCONVOLUTIONPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void ( * PFNGLGETHISTOGRAMPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
typedef void ( * PFNGLGETHISTOGRAMPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void ( * PFNGLGETHISTOGRAMPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void ( * PFNGLGETMINMAXPROC) (GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values);
typedef void ( * PFNGLGETMINMAXPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void ( * PFNGLGETMINMAXPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void ( * PFNGLGETSEPARABLEFILTERPROC) (GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
typedef void ( * PFNGLHISTOGRAMPROC) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
typedef void ( * PFNGLMINMAXPROC) (GLenum target, GLenum internalformat, GLboolean sink);
typedef void ( * PFNGLRESETHISTOGRAMPROC) (GLenum target);
typedef void ( * PFNGLRESETMINMAXPROC) (GLenum target);
typedef void ( * PFNGLSEPARABLEFILTER2DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);

typedef void ( * PFNGLDRAWARRAYSINSTANCEDARBPROC) (GLenum mode, GLint first, GLsizei count, GLsizei primcount);
typedef void ( * PFNGLDRAWELEMENTSINSTANCEDARBPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount);
typedef void ( * PFNGLVERTEXATTRIBDIVISORARBPROC) (GLuint index, GLuint divisor);

typedef void ( * PFNGLGETINTERNALFORMATIVPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params);

typedef void ( * PFNGLFLUSHMAPPEDBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length);
typedef GLvoid * ( * PFNGLMAPBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);

typedef void ( * PFNGLCURRENTPALETTEMATRIXARBPROC) (GLint index);
typedef void ( * PFNGLMATRIXINDEXPOINTERARBPROC) (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
typedef void ( * PFNGLMATRIXINDEXUBVARBPROC) (GLint size, GLubyte *indices);
typedef void ( * PFNGLMATRIXINDEXUIVARBPROC) (GLint size, GLuint *indices);
typedef void ( * PFNGLMATRIXINDEXUSVARBPROC) (GLint size, GLushort *indices);

typedef void ( * PFNGLSAMPLECOVERAGEARBPROC) (GLclampf value, GLboolean invert);

typedef void ( * PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef void ( * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);
typedef void ( * PFNGLMULTITEXCOORD1DARBPROC) (GLenum target, GLdouble s);
typedef void ( * PFNGLMULTITEXCOORD1DVARBPROC) (GLenum target, const GLdouble *v);
typedef void ( * PFNGLMULTITEXCOORD1FARBPROC) (GLenum target, GLfloat s);
typedef void ( * PFNGLMULTITEXCOORD1FVARBPROC) (GLenum target, const GLfloat *v);
typedef void ( * PFNGLMULTITEXCOORD1IARBPROC) (GLenum target, GLint s);
typedef void ( * PFNGLMULTITEXCOORD1IVARBPROC) (GLenum target, const GLint *v);
typedef void ( * PFNGLMULTITEXCOORD1SARBPROC) (GLenum target, GLshort s);
typedef void ( * PFNGLMULTITEXCOORD1SVARBPROC) (GLenum target, const GLshort *v);
typedef void ( * PFNGLMULTITEXCOORD2DARBPROC) (GLenum target, GLdouble s, GLdouble t);
typedef void ( * PFNGLMULTITEXCOORD2DVARBPROC) (GLenum target, const GLdouble *v);
typedef void ( * PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void ( * PFNGLMULTITEXCOORD2FVARBPROC) (GLenum target, const GLfloat *v);
typedef void ( * PFNGLMULTITEXCOORD2IARBPROC) (GLenum target, GLint s, GLint t);
typedef void ( * PFNGLMULTITEXCOORD2IVARBPROC) (GLenum target, const GLint *v);
typedef void ( * PFNGLMULTITEXCOORD2SARBPROC) (GLenum target, GLshort s, GLshort t);
typedef void ( * PFNGLMULTITEXCOORD2SVARBPROC) (GLenum target, const GLshort *v);
typedef void ( * PFNGLMULTITEXCOORD3DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void ( * PFNGLMULTITEXCOORD3DVARBPROC) (GLenum target, const GLdouble *v);
typedef void ( * PFNGLMULTITEXCOORD3FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void ( * PFNGLMULTITEXCOORD3FVARBPROC) (GLenum target, const GLfloat *v);
typedef void ( * PFNGLMULTITEXCOORD3IARBPROC) (GLenum target, GLint s, GLint t, GLint r);
typedef void ( * PFNGLMULTITEXCOORD3IVARBPROC) (GLenum target, const GLint *v);
typedef void ( * PFNGLMULTITEXCOORD3SARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void ( * PFNGLMULTITEXCOORD3SVARBPROC) (GLenum target, const GLshort *v);
typedef void ( * PFNGLMULTITEXCOORD4DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void ( * PFNGLMULTITEXCOORD4DVARBPROC) (GLenum target, const GLdouble *v);
typedef void ( * PFNGLMULTITEXCOORD4FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void ( * PFNGLMULTITEXCOORD4FVARBPROC) (GLenum target, const GLfloat *v);
typedef void ( * PFNGLMULTITEXCOORD4IARBPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void ( * PFNGLMULTITEXCOORD4IVARBPROC) (GLenum target, const GLint *v);
typedef void ( * PFNGLMULTITEXCOORD4SARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void ( * PFNGLMULTITEXCOORD4SVARBPROC) (GLenum target, const GLshort *v);

typedef void ( * PFNGLBEGINQUERYARBPROC) (GLenum target, GLuint id);
typedef void ( * PFNGLDELETEQUERIESARBPROC) (GLsizei n, const GLuint* ids);
typedef void ( * PFNGLENDQUERYARBPROC) (GLenum target);
typedef void ( * PFNGLGENQUERIESARBPROC) (GLsizei n, GLuint* ids);
typedef void ( * PFNGLGETQUERYOBJECTIVARBPROC) (GLuint id, GLenum pname, GLint* params);
typedef void ( * PFNGLGETQUERYOBJECTUIVARBPROC) (GLuint id, GLenum pname, GLuint* params);
typedef void ( * PFNGLGETQUERYIVARBPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean ( * PFNGLISQUERYARBPROC) (GLuint id);

typedef void ( * PFNGLPOINTPARAMETERFARBPROC) (GLenum pname, GLfloat param);
typedef void ( * PFNGLPOINTPARAMETERFVARBPROC) (GLenum pname, const GLfloat* params);

typedef void ( * PFNGLPROVOKINGVERTEXPROC) (GLenum mode);

typedef GLenum ( * PFNGLGETGRAPHICSRESETSTATUSARBPROC) (void);
typedef void ( * PFNGLGETNCOLORTABLEARBPROC) (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void* table);
typedef void ( * PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC) (GLenum target, GLint lod, GLsizei bufSize, void* img);
typedef void ( * PFNGLGETNCONVOLUTIONFILTERARBPROC) (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void* image);
typedef void ( * PFNGLGETNHISTOGRAMARBPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void* values);
typedef void ( * PFNGLGETNMAPDVARBPROC) (GLenum target, GLenum query, GLsizei bufSize, GLdouble* v);
typedef void ( * PFNGLGETNMAPFVARBPROC) (GLenum target, GLenum query, GLsizei bufSize, GLfloat* v);
typedef void ( * PFNGLGETNMAPIVARBPROC) (GLenum target, GLenum query, GLsizei bufSize, GLint* v);
typedef void ( * PFNGLGETNMINMAXARBPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void* values);
typedef void ( * PFNGLGETNPIXELMAPFVARBPROC) (GLenum map, GLsizei bufSize, GLfloat* values);
typedef void ( * PFNGLGETNPIXELMAPUIVARBPROC) (GLenum map, GLsizei bufSize, GLuint* values);
typedef void ( * PFNGLGETNPIXELMAPUSVARBPROC) (GLenum map, GLsizei bufSize, GLushort* values);
typedef void ( * PFNGLGETNPOLYGONSTIPPLEARBPROC) (GLsizei bufSize, GLubyte* pattern);
typedef void ( * PFNGLGETNSEPARABLEFILTERARBPROC) (GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void* row, GLsizei columnBufSize, GLvoid*column, GLvoid*span);
typedef void ( * PFNGLGETNTEXIMAGEARBPROC) (GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void* img);
typedef void ( * PFNGLGETNUNIFORMDVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLdouble* params);
typedef void ( * PFNGLGETNUNIFORMFVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLfloat* params);
typedef void ( * PFNGLGETNUNIFORMIVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLint* params);
typedef void ( * PFNGLGETNUNIFORMUIVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLuint* params);
typedef void ( * PFNGLREADNPIXELSARBPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void* data);

typedef void ( * PFNGLMINSAMPLESHADINGARBPROC) (GLclampf value);

typedef void ( * PFNGLBINDSAMPLERPROC) (GLuint unit, GLuint sampler);
typedef void ( * PFNGLDELETESAMPLERSPROC) (GLsizei count, const GLuint * samplers);
typedef void ( * PFNGLGENSAMPLERSPROC) (GLsizei count, GLuint* samplers);
typedef void ( * PFNGLGETSAMPLERPARAMETERIIVPROC) (GLuint sampler, GLenum pname, GLint* params);
typedef void ( * PFNGLGETSAMPLERPARAMETERIUIVPROC) (GLuint sampler, GLenum pname, GLuint* params);
typedef void ( * PFNGLGETSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETSAMPLERPARAMETERIVPROC) (GLuint sampler, GLenum pname, GLint* params);
typedef GLboolean ( * PFNGLISSAMPLERPROC) (GLuint sampler);
typedef void ( * PFNGLSAMPLERPARAMETERIIVPROC) (GLuint sampler, GLenum pname, const GLint* params);
typedef void ( * PFNGLSAMPLERPARAMETERIUIVPROC) (GLuint sampler, GLenum pname, const GLuint* params);
typedef void ( * PFNGLSAMPLERPARAMETERFPROC) (GLuint sampler, GLenum pname, GLfloat param);
typedef void ( * PFNGLSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLSAMPLERPARAMETERIPROC) (GLuint sampler, GLenum pname, GLint param);
typedef void ( * PFNGLSAMPLERPARAMETERIVPROC) (GLuint sampler, GLenum pname, const GLint* params);

typedef void ( * PFNGLACTIVESHADERPROGRAMPROC) (GLuint pipeline, GLuint program);
typedef void ( * PFNGLBINDPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef GLuint ( * PFNGLCREATESHADERPROGRAMVPROC) (GLenum type, GLsizei count, const char ** strings);
typedef void ( * PFNGLDELETEPROGRAMPIPELINESPROC) (GLsizei n, const GLuint* pipelines);
typedef void ( * PFNGLGENPROGRAMPIPELINESPROC) (GLsizei n, GLuint* pipelines);
typedef void ( * PFNGLGETPROGRAMPIPELINEINFOLOGPROC) (GLuint pipeline, GLsizei bufSize, GLsizei* length, char *infoLog);
typedef void ( * PFNGLGETPROGRAMPIPELINEIVPROC) (GLuint pipeline, GLenum pname, GLint* params);
typedef GLboolean ( * PFNGLISPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void ( * PFNGLPROGRAMUNIFORM1DPROC) (GLuint program, GLint location, GLdouble x);
typedef void ( * PFNGLPROGRAMUNIFORM1DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORM1FPROC) (GLuint program, GLint location, GLfloat x);
typedef void ( * PFNGLPROGRAMUNIFORM1FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORM1IPROC) (GLuint program, GLint location, GLint x);
typedef void ( * PFNGLPROGRAMUNIFORM1IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLPROGRAMUNIFORM1UIPROC) (GLuint program, GLint location, GLuint x);
typedef void ( * PFNGLPROGRAMUNIFORM1UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void ( * PFNGLPROGRAMUNIFORM2DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y);
typedef void ( * PFNGLPROGRAMUNIFORM2DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORM2FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y);
typedef void ( * PFNGLPROGRAMUNIFORM2FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORM2IPROC) (GLuint program, GLint location, GLint x, GLint y);
typedef void ( * PFNGLPROGRAMUNIFORM2IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLPROGRAMUNIFORM2UIPROC) (GLuint program, GLint location, GLuint x, GLuint y);
typedef void ( * PFNGLPROGRAMUNIFORM2UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void ( * PFNGLPROGRAMUNIFORM3DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLPROGRAMUNIFORM3DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORM3FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLPROGRAMUNIFORM3FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORM3IPROC) (GLuint program, GLint location, GLint x, GLint y, GLint z);
typedef void ( * PFNGLPROGRAMUNIFORM3IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLPROGRAMUNIFORM3UIPROC) (GLuint program, GLint location, GLuint x, GLuint y, GLuint z);
typedef void ( * PFNGLPROGRAMUNIFORM3UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void ( * PFNGLPROGRAMUNIFORM4DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void ( * PFNGLPROGRAMUNIFORM4DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORM4FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void ( * PFNGLPROGRAMUNIFORM4FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORM4IPROC) (GLuint program, GLint location, GLint x, GLint y, GLint z, GLint w);
typedef void ( * PFNGLPROGRAMUNIFORM4IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLPROGRAMUNIFORM4UIPROC) (GLuint program, GLint location, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void ( * PFNGLPROGRAMUNIFORM4UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLUSEPROGRAMSTAGESPROC) (GLuint pipeline, GLbitfield stages, GLuint program);
typedef void ( * PFNGLVALIDATEPROGRAMPIPELINEPROC) (GLuint pipeline);

typedef void ( * PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC) (GLuint program, GLuint bufferIndex, GLenum pname, GLint* params);

typedef void ( * PFNGLBINDIMAGETEXTUREPROC) (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
typedef void ( * PFNGLMEMORYBARRIERPROC) (GLbitfield barriers);

typedef char GLcharARB;
typedef unsigned int GLhandleARB;

typedef void ( * PFNGLATTACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB obj);
typedef void ( * PFNGLCOMPILESHADERARBPROC) (GLhandleARB shaderObj);
typedef GLhandleARB ( * PFNGLCREATEPROGRAMOBJECTARBPROC) (void);
typedef GLhandleARB ( * PFNGLCREATESHADEROBJECTARBPROC) (GLenum shaderType);
typedef void ( * PFNGLDELETEOBJECTARBPROC) (GLhandleARB obj);
typedef void ( * PFNGLDETACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB attachedObj);
typedef void ( * PFNGLGETACTIVEUNIFORMARBPROC) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei* length, GLint *size, GLenum *type, GLcharARB *name);
typedef void ( * PFNGLGETATTACHEDOBJECTSARBPROC) (GLhandleARB containerObj, GLsizei maxCount, GLsizei* count, GLhandleARB *obj);
typedef GLhandleARB ( * PFNGLGETHANDLEARBPROC) (GLenum pname);
typedef void ( * PFNGLGETINFOLOGARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *infoLog);
typedef void ( * PFNGLGETOBJECTPARAMETERFVARBPROC) (GLhandleARB obj, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETOBJECTPARAMETERIVARBPROC) (GLhandleARB obj, GLenum pname, GLint* params);
typedef void ( * PFNGLGETSHADERSOURCEARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *source);
typedef GLint ( * PFNGLGETUNIFORMLOCATIONARBPROC) (GLhandleARB programObj, const GLcharARB* name);
typedef void ( * PFNGLGETUNIFORMFVARBPROC) (GLhandleARB programObj, GLint location, GLfloat* params);
typedef void ( * PFNGLGETUNIFORMIVARBPROC) (GLhandleARB programObj, GLint location, GLint* params);
typedef void ( * PFNGLLINKPROGRAMARBPROC) (GLhandleARB programObj);
typedef void ( * PFNGLSHADERSOURCEARBPROC) (GLhandleARB shaderObj, GLsizei count, const GLcharARB ** string, const GLint *length);
typedef void ( * PFNGLUNIFORM1FARBPROC) (GLint location, GLfloat v0);
typedef void ( * PFNGLUNIFORM1FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLUNIFORM1IARBPROC) (GLint location, GLint v0);
typedef void ( * PFNGLUNIFORM1IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLUNIFORM2FARBPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void ( * PFNGLUNIFORM2FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLUNIFORM2IARBPROC) (GLint location, GLint v0, GLint v1);
typedef void ( * PFNGLUNIFORM2IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLUNIFORM3FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void ( * PFNGLUNIFORM3FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLUNIFORM3IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void ( * PFNGLUNIFORM3IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLUNIFORM4FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void ( * PFNGLUNIFORM4FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLUNIFORM4IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void ( * PFNGLUNIFORM4IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLUNIFORMMATRIX2FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLUNIFORMMATRIX3FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLUNIFORMMATRIX4FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLUSEPROGRAMOBJECTARBPROC) (GLhandleARB programObj);
typedef void ( * PFNGLVALIDATEPROGRAMARBPROC) (GLhandleARB programObj);

typedef void ( * PFNGLGETACTIVESUBROUTINENAMEPROC) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, char *name);
typedef void ( * PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, char *name);
typedef void ( * PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC) (GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint* values);
typedef void ( * PFNGLGETPROGRAMSTAGEIVPROC) (GLuint program, GLenum shadertype, GLenum pname, GLint* values);
typedef GLuint ( * PFNGLGETSUBROUTINEINDEXPROC) (GLuint program, GLenum shadertype, const char* name);
typedef GLint ( * PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC) (GLuint program, GLenum shadertype, const char* name);
typedef void ( * PFNGLGETUNIFORMSUBROUTINEUIVPROC) (GLenum shadertype, GLint location, GLuint* params);
typedef void ( * PFNGLUNIFORMSUBROUTINESUIVPROC) (GLenum shadertype, GLsizei count, const GLuint* indices);

typedef void ( * PFNGLCOMPILESHADERINCLUDEARBPROC) (GLuint shader, GLsizei count, const char ** path, const GLint *length);
typedef void ( * PFNGLDELETENAMEDSTRINGARBPROC) (GLint namelen, const char* name);
typedef void ( * PFNGLGETNAMEDSTRINGARBPROC) (GLint namelen, const char* name, GLsizei bufSize, GLint *stringlen, char *string);
typedef void ( * PFNGLGETNAMEDSTRINGIVARBPROC) (GLint namelen, const char* name, GLenum pname, GLint *params);
typedef GLboolean ( * PFNGLISNAMEDSTRINGARBPROC) (GLint namelen, const char* name);
typedef void ( * PFNGLNAMEDSTRINGARBPROC) (GLenum type, GLint namelen, const char* name, GLint stringlen, const char *string);

typedef GLenum ( * PFNGLCLIENTWAITSYNCPROC) (GLsync GLsync,GLbitfield flags,GLuint64 timeout);
typedef void ( * PFNGLDELETESYNCPROC) (GLsync GLsync);
typedef GLsync ( * PFNGLFENCESYNCPROC) (GLenum condition,GLbitfield flags);
typedef void ( * PFNGLGETINTEGER64VPROC) (GLenum pname, GLint64* params);
typedef void ( * PFNGLGETSYNCIVPROC) (GLsync GLsync,GLenum pname,GLsizei bufSize,GLsizei* length, GLint *values);
typedef GLboolean ( * PFNGLISSYNCPROC) (GLsync GLsync);
typedef void ( * PFNGLWAITSYNCPROC) (GLsync GLsync,GLbitfield flags,GLuint64 timeout);

typedef void ( * PFNGLPATCHPARAMETERFVPROC) (GLenum pname, const GLfloat* values);
typedef void ( * PFNGLPATCHPARAMETERIPROC) (GLenum pname, GLint value);

typedef void ( * PFNGLTEXBUFFERARBPROC) (GLenum target, GLenum internalformat, GLuint buffer);

typedef void ( * PFNGLCOMPRESSEDTEXIMAGE1DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDTEXIMAGE2DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDTEXIMAGE3DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
typedef void ( * PFNGLGETCOMPRESSEDTEXIMAGEARBPROC) (GLenum target, GLint lod, void* img);

typedef void ( * PFNGLGETMULTISAMPLEFVPROC) (GLenum pname, GLuint index, GLfloat* val);
typedef void ( * PFNGLSAMPLEMASKIPROC) (GLuint index, GLbitfield mask);
typedef void ( * PFNGLTEXIMAGE2DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void ( * PFNGLTEXIMAGE3DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);

typedef void ( * PFNGLTEXSTORAGE1DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void ( * PFNGLTEXSTORAGE2DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void ( * PFNGLTEXSTORAGE3DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void ( * PFNGLTEXTURESTORAGE1DEXTPROC) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void ( * PFNGLTEXTURESTORAGE2DEXTPROC) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void ( * PFNGLTEXTURESTORAGE3DEXTPROC) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);

typedef void ( * PFNGLGETQUERYOBJECTI64VPROC) (GLuint id, GLenum pname, GLint64* params);
typedef void ( * PFNGLGETQUERYOBJECTUI64VPROC) (GLuint id, GLenum pname, GLuint64* params);
typedef void ( * PFNGLQUERYCOUNTERPROC) (GLuint id, GLenum target);

typedef void ( * PFNGLBINDTRANSFORMFEEDBACKPROC) (GLenum target, GLuint id);
typedef void ( * PFNGLDELETETRANSFORMFEEDBACKSPROC) (GLsizei n, const GLuint* ids);
typedef void ( * PFNGLDRAWTRANSFORMFEEDBACKPROC) (GLenum mode, GLuint id);
typedef void ( * PFNGLGENTRANSFORMFEEDBACKSPROC) (GLsizei n, GLuint* ids);
typedef GLboolean ( * PFNGLISTRANSFORMFEEDBACKPROC) (GLuint id);
typedef void ( * PFNGLPAUSETRANSFORMFEEDBACKPROC) (void);
typedef void ( * PFNGLRESUMETRANSFORMFEEDBACKPROC) (void);

typedef void ( * PFNGLBEGINQUERYINDEXEDPROC) (GLenum target, GLuint index, GLuint id);
typedef void ( * PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC) (GLenum mode, GLuint id, GLuint stream);
typedef void ( * PFNGLENDQUERYINDEXEDPROC) (GLenum target, GLuint index);
typedef void ( * PFNGLGETQUERYINDEXEDIVPROC) (GLenum target, GLuint index, GLenum pname, GLint* params);

typedef void ( * PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC) (GLenum mode, GLuint id, GLsizei primcount);
typedef void ( * PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC) (GLenum mode, GLuint id, GLuint stream, GLsizei primcount);

typedef void ( * PFNGLLOADTRANSPOSEMATRIXDARBPROC) (GLdouble m[16]);
typedef void ( * PFNGLLOADTRANSPOSEMATRIXFARBPROC) (GLfloat m[16]);
typedef void ( * PFNGLMULTTRANSPOSEMATRIXDARBPROC) (GLdouble m[16]);
typedef void ( * PFNGLMULTTRANSPOSEMATRIXFARBPROC) (GLfloat m[16]);

typedef void ( * PFNGLBINDBUFFERBASEPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void ( * PFNGLBINDBUFFERRANGEPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void ( * PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, char* uniformBlockName);
typedef void ( * PFNGLGETACTIVEUNIFORMBLOCKIVPROC) (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
typedef void ( * PFNGLGETACTIVEUNIFORMNAMEPROC) (GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, char* uniformName);
typedef void ( * PFNGLGETACTIVEUNIFORMSIVPROC) (GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
typedef void ( * PFNGLGETINTEGERI_VPROC) (GLenum target, GLuint index, GLint* data);
typedef GLuint ( * PFNGLGETUNIFORMBLOCKINDEXPROC) (GLuint program, const char* uniformBlockName);
typedef void ( * PFNGLGETUNIFORMINDICESPROC) (GLuint program, GLsizei uniformCount, const char** uniformNames, GLuint* uniformIndices);
typedef void ( * PFNGLUNIFORMBLOCKBINDINGPROC) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

typedef void ( * PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void ( * PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint* arrays);
typedef void ( * PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
typedef GLboolean ( * PFNGLISVERTEXARRAYPROC) (GLuint array);

typedef void ( * PFNGLGETVERTEXATTRIBLDVPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void ( * PFNGLVERTEXATTRIBL1DPROC) (GLuint index, GLdouble x);
typedef void ( * PFNGLVERTEXATTRIBL1DVPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIBL2DPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void ( * PFNGLVERTEXATTRIBL2DVPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIBL3DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLVERTEXATTRIBL3DVPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIBL4DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void ( * PFNGLVERTEXATTRIBL4DVPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIBLPOINTERPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);

typedef void ( * PFNGLVERTEXBLENDARBPROC) (GLint count);
typedef void ( * PFNGLWEIGHTPOINTERARBPROC) (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
typedef void ( * PFNGLWEIGHTBVARBPROC) (GLint size, GLbyte *weights);
typedef void ( * PFNGLWEIGHTDVARBPROC) (GLint size, GLdouble *weights);
typedef void ( * PFNGLWEIGHTFVARBPROC) (GLint size, GLfloat *weights);
typedef void ( * PFNGLWEIGHTIVARBPROC) (GLint size, GLint *weights);
typedef void ( * PFNGLWEIGHTSVARBPROC) (GLint size, GLshort *weights);
typedef void ( * PFNGLWEIGHTUBVARBPROC) (GLint size, GLubyte *weights);
typedef void ( * PFNGLWEIGHTUIVARBPROC) (GLint size, GLuint *weights);
typedef void ( * PFNGLWEIGHTUSVARBPROC) (GLint size, GLushort *weights);

typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;

typedef void ( * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void ( * PFNGLBUFFERDATAARBPROC) (GLenum target, GLsizeiptrARB size, const GLvoid* data, GLenum usage);
typedef void ( * PFNGLBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid* data);
typedef void ( * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint* buffers);
typedef void ( * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint* buffers);
typedef void ( * PFNGLGETBUFFERPARAMETERIVARBPROC) (GLenum target, GLenum pname, GLint* params);
typedef void ( * PFNGLGETBUFFERPOINTERVARBPROC) (GLenum target, GLenum pname, GLvoid** params);
typedef void ( * PFNGLGETBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid* data);
typedef GLboolean ( * PFNGLISBUFFERARBPROC) (GLuint buffer);
typedef GLvoid * ( * PFNGLMAPBUFFERARBPROC) (GLenum target, GLenum access);
typedef GLboolean ( * PFNGLUNMAPBUFFERARBPROC) (GLenum target);

typedef void ( * PFNGLBINDPROGRAMARBPROC) (GLenum target, GLuint program);
typedef void ( * PFNGLDELETEPROGRAMSARBPROC) (GLsizei n, const GLuint* programs);
typedef void ( * PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void ( * PFNGLENABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void ( * PFNGLGENPROGRAMSARBPROC) (GLsizei n, GLuint* programs);
typedef void ( * PFNGLGETPROGRAMENVPARAMETERDVARBPROC) (GLenum target, GLuint index, GLdouble* params);
typedef void ( * PFNGLGETPROGRAMENVPARAMETERFVARBPROC) (GLenum target, GLuint index, GLfloat* params);
typedef void ( * PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC) (GLenum target, GLuint index, GLdouble* params);
typedef void ( * PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC) (GLenum target, GLuint index, GLfloat* params);
typedef void ( * PFNGLGETPROGRAMSTRINGARBPROC) (GLenum target, GLenum pname, void* string);
typedef void ( * PFNGLGETPROGRAMIVARBPROC) (GLenum target, GLenum pname, GLint* params);
typedef void ( * PFNGLGETVERTEXATTRIBPOINTERVARBPROC) (GLuint index, GLenum pname, GLvoid** pointer);
typedef void ( * PFNGLGETVERTEXATTRIBDVARBPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void ( * PFNGLGETVERTEXATTRIBFVARBPROC) (GLuint index, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETVERTEXATTRIBIVARBPROC) (GLuint index, GLenum pname, GLint* params);
typedef GLboolean ( * PFNGLISPROGRAMARBPROC) (GLuint program);
typedef void ( * PFNGLPROGRAMENVPARAMETER4DARBPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void ( * PFNGLPROGRAMENVPARAMETER4DVARBPROC) (GLenum target, GLuint index, const GLdouble* params);
typedef void ( * PFNGLPROGRAMENVPARAMETER4FARBPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void ( * PFNGLPROGRAMENVPARAMETER4FVARBPROC) (GLenum target, GLuint index, const GLfloat* params);
typedef void ( * PFNGLPROGRAMLOCALPARAMETER4DARBPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void ( * PFNGLPROGRAMLOCALPARAMETER4DVARBPROC) (GLenum target, GLuint index, const GLdouble* params);
typedef void ( * PFNGLPROGRAMLOCALPARAMETER4FARBPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void ( * PFNGLPROGRAMLOCALPARAMETER4FVARBPROC) (GLenum target, GLuint index, const GLfloat* params);
typedef void ( * PFNGLPROGRAMSTRINGARBPROC) (GLenum target, GLenum format, GLsizei len, const void* string);
typedef void ( * PFNGLVERTEXATTRIB1DARBPROC) (GLuint index, GLdouble x);
typedef void ( * PFNGLVERTEXATTRIB1DVARBPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIB1FARBPROC) (GLuint index, GLfloat x);
typedef void ( * PFNGLVERTEXATTRIB1FVARBPROC) (GLuint index, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIB1SARBPROC) (GLuint index, GLshort x);
typedef void ( * PFNGLVERTEXATTRIB1SVARBPROC) (GLuint index, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIB2DARBPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void ( * PFNGLVERTEXATTRIB2DVARBPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIB2FARBPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void ( * PFNGLVERTEXATTRIB2FVARBPROC) (GLuint index, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIB2SARBPROC) (GLuint index, GLshort x, GLshort y);
typedef void ( * PFNGLVERTEXATTRIB2SVARBPROC) (GLuint index, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIB3DARBPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLVERTEXATTRIB3DVARBPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIB3FARBPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLVERTEXATTRIB3FVARBPROC) (GLuint index, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIB3SARBPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void ( * PFNGLVERTEXATTRIB3SVARBPROC) (GLuint index, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIB4NBVARBPROC) (GLuint index, const GLbyte* v);
typedef void ( * PFNGLVERTEXATTRIB4NIVARBPROC) (GLuint index, const GLint* v);
typedef void ( * PFNGLVERTEXATTRIB4NSVARBPROC) (GLuint index, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIB4NUBARBPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void ( * PFNGLVERTEXATTRIB4NUBVARBPROC) (GLuint index, const GLubyte* v);
typedef void ( * PFNGLVERTEXATTRIB4NUIVARBPROC) (GLuint index, const GLuint* v);
typedef void ( * PFNGLVERTEXATTRIB4NUSVARBPROC) (GLuint index, const GLushort* v);
typedef void ( * PFNGLVERTEXATTRIB4BVARBPROC) (GLuint index, const GLbyte* v);
typedef void ( * PFNGLVERTEXATTRIB4DARBPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void ( * PFNGLVERTEXATTRIB4DVARBPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIB4FARBPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void ( * PFNGLVERTEXATTRIB4FVARBPROC) (GLuint index, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIB4IVARBPROC) (GLuint index, const GLint* v);
typedef void ( * PFNGLVERTEXATTRIB4SARBPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void ( * PFNGLVERTEXATTRIB4SVARBPROC) (GLuint index, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIB4UBVARBPROC) (GLuint index, const GLubyte* v);
typedef void ( * PFNGLVERTEXATTRIB4UIVARBPROC) (GLuint index, const GLuint* v);
typedef void ( * PFNGLVERTEXATTRIB4USVARBPROC) (GLuint index, const GLushort* v);
typedef void ( * PFNGLVERTEXATTRIBPOINTERARBPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

typedef void ( * PFNGLBINDATTRIBLOCATIONARBPROC) (GLhandleARB programObj, GLuint index, const GLcharARB* name);
typedef void ( * PFNGLGETACTIVEATTRIBARBPROC) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei* length, GLint *size, GLenum *type, GLcharARB *name);
typedef GLint ( * PFNGLGETATTRIBLOCATIONARBPROC) (GLhandleARB programObj, const GLcharARB* name);

typedef void ( * PFNGLCOLORP3UIPROC) (GLenum type, GLuint color);
typedef void ( * PFNGLCOLORP3UIVPROC) (GLenum type, const GLuint* color);
typedef void ( * PFNGLCOLORP4UIPROC) (GLenum type, GLuint color);
typedef void ( * PFNGLCOLORP4UIVPROC) (GLenum type, const GLuint* color);
typedef void ( * PFNGLMULTITEXCOORDP1UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void ( * PFNGLMULTITEXCOORDP1UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void ( * PFNGLMULTITEXCOORDP2UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void ( * PFNGLMULTITEXCOORDP2UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void ( * PFNGLMULTITEXCOORDP3UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void ( * PFNGLMULTITEXCOORDP3UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void ( * PFNGLMULTITEXCOORDP4UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void ( * PFNGLMULTITEXCOORDP4UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void ( * PFNGLNORMALP3UIPROC) (GLenum type, GLuint coords);
typedef void ( * PFNGLNORMALP3UIVPROC) (GLenum type, const GLuint* coords);
typedef void ( * PFNGLSECONDARYCOLORP3UIPROC) (GLenum type, GLuint color);
typedef void ( * PFNGLSECONDARYCOLORP3UIVPROC) (GLenum type, const GLuint* color);
typedef void ( * PFNGLTEXCOORDP1UIPROC) (GLenum type, GLuint coords);
typedef void ( * PFNGLTEXCOORDP1UIVPROC) (GLenum type, const GLuint* coords);
typedef void ( * PFNGLTEXCOORDP2UIPROC) (GLenum type, GLuint coords);
typedef void ( * PFNGLTEXCOORDP2UIVPROC) (GLenum type, const GLuint* coords);
typedef void ( * PFNGLTEXCOORDP3UIPROC) (GLenum type, GLuint coords);
typedef void ( * PFNGLTEXCOORDP3UIVPROC) (GLenum type, const GLuint* coords);
typedef void ( * PFNGLTEXCOORDP4UIPROC) (GLenum type, GLuint coords);
typedef void ( * PFNGLTEXCOORDP4UIVPROC) (GLenum type, const GLuint* coords);
typedef void ( * PFNGLVERTEXATTRIBP1UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void ( * PFNGLVERTEXATTRIBP1UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void ( * PFNGLVERTEXATTRIBP2UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void ( * PFNGLVERTEXATTRIBP2UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void ( * PFNGLVERTEXATTRIBP3UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void ( * PFNGLVERTEXATTRIBP3UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void ( * PFNGLVERTEXATTRIBP4UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void ( * PFNGLVERTEXATTRIBP4UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void ( * PFNGLVERTEXP2UIPROC) (GLenum type, GLuint value);
typedef void ( * PFNGLVERTEXP2UIVPROC) (GLenum type, const GLuint* value);
typedef void ( * PFNGLVERTEXP3UIPROC) (GLenum type, GLuint value);
typedef void ( * PFNGLVERTEXP3UIVPROC) (GLenum type, const GLuint* value);
typedef void ( * PFNGLVERTEXP4UIPROC) (GLenum type, GLuint value);
typedef void ( * PFNGLVERTEXP4UIVPROC) (GLenum type, const GLuint* value);

typedef void ( * PFNGLDEPTHRANGEARRAYVPROC) (GLuint first, GLsizei count, const GLclampd * v);
typedef void ( * PFNGLDEPTHRANGEINDEXEDPROC) (GLuint index, GLclampd n, GLclampd f);
typedef void ( * PFNGLGETDOUBLEI_VPROC) (GLenum target, GLuint index, GLdouble* data);
typedef void ( * PFNGLGETFLOATI_VPROC) (GLenum target, GLuint index, GLfloat* data);
typedef void ( * PFNGLSCISSORARRAYVPROC) (GLuint first, GLsizei count, const GLint * v);
typedef void ( * PFNGLSCISSORINDEXEDPROC) (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
typedef void ( * PFNGLSCISSORINDEXEDVPROC) (GLuint index, const GLint * v);
typedef void ( * PFNGLVIEWPORTARRAYVPROC) (GLuint first, GLsizei count, const GLfloat * v);
typedef void ( * PFNGLVIEWPORTINDEXEDFPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
typedef void ( * PFNGLVIEWPORTINDEXEDFVPROC) (GLuint index, const GLfloat * v);

typedef void ( * PFNGLWINDOWPOS2DARBPROC) (GLdouble x, GLdouble y);
typedef void ( * PFNGLWINDOWPOS2DVARBPROC) (const GLdouble* p);
typedef void ( * PFNGLWINDOWPOS2FARBPROC) (GLfloat x, GLfloat y);
typedef void ( * PFNGLWINDOWPOS2FVARBPROC) (const GLfloat* p);
typedef void ( * PFNGLWINDOWPOS2IARBPROC) (GLint x, GLint y);
typedef void ( * PFNGLWINDOWPOS2IVARBPROC) (const GLint* p);
typedef void ( * PFNGLWINDOWPOS2SARBPROC) (GLshort x, GLshort y);
typedef void ( * PFNGLWINDOWPOS2SVARBPROC) (const GLshort* p);
typedef void ( * PFNGLWINDOWPOS3DARBPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLWINDOWPOS3DVARBPROC) (const GLdouble* p);
typedef void ( * PFNGLWINDOWPOS3FARBPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLWINDOWPOS3FVARBPROC) (const GLfloat* p);
typedef void ( * PFNGLWINDOWPOS3IARBPROC) (GLint x, GLint y, GLint z);
typedef void ( * PFNGLWINDOWPOS3IVARBPROC) (const GLint* p);
typedef void ( * PFNGLWINDOWPOS3SARBPROC) (GLshort x, GLshort y, GLshort z);
typedef void ( * PFNGLWINDOWPOS3SVARBPROC) (const GLshort* p);

typedef void ( * PFNGLDRAWBUFFERSATIPROC) (GLsizei n, const GLenum* bufs);

typedef void ( * PFNGLDRAWELEMENTARRAYATIPROC) (GLenum mode, GLsizei count);
typedef void ( * PFNGLDRAWRANGEELEMENTARRAYATIPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count);
typedef void ( * PFNGLELEMENTPOINTERATIPROC) (GLenum type, const void* pointer);

typedef void ( * PFNGLGETTEXBUMPPARAMETERFVATIPROC) (GLenum pname, GLfloat *param);
typedef void ( * PFNGLGETTEXBUMPPARAMETERIVATIPROC) (GLenum pname, GLint *param);
typedef void ( * PFNGLTEXBUMPPARAMETERFVATIPROC) (GLenum pname, GLfloat *param);
typedef void ( * PFNGLTEXBUMPPARAMETERIVATIPROC) (GLenum pname, GLint *param);

typedef void ( * PFNGLALPHAFRAGMENTOP1ATIPROC) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
typedef void ( * PFNGLALPHAFRAGMENTOP2ATIPROC) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
typedef void ( * PFNGLALPHAFRAGMENTOP3ATIPROC) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
typedef void ( * PFNGLBEGINFRAGMENTSHADERATIPROC) (void);
typedef void ( * PFNGLBINDFRAGMENTSHADERATIPROC) (GLuint id);
typedef void ( * PFNGLCOLORFRAGMENTOP1ATIPROC) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
typedef void ( * PFNGLCOLORFRAGMENTOP2ATIPROC) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
typedef void ( * PFNGLCOLORFRAGMENTOP3ATIPROC) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
typedef void ( * PFNGLDELETEFRAGMENTSHADERATIPROC) (GLuint id);
typedef void ( * PFNGLENDFRAGMENTSHADERATIPROC) (void);
typedef GLuint ( * PFNGLGENFRAGMENTSHADERSATIPROC) (GLuint range);
typedef void ( * PFNGLPASSTEXCOORDATIPROC) (GLuint dst, GLuint coord, GLenum swizzle);
typedef void ( * PFNGLSAMPLEMAPATIPROC) (GLuint dst, GLuint interp, GLenum swizzle);
typedef void ( * PFNGLSETFRAGMENTSHADERCONSTANTATIPROC) (GLuint dst, const GLfloat* value);

typedef void* ( * PFNGLMAPOBJECTBUFFERATIPROC) (GLuint buffer);
typedef void ( * PFNGLUNMAPOBJECTBUFFERATIPROC) (GLuint buffer);

typedef void ( * PFNGLPNTRIANGLESFATIPROC) (GLenum pname, GLfloat param);
typedef void ( * PFNGLPNTRIANGLESIATIPROC) (GLenum pname, GLint param);

typedef void ( * PFNGLSTENCILFUNCSEPARATEATIPROC) (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
typedef void ( * PFNGLSTENCILOPSEPARATEATIPROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);

typedef void ( * PFNGLARRAYOBJECTATIPROC) (GLenum array, GLint size, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);
typedef void ( * PFNGLFREEOBJECTBUFFERATIPROC) (GLuint buffer);
typedef void ( * PFNGLGETARRAYOBJECTFVATIPROC) (GLenum array, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETARRAYOBJECTIVATIPROC) (GLenum array, GLenum pname, GLint* params);
typedef void ( * PFNGLGETOBJECTBUFFERFVATIPROC) (GLuint buffer, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETOBJECTBUFFERIVATIPROC) (GLuint buffer, GLenum pname, GLint* params);
typedef void ( * PFNGLGETVARIANTARRAYOBJECTFVATIPROC) (GLuint id, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETVARIANTARRAYOBJECTIVATIPROC) (GLuint id, GLenum pname, GLint* params);
typedef GLboolean ( * PFNGLISOBJECTBUFFERATIPROC) (GLuint buffer);
typedef GLuint ( * PFNGLNEWOBJECTBUFFERATIPROC) (GLsizei size, const void* pointer, GLenum usage);
typedef void ( * PFNGLUPDATEOBJECTBUFFERATIPROC) (GLuint buffer, GLuint offset, GLsizei size, const void* pointer, GLenum preserve);
typedef void ( * PFNGLVARIANTARRAYOBJECTATIPROC) (GLuint id, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);

typedef void ( * PFNGLGETVERTEXATTRIBARRAYOBJECTFVATIPROC) (GLuint index, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETVERTEXATTRIBARRAYOBJECTIVATIPROC) (GLuint index, GLenum pname, GLint* params);
typedef void ( * PFNGLVERTEXATTRIBARRAYOBJECTATIPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset);

typedef void ( * PFNGLCLIENTACTIVEVERTEXSTREAMATIPROC) (GLenum stream);
typedef void ( * PFNGLNORMALSTREAM3BATIPROC) (GLenum stream, GLbyte x, GLbyte y, GLbyte z);
typedef void ( * PFNGLNORMALSTREAM3BVATIPROC) (GLenum stream, const GLbyte *v);
typedef void ( * PFNGLNORMALSTREAM3DATIPROC) (GLenum stream, GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLNORMALSTREAM3DVATIPROC) (GLenum stream, const GLdouble *v);
typedef void ( * PFNGLNORMALSTREAM3FATIPROC) (GLenum stream, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLNORMALSTREAM3FVATIPROC) (GLenum stream, const GLfloat *v);
typedef void ( * PFNGLNORMALSTREAM3IATIPROC) (GLenum stream, GLint x, GLint y, GLint z);
typedef void ( * PFNGLNORMALSTREAM3IVATIPROC) (GLenum stream, const GLint *v);
typedef void ( * PFNGLNORMALSTREAM3SATIPROC) (GLenum stream, GLshort x, GLshort y, GLshort z);
typedef void ( * PFNGLNORMALSTREAM3SVATIPROC) (GLenum stream, const GLshort *v);
typedef void ( * PFNGLVERTEXBLENDENVFATIPROC) (GLenum pname, GLfloat param);
typedef void ( * PFNGLVERTEXBLENDENVIATIPROC) (GLenum pname, GLint param);
typedef void ( * PFNGLVERTEXSTREAM2DATIPROC) (GLenum stream, GLdouble x, GLdouble y);
typedef void ( * PFNGLVERTEXSTREAM2DVATIPROC) (GLenum stream, const GLdouble *v);
typedef void ( * PFNGLVERTEXSTREAM2FATIPROC) (GLenum stream, GLfloat x, GLfloat y);
typedef void ( * PFNGLVERTEXSTREAM2FVATIPROC) (GLenum stream, const GLfloat *v);
typedef void ( * PFNGLVERTEXSTREAM2IATIPROC) (GLenum stream, GLint x, GLint y);
typedef void ( * PFNGLVERTEXSTREAM2IVATIPROC) (GLenum stream, const GLint *v);
typedef void ( * PFNGLVERTEXSTREAM2SATIPROC) (GLenum stream, GLshort x, GLshort y);
typedef void ( * PFNGLVERTEXSTREAM2SVATIPROC) (GLenum stream, const GLshort *v);
typedef void ( * PFNGLVERTEXSTREAM3DATIPROC) (GLenum stream, GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLVERTEXSTREAM3DVATIPROC) (GLenum stream, const GLdouble *v);
typedef void ( * PFNGLVERTEXSTREAM3FATIPROC) (GLenum stream, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLVERTEXSTREAM3FVATIPROC) (GLenum stream, const GLfloat *v);
typedef void ( * PFNGLVERTEXSTREAM3IATIPROC) (GLenum stream, GLint x, GLint y, GLint z);
typedef void ( * PFNGLVERTEXSTREAM3IVATIPROC) (GLenum stream, const GLint *v);
typedef void ( * PFNGLVERTEXSTREAM3SATIPROC) (GLenum stream, GLshort x, GLshort y, GLshort z);
typedef void ( * PFNGLVERTEXSTREAM3SVATIPROC) (GLenum stream, const GLshort *v);
typedef void ( * PFNGLVERTEXSTREAM4DATIPROC) (GLenum stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void ( * PFNGLVERTEXSTREAM4DVATIPROC) (GLenum stream, const GLdouble *v);
typedef void ( * PFNGLVERTEXSTREAM4FATIPROC) (GLenum stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void ( * PFNGLVERTEXSTREAM4FVATIPROC) (GLenum stream, const GLfloat *v);
typedef void ( * PFNGLVERTEXSTREAM4IATIPROC) (GLenum stream, GLint x, GLint y, GLint z, GLint w);
typedef void ( * PFNGLVERTEXSTREAM4IVATIPROC) (GLenum stream, const GLint *v);
typedef void ( * PFNGLVERTEXSTREAM4SATIPROC) (GLenum stream, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void ( * PFNGLVERTEXSTREAM4SVATIPROC) (GLenum stream, const GLshort *v);

typedef GLint ( * PFNGLGETUNIFORMBUFFERSIZEEXTPROC) (GLuint program, GLint location);
typedef GLintptr ( * PFNGLGETUNIFORMOFFSETEXTPROC) (GLuint program, GLint location);
typedef void ( * PFNGLUNIFORMBUFFEREXTPROC) (GLuint program, GLint location, GLuint buffer);

typedef void ( * PFNGLBLENDCOLOREXTPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

typedef void ( * PFNGLBLENDEQUATIONSEPARATEEXTPROC) (GLenum modeRGB, GLenum modeAlpha);

typedef void ( * PFNGLBLENDFUNCSEPARATEEXTPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

typedef void ( * PFNGLBLENDEQUATIONEXTPROC) (GLenum mode);

typedef void ( * PFNGLCOLORSUBTABLEEXTPROC) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void* data);
typedef void ( * PFNGLCOPYCOLORSUBTABLEEXTPROC) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);

typedef void ( * PFNGLLOCKARRAYSEXTPROC) (GLint first, GLsizei count);
typedef void ( * PFNGLUNLOCKARRAYSEXTPROC) (void);

typedef void ( * PFNGLCONVOLUTIONFILTER1DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void* image);
typedef void ( * PFNGLCONVOLUTIONFILTER2DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* image);
typedef void ( * PFNGLCONVOLUTIONPARAMETERFEXTPROC) (GLenum target, GLenum pname, GLfloat param);
typedef void ( * PFNGLCONVOLUTIONPARAMETERFVEXTPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLCONVOLUTIONPARAMETERIEXTPROC) (GLenum target, GLenum pname, GLint param);
typedef void ( * PFNGLCONVOLUTIONPARAMETERIVEXTPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void ( * PFNGLCOPYCONVOLUTIONFILTER1DEXTPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void ( * PFNGLCOPYCONVOLUTIONFILTER2DEXTPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void ( * PFNGLGETCONVOLUTIONFILTEREXTPROC) (GLenum target, GLenum format, GLenum type, void* image);
typedef void ( * PFNGLGETCONVOLUTIONPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETCONVOLUTIONPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef void ( * PFNGLGETSEPARABLEFILTEREXTPROC) (GLenum target, GLenum format, GLenum type, void* row, void* column, void* span);
typedef void ( * PFNGLSEPARABLEFILTER2DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* row, const void* column);

typedef void ( * PFNGLBINORMALPOINTEREXTPROC) (GLenum type, GLsizei stride, void* pointer);
typedef void ( * PFNGLTANGENTPOINTEREXTPROC) (GLenum type, GLsizei stride, void* pointer);

typedef void ( * PFNGLCOPYTEXIMAGE1DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void ( * PFNGLCOPYTEXIMAGE2DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void ( * PFNGLCOPYTEXSUBIMAGE1DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void ( * PFNGLCOPYTEXSUBIMAGE2DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void ( * PFNGLCOPYTEXSUBIMAGE3DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

typedef void ( * PFNGLCULLPARAMETERDVEXTPROC) (GLenum pname, GLdouble* params);
typedef void ( * PFNGLCULLPARAMETERFVEXTPROC) (GLenum pname, GLfloat* params);

typedef void ( * PFNGLDEPTHBOUNDSEXTPROC) (GLclampd zmin, GLclampd zmax);

typedef void ( * PFNGLBINDMULTITEXTUREEXTPROC) (GLenum texunit, GLenum target, GLuint texture);
typedef GLenum ( * PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC) (GLuint framebuffer, GLenum target);
typedef void ( * PFNGLCLIENTATTRIBDEFAULTEXTPROC) (GLbitfield mask);
typedef void ( * PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
typedef void ( * PFNGLCOPYMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void ( * PFNGLCOPYMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void ( * PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void ( * PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void ( * PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void ( * PFNGLCOPYTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void ( * PFNGLCOPYTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void ( * PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void ( * PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void ( * PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void ( * PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC) (GLenum array, GLuint index);
typedef void ( * PFNGLDISABLECLIENTSTATEIEXTPROC) (GLenum array, GLuint index);
typedef void ( * PFNGLDISABLEVERTEXARRAYATTRIBEXTPROC) (GLuint vaobj, GLuint index);
typedef void ( * PFNGLDISABLEVERTEXARRAYEXTPROC) (GLuint vaobj, GLenum array);
typedef void ( * PFNGLENABLECLIENTSTATEINDEXEDEXTPROC) (GLenum array, GLuint index);
typedef void ( * PFNGLENABLECLIENTSTATEIEXTPROC) (GLenum array, GLuint index);
typedef void ( * PFNGLENABLEVERTEXARRAYATTRIBEXTPROC) (GLuint vaobj, GLuint index);
typedef void ( * PFNGLENABLEVERTEXARRAYEXTPROC) (GLuint vaobj, GLenum array);
typedef void ( * PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void ( * PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC) (GLuint framebuffer, GLenum mode);
typedef void ( * PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC) (GLuint framebuffer, GLsizei n, const GLenum* bufs);
typedef void ( * PFNGLFRAMEBUFFERREADBUFFEREXTPROC) (GLuint framebuffer, GLenum mode);
typedef void ( * PFNGLGENERATEMULTITEXMIPMAPEXTPROC) (GLenum texunit, GLenum target);
typedef void ( * PFNGLGENERATETEXTUREMIPMAPEXTPROC) (GLuint texture, GLenum target);
typedef void ( * PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC) (GLenum texunit, GLenum target, GLint level, void* img);
typedef void ( * PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC) (GLuint texture, GLenum target, GLint level, void* img);
typedef void ( * PFNGLGETDOUBLEINDEXEDVEXTPROC) (GLenum target, GLuint index, GLdouble* params);
typedef void ( * PFNGLGETDOUBLEI_VEXTPROC) (GLenum pname, GLuint index, GLdouble* params);
typedef void ( * PFNGLGETFLOATINDEXEDVEXTPROC) (GLenum target, GLuint index, GLfloat* params);
typedef void ( * PFNGLGETFLOATI_VEXTPROC) (GLenum pname, GLuint index, GLfloat* params);
typedef void ( * PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum pname, GLint* param);
typedef void ( * PFNGLGETMULTITEXENVFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETMULTITEXENVIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void ( * PFNGLGETMULTITEXGENDVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble* params);
typedef void ( * PFNGLGETMULTITEXGENFVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETMULTITEXGENIVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint* params);
typedef void ( * PFNGLGETMULTITEXIMAGEEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
typedef void ( * PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLint* params);
typedef void ( * PFNGLGETMULTITEXPARAMETERIIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void ( * PFNGLGETMULTITEXPARAMETERIUIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLuint* params);
typedef void ( * PFNGLGETMULTITEXPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETMULTITEXPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void ( * PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC) (GLuint buffer, GLenum pname, GLint* params);
typedef void ( * PFNGLGETNAMEDBUFFERPOINTERVEXTPROC) (GLuint buffer, GLenum pname, void** params);
typedef void ( * PFNGLGETNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, void* data);
typedef void ( * PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum pname, GLint* params);
typedef void ( * PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLint* params);
typedef void ( * PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint* params);
typedef void ( * PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble* params);
typedef void ( * PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat* params);
typedef void ( * PFNGLGETNAMEDPROGRAMSTRINGEXTPROC) (GLuint program, GLenum target, GLenum pname, void* string);
typedef void ( * PFNGLGETNAMEDPROGRAMIVEXTPROC) (GLuint program, GLenum target, GLenum pname, GLint* params);
typedef void ( * PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC) (GLuint renderbuffer, GLenum pname, GLint* params);
typedef void ( * PFNGLGETPOINTERINDEXEDVEXTPROC) (GLenum target, GLuint index, GLvoid** params);
typedef void ( * PFNGLGETPOINTERI_VEXTPROC) (GLenum pname, GLuint index, GLvoid** params);
typedef void ( * PFNGLGETTEXTUREIMAGEEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
typedef void ( * PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum pname, GLint* params);
typedef void ( * PFNGLGETTEXTUREPARAMETERIIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint* params);
typedef void ( * PFNGLGETTEXTUREPARAMETERIUIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLuint* params);
typedef void ( * PFNGLGETTEXTUREPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETTEXTUREPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint* params);
typedef void ( * PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC) (GLuint vaobj, GLuint index, GLenum pname, GLint* param);
typedef void ( * PFNGLGETVERTEXARRAYINTEGERVEXTPROC) (GLuint vaobj, GLenum pname, GLint* param);
typedef void ( * PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC) (GLuint vaobj, GLuint index, GLenum pname, GLvoid** param);
typedef void ( * PFNGLGETVERTEXARRAYPOINTERVEXTPROC) (GLuint vaobj, GLenum pname, GLvoid** param);
typedef GLvoid * ( * PFNGLMAPNAMEDBUFFEREXTPROC) (GLuint buffer, GLenum access);
typedef GLvoid * ( * PFNGLMAPNAMEDBUFFERRANGEEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void ( * PFNGLMATRIXFRUSTUMEXTPROC) (GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
typedef void ( * PFNGLMATRIXLOADIDENTITYEXTPROC) (GLenum matrixMode);
typedef void ( * PFNGLMATRIXLOADTRANSPOSEDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void ( * PFNGLMATRIXLOADTRANSPOSEFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void ( * PFNGLMATRIXLOADDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void ( * PFNGLMATRIXLOADFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void ( * PFNGLMATRIXMULTTRANSPOSEDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void ( * PFNGLMATRIXMULTTRANSPOSEFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void ( * PFNGLMATRIXMULTDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void ( * PFNGLMATRIXMULTFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void ( * PFNGLMATRIXORTHOEXTPROC) (GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
typedef void ( * PFNGLMATRIXPOPEXTPROC) (GLenum matrixMode);
typedef void ( * PFNGLMATRIXPUSHEXTPROC) (GLenum matrixMode);
typedef void ( * PFNGLMATRIXROTATEDEXTPROC) (GLenum matrixMode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLMATRIXROTATEFEXTPROC) (GLenum matrixMode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLMATRIXSCALEDEXTPROC) (GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLMATRIXSCALEFEXTPROC) (GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLMATRIXTRANSLATEDEXTPROC) (GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLMATRIXTRANSLATEFEXTPROC) (GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLMULTITEXBUFFEREXTPROC) (GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer);
typedef void ( * PFNGLMULTITEXCOORDPOINTEREXTPROC) (GLenum texunit, GLint size, GLenum type, GLsizei stride, const void* pointer);
typedef void ( * PFNGLMULTITEXENVFEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
typedef void ( * PFNGLMULTITEXENVFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLMULTITEXENVIEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint param);
typedef void ( * PFNGLMULTITEXENVIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* params);
typedef void ( * PFNGLMULTITEXGENDEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble param);
typedef void ( * PFNGLMULTITEXGENDVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLdouble* params);
typedef void ( * PFNGLMULTITEXGENFEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat param);
typedef void ( * PFNGLMULTITEXGENFVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLMULTITEXGENIEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint param);
typedef void ( * PFNGLMULTITEXGENIVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLint* params);
typedef void ( * PFNGLMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void ( * PFNGLMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void ( * PFNGLMULTITEXIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void ( * PFNGLMULTITEXPARAMETERIIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* params);
typedef void ( * PFNGLMULTITEXPARAMETERIUIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLuint* params);
typedef void ( * PFNGLMULTITEXPARAMETERFEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
typedef void ( * PFNGLMULTITEXPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLfloat* param);
typedef void ( * PFNGLMULTITEXPARAMETERIEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint param);
typedef void ( * PFNGLMULTITEXPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* param);
typedef void ( * PFNGLMULTITEXRENDERBUFFEREXTPROC) (GLenum texunit, GLenum target, GLuint renderbuffer);
typedef void ( * PFNGLMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
typedef void ( * PFNGLMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void ( * PFNGLMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
typedef void ( * PFNGLNAMEDBUFFERDATAEXTPROC) (GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
typedef void ( * PFNGLNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data);
typedef void ( * PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC) (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void ( * PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC) (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void ( * PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void ( * PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void ( * PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void ( * PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
typedef void ( * PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void ( * PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void ( * PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void ( * PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLdouble* params);
typedef void ( * PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void ( * PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLfloat* params);
typedef void ( * PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC) (GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void ( * PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLint* params);
typedef void ( * PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void ( * PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLuint* params);
typedef void ( * PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat* params);
typedef void ( * PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLint* params);
typedef void ( * PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint* params);
typedef void ( * PFNGLNAMEDPROGRAMSTRINGEXTPROC) (GLuint program, GLenum target, GLenum format, GLsizei len, const void* string);
typedef void ( * PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC) (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
typedef void ( * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC) (GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void ( * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void ( * PFNGLPROGRAMUNIFORM1FEXTPROC) (GLuint program, GLint location, GLfloat v0);
typedef void ( * PFNGLPROGRAMUNIFORM1FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORM1IEXTPROC) (GLuint program, GLint location, GLint v0);
typedef void ( * PFNGLPROGRAMUNIFORM1IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLPROGRAMUNIFORM1UIEXTPROC) (GLuint program, GLint location, GLuint v0);
typedef void ( * PFNGLPROGRAMUNIFORM1UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void ( * PFNGLPROGRAMUNIFORM2FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
typedef void ( * PFNGLPROGRAMUNIFORM2FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORM2IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1);
typedef void ( * PFNGLPROGRAMUNIFORM2IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLPROGRAMUNIFORM2UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1);
typedef void ( * PFNGLPROGRAMUNIFORM2UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void ( * PFNGLPROGRAMUNIFORM3FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void ( * PFNGLPROGRAMUNIFORM3FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORM3IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
typedef void ( * PFNGLPROGRAMUNIFORM3IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLPROGRAMUNIFORM3UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void ( * PFNGLPROGRAMUNIFORM3UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void ( * PFNGLPROGRAMUNIFORM4FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void ( * PFNGLPROGRAMUNIFORM4FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORM4IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void ( * PFNGLPROGRAMUNIFORM4IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void ( * PFNGLPROGRAMUNIFORM4UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void ( * PFNGLPROGRAMUNIFORM4UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void ( * PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC) (GLbitfield mask);
typedef void ( * PFNGLTEXTUREBUFFEREXTPROC) (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
typedef void ( * PFNGLTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void ( * PFNGLTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void ( * PFNGLTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void ( * PFNGLTEXTUREPARAMETERIIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLint* params);
typedef void ( * PFNGLTEXTUREPARAMETERIUIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLuint* params);
typedef void ( * PFNGLTEXTUREPARAMETERFEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat param);
typedef void ( * PFNGLTEXTUREPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLfloat* param);
typedef void ( * PFNGLTEXTUREPARAMETERIEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint param);
typedef void ( * PFNGLTEXTUREPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLint* param);
typedef void ( * PFNGLTEXTURERENDERBUFFEREXTPROC) (GLuint texture, GLenum target, GLuint renderbuffer);
typedef void ( * PFNGLTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
typedef void ( * PFNGLTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void ( * PFNGLTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
typedef GLboolean ( * PFNGLUNMAPNAMEDBUFFEREXTPROC) (GLuint buffer);
typedef void ( * PFNGLVERTEXARRAYCOLOROFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void ( * PFNGLVERTEXARRAYEDGEFLAGOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset);
typedef void ( * PFNGLVERTEXARRAYFOGCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void ( * PFNGLVERTEXARRAYINDEXOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void ( * PFNGLVERTEXARRAYMULTITEXCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void ( * PFNGLVERTEXARRAYNORMALOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void ( * PFNGLVERTEXARRAYSECONDARYCOLOROFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void ( * PFNGLVERTEXARRAYTEXCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void ( * PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void ( * PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
typedef void ( * PFNGLVERTEXARRAYVERTEXOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);

typedef void ( * PFNGLCOLORMASKINDEXEDEXTPROC) (GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
typedef void ( * PFNGLDISABLEINDEXEDEXTPROC) (GLenum target, GLuint index);
typedef void ( * PFNGLENABLEINDEXEDEXTPROC) (GLenum target, GLuint index);
typedef void ( * PFNGLGETBOOLEANINDEXEDVEXTPROC) (GLenum value, GLuint index, GLboolean* data);
typedef void ( * PFNGLGETINTEGERINDEXEDVEXTPROC) (GLenum value, GLuint index, GLint* data);
typedef GLboolean ( * PFNGLISENABLEDINDEXEDEXTPROC) (GLenum target, GLuint index);

typedef void ( * PFNGLDRAWARRAYSINSTANCEDEXTPROC) (GLenum mode, GLint start, GLsizei count, GLsizei primcount);
typedef void ( * PFNGLDRAWELEMENTSINSTANCEDEXTPROC) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);

typedef void ( * PFNGLDRAWRANGEELEMENTSEXTPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);

typedef void ( * PFNGLFOGCOORDPOINTEREXTPROC) (GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void ( * PFNGLFOGCOORDDEXTPROC) (GLdouble coord);
typedef void ( * PFNGLFOGCOORDDVEXTPROC) (const GLdouble *coord);
typedef void ( * PFNGLFOGCOORDFEXTPROC) (GLfloat coord);
typedef void ( * PFNGLFOGCOORDFVEXTPROC) (const GLfloat *coord);

typedef void ( * PFNGLFRAGMENTCOLORMATERIALEXTPROC) (GLenum face, GLenum mode);
typedef void ( * PFNGLFRAGMENTLIGHTMODELFEXTPROC) (GLenum pname, GLfloat param);
typedef void ( * PFNGLFRAGMENTLIGHTMODELFVEXTPROC) (GLenum pname, GLfloat* params);
typedef void ( * PFNGLFRAGMENTLIGHTMODELIEXTPROC) (GLenum pname, GLint param);
typedef void ( * PFNGLFRAGMENTLIGHTMODELIVEXTPROC) (GLenum pname, GLint* params);
typedef void ( * PFNGLFRAGMENTLIGHTFEXTPROC) (GLenum light, GLenum pname, GLfloat param);
typedef void ( * PFNGLFRAGMENTLIGHTFVEXTPROC) (GLenum light, GLenum pname, GLfloat* params);
typedef void ( * PFNGLFRAGMENTLIGHTIEXTPROC) (GLenum light, GLenum pname, GLint param);
typedef void ( * PFNGLFRAGMENTLIGHTIVEXTPROC) (GLenum light, GLenum pname, GLint* params);
typedef void ( * PFNGLFRAGMENTMATERIALFEXTPROC) (GLenum face, GLenum pname, const GLfloat param);
typedef void ( * PFNGLFRAGMENTMATERIALFVEXTPROC) (GLenum face, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLFRAGMENTMATERIALIEXTPROC) (GLenum face, GLenum pname, const GLint param);
typedef void ( * PFNGLFRAGMENTMATERIALIVEXTPROC) (GLenum face, GLenum pname, const GLint* params);
typedef void ( * PFNGLGETFRAGMENTLIGHTFVEXTPROC) (GLenum light, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETFRAGMENTLIGHTIVEXTPROC) (GLenum light, GLenum pname, GLint* params);
typedef void ( * PFNGLGETFRAGMENTMATERIALFVEXTPROC) (GLenum face, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLGETFRAGMENTMATERIALIVEXTPROC) (GLenum face, GLenum pname, const GLint* params);
typedef void ( * PFNGLLIGHTENVIEXTPROC) (GLenum pname, GLint param);

typedef void ( * PFNGLBLITFRAMEBUFFEREXTPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

typedef void ( * PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

typedef void ( * PFNGLBINDFRAMEBUFFEREXTPROC) (GLenum target, GLuint framebuffer);
typedef void ( * PFNGLBINDRENDERBUFFEREXTPROC) (GLenum target, GLuint renderbuffer);
typedef GLenum ( * PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) (GLenum target);
typedef void ( * PFNGLDELETEFRAMEBUFFERSEXTPROC) (GLsizei n, const GLuint* framebuffers);
typedef void ( * PFNGLDELETERENDERBUFFERSEXTPROC) (GLsizei n, const GLuint* renderbuffers);
typedef void ( * PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void ( * PFNGLFRAMEBUFFERTEXTURE1DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void ( * PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void ( * PFNGLFRAMEBUFFERTEXTURE3DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void ( * PFNGLGENFRAMEBUFFERSEXTPROC) (GLsizei n, GLuint* framebuffers);
typedef void ( * PFNGLGENRENDERBUFFERSEXTPROC) (GLsizei n, GLuint* renderbuffers);
typedef void ( * PFNGLGENERATEMIPMAPEXTPROC) (GLenum target);
typedef void ( * PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
typedef void ( * PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean ( * PFNGLISFRAMEBUFFEREXTPROC) (GLuint framebuffer);
typedef GLboolean ( * PFNGLISRENDERBUFFEREXTPROC) (GLuint renderbuffer);
typedef void ( * PFNGLRENDERBUFFERSTORAGEEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);

typedef void ( * PFNGLFRAMEBUFFERTEXTUREEXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void ( * PFNGLFRAMEBUFFERTEXTUREFACEEXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void ( * PFNGLPROGRAMPARAMETERIEXTPROC) (GLuint program, GLenum pname, GLint value);

typedef void ( * PFNGLPROGRAMENVPARAMETERS4FVEXTPROC) (GLenum target, GLuint index, GLsizei count, const GLfloat* params);
typedef void ( * PFNGLPROGRAMLOCALPARAMETERS4FVEXTPROC) (GLenum target, GLuint index, GLsizei count, const GLfloat* params);

typedef void ( * PFNGLBINDFRAGDATALOCATIONEXTPROC) (GLuint program, GLuint color, const GLchar *name);
typedef GLint ( * PFNGLGETFRAGDATALOCATIONEXTPROC) (GLuint program, const GLchar *name);
typedef void ( * PFNGLGETUNIFORMUIVEXTPROC) (GLuint program, GLint location, GLuint *params);
typedef void ( * PFNGLGETVERTEXATTRIBIIVEXTPROC) (GLuint index, GLenum pname, GLint *params);
typedef void ( * PFNGLGETVERTEXATTRIBIUIVEXTPROC) (GLuint index, GLenum pname, GLuint *params);
typedef void ( * PFNGLUNIFORM1UIEXTPROC) (GLint location, GLuint v0);
typedef void ( * PFNGLUNIFORM1UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void ( * PFNGLUNIFORM2UIEXTPROC) (GLint location, GLuint v0, GLuint v1);
typedef void ( * PFNGLUNIFORM2UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void ( * PFNGLUNIFORM3UIEXTPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void ( * PFNGLUNIFORM3UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void ( * PFNGLUNIFORM4UIEXTPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void ( * PFNGLUNIFORM4UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void ( * PFNGLVERTEXATTRIBI1IEXTPROC) (GLuint index, GLint x);
typedef void ( * PFNGLVERTEXATTRIBI1IVEXTPROC) (GLuint index, const GLint *v);
typedef void ( * PFNGLVERTEXATTRIBI1UIEXTPROC) (GLuint index, GLuint x);
typedef void ( * PFNGLVERTEXATTRIBI1UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void ( * PFNGLVERTEXATTRIBI2IEXTPROC) (GLuint index, GLint x, GLint y);
typedef void ( * PFNGLVERTEXATTRIBI2IVEXTPROC) (GLuint index, const GLint *v);
typedef void ( * PFNGLVERTEXATTRIBI2UIEXTPROC) (GLuint index, GLuint x, GLuint y);
typedef void ( * PFNGLVERTEXATTRIBI2UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void ( * PFNGLVERTEXATTRIBI3IEXTPROC) (GLuint index, GLint x, GLint y, GLint z);
typedef void ( * PFNGLVERTEXATTRIBI3IVEXTPROC) (GLuint index, const GLint *v);
typedef void ( * PFNGLVERTEXATTRIBI3UIEXTPROC) (GLuint index, GLuint x, GLuint y, GLuint z);
typedef void ( * PFNGLVERTEXATTRIBI3UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void ( * PFNGLVERTEXATTRIBI4BVEXTPROC) (GLuint index, const GLbyte *v);
typedef void ( * PFNGLVERTEXATTRIBI4IEXTPROC) (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void ( * PFNGLVERTEXATTRIBI4IVEXTPROC) (GLuint index, const GLint *v);
typedef void ( * PFNGLVERTEXATTRIBI4SVEXTPROC) (GLuint index, const GLshort *v);
typedef void ( * PFNGLVERTEXATTRIBI4UBVEXTPROC) (GLuint index, const GLubyte *v);
typedef void ( * PFNGLVERTEXATTRIBI4UIEXTPROC) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void ( * PFNGLVERTEXATTRIBI4UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void ( * PFNGLVERTEXATTRIBI4USVEXTPROC) (GLuint index, const GLushort *v);
typedef void ( * PFNGLVERTEXATTRIBIPOINTEREXTPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

typedef void ( * PFNGLGETHISTOGRAMEXTPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, void* values);
typedef void ( * PFNGLGETHISTOGRAMPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETHISTOGRAMPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef void ( * PFNGLGETMINMAXEXTPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, void* values);
typedef void ( * PFNGLGETMINMAXPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETMINMAXPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef void ( * PFNGLHISTOGRAMEXTPROC) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
typedef void ( * PFNGLMINMAXEXTPROC) (GLenum target, GLenum internalformat, GLboolean sink);
typedef void ( * PFNGLRESETHISTOGRAMEXTPROC) (GLenum target);
typedef void ( * PFNGLRESETMINMAXEXTPROC) (GLenum target);

typedef void ( * PFNGLINDEXFUNCEXTPROC) (GLenum func, GLfloat ref);

typedef void ( * PFNGLINDEXMATERIALEXTPROC) (GLenum face, GLenum mode);

typedef void ( * PFNGLAPPLYTEXTUREEXTPROC) (GLenum mode);
typedef void ( * PFNGLTEXTURELIGHTEXTPROC) (GLenum pname);
typedef void ( * PFNGLTEXTUREMATERIALEXTPROC) (GLenum face, GLenum mode);

typedef void ( * PFNGLMULTIDRAWARRAYSEXTPROC) (GLenum mode, const GLint* first, const GLsizei *count, GLsizei primcount);
typedef void ( * PFNGLMULTIDRAWELEMENTSEXTPROC) (GLenum mode, GLsizei* count, GLenum type, const GLvoid **indices, GLsizei primcount);

typedef void ( * PFNGLSAMPLEMASKEXTPROC) (GLclampf value, GLboolean invert);
typedef void ( * PFNGLSAMPLEPATTERNEXTPROC) (GLenum pattern);

typedef void ( * PFNGLCOLORTABLEEXTPROC) (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const void* data);
typedef void ( * PFNGLGETCOLORTABLEEXTPROC) (GLenum target, GLenum format, GLenum type, void* data);
typedef void ( * PFNGLGETCOLORTABLEPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETCOLORTABLEPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);

typedef void ( * PFNGLGETPIXELTRANSFORMPARAMETERFVEXTPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLGETPIXELTRANSFORMPARAMETERIVEXTPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void ( * PFNGLPIXELTRANSFORMPARAMETERFEXTPROC) (GLenum target, GLenum pname, const GLfloat param);
typedef void ( * PFNGLPIXELTRANSFORMPARAMETERFVEXTPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLPIXELTRANSFORMPARAMETERIEXTPROC) (GLenum target, GLenum pname, const GLint param);
typedef void ( * PFNGLPIXELTRANSFORMPARAMETERIVEXTPROC) (GLenum target, GLenum pname, const GLint* params);

typedef void ( * PFNGLPOINTPARAMETERFEXTPROC) (GLenum pname, GLfloat param);
typedef void ( * PFNGLPOINTPARAMETERFVEXTPROC) (GLenum pname, const GLfloat* params);

typedef void ( * PFNGLPOLYGONOFFSETEXTPROC) (GLfloat factor, GLfloat bias);

typedef void ( * PFNGLPROVOKINGVERTEXEXTPROC) (GLenum mode);

typedef void ( * PFNGLBEGINSCENEEXTPROC) (void);
typedef void ( * PFNGLENDSCENEEXTPROC) (void);

typedef void ( * PFNGLSECONDARYCOLOR3BEXTPROC) (GLbyte red, GLbyte green, GLbyte blue);
typedef void ( * PFNGLSECONDARYCOLOR3BVEXTPROC) (const GLbyte *v);
typedef void ( * PFNGLSECONDARYCOLOR3DEXTPROC) (GLdouble red, GLdouble green, GLdouble blue);
typedef void ( * PFNGLSECONDARYCOLOR3DVEXTPROC) (const GLdouble *v);
typedef void ( * PFNGLSECONDARYCOLOR3FEXTPROC) (GLfloat red, GLfloat green, GLfloat blue);
typedef void ( * PFNGLSECONDARYCOLOR3FVEXTPROC) (const GLfloat *v);
typedef void ( * PFNGLSECONDARYCOLOR3IEXTPROC) (GLint red, GLint green, GLint blue);
typedef void ( * PFNGLSECONDARYCOLOR3IVEXTPROC) (const GLint *v);
typedef void ( * PFNGLSECONDARYCOLOR3SEXTPROC) (GLshort red, GLshort green, GLshort blue);
typedef void ( * PFNGLSECONDARYCOLOR3SVEXTPROC) (const GLshort *v);
typedef void ( * PFNGLSECONDARYCOLOR3UBEXTPROC) (GLubyte red, GLubyte green, GLubyte blue);
typedef void ( * PFNGLSECONDARYCOLOR3UBVEXTPROC) (const GLubyte *v);
typedef void ( * PFNGLSECONDARYCOLOR3UIEXTPROC) (GLuint red, GLuint green, GLuint blue);
typedef void ( * PFNGLSECONDARYCOLOR3UIVEXTPROC) (const GLuint *v);
typedef void ( * PFNGLSECONDARYCOLOR3USEXTPROC) (GLushort red, GLushort green, GLushort blue);
typedef void ( * PFNGLSECONDARYCOLOR3USVEXTPROC) (const GLushort *v);
typedef void ( * PFNGLSECONDARYCOLORPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

typedef void ( * PFNGLACTIVEPROGRAMEXTPROC) (GLuint program);
typedef GLuint ( * PFNGLCREATESHADERPROGRAMEXTPROC) (GLenum type, const char* string);
typedef void ( * PFNGLUSESHADERPROGRAMEXTPROC) (GLenum type, GLuint program);

typedef void ( * PFNGLBINDIMAGETEXTUREEXTPROC) (GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format);
typedef void ( * PFNGLMEMORYBARRIEREXTPROC) (GLbitfield barriers);

typedef void ( * PFNGLACTIVESTENCILFACEEXTPROC) (GLenum face);

typedef void ( * PFNGLTEXSUBIMAGE1DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
typedef void ( * PFNGLTEXSUBIMAGE2DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void ( * PFNGLTEXSUBIMAGE3DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);

typedef void ( * PFNGLTEXIMAGE3DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);

typedef void ( * PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

typedef void ( * PFNGLTEXBUFFEREXTPROC) (GLenum target, GLenum internalformat, GLuint buffer);

typedef void ( * PFNGLCLEARCOLORIIEXTPROC) (GLint red, GLint green, GLint blue, GLint alpha);
typedef void ( * PFNGLCLEARCOLORIUIEXTPROC) (GLuint red, GLuint green, GLuint blue, GLuint alpha);
typedef void ( * PFNGLGETTEXPARAMETERIIVEXTPROC) (GLenum target, GLenum pname, GLint *params);
typedef void ( * PFNGLGETTEXPARAMETERIUIVEXTPROC) (GLenum target, GLenum pname, GLuint *params);
typedef void ( * PFNGLTEXPARAMETERIIVEXTPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void ( * PFNGLTEXPARAMETERIUIVEXTPROC) (GLenum target, GLenum pname, const GLuint *params);

typedef GLboolean ( * PFNGLARETEXTURESRESIDENTEXTPROC) (GLsizei n, const GLuint* textures, GLboolean* residences);
typedef void ( * PFNGLBINDTEXTUREEXTPROC) (GLenum target, GLuint texture);
typedef void ( * PFNGLDELETETEXTURESEXTPROC) (GLsizei n, const GLuint* textures);
typedef void ( * PFNGLGENTEXTURESEXTPROC) (GLsizei n, GLuint* textures);
typedef GLboolean ( * PFNGLISTEXTUREEXTPROC) (GLuint texture);
typedef void ( * PFNGLPRIORITIZETEXTURESEXTPROC) (GLsizei n, const GLuint* textures, const GLclampf* priorities);

typedef void ( * PFNGLTEXTURENORMALEXTPROC) (GLenum mode);

typedef void ( * PFNGLGETQUERYOBJECTI64VEXTPROC) (GLuint id, GLenum pname, GLint64EXT *params);
typedef void ( * PFNGLGETQUERYOBJECTUI64VEXTPROC) (GLuint id, GLenum pname, GLuint64EXT *params);

typedef void ( * PFNGLBEGINTRANSFORMFEEDBACKEXTPROC) (GLenum primitiveMode);
typedef void ( * PFNGLBINDBUFFERBASEEXTPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void ( * PFNGLBINDBUFFEROFFSETEXTPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset);
typedef void ( * PFNGLBINDBUFFERRANGEEXTPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void ( * PFNGLENDTRANSFORMFEEDBACKEXTPROC) (void);
typedef void ( * PFNGLGETTRANSFORMFEEDBACKVARYINGEXTPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei *size, GLenum *type, char *name);
typedef void ( * PFNGLTRANSFORMFEEDBACKVARYINGSEXTPROC) (GLuint program, GLsizei count, const char ** varyings, GLenum bufferMode);

typedef void ( * PFNGLARRAYELEMENTEXTPROC) (GLint i);
typedef void ( * PFNGLCOLORPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void* pointer);
typedef void ( * PFNGLDRAWARRAYSEXTPROC) (GLenum mode, GLint first, GLsizei count);
typedef void ( * PFNGLEDGEFLAGPOINTEREXTPROC) (GLsizei stride, GLsizei count, const GLboolean* pointer);
typedef void ( * PFNGLINDEXPOINTEREXTPROC) (GLenum type, GLsizei stride, GLsizei count, const void* pointer);
typedef void ( * PFNGLNORMALPOINTEREXTPROC) (GLenum type, GLsizei stride, GLsizei count, const void* pointer);
typedef void ( * PFNGLTEXCOORDPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void* pointer);
typedef void ( * PFNGLVERTEXPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void* pointer);

typedef void ( * PFNGLGETVERTEXATTRIBLDVEXTPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void ( * PFNGLVERTEXARRAYVERTEXATTRIBLOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void ( * PFNGLVERTEXATTRIBL1DEXTPROC) (GLuint index, GLdouble x);
typedef void ( * PFNGLVERTEXATTRIBL1DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIBL2DEXTPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void ( * PFNGLVERTEXATTRIBL2DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIBL3DEXTPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLVERTEXATTRIBL3DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIBL4DEXTPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void ( * PFNGLVERTEXATTRIBL4DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIBLPOINTEREXTPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);

typedef void ( * PFNGLBEGINVERTEXSHADEREXTPROC) (void);
typedef GLuint ( * PFNGLBINDLIGHTPARAMETEREXTPROC) (GLenum light, GLenum value);
typedef GLuint ( * PFNGLBINDMATERIALPARAMETEREXTPROC) (GLenum face, GLenum value);
typedef GLuint ( * PFNGLBINDPARAMETEREXTPROC) (GLenum value);
typedef GLuint ( * PFNGLBINDTEXGENPARAMETEREXTPROC) (GLenum unit, GLenum coord, GLenum value);
typedef GLuint ( * PFNGLBINDTEXTUREUNITPARAMETEREXTPROC) (GLenum unit, GLenum value);
typedef void ( * PFNGLBINDVERTEXSHADEREXTPROC) (GLuint id);
typedef void ( * PFNGLDELETEVERTEXSHADEREXTPROC) (GLuint id);
typedef void ( * PFNGLDISABLEVARIANTCLIENTSTATEEXTPROC) (GLuint id);
typedef void ( * PFNGLENABLEVARIANTCLIENTSTATEEXTPROC) (GLuint id);
typedef void ( * PFNGLENDVERTEXSHADEREXTPROC) (void);
typedef void ( * PFNGLEXTRACTCOMPONENTEXTPROC) (GLuint res, GLuint src, GLuint num);
typedef GLuint ( * PFNGLGENSYMBOLSEXTPROC) (GLenum dataType, GLenum storageType, GLenum range, GLuint components);
typedef GLuint ( * PFNGLGENVERTEXSHADERSEXTPROC) (GLuint range);
typedef void ( * PFNGLGETINVARIANTBOOLEANVEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void ( * PFNGLGETINVARIANTFLOATVEXTPROC) (GLuint id, GLenum value, GLfloat *data);
typedef void ( * PFNGLGETINVARIANTINTEGERVEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void ( * PFNGLGETLOCALCONSTANTBOOLEANVEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void ( * PFNGLGETLOCALCONSTANTFLOATVEXTPROC) (GLuint id, GLenum value, GLfloat *data);
typedef void ( * PFNGLGETLOCALCONSTANTINTEGERVEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void ( * PFNGLGETVARIANTBOOLEANVEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void ( * PFNGLGETVARIANTFLOATVEXTPROC) (GLuint id, GLenum value, GLfloat *data);
typedef void ( * PFNGLGETVARIANTINTEGERVEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void ( * PFNGLGETVARIANTPOINTERVEXTPROC) (GLuint id, GLenum value, GLvoid **data);
typedef void ( * PFNGLINSERTCOMPONENTEXTPROC) (GLuint res, GLuint src, GLuint num);
typedef GLboolean ( * PFNGLISVARIANTENABLEDEXTPROC) (GLuint id, GLenum cap);
typedef void ( * PFNGLSETINVARIANTEXTPROC) (GLuint id, GLenum type, GLvoid *addr);
typedef void ( * PFNGLSETLOCALCONSTANTEXTPROC) (GLuint id, GLenum type, GLvoid *addr);
typedef void ( * PFNGLSHADEROP1EXTPROC) (GLenum op, GLuint res, GLuint arg1);
typedef void ( * PFNGLSHADEROP2EXTPROC) (GLenum op, GLuint res, GLuint arg1, GLuint arg2);
typedef void ( * PFNGLSHADEROP3EXTPROC) (GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3);
typedef void ( * PFNGLSWIZZLEEXTPROC) (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
typedef void ( * PFNGLVARIANTPOINTEREXTPROC) (GLuint id, GLenum type, GLuint stride, GLvoid *addr);
typedef void ( * PFNGLVARIANTBVEXTPROC) (GLuint id, GLbyte *addr);
typedef void ( * PFNGLVARIANTDVEXTPROC) (GLuint id, GLdouble *addr);
typedef void ( * PFNGLVARIANTFVEXTPROC) (GLuint id, GLfloat *addr);
typedef void ( * PFNGLVARIANTIVEXTPROC) (GLuint id, GLint *addr);
typedef void ( * PFNGLVARIANTSVEXTPROC) (GLuint id, GLshort *addr);
typedef void ( * PFNGLVARIANTUBVEXTPROC) (GLuint id, GLubyte *addr);
typedef void ( * PFNGLVARIANTUIVEXTPROC) (GLuint id, GLuint *addr);
typedef void ( * PFNGLVARIANTUSVEXTPROC) (GLuint id, GLushort *addr);
typedef void ( * PFNGLWRITEMASKEXTPROC) (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);

typedef void ( * PFNGLVERTEXWEIGHTPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, void* pointer);
typedef void ( * PFNGLVERTEXWEIGHTFEXTPROC) (GLfloat weight);
typedef void ( * PFNGLVERTEXWEIGHTFVEXTPROC) (GLfloat* weight);

typedef GLsync ( * PFNGLIMPORTSYNCEXTPROC) (GLenum external_sync_type, GLintptr external_sync, GLbitfield flags);

typedef void ( * PFNGLFRAMETERMINATORGREMEDYPROC) (void);

typedef void ( * PFNGLSTRINGMARKERGREMEDYPROC) (GLsizei len, const void* string);

typedef void ( * PFNGLGETIMAGETRANSFORMPARAMETERFVHPPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLGETIMAGETRANSFORMPARAMETERIVHPPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void ( * PFNGLIMAGETRANSFORMPARAMETERFHPPROC) (GLenum target, GLenum pname, const GLfloat param);
typedef void ( * PFNGLIMAGETRANSFORMPARAMETERFVHPPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLIMAGETRANSFORMPARAMETERIHPPROC) (GLenum target, GLenum pname, const GLint param);
typedef void ( * PFNGLIMAGETRANSFORMPARAMETERIVHPPROC) (GLenum target, GLenum pname, const GLint* params);

typedef void ( * PFNGLMULTIMODEDRAWARRAYSIBMPROC) (const GLenum* mode, const GLint *first, const GLsizei *count, GLsizei primcount, GLint modestride);
typedef void ( * PFNGLMULTIMODEDRAWELEMENTSIBMPROC) (const GLenum* mode, const GLsizei *count, GLenum type, const GLvoid * const *indices, GLsizei primcount, GLint modestride);

typedef void ( * PFNGLCOLORPOINTERLISTIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void ( * PFNGLEDGEFLAGPOINTERLISTIBMPROC) (GLint stride, const GLboolean ** pointer, GLint ptrstride);
typedef void ( * PFNGLFOGCOORDPOINTERLISTIBMPROC) (GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void ( * PFNGLINDEXPOINTERLISTIBMPROC) (GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void ( * PFNGLNORMALPOINTERLISTIBMPROC) (GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void ( * PFNGLSECONDARYCOLORPOINTERLISTIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void ( * PFNGLTEXCOORDPOINTERLISTIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void ( * PFNGLVERTEXPOINTERLISTIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);

typedef void ( * PFNGLCOLORPOINTERVINTELPROC) (GLint size, GLenum type, const void** pointer);
typedef void ( * PFNGLNORMALPOINTERVINTELPROC) (GLenum type, const void** pointer);
typedef void ( * PFNGLTEXCOORDPOINTERVINTELPROC) (GLint size, GLenum type, const void** pointer);
typedef void ( * PFNGLVERTEXPOINTERVINTELPROC) (GLint size, GLenum type, const void** pointer);

typedef void ( * PFNGLTEXSCISSORFUNCINTELPROC) (GLenum target, GLenum lfunc, GLenum hfunc);
typedef void ( * PFNGLTEXSCISSORINTELPROC) (GLenum target, GLclampf tlow, GLclampf thigh);

typedef GLuint ( * PFNGLBUFFERREGIONENABLEDPROC) (void);
typedef void ( * PFNGLDELETEBUFFERREGIONPROC) (GLenum region);
typedef void ( * PFNGLDRAWBUFFERREGIONPROC) (GLuint region, GLint x, GLint y, GLsizei width, GLsizei height, GLint xDest, GLint yDest);
typedef GLuint ( * PFNGLNEWBUFFERREGIONPROC) (GLenum region);
typedef void ( * PFNGLREADBUFFERREGIONPROC) (GLuint region, GLint x, GLint y, GLsizei width, GLsizei height);

typedef void ( * PFNGLRESIZEBUFFERSMESAPROC) (void);

typedef void ( * PFNGLWINDOWPOS2DMESAPROC) (GLdouble x, GLdouble y);
typedef void ( * PFNGLWINDOWPOS2DVMESAPROC) (const GLdouble* p);
typedef void ( * PFNGLWINDOWPOS2FMESAPROC) (GLfloat x, GLfloat y);
typedef void ( * PFNGLWINDOWPOS2FVMESAPROC) (const GLfloat* p);
typedef void ( * PFNGLWINDOWPOS2IMESAPROC) (GLint x, GLint y);
typedef void ( * PFNGLWINDOWPOS2IVMESAPROC) (const GLint* p);
typedef void ( * PFNGLWINDOWPOS2SMESAPROC) (GLshort x, GLshort y);
typedef void ( * PFNGLWINDOWPOS2SVMESAPROC) (const GLshort* p);
typedef void ( * PFNGLWINDOWPOS3DMESAPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLWINDOWPOS3DVMESAPROC) (const GLdouble* p);
typedef void ( * PFNGLWINDOWPOS3FMESAPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLWINDOWPOS3FVMESAPROC) (const GLfloat* p);
typedef void ( * PFNGLWINDOWPOS3IMESAPROC) (GLint x, GLint y, GLint z);
typedef void ( * PFNGLWINDOWPOS3IVMESAPROC) (const GLint* p);
typedef void ( * PFNGLWINDOWPOS3SMESAPROC) (GLshort x, GLshort y, GLshort z);
typedef void ( * PFNGLWINDOWPOS3SVMESAPROC) (const GLshort* p);
typedef void ( * PFNGLWINDOWPOS4DMESAPROC) (GLdouble x, GLdouble y, GLdouble z, GLdouble);
typedef void ( * PFNGLWINDOWPOS4DVMESAPROC) (const GLdouble* p);
typedef void ( * PFNGLWINDOWPOS4FMESAPROC) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void ( * PFNGLWINDOWPOS4FVMESAPROC) (const GLfloat* p);
typedef void ( * PFNGLWINDOWPOS4IMESAPROC) (GLint x, GLint y, GLint z, GLint w);
typedef void ( * PFNGLWINDOWPOS4IVMESAPROC) (const GLint* p);
typedef void ( * PFNGLWINDOWPOS4SMESAPROC) (GLshort x, GLshort y, GLshort z, GLshort w);
typedef void ( * PFNGLWINDOWPOS4SVMESAPROC) (const GLshort* p);

typedef void ( * PFNGLBEGINCONDITIONALRENDERNVPROC) (GLuint id, GLenum mode);
typedef void ( * PFNGLENDCONDITIONALRENDERNVPROC) (void);

typedef void ( * PFNGLCOPYIMAGESUBDATANVPROC) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);

typedef void ( * PFNGLCLEARDEPTHDNVPROC) (GLdouble depth);
typedef void ( * PFNGLDEPTHBOUNDSDNVPROC) (GLdouble zmin, GLdouble zmax);
typedef void ( * PFNGLDEPTHRANGEDNVPROC) (GLdouble zNear, GLdouble zFar);

typedef void ( * PFNGLEVALMAPSNVPROC) (GLenum target, GLenum mode);
typedef void ( * PFNGLGETMAPATTRIBPARAMETERFVNVPROC) (GLenum target, GLuint index, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETMAPATTRIBPARAMETERIVNVPROC) (GLenum target, GLuint index, GLenum pname, GLint* params);
typedef void ( * PFNGLGETMAPCONTROLPOINTSNVPROC) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, void* points);
typedef void ( * PFNGLGETMAPPARAMETERFVNVPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETMAPPARAMETERIVNVPROC) (GLenum target, GLenum pname, GLint* params);
typedef void ( * PFNGLMAPCONTROLPOINTSNVPROC) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const void* points);
typedef void ( * PFNGLMAPPARAMETERFVNVPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLMAPPARAMETERIVNVPROC) (GLenum target, GLenum pname, const GLint* params);

typedef void ( * PFNGLGETMULTISAMPLEFVNVPROC) (GLenum pname, GLuint index, GLfloat* val);
typedef void ( * PFNGLSAMPLEMASKINDEXEDNVPROC) (GLuint index, GLbitfield mask);
typedef void ( * PFNGLTEXRENDERBUFFERNVPROC) (GLenum target, GLuint renderbuffer);

typedef void ( * PFNGLDELETEFENCESNVPROC) (GLsizei n, const GLuint* fences);
typedef void ( * PFNGLFINISHFENCENVPROC) (GLuint fence);
typedef void ( * PFNGLGENFENCESNVPROC) (GLsizei n, GLuint* fences);
typedef void ( * PFNGLGETFENCEIVNVPROC) (GLuint fence, GLenum pname, GLint* params);
typedef GLboolean ( * PFNGLISFENCENVPROC) (GLuint fence);
typedef void ( * PFNGLSETFENCENVPROC) (GLuint fence, GLenum condition);
typedef GLboolean ( * PFNGLTESTFENCENVPROC) (GLuint fence);

typedef void ( * PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC) (GLuint id, GLsizei len, const GLubyte* name, GLdouble *params);
typedef void ( * PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC) (GLuint id, GLsizei len, const GLubyte* name, GLfloat *params);
typedef void ( * PFNGLPROGRAMNAMEDPARAMETER4DNVPROC) (GLuint id, GLsizei len, const GLubyte* name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void ( * PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC) (GLuint id, GLsizei len, const GLubyte* name, const GLdouble v[]);
typedef void ( * PFNGLPROGRAMNAMEDPARAMETER4FNVPROC) (GLuint id, GLsizei len, const GLubyte* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void ( * PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC) (GLuint id, GLsizei len, const GLubyte* name, const GLfloat v[]);

typedef void ( * PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);

typedef void ( * PFNGLPROGRAMVERTEXLIMITNVPROC) (GLenum target, GLint limit);

typedef void ( * PFNGLPROGRAMENVPARAMETERI4INVPROC) (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void ( * PFNGLPROGRAMENVPARAMETERI4IVNVPROC) (GLenum target, GLuint index, const GLint *params);
typedef void ( * PFNGLPROGRAMENVPARAMETERI4UINVPROC) (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void ( * PFNGLPROGRAMENVPARAMETERI4UIVNVPROC) (GLenum target, GLuint index, const GLuint *params);
typedef void ( * PFNGLPROGRAMENVPARAMETERSI4IVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLint *params);
typedef void ( * PFNGLPROGRAMENVPARAMETERSI4UIVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLuint *params);
typedef void ( * PFNGLPROGRAMLOCALPARAMETERI4INVPROC) (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void ( * PFNGLPROGRAMLOCALPARAMETERI4IVNVPROC) (GLenum target, GLuint index, const GLint *params);
typedef void ( * PFNGLPROGRAMLOCALPARAMETERI4UINVPROC) (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void ( * PFNGLPROGRAMLOCALPARAMETERI4UIVNVPROC) (GLenum target, GLuint index, const GLuint *params);
typedef void ( * PFNGLPROGRAMLOCALPARAMETERSI4IVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLint *params);
typedef void ( * PFNGLPROGRAMLOCALPARAMETERSI4UIVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLuint *params);

typedef void ( * PFNGLGETUNIFORMI64VNVPROC) (GLuint program, GLint location, GLint64EXT* params);
typedef void ( * PFNGLGETUNIFORMUI64VNVPROC) (GLuint program, GLint location, GLuint64EXT* params);
typedef void ( * PFNGLPROGRAMUNIFORM1I64NVPROC) (GLuint program, GLint location, GLint64EXT x);
typedef void ( * PFNGLPROGRAMUNIFORM1I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
typedef void ( * PFNGLPROGRAMUNIFORM1UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x);
typedef void ( * PFNGLPROGRAMUNIFORM1UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void ( * PFNGLPROGRAMUNIFORM2I64NVPROC) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y);
typedef void ( * PFNGLPROGRAMUNIFORM2I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
typedef void ( * PFNGLPROGRAMUNIFORM2UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y);
typedef void ( * PFNGLPROGRAMUNIFORM2UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void ( * PFNGLPROGRAMUNIFORM3I64NVPROC) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
typedef void ( * PFNGLPROGRAMUNIFORM3I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
typedef void ( * PFNGLPROGRAMUNIFORM3UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
typedef void ( * PFNGLPROGRAMUNIFORM3UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void ( * PFNGLPROGRAMUNIFORM4I64NVPROC) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
typedef void ( * PFNGLPROGRAMUNIFORM4I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
typedef void ( * PFNGLPROGRAMUNIFORM4UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
typedef void ( * PFNGLPROGRAMUNIFORM4UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void ( * PFNGLUNIFORM1I64NVPROC) (GLint location, GLint64EXT x);
typedef void ( * PFNGLUNIFORM1I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT* value);
typedef void ( * PFNGLUNIFORM1UI64NVPROC) (GLint location, GLuint64EXT x);
typedef void ( * PFNGLUNIFORM1UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);
typedef void ( * PFNGLUNIFORM2I64NVPROC) (GLint location, GLint64EXT x, GLint64EXT y);
typedef void ( * PFNGLUNIFORM2I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT* value);
typedef void ( * PFNGLUNIFORM2UI64NVPROC) (GLint location, GLuint64EXT x, GLuint64EXT y);
typedef void ( * PFNGLUNIFORM2UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);
typedef void ( * PFNGLUNIFORM3I64NVPROC) (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
typedef void ( * PFNGLUNIFORM3I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT* value);
typedef void ( * PFNGLUNIFORM3UI64NVPROC) (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
typedef void ( * PFNGLUNIFORM3UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);
typedef void ( * PFNGLUNIFORM4I64NVPROC) (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
typedef void ( * PFNGLUNIFORM4I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT* value);
typedef void ( * PFNGLUNIFORM4UI64NVPROC) (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
typedef void ( * PFNGLUNIFORM4UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);

typedef unsigned short GLhalf;

typedef void ( * PFNGLCOLOR3HNVPROC) (GLhalf red, GLhalf green, GLhalf blue);
typedef void ( * PFNGLCOLOR3HVNVPROC) (const GLhalf* v);
typedef void ( * PFNGLCOLOR4HNVPROC) (GLhalf red, GLhalf green, GLhalf blue, GLhalf alpha);
typedef void ( * PFNGLCOLOR4HVNVPROC) (const GLhalf* v);
typedef void ( * PFNGLFOGCOORDHNVPROC) (GLhalf fog);
typedef void ( * PFNGLFOGCOORDHVNVPROC) (const GLhalf* fog);
typedef void ( * PFNGLMULTITEXCOORD1HNVPROC) (GLenum target, GLhalf s);
typedef void ( * PFNGLMULTITEXCOORD1HVNVPROC) (GLenum target, const GLhalf* v);
typedef void ( * PFNGLMULTITEXCOORD2HNVPROC) (GLenum target, GLhalf s, GLhalf t);
typedef void ( * PFNGLMULTITEXCOORD2HVNVPROC) (GLenum target, const GLhalf* v);
typedef void ( * PFNGLMULTITEXCOORD3HNVPROC) (GLenum target, GLhalf s, GLhalf t, GLhalf r);
typedef void ( * PFNGLMULTITEXCOORD3HVNVPROC) (GLenum target, const GLhalf* v);
typedef void ( * PFNGLMULTITEXCOORD4HNVPROC) (GLenum target, GLhalf s, GLhalf t, GLhalf r, GLhalf q);
typedef void ( * PFNGLMULTITEXCOORD4HVNVPROC) (GLenum target, const GLhalf* v);
typedef void ( * PFNGLNORMAL3HNVPROC) (GLhalf nx, GLhalf ny, GLhalf nz);
typedef void ( * PFNGLNORMAL3HVNVPROC) (const GLhalf* v);
typedef void ( * PFNGLSECONDARYCOLOR3HNVPROC) (GLhalf red, GLhalf green, GLhalf blue);
typedef void ( * PFNGLSECONDARYCOLOR3HVNVPROC) (const GLhalf* v);
typedef void ( * PFNGLTEXCOORD1HNVPROC) (GLhalf s);
typedef void ( * PFNGLTEXCOORD1HVNVPROC) (const GLhalf* v);
typedef void ( * PFNGLTEXCOORD2HNVPROC) (GLhalf s, GLhalf t);
typedef void ( * PFNGLTEXCOORD2HVNVPROC) (const GLhalf* v);
typedef void ( * PFNGLTEXCOORD3HNVPROC) (GLhalf s, GLhalf t, GLhalf r);
typedef void ( * PFNGLTEXCOORD3HVNVPROC) (const GLhalf* v);
typedef void ( * PFNGLTEXCOORD4HNVPROC) (GLhalf s, GLhalf t, GLhalf r, GLhalf q);
typedef void ( * PFNGLTEXCOORD4HVNVPROC) (const GLhalf* v);
typedef void ( * PFNGLVERTEX2HNVPROC) (GLhalf x, GLhalf y);
typedef void ( * PFNGLVERTEX2HVNVPROC) (const GLhalf* v);
typedef void ( * PFNGLVERTEX3HNVPROC) (GLhalf x, GLhalf y, GLhalf z);
typedef void ( * PFNGLVERTEX3HVNVPROC) (const GLhalf* v);
typedef void ( * PFNGLVERTEX4HNVPROC) (GLhalf x, GLhalf y, GLhalf z, GLhalf w);
typedef void ( * PFNGLVERTEX4HVNVPROC) (const GLhalf* v);
typedef void ( * PFNGLVERTEXATTRIB1HNVPROC) (GLuint index, GLhalf x);
typedef void ( * PFNGLVERTEXATTRIB1HVNVPROC) (GLuint index, const GLhalf* v);
typedef void ( * PFNGLVERTEXATTRIB2HNVPROC) (GLuint index, GLhalf x, GLhalf y);
typedef void ( * PFNGLVERTEXATTRIB2HVNVPROC) (GLuint index, const GLhalf* v);
typedef void ( * PFNGLVERTEXATTRIB3HNVPROC) (GLuint index, GLhalf x, GLhalf y, GLhalf z);
typedef void ( * PFNGLVERTEXATTRIB3HVNVPROC) (GLuint index, const GLhalf* v);
typedef void ( * PFNGLVERTEXATTRIB4HNVPROC) (GLuint index, GLhalf x, GLhalf y, GLhalf z, GLhalf w);
typedef void ( * PFNGLVERTEXATTRIB4HVNVPROC) (GLuint index, const GLhalf* v);
typedef void ( * PFNGLVERTEXATTRIBS1HVNVPROC) (GLuint index, GLsizei n, const GLhalf* v);
typedef void ( * PFNGLVERTEXATTRIBS2HVNVPROC) (GLuint index, GLsizei n, const GLhalf* v);
typedef void ( * PFNGLVERTEXATTRIBS3HVNVPROC) (GLuint index, GLsizei n, const GLhalf* v);
typedef void ( * PFNGLVERTEXATTRIBS4HVNVPROC) (GLuint index, GLsizei n, const GLhalf* v);
typedef void ( * PFNGLVERTEXWEIGHTHNVPROC) (GLhalf weight);
typedef void ( * PFNGLVERTEXWEIGHTHVNVPROC) (const GLhalf* weight);

typedef void ( * PFNGLBEGINOCCLUSIONQUERYNVPROC) (GLuint id);
typedef void ( * PFNGLDELETEOCCLUSIONQUERIESNVPROC) (GLsizei n, const GLuint* ids);
typedef void ( * PFNGLENDOCCLUSIONQUERYNVPROC) (void);
typedef void ( * PFNGLGENOCCLUSIONQUERIESNVPROC) (GLsizei n, GLuint* ids);
typedef void ( * PFNGLGETOCCLUSIONQUERYIVNVPROC) (GLuint id, GLenum pname, GLint* params);
typedef void ( * PFNGLGETOCCLUSIONQUERYUIVNVPROC) (GLuint id, GLenum pname, GLuint* params);
typedef GLboolean ( * PFNGLISOCCLUSIONQUERYNVPROC) (GLuint id);

typedef void ( * PFNGLPROGRAMBUFFERPARAMETERSIIVNVPROC) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLint *params);
typedef void ( * PFNGLPROGRAMBUFFERPARAMETERSIUIVNVPROC) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLuint *params);
typedef void ( * PFNGLPROGRAMBUFFERPARAMETERSFVNVPROC) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLfloat *params);

typedef void ( * PFNGLCOPYPATHNVPROC) (GLuint resultPath, GLuint srcPath);
typedef void ( * PFNGLCOVERFILLPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
typedef void ( * PFNGLCOVERFILLPATHNVPROC) (GLuint path, GLenum coverMode);
typedef void ( * PFNGLCOVERSTROKEPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
typedef void ( * PFNGLCOVERSTROKEPATHNVPROC) (GLuint name, GLenum coverMode);
typedef void ( * PFNGLDELETEPATHSNVPROC) (GLuint path, GLsizei range);
typedef GLuint ( * PFNGLGENPATHSNVPROC) (GLsizei range);
typedef void ( * PFNGLGETPATHCOLORGENFVNVPROC) (GLenum color, GLenum pname, GLfloat* value);
typedef void ( * PFNGLGETPATHCOLORGENIVNVPROC) (GLenum color, GLenum pname, GLint* value);
typedef void ( * PFNGLGETPATHCOMMANDSNVPROC) (GLuint name, GLubyte* commands);
typedef void ( * PFNGLGETPATHCOORDSNVPROC) (GLuint name, GLfloat* coords);
typedef void ( * PFNGLGETPATHDASHARRAYNVPROC) (GLuint name, GLfloat* dashArray);
typedef GLfloat ( * PFNGLGETPATHLENGTHNVPROC) (GLuint path, GLsizei startSegment, GLsizei numSegments);
typedef void ( * PFNGLGETPATHMETRICRANGENVPROC) (GLbitfield metricQueryMask, GLuint fistPathName, GLsizei numPaths, GLsizei stride, GLfloat* metrics);
typedef void ( * PFNGLGETPATHMETRICSNVPROC) (GLbitfield metricQueryMask, GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLsizei stride, GLfloat *metrics);
typedef void ( * PFNGLGETPATHPARAMETERFVNVPROC) (GLuint name, GLenum param, GLfloat* value);
typedef void ( * PFNGLGETPATHPARAMETERIVNVPROC) (GLuint name, GLenum param, GLint* value);
typedef void ( * PFNGLGETPATHSPACINGNVPROC) (GLenum pathListMode, GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLfloat advanceScale, GLfloat kerningScale, GLenum transformType, GLfloat *returnedSpacing);
typedef void ( * PFNGLGETPATHTEXGENFVNVPROC) (GLenum texCoordSet, GLenum pname, GLfloat* value);
typedef void ( * PFNGLGETPATHTEXGENIVNVPROC) (GLenum texCoordSet, GLenum pname, GLint* value);
typedef void ( * PFNGLINTERPOLATEPATHSNVPROC) (GLuint resultPath, GLuint pathA, GLuint pathB, GLfloat weight);
typedef GLboolean ( * PFNGLISPATHNVPROC) (GLuint path);
typedef GLboolean ( * PFNGLISPOINTINFILLPATHNVPROC) (GLuint path, GLuint mask, GLfloat x, GLfloat y);
typedef GLboolean ( * PFNGLISPOINTINSTROKEPATHNVPROC) (GLuint path, GLfloat x, GLfloat y);
typedef void ( * PFNGLPATHCOLORGENNVPROC) (GLenum color, GLenum genMode, GLenum colorFormat, const GLfloat* coeffs);
typedef void ( * PFNGLPATHCOMMANDSNVPROC) (GLuint path, GLsizei numCommands, const GLubyte* commands, GLsizei numCoords, GLenum coordType, const GLvoid*coords);
typedef void ( * PFNGLPATHCOORDSNVPROC) (GLuint path, GLsizei numCoords, GLenum coordType, const void* coords);
typedef void ( * PFNGLPATHCOVERDEPTHFUNCNVPROC) (GLenum zfunc);
typedef void ( * PFNGLPATHDASHARRAYNVPROC) (GLuint path, GLsizei dashCount, const GLfloat* dashArray);
typedef void ( * PFNGLPATHFOGGENNVPROC) (GLenum genMode);
typedef void ( * PFNGLPATHGLYPHRANGENVPROC) (GLuint firstPathName, GLenum fontTarget, const void* fontName, GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
typedef void ( * PFNGLPATHGLYPHSNVPROC) (GLuint firstPathName, GLenum fontTarget, const void* fontName, GLbitfield fontStyle, GLsizei numGlyphs, GLenum type, const GLvoid*charcodes, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
typedef void ( * PFNGLPATHPARAMETERFNVPROC) (GLuint path, GLenum pname, GLfloat value);
typedef void ( * PFNGLPATHPARAMETERFVNVPROC) (GLuint path, GLenum pname, const GLfloat* value);
typedef void ( * PFNGLPATHPARAMETERINVPROC) (GLuint path, GLenum pname, GLint value);
typedef void ( * PFNGLPATHPARAMETERIVNVPROC) (GLuint path, GLenum pname, const GLint* value);
typedef void ( * PFNGLPATHSTENCILDEPTHOFFSETNVPROC) (GLfloat factor, GLint units);
typedef void ( * PFNGLPATHSTENCILFUNCNVPROC) (GLenum func, GLint ref, GLuint mask);
typedef void ( * PFNGLPATHSTRINGNVPROC) (GLuint path, GLenum format, GLsizei length, const void* pathString);
typedef void ( * PFNGLPATHSUBCOMMANDSNVPROC) (GLuint path, GLsizei commandStart, GLsizei commandsToDelete, GLsizei numCommands, const GLubyte* commands, GLsizei numCoords, GLenum coordType, const GLvoid*coords);
typedef void ( * PFNGLPATHSUBCOORDSNVPROC) (GLuint path, GLsizei coordStart, GLsizei numCoords, GLenum coordType, const void* coords);
typedef void ( * PFNGLPATHTEXGENNVPROC) (GLenum texCoordSet, GLenum genMode, GLint components, const GLfloat* coeffs);
typedef GLboolean ( * PFNGLPOINTALONGPATHNVPROC) (GLuint path, GLsizei startSegment, GLsizei numSegments, GLfloat distance, GLfloat* x, GLfloat *y, GLfloat *tangentX, GLfloat *tangentY);
typedef void ( * PFNGLSTENCILFILLPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat *transformValues);
typedef void ( * PFNGLSTENCILFILLPATHNVPROC) (GLuint path, GLenum fillMode, GLuint mask);
typedef void ( * PFNGLSTENCILSTROKEPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLint reference, GLuint mask, GLenum transformType, const GLfloat *transformValues);
typedef void ( * PFNGLSTENCILSTROKEPATHNVPROC) (GLuint path, GLint reference, GLuint mask);
typedef void ( * PFNGLTRANSFORMPATHNVPROC) (GLuint resultPath, GLuint srcPath, GLenum transformType, const GLfloat* transformValues);
typedef void ( * PFNGLWEIGHTPATHSNVPROC) (GLuint resultPath, GLsizei numPaths, const GLuint paths[], const GLfloat weights[]);

typedef void ( * PFNGLFLUSHPIXELDATARANGENVPROC) (GLenum target);
typedef void ( * PFNGLPIXELDATARANGENVPROC) (GLenum target, GLsizei length, void* pointer);

typedef void ( * PFNGLPOINTPARAMETERINVPROC) (GLenum pname, GLint param);
typedef void ( * PFNGLPOINTPARAMETERIVNVPROC) (GLenum pname, const GLint* params);

typedef void ( * PFNGLGETVIDEOI64VNVPROC) (GLuint video_slot, GLenum pname, GLint64EXT* params);
typedef void ( * PFNGLGETVIDEOIVNVPROC) (GLuint video_slot, GLenum pname, GLint* params);
typedef void ( * PFNGLGETVIDEOUI64VNVPROC) (GLuint video_slot, GLenum pname, GLuint64EXT* params);
typedef void ( * PFNGLGETVIDEOUIVNVPROC) (GLuint video_slot, GLenum pname, GLuint* params);
typedef void ( * PFNGLPRESENTFRAMEDUALFILLNVPROC) (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLenum target1, GLuint fill1, GLenum target2, GLuint fill2, GLenum target3, GLuint fill3);
typedef void ( * PFNGLPRESENTFRAMEKEYEDNVPROC) (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLuint key0, GLenum target1, GLuint fill1, GLuint key1);

typedef void ( * PFNGLPRIMITIVERESTARTINDEXNVPROC) (GLuint index);
typedef void ( * PFNGLPRIMITIVERESTARTNVPROC) (void);

typedef void ( * PFNGLCOMBINERINPUTNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
typedef void ( * PFNGLCOMBINEROUTPUTNVPROC) (GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
typedef void ( * PFNGLCOMBINERPARAMETERFNVPROC) (GLenum pname, GLfloat param);
typedef void ( * PFNGLCOMBINERPARAMETERFVNVPROC) (GLenum pname, const GLfloat* params);
typedef void ( * PFNGLCOMBINERPARAMETERINVPROC) (GLenum pname, GLint param);
typedef void ( * PFNGLCOMBINERPARAMETERIVNVPROC) (GLenum pname, const GLint* params);
typedef void ( * PFNGLFINALCOMBINERINPUTNVPROC) (GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
typedef void ( * PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint* params);
typedef void ( * PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC) (GLenum stage, GLenum portion, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC) (GLenum stage, GLenum portion, GLenum pname, GLint* params);
typedef void ( * PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC) (GLenum variable, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC) (GLenum variable, GLenum pname, GLint* params);

typedef void ( * PFNGLCOMBINERSTAGEPARAMETERFVNVPROC) (GLenum stage, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC) (GLenum stage, GLenum pname, GLfloat* params);

typedef void ( * PFNGLGETBUFFERPARAMETERUI64VNVPROC) (GLenum target, GLenum pname, GLuint64EXT* params);
typedef void ( * PFNGLGETINTEGERUI64VNVPROC) (GLenum value, GLuint64EXT* result);
typedef void ( * PFNGLGETNAMEDBUFFERPARAMETERUI64VNVPROC) (GLuint buffer, GLenum pname, GLuint64EXT* params);
typedef GLboolean ( * PFNGLISBUFFERRESIDENTNVPROC) (GLenum target);
typedef GLboolean ( * PFNGLISNAMEDBUFFERRESIDENTNVPROC) (GLuint buffer);
typedef void ( * PFNGLMAKEBUFFERNONRESIDENTNVPROC) (GLenum target);
typedef void ( * PFNGLMAKEBUFFERRESIDENTNVPROC) (GLenum target, GLenum access);
typedef void ( * PFNGLMAKENAMEDBUFFERNONRESIDENTNVPROC) (GLuint buffer);
typedef void ( * PFNGLMAKENAMEDBUFFERRESIDENTNVPROC) (GLuint buffer, GLenum access);
typedef void ( * PFNGLPROGRAMUNIFORMUI64NVPROC) (GLuint program, GLint location, GLuint64EXT value);
typedef void ( * PFNGLPROGRAMUNIFORMUI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void ( * PFNGLUNIFORMUI64NVPROC) (GLint location, GLuint64EXT value);
typedef void ( * PFNGLUNIFORMUI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);

typedef void ( * PFNGLTEXTUREBARRIERNVPROC) (void);

typedef void ( * PFNGLTEXIMAGE2DMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
typedef void ( * PFNGLTEXIMAGE3DMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
typedef void ( * PFNGLTEXTUREIMAGE2DMULTISAMPLECOVERAGENVPROC) (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
typedef void ( * PFNGLTEXTUREIMAGE2DMULTISAMPLENVPROC) (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
typedef void ( * PFNGLTEXTUREIMAGE3DMULTISAMPLECOVERAGENVPROC) (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
typedef void ( * PFNGLTEXTUREIMAGE3DMULTISAMPLENVPROC) (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);

typedef void ( * PFNGLACTIVEVARYINGNVPROC) (GLuint program, const GLchar *name);
typedef void ( * PFNGLBEGINTRANSFORMFEEDBACKNVPROC) (GLenum primitiveMode);
typedef void ( * PFNGLBINDBUFFERBASENVPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void ( * PFNGLBINDBUFFEROFFSETNVPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset);
typedef void ( * PFNGLBINDBUFFERRANGENVPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void ( * PFNGLENDTRANSFORMFEEDBACKNVPROC) (void);
typedef void ( * PFNGLGETACTIVEVARYINGNVPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
typedef void ( * PFNGLGETTRANSFORMFEEDBACKVARYINGNVPROC) (GLuint program, GLuint index, GLint *location);
typedef GLint ( * PFNGLGETVARYINGLOCATIONNVPROC) (GLuint program, const GLchar *name);
typedef void ( * PFNGLTRANSFORMFEEDBACKATTRIBSNVPROC) (GLuint count, const GLint *attribs, GLenum bufferMode);
typedef void ( * PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC) (GLuint program, GLsizei count, const GLint *locations, GLenum bufferMode);

typedef void ( * PFNGLBINDTRANSFORMFEEDBACKNVPROC) (GLenum target, GLuint id);
typedef void ( * PFNGLDELETETRANSFORMFEEDBACKSNVPROC) (GLsizei n, const GLuint* ids);
typedef void ( * PFNGLDRAWTRANSFORMFEEDBACKNVPROC) (GLenum mode, GLuint id);
typedef void ( * PFNGLGENTRANSFORMFEEDBACKSNVPROC) (GLsizei n, GLuint* ids);
typedef GLboolean ( * PFNGLISTRANSFORMFEEDBACKNVPROC) (GLuint id);
typedef void ( * PFNGLPAUSETRANSFORMFEEDBACKNVPROC) (void);
typedef void ( * PFNGLRESUMETRANSFORMFEEDBACKNVPROC) (void);

typedef GLintptr GLvdpauSurfaceNV;

typedef void ( * PFNGLVDPAUFININVPROC) (void);
typedef void ( * PFNGLVDPAUGETSURFACEIVNVPROC) (GLvdpauSurfaceNV surface, GLenum pname, GLsizei bufSize, GLsizei* length, GLint *values);
typedef void ( * PFNGLVDPAUINITNVPROC) (const void* vdpDevice, const GLvoid*getProcAddress);
typedef void ( * PFNGLVDPAUISSURFACENVPROC) (GLvdpauSurfaceNV surface);
typedef void ( * PFNGLVDPAUMAPSURFACESNVPROC) (GLsizei numSurfaces, const GLvdpauSurfaceNV* surfaces);
typedef GLvdpauSurfaceNV ( * PFNGLVDPAUREGISTEROUTPUTSURFACENVPROC) (const void* vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
typedef GLvdpauSurfaceNV ( * PFNGLVDPAUREGISTERVIDEOSURFACENVPROC) (const void* vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
typedef void ( * PFNGLVDPAUSURFACEACCESSNVPROC) (GLvdpauSurfaceNV surface, GLenum access);
typedef void ( * PFNGLVDPAUUNMAPSURFACESNVPROC) (GLsizei numSurface, const GLvdpauSurfaceNV* surfaces);
typedef void ( * PFNGLVDPAUUNREGISTERSURFACENVPROC) (GLvdpauSurfaceNV surface);

typedef void ( * PFNGLFLUSHVERTEXARRAYRANGENVPROC) (void);
typedef void ( * PFNGLVERTEXARRAYRANGENVPROC) (GLsizei length, void* pointer);

typedef void ( * PFNGLGETVERTEXATTRIBLI64VNVPROC) (GLuint index, GLenum pname, GLint64EXT* params);
typedef void ( * PFNGLGETVERTEXATTRIBLUI64VNVPROC) (GLuint index, GLenum pname, GLuint64EXT* params);
typedef void ( * PFNGLVERTEXATTRIBL1I64NVPROC) (GLuint index, GLint64EXT x);
typedef void ( * PFNGLVERTEXATTRIBL1I64VNVPROC) (GLuint index, const GLint64EXT* v);
typedef void ( * PFNGLVERTEXATTRIBL1UI64NVPROC) (GLuint index, GLuint64EXT x);
typedef void ( * PFNGLVERTEXATTRIBL1UI64VNVPROC) (GLuint index, const GLuint64EXT* v);
typedef void ( * PFNGLVERTEXATTRIBL2I64NVPROC) (GLuint index, GLint64EXT x, GLint64EXT y);
typedef void ( * PFNGLVERTEXATTRIBL2I64VNVPROC) (GLuint index, const GLint64EXT* v);
typedef void ( * PFNGLVERTEXATTRIBL2UI64NVPROC) (GLuint index, GLuint64EXT x, GLuint64EXT y);
typedef void ( * PFNGLVERTEXATTRIBL2UI64VNVPROC) (GLuint index, const GLuint64EXT* v);
typedef void ( * PFNGLVERTEXATTRIBL3I64NVPROC) (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z);
typedef void ( * PFNGLVERTEXATTRIBL3I64VNVPROC) (GLuint index, const GLint64EXT* v);
typedef void ( * PFNGLVERTEXATTRIBL3UI64NVPROC) (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
typedef void ( * PFNGLVERTEXATTRIBL3UI64VNVPROC) (GLuint index, const GLuint64EXT* v);
typedef void ( * PFNGLVERTEXATTRIBL4I64NVPROC) (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
typedef void ( * PFNGLVERTEXATTRIBL4I64VNVPROC) (GLuint index, const GLint64EXT* v);
typedef void ( * PFNGLVERTEXATTRIBL4UI64NVPROC) (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
typedef void ( * PFNGLVERTEXATTRIBL4UI64VNVPROC) (GLuint index, const GLuint64EXT* v);
typedef void ( * PFNGLVERTEXATTRIBLFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLsizei stride);

typedef void ( * PFNGLBUFFERADDRESSRANGENVPROC) (GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length);
typedef void ( * PFNGLCOLORFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);
typedef void ( * PFNGLEDGEFLAGFORMATNVPROC) (GLsizei stride);
typedef void ( * PFNGLFOGCOORDFORMATNVPROC) (GLenum type, GLsizei stride);
typedef void ( * PFNGLGETINTEGERUI64I_VNVPROC) (GLenum value, GLuint index, GLuint64EXT result[]);
typedef void ( * PFNGLINDEXFORMATNVPROC) (GLenum type, GLsizei stride);
typedef void ( * PFNGLNORMALFORMATNVPROC) (GLenum type, GLsizei stride);
typedef void ( * PFNGLSECONDARYCOLORFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);
typedef void ( * PFNGLTEXCOORDFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);
typedef void ( * PFNGLVERTEXATTRIBFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride);
typedef void ( * PFNGLVERTEXATTRIBIFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLsizei stride);
typedef void ( * PFNGLVERTEXFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);

typedef GLboolean ( * PFNGLAREPROGRAMSRESIDENTNVPROC) (GLsizei n, const GLuint* ids, GLboolean *residences);
typedef void ( * PFNGLBINDPROGRAMNVPROC) (GLenum target, GLuint id);
typedef void ( * PFNGLDELETEPROGRAMSNVPROC) (GLsizei n, const GLuint* ids);
typedef void ( * PFNGLEXECUTEPROGRAMNVPROC) (GLenum target, GLuint id, const GLfloat* params);
typedef void ( * PFNGLGENPROGRAMSNVPROC) (GLsizei n, GLuint* ids);
typedef void ( * PFNGLGETPROGRAMPARAMETERDVNVPROC) (GLenum target, GLuint index, GLenum pname, GLdouble* params);
typedef void ( * PFNGLGETPROGRAMPARAMETERFVNVPROC) (GLenum target, GLuint index, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETPROGRAMSTRINGNVPROC) (GLuint id, GLenum pname, GLubyte* program);
typedef void ( * PFNGLGETPROGRAMIVNVPROC) (GLuint id, GLenum pname, GLint* params);
typedef void ( * PFNGLGETTRACKMATRIXIVNVPROC) (GLenum target, GLuint address, GLenum pname, GLint* params);
typedef void ( * PFNGLGETVERTEXATTRIBPOINTERVNVPROC) (GLuint index, GLenum pname, GLvoid** pointer);
typedef void ( * PFNGLGETVERTEXATTRIBDVNVPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void ( * PFNGLGETVERTEXATTRIBFVNVPROC) (GLuint index, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETVERTEXATTRIBIVNVPROC) (GLuint index, GLenum pname, GLint* params);
typedef GLboolean ( * PFNGLISPROGRAMNVPROC) (GLuint id);
typedef void ( * PFNGLLOADPROGRAMNVPROC) (GLenum target, GLuint id, GLsizei len, const GLubyte* program);
typedef void ( * PFNGLPROGRAMPARAMETER4DNVPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void ( * PFNGLPROGRAMPARAMETER4DVNVPROC) (GLenum target, GLuint index, const GLdouble* params);
typedef void ( * PFNGLPROGRAMPARAMETER4FNVPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void ( * PFNGLPROGRAMPARAMETER4FVNVPROC) (GLenum target, GLuint index, const GLfloat* params);
typedef void ( * PFNGLPROGRAMPARAMETERS4DVNVPROC) (GLenum target, GLuint index, GLsizei num, const GLdouble* params);
typedef void ( * PFNGLPROGRAMPARAMETERS4FVNVPROC) (GLenum target, GLuint index, GLsizei num, const GLfloat* params);
typedef void ( * PFNGLREQUESTRESIDENTPROGRAMSNVPROC) (GLsizei n, GLuint* ids);
typedef void ( * PFNGLTRACKMATRIXNVPROC) (GLenum target, GLuint address, GLenum matrix, GLenum transform);
typedef void ( * PFNGLVERTEXATTRIB1DNVPROC) (GLuint index, GLdouble x);
typedef void ( * PFNGLVERTEXATTRIB1DVNVPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIB1FNVPROC) (GLuint index, GLfloat x);
typedef void ( * PFNGLVERTEXATTRIB1FVNVPROC) (GLuint index, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIB1SNVPROC) (GLuint index, GLshort x);
typedef void ( * PFNGLVERTEXATTRIB1SVNVPROC) (GLuint index, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIB2DNVPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void ( * PFNGLVERTEXATTRIB2DVNVPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIB2FNVPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void ( * PFNGLVERTEXATTRIB2FVNVPROC) (GLuint index, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIB2SNVPROC) (GLuint index, GLshort x, GLshort y);
typedef void ( * PFNGLVERTEXATTRIB2SVNVPROC) (GLuint index, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIB3DNVPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void ( * PFNGLVERTEXATTRIB3DVNVPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIB3FNVPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLVERTEXATTRIB3FVNVPROC) (GLuint index, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIB3SNVPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void ( * PFNGLVERTEXATTRIB3SVNVPROC) (GLuint index, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIB4DNVPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void ( * PFNGLVERTEXATTRIB4DVNVPROC) (GLuint index, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIB4FNVPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void ( * PFNGLVERTEXATTRIB4FVNVPROC) (GLuint index, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIB4SNVPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void ( * PFNGLVERTEXATTRIB4SVNVPROC) (GLuint index, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIB4UBNVPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void ( * PFNGLVERTEXATTRIB4UBVNVPROC) (GLuint index, const GLubyte* v);
typedef void ( * PFNGLVERTEXATTRIBPOINTERNVPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
typedef void ( * PFNGLVERTEXATTRIBS1DVNVPROC) (GLuint index, GLsizei n, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIBS1FVNVPROC) (GLuint index, GLsizei n, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIBS1SVNVPROC) (GLuint index, GLsizei n, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIBS2DVNVPROC) (GLuint index, GLsizei n, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIBS2FVNVPROC) (GLuint index, GLsizei n, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIBS2SVNVPROC) (GLuint index, GLsizei n, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIBS3DVNVPROC) (GLuint index, GLsizei n, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIBS3FVNVPROC) (GLuint index, GLsizei n, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIBS3SVNVPROC) (GLuint index, GLsizei n, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIBS4DVNVPROC) (GLuint index, GLsizei n, const GLdouble* v);
typedef void ( * PFNGLVERTEXATTRIBS4FVNVPROC) (GLuint index, GLsizei n, const GLfloat* v);
typedef void ( * PFNGLVERTEXATTRIBS4SVNVPROC) (GLuint index, GLsizei n, const GLshort* v);
typedef void ( * PFNGLVERTEXATTRIBS4UBVNVPROC) (GLuint index, GLsizei n, const GLubyte* v);

typedef void ( * PFNGLBEGINVIDEOCAPTURENVPROC) (GLuint video_capture_slot);
typedef void ( * PFNGLBINDVIDEOCAPTURESTREAMBUFFERNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLintptrARB offset);
typedef void ( * PFNGLBINDVIDEOCAPTURESTREAMTEXTURENVPROC) (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLenum target, GLuint texture);
typedef void ( * PFNGLENDVIDEOCAPTURENVPROC) (GLuint video_capture_slot);
typedef void ( * PFNGLGETVIDEOCAPTURESTREAMDVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLdouble* params);
typedef void ( * PFNGLGETVIDEOCAPTURESTREAMFVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETVIDEOCAPTURESTREAMIVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLint* params);
typedef void ( * PFNGLGETVIDEOCAPTUREIVNVPROC) (GLuint video_capture_slot, GLenum pname, GLint* params);
typedef GLenum ( * PFNGLVIDEOCAPTURENVPROC) (GLuint video_capture_slot, GLuint* sequence_num, GLuint64EXT *capture_time);
typedef void ( * PFNGLVIDEOCAPTURESTREAMPARAMETERDVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLdouble* params);
typedef void ( * PFNGLVIDEOCAPTURESTREAMPARAMETERFVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLVIDEOCAPTURESTREAMPARAMETERIVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLint* params);

typedef void ( * PFNGLCLEARDEPTHFOESPROC) (GLclampd depth);
typedef void ( * PFNGLCLIPPLANEFOESPROC) (GLenum plane, const GLfloat* equation);
typedef void ( * PFNGLDEPTHRANGEFOESPROC) (GLclampf n, GLclampf f);
typedef void ( * PFNGLFRUSTUMFOESPROC) (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);
typedef void ( * PFNGLGETCLIPPLANEFOESPROC) (GLenum plane, GLfloat* equation);
typedef void ( * PFNGLORTHOFOESPROC) (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);

typedef void ( * PFNGLDETAILTEXFUNCSGISPROC) (GLenum target, GLsizei n, const GLfloat* points);
typedef void ( * PFNGLGETDETAILTEXFUNCSGISPROC) (GLenum target, GLfloat* points);

typedef void ( * PFNGLFOGFUNCSGISPROC) (GLsizei n, const GLfloat* points);
typedef void ( * PFNGLGETFOGFUNCSGISPROC) (GLfloat* points);

typedef void ( * PFNGLSAMPLEMASKSGISPROC) (GLclampf value, GLboolean invert);
typedef void ( * PFNGLSAMPLEPATTERNSGISPROC) (GLenum pattern);

typedef void ( * PFNGLGETSHARPENTEXFUNCSGISPROC) (GLenum target, GLfloat* points);
typedef void ( * PFNGLSHARPENTEXFUNCSGISPROC) (GLenum target, GLsizei n, const GLfloat* points);

typedef void ( * PFNGLTEXIMAGE4DSGISPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei extent, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void ( * PFNGLTEXSUBIMAGE4DSGISPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei extent, GLenum format, GLenum type, const void* pixels);

typedef void ( * PFNGLGETTEXFILTERFUNCSGISPROC) (GLenum target, GLenum filter, GLfloat* weights);
typedef void ( * PFNGLTEXFILTERFUNCSGISPROC) (GLenum target, GLenum filter, GLsizei n, const GLfloat* weights);

typedef void ( * PFNGLASYNCMARKERSGIXPROC) (GLuint marker);
typedef void ( * PFNGLDELETEASYNCMARKERSSGIXPROC) (GLuint marker, GLsizei range);
typedef GLint ( * PFNGLFINISHASYNCSGIXPROC) (GLuint* markerp);
typedef GLuint ( * PFNGLGENASYNCMARKERSSGIXPROC) (GLsizei range);
typedef GLboolean ( * PFNGLISASYNCMARKERSGIXPROC) (GLuint marker);
typedef GLint ( * PFNGLPOLLASYNCSGIXPROC) (GLuint* markerp);

typedef void ( * PFNGLFLUSHRASTERSGIXPROC) (void);

typedef void ( * PFNGLTEXTUREFOGSGIXPROC) (GLenum pname);

typedef void ( * PFNGLFRAGMENTCOLORMATERIALSGIXPROC) (GLenum face, GLenum mode);
typedef void ( * PFNGLFRAGMENTLIGHTMODELFSGIXPROC) (GLenum pname, GLfloat param);
typedef void ( * PFNGLFRAGMENTLIGHTMODELFVSGIXPROC) (GLenum pname, GLfloat* params);
typedef void ( * PFNGLFRAGMENTLIGHTMODELISGIXPROC) (GLenum pname, GLint param);
typedef void ( * PFNGLFRAGMENTLIGHTMODELIVSGIXPROC) (GLenum pname, GLint* params);
typedef void ( * PFNGLFRAGMENTLIGHTFSGIXPROC) (GLenum light, GLenum pname, GLfloat param);
typedef void ( * PFNGLFRAGMENTLIGHTFVSGIXPROC) (GLenum light, GLenum pname, GLfloat* params);
typedef void ( * PFNGLFRAGMENTLIGHTISGIXPROC) (GLenum light, GLenum pname, GLint param);
typedef void ( * PFNGLFRAGMENTLIGHTIVSGIXPROC) (GLenum light, GLenum pname, GLint* params);
typedef void ( * PFNGLFRAGMENTMATERIALFSGIXPROC) (GLenum face, GLenum pname, const GLfloat param);
typedef void ( * PFNGLFRAGMENTMATERIALFVSGIXPROC) (GLenum face, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLFRAGMENTMATERIALISGIXPROC) (GLenum face, GLenum pname, const GLint param);
typedef void ( * PFNGLFRAGMENTMATERIALIVSGIXPROC) (GLenum face, GLenum pname, const GLint* params);
typedef void ( * PFNGLGETFRAGMENTLIGHTFVSGIXPROC) (GLenum light, GLenum value, GLfloat* data);
typedef void ( * PFNGLGETFRAGMENTLIGHTIVSGIXPROC) (GLenum light, GLenum value, GLint* data);
typedef void ( * PFNGLGETFRAGMENTMATERIALFVSGIXPROC) (GLenum face, GLenum pname, const GLfloat* data);
typedef void ( * PFNGLGETFRAGMENTMATERIALIVSGIXPROC) (GLenum face, GLenum pname, const GLint* data);

typedef void ( * PFNGLFRAMEZOOMSGIXPROC) (GLint factor);

typedef void ( * PFNGLPIXELTEXGENSGIXPROC) (GLenum mode);

typedef void ( * PFNGLREFERENCEPLANESGIXPROC) (const GLdouble* equation);

typedef void ( * PFNGLSPRITEPARAMETERFSGIXPROC) (GLenum pname, GLfloat param);
typedef void ( * PFNGLSPRITEPARAMETERFVSGIXPROC) (GLenum pname, GLfloat* params);
typedef void ( * PFNGLSPRITEPARAMETERISGIXPROC) (GLenum pname, GLint param);
typedef void ( * PFNGLSPRITEPARAMETERIVSGIXPROC) (GLenum pname, GLint* params);

typedef void ( * PFNGLTAGSAMPLEBUFFERSGIXPROC) (void);

typedef void ( * PFNGLCOLORTABLEPARAMETERFVSGIPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void ( * PFNGLCOLORTABLEPARAMETERIVSGIPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void ( * PFNGLCOLORTABLESGIPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void* table);
typedef void ( * PFNGLCOPYCOLORTABLESGIPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void ( * PFNGLGETCOLORTABLEPARAMETERFVSGIPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void ( * PFNGLGETCOLORTABLEPARAMETERIVSGIPROC) (GLenum target, GLenum pname, GLint* params);
typedef void ( * PFNGLGETCOLORTABLESGIPROC) (GLenum target, GLenum format, GLenum type, void* table);

typedef void ( * PFNGLFINISHTEXTURESUNXPROC) (void);

typedef void ( * PFNGLGLOBALALPHAFACTORBSUNPROC) (GLbyte factor);
typedef void ( * PFNGLGLOBALALPHAFACTORDSUNPROC) (GLdouble factor);
typedef void ( * PFNGLGLOBALALPHAFACTORFSUNPROC) (GLfloat factor);
typedef void ( * PFNGLGLOBALALPHAFACTORISUNPROC) (GLint factor);
typedef void ( * PFNGLGLOBALALPHAFACTORSSUNPROC) (GLshort factor);
typedef void ( * PFNGLGLOBALALPHAFACTORUBSUNPROC) (GLubyte factor);
typedef void ( * PFNGLGLOBALALPHAFACTORUISUNPROC) (GLuint factor);
typedef void ( * PFNGLGLOBALALPHAFACTORUSSUNPROC) (GLushort factor);

typedef void ( * PFNGLREADVIDEOPIXELSSUNPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);

typedef void ( * PFNGLREPLACEMENTCODEPOINTERSUNPROC) (GLenum type, GLsizei stride, const void* pointer);
typedef void ( * PFNGLREPLACEMENTCODEUBSUNPROC) (GLubyte code);
typedef void ( * PFNGLREPLACEMENTCODEUBVSUNPROC) (const GLubyte* code);
typedef void ( * PFNGLREPLACEMENTCODEUISUNPROC) (GLuint code);
typedef void ( * PFNGLREPLACEMENTCODEUIVSUNPROC) (const GLuint* code);
typedef void ( * PFNGLREPLACEMENTCODEUSSUNPROC) (GLushort code);
typedef void ( * PFNGLREPLACEMENTCODEUSVSUNPROC) (const GLushort* code);

typedef void ( * PFNGLCOLOR3FVERTEX3FSUNPROC) (GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLCOLOR3FVERTEX3FVSUNPROC) (const GLfloat* c, const GLfloat *v);
typedef void ( * PFNGLCOLOR4FNORMAL3FVERTEX3FSUNPROC) (GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLCOLOR4FNORMAL3FVERTEX3FVSUNPROC) (const GLfloat* c, const GLfloat *n, const GLfloat *v);
typedef void ( * PFNGLCOLOR4UBVERTEX2FSUNPROC) (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y);
typedef void ( * PFNGLCOLOR4UBVERTEX2FVSUNPROC) (const GLubyte* c, const GLfloat *v);
typedef void ( * PFNGLCOLOR4UBVERTEX3FSUNPROC) (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLCOLOR4UBVERTEX3FVSUNPROC) (const GLubyte* c, const GLfloat *v);
typedef void ( * PFNGLNORMAL3FVERTEX3FSUNPROC) (GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLNORMAL3FVERTEX3FVSUNPROC) (const GLfloat* n, const GLfloat *v);
typedef void ( * PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FSUNPROC) (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *c, const GLfloat *v);
typedef void ( * PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FSUNPROC) (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void ( * PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FSUNPROC) (GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FVSUNPROC) (const GLuint* rc, const GLubyte *c, const GLfloat *v);
typedef void ( * PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FSUNPROC) (GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *n, const GLfloat *v);
typedef void ( * PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC) (GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void ( * PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FSUNPROC) (GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *tc, const GLfloat *n, const GLfloat *v);
typedef void ( * PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FSUNPROC) (GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *tc, const GLfloat *v);
typedef void ( * PFNGLREPLACEMENTCODEUIVERTEX3FSUNPROC) (GLuint rc, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLREPLACEMENTCODEUIVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *v);
typedef void ( * PFNGLTEXCOORD2FCOLOR3FVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLTEXCOORD2FCOLOR3FVERTEX3FVSUNPROC) (const GLfloat* tc, const GLfloat *c, const GLfloat *v);
typedef void ( * PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC) (const GLfloat* tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void ( * PFNGLTEXCOORD2FCOLOR4UBVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLTEXCOORD2FCOLOR4UBVERTEX3FVSUNPROC) (const GLfloat* tc, const GLubyte *c, const GLfloat *v);
typedef void ( * PFNGLTEXCOORD2FNORMAL3FVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLTEXCOORD2FNORMAL3FVERTEX3FVSUNPROC) (const GLfloat* tc, const GLfloat *n, const GLfloat *v);
typedef void ( * PFNGLTEXCOORD2FVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
typedef void ( * PFNGLTEXCOORD2FVERTEX3FVSUNPROC) (const GLfloat* tc, const GLfloat *v);
typedef void ( * PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FSUNPROC) (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void ( * PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FVSUNPROC) (const GLfloat* tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void ( * PFNGLTEXCOORD4FVERTEX4FSUNPROC) (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void ( * PFNGLTEXCOORD4FVERTEX4FVSUNPROC) (const GLfloat* tc, const GLfloat *v);

typedef void ( * PFNGLADDSWAPHINTRECTWINPROC) (GLint x, GLint y, GLsizei width, GLsizei height);

extern PFNGLCOPYTEXSUBIMAGE3DPROC __glewCopyTexSubImage3D;
extern PFNGLDRAWRANGEELEMENTSPROC __glewDrawRangeElements;
extern PFNGLTEXIMAGE3DPROC __glewTexImage3D;
extern PFNGLTEXSUBIMAGE3DPROC __glewTexSubImage3D;

extern PFNGLACTIVETEXTUREPROC __glewActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREPROC __glewClientActiveTexture;
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC __glewCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC __glewCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC __glewCompressedTexImage3D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC __glewCompressedTexSubImage1D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC __glewCompressedTexSubImage2D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC __glewCompressedTexSubImage3D;
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC __glewGetCompressedTexImage;
extern PFNGLLOADTRANSPOSEMATRIXDPROC __glewLoadTransposeMatrixd;
extern PFNGLLOADTRANSPOSEMATRIXFPROC __glewLoadTransposeMatrixf;
extern PFNGLMULTTRANSPOSEMATRIXDPROC __glewMultTransposeMatrixd;
extern PFNGLMULTTRANSPOSEMATRIXFPROC __glewMultTransposeMatrixf;
extern PFNGLMULTITEXCOORD1DPROC __glewMultiTexCoord1d;
extern PFNGLMULTITEXCOORD1DVPROC __glewMultiTexCoord1dv;
extern PFNGLMULTITEXCOORD1FPROC __glewMultiTexCoord1f;
extern PFNGLMULTITEXCOORD1FVPROC __glewMultiTexCoord1fv;
extern PFNGLMULTITEXCOORD1IPROC __glewMultiTexCoord1i;
extern PFNGLMULTITEXCOORD1IVPROC __glewMultiTexCoord1iv;
extern PFNGLMULTITEXCOORD1SPROC __glewMultiTexCoord1s;
extern PFNGLMULTITEXCOORD1SVPROC __glewMultiTexCoord1sv;
extern PFNGLMULTITEXCOORD2DPROC __glewMultiTexCoord2d;
extern PFNGLMULTITEXCOORD2DVPROC __glewMultiTexCoord2dv;
extern PFNGLMULTITEXCOORD2FPROC __glewMultiTexCoord2f;
extern PFNGLMULTITEXCOORD2FVPROC __glewMultiTexCoord2fv;
extern PFNGLMULTITEXCOORD2IPROC __glewMultiTexCoord2i;
extern PFNGLMULTITEXCOORD2IVPROC __glewMultiTexCoord2iv;
extern PFNGLMULTITEXCOORD2SPROC __glewMultiTexCoord2s;
extern PFNGLMULTITEXCOORD2SVPROC __glewMultiTexCoord2sv;
extern PFNGLMULTITEXCOORD3DPROC __glewMultiTexCoord3d;
extern PFNGLMULTITEXCOORD3DVPROC __glewMultiTexCoord3dv;
extern PFNGLMULTITEXCOORD3FPROC __glewMultiTexCoord3f;
extern PFNGLMULTITEXCOORD3FVPROC __glewMultiTexCoord3fv;
extern PFNGLMULTITEXCOORD3IPROC __glewMultiTexCoord3i;
extern PFNGLMULTITEXCOORD3IVPROC __glewMultiTexCoord3iv;
extern PFNGLMULTITEXCOORD3SPROC __glewMultiTexCoord3s;
extern PFNGLMULTITEXCOORD3SVPROC __glewMultiTexCoord3sv;
extern PFNGLMULTITEXCOORD4DPROC __glewMultiTexCoord4d;
extern PFNGLMULTITEXCOORD4DVPROC __glewMultiTexCoord4dv;
extern PFNGLMULTITEXCOORD4FPROC __glewMultiTexCoord4f;
extern PFNGLMULTITEXCOORD4FVPROC __glewMultiTexCoord4fv;
extern PFNGLMULTITEXCOORD4IPROC __glewMultiTexCoord4i;
extern PFNGLMULTITEXCOORD4IVPROC __glewMultiTexCoord4iv;
extern PFNGLMULTITEXCOORD4SPROC __glewMultiTexCoord4s;
extern PFNGLMULTITEXCOORD4SVPROC __glewMultiTexCoord4sv;
extern PFNGLSAMPLECOVERAGEPROC __glewSampleCoverage;

extern PFNGLBLENDCOLORPROC __glewBlendColor;
extern PFNGLBLENDEQUATIONPROC __glewBlendEquation;
extern PFNGLBLENDFUNCSEPARATEPROC __glewBlendFuncSeparate;
extern PFNGLFOGCOORDPOINTERPROC __glewFogCoordPointer;
extern PFNGLFOGCOORDDPROC __glewFogCoordd;
extern PFNGLFOGCOORDDVPROC __glewFogCoorddv;
extern PFNGLFOGCOORDFPROC __glewFogCoordf;
extern PFNGLFOGCOORDFVPROC __glewFogCoordfv;
extern PFNGLMULTIDRAWARRAYSPROC __glewMultiDrawArrays;
extern PFNGLMULTIDRAWELEMENTSPROC __glewMultiDrawElements;
extern PFNGLPOINTPARAMETERFPROC __glewPointParameterf;
extern PFNGLPOINTPARAMETERFVPROC __glewPointParameterfv;
extern PFNGLPOINTPARAMETERIPROC __glewPointParameteri;
extern PFNGLPOINTPARAMETERIVPROC __glewPointParameteriv;
extern PFNGLSECONDARYCOLOR3BPROC __glewSecondaryColor3b;
extern PFNGLSECONDARYCOLOR3BVPROC __glewSecondaryColor3bv;
extern PFNGLSECONDARYCOLOR3DPROC __glewSecondaryColor3d;
extern PFNGLSECONDARYCOLOR3DVPROC __glewSecondaryColor3dv;
extern PFNGLSECONDARYCOLOR3FPROC __glewSecondaryColor3f;
extern PFNGLSECONDARYCOLOR3FVPROC __glewSecondaryColor3fv;
extern PFNGLSECONDARYCOLOR3IPROC __glewSecondaryColor3i;
extern PFNGLSECONDARYCOLOR3IVPROC __glewSecondaryColor3iv;
extern PFNGLSECONDARYCOLOR3SPROC __glewSecondaryColor3s;
extern PFNGLSECONDARYCOLOR3SVPROC __glewSecondaryColor3sv;
extern PFNGLSECONDARYCOLOR3UBPROC __glewSecondaryColor3ub;
extern PFNGLSECONDARYCOLOR3UBVPROC __glewSecondaryColor3ubv;
extern PFNGLSECONDARYCOLOR3UIPROC __glewSecondaryColor3ui;
extern PFNGLSECONDARYCOLOR3UIVPROC __glewSecondaryColor3uiv;
extern PFNGLSECONDARYCOLOR3USPROC __glewSecondaryColor3us;
extern PFNGLSECONDARYCOLOR3USVPROC __glewSecondaryColor3usv;
extern PFNGLSECONDARYCOLORPOINTERPROC __glewSecondaryColorPointer;
extern PFNGLWINDOWPOS2DPROC __glewWindowPos2d;
extern PFNGLWINDOWPOS2DVPROC __glewWindowPos2dv;
extern PFNGLWINDOWPOS2FPROC __glewWindowPos2f;
extern PFNGLWINDOWPOS2FVPROC __glewWindowPos2fv;
extern PFNGLWINDOWPOS2IPROC __glewWindowPos2i;
extern PFNGLWINDOWPOS2IVPROC __glewWindowPos2iv;
extern PFNGLWINDOWPOS2SPROC __glewWindowPos2s;
extern PFNGLWINDOWPOS2SVPROC __glewWindowPos2sv;
extern PFNGLWINDOWPOS3DPROC __glewWindowPos3d;
extern PFNGLWINDOWPOS3DVPROC __glewWindowPos3dv;
extern PFNGLWINDOWPOS3FPROC __glewWindowPos3f;
extern PFNGLWINDOWPOS3FVPROC __glewWindowPos3fv;
extern PFNGLWINDOWPOS3IPROC __glewWindowPos3i;
extern PFNGLWINDOWPOS3IVPROC __glewWindowPos3iv;
extern PFNGLWINDOWPOS3SPROC __glewWindowPos3s;
extern PFNGLWINDOWPOS3SVPROC __glewWindowPos3sv;

extern PFNGLBEGINQUERYPROC __glewBeginQuery;
extern PFNGLBINDBUFFERPROC __glewBindBuffer;
extern PFNGLBUFFERDATAPROC __glewBufferData;
extern PFNGLBUFFERSUBDATAPROC __glewBufferSubData;
extern PFNGLDELETEBUFFERSPROC __glewDeleteBuffers;
extern PFNGLDELETEQUERIESPROC __glewDeleteQueries;
extern PFNGLENDQUERYPROC __glewEndQuery;
extern PFNGLGENBUFFERSPROC __glewGenBuffers;
extern PFNGLGENQUERIESPROC __glewGenQueries;
extern PFNGLGETBUFFERPARAMETERIVPROC __glewGetBufferParameteriv;
extern PFNGLGETBUFFERPOINTERVPROC __glewGetBufferPointerv;
extern PFNGLGETBUFFERSUBDATAPROC __glewGetBufferSubData;
extern PFNGLGETQUERYOBJECTIVPROC __glewGetQueryObjectiv;
extern PFNGLGETQUERYOBJECTUIVPROC __glewGetQueryObjectuiv;
extern PFNGLGETQUERYIVPROC __glewGetQueryiv;
extern PFNGLISBUFFERPROC __glewIsBuffer;
extern PFNGLISQUERYPROC __glewIsQuery;
extern PFNGLMAPBUFFERPROC __glewMapBuffer;
extern PFNGLUNMAPBUFFERPROC __glewUnmapBuffer;

extern PFNGLATTACHSHADERPROC __glewAttachShader;
extern PFNGLBINDATTRIBLOCATIONPROC __glewBindAttribLocation;
extern PFNGLBLENDEQUATIONSEPARATEPROC __glewBlendEquationSeparate;
extern PFNGLCOMPILESHADERPROC __glewCompileShader;
extern PFNGLCREATEPROGRAMPROC __glewCreateProgram;
extern PFNGLCREATESHADERPROC __glewCreateShader;
extern PFNGLDELETEPROGRAMPROC __glewDeleteProgram;
extern PFNGLDELETESHADERPROC __glewDeleteShader;
extern PFNGLDETACHSHADERPROC __glewDetachShader;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC __glewDisableVertexAttribArray;
extern PFNGLDRAWBUFFERSPROC __glewDrawBuffers;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC __glewEnableVertexAttribArray;
extern PFNGLGETACTIVEATTRIBPROC __glewGetActiveAttrib;
extern PFNGLGETACTIVEUNIFORMPROC __glewGetActiveUniform;
extern PFNGLGETATTACHEDSHADERSPROC __glewGetAttachedShaders;
extern PFNGLGETATTRIBLOCATIONPROC __glewGetAttribLocation;
extern PFNGLGETPROGRAMINFOLOGPROC __glewGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC __glewGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC __glewGetShaderInfoLog;
extern PFNGLGETSHADERSOURCEPROC __glewGetShaderSource;
extern PFNGLGETSHADERIVPROC __glewGetShaderiv;
extern PFNGLGETUNIFORMLOCATIONPROC __glewGetUniformLocation;
extern PFNGLGETUNIFORMFVPROC __glewGetUniformfv;
extern PFNGLGETUNIFORMIVPROC __glewGetUniformiv;
extern PFNGLGETVERTEXATTRIBPOINTERVPROC __glewGetVertexAttribPointerv;
extern PFNGLGETVERTEXATTRIBDVPROC __glewGetVertexAttribdv;
extern PFNGLGETVERTEXATTRIBFVPROC __glewGetVertexAttribfv;
extern PFNGLGETVERTEXATTRIBIVPROC __glewGetVertexAttribiv;
extern PFNGLISPROGRAMPROC __glewIsProgram;
extern PFNGLISSHADERPROC __glewIsShader;
extern PFNGLLINKPROGRAMPROC __glewLinkProgram;
extern PFNGLSHADERSOURCEPROC __glewShaderSource;
extern PFNGLSTENCILFUNCSEPARATEPROC __glewStencilFuncSeparate;
extern PFNGLSTENCILMASKSEPARATEPROC __glewStencilMaskSeparate;
extern PFNGLSTENCILOPSEPARATEPROC __glewStencilOpSeparate;
extern PFNGLUNIFORM1FPROC __glewUniform1f;
extern PFNGLUNIFORM1FVPROC __glewUniform1fv;
extern PFNGLUNIFORM1IPROC __glewUniform1i;
extern PFNGLUNIFORM1IVPROC __glewUniform1iv;
extern PFNGLUNIFORM2FPROC __glewUniform2f;
extern PFNGLUNIFORM2FVPROC __glewUniform2fv;
extern PFNGLUNIFORM2IPROC __glewUniform2i;
extern PFNGLUNIFORM2IVPROC __glewUniform2iv;
extern PFNGLUNIFORM3FPROC __glewUniform3f;
extern PFNGLUNIFORM3FVPROC __glewUniform3fv;
extern PFNGLUNIFORM3IPROC __glewUniform3i;
extern PFNGLUNIFORM3IVPROC __glewUniform3iv;
extern PFNGLUNIFORM4FPROC __glewUniform4f;
extern PFNGLUNIFORM4FVPROC __glewUniform4fv;
extern PFNGLUNIFORM4IPROC __glewUniform4i;
extern PFNGLUNIFORM4IVPROC __glewUniform4iv;
extern PFNGLUNIFORMMATRIX2FVPROC __glewUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC __glewUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC __glewUniformMatrix4fv;
extern PFNGLUSEPROGRAMPROC __glewUseProgram;
extern PFNGLVALIDATEPROGRAMPROC __glewValidateProgram;
extern PFNGLVERTEXATTRIB1DPROC __glewVertexAttrib1d;
extern PFNGLVERTEXATTRIB1DVPROC __glewVertexAttrib1dv;
extern PFNGLVERTEXATTRIB1FPROC __glewVertexAttrib1f;
extern PFNGLVERTEXATTRIB1FVPROC __glewVertexAttrib1fv;
extern PFNGLVERTEXATTRIB1SPROC __glewVertexAttrib1s;
extern PFNGLVERTEXATTRIB1SVPROC __glewVertexAttrib1sv;
extern PFNGLVERTEXATTRIB2DPROC __glewVertexAttrib2d;
extern PFNGLVERTEXATTRIB2DVPROC __glewVertexAttrib2dv;
extern PFNGLVERTEXATTRIB2FPROC __glewVertexAttrib2f;
extern PFNGLVERTEXATTRIB2FVPROC __glewVertexAttrib2fv;
extern PFNGLVERTEXATTRIB2SPROC __glewVertexAttrib2s;
extern PFNGLVERTEXATTRIB2SVPROC __glewVertexAttrib2sv;
extern PFNGLVERTEXATTRIB3DPROC __glewVertexAttrib3d;
extern PFNGLVERTEXATTRIB3DVPROC __glewVertexAttrib3dv;
extern PFNGLVERTEXATTRIB3FPROC __glewVertexAttrib3f;
extern PFNGLVERTEXATTRIB3FVPROC __glewVertexAttrib3fv;
extern PFNGLVERTEXATTRIB3SPROC __glewVertexAttrib3s;
extern PFNGLVERTEXATTRIB3SVPROC __glewVertexAttrib3sv;
extern PFNGLVERTEXATTRIB4NBVPROC __glewVertexAttrib4Nbv;
extern PFNGLVERTEXATTRIB4NIVPROC __glewVertexAttrib4Niv;
extern PFNGLVERTEXATTRIB4NSVPROC __glewVertexAttrib4Nsv;
extern PFNGLVERTEXATTRIB4NUBPROC __glewVertexAttrib4Nub;
extern PFNGLVERTEXATTRIB4NUBVPROC __glewVertexAttrib4Nubv;
extern PFNGLVERTEXATTRIB4NUIVPROC __glewVertexAttrib4Nuiv;
extern PFNGLVERTEXATTRIB4NUSVPROC __glewVertexAttrib4Nusv;
extern PFNGLVERTEXATTRIB4BVPROC __glewVertexAttrib4bv;
extern PFNGLVERTEXATTRIB4DPROC __glewVertexAttrib4d;
extern PFNGLVERTEXATTRIB4DVPROC __glewVertexAttrib4dv;
extern PFNGLVERTEXATTRIB4FPROC __glewVertexAttrib4f;
extern PFNGLVERTEXATTRIB4FVPROC __glewVertexAttrib4fv;
extern PFNGLVERTEXATTRIB4IVPROC __glewVertexAttrib4iv;
extern PFNGLVERTEXATTRIB4SPROC __glewVertexAttrib4s;
extern PFNGLVERTEXATTRIB4SVPROC __glewVertexAttrib4sv;
extern PFNGLVERTEXATTRIB4UBVPROC __glewVertexAttrib4ubv;
extern PFNGLVERTEXATTRIB4UIVPROC __glewVertexAttrib4uiv;
extern PFNGLVERTEXATTRIB4USVPROC __glewVertexAttrib4usv;
extern PFNGLVERTEXATTRIBPOINTERPROC __glewVertexAttribPointer;

extern PFNGLUNIFORMMATRIX2X3FVPROC __glewUniformMatrix2x3fv;
extern PFNGLUNIFORMMATRIX2X4FVPROC __glewUniformMatrix2x4fv;
extern PFNGLUNIFORMMATRIX3X2FVPROC __glewUniformMatrix3x2fv;
extern PFNGLUNIFORMMATRIX3X4FVPROC __glewUniformMatrix3x4fv;
extern PFNGLUNIFORMMATRIX4X2FVPROC __glewUniformMatrix4x2fv;
extern PFNGLUNIFORMMATRIX4X3FVPROC __glewUniformMatrix4x3fv;

extern PFNGLBEGINCONDITIONALRENDERPROC __glewBeginConditionalRender;
extern PFNGLBEGINTRANSFORMFEEDBACKPROC __glewBeginTransformFeedback;
extern PFNGLBINDFRAGDATALOCATIONPROC __glewBindFragDataLocation;
extern PFNGLCLAMPCOLORPROC __glewClampColor;
extern PFNGLCLEARBUFFERFIPROC __glewClearBufferfi;
extern PFNGLCLEARBUFFERFVPROC __glewClearBufferfv;
extern PFNGLCLEARBUFFERIVPROC __glewClearBufferiv;
extern PFNGLCLEARBUFFERUIVPROC __glewClearBufferuiv;
extern PFNGLCOLORMASKIPROC __glewColorMaski;
extern PFNGLDISABLEIPROC __glewDisablei;
extern PFNGLENABLEIPROC __glewEnablei;
extern PFNGLENDCONDITIONALRENDERPROC __glewEndConditionalRender;
extern PFNGLENDTRANSFORMFEEDBACKPROC __glewEndTransformFeedback;
extern PFNGLGETBOOLEANI_VPROC __glewGetBooleani_v;
extern PFNGLGETFRAGDATALOCATIONPROC __glewGetFragDataLocation;
extern PFNGLGETSTRINGIPROC __glewGetStringi;
extern PFNGLGETTEXPARAMETERIIVPROC __glewGetTexParameterIiv;
extern PFNGLGETTEXPARAMETERIUIVPROC __glewGetTexParameterIuiv;
extern PFNGLGETTRANSFORMFEEDBACKVARYINGPROC __glewGetTransformFeedbackVarying;
extern PFNGLGETUNIFORMUIVPROC __glewGetUniformuiv;
extern PFNGLGETVERTEXATTRIBIIVPROC __glewGetVertexAttribIiv;
extern PFNGLGETVERTEXATTRIBIUIVPROC __glewGetVertexAttribIuiv;
extern PFNGLISENABLEDIPROC __glewIsEnabledi;
extern PFNGLTEXPARAMETERIIVPROC __glewTexParameterIiv;
extern PFNGLTEXPARAMETERIUIVPROC __glewTexParameterIuiv;
extern PFNGLTRANSFORMFEEDBACKVARYINGSPROC __glewTransformFeedbackVaryings;
extern PFNGLUNIFORM1UIPROC __glewUniform1ui;
extern PFNGLUNIFORM1UIVPROC __glewUniform1uiv;
extern PFNGLUNIFORM2UIPROC __glewUniform2ui;
extern PFNGLUNIFORM2UIVPROC __glewUniform2uiv;
extern PFNGLUNIFORM3UIPROC __glewUniform3ui;
extern PFNGLUNIFORM3UIVPROC __glewUniform3uiv;
extern PFNGLUNIFORM4UIPROC __glewUniform4ui;
extern PFNGLUNIFORM4UIVPROC __glewUniform4uiv;
extern PFNGLVERTEXATTRIBI1IPROC __glewVertexAttribI1i;
extern PFNGLVERTEXATTRIBI1IVPROC __glewVertexAttribI1iv;
extern PFNGLVERTEXATTRIBI1UIPROC __glewVertexAttribI1ui;
extern PFNGLVERTEXATTRIBI1UIVPROC __glewVertexAttribI1uiv;
extern PFNGLVERTEXATTRIBI2IPROC __glewVertexAttribI2i;
extern PFNGLVERTEXATTRIBI2IVPROC __glewVertexAttribI2iv;
extern PFNGLVERTEXATTRIBI2UIPROC __glewVertexAttribI2ui;
extern PFNGLVERTEXATTRIBI2UIVPROC __glewVertexAttribI2uiv;
extern PFNGLVERTEXATTRIBI3IPROC __glewVertexAttribI3i;
extern PFNGLVERTEXATTRIBI3IVPROC __glewVertexAttribI3iv;
extern PFNGLVERTEXATTRIBI3UIPROC __glewVertexAttribI3ui;
extern PFNGLVERTEXATTRIBI3UIVPROC __glewVertexAttribI3uiv;
extern PFNGLVERTEXATTRIBI4BVPROC __glewVertexAttribI4bv;
extern PFNGLVERTEXATTRIBI4IPROC __glewVertexAttribI4i;
extern PFNGLVERTEXATTRIBI4IVPROC __glewVertexAttribI4iv;
extern PFNGLVERTEXATTRIBI4SVPROC __glewVertexAttribI4sv;
extern PFNGLVERTEXATTRIBI4UBVPROC __glewVertexAttribI4ubv;
extern PFNGLVERTEXATTRIBI4UIPROC __glewVertexAttribI4ui;
extern PFNGLVERTEXATTRIBI4UIVPROC __glewVertexAttribI4uiv;
extern PFNGLVERTEXATTRIBI4USVPROC __glewVertexAttribI4usv;
extern PFNGLVERTEXATTRIBIPOINTERPROC __glewVertexAttribIPointer;

extern PFNGLDRAWARRAYSINSTANCEDPROC __glewDrawArraysInstanced;
extern PFNGLDRAWELEMENTSINSTANCEDPROC __glewDrawElementsInstanced;
extern PFNGLPRIMITIVERESTARTINDEXPROC __glewPrimitiveRestartIndex;
extern PFNGLTEXBUFFERPROC __glewTexBuffer;

extern PFNGLFRAMEBUFFERTEXTUREPROC __glewFramebufferTexture;
extern PFNGLGETBUFFERPARAMETERI64VPROC __glewGetBufferParameteri64v;
extern PFNGLGETINTEGER64I_VPROC __glewGetInteger64i_v;

extern PFNGLVERTEXATTRIBDIVISORPROC __glewVertexAttribDivisor;

extern PFNGLBLENDEQUATIONSEPARATEIPROC __glewBlendEquationSeparatei;
extern PFNGLBLENDEQUATIONIPROC __glewBlendEquationi;
extern PFNGLBLENDFUNCSEPARATEIPROC __glewBlendFuncSeparatei;
extern PFNGLBLENDFUNCIPROC __glewBlendFunci;
extern PFNGLMINSAMPLESHADINGPROC __glewMinSampleShading;

extern PFNGLTBUFFERMASK3DFXPROC __glewTbufferMask3DFX;

extern PFNGLDEBUGMESSAGECALLBACKAMDPROC __glewDebugMessageCallbackAMD;
extern PFNGLDEBUGMESSAGEENABLEAMDPROC __glewDebugMessageEnableAMD;
extern PFNGLDEBUGMESSAGEINSERTAMDPROC __glewDebugMessageInsertAMD;
extern PFNGLGETDEBUGMESSAGELOGAMDPROC __glewGetDebugMessageLogAMD;

extern PFNGLBLENDEQUATIONINDEXEDAMDPROC __glewBlendEquationIndexedAMD;
extern PFNGLBLENDEQUATIONSEPARATEINDEXEDAMDPROC __glewBlendEquationSeparateIndexedAMD;
extern PFNGLBLENDFUNCINDEXEDAMDPROC __glewBlendFuncIndexedAMD;
extern PFNGLBLENDFUNCSEPARATEINDEXEDAMDPROC __glewBlendFuncSeparateIndexedAMD;

extern PFNGLMULTIDRAWARRAYSINDIRECTAMDPROC __glewMultiDrawArraysIndirectAMD;
extern PFNGLMULTIDRAWELEMENTSINDIRECTAMDPROC __glewMultiDrawElementsIndirectAMD;

extern PFNGLDELETENAMESAMDPROC __glewDeleteNamesAMD;
extern PFNGLGENNAMESAMDPROC __glewGenNamesAMD;
extern PFNGLISNAMEAMDPROC __glewIsNameAMD;

extern PFNGLBEGINPERFMONITORAMDPROC __glewBeginPerfMonitorAMD;
extern PFNGLDELETEPERFMONITORSAMDPROC __glewDeletePerfMonitorsAMD;
extern PFNGLENDPERFMONITORAMDPROC __glewEndPerfMonitorAMD;
extern PFNGLGENPERFMONITORSAMDPROC __glewGenPerfMonitorsAMD;
extern PFNGLGETPERFMONITORCOUNTERDATAAMDPROC __glewGetPerfMonitorCounterDataAMD;
extern PFNGLGETPERFMONITORCOUNTERINFOAMDPROC __glewGetPerfMonitorCounterInfoAMD;
extern PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC __glewGetPerfMonitorCounterStringAMD;
extern PFNGLGETPERFMONITORCOUNTERSAMDPROC __glewGetPerfMonitorCountersAMD;
extern PFNGLGETPERFMONITORGROUPSTRINGAMDPROC __glewGetPerfMonitorGroupStringAMD;
extern PFNGLGETPERFMONITORGROUPSAMDPROC __glewGetPerfMonitorGroupsAMD;
extern PFNGLSELECTPERFMONITORCOUNTERSAMDPROC __glewSelectPerfMonitorCountersAMD;

extern PFNGLSETMULTISAMPLEFVAMDPROC __glewSetMultisamplefvAMD;

extern PFNGLTESSELLATIONFACTORAMDPROC __glewTessellationFactorAMD;
extern PFNGLTESSELLATIONMODEAMDPROC __glewTessellationModeAMD;

extern PFNGLDRAWELEMENTARRAYAPPLEPROC __glewDrawElementArrayAPPLE;
extern PFNGLDRAWRANGEELEMENTARRAYAPPLEPROC __glewDrawRangeElementArrayAPPLE;
extern PFNGLELEMENTPOINTERAPPLEPROC __glewElementPointerAPPLE;
extern PFNGLMULTIDRAWELEMENTARRAYAPPLEPROC __glewMultiDrawElementArrayAPPLE;
extern PFNGLMULTIDRAWRANGEELEMENTARRAYAPPLEPROC __glewMultiDrawRangeElementArrayAPPLE;

extern PFNGLDELETEFENCESAPPLEPROC __glewDeleteFencesAPPLE;
extern PFNGLFINISHFENCEAPPLEPROC __glewFinishFenceAPPLE;
extern PFNGLFINISHOBJECTAPPLEPROC __glewFinishObjectAPPLE;
extern PFNGLGENFENCESAPPLEPROC __glewGenFencesAPPLE;
extern PFNGLISFENCEAPPLEPROC __glewIsFenceAPPLE;
extern PFNGLSETFENCEAPPLEPROC __glewSetFenceAPPLE;
extern PFNGLTESTFENCEAPPLEPROC __glewTestFenceAPPLE;
extern PFNGLTESTOBJECTAPPLEPROC __glewTestObjectAPPLE;

extern PFNGLBUFFERPARAMETERIAPPLEPROC __glewBufferParameteriAPPLE;
extern PFNGLFLUSHMAPPEDBUFFERRANGEAPPLEPROC __glewFlushMappedBufferRangeAPPLE;

extern PFNGLGETOBJECTPARAMETERIVAPPLEPROC __glewGetObjectParameterivAPPLE;
extern PFNGLOBJECTPURGEABLEAPPLEPROC __glewObjectPurgeableAPPLE;
extern PFNGLOBJECTUNPURGEABLEAPPLEPROC __glewObjectUnpurgeableAPPLE;

extern PFNGLGETTEXPARAMETERPOINTERVAPPLEPROC __glewGetTexParameterPointervAPPLE;
extern PFNGLTEXTURERANGEAPPLEPROC __glewTextureRangeAPPLE;

extern PFNGLBINDVERTEXARRAYAPPLEPROC __glewBindVertexArrayAPPLE;
extern PFNGLDELETEVERTEXARRAYSAPPLEPROC __glewDeleteVertexArraysAPPLE;
extern PFNGLGENVERTEXARRAYSAPPLEPROC __glewGenVertexArraysAPPLE;
extern PFNGLISVERTEXARRAYAPPLEPROC __glewIsVertexArrayAPPLE;

extern PFNGLFLUSHVERTEXARRAYRANGEAPPLEPROC __glewFlushVertexArrayRangeAPPLE;
extern PFNGLVERTEXARRAYPARAMETERIAPPLEPROC __glewVertexArrayParameteriAPPLE;
extern PFNGLVERTEXARRAYRANGEAPPLEPROC __glewVertexArrayRangeAPPLE;

extern PFNGLDISABLEVERTEXATTRIBAPPLEPROC __glewDisableVertexAttribAPPLE;
extern PFNGLENABLEVERTEXATTRIBAPPLEPROC __glewEnableVertexAttribAPPLE;
extern PFNGLISVERTEXATTRIBENABLEDAPPLEPROC __glewIsVertexAttribEnabledAPPLE;
extern PFNGLMAPVERTEXATTRIB1DAPPLEPROC __glewMapVertexAttrib1dAPPLE;
extern PFNGLMAPVERTEXATTRIB1FAPPLEPROC __glewMapVertexAttrib1fAPPLE;
extern PFNGLMAPVERTEXATTRIB2DAPPLEPROC __glewMapVertexAttrib2dAPPLE;
extern PFNGLMAPVERTEXATTRIB2FAPPLEPROC __glewMapVertexAttrib2fAPPLE;

extern PFNGLCLEARDEPTHFPROC __glewClearDepthf;
extern PFNGLDEPTHRANGEFPROC __glewDepthRangef;
extern PFNGLGETSHADERPRECISIONFORMATPROC __glewGetShaderPrecisionFormat;
extern PFNGLRELEASESHADERCOMPILERPROC __glewReleaseShaderCompiler;
extern PFNGLSHADERBINARYPROC __glewShaderBinary;

extern PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC __glewDrawArraysInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC __glewDrawElementsInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC __glewDrawElementsInstancedBaseVertexBaseInstance;

extern PFNGLBINDFRAGDATALOCATIONINDEXEDPROC __glewBindFragDataLocationIndexed;
extern PFNGLGETFRAGDATAINDEXPROC __glewGetFragDataIndex;

extern PFNGLCREATESYNCFROMCLEVENTARBPROC __glewCreateSyncFromCLeventARB;

extern PFNGLCLAMPCOLORARBPROC __glewClampColorARB;

extern PFNGLCOPYBUFFERSUBDATAPROC __glewCopyBufferSubData;

extern PFNGLDEBUGMESSAGECALLBACKARBPROC __glewDebugMessageCallbackARB;
extern PFNGLDEBUGMESSAGECONTROLARBPROC __glewDebugMessageControlARB;
extern PFNGLDEBUGMESSAGEINSERTARBPROC __glewDebugMessageInsertARB;
extern PFNGLGETDEBUGMESSAGELOGARBPROC __glewGetDebugMessageLogARB;

extern PFNGLDRAWBUFFERSARBPROC __glewDrawBuffersARB;

extern PFNGLBLENDEQUATIONSEPARATEIARBPROC __glewBlendEquationSeparateiARB;
extern PFNGLBLENDEQUATIONIARBPROC __glewBlendEquationiARB;
extern PFNGLBLENDFUNCSEPARATEIARBPROC __glewBlendFuncSeparateiARB;
extern PFNGLBLENDFUNCIARBPROC __glewBlendFunciARB;

extern PFNGLDRAWELEMENTSBASEVERTEXPROC __glewDrawElementsBaseVertex;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC __glewDrawElementsInstancedBaseVertex;
extern PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC __glewDrawRangeElementsBaseVertex;
extern PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC __glewMultiDrawElementsBaseVertex;

extern PFNGLDRAWARRAYSINDIRECTPROC __glewDrawArraysIndirect;
extern PFNGLDRAWELEMENTSINDIRECTPROC __glewDrawElementsIndirect;

extern PFNGLBINDFRAMEBUFFERPROC __glewBindFramebuffer;
extern PFNGLBINDRENDERBUFFERPROC __glewBindRenderbuffer;
extern PFNGLBLITFRAMEBUFFERPROC __glewBlitFramebuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC __glewCheckFramebufferStatus;
extern PFNGLDELETEFRAMEBUFFERSPROC __glewDeleteFramebuffers;
extern PFNGLDELETERENDERBUFFERSPROC __glewDeleteRenderbuffers;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC __glewFramebufferRenderbuffer;
extern PFNGLFRAMEBUFFERTEXTURE1DPROC __glewFramebufferTexture1D;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC __glewFramebufferTexture2D;
extern PFNGLFRAMEBUFFERTEXTURE3DPROC __glewFramebufferTexture3D;
extern PFNGLFRAMEBUFFERTEXTURELAYERPROC __glewFramebufferTextureLayer;
extern PFNGLGENFRAMEBUFFERSPROC __glewGenFramebuffers;
extern PFNGLGENRENDERBUFFERSPROC __glewGenRenderbuffers;
extern PFNGLGENERATEMIPMAPPROC __glewGenerateMipmap;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC __glewGetFramebufferAttachmentParameteriv;
extern PFNGLGETRENDERBUFFERPARAMETERIVPROC __glewGetRenderbufferParameteriv;
extern PFNGLISFRAMEBUFFERPROC __glewIsFramebuffer;
extern PFNGLISRENDERBUFFERPROC __glewIsRenderbuffer;
extern PFNGLRENDERBUFFERSTORAGEPROC __glewRenderbufferStorage;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC __glewRenderbufferStorageMultisample;

extern PFNGLFRAMEBUFFERTEXTUREARBPROC __glewFramebufferTextureARB;
extern PFNGLFRAMEBUFFERTEXTUREFACEARBPROC __glewFramebufferTextureFaceARB;
extern PFNGLFRAMEBUFFERTEXTURELAYERARBPROC __glewFramebufferTextureLayerARB;
extern PFNGLPROGRAMPARAMETERIARBPROC __glewProgramParameteriARB;

extern PFNGLGETPROGRAMBINARYPROC __glewGetProgramBinary;
extern PFNGLPROGRAMBINARYPROC __glewProgramBinary;
extern PFNGLPROGRAMPARAMETERIPROC __glewProgramParameteri;

extern PFNGLGETUNIFORMDVPROC __glewGetUniformdv;
extern PFNGLPROGRAMUNIFORM1DEXTPROC __glewProgramUniform1dEXT;
extern PFNGLPROGRAMUNIFORM1DVEXTPROC __glewProgramUniform1dvEXT;
extern PFNGLPROGRAMUNIFORM2DEXTPROC __glewProgramUniform2dEXT;
extern PFNGLPROGRAMUNIFORM2DVEXTPROC __glewProgramUniform2dvEXT;
extern PFNGLPROGRAMUNIFORM3DEXTPROC __glewProgramUniform3dEXT;
extern PFNGLPROGRAMUNIFORM3DVEXTPROC __glewProgramUniform3dvEXT;
extern PFNGLPROGRAMUNIFORM4DEXTPROC __glewProgramUniform4dEXT;
extern PFNGLPROGRAMUNIFORM4DVEXTPROC __glewProgramUniform4dvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX2DVEXTPROC __glewProgramUniformMatrix2dvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX2X3DVEXTPROC __glewProgramUniformMatrix2x3dvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX2X4DVEXTPROC __glewProgramUniformMatrix2x4dvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX3DVEXTPROC __glewProgramUniformMatrix3dvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX3X2DVEXTPROC __glewProgramUniformMatrix3x2dvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX3X4DVEXTPROC __glewProgramUniformMatrix3x4dvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX4DVEXTPROC __glewProgramUniformMatrix4dvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX4X2DVEXTPROC __glewProgramUniformMatrix4x2dvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX4X3DVEXTPROC __glewProgramUniformMatrix4x3dvEXT;
extern PFNGLUNIFORM1DPROC __glewUniform1d;
extern PFNGLUNIFORM1DVPROC __glewUniform1dv;
extern PFNGLUNIFORM2DPROC __glewUniform2d;
extern PFNGLUNIFORM2DVPROC __glewUniform2dv;
extern PFNGLUNIFORM3DPROC __glewUniform3d;
extern PFNGLUNIFORM3DVPROC __glewUniform3dv;
extern PFNGLUNIFORM4DPROC __glewUniform4d;
extern PFNGLUNIFORM4DVPROC __glewUniform4dv;
extern PFNGLUNIFORMMATRIX2DVPROC __glewUniformMatrix2dv;
extern PFNGLUNIFORMMATRIX2X3DVPROC __glewUniformMatrix2x3dv;
extern PFNGLUNIFORMMATRIX2X4DVPROC __glewUniformMatrix2x4dv;
extern PFNGLUNIFORMMATRIX3DVPROC __glewUniformMatrix3dv;
extern PFNGLUNIFORMMATRIX3X2DVPROC __glewUniformMatrix3x2dv;
extern PFNGLUNIFORMMATRIX3X4DVPROC __glewUniformMatrix3x4dv;
extern PFNGLUNIFORMMATRIX4DVPROC __glewUniformMatrix4dv;
extern PFNGLUNIFORMMATRIX4X2DVPROC __glewUniformMatrix4x2dv;
extern PFNGLUNIFORMMATRIX4X3DVPROC __glewUniformMatrix4x3dv;

extern PFNGLCOLORSUBTABLEPROC __glewColorSubTable;
extern PFNGLCOLORTABLEPROC __glewColorTable;
extern PFNGLCOLORTABLEPARAMETERFVPROC __glewColorTableParameterfv;
extern PFNGLCOLORTABLEPARAMETERIVPROC __glewColorTableParameteriv;
extern PFNGLCONVOLUTIONFILTER1DPROC __glewConvolutionFilter1D;
extern PFNGLCONVOLUTIONFILTER2DPROC __glewConvolutionFilter2D;
extern PFNGLCONVOLUTIONPARAMETERFPROC __glewConvolutionParameterf;
extern PFNGLCONVOLUTIONPARAMETERFVPROC __glewConvolutionParameterfv;
extern PFNGLCONVOLUTIONPARAMETERIPROC __glewConvolutionParameteri;
extern PFNGLCONVOLUTIONPARAMETERIVPROC __glewConvolutionParameteriv;
extern PFNGLCOPYCOLORSUBTABLEPROC __glewCopyColorSubTable;
extern PFNGLCOPYCOLORTABLEPROC __glewCopyColorTable;
extern PFNGLCOPYCONVOLUTIONFILTER1DPROC __glewCopyConvolutionFilter1D;
extern PFNGLCOPYCONVOLUTIONFILTER2DPROC __glewCopyConvolutionFilter2D;
extern PFNGLGETCOLORTABLEPROC __glewGetColorTable;
extern PFNGLGETCOLORTABLEPARAMETERFVPROC __glewGetColorTableParameterfv;
extern PFNGLGETCOLORTABLEPARAMETERIVPROC __glewGetColorTableParameteriv;
extern PFNGLGETCONVOLUTIONFILTERPROC __glewGetConvolutionFilter;
extern PFNGLGETCONVOLUTIONPARAMETERFVPROC __glewGetConvolutionParameterfv;
extern PFNGLGETCONVOLUTIONPARAMETERIVPROC __glewGetConvolutionParameteriv;
extern PFNGLGETHISTOGRAMPROC __glewGetHistogram;
extern PFNGLGETHISTOGRAMPARAMETERFVPROC __glewGetHistogramParameterfv;
extern PFNGLGETHISTOGRAMPARAMETERIVPROC __glewGetHistogramParameteriv;
extern PFNGLGETMINMAXPROC __glewGetMinmax;
extern PFNGLGETMINMAXPARAMETERFVPROC __glewGetMinmaxParameterfv;
extern PFNGLGETMINMAXPARAMETERIVPROC __glewGetMinmaxParameteriv;
extern PFNGLGETSEPARABLEFILTERPROC __glewGetSeparableFilter;
extern PFNGLHISTOGRAMPROC __glewHistogram;
extern PFNGLMINMAXPROC __glewMinmax;
extern PFNGLRESETHISTOGRAMPROC __glewResetHistogram;
extern PFNGLRESETMINMAXPROC __glewResetMinmax;
extern PFNGLSEPARABLEFILTER2DPROC __glewSeparableFilter2D;

extern PFNGLDRAWARRAYSINSTANCEDARBPROC __glewDrawArraysInstancedARB;
extern PFNGLDRAWELEMENTSINSTANCEDARBPROC __glewDrawElementsInstancedARB;
extern PFNGLVERTEXATTRIBDIVISORARBPROC __glewVertexAttribDivisorARB;

extern PFNGLGETINTERNALFORMATIVPROC __glewGetInternalformativ;

extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC __glewFlushMappedBufferRange;
extern PFNGLMAPBUFFERRANGEPROC __glewMapBufferRange;

extern PFNGLCURRENTPALETTEMATRIXARBPROC __glewCurrentPaletteMatrixARB;
extern PFNGLMATRIXINDEXPOINTERARBPROC __glewMatrixIndexPointerARB;
extern PFNGLMATRIXINDEXUBVARBPROC __glewMatrixIndexubvARB;
extern PFNGLMATRIXINDEXUIVARBPROC __glewMatrixIndexuivARB;
extern PFNGLMATRIXINDEXUSVARBPROC __glewMatrixIndexusvARB;

extern PFNGLSAMPLECOVERAGEARBPROC __glewSampleCoverageARB;

extern PFNGLACTIVETEXTUREARBPROC __glewActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC __glewClientActiveTextureARB;
extern PFNGLMULTITEXCOORD1DARBPROC __glewMultiTexCoord1dARB;
extern PFNGLMULTITEXCOORD1DVARBPROC __glewMultiTexCoord1dvARB;
extern PFNGLMULTITEXCOORD1FARBPROC __glewMultiTexCoord1fARB;
extern PFNGLMULTITEXCOORD1FVARBPROC __glewMultiTexCoord1fvARB;
extern PFNGLMULTITEXCOORD1IARBPROC __glewMultiTexCoord1iARB;
extern PFNGLMULTITEXCOORD1IVARBPROC __glewMultiTexCoord1ivARB;
extern PFNGLMULTITEXCOORD1SARBPROC __glewMultiTexCoord1sARB;
extern PFNGLMULTITEXCOORD1SVARBPROC __glewMultiTexCoord1svARB;
extern PFNGLMULTITEXCOORD2DARBPROC __glewMultiTexCoord2dARB;
extern PFNGLMULTITEXCOORD2DVARBPROC __glewMultiTexCoord2dvARB;
extern PFNGLMULTITEXCOORD2FARBPROC __glewMultiTexCoord2fARB;
extern PFNGLMULTITEXCOORD2FVARBPROC __glewMultiTexCoord2fvARB;
extern PFNGLMULTITEXCOORD2IARBPROC __glewMultiTexCoord2iARB;
extern PFNGLMULTITEXCOORD2IVARBPROC __glewMultiTexCoord2ivARB;
extern PFNGLMULTITEXCOORD2SARBPROC __glewMultiTexCoord2sARB;
extern PFNGLMULTITEXCOORD2SVARBPROC __glewMultiTexCoord2svARB;
extern PFNGLMULTITEXCOORD3DARBPROC __glewMultiTexCoord3dARB;
extern PFNGLMULTITEXCOORD3DVARBPROC __glewMultiTexCoord3dvARB;
extern PFNGLMULTITEXCOORD3FARBPROC __glewMultiTexCoord3fARB;
extern PFNGLMULTITEXCOORD3FVARBPROC __glewMultiTexCoord3fvARB;
extern PFNGLMULTITEXCOORD3IARBPROC __glewMultiTexCoord3iARB;
extern PFNGLMULTITEXCOORD3IVARBPROC __glewMultiTexCoord3ivARB;
extern PFNGLMULTITEXCOORD3SARBPROC __glewMultiTexCoord3sARB;
extern PFNGLMULTITEXCOORD3SVARBPROC __glewMultiTexCoord3svARB;
extern PFNGLMULTITEXCOORD4DARBPROC __glewMultiTexCoord4dARB;
extern PFNGLMULTITEXCOORD4DVARBPROC __glewMultiTexCoord4dvARB;
extern PFNGLMULTITEXCOORD4FARBPROC __glewMultiTexCoord4fARB;
extern PFNGLMULTITEXCOORD4FVARBPROC __glewMultiTexCoord4fvARB;
extern PFNGLMULTITEXCOORD4IARBPROC __glewMultiTexCoord4iARB;
extern PFNGLMULTITEXCOORD4IVARBPROC __glewMultiTexCoord4ivARB;
extern PFNGLMULTITEXCOORD4SARBPROC __glewMultiTexCoord4sARB;
extern PFNGLMULTITEXCOORD4SVARBPROC __glewMultiTexCoord4svARB;

extern PFNGLBEGINQUERYARBPROC __glewBeginQueryARB;
extern PFNGLDELETEQUERIESARBPROC __glewDeleteQueriesARB;
extern PFNGLENDQUERYARBPROC __glewEndQueryARB;
extern PFNGLGENQUERIESARBPROC __glewGenQueriesARB;
extern PFNGLGETQUERYOBJECTIVARBPROC __glewGetQueryObjectivARB;
extern PFNGLGETQUERYOBJECTUIVARBPROC __glewGetQueryObjectuivARB;
extern PFNGLGETQUERYIVARBPROC __glewGetQueryivARB;
extern PFNGLISQUERYARBPROC __glewIsQueryARB;

extern PFNGLPOINTPARAMETERFARBPROC __glewPointParameterfARB;
extern PFNGLPOINTPARAMETERFVARBPROC __glewPointParameterfvARB;

extern PFNGLPROVOKINGVERTEXPROC __glewProvokingVertex;

extern PFNGLGETGRAPHICSRESETSTATUSARBPROC __glewGetGraphicsResetStatusARB;
extern PFNGLGETNCOLORTABLEARBPROC __glewGetnColorTableARB;
extern PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC __glewGetnCompressedTexImageARB;
extern PFNGLGETNCONVOLUTIONFILTERARBPROC __glewGetnConvolutionFilterARB;
extern PFNGLGETNHISTOGRAMARBPROC __glewGetnHistogramARB;
extern PFNGLGETNMAPDVARBPROC __glewGetnMapdvARB;
extern PFNGLGETNMAPFVARBPROC __glewGetnMapfvARB;
extern PFNGLGETNMAPIVARBPROC __glewGetnMapivARB;
extern PFNGLGETNMINMAXARBPROC __glewGetnMinmaxARB;
extern PFNGLGETNPIXELMAPFVARBPROC __glewGetnPixelMapfvARB;
extern PFNGLGETNPIXELMAPUIVARBPROC __glewGetnPixelMapuivARB;
extern PFNGLGETNPIXELMAPUSVARBPROC __glewGetnPixelMapusvARB;
extern PFNGLGETNPOLYGONSTIPPLEARBPROC __glewGetnPolygonStippleARB;
extern PFNGLGETNSEPARABLEFILTERARBPROC __glewGetnSeparableFilterARB;
extern PFNGLGETNTEXIMAGEARBPROC __glewGetnTexImageARB;
extern PFNGLGETNUNIFORMDVARBPROC __glewGetnUniformdvARB;
extern PFNGLGETNUNIFORMFVARBPROC __glewGetnUniformfvARB;
extern PFNGLGETNUNIFORMIVARBPROC __glewGetnUniformivARB;
extern PFNGLGETNUNIFORMUIVARBPROC __glewGetnUniformuivARB;
extern PFNGLREADNPIXELSARBPROC __glewReadnPixelsARB;

extern PFNGLMINSAMPLESHADINGARBPROC __glewMinSampleShadingARB;

extern PFNGLBINDSAMPLERPROC __glewBindSampler;
extern PFNGLDELETESAMPLERSPROC __glewDeleteSamplers;
extern PFNGLGENSAMPLERSPROC __glewGenSamplers;
extern PFNGLGETSAMPLERPARAMETERIIVPROC __glewGetSamplerParameterIiv;
extern PFNGLGETSAMPLERPARAMETERIUIVPROC __glewGetSamplerParameterIuiv;
extern PFNGLGETSAMPLERPARAMETERFVPROC __glewGetSamplerParameterfv;
extern PFNGLGETSAMPLERPARAMETERIVPROC __glewGetSamplerParameteriv;
extern PFNGLISSAMPLERPROC __glewIsSampler;
extern PFNGLSAMPLERPARAMETERIIVPROC __glewSamplerParameterIiv;
extern PFNGLSAMPLERPARAMETERIUIVPROC __glewSamplerParameterIuiv;
extern PFNGLSAMPLERPARAMETERFPROC __glewSamplerParameterf;
extern PFNGLSAMPLERPARAMETERFVPROC __glewSamplerParameterfv;
extern PFNGLSAMPLERPARAMETERIPROC __glewSamplerParameteri;
extern PFNGLSAMPLERPARAMETERIVPROC __glewSamplerParameteriv;

extern PFNGLACTIVESHADERPROGRAMPROC __glewActiveShaderProgram;
extern PFNGLBINDPROGRAMPIPELINEPROC __glewBindProgramPipeline;
extern PFNGLCREATESHADERPROGRAMVPROC __glewCreateShaderProgramv;
extern PFNGLDELETEPROGRAMPIPELINESPROC __glewDeleteProgramPipelines;
extern PFNGLGENPROGRAMPIPELINESPROC __glewGenProgramPipelines;
extern PFNGLGETPROGRAMPIPELINEINFOLOGPROC __glewGetProgramPipelineInfoLog;
extern PFNGLGETPROGRAMPIPELINEIVPROC __glewGetProgramPipelineiv;
extern PFNGLISPROGRAMPIPELINEPROC __glewIsProgramPipeline;
extern PFNGLPROGRAMUNIFORM1DPROC __glewProgramUniform1d;
extern PFNGLPROGRAMUNIFORM1DVPROC __glewProgramUniform1dv;
extern PFNGLPROGRAMUNIFORM1FPROC __glewProgramUniform1f;
extern PFNGLPROGRAMUNIFORM1FVPROC __glewProgramUniform1fv;
extern PFNGLPROGRAMUNIFORM1IPROC __glewProgramUniform1i;
extern PFNGLPROGRAMUNIFORM1IVPROC __glewProgramUniform1iv;
extern PFNGLPROGRAMUNIFORM1UIPROC __glewProgramUniform1ui;
extern PFNGLPROGRAMUNIFORM1UIVPROC __glewProgramUniform1uiv;
extern PFNGLPROGRAMUNIFORM2DPROC __glewProgramUniform2d;
extern PFNGLPROGRAMUNIFORM2DVPROC __glewProgramUniform2dv;
extern PFNGLPROGRAMUNIFORM2FPROC __glewProgramUniform2f;
extern PFNGLPROGRAMUNIFORM2FVPROC __glewProgramUniform2fv;
extern PFNGLPROGRAMUNIFORM2IPROC __glewProgramUniform2i;
extern PFNGLPROGRAMUNIFORM2IVPROC __glewProgramUniform2iv;
extern PFNGLPROGRAMUNIFORM2UIPROC __glewProgramUniform2ui;
extern PFNGLPROGRAMUNIFORM2UIVPROC __glewProgramUniform2uiv;
extern PFNGLPROGRAMUNIFORM3DPROC __glewProgramUniform3d;
extern PFNGLPROGRAMUNIFORM3DVPROC __glewProgramUniform3dv;
extern PFNGLPROGRAMUNIFORM3FPROC __glewProgramUniform3f;
extern PFNGLPROGRAMUNIFORM3FVPROC __glewProgramUniform3fv;
extern PFNGLPROGRAMUNIFORM3IPROC __glewProgramUniform3i;
extern PFNGLPROGRAMUNIFORM3IVPROC __glewProgramUniform3iv;
extern PFNGLPROGRAMUNIFORM3UIPROC __glewProgramUniform3ui;
extern PFNGLPROGRAMUNIFORM3UIVPROC __glewProgramUniform3uiv;
extern PFNGLPROGRAMUNIFORM4DPROC __glewProgramUniform4d;
extern PFNGLPROGRAMUNIFORM4DVPROC __glewProgramUniform4dv;
extern PFNGLPROGRAMUNIFORM4FPROC __glewProgramUniform4f;
extern PFNGLPROGRAMUNIFORM4FVPROC __glewProgramUniform4fv;
extern PFNGLPROGRAMUNIFORM4IPROC __glewProgramUniform4i;
extern PFNGLPROGRAMUNIFORM4IVPROC __glewProgramUniform4iv;
extern PFNGLPROGRAMUNIFORM4UIPROC __glewProgramUniform4ui;
extern PFNGLPROGRAMUNIFORM4UIVPROC __glewProgramUniform4uiv;
extern PFNGLPROGRAMUNIFORMMATRIX2DVPROC __glewProgramUniformMatrix2dv;
extern PFNGLPROGRAMUNIFORMMATRIX2FVPROC __glewProgramUniformMatrix2fv;
extern PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC __glewProgramUniformMatrix2x3dv;
extern PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC __glewProgramUniformMatrix2x3fv;
extern PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC __glewProgramUniformMatrix2x4dv;
extern PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC __glewProgramUniformMatrix2x4fv;
extern PFNGLPROGRAMUNIFORMMATRIX3DVPROC __glewProgramUniformMatrix3dv;
extern PFNGLPROGRAMUNIFORMMATRIX3FVPROC __glewProgramUniformMatrix3fv;
extern PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC __glewProgramUniformMatrix3x2dv;
extern PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC __glewProgramUniformMatrix3x2fv;
extern PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC __glewProgramUniformMatrix3x4dv;
extern PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC __glewProgramUniformMatrix3x4fv;
extern PFNGLPROGRAMUNIFORMMATRIX4DVPROC __glewProgramUniformMatrix4dv;
extern PFNGLPROGRAMUNIFORMMATRIX4FVPROC __glewProgramUniformMatrix4fv;
extern PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC __glewProgramUniformMatrix4x2dv;
extern PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC __glewProgramUniformMatrix4x2fv;
extern PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC __glewProgramUniformMatrix4x3dv;
extern PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC __glewProgramUniformMatrix4x3fv;
extern PFNGLUSEPROGRAMSTAGESPROC __glewUseProgramStages;
extern PFNGLVALIDATEPROGRAMPIPELINEPROC __glewValidateProgramPipeline;

extern PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC __glewGetActiveAtomicCounterBufferiv;

extern PFNGLBINDIMAGETEXTUREPROC __glewBindImageTexture;
extern PFNGLMEMORYBARRIERPROC __glewMemoryBarrier;

extern PFNGLATTACHOBJECTARBPROC __glewAttachObjectARB;
extern PFNGLCOMPILESHADERARBPROC __glewCompileShaderARB;
extern PFNGLCREATEPROGRAMOBJECTARBPROC __glewCreateProgramObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC __glewCreateShaderObjectARB;
extern PFNGLDELETEOBJECTARBPROC __glewDeleteObjectARB;
extern PFNGLDETACHOBJECTARBPROC __glewDetachObjectARB;
extern PFNGLGETACTIVEUNIFORMARBPROC __glewGetActiveUniformARB;
extern PFNGLGETATTACHEDOBJECTSARBPROC __glewGetAttachedObjectsARB;
extern PFNGLGETHANDLEARBPROC __glewGetHandleARB;
extern PFNGLGETINFOLOGARBPROC __glewGetInfoLogARB;
extern PFNGLGETOBJECTPARAMETERFVARBPROC __glewGetObjectParameterfvARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC __glewGetObjectParameterivARB;
extern PFNGLGETSHADERSOURCEARBPROC __glewGetShaderSourceARB;
extern PFNGLGETUNIFORMLOCATIONARBPROC __glewGetUniformLocationARB;
extern PFNGLGETUNIFORMFVARBPROC __glewGetUniformfvARB;
extern PFNGLGETUNIFORMIVARBPROC __glewGetUniformivARB;
extern PFNGLLINKPROGRAMARBPROC __glewLinkProgramARB;
extern PFNGLSHADERSOURCEARBPROC __glewShaderSourceARB;
extern PFNGLUNIFORM1FARBPROC __glewUniform1fARB;
extern PFNGLUNIFORM1FVARBPROC __glewUniform1fvARB;
extern PFNGLUNIFORM1IARBPROC __glewUniform1iARB;
extern PFNGLUNIFORM1IVARBPROC __glewUniform1ivARB;
extern PFNGLUNIFORM2FARBPROC __glewUniform2fARB;
extern PFNGLUNIFORM2FVARBPROC __glewUniform2fvARB;
extern PFNGLUNIFORM2IARBPROC __glewUniform2iARB;
extern PFNGLUNIFORM2IVARBPROC __glewUniform2ivARB;
extern PFNGLUNIFORM3FARBPROC __glewUniform3fARB;
extern PFNGLUNIFORM3FVARBPROC __glewUniform3fvARB;
extern PFNGLUNIFORM3IARBPROC __glewUniform3iARB;
extern PFNGLUNIFORM3IVARBPROC __glewUniform3ivARB;
extern PFNGLUNIFORM4FARBPROC __glewUniform4fARB;
extern PFNGLUNIFORM4FVARBPROC __glewUniform4fvARB;
extern PFNGLUNIFORM4IARBPROC __glewUniform4iARB;
extern PFNGLUNIFORM4IVARBPROC __glewUniform4ivARB;
extern PFNGLUNIFORMMATRIX2FVARBPROC __glewUniformMatrix2fvARB;
extern PFNGLUNIFORMMATRIX3FVARBPROC __glewUniformMatrix3fvARB;
extern PFNGLUNIFORMMATRIX4FVARBPROC __glewUniformMatrix4fvARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC __glewUseProgramObjectARB;
extern PFNGLVALIDATEPROGRAMARBPROC __glewValidateProgramARB;

extern PFNGLGETACTIVESUBROUTINENAMEPROC __glewGetActiveSubroutineName;
extern PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC __glewGetActiveSubroutineUniformName;
extern PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC __glewGetActiveSubroutineUniformiv;
extern PFNGLGETPROGRAMSTAGEIVPROC __glewGetProgramStageiv;
extern PFNGLGETSUBROUTINEINDEXPROC __glewGetSubroutineIndex;
extern PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC __glewGetSubroutineUniformLocation;
extern PFNGLGETUNIFORMSUBROUTINEUIVPROC __glewGetUniformSubroutineuiv;
extern PFNGLUNIFORMSUBROUTINESUIVPROC __glewUniformSubroutinesuiv;

extern PFNGLCOMPILESHADERINCLUDEARBPROC __glewCompileShaderIncludeARB;
extern PFNGLDELETENAMEDSTRINGARBPROC __glewDeleteNamedStringARB;
extern PFNGLGETNAMEDSTRINGARBPROC __glewGetNamedStringARB;
extern PFNGLGETNAMEDSTRINGIVARBPROC __glewGetNamedStringivARB;
extern PFNGLISNAMEDSTRINGARBPROC __glewIsNamedStringARB;
extern PFNGLNAMEDSTRINGARBPROC __glewNamedStringARB;

extern PFNGLCLIENTWAITSYNCPROC __glewClientWaitSync;
extern PFNGLDELETESYNCPROC __glewDeleteSync;
extern PFNGLFENCESYNCPROC __glewFenceSync;
extern PFNGLGETINTEGER64VPROC __glewGetInteger64v;
extern PFNGLGETSYNCIVPROC __glewGetSynciv;
extern PFNGLISSYNCPROC __glewIsSync;
extern PFNGLWAITSYNCPROC __glewWaitSync;

extern PFNGLPATCHPARAMETERFVPROC __glewPatchParameterfv;
extern PFNGLPATCHPARAMETERIPROC __glewPatchParameteri;

extern PFNGLTEXBUFFERARBPROC __glewTexBufferARB;

extern PFNGLCOMPRESSEDTEXIMAGE1DARBPROC __glewCompressedTexImage1DARB;
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC __glewCompressedTexImage2DARB;
extern PFNGLCOMPRESSEDTEXIMAGE3DARBPROC __glewCompressedTexImage3DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC __glewCompressedTexSubImage1DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC __glewCompressedTexSubImage2DARB;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC __glewCompressedTexSubImage3DARB;
extern PFNGLGETCOMPRESSEDTEXIMAGEARBPROC __glewGetCompressedTexImageARB;

extern PFNGLGETMULTISAMPLEFVPROC __glewGetMultisamplefv;
extern PFNGLSAMPLEMASKIPROC __glewSampleMaski;
extern PFNGLTEXIMAGE2DMULTISAMPLEPROC __glewTexImage2DMultisample;
extern PFNGLTEXIMAGE3DMULTISAMPLEPROC __glewTexImage3DMultisample;

extern PFNGLTEXSTORAGE1DPROC __glewTexStorage1D;
extern PFNGLTEXSTORAGE2DPROC __glewTexStorage2D;
extern PFNGLTEXSTORAGE3DPROC __glewTexStorage3D;
extern PFNGLTEXTURESTORAGE1DEXTPROC __glewTextureStorage1DEXT;
extern PFNGLTEXTURESTORAGE2DEXTPROC __glewTextureStorage2DEXT;
extern PFNGLTEXTURESTORAGE3DEXTPROC __glewTextureStorage3DEXT;

extern PFNGLGETQUERYOBJECTI64VPROC __glewGetQueryObjecti64v;
extern PFNGLGETQUERYOBJECTUI64VPROC __glewGetQueryObjectui64v;
extern PFNGLQUERYCOUNTERPROC __glewQueryCounter;

extern PFNGLBINDTRANSFORMFEEDBACKPROC __glewBindTransformFeedback;
extern PFNGLDELETETRANSFORMFEEDBACKSPROC __glewDeleteTransformFeedbacks;
extern PFNGLDRAWTRANSFORMFEEDBACKPROC __glewDrawTransformFeedback;
extern PFNGLGENTRANSFORMFEEDBACKSPROC __glewGenTransformFeedbacks;
extern PFNGLISTRANSFORMFEEDBACKPROC __glewIsTransformFeedback;
extern PFNGLPAUSETRANSFORMFEEDBACKPROC __glewPauseTransformFeedback;
extern PFNGLRESUMETRANSFORMFEEDBACKPROC __glewResumeTransformFeedback;

extern PFNGLBEGINQUERYINDEXEDPROC __glewBeginQueryIndexed;
extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC __glewDrawTransformFeedbackStream;
extern PFNGLENDQUERYINDEXEDPROC __glewEndQueryIndexed;
extern PFNGLGETQUERYINDEXEDIVPROC __glewGetQueryIndexediv;

extern PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC __glewDrawTransformFeedbackInstanced;
extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC __glewDrawTransformFeedbackStreamInstanced;

extern PFNGLLOADTRANSPOSEMATRIXDARBPROC __glewLoadTransposeMatrixdARB;
extern PFNGLLOADTRANSPOSEMATRIXFARBPROC __glewLoadTransposeMatrixfARB;
extern PFNGLMULTTRANSPOSEMATRIXDARBPROC __glewMultTransposeMatrixdARB;
extern PFNGLMULTTRANSPOSEMATRIXFARBPROC __glewMultTransposeMatrixfARB;

extern PFNGLBINDBUFFERBASEPROC __glewBindBufferBase;
extern PFNGLBINDBUFFERRANGEPROC __glewBindBufferRange;
extern PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC __glewGetActiveUniformBlockName;
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC __glewGetActiveUniformBlockiv;
extern PFNGLGETACTIVEUNIFORMNAMEPROC __glewGetActiveUniformName;
extern PFNGLGETACTIVEUNIFORMSIVPROC __glewGetActiveUniformsiv;
extern PFNGLGETINTEGERI_VPROC __glewGetIntegeri_v;
extern PFNGLGETUNIFORMBLOCKINDEXPROC __glewGetUniformBlockIndex;
extern PFNGLGETUNIFORMINDICESPROC __glewGetUniformIndices;
extern PFNGLUNIFORMBLOCKBINDINGPROC __glewUniformBlockBinding;

extern PFNGLBINDVERTEXARRAYPROC __glewBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC __glewDeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSPROC __glewGenVertexArrays;
extern PFNGLISVERTEXARRAYPROC __glewIsVertexArray;

extern PFNGLGETVERTEXATTRIBLDVPROC __glewGetVertexAttribLdv;
extern PFNGLVERTEXATTRIBL1DPROC __glewVertexAttribL1d;
extern PFNGLVERTEXATTRIBL1DVPROC __glewVertexAttribL1dv;
extern PFNGLVERTEXATTRIBL2DPROC __glewVertexAttribL2d;
extern PFNGLVERTEXATTRIBL2DVPROC __glewVertexAttribL2dv;
extern PFNGLVERTEXATTRIBL3DPROC __glewVertexAttribL3d;
extern PFNGLVERTEXATTRIBL3DVPROC __glewVertexAttribL3dv;
extern PFNGLVERTEXATTRIBL4DPROC __glewVertexAttribL4d;
extern PFNGLVERTEXATTRIBL4DVPROC __glewVertexAttribL4dv;
extern PFNGLVERTEXATTRIBLPOINTERPROC __glewVertexAttribLPointer;

extern PFNGLVERTEXBLENDARBPROC __glewVertexBlendARB;
extern PFNGLWEIGHTPOINTERARBPROC __glewWeightPointerARB;
extern PFNGLWEIGHTBVARBPROC __glewWeightbvARB;
extern PFNGLWEIGHTDVARBPROC __glewWeightdvARB;
extern PFNGLWEIGHTFVARBPROC __glewWeightfvARB;
extern PFNGLWEIGHTIVARBPROC __glewWeightivARB;
extern PFNGLWEIGHTSVARBPROC __glewWeightsvARB;
extern PFNGLWEIGHTUBVARBPROC __glewWeightubvARB;
extern PFNGLWEIGHTUIVARBPROC __glewWeightuivARB;
extern PFNGLWEIGHTUSVARBPROC __glewWeightusvARB;

extern PFNGLBINDBUFFERARBPROC __glewBindBufferARB;
extern PFNGLBUFFERDATAARBPROC __glewBufferDataARB;
extern PFNGLBUFFERSUBDATAARBPROC __glewBufferSubDataARB;
extern PFNGLDELETEBUFFERSARBPROC __glewDeleteBuffersARB;
extern PFNGLGENBUFFERSARBPROC __glewGenBuffersARB;
extern PFNGLGETBUFFERPARAMETERIVARBPROC __glewGetBufferParameterivARB;
extern PFNGLGETBUFFERPOINTERVARBPROC __glewGetBufferPointervARB;
extern PFNGLGETBUFFERSUBDATAARBPROC __glewGetBufferSubDataARB;
extern PFNGLISBUFFERARBPROC __glewIsBufferARB;
extern PFNGLMAPBUFFERARBPROC __glewMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC __glewUnmapBufferARB;

extern PFNGLBINDPROGRAMARBPROC __glewBindProgramARB;
extern PFNGLDELETEPROGRAMSARBPROC __glewDeleteProgramsARB;
extern PFNGLDISABLEVERTEXATTRIBARRAYARBPROC __glewDisableVertexAttribArrayARB;
extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC __glewEnableVertexAttribArrayARB;
extern PFNGLGENPROGRAMSARBPROC __glewGenProgramsARB;
extern PFNGLGETPROGRAMENVPARAMETERDVARBPROC __glewGetProgramEnvParameterdvARB;
extern PFNGLGETPROGRAMENVPARAMETERFVARBPROC __glewGetProgramEnvParameterfvARB;
extern PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC __glewGetProgramLocalParameterdvARB;
extern PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC __glewGetProgramLocalParameterfvARB;
extern PFNGLGETPROGRAMSTRINGARBPROC __glewGetProgramStringARB;
extern PFNGLGETPROGRAMIVARBPROC __glewGetProgramivARB;
extern PFNGLGETVERTEXATTRIBPOINTERVARBPROC __glewGetVertexAttribPointervARB;
extern PFNGLGETVERTEXATTRIBDVARBPROC __glewGetVertexAttribdvARB;
extern PFNGLGETVERTEXATTRIBFVARBPROC __glewGetVertexAttribfvARB;
extern PFNGLGETVERTEXATTRIBIVARBPROC __glewGetVertexAttribivARB;
extern PFNGLISPROGRAMARBPROC __glewIsProgramARB;
extern PFNGLPROGRAMENVPARAMETER4DARBPROC __glewProgramEnvParameter4dARB;
extern PFNGLPROGRAMENVPARAMETER4DVARBPROC __glewProgramEnvParameter4dvARB;
extern PFNGLPROGRAMENVPARAMETER4FARBPROC __glewProgramEnvParameter4fARB;
extern PFNGLPROGRAMENVPARAMETER4FVARBPROC __glewProgramEnvParameter4fvARB;
extern PFNGLPROGRAMLOCALPARAMETER4DARBPROC __glewProgramLocalParameter4dARB;
extern PFNGLPROGRAMLOCALPARAMETER4DVARBPROC __glewProgramLocalParameter4dvARB;
extern PFNGLPROGRAMLOCALPARAMETER4FARBPROC __glewProgramLocalParameter4fARB;
extern PFNGLPROGRAMLOCALPARAMETER4FVARBPROC __glewProgramLocalParameter4fvARB;
extern PFNGLPROGRAMSTRINGARBPROC __glewProgramStringARB;
extern PFNGLVERTEXATTRIB1DARBPROC __glewVertexAttrib1dARB;
extern PFNGLVERTEXATTRIB1DVARBPROC __glewVertexAttrib1dvARB;
extern PFNGLVERTEXATTRIB1FARBPROC __glewVertexAttrib1fARB;
extern PFNGLVERTEXATTRIB1FVARBPROC __glewVertexAttrib1fvARB;
extern PFNGLVERTEXATTRIB1SARBPROC __glewVertexAttrib1sARB;
extern PFNGLVERTEXATTRIB1SVARBPROC __glewVertexAttrib1svARB;
extern PFNGLVERTEXATTRIB2DARBPROC __glewVertexAttrib2dARB;
extern PFNGLVERTEXATTRIB2DVARBPROC __glewVertexAttrib2dvARB;
extern PFNGLVERTEXATTRIB2FARBPROC __glewVertexAttrib2fARB;
extern PFNGLVERTEXATTRIB2FVARBPROC __glewVertexAttrib2fvARB;
extern PFNGLVERTEXATTRIB2SARBPROC __glewVertexAttrib2sARB;
extern PFNGLVERTEXATTRIB2SVARBPROC __glewVertexAttrib2svARB;
extern PFNGLVERTEXATTRIB3DARBPROC __glewVertexAttrib3dARB;
extern PFNGLVERTEXATTRIB3DVARBPROC __glewVertexAttrib3dvARB;
extern PFNGLVERTEXATTRIB3FARBPROC __glewVertexAttrib3fARB;
extern PFNGLVERTEXATTRIB3FVARBPROC __glewVertexAttrib3fvARB;
extern PFNGLVERTEXATTRIB3SARBPROC __glewVertexAttrib3sARB;
extern PFNGLVERTEXATTRIB3SVARBPROC __glewVertexAttrib3svARB;
extern PFNGLVERTEXATTRIB4NBVARBPROC __glewVertexAttrib4NbvARB;
extern PFNGLVERTEXATTRIB4NIVARBPROC __glewVertexAttrib4NivARB;
extern PFNGLVERTEXATTRIB4NSVARBPROC __glewVertexAttrib4NsvARB;
extern PFNGLVERTEXATTRIB4NUBARBPROC __glewVertexAttrib4NubARB;
extern PFNGLVERTEXATTRIB4NUBVARBPROC __glewVertexAttrib4NubvARB;
extern PFNGLVERTEXATTRIB4NUIVARBPROC __glewVertexAttrib4NuivARB;
extern PFNGLVERTEXATTRIB4NUSVARBPROC __glewVertexAttrib4NusvARB;
extern PFNGLVERTEXATTRIB4BVARBPROC __glewVertexAttrib4bvARB;
extern PFNGLVERTEXATTRIB4DARBPROC __glewVertexAttrib4dARB;
extern PFNGLVERTEXATTRIB4DVARBPROC __glewVertexAttrib4dvARB;
extern PFNGLVERTEXATTRIB4FARBPROC __glewVertexAttrib4fARB;
extern PFNGLVERTEXATTRIB4FVARBPROC __glewVertexAttrib4fvARB;
extern PFNGLVERTEXATTRIB4IVARBPROC __glewVertexAttrib4ivARB;
extern PFNGLVERTEXATTRIB4SARBPROC __glewVertexAttrib4sARB;
extern PFNGLVERTEXATTRIB4SVARBPROC __glewVertexAttrib4svARB;
extern PFNGLVERTEXATTRIB4UBVARBPROC __glewVertexAttrib4ubvARB;
extern PFNGLVERTEXATTRIB4UIVARBPROC __glewVertexAttrib4uivARB;
extern PFNGLVERTEXATTRIB4USVARBPROC __glewVertexAttrib4usvARB;
extern PFNGLVERTEXATTRIBPOINTERARBPROC __glewVertexAttribPointerARB;

extern PFNGLBINDATTRIBLOCATIONARBPROC __glewBindAttribLocationARB;
extern PFNGLGETACTIVEATTRIBARBPROC __glewGetActiveAttribARB;
extern PFNGLGETATTRIBLOCATIONARBPROC __glewGetAttribLocationARB;

extern PFNGLCOLORP3UIPROC __glewColorP3ui;
extern PFNGLCOLORP3UIVPROC __glewColorP3uiv;
extern PFNGLCOLORP4UIPROC __glewColorP4ui;
extern PFNGLCOLORP4UIVPROC __glewColorP4uiv;
extern PFNGLMULTITEXCOORDP1UIPROC __glewMultiTexCoordP1ui;
extern PFNGLMULTITEXCOORDP1UIVPROC __glewMultiTexCoordP1uiv;
extern PFNGLMULTITEXCOORDP2UIPROC __glewMultiTexCoordP2ui;
extern PFNGLMULTITEXCOORDP2UIVPROC __glewMultiTexCoordP2uiv;
extern PFNGLMULTITEXCOORDP3UIPROC __glewMultiTexCoordP3ui;
extern PFNGLMULTITEXCOORDP3UIVPROC __glewMultiTexCoordP3uiv;
extern PFNGLMULTITEXCOORDP4UIPROC __glewMultiTexCoordP4ui;
extern PFNGLMULTITEXCOORDP4UIVPROC __glewMultiTexCoordP4uiv;
extern PFNGLNORMALP3UIPROC __glewNormalP3ui;
extern PFNGLNORMALP3UIVPROC __glewNormalP3uiv;
extern PFNGLSECONDARYCOLORP3UIPROC __glewSecondaryColorP3ui;
extern PFNGLSECONDARYCOLORP3UIVPROC __glewSecondaryColorP3uiv;
extern PFNGLTEXCOORDP1UIPROC __glewTexCoordP1ui;
extern PFNGLTEXCOORDP1UIVPROC __glewTexCoordP1uiv;
extern PFNGLTEXCOORDP2UIPROC __glewTexCoordP2ui;
extern PFNGLTEXCOORDP2UIVPROC __glewTexCoordP2uiv;
extern PFNGLTEXCOORDP3UIPROC __glewTexCoordP3ui;
extern PFNGLTEXCOORDP3UIVPROC __glewTexCoordP3uiv;
extern PFNGLTEXCOORDP4UIPROC __glewTexCoordP4ui;
extern PFNGLTEXCOORDP4UIVPROC __glewTexCoordP4uiv;
extern PFNGLVERTEXATTRIBP1UIPROC __glewVertexAttribP1ui;
extern PFNGLVERTEXATTRIBP1UIVPROC __glewVertexAttribP1uiv;
extern PFNGLVERTEXATTRIBP2UIPROC __glewVertexAttribP2ui;
extern PFNGLVERTEXATTRIBP2UIVPROC __glewVertexAttribP2uiv;
extern PFNGLVERTEXATTRIBP3UIPROC __glewVertexAttribP3ui;
extern PFNGLVERTEXATTRIBP3UIVPROC __glewVertexAttribP3uiv;
extern PFNGLVERTEXATTRIBP4UIPROC __glewVertexAttribP4ui;
extern PFNGLVERTEXATTRIBP4UIVPROC __glewVertexAttribP4uiv;
extern PFNGLVERTEXP2UIPROC __glewVertexP2ui;
extern PFNGLVERTEXP2UIVPROC __glewVertexP2uiv;
extern PFNGLVERTEXP3UIPROC __glewVertexP3ui;
extern PFNGLVERTEXP3UIVPROC __glewVertexP3uiv;
extern PFNGLVERTEXP4UIPROC __glewVertexP4ui;
extern PFNGLVERTEXP4UIVPROC __glewVertexP4uiv;

extern PFNGLDEPTHRANGEARRAYVPROC __glewDepthRangeArrayv;
extern PFNGLDEPTHRANGEINDEXEDPROC __glewDepthRangeIndexed;
extern PFNGLGETDOUBLEI_VPROC __glewGetDoublei_v;
extern PFNGLGETFLOATI_VPROC __glewGetFloati_v;
extern PFNGLSCISSORARRAYVPROC __glewScissorArrayv;
extern PFNGLSCISSORINDEXEDPROC __glewScissorIndexed;
extern PFNGLSCISSORINDEXEDVPROC __glewScissorIndexedv;
extern PFNGLVIEWPORTARRAYVPROC __glewViewportArrayv;
extern PFNGLVIEWPORTINDEXEDFPROC __glewViewportIndexedf;
extern PFNGLVIEWPORTINDEXEDFVPROC __glewViewportIndexedfv;

extern PFNGLWINDOWPOS2DARBPROC __glewWindowPos2dARB;
extern PFNGLWINDOWPOS2DVARBPROC __glewWindowPos2dvARB;
extern PFNGLWINDOWPOS2FARBPROC __glewWindowPos2fARB;
extern PFNGLWINDOWPOS2FVARBPROC __glewWindowPos2fvARB;
extern PFNGLWINDOWPOS2IARBPROC __glewWindowPos2iARB;
extern PFNGLWINDOWPOS2IVARBPROC __glewWindowPos2ivARB;
extern PFNGLWINDOWPOS2SARBPROC __glewWindowPos2sARB;
extern PFNGLWINDOWPOS2SVARBPROC __glewWindowPos2svARB;
extern PFNGLWINDOWPOS3DARBPROC __glewWindowPos3dARB;
extern PFNGLWINDOWPOS3DVARBPROC __glewWindowPos3dvARB;
extern PFNGLWINDOWPOS3FARBPROC __glewWindowPos3fARB;
extern PFNGLWINDOWPOS3FVARBPROC __glewWindowPos3fvARB;
extern PFNGLWINDOWPOS3IARBPROC __glewWindowPos3iARB;
extern PFNGLWINDOWPOS3IVARBPROC __glewWindowPos3ivARB;
extern PFNGLWINDOWPOS3SARBPROC __glewWindowPos3sARB;
extern PFNGLWINDOWPOS3SVARBPROC __glewWindowPos3svARB;

extern PFNGLDRAWBUFFERSATIPROC __glewDrawBuffersATI;

extern PFNGLDRAWELEMENTARRAYATIPROC __glewDrawElementArrayATI;
extern PFNGLDRAWRANGEELEMENTARRAYATIPROC __glewDrawRangeElementArrayATI;
extern PFNGLELEMENTPOINTERATIPROC __glewElementPointerATI;

extern PFNGLGETTEXBUMPPARAMETERFVATIPROC __glewGetTexBumpParameterfvATI;
extern PFNGLGETTEXBUMPPARAMETERIVATIPROC __glewGetTexBumpParameterivATI;
extern PFNGLTEXBUMPPARAMETERFVATIPROC __glewTexBumpParameterfvATI;
extern PFNGLTEXBUMPPARAMETERIVATIPROC __glewTexBumpParameterivATI;

extern PFNGLALPHAFRAGMENTOP1ATIPROC __glewAlphaFragmentOp1ATI;
extern PFNGLALPHAFRAGMENTOP2ATIPROC __glewAlphaFragmentOp2ATI;
extern PFNGLALPHAFRAGMENTOP3ATIPROC __glewAlphaFragmentOp3ATI;
extern PFNGLBEGINFRAGMENTSHADERATIPROC __glewBeginFragmentShaderATI;
extern PFNGLBINDFRAGMENTSHADERATIPROC __glewBindFragmentShaderATI;
extern PFNGLCOLORFRAGMENTOP1ATIPROC __glewColorFragmentOp1ATI;
extern PFNGLCOLORFRAGMENTOP2ATIPROC __glewColorFragmentOp2ATI;
extern PFNGLCOLORFRAGMENTOP3ATIPROC __glewColorFragmentOp3ATI;
extern PFNGLDELETEFRAGMENTSHADERATIPROC __glewDeleteFragmentShaderATI;
extern PFNGLENDFRAGMENTSHADERATIPROC __glewEndFragmentShaderATI;
extern PFNGLGENFRAGMENTSHADERSATIPROC __glewGenFragmentShadersATI;
extern PFNGLPASSTEXCOORDATIPROC __glewPassTexCoordATI;
extern PFNGLSAMPLEMAPATIPROC __glewSampleMapATI;
extern PFNGLSETFRAGMENTSHADERCONSTANTATIPROC __glewSetFragmentShaderConstantATI;

extern PFNGLMAPOBJECTBUFFERATIPROC __glewMapObjectBufferATI;
extern PFNGLUNMAPOBJECTBUFFERATIPROC __glewUnmapObjectBufferATI;

extern PFNGLPNTRIANGLESFATIPROC __glewPNTrianglesfATI;
extern PFNGLPNTRIANGLESIATIPROC __glewPNTrianglesiATI;

extern PFNGLSTENCILFUNCSEPARATEATIPROC __glewStencilFuncSeparateATI;
extern PFNGLSTENCILOPSEPARATEATIPROC __glewStencilOpSeparateATI;

extern PFNGLARRAYOBJECTATIPROC __glewArrayObjectATI;
extern PFNGLFREEOBJECTBUFFERATIPROC __glewFreeObjectBufferATI;
extern PFNGLGETARRAYOBJECTFVATIPROC __glewGetArrayObjectfvATI;
extern PFNGLGETARRAYOBJECTIVATIPROC __glewGetArrayObjectivATI;
extern PFNGLGETOBJECTBUFFERFVATIPROC __glewGetObjectBufferfvATI;
extern PFNGLGETOBJECTBUFFERIVATIPROC __glewGetObjectBufferivATI;
extern PFNGLGETVARIANTARRAYOBJECTFVATIPROC __glewGetVariantArrayObjectfvATI;
extern PFNGLGETVARIANTARRAYOBJECTIVATIPROC __glewGetVariantArrayObjectivATI;
extern PFNGLISOBJECTBUFFERATIPROC __glewIsObjectBufferATI;
extern PFNGLNEWOBJECTBUFFERATIPROC __glewNewObjectBufferATI;
extern PFNGLUPDATEOBJECTBUFFERATIPROC __glewUpdateObjectBufferATI;
extern PFNGLVARIANTARRAYOBJECTATIPROC __glewVariantArrayObjectATI;

extern PFNGLGETVERTEXATTRIBARRAYOBJECTFVATIPROC __glewGetVertexAttribArrayObjectfvATI;
extern PFNGLGETVERTEXATTRIBARRAYOBJECTIVATIPROC __glewGetVertexAttribArrayObjectivATI;
extern PFNGLVERTEXATTRIBARRAYOBJECTATIPROC __glewVertexAttribArrayObjectATI;

extern PFNGLCLIENTACTIVEVERTEXSTREAMATIPROC __glewClientActiveVertexStreamATI;
extern PFNGLNORMALSTREAM3BATIPROC __glewNormalStream3bATI;
extern PFNGLNORMALSTREAM3BVATIPROC __glewNormalStream3bvATI;
extern PFNGLNORMALSTREAM3DATIPROC __glewNormalStream3dATI;
extern PFNGLNORMALSTREAM3DVATIPROC __glewNormalStream3dvATI;
extern PFNGLNORMALSTREAM3FATIPROC __glewNormalStream3fATI;
extern PFNGLNORMALSTREAM3FVATIPROC __glewNormalStream3fvATI;
extern PFNGLNORMALSTREAM3IATIPROC __glewNormalStream3iATI;
extern PFNGLNORMALSTREAM3IVATIPROC __glewNormalStream3ivATI;
extern PFNGLNORMALSTREAM3SATIPROC __glewNormalStream3sATI;
extern PFNGLNORMALSTREAM3SVATIPROC __glewNormalStream3svATI;
extern PFNGLVERTEXBLENDENVFATIPROC __glewVertexBlendEnvfATI;
extern PFNGLVERTEXBLENDENVIATIPROC __glewVertexBlendEnviATI;
extern PFNGLVERTEXSTREAM2DATIPROC __glewVertexStream2dATI;
extern PFNGLVERTEXSTREAM2DVATIPROC __glewVertexStream2dvATI;
extern PFNGLVERTEXSTREAM2FATIPROC __glewVertexStream2fATI;
extern PFNGLVERTEXSTREAM2FVATIPROC __glewVertexStream2fvATI;
extern PFNGLVERTEXSTREAM2IATIPROC __glewVertexStream2iATI;
extern PFNGLVERTEXSTREAM2IVATIPROC __glewVertexStream2ivATI;
extern PFNGLVERTEXSTREAM2SATIPROC __glewVertexStream2sATI;
extern PFNGLVERTEXSTREAM2SVATIPROC __glewVertexStream2svATI;
extern PFNGLVERTEXSTREAM3DATIPROC __glewVertexStream3dATI;
extern PFNGLVERTEXSTREAM3DVATIPROC __glewVertexStream3dvATI;
extern PFNGLVERTEXSTREAM3FATIPROC __glewVertexStream3fATI;
extern PFNGLVERTEXSTREAM3FVATIPROC __glewVertexStream3fvATI;
extern PFNGLVERTEXSTREAM3IATIPROC __glewVertexStream3iATI;
extern PFNGLVERTEXSTREAM3IVATIPROC __glewVertexStream3ivATI;
extern PFNGLVERTEXSTREAM3SATIPROC __glewVertexStream3sATI;
extern PFNGLVERTEXSTREAM3SVATIPROC __glewVertexStream3svATI;
extern PFNGLVERTEXSTREAM4DATIPROC __glewVertexStream4dATI;
extern PFNGLVERTEXSTREAM4DVATIPROC __glewVertexStream4dvATI;
extern PFNGLVERTEXSTREAM4FATIPROC __glewVertexStream4fATI;
extern PFNGLVERTEXSTREAM4FVATIPROC __glewVertexStream4fvATI;
extern PFNGLVERTEXSTREAM4IATIPROC __glewVertexStream4iATI;
extern PFNGLVERTEXSTREAM4IVATIPROC __glewVertexStream4ivATI;
extern PFNGLVERTEXSTREAM4SATIPROC __glewVertexStream4sATI;
extern PFNGLVERTEXSTREAM4SVATIPROC __glewVertexStream4svATI;

extern PFNGLGETUNIFORMBUFFERSIZEEXTPROC __glewGetUniformBufferSizeEXT;
extern PFNGLGETUNIFORMOFFSETEXTPROC __glewGetUniformOffsetEXT;
extern PFNGLUNIFORMBUFFEREXTPROC __glewUniformBufferEXT;

extern PFNGLBLENDCOLOREXTPROC __glewBlendColorEXT;

extern PFNGLBLENDEQUATIONSEPARATEEXTPROC __glewBlendEquationSeparateEXT;

extern PFNGLBLENDFUNCSEPARATEEXTPROC __glewBlendFuncSeparateEXT;

extern PFNGLBLENDEQUATIONEXTPROC __glewBlendEquationEXT;

extern PFNGLCOLORSUBTABLEEXTPROC __glewColorSubTableEXT;
extern PFNGLCOPYCOLORSUBTABLEEXTPROC __glewCopyColorSubTableEXT;

extern PFNGLLOCKARRAYSEXTPROC __glewLockArraysEXT;
extern PFNGLUNLOCKARRAYSEXTPROC __glewUnlockArraysEXT;

extern PFNGLCONVOLUTIONFILTER1DEXTPROC __glewConvolutionFilter1DEXT;
extern PFNGLCONVOLUTIONFILTER2DEXTPROC __glewConvolutionFilter2DEXT;
extern PFNGLCONVOLUTIONPARAMETERFEXTPROC __glewConvolutionParameterfEXT;
extern PFNGLCONVOLUTIONPARAMETERFVEXTPROC __glewConvolutionParameterfvEXT;
extern PFNGLCONVOLUTIONPARAMETERIEXTPROC __glewConvolutionParameteriEXT;
extern PFNGLCONVOLUTIONPARAMETERIVEXTPROC __glewConvolutionParameterivEXT;
extern PFNGLCOPYCONVOLUTIONFILTER1DEXTPROC __glewCopyConvolutionFilter1DEXT;
extern PFNGLCOPYCONVOLUTIONFILTER2DEXTPROC __glewCopyConvolutionFilter2DEXT;
extern PFNGLGETCONVOLUTIONFILTEREXTPROC __glewGetConvolutionFilterEXT;
extern PFNGLGETCONVOLUTIONPARAMETERFVEXTPROC __glewGetConvolutionParameterfvEXT;
extern PFNGLGETCONVOLUTIONPARAMETERIVEXTPROC __glewGetConvolutionParameterivEXT;
extern PFNGLGETSEPARABLEFILTEREXTPROC __glewGetSeparableFilterEXT;
extern PFNGLSEPARABLEFILTER2DEXTPROC __glewSeparableFilter2DEXT;

extern PFNGLBINORMALPOINTEREXTPROC __glewBinormalPointerEXT;
extern PFNGLTANGENTPOINTEREXTPROC __glewTangentPointerEXT;

extern PFNGLCOPYTEXIMAGE1DEXTPROC __glewCopyTexImage1DEXT;
extern PFNGLCOPYTEXIMAGE2DEXTPROC __glewCopyTexImage2DEXT;
extern PFNGLCOPYTEXSUBIMAGE1DEXTPROC __glewCopyTexSubImage1DEXT;
extern PFNGLCOPYTEXSUBIMAGE2DEXTPROC __glewCopyTexSubImage2DEXT;
extern PFNGLCOPYTEXSUBIMAGE3DEXTPROC __glewCopyTexSubImage3DEXT;

extern PFNGLCULLPARAMETERDVEXTPROC __glewCullParameterdvEXT;
extern PFNGLCULLPARAMETERFVEXTPROC __glewCullParameterfvEXT;

extern PFNGLDEPTHBOUNDSEXTPROC __glewDepthBoundsEXT;

extern PFNGLBINDMULTITEXTUREEXTPROC __glewBindMultiTextureEXT;
extern PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC __glewCheckNamedFramebufferStatusEXT;
extern PFNGLCLIENTATTRIBDEFAULTEXTPROC __glewClientAttribDefaultEXT;
extern PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC __glewCompressedMultiTexImage1DEXT;
extern PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC __glewCompressedMultiTexImage2DEXT;
extern PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC __glewCompressedMultiTexImage3DEXT;
extern PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC __glewCompressedMultiTexSubImage1DEXT;
extern PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC __glewCompressedMultiTexSubImage2DEXT;
extern PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC __glewCompressedMultiTexSubImage3DEXT;
extern PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC __glewCompressedTextureImage1DEXT;
extern PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC __glewCompressedTextureImage2DEXT;
extern PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC __glewCompressedTextureImage3DEXT;
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC __glewCompressedTextureSubImage1DEXT;
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC __glewCompressedTextureSubImage2DEXT;
extern PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC __glewCompressedTextureSubImage3DEXT;
extern PFNGLCOPYMULTITEXIMAGE1DEXTPROC __glewCopyMultiTexImage1DEXT;
extern PFNGLCOPYMULTITEXIMAGE2DEXTPROC __glewCopyMultiTexImage2DEXT;
extern PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC __glewCopyMultiTexSubImage1DEXT;
extern PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC __glewCopyMultiTexSubImage2DEXT;
extern PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC __glewCopyMultiTexSubImage3DEXT;
extern PFNGLCOPYTEXTUREIMAGE1DEXTPROC __glewCopyTextureImage1DEXT;
extern PFNGLCOPYTEXTUREIMAGE2DEXTPROC __glewCopyTextureImage2DEXT;
extern PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC __glewCopyTextureSubImage1DEXT;
extern PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC __glewCopyTextureSubImage2DEXT;
extern PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC __glewCopyTextureSubImage3DEXT;
extern PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC __glewDisableClientStateIndexedEXT;
extern PFNGLDISABLECLIENTSTATEIEXTPROC __glewDisableClientStateiEXT;
extern PFNGLDISABLEVERTEXARRAYATTRIBEXTPROC __glewDisableVertexArrayAttribEXT;
extern PFNGLDISABLEVERTEXARRAYEXTPROC __glewDisableVertexArrayEXT;
extern PFNGLENABLECLIENTSTATEINDEXEDEXTPROC __glewEnableClientStateIndexedEXT;
extern PFNGLENABLECLIENTSTATEIEXTPROC __glewEnableClientStateiEXT;
extern PFNGLENABLEVERTEXARRAYATTRIBEXTPROC __glewEnableVertexArrayAttribEXT;
extern PFNGLENABLEVERTEXARRAYEXTPROC __glewEnableVertexArrayEXT;
extern PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC __glewFlushMappedNamedBufferRangeEXT;
extern PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC __glewFramebufferDrawBufferEXT;
extern PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC __glewFramebufferDrawBuffersEXT;
extern PFNGLFRAMEBUFFERREADBUFFEREXTPROC __glewFramebufferReadBufferEXT;
extern PFNGLGENERATEMULTITEXMIPMAPEXTPROC __glewGenerateMultiTexMipmapEXT;
extern PFNGLGENERATETEXTUREMIPMAPEXTPROC __glewGenerateTextureMipmapEXT;
extern PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC __glewGetCompressedMultiTexImageEXT;
extern PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC __glewGetCompressedTextureImageEXT;
extern PFNGLGETDOUBLEINDEXEDVEXTPROC __glewGetDoubleIndexedvEXT;
extern PFNGLGETDOUBLEI_VEXTPROC __glewGetDoublei_vEXT;
extern PFNGLGETFLOATINDEXEDVEXTPROC __glewGetFloatIndexedvEXT;
extern PFNGLGETFLOATI_VEXTPROC __glewGetFloati_vEXT;
extern PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC __glewGetFramebufferParameterivEXT;
extern PFNGLGETMULTITEXENVFVEXTPROC __glewGetMultiTexEnvfvEXT;
extern PFNGLGETMULTITEXENVIVEXTPROC __glewGetMultiTexEnvivEXT;
extern PFNGLGETMULTITEXGENDVEXTPROC __glewGetMultiTexGendvEXT;
extern PFNGLGETMULTITEXGENFVEXTPROC __glewGetMultiTexGenfvEXT;
extern PFNGLGETMULTITEXGENIVEXTPROC __glewGetMultiTexGenivEXT;
extern PFNGLGETMULTITEXIMAGEEXTPROC __glewGetMultiTexImageEXT;
extern PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC __glewGetMultiTexLevelParameterfvEXT;
extern PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC __glewGetMultiTexLevelParameterivEXT;
extern PFNGLGETMULTITEXPARAMETERIIVEXTPROC __glewGetMultiTexParameterIivEXT;
extern PFNGLGETMULTITEXPARAMETERIUIVEXTPROC __glewGetMultiTexParameterIuivEXT;
extern PFNGLGETMULTITEXPARAMETERFVEXTPROC __glewGetMultiTexParameterfvEXT;
extern PFNGLGETMULTITEXPARAMETERIVEXTPROC __glewGetMultiTexParameterivEXT;
extern PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC __glewGetNamedBufferParameterivEXT;
extern PFNGLGETNAMEDBUFFERPOINTERVEXTPROC __glewGetNamedBufferPointervEXT;
extern PFNGLGETNAMEDBUFFERSUBDATAEXTPROC __glewGetNamedBufferSubDataEXT;
extern PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC __glewGetNamedFramebufferAttachmentParameterivEXT;
extern PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC __glewGetNamedProgramLocalParameterIivEXT;
extern PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC __glewGetNamedProgramLocalParameterIuivEXT;
extern PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC __glewGetNamedProgramLocalParameterdvEXT;
extern PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC __glewGetNamedProgramLocalParameterfvEXT;
extern PFNGLGETNAMEDPROGRAMSTRINGEXTPROC __glewGetNamedProgramStringEXT;
extern PFNGLGETNAMEDPROGRAMIVEXTPROC __glewGetNamedProgramivEXT;
extern PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC __glewGetNamedRenderbufferParameterivEXT;
extern PFNGLGETPOINTERINDEXEDVEXTPROC __glewGetPointerIndexedvEXT;
extern PFNGLGETPOINTERI_VEXTPROC __glewGetPointeri_vEXT;
extern PFNGLGETTEXTUREIMAGEEXTPROC __glewGetTextureImageEXT;
extern PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC __glewGetTextureLevelParameterfvEXT;
extern PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC __glewGetTextureLevelParameterivEXT;
extern PFNGLGETTEXTUREPARAMETERIIVEXTPROC __glewGetTextureParameterIivEXT;
extern PFNGLGETTEXTUREPARAMETERIUIVEXTPROC __glewGetTextureParameterIuivEXT;
extern PFNGLGETTEXTUREPARAMETERFVEXTPROC __glewGetTextureParameterfvEXT;
extern PFNGLGETTEXTUREPARAMETERIVEXTPROC __glewGetTextureParameterivEXT;
extern PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC __glewGetVertexArrayIntegeri_vEXT;
extern PFNGLGETVERTEXARRAYINTEGERVEXTPROC __glewGetVertexArrayIntegervEXT;
extern PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC __glewGetVertexArrayPointeri_vEXT;
extern PFNGLGETVERTEXARRAYPOINTERVEXTPROC __glewGetVertexArrayPointervEXT;
extern PFNGLMAPNAMEDBUFFEREXTPROC __glewMapNamedBufferEXT;
extern PFNGLMAPNAMEDBUFFERRANGEEXTPROC __glewMapNamedBufferRangeEXT;
extern PFNGLMATRIXFRUSTUMEXTPROC __glewMatrixFrustumEXT;
extern PFNGLMATRIXLOADIDENTITYEXTPROC __glewMatrixLoadIdentityEXT;
extern PFNGLMATRIXLOADTRANSPOSEDEXTPROC __glewMatrixLoadTransposedEXT;
extern PFNGLMATRIXLOADTRANSPOSEFEXTPROC __glewMatrixLoadTransposefEXT;
extern PFNGLMATRIXLOADDEXTPROC __glewMatrixLoaddEXT;
extern PFNGLMATRIXLOADFEXTPROC __glewMatrixLoadfEXT;
extern PFNGLMATRIXMULTTRANSPOSEDEXTPROC __glewMatrixMultTransposedEXT;
extern PFNGLMATRIXMULTTRANSPOSEFEXTPROC __glewMatrixMultTransposefEXT;
extern PFNGLMATRIXMULTDEXTPROC __glewMatrixMultdEXT;
extern PFNGLMATRIXMULTFEXTPROC __glewMatrixMultfEXT;
extern PFNGLMATRIXORTHOEXTPROC __glewMatrixOrthoEXT;
extern PFNGLMATRIXPOPEXTPROC __glewMatrixPopEXT;
extern PFNGLMATRIXPUSHEXTPROC __glewMatrixPushEXT;
extern PFNGLMATRIXROTATEDEXTPROC __glewMatrixRotatedEXT;
extern PFNGLMATRIXROTATEFEXTPROC __glewMatrixRotatefEXT;
extern PFNGLMATRIXSCALEDEXTPROC __glewMatrixScaledEXT;
extern PFNGLMATRIXSCALEFEXTPROC __glewMatrixScalefEXT;
extern PFNGLMATRIXTRANSLATEDEXTPROC __glewMatrixTranslatedEXT;
extern PFNGLMATRIXTRANSLATEFEXTPROC __glewMatrixTranslatefEXT;
extern PFNGLMULTITEXBUFFEREXTPROC __glewMultiTexBufferEXT;
extern PFNGLMULTITEXCOORDPOINTEREXTPROC __glewMultiTexCoordPointerEXT;
extern PFNGLMULTITEXENVFEXTPROC __glewMultiTexEnvfEXT;
extern PFNGLMULTITEXENVFVEXTPROC __glewMultiTexEnvfvEXT;
extern PFNGLMULTITEXENVIEXTPROC __glewMultiTexEnviEXT;
extern PFNGLMULTITEXENVIVEXTPROC __glewMultiTexEnvivEXT;
extern PFNGLMULTITEXGENDEXTPROC __glewMultiTexGendEXT;
extern PFNGLMULTITEXGENDVEXTPROC __glewMultiTexGendvEXT;
extern PFNGLMULTITEXGENFEXTPROC __glewMultiTexGenfEXT;
extern PFNGLMULTITEXGENFVEXTPROC __glewMultiTexGenfvEXT;
extern PFNGLMULTITEXGENIEXTPROC __glewMultiTexGeniEXT;
extern PFNGLMULTITEXGENIVEXTPROC __glewMultiTexGenivEXT;
extern PFNGLMULTITEXIMAGE1DEXTPROC __glewMultiTexImage1DEXT;
extern PFNGLMULTITEXIMAGE2DEXTPROC __glewMultiTexImage2DEXT;
extern PFNGLMULTITEXIMAGE3DEXTPROC __glewMultiTexImage3DEXT;
extern PFNGLMULTITEXPARAMETERIIVEXTPROC __glewMultiTexParameterIivEXT;
extern PFNGLMULTITEXPARAMETERIUIVEXTPROC __glewMultiTexParameterIuivEXT;
extern PFNGLMULTITEXPARAMETERFEXTPROC __glewMultiTexParameterfEXT;
extern PFNGLMULTITEXPARAMETERFVEXTPROC __glewMultiTexParameterfvEXT;
extern PFNGLMULTITEXPARAMETERIEXTPROC __glewMultiTexParameteriEXT;
extern PFNGLMULTITEXPARAMETERIVEXTPROC __glewMultiTexParameterivEXT;
extern PFNGLMULTITEXRENDERBUFFEREXTPROC __glewMultiTexRenderbufferEXT;
extern PFNGLMULTITEXSUBIMAGE1DEXTPROC __glewMultiTexSubImage1DEXT;
extern PFNGLMULTITEXSUBIMAGE2DEXTPROC __glewMultiTexSubImage2DEXT;
extern PFNGLMULTITEXSUBIMAGE3DEXTPROC __glewMultiTexSubImage3DEXT;
extern PFNGLNAMEDBUFFERDATAEXTPROC __glewNamedBufferDataEXT;
extern PFNGLNAMEDBUFFERSUBDATAEXTPROC __glewNamedBufferSubDataEXT;
extern PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC __glewNamedCopyBufferSubDataEXT;
extern PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC __glewNamedFramebufferRenderbufferEXT;
extern PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC __glewNamedFramebufferTexture1DEXT;
extern PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC __glewNamedFramebufferTexture2DEXT;
extern PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC __glewNamedFramebufferTexture3DEXT;
extern PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC __glewNamedFramebufferTextureEXT;
extern PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC __glewNamedFramebufferTextureFaceEXT;
extern PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC __glewNamedFramebufferTextureLayerEXT;
extern PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC __glewNamedProgramLocalParameter4dEXT;
extern PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC __glewNamedProgramLocalParameter4dvEXT;
extern PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC __glewNamedProgramLocalParameter4fEXT;
extern PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC __glewNamedProgramLocalParameter4fvEXT;
extern PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC __glewNamedProgramLocalParameterI4iEXT;
extern PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC __glewNamedProgramLocalParameterI4ivEXT;
extern PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC __glewNamedProgramLocalParameterI4uiEXT;
extern PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC __glewNamedProgramLocalParameterI4uivEXT;
extern PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC __glewNamedProgramLocalParameters4fvEXT;
extern PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC __glewNamedProgramLocalParametersI4ivEXT;
extern PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC __glewNamedProgramLocalParametersI4uivEXT;
extern PFNGLNAMEDPROGRAMSTRINGEXTPROC __glewNamedProgramStringEXT;
extern PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC __glewNamedRenderbufferStorageEXT;
extern PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC __glewNamedRenderbufferStorageMultisampleCoverageEXT;
extern PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC __glewNamedRenderbufferStorageMultisampleEXT;
extern PFNGLPROGRAMUNIFORM1FEXTPROC __glewProgramUniform1fEXT;
extern PFNGLPROGRAMUNIFORM1FVEXTPROC __glewProgramUniform1fvEXT;
extern PFNGLPROGRAMUNIFORM1IEXTPROC __glewProgramUniform1iEXT;
extern PFNGLPROGRAMUNIFORM1IVEXTPROC __glewProgramUniform1ivEXT;
extern PFNGLPROGRAMUNIFORM1UIEXTPROC __glewProgramUniform1uiEXT;
extern PFNGLPROGRAMUNIFORM1UIVEXTPROC __glewProgramUniform1uivEXT;
extern PFNGLPROGRAMUNIFORM2FEXTPROC __glewProgramUniform2fEXT;
extern PFNGLPROGRAMUNIFORM2FVEXTPROC __glewProgramUniform2fvEXT;
extern PFNGLPROGRAMUNIFORM2IEXTPROC __glewProgramUniform2iEXT;
extern PFNGLPROGRAMUNIFORM2IVEXTPROC __glewProgramUniform2ivEXT;
extern PFNGLPROGRAMUNIFORM2UIEXTPROC __glewProgramUniform2uiEXT;
extern PFNGLPROGRAMUNIFORM2UIVEXTPROC __glewProgramUniform2uivEXT;
extern PFNGLPROGRAMUNIFORM3FEXTPROC __glewProgramUniform3fEXT;
extern PFNGLPROGRAMUNIFORM3FVEXTPROC __glewProgramUniform3fvEXT;
extern PFNGLPROGRAMUNIFORM3IEXTPROC __glewProgramUniform3iEXT;
extern PFNGLPROGRAMUNIFORM3IVEXTPROC __glewProgramUniform3ivEXT;
extern PFNGLPROGRAMUNIFORM3UIEXTPROC __glewProgramUniform3uiEXT;
extern PFNGLPROGRAMUNIFORM3UIVEXTPROC __glewProgramUniform3uivEXT;
extern PFNGLPROGRAMUNIFORM4FEXTPROC __glewProgramUniform4fEXT;
extern PFNGLPROGRAMUNIFORM4FVEXTPROC __glewProgramUniform4fvEXT;
extern PFNGLPROGRAMUNIFORM4IEXTPROC __glewProgramUniform4iEXT;
extern PFNGLPROGRAMUNIFORM4IVEXTPROC __glewProgramUniform4ivEXT;
extern PFNGLPROGRAMUNIFORM4UIEXTPROC __glewProgramUniform4uiEXT;
extern PFNGLPROGRAMUNIFORM4UIVEXTPROC __glewProgramUniform4uivEXT;
extern PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC __glewProgramUniformMatrix2fvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC __glewProgramUniformMatrix2x3fvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC __glewProgramUniformMatrix2x4fvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC __glewProgramUniformMatrix3fvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC __glewProgramUniformMatrix3x2fvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC __glewProgramUniformMatrix3x4fvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC __glewProgramUniformMatrix4fvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC __glewProgramUniformMatrix4x2fvEXT;
extern PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC __glewProgramUniformMatrix4x3fvEXT;
extern PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC __glewPushClientAttribDefaultEXT;
extern PFNGLTEXTUREBUFFEREXTPROC __glewTextureBufferEXT;
extern PFNGLTEXTUREIMAGE1DEXTPROC __glewTextureImage1DEXT;
extern PFNGLTEXTUREIMAGE2DEXTPROC __glewTextureImage2DEXT;
extern PFNGLTEXTUREIMAGE3DEXTPROC __glewTextureImage3DEXT;
extern PFNGLTEXTUREPARAMETERIIVEXTPROC __glewTextureParameterIivEXT;
extern PFNGLTEXTUREPARAMETERIUIVEXTPROC __glewTextureParameterIuivEXT;
extern PFNGLTEXTUREPARAMETERFEXTPROC __glewTextureParameterfEXT;
extern PFNGLTEXTUREPARAMETERFVEXTPROC __glewTextureParameterfvEXT;
extern PFNGLTEXTUREPARAMETERIEXTPROC __glewTextureParameteriEXT;
extern PFNGLTEXTUREPARAMETERIVEXTPROC __glewTextureParameterivEXT;
extern PFNGLTEXTURERENDERBUFFEREXTPROC __glewTextureRenderbufferEXT;
extern PFNGLTEXTURESUBIMAGE1DEXTPROC __glewTextureSubImage1DEXT;
extern PFNGLTEXTURESUBIMAGE2DEXTPROC __glewTextureSubImage2DEXT;
extern PFNGLTEXTURESUBIMAGE3DEXTPROC __glewTextureSubImage3DEXT;
extern PFNGLUNMAPNAMEDBUFFEREXTPROC __glewUnmapNamedBufferEXT;
extern PFNGLVERTEXARRAYCOLOROFFSETEXTPROC __glewVertexArrayColorOffsetEXT;
extern PFNGLVERTEXARRAYEDGEFLAGOFFSETEXTPROC __glewVertexArrayEdgeFlagOffsetEXT;
extern PFNGLVERTEXARRAYFOGCOORDOFFSETEXTPROC __glewVertexArrayFogCoordOffsetEXT;
extern PFNGLVERTEXARRAYINDEXOFFSETEXTPROC __glewVertexArrayIndexOffsetEXT;
extern PFNGLVERTEXARRAYMULTITEXCOORDOFFSETEXTPROC __glewVertexArrayMultiTexCoordOffsetEXT;
extern PFNGLVERTEXARRAYNORMALOFFSETEXTPROC __glewVertexArrayNormalOffsetEXT;
extern PFNGLVERTEXARRAYSECONDARYCOLOROFFSETEXTPROC __glewVertexArraySecondaryColorOffsetEXT;
extern PFNGLVERTEXARRAYTEXCOORDOFFSETEXTPROC __glewVertexArrayTexCoordOffsetEXT;
extern PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXTPROC __glewVertexArrayVertexAttribIOffsetEXT;
extern PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC __glewVertexArrayVertexAttribOffsetEXT;
extern PFNGLVERTEXARRAYVERTEXOFFSETEXTPROC __glewVertexArrayVertexOffsetEXT;

extern PFNGLCOLORMASKINDEXEDEXTPROC __glewColorMaskIndexedEXT;
extern PFNGLDISABLEINDEXEDEXTPROC __glewDisableIndexedEXT;
extern PFNGLENABLEINDEXEDEXTPROC __glewEnableIndexedEXT;
extern PFNGLGETBOOLEANINDEXEDVEXTPROC __glewGetBooleanIndexedvEXT;
extern PFNGLGETINTEGERINDEXEDVEXTPROC __glewGetIntegerIndexedvEXT;
extern PFNGLISENABLEDINDEXEDEXTPROC __glewIsEnabledIndexedEXT;

extern PFNGLDRAWARRAYSINSTANCEDEXTPROC __glewDrawArraysInstancedEXT;
extern PFNGLDRAWELEMENTSINSTANCEDEXTPROC __glewDrawElementsInstancedEXT;

extern PFNGLDRAWRANGEELEMENTSEXTPROC __glewDrawRangeElementsEXT;

extern PFNGLFOGCOORDPOINTEREXTPROC __glewFogCoordPointerEXT;
extern PFNGLFOGCOORDDEXTPROC __glewFogCoorddEXT;
extern PFNGLFOGCOORDDVEXTPROC __glewFogCoorddvEXT;
extern PFNGLFOGCOORDFEXTPROC __glewFogCoordfEXT;
extern PFNGLFOGCOORDFVEXTPROC __glewFogCoordfvEXT;

extern PFNGLFRAGMENTCOLORMATERIALEXTPROC __glewFragmentColorMaterialEXT;
extern PFNGLFRAGMENTLIGHTMODELFEXTPROC __glewFragmentLightModelfEXT;
extern PFNGLFRAGMENTLIGHTMODELFVEXTPROC __glewFragmentLightModelfvEXT;
extern PFNGLFRAGMENTLIGHTMODELIEXTPROC __glewFragmentLightModeliEXT;
extern PFNGLFRAGMENTLIGHTMODELIVEXTPROC __glewFragmentLightModelivEXT;
extern PFNGLFRAGMENTLIGHTFEXTPROC __glewFragmentLightfEXT;
extern PFNGLFRAGMENTLIGHTFVEXTPROC __glewFragmentLightfvEXT;
extern PFNGLFRAGMENTLIGHTIEXTPROC __glewFragmentLightiEXT;
extern PFNGLFRAGMENTLIGHTIVEXTPROC __glewFragmentLightivEXT;
extern PFNGLFRAGMENTMATERIALFEXTPROC __glewFragmentMaterialfEXT;
extern PFNGLFRAGMENTMATERIALFVEXTPROC __glewFragmentMaterialfvEXT;
extern PFNGLFRAGMENTMATERIALIEXTPROC __glewFragmentMaterialiEXT;
extern PFNGLFRAGMENTMATERIALIVEXTPROC __glewFragmentMaterialivEXT;
extern PFNGLGETFRAGMENTLIGHTFVEXTPROC __glewGetFragmentLightfvEXT;
extern PFNGLGETFRAGMENTLIGHTIVEXTPROC __glewGetFragmentLightivEXT;
extern PFNGLGETFRAGMENTMATERIALFVEXTPROC __glewGetFragmentMaterialfvEXT;
extern PFNGLGETFRAGMENTMATERIALIVEXTPROC __glewGetFragmentMaterialivEXT;
extern PFNGLLIGHTENVIEXTPROC __glewLightEnviEXT;

extern PFNGLBLITFRAMEBUFFEREXTPROC __glewBlitFramebufferEXT;

extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC __glewRenderbufferStorageMultisampleEXT;

extern PFNGLBINDFRAMEBUFFEREXTPROC __glewBindFramebufferEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC __glewBindRenderbufferEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC __glewCheckFramebufferStatusEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC __glewDeleteFramebuffersEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC __glewDeleteRenderbuffersEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC __glewFramebufferRenderbufferEXT;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC __glewFramebufferTexture1DEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC __glewFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC __glewFramebufferTexture3DEXT;
extern PFNGLGENFRAMEBUFFERSEXTPROC __glewGenFramebuffersEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC __glewGenRenderbuffersEXT;
extern PFNGLGENERATEMIPMAPEXTPROC __glewGenerateMipmapEXT;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC __glewGetFramebufferAttachmentParameterivEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC __glewGetRenderbufferParameterivEXT;
extern PFNGLISFRAMEBUFFEREXTPROC __glewIsFramebufferEXT;
extern PFNGLISRENDERBUFFEREXTPROC __glewIsRenderbufferEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC __glewRenderbufferStorageEXT;

extern PFNGLFRAMEBUFFERTEXTUREEXTPROC __glewFramebufferTextureEXT;
extern PFNGLFRAMEBUFFERTEXTUREFACEEXTPROC __glewFramebufferTextureFaceEXT;
extern PFNGLPROGRAMPARAMETERIEXTPROC __glewProgramParameteriEXT;

extern PFNGLPROGRAMENVPARAMETERS4FVEXTPROC __glewProgramEnvParameters4fvEXT;
extern PFNGLPROGRAMLOCALPARAMETERS4FVEXTPROC __glewProgramLocalParameters4fvEXT;

extern PFNGLBINDFRAGDATALOCATIONEXTPROC __glewBindFragDataLocationEXT;
extern PFNGLGETFRAGDATALOCATIONEXTPROC __glewGetFragDataLocationEXT;
extern PFNGLGETUNIFORMUIVEXTPROC __glewGetUniformuivEXT;
extern PFNGLGETVERTEXATTRIBIIVEXTPROC __glewGetVertexAttribIivEXT;
extern PFNGLGETVERTEXATTRIBIUIVEXTPROC __glewGetVertexAttribIuivEXT;
extern PFNGLUNIFORM1UIEXTPROC __glewUniform1uiEXT;
extern PFNGLUNIFORM1UIVEXTPROC __glewUniform1uivEXT;
extern PFNGLUNIFORM2UIEXTPROC __glewUniform2uiEXT;
extern PFNGLUNIFORM2UIVEXTPROC __glewUniform2uivEXT;
extern PFNGLUNIFORM3UIEXTPROC __glewUniform3uiEXT;
extern PFNGLUNIFORM3UIVEXTPROC __glewUniform3uivEXT;
extern PFNGLUNIFORM4UIEXTPROC __glewUniform4uiEXT;
extern PFNGLUNIFORM4UIVEXTPROC __glewUniform4uivEXT;
extern PFNGLVERTEXATTRIBI1IEXTPROC __glewVertexAttribI1iEXT;
extern PFNGLVERTEXATTRIBI1IVEXTPROC __glewVertexAttribI1ivEXT;
extern PFNGLVERTEXATTRIBI1UIEXTPROC __glewVertexAttribI1uiEXT;
extern PFNGLVERTEXATTRIBI1UIVEXTPROC __glewVertexAttribI1uivEXT;
extern PFNGLVERTEXATTRIBI2IEXTPROC __glewVertexAttribI2iEXT;
extern PFNGLVERTEXATTRIBI2IVEXTPROC __glewVertexAttribI2ivEXT;
extern PFNGLVERTEXATTRIBI2UIEXTPROC __glewVertexAttribI2uiEXT;
extern PFNGLVERTEXATTRIBI2UIVEXTPROC __glewVertexAttribI2uivEXT;
extern PFNGLVERTEXATTRIBI3IEXTPROC __glewVertexAttribI3iEXT;
extern PFNGLVERTEXATTRIBI3IVEXTPROC __glewVertexAttribI3ivEXT;
extern PFNGLVERTEXATTRIBI3UIEXTPROC __glewVertexAttribI3uiEXT;
extern PFNGLVERTEXATTRIBI3UIVEXTPROC __glewVertexAttribI3uivEXT;
extern PFNGLVERTEXATTRIBI4BVEXTPROC __glewVertexAttribI4bvEXT;
extern PFNGLVERTEXATTRIBI4IEXTPROC __glewVertexAttribI4iEXT;
extern PFNGLVERTEXATTRIBI4IVEXTPROC __glewVertexAttribI4ivEXT;
extern PFNGLVERTEXATTRIBI4SVEXTPROC __glewVertexAttribI4svEXT;
extern PFNGLVERTEXATTRIBI4UBVEXTPROC __glewVertexAttribI4ubvEXT;
extern PFNGLVERTEXATTRIBI4UIEXTPROC __glewVertexAttribI4uiEXT;
extern PFNGLVERTEXATTRIBI4UIVEXTPROC __glewVertexAttribI4uivEXT;
extern PFNGLVERTEXATTRIBI4USVEXTPROC __glewVertexAttribI4usvEXT;
extern PFNGLVERTEXATTRIBIPOINTEREXTPROC __glewVertexAttribIPointerEXT;

extern PFNGLGETHISTOGRAMEXTPROC __glewGetHistogramEXT;
extern PFNGLGETHISTOGRAMPARAMETERFVEXTPROC __glewGetHistogramParameterfvEXT;
extern PFNGLGETHISTOGRAMPARAMETERIVEXTPROC __glewGetHistogramParameterivEXT;
extern PFNGLGETMINMAXEXTPROC __glewGetMinmaxEXT;
extern PFNGLGETMINMAXPARAMETERFVEXTPROC __glewGetMinmaxParameterfvEXT;
extern PFNGLGETMINMAXPARAMETERIVEXTPROC __glewGetMinmaxParameterivEXT;
extern PFNGLHISTOGRAMEXTPROC __glewHistogramEXT;
extern PFNGLMINMAXEXTPROC __glewMinmaxEXT;
extern PFNGLRESETHISTOGRAMEXTPROC __glewResetHistogramEXT;
extern PFNGLRESETMINMAXEXTPROC __glewResetMinmaxEXT;

extern PFNGLINDEXFUNCEXTPROC __glewIndexFuncEXT;

extern PFNGLINDEXMATERIALEXTPROC __glewIndexMaterialEXT;

extern PFNGLAPPLYTEXTUREEXTPROC __glewApplyTextureEXT;
extern PFNGLTEXTURELIGHTEXTPROC __glewTextureLightEXT;
extern PFNGLTEXTUREMATERIALEXTPROC __glewTextureMaterialEXT;

extern PFNGLMULTIDRAWARRAYSEXTPROC __glewMultiDrawArraysEXT;
extern PFNGLMULTIDRAWELEMENTSEXTPROC __glewMultiDrawElementsEXT;

extern PFNGLSAMPLEMASKEXTPROC __glewSampleMaskEXT;
extern PFNGLSAMPLEPATTERNEXTPROC __glewSamplePatternEXT;

extern PFNGLCOLORTABLEEXTPROC __glewColorTableEXT;
extern PFNGLGETCOLORTABLEEXTPROC __glewGetColorTableEXT;
extern PFNGLGETCOLORTABLEPARAMETERFVEXTPROC __glewGetColorTableParameterfvEXT;
extern PFNGLGETCOLORTABLEPARAMETERIVEXTPROC __glewGetColorTableParameterivEXT;

extern PFNGLGETPIXELTRANSFORMPARAMETERFVEXTPROC __glewGetPixelTransformParameterfvEXT;
extern PFNGLGETPIXELTRANSFORMPARAMETERIVEXTPROC __glewGetPixelTransformParameterivEXT;
extern PFNGLPIXELTRANSFORMPARAMETERFEXTPROC __glewPixelTransformParameterfEXT;
extern PFNGLPIXELTRANSFORMPARAMETERFVEXTPROC __glewPixelTransformParameterfvEXT;
extern PFNGLPIXELTRANSFORMPARAMETERIEXTPROC __glewPixelTransformParameteriEXT;
extern PFNGLPIXELTRANSFORMPARAMETERIVEXTPROC __glewPixelTransformParameterivEXT;

extern PFNGLPOINTPARAMETERFEXTPROC __glewPointParameterfEXT;
extern PFNGLPOINTPARAMETERFVEXTPROC __glewPointParameterfvEXT;

extern PFNGLPOLYGONOFFSETEXTPROC __glewPolygonOffsetEXT;

extern PFNGLPROVOKINGVERTEXEXTPROC __glewProvokingVertexEXT;

extern PFNGLBEGINSCENEEXTPROC __glewBeginSceneEXT;
extern PFNGLENDSCENEEXTPROC __glewEndSceneEXT;

extern PFNGLSECONDARYCOLOR3BEXTPROC __glewSecondaryColor3bEXT;
extern PFNGLSECONDARYCOLOR3BVEXTPROC __glewSecondaryColor3bvEXT;
extern PFNGLSECONDARYCOLOR3DEXTPROC __glewSecondaryColor3dEXT;
extern PFNGLSECONDARYCOLOR3DVEXTPROC __glewSecondaryColor3dvEXT;
extern PFNGLSECONDARYCOLOR3FEXTPROC __glewSecondaryColor3fEXT;
extern PFNGLSECONDARYCOLOR3FVEXTPROC __glewSecondaryColor3fvEXT;
extern PFNGLSECONDARYCOLOR3IEXTPROC __glewSecondaryColor3iEXT;
extern PFNGLSECONDARYCOLOR3IVEXTPROC __glewSecondaryColor3ivEXT;
extern PFNGLSECONDARYCOLOR3SEXTPROC __glewSecondaryColor3sEXT;
extern PFNGLSECONDARYCOLOR3SVEXTPROC __glewSecondaryColor3svEXT;
extern PFNGLSECONDARYCOLOR3UBEXTPROC __glewSecondaryColor3ubEXT;
extern PFNGLSECONDARYCOLOR3UBVEXTPROC __glewSecondaryColor3ubvEXT;
extern PFNGLSECONDARYCOLOR3UIEXTPROC __glewSecondaryColor3uiEXT;
extern PFNGLSECONDARYCOLOR3UIVEXTPROC __glewSecondaryColor3uivEXT;
extern PFNGLSECONDARYCOLOR3USEXTPROC __glewSecondaryColor3usEXT;
extern PFNGLSECONDARYCOLOR3USVEXTPROC __glewSecondaryColor3usvEXT;
extern PFNGLSECONDARYCOLORPOINTEREXTPROC __glewSecondaryColorPointerEXT;

extern PFNGLACTIVEPROGRAMEXTPROC __glewActiveProgramEXT;
extern PFNGLCREATESHADERPROGRAMEXTPROC __glewCreateShaderProgramEXT;
extern PFNGLUSESHADERPROGRAMEXTPROC __glewUseShaderProgramEXT;

extern PFNGLBINDIMAGETEXTUREEXTPROC __glewBindImageTextureEXT;
extern PFNGLMEMORYBARRIEREXTPROC __glewMemoryBarrierEXT;

extern PFNGLACTIVESTENCILFACEEXTPROC __glewActiveStencilFaceEXT;

extern PFNGLTEXSUBIMAGE1DEXTPROC __glewTexSubImage1DEXT;
extern PFNGLTEXSUBIMAGE2DEXTPROC __glewTexSubImage2DEXT;
extern PFNGLTEXSUBIMAGE3DEXTPROC __glewTexSubImage3DEXT;

extern PFNGLTEXIMAGE3DEXTPROC __glewTexImage3DEXT;

extern PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC __glewFramebufferTextureLayerEXT;

extern PFNGLTEXBUFFEREXTPROC __glewTexBufferEXT;

extern PFNGLCLEARCOLORIIEXTPROC __glewClearColorIiEXT;
extern PFNGLCLEARCOLORIUIEXTPROC __glewClearColorIuiEXT;
extern PFNGLGETTEXPARAMETERIIVEXTPROC __glewGetTexParameterIivEXT;
extern PFNGLGETTEXPARAMETERIUIVEXTPROC __glewGetTexParameterIuivEXT;
extern PFNGLTEXPARAMETERIIVEXTPROC __glewTexParameterIivEXT;
extern PFNGLTEXPARAMETERIUIVEXTPROC __glewTexParameterIuivEXT;

extern PFNGLARETEXTURESRESIDENTEXTPROC __glewAreTexturesResidentEXT;
extern PFNGLBINDTEXTUREEXTPROC __glewBindTextureEXT;
extern PFNGLDELETETEXTURESEXTPROC __glewDeleteTexturesEXT;
extern PFNGLGENTEXTURESEXTPROC __glewGenTexturesEXT;
extern PFNGLISTEXTUREEXTPROC __glewIsTextureEXT;
extern PFNGLPRIORITIZETEXTURESEXTPROC __glewPrioritizeTexturesEXT;

extern PFNGLTEXTURENORMALEXTPROC __glewTextureNormalEXT;

extern PFNGLGETQUERYOBJECTI64VEXTPROC __glewGetQueryObjecti64vEXT;
extern PFNGLGETQUERYOBJECTUI64VEXTPROC __glewGetQueryObjectui64vEXT;

extern PFNGLBEGINTRANSFORMFEEDBACKEXTPROC __glewBeginTransformFeedbackEXT;
extern PFNGLBINDBUFFERBASEEXTPROC __glewBindBufferBaseEXT;
extern PFNGLBINDBUFFEROFFSETEXTPROC __glewBindBufferOffsetEXT;
extern PFNGLBINDBUFFERRANGEEXTPROC __glewBindBufferRangeEXT;
extern PFNGLENDTRANSFORMFEEDBACKEXTPROC __glewEndTransformFeedbackEXT;
extern PFNGLGETTRANSFORMFEEDBACKVARYINGEXTPROC __glewGetTransformFeedbackVaryingEXT;
extern PFNGLTRANSFORMFEEDBACKVARYINGSEXTPROC __glewTransformFeedbackVaryingsEXT;

extern PFNGLARRAYELEMENTEXTPROC __glewArrayElementEXT;
extern PFNGLCOLORPOINTEREXTPROC __glewColorPointerEXT;
extern PFNGLDRAWARRAYSEXTPROC __glewDrawArraysEXT;
extern PFNGLEDGEFLAGPOINTEREXTPROC __glewEdgeFlagPointerEXT;
extern PFNGLINDEXPOINTEREXTPROC __glewIndexPointerEXT;
extern PFNGLNORMALPOINTEREXTPROC __glewNormalPointerEXT;
extern PFNGLTEXCOORDPOINTEREXTPROC __glewTexCoordPointerEXT;
extern PFNGLVERTEXPOINTEREXTPROC __glewVertexPointerEXT;

extern PFNGLGETVERTEXATTRIBLDVEXTPROC __glewGetVertexAttribLdvEXT;
extern PFNGLVERTEXARRAYVERTEXATTRIBLOFFSETEXTPROC __glewVertexArrayVertexAttribLOffsetEXT;
extern PFNGLVERTEXATTRIBL1DEXTPROC __glewVertexAttribL1dEXT;
extern PFNGLVERTEXATTRIBL1DVEXTPROC __glewVertexAttribL1dvEXT;
extern PFNGLVERTEXATTRIBL2DEXTPROC __glewVertexAttribL2dEXT;
extern PFNGLVERTEXATTRIBL2DVEXTPROC __glewVertexAttribL2dvEXT;
extern PFNGLVERTEXATTRIBL3DEXTPROC __glewVertexAttribL3dEXT;
extern PFNGLVERTEXATTRIBL3DVEXTPROC __glewVertexAttribL3dvEXT;
extern PFNGLVERTEXATTRIBL4DEXTPROC __glewVertexAttribL4dEXT;
extern PFNGLVERTEXATTRIBL4DVEXTPROC __glewVertexAttribL4dvEXT;
extern PFNGLVERTEXATTRIBLPOINTEREXTPROC __glewVertexAttribLPointerEXT;

extern PFNGLBEGINVERTEXSHADEREXTPROC __glewBeginVertexShaderEXT;
extern PFNGLBINDLIGHTPARAMETEREXTPROC __glewBindLightParameterEXT;
extern PFNGLBINDMATERIALPARAMETEREXTPROC __glewBindMaterialParameterEXT;
extern PFNGLBINDPARAMETEREXTPROC __glewBindParameterEXT;
extern PFNGLBINDTEXGENPARAMETEREXTPROC __glewBindTexGenParameterEXT;
extern PFNGLBINDTEXTUREUNITPARAMETEREXTPROC __glewBindTextureUnitParameterEXT;
extern PFNGLBINDVERTEXSHADEREXTPROC __glewBindVertexShaderEXT;
extern PFNGLDELETEVERTEXSHADEREXTPROC __glewDeleteVertexShaderEXT;
extern PFNGLDISABLEVARIANTCLIENTSTATEEXTPROC __glewDisableVariantClientStateEXT;
extern PFNGLENABLEVARIANTCLIENTSTATEEXTPROC __glewEnableVariantClientStateEXT;
extern PFNGLENDVERTEXSHADEREXTPROC __glewEndVertexShaderEXT;
extern PFNGLEXTRACTCOMPONENTEXTPROC __glewExtractComponentEXT;
extern PFNGLGENSYMBOLSEXTPROC __glewGenSymbolsEXT;
extern PFNGLGENVERTEXSHADERSEXTPROC __glewGenVertexShadersEXT;
extern PFNGLGETINVARIANTBOOLEANVEXTPROC __glewGetInvariantBooleanvEXT;
extern PFNGLGETINVARIANTFLOATVEXTPROC __glewGetInvariantFloatvEXT;
extern PFNGLGETINVARIANTINTEGERVEXTPROC __glewGetInvariantIntegervEXT;
extern PFNGLGETLOCALCONSTANTBOOLEANVEXTPROC __glewGetLocalConstantBooleanvEXT;
extern PFNGLGETLOCALCONSTANTFLOATVEXTPROC __glewGetLocalConstantFloatvEXT;
extern PFNGLGETLOCALCONSTANTINTEGERVEXTPROC __glewGetLocalConstantIntegervEXT;
extern PFNGLGETVARIANTBOOLEANVEXTPROC __glewGetVariantBooleanvEXT;
extern PFNGLGETVARIANTFLOATVEXTPROC __glewGetVariantFloatvEXT;
extern PFNGLGETVARIANTINTEGERVEXTPROC __glewGetVariantIntegervEXT;
extern PFNGLGETVARIANTPOINTERVEXTPROC __glewGetVariantPointervEXT;
extern PFNGLINSERTCOMPONENTEXTPROC __glewInsertComponentEXT;
extern PFNGLISVARIANTENABLEDEXTPROC __glewIsVariantEnabledEXT;
extern PFNGLSETINVARIANTEXTPROC __glewSetInvariantEXT;
extern PFNGLSETLOCALCONSTANTEXTPROC __glewSetLocalConstantEXT;
extern PFNGLSHADEROP1EXTPROC __glewShaderOp1EXT;
extern PFNGLSHADEROP2EXTPROC __glewShaderOp2EXT;
extern PFNGLSHADEROP3EXTPROC __glewShaderOp3EXT;
extern PFNGLSWIZZLEEXTPROC __glewSwizzleEXT;
extern PFNGLVARIANTPOINTEREXTPROC __glewVariantPointerEXT;
extern PFNGLVARIANTBVEXTPROC __glewVariantbvEXT;
extern PFNGLVARIANTDVEXTPROC __glewVariantdvEXT;
extern PFNGLVARIANTFVEXTPROC __glewVariantfvEXT;
extern PFNGLVARIANTIVEXTPROC __glewVariantivEXT;
extern PFNGLVARIANTSVEXTPROC __glewVariantsvEXT;
extern PFNGLVARIANTUBVEXTPROC __glewVariantubvEXT;
extern PFNGLVARIANTUIVEXTPROC __glewVariantuivEXT;
extern PFNGLVARIANTUSVEXTPROC __glewVariantusvEXT;
extern PFNGLWRITEMASKEXTPROC __glewWriteMaskEXT;

extern PFNGLVERTEXWEIGHTPOINTEREXTPROC __glewVertexWeightPointerEXT;
extern PFNGLVERTEXWEIGHTFEXTPROC __glewVertexWeightfEXT;
extern PFNGLVERTEXWEIGHTFVEXTPROC __glewVertexWeightfvEXT;

extern PFNGLIMPORTSYNCEXTPROC __glewImportSyncEXT;

extern PFNGLFRAMETERMINATORGREMEDYPROC __glewFrameTerminatorGREMEDY;

extern PFNGLSTRINGMARKERGREMEDYPROC __glewStringMarkerGREMEDY;

extern PFNGLGETIMAGETRANSFORMPARAMETERFVHPPROC __glewGetImageTransformParameterfvHP;
extern PFNGLGETIMAGETRANSFORMPARAMETERIVHPPROC __glewGetImageTransformParameterivHP;
extern PFNGLIMAGETRANSFORMPARAMETERFHPPROC __glewImageTransformParameterfHP;
extern PFNGLIMAGETRANSFORMPARAMETERFVHPPROC __glewImageTransformParameterfvHP;
extern PFNGLIMAGETRANSFORMPARAMETERIHPPROC __glewImageTransformParameteriHP;
extern PFNGLIMAGETRANSFORMPARAMETERIVHPPROC __glewImageTransformParameterivHP;

extern PFNGLMULTIMODEDRAWARRAYSIBMPROC __glewMultiModeDrawArraysIBM;
extern PFNGLMULTIMODEDRAWELEMENTSIBMPROC __glewMultiModeDrawElementsIBM;

extern PFNGLCOLORPOINTERLISTIBMPROC __glewColorPointerListIBM;
extern PFNGLEDGEFLAGPOINTERLISTIBMPROC __glewEdgeFlagPointerListIBM;
extern PFNGLFOGCOORDPOINTERLISTIBMPROC __glewFogCoordPointerListIBM;
extern PFNGLINDEXPOINTERLISTIBMPROC __glewIndexPointerListIBM;
extern PFNGLNORMALPOINTERLISTIBMPROC __glewNormalPointerListIBM;
extern PFNGLSECONDARYCOLORPOINTERLISTIBMPROC __glewSecondaryColorPointerListIBM;
extern PFNGLTEXCOORDPOINTERLISTIBMPROC __glewTexCoordPointerListIBM;
extern PFNGLVERTEXPOINTERLISTIBMPROC __glewVertexPointerListIBM;

extern PFNGLCOLORPOINTERVINTELPROC __glewColorPointervINTEL;
extern PFNGLNORMALPOINTERVINTELPROC __glewNormalPointervINTEL;
extern PFNGLTEXCOORDPOINTERVINTELPROC __glewTexCoordPointervINTEL;
extern PFNGLVERTEXPOINTERVINTELPROC __glewVertexPointervINTEL;

extern PFNGLTEXSCISSORFUNCINTELPROC __glewTexScissorFuncINTEL;
extern PFNGLTEXSCISSORINTELPROC __glewTexScissorINTEL;

extern PFNGLBUFFERREGIONENABLEDPROC __glewBufferRegionEnabled;
extern PFNGLDELETEBUFFERREGIONPROC __glewDeleteBufferRegion;
extern PFNGLDRAWBUFFERREGIONPROC __glewDrawBufferRegion;
extern PFNGLNEWBUFFERREGIONPROC __glewNewBufferRegion;
extern PFNGLREADBUFFERREGIONPROC __glewReadBufferRegion;

extern PFNGLRESIZEBUFFERSMESAPROC __glewResizeBuffersMESA;

extern PFNGLWINDOWPOS2DMESAPROC __glewWindowPos2dMESA;
extern PFNGLWINDOWPOS2DVMESAPROC __glewWindowPos2dvMESA;
extern PFNGLWINDOWPOS2FMESAPROC __glewWindowPos2fMESA;
extern PFNGLWINDOWPOS2FVMESAPROC __glewWindowPos2fvMESA;
extern PFNGLWINDOWPOS2IMESAPROC __glewWindowPos2iMESA;
extern PFNGLWINDOWPOS2IVMESAPROC __glewWindowPos2ivMESA;
extern PFNGLWINDOWPOS2SMESAPROC __glewWindowPos2sMESA;
extern PFNGLWINDOWPOS2SVMESAPROC __glewWindowPos2svMESA;
extern PFNGLWINDOWPOS3DMESAPROC __glewWindowPos3dMESA;
extern PFNGLWINDOWPOS3DVMESAPROC __glewWindowPos3dvMESA;
extern PFNGLWINDOWPOS3FMESAPROC __glewWindowPos3fMESA;
extern PFNGLWINDOWPOS3FVMESAPROC __glewWindowPos3fvMESA;
extern PFNGLWINDOWPOS3IMESAPROC __glewWindowPos3iMESA;
extern PFNGLWINDOWPOS3IVMESAPROC __glewWindowPos3ivMESA;
extern PFNGLWINDOWPOS3SMESAPROC __glewWindowPos3sMESA;
extern PFNGLWINDOWPOS3SVMESAPROC __glewWindowPos3svMESA;
extern PFNGLWINDOWPOS4DMESAPROC __glewWindowPos4dMESA;
extern PFNGLWINDOWPOS4DVMESAPROC __glewWindowPos4dvMESA;
extern PFNGLWINDOWPOS4FMESAPROC __glewWindowPos4fMESA;
extern PFNGLWINDOWPOS4FVMESAPROC __glewWindowPos4fvMESA;
extern PFNGLWINDOWPOS4IMESAPROC __glewWindowPos4iMESA;
extern PFNGLWINDOWPOS4IVMESAPROC __glewWindowPos4ivMESA;
extern PFNGLWINDOWPOS4SMESAPROC __glewWindowPos4sMESA;
extern PFNGLWINDOWPOS4SVMESAPROC __glewWindowPos4svMESA;

extern PFNGLBEGINCONDITIONALRENDERNVPROC __glewBeginConditionalRenderNV;
extern PFNGLENDCONDITIONALRENDERNVPROC __glewEndConditionalRenderNV;

extern PFNGLCOPYIMAGESUBDATANVPROC __glewCopyImageSubDataNV;

extern PFNGLCLEARDEPTHDNVPROC __glewClearDepthdNV;
extern PFNGLDEPTHBOUNDSDNVPROC __glewDepthBoundsdNV;
extern PFNGLDEPTHRANGEDNVPROC __glewDepthRangedNV;

extern PFNGLEVALMAPSNVPROC __glewEvalMapsNV;
extern PFNGLGETMAPATTRIBPARAMETERFVNVPROC __glewGetMapAttribParameterfvNV;
extern PFNGLGETMAPATTRIBPARAMETERIVNVPROC __glewGetMapAttribParameterivNV;
extern PFNGLGETMAPCONTROLPOINTSNVPROC __glewGetMapControlPointsNV;
extern PFNGLGETMAPPARAMETERFVNVPROC __glewGetMapParameterfvNV;
extern PFNGLGETMAPPARAMETERIVNVPROC __glewGetMapParameterivNV;
extern PFNGLMAPCONTROLPOINTSNVPROC __glewMapControlPointsNV;
extern PFNGLMAPPARAMETERFVNVPROC __glewMapParameterfvNV;
extern PFNGLMAPPARAMETERIVNVPROC __glewMapParameterivNV;

extern PFNGLGETMULTISAMPLEFVNVPROC __glewGetMultisamplefvNV;
extern PFNGLSAMPLEMASKINDEXEDNVPROC __glewSampleMaskIndexedNV;
extern PFNGLTEXRENDERBUFFERNVPROC __glewTexRenderbufferNV;

extern PFNGLDELETEFENCESNVPROC __glewDeleteFencesNV;
extern PFNGLFINISHFENCENVPROC __glewFinishFenceNV;
extern PFNGLGENFENCESNVPROC __glewGenFencesNV;
extern PFNGLGETFENCEIVNVPROC __glewGetFenceivNV;
extern PFNGLISFENCENVPROC __glewIsFenceNV;
extern PFNGLSETFENCENVPROC __glewSetFenceNV;
extern PFNGLTESTFENCENVPROC __glewTestFenceNV;

extern PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC __glewGetProgramNamedParameterdvNV;
extern PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC __glewGetProgramNamedParameterfvNV;
extern PFNGLPROGRAMNAMEDPARAMETER4DNVPROC __glewProgramNamedParameter4dNV;
extern PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC __glewProgramNamedParameter4dvNV;
extern PFNGLPROGRAMNAMEDPARAMETER4FNVPROC __glewProgramNamedParameter4fNV;
extern PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC __glewProgramNamedParameter4fvNV;

extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC __glewRenderbufferStorageMultisampleCoverageNV;

extern PFNGLPROGRAMVERTEXLIMITNVPROC __glewProgramVertexLimitNV;

extern PFNGLPROGRAMENVPARAMETERI4INVPROC __glewProgramEnvParameterI4iNV;
extern PFNGLPROGRAMENVPARAMETERI4IVNVPROC __glewProgramEnvParameterI4ivNV;
extern PFNGLPROGRAMENVPARAMETERI4UINVPROC __glewProgramEnvParameterI4uiNV;
extern PFNGLPROGRAMENVPARAMETERI4UIVNVPROC __glewProgramEnvParameterI4uivNV;
extern PFNGLPROGRAMENVPARAMETERSI4IVNVPROC __glewProgramEnvParametersI4ivNV;
extern PFNGLPROGRAMENVPARAMETERSI4UIVNVPROC __glewProgramEnvParametersI4uivNV;
extern PFNGLPROGRAMLOCALPARAMETERI4INVPROC __glewProgramLocalParameterI4iNV;
extern PFNGLPROGRAMLOCALPARAMETERI4IVNVPROC __glewProgramLocalParameterI4ivNV;
extern PFNGLPROGRAMLOCALPARAMETERI4UINVPROC __glewProgramLocalParameterI4uiNV;
extern PFNGLPROGRAMLOCALPARAMETERI4UIVNVPROC __glewProgramLocalParameterI4uivNV;
extern PFNGLPROGRAMLOCALPARAMETERSI4IVNVPROC __glewProgramLocalParametersI4ivNV;
extern PFNGLPROGRAMLOCALPARAMETERSI4UIVNVPROC __glewProgramLocalParametersI4uivNV;

extern PFNGLGETUNIFORMI64VNVPROC __glewGetUniformi64vNV;
extern PFNGLGETUNIFORMUI64VNVPROC __glewGetUniformui64vNV;
extern PFNGLPROGRAMUNIFORM1I64NVPROC __glewProgramUniform1i64NV;
extern PFNGLPROGRAMUNIFORM1I64VNVPROC __glewProgramUniform1i64vNV;
extern PFNGLPROGRAMUNIFORM1UI64NVPROC __glewProgramUniform1ui64NV;
extern PFNGLPROGRAMUNIFORM1UI64VNVPROC __glewProgramUniform1ui64vNV;
extern PFNGLPROGRAMUNIFORM2I64NVPROC __glewProgramUniform2i64NV;
extern PFNGLPROGRAMUNIFORM2I64VNVPROC __glewProgramUniform2i64vNV;
extern PFNGLPROGRAMUNIFORM2UI64NVPROC __glewProgramUniform2ui64NV;
extern PFNGLPROGRAMUNIFORM2UI64VNVPROC __glewProgramUniform2ui64vNV;
extern PFNGLPROGRAMUNIFORM3I64NVPROC __glewProgramUniform3i64NV;
extern PFNGLPROGRAMUNIFORM3I64VNVPROC __glewProgramUniform3i64vNV;
extern PFNGLPROGRAMUNIFORM3UI64NVPROC __glewProgramUniform3ui64NV;
extern PFNGLPROGRAMUNIFORM3UI64VNVPROC __glewProgramUniform3ui64vNV;
extern PFNGLPROGRAMUNIFORM4I64NVPROC __glewProgramUniform4i64NV;
extern PFNGLPROGRAMUNIFORM4I64VNVPROC __glewProgramUniform4i64vNV;
extern PFNGLPROGRAMUNIFORM4UI64NVPROC __glewProgramUniform4ui64NV;
extern PFNGLPROGRAMUNIFORM4UI64VNVPROC __glewProgramUniform4ui64vNV;
extern PFNGLUNIFORM1I64NVPROC __glewUniform1i64NV;
extern PFNGLUNIFORM1I64VNVPROC __glewUniform1i64vNV;
extern PFNGLUNIFORM1UI64NVPROC __glewUniform1ui64NV;
extern PFNGLUNIFORM1UI64VNVPROC __glewUniform1ui64vNV;
extern PFNGLUNIFORM2I64NVPROC __glewUniform2i64NV;
extern PFNGLUNIFORM2I64VNVPROC __glewUniform2i64vNV;
extern PFNGLUNIFORM2UI64NVPROC __glewUniform2ui64NV;
extern PFNGLUNIFORM2UI64VNVPROC __glewUniform2ui64vNV;
extern PFNGLUNIFORM3I64NVPROC __glewUniform3i64NV;
extern PFNGLUNIFORM3I64VNVPROC __glewUniform3i64vNV;
extern PFNGLUNIFORM3UI64NVPROC __glewUniform3ui64NV;
extern PFNGLUNIFORM3UI64VNVPROC __glewUniform3ui64vNV;
extern PFNGLUNIFORM4I64NVPROC __glewUniform4i64NV;
extern PFNGLUNIFORM4I64VNVPROC __glewUniform4i64vNV;
extern PFNGLUNIFORM4UI64NVPROC __glewUniform4ui64NV;
extern PFNGLUNIFORM4UI64VNVPROC __glewUniform4ui64vNV;

extern PFNGLCOLOR3HNVPROC __glewColor3hNV;
extern PFNGLCOLOR3HVNVPROC __glewColor3hvNV;
extern PFNGLCOLOR4HNVPROC __glewColor4hNV;
extern PFNGLCOLOR4HVNVPROC __glewColor4hvNV;
extern PFNGLFOGCOORDHNVPROC __glewFogCoordhNV;
extern PFNGLFOGCOORDHVNVPROC __glewFogCoordhvNV;
extern PFNGLMULTITEXCOORD1HNVPROC __glewMultiTexCoord1hNV;
extern PFNGLMULTITEXCOORD1HVNVPROC __glewMultiTexCoord1hvNV;
extern PFNGLMULTITEXCOORD2HNVPROC __glewMultiTexCoord2hNV;
extern PFNGLMULTITEXCOORD2HVNVPROC __glewMultiTexCoord2hvNV;
extern PFNGLMULTITEXCOORD3HNVPROC __glewMultiTexCoord3hNV;
extern PFNGLMULTITEXCOORD3HVNVPROC __glewMultiTexCoord3hvNV;
extern PFNGLMULTITEXCOORD4HNVPROC __glewMultiTexCoord4hNV;
extern PFNGLMULTITEXCOORD4HVNVPROC __glewMultiTexCoord4hvNV;
extern PFNGLNORMAL3HNVPROC __glewNormal3hNV;
extern PFNGLNORMAL3HVNVPROC __glewNormal3hvNV;
extern PFNGLSECONDARYCOLOR3HNVPROC __glewSecondaryColor3hNV;
extern PFNGLSECONDARYCOLOR3HVNVPROC __glewSecondaryColor3hvNV;
extern PFNGLTEXCOORD1HNVPROC __glewTexCoord1hNV;
extern PFNGLTEXCOORD1HVNVPROC __glewTexCoord1hvNV;
extern PFNGLTEXCOORD2HNVPROC __glewTexCoord2hNV;
extern PFNGLTEXCOORD2HVNVPROC __glewTexCoord2hvNV;
extern PFNGLTEXCOORD3HNVPROC __glewTexCoord3hNV;
extern PFNGLTEXCOORD3HVNVPROC __glewTexCoord3hvNV;
extern PFNGLTEXCOORD4HNVPROC __glewTexCoord4hNV;
extern PFNGLTEXCOORD4HVNVPROC __glewTexCoord4hvNV;
extern PFNGLVERTEX2HNVPROC __glewVertex2hNV;
extern PFNGLVERTEX2HVNVPROC __glewVertex2hvNV;
extern PFNGLVERTEX3HNVPROC __glewVertex3hNV;
extern PFNGLVERTEX3HVNVPROC __glewVertex3hvNV;
extern PFNGLVERTEX4HNVPROC __glewVertex4hNV;
extern PFNGLVERTEX4HVNVPROC __glewVertex4hvNV;
extern PFNGLVERTEXATTRIB1HNVPROC __glewVertexAttrib1hNV;
extern PFNGLVERTEXATTRIB1HVNVPROC __glewVertexAttrib1hvNV;
extern PFNGLVERTEXATTRIB2HNVPROC __glewVertexAttrib2hNV;
extern PFNGLVERTEXATTRIB2HVNVPROC __glewVertexAttrib2hvNV;
extern PFNGLVERTEXATTRIB3HNVPROC __glewVertexAttrib3hNV;
extern PFNGLVERTEXATTRIB3HVNVPROC __glewVertexAttrib3hvNV;
extern PFNGLVERTEXATTRIB4HNVPROC __glewVertexAttrib4hNV;
extern PFNGLVERTEXATTRIB4HVNVPROC __glewVertexAttrib4hvNV;
extern PFNGLVERTEXATTRIBS1HVNVPROC __glewVertexAttribs1hvNV;
extern PFNGLVERTEXATTRIBS2HVNVPROC __glewVertexAttribs2hvNV;
extern PFNGLVERTEXATTRIBS3HVNVPROC __glewVertexAttribs3hvNV;
extern PFNGLVERTEXATTRIBS4HVNVPROC __glewVertexAttribs4hvNV;
extern PFNGLVERTEXWEIGHTHNVPROC __glewVertexWeighthNV;
extern PFNGLVERTEXWEIGHTHVNVPROC __glewVertexWeighthvNV;

extern PFNGLBEGINOCCLUSIONQUERYNVPROC __glewBeginOcclusionQueryNV;
extern PFNGLDELETEOCCLUSIONQUERIESNVPROC __glewDeleteOcclusionQueriesNV;
extern PFNGLENDOCCLUSIONQUERYNVPROC __glewEndOcclusionQueryNV;
extern PFNGLGENOCCLUSIONQUERIESNVPROC __glewGenOcclusionQueriesNV;
extern PFNGLGETOCCLUSIONQUERYIVNVPROC __glewGetOcclusionQueryivNV;
extern PFNGLGETOCCLUSIONQUERYUIVNVPROC __glewGetOcclusionQueryuivNV;
extern PFNGLISOCCLUSIONQUERYNVPROC __glewIsOcclusionQueryNV;

extern PFNGLPROGRAMBUFFERPARAMETERSIIVNVPROC __glewProgramBufferParametersIivNV;
extern PFNGLPROGRAMBUFFERPARAMETERSIUIVNVPROC __glewProgramBufferParametersIuivNV;
extern PFNGLPROGRAMBUFFERPARAMETERSFVNVPROC __glewProgramBufferParametersfvNV;

extern PFNGLCOPYPATHNVPROC __glewCopyPathNV;
extern PFNGLCOVERFILLPATHINSTANCEDNVPROC __glewCoverFillPathInstancedNV;
extern PFNGLCOVERFILLPATHNVPROC __glewCoverFillPathNV;
extern PFNGLCOVERSTROKEPATHINSTANCEDNVPROC __glewCoverStrokePathInstancedNV;
extern PFNGLCOVERSTROKEPATHNVPROC __glewCoverStrokePathNV;
extern PFNGLDELETEPATHSNVPROC __glewDeletePathsNV;
extern PFNGLGENPATHSNVPROC __glewGenPathsNV;
extern PFNGLGETPATHCOLORGENFVNVPROC __glewGetPathColorGenfvNV;
extern PFNGLGETPATHCOLORGENIVNVPROC __glewGetPathColorGenivNV;
extern PFNGLGETPATHCOMMANDSNVPROC __glewGetPathCommandsNV;
extern PFNGLGETPATHCOORDSNVPROC __glewGetPathCoordsNV;
extern PFNGLGETPATHDASHARRAYNVPROC __glewGetPathDashArrayNV;
extern PFNGLGETPATHLENGTHNVPROC __glewGetPathLengthNV;
extern PFNGLGETPATHMETRICRANGENVPROC __glewGetPathMetricRangeNV;
extern PFNGLGETPATHMETRICSNVPROC __glewGetPathMetricsNV;
extern PFNGLGETPATHPARAMETERFVNVPROC __glewGetPathParameterfvNV;
extern PFNGLGETPATHPARAMETERIVNVPROC __glewGetPathParameterivNV;
extern PFNGLGETPATHSPACINGNVPROC __glewGetPathSpacingNV;
extern PFNGLGETPATHTEXGENFVNVPROC __glewGetPathTexGenfvNV;
extern PFNGLGETPATHTEXGENIVNVPROC __glewGetPathTexGenivNV;
extern PFNGLINTERPOLATEPATHSNVPROC __glewInterpolatePathsNV;
extern PFNGLISPATHNVPROC __glewIsPathNV;
extern PFNGLISPOINTINFILLPATHNVPROC __glewIsPointInFillPathNV;
extern PFNGLISPOINTINSTROKEPATHNVPROC __glewIsPointInStrokePathNV;
extern PFNGLPATHCOLORGENNVPROC __glewPathColorGenNV;
extern PFNGLPATHCOMMANDSNVPROC __glewPathCommandsNV;
extern PFNGLPATHCOORDSNVPROC __glewPathCoordsNV;
extern PFNGLPATHCOVERDEPTHFUNCNVPROC __glewPathCoverDepthFuncNV;
extern PFNGLPATHDASHARRAYNVPROC __glewPathDashArrayNV;
extern PFNGLPATHFOGGENNVPROC __glewPathFogGenNV;
extern PFNGLPATHGLYPHRANGENVPROC __glewPathGlyphRangeNV;
extern PFNGLPATHGLYPHSNVPROC __glewPathGlyphsNV;
extern PFNGLPATHPARAMETERFNVPROC __glewPathParameterfNV;
extern PFNGLPATHPARAMETERFVNVPROC __glewPathParameterfvNV;
extern PFNGLPATHPARAMETERINVPROC __glewPathParameteriNV;
extern PFNGLPATHPARAMETERIVNVPROC __glewPathParameterivNV;
extern PFNGLPATHSTENCILDEPTHOFFSETNVPROC __glewPathStencilDepthOffsetNV;
extern PFNGLPATHSTENCILFUNCNVPROC __glewPathStencilFuncNV;
extern PFNGLPATHSTRINGNVPROC __glewPathStringNV;
extern PFNGLPATHSUBCOMMANDSNVPROC __glewPathSubCommandsNV;
extern PFNGLPATHSUBCOORDSNVPROC __glewPathSubCoordsNV;
extern PFNGLPATHTEXGENNVPROC __glewPathTexGenNV;
extern PFNGLPOINTALONGPATHNVPROC __glewPointAlongPathNV;
extern PFNGLSTENCILFILLPATHINSTANCEDNVPROC __glewStencilFillPathInstancedNV;
extern PFNGLSTENCILFILLPATHNVPROC __glewStencilFillPathNV;
extern PFNGLSTENCILSTROKEPATHINSTANCEDNVPROC __glewStencilStrokePathInstancedNV;
extern PFNGLSTENCILSTROKEPATHNVPROC __glewStencilStrokePathNV;
extern PFNGLTRANSFORMPATHNVPROC __glewTransformPathNV;
extern PFNGLWEIGHTPATHSNVPROC __glewWeightPathsNV;

extern PFNGLFLUSHPIXELDATARANGENVPROC __glewFlushPixelDataRangeNV;
extern PFNGLPIXELDATARANGENVPROC __glewPixelDataRangeNV;

extern PFNGLPOINTPARAMETERINVPROC __glewPointParameteriNV;
extern PFNGLPOINTPARAMETERIVNVPROC __glewPointParameterivNV;

extern PFNGLGETVIDEOI64VNVPROC __glewGetVideoi64vNV;
extern PFNGLGETVIDEOIVNVPROC __glewGetVideoivNV;
extern PFNGLGETVIDEOUI64VNVPROC __glewGetVideoui64vNV;
extern PFNGLGETVIDEOUIVNVPROC __glewGetVideouivNV;
extern PFNGLPRESENTFRAMEDUALFILLNVPROC __glewPresentFrameDualFillNV;
extern PFNGLPRESENTFRAMEKEYEDNVPROC __glewPresentFrameKeyedNV;

extern PFNGLPRIMITIVERESTARTINDEXNVPROC __glewPrimitiveRestartIndexNV;
extern PFNGLPRIMITIVERESTARTNVPROC __glewPrimitiveRestartNV;

extern PFNGLCOMBINERINPUTNVPROC __glewCombinerInputNV;
extern PFNGLCOMBINEROUTPUTNVPROC __glewCombinerOutputNV;
extern PFNGLCOMBINERPARAMETERFNVPROC __glewCombinerParameterfNV;
extern PFNGLCOMBINERPARAMETERFVNVPROC __glewCombinerParameterfvNV;
extern PFNGLCOMBINERPARAMETERINVPROC __glewCombinerParameteriNV;
extern PFNGLCOMBINERPARAMETERIVNVPROC __glewCombinerParameterivNV;
extern PFNGLFINALCOMBINERINPUTNVPROC __glewFinalCombinerInputNV;
extern PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC __glewGetCombinerInputParameterfvNV;
extern PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC __glewGetCombinerInputParameterivNV;
extern PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC __glewGetCombinerOutputParameterfvNV;
extern PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC __glewGetCombinerOutputParameterivNV;
extern PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC __glewGetFinalCombinerInputParameterfvNV;
extern PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC __glewGetFinalCombinerInputParameterivNV;

extern PFNGLCOMBINERSTAGEPARAMETERFVNVPROC __glewCombinerStageParameterfvNV;
extern PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC __glewGetCombinerStageParameterfvNV;

extern PFNGLGETBUFFERPARAMETERUI64VNVPROC __glewGetBufferParameterui64vNV;
extern PFNGLGETINTEGERUI64VNVPROC __glewGetIntegerui64vNV;
extern PFNGLGETNAMEDBUFFERPARAMETERUI64VNVPROC __glewGetNamedBufferParameterui64vNV;
extern PFNGLISBUFFERRESIDENTNVPROC __glewIsBufferResidentNV;
extern PFNGLISNAMEDBUFFERRESIDENTNVPROC __glewIsNamedBufferResidentNV;
extern PFNGLMAKEBUFFERNONRESIDENTNVPROC __glewMakeBufferNonResidentNV;
extern PFNGLMAKEBUFFERRESIDENTNVPROC __glewMakeBufferResidentNV;
extern PFNGLMAKENAMEDBUFFERNONRESIDENTNVPROC __glewMakeNamedBufferNonResidentNV;
extern PFNGLMAKENAMEDBUFFERRESIDENTNVPROC __glewMakeNamedBufferResidentNV;
extern PFNGLPROGRAMUNIFORMUI64NVPROC __glewProgramUniformui64NV;
extern PFNGLPROGRAMUNIFORMUI64VNVPROC __glewProgramUniformui64vNV;
extern PFNGLUNIFORMUI64NVPROC __glewUniformui64NV;
extern PFNGLUNIFORMUI64VNVPROC __glewUniformui64vNV;

extern PFNGLTEXTUREBARRIERNVPROC __glewTextureBarrierNV;

extern PFNGLTEXIMAGE2DMULTISAMPLECOVERAGENVPROC __glewTexImage2DMultisampleCoverageNV;
extern PFNGLTEXIMAGE3DMULTISAMPLECOVERAGENVPROC __glewTexImage3DMultisampleCoverageNV;
extern PFNGLTEXTUREIMAGE2DMULTISAMPLECOVERAGENVPROC __glewTextureImage2DMultisampleCoverageNV;
extern PFNGLTEXTUREIMAGE2DMULTISAMPLENVPROC __glewTextureImage2DMultisampleNV;
extern PFNGLTEXTUREIMAGE3DMULTISAMPLECOVERAGENVPROC __glewTextureImage3DMultisampleCoverageNV;
extern PFNGLTEXTUREIMAGE3DMULTISAMPLENVPROC __glewTextureImage3DMultisampleNV;

extern PFNGLACTIVEVARYINGNVPROC __glewActiveVaryingNV;
extern PFNGLBEGINTRANSFORMFEEDBACKNVPROC __glewBeginTransformFeedbackNV;
extern PFNGLBINDBUFFERBASENVPROC __glewBindBufferBaseNV;
extern PFNGLBINDBUFFEROFFSETNVPROC __glewBindBufferOffsetNV;
extern PFNGLBINDBUFFERRANGENVPROC __glewBindBufferRangeNV;
extern PFNGLENDTRANSFORMFEEDBACKNVPROC __glewEndTransformFeedbackNV;
extern PFNGLGETACTIVEVARYINGNVPROC __glewGetActiveVaryingNV;
extern PFNGLGETTRANSFORMFEEDBACKVARYINGNVPROC __glewGetTransformFeedbackVaryingNV;
extern PFNGLGETVARYINGLOCATIONNVPROC __glewGetVaryingLocationNV;
extern PFNGLTRANSFORMFEEDBACKATTRIBSNVPROC __glewTransformFeedbackAttribsNV;
extern PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC __glewTransformFeedbackVaryingsNV;

extern PFNGLBINDTRANSFORMFEEDBACKNVPROC __glewBindTransformFeedbackNV;
extern PFNGLDELETETRANSFORMFEEDBACKSNVPROC __glewDeleteTransformFeedbacksNV;
extern PFNGLDRAWTRANSFORMFEEDBACKNVPROC __glewDrawTransformFeedbackNV;
extern PFNGLGENTRANSFORMFEEDBACKSNVPROC __glewGenTransformFeedbacksNV;
extern PFNGLISTRANSFORMFEEDBACKNVPROC __glewIsTransformFeedbackNV;
extern PFNGLPAUSETRANSFORMFEEDBACKNVPROC __glewPauseTransformFeedbackNV;
extern PFNGLRESUMETRANSFORMFEEDBACKNVPROC __glewResumeTransformFeedbackNV;

extern PFNGLVDPAUFININVPROC __glewVDPAUFiniNV;
extern PFNGLVDPAUGETSURFACEIVNVPROC __glewVDPAUGetSurfaceivNV;
extern PFNGLVDPAUINITNVPROC __glewVDPAUInitNV;
extern PFNGLVDPAUISSURFACENVPROC __glewVDPAUIsSurfaceNV;
extern PFNGLVDPAUMAPSURFACESNVPROC __glewVDPAUMapSurfacesNV;
extern PFNGLVDPAUREGISTEROUTPUTSURFACENVPROC __glewVDPAURegisterOutputSurfaceNV;
extern PFNGLVDPAUREGISTERVIDEOSURFACENVPROC __glewVDPAURegisterVideoSurfaceNV;
extern PFNGLVDPAUSURFACEACCESSNVPROC __glewVDPAUSurfaceAccessNV;
extern PFNGLVDPAUUNMAPSURFACESNVPROC __glewVDPAUUnmapSurfacesNV;
extern PFNGLVDPAUUNREGISTERSURFACENVPROC __glewVDPAUUnregisterSurfaceNV;

extern PFNGLFLUSHVERTEXARRAYRANGENVPROC __glewFlushVertexArrayRangeNV;
extern PFNGLVERTEXARRAYRANGENVPROC __glewVertexArrayRangeNV;

extern PFNGLGETVERTEXATTRIBLI64VNVPROC __glewGetVertexAttribLi64vNV;
extern PFNGLGETVERTEXATTRIBLUI64VNVPROC __glewGetVertexAttribLui64vNV;
extern PFNGLVERTEXATTRIBL1I64NVPROC __glewVertexAttribL1i64NV;
extern PFNGLVERTEXATTRIBL1I64VNVPROC __glewVertexAttribL1i64vNV;
extern PFNGLVERTEXATTRIBL1UI64NVPROC __glewVertexAttribL1ui64NV;
extern PFNGLVERTEXATTRIBL1UI64VNVPROC __glewVertexAttribL1ui64vNV;
extern PFNGLVERTEXATTRIBL2I64NVPROC __glewVertexAttribL2i64NV;
extern PFNGLVERTEXATTRIBL2I64VNVPROC __glewVertexAttribL2i64vNV;
extern PFNGLVERTEXATTRIBL2UI64NVPROC __glewVertexAttribL2ui64NV;
extern PFNGLVERTEXATTRIBL2UI64VNVPROC __glewVertexAttribL2ui64vNV;
extern PFNGLVERTEXATTRIBL3I64NVPROC __glewVertexAttribL3i64NV;
extern PFNGLVERTEXATTRIBL3I64VNVPROC __glewVertexAttribL3i64vNV;
extern PFNGLVERTEXATTRIBL3UI64NVPROC __glewVertexAttribL3ui64NV;
extern PFNGLVERTEXATTRIBL3UI64VNVPROC __glewVertexAttribL3ui64vNV;
extern PFNGLVERTEXATTRIBL4I64NVPROC __glewVertexAttribL4i64NV;
extern PFNGLVERTEXATTRIBL4I64VNVPROC __glewVertexAttribL4i64vNV;
extern PFNGLVERTEXATTRIBL4UI64NVPROC __glewVertexAttribL4ui64NV;
extern PFNGLVERTEXATTRIBL4UI64VNVPROC __glewVertexAttribL4ui64vNV;
extern PFNGLVERTEXATTRIBLFORMATNVPROC __glewVertexAttribLFormatNV;

extern PFNGLBUFFERADDRESSRANGENVPROC __glewBufferAddressRangeNV;
extern PFNGLCOLORFORMATNVPROC __glewColorFormatNV;
extern PFNGLEDGEFLAGFORMATNVPROC __glewEdgeFlagFormatNV;
extern PFNGLFOGCOORDFORMATNVPROC __glewFogCoordFormatNV;
extern PFNGLGETINTEGERUI64I_VNVPROC __glewGetIntegerui64i_vNV;
extern PFNGLINDEXFORMATNVPROC __glewIndexFormatNV;
extern PFNGLNORMALFORMATNVPROC __glewNormalFormatNV;
extern PFNGLSECONDARYCOLORFORMATNVPROC __glewSecondaryColorFormatNV;
extern PFNGLTEXCOORDFORMATNVPROC __glewTexCoordFormatNV;
extern PFNGLVERTEXATTRIBFORMATNVPROC __glewVertexAttribFormatNV;
extern PFNGLVERTEXATTRIBIFORMATNVPROC __glewVertexAttribIFormatNV;
extern PFNGLVERTEXFORMATNVPROC __glewVertexFormatNV;

extern PFNGLAREPROGRAMSRESIDENTNVPROC __glewAreProgramsResidentNV;
extern PFNGLBINDPROGRAMNVPROC __glewBindProgramNV;
extern PFNGLDELETEPROGRAMSNVPROC __glewDeleteProgramsNV;
extern PFNGLEXECUTEPROGRAMNVPROC __glewExecuteProgramNV;
extern PFNGLGENPROGRAMSNVPROC __glewGenProgramsNV;
extern PFNGLGETPROGRAMPARAMETERDVNVPROC __glewGetProgramParameterdvNV;
extern PFNGLGETPROGRAMPARAMETERFVNVPROC __glewGetProgramParameterfvNV;
extern PFNGLGETPROGRAMSTRINGNVPROC __glewGetProgramStringNV;
extern PFNGLGETPROGRAMIVNVPROC __glewGetProgramivNV;
extern PFNGLGETTRACKMATRIXIVNVPROC __glewGetTrackMatrixivNV;
extern PFNGLGETVERTEXATTRIBPOINTERVNVPROC __glewGetVertexAttribPointervNV;
extern PFNGLGETVERTEXATTRIBDVNVPROC __glewGetVertexAttribdvNV;
extern PFNGLGETVERTEXATTRIBFVNVPROC __glewGetVertexAttribfvNV;
extern PFNGLGETVERTEXATTRIBIVNVPROC __glewGetVertexAttribivNV;
extern PFNGLISPROGRAMNVPROC __glewIsProgramNV;
extern PFNGLLOADPROGRAMNVPROC __glewLoadProgramNV;
extern PFNGLPROGRAMPARAMETER4DNVPROC __glewProgramParameter4dNV;
extern PFNGLPROGRAMPARAMETER4DVNVPROC __glewProgramParameter4dvNV;
extern PFNGLPROGRAMPARAMETER4FNVPROC __glewProgramParameter4fNV;
extern PFNGLPROGRAMPARAMETER4FVNVPROC __glewProgramParameter4fvNV;
extern PFNGLPROGRAMPARAMETERS4DVNVPROC __glewProgramParameters4dvNV;
extern PFNGLPROGRAMPARAMETERS4FVNVPROC __glewProgramParameters4fvNV;
extern PFNGLREQUESTRESIDENTPROGRAMSNVPROC __glewRequestResidentProgramsNV;
extern PFNGLTRACKMATRIXNVPROC __glewTrackMatrixNV;
extern PFNGLVERTEXATTRIB1DNVPROC __glewVertexAttrib1dNV;
extern PFNGLVERTEXATTRIB1DVNVPROC __glewVertexAttrib1dvNV;
extern PFNGLVERTEXATTRIB1FNVPROC __glewVertexAttrib1fNV;
extern PFNGLVERTEXATTRIB1FVNVPROC __glewVertexAttrib1fvNV;
extern PFNGLVERTEXATTRIB1SNVPROC __glewVertexAttrib1sNV;
extern PFNGLVERTEXATTRIB1SVNVPROC __glewVertexAttrib1svNV;
extern PFNGLVERTEXATTRIB2DNVPROC __glewVertexAttrib2dNV;
extern PFNGLVERTEXATTRIB2DVNVPROC __glewVertexAttrib2dvNV;
extern PFNGLVERTEXATTRIB2FNVPROC __glewVertexAttrib2fNV;
extern PFNGLVERTEXATTRIB2FVNVPROC __glewVertexAttrib2fvNV;
extern PFNGLVERTEXATTRIB2SNVPROC __glewVertexAttrib2sNV;
extern PFNGLVERTEXATTRIB2SVNVPROC __glewVertexAttrib2svNV;
extern PFNGLVERTEXATTRIB3DNVPROC __glewVertexAttrib3dNV;
extern PFNGLVERTEXATTRIB3DVNVPROC __glewVertexAttrib3dvNV;
extern PFNGLVERTEXATTRIB3FNVPROC __glewVertexAttrib3fNV;
extern PFNGLVERTEXATTRIB3FVNVPROC __glewVertexAttrib3fvNV;
extern PFNGLVERTEXATTRIB3SNVPROC __glewVertexAttrib3sNV;
extern PFNGLVERTEXATTRIB3SVNVPROC __glewVertexAttrib3svNV;
extern PFNGLVERTEXATTRIB4DNVPROC __glewVertexAttrib4dNV;
extern PFNGLVERTEXATTRIB4DVNVPROC __glewVertexAttrib4dvNV;
extern PFNGLVERTEXATTRIB4FNVPROC __glewVertexAttrib4fNV;
extern PFNGLVERTEXATTRIB4FVNVPROC __glewVertexAttrib4fvNV;
extern PFNGLVERTEXATTRIB4SNVPROC __glewVertexAttrib4sNV;
extern PFNGLVERTEXATTRIB4SVNVPROC __glewVertexAttrib4svNV;
extern PFNGLVERTEXATTRIB4UBNVPROC __glewVertexAttrib4ubNV;
extern PFNGLVERTEXATTRIB4UBVNVPROC __glewVertexAttrib4ubvNV;
extern PFNGLVERTEXATTRIBPOINTERNVPROC __glewVertexAttribPointerNV;
extern PFNGLVERTEXATTRIBS1DVNVPROC __glewVertexAttribs1dvNV;
extern PFNGLVERTEXATTRIBS1FVNVPROC __glewVertexAttribs1fvNV;
extern PFNGLVERTEXATTRIBS1SVNVPROC __glewVertexAttribs1svNV;
extern PFNGLVERTEXATTRIBS2DVNVPROC __glewVertexAttribs2dvNV;
extern PFNGLVERTEXATTRIBS2FVNVPROC __glewVertexAttribs2fvNV;
extern PFNGLVERTEXATTRIBS2SVNVPROC __glewVertexAttribs2svNV;
extern PFNGLVERTEXATTRIBS3DVNVPROC __glewVertexAttribs3dvNV;
extern PFNGLVERTEXATTRIBS3FVNVPROC __glewVertexAttribs3fvNV;
extern PFNGLVERTEXATTRIBS3SVNVPROC __glewVertexAttribs3svNV;
extern PFNGLVERTEXATTRIBS4DVNVPROC __glewVertexAttribs4dvNV;
extern PFNGLVERTEXATTRIBS4FVNVPROC __glewVertexAttribs4fvNV;
extern PFNGLVERTEXATTRIBS4SVNVPROC __glewVertexAttribs4svNV;
extern PFNGLVERTEXATTRIBS4UBVNVPROC __glewVertexAttribs4ubvNV;

extern PFNGLBEGINVIDEOCAPTURENVPROC __glewBeginVideoCaptureNV;
extern PFNGLBINDVIDEOCAPTURESTREAMBUFFERNVPROC __glewBindVideoCaptureStreamBufferNV;
extern PFNGLBINDVIDEOCAPTURESTREAMTEXTURENVPROC __glewBindVideoCaptureStreamTextureNV;
extern PFNGLENDVIDEOCAPTURENVPROC __glewEndVideoCaptureNV;
extern PFNGLGETVIDEOCAPTURESTREAMDVNVPROC __glewGetVideoCaptureStreamdvNV;
extern PFNGLGETVIDEOCAPTURESTREAMFVNVPROC __glewGetVideoCaptureStreamfvNV;
extern PFNGLGETVIDEOCAPTURESTREAMIVNVPROC __glewGetVideoCaptureStreamivNV;
extern PFNGLGETVIDEOCAPTUREIVNVPROC __glewGetVideoCaptureivNV;
extern PFNGLVIDEOCAPTURENVPROC __glewVideoCaptureNV;
extern PFNGLVIDEOCAPTURESTREAMPARAMETERDVNVPROC __glewVideoCaptureStreamParameterdvNV;
extern PFNGLVIDEOCAPTURESTREAMPARAMETERFVNVPROC __glewVideoCaptureStreamParameterfvNV;
extern PFNGLVIDEOCAPTURESTREAMPARAMETERIVNVPROC __glewVideoCaptureStreamParameterivNV;

extern PFNGLCLEARDEPTHFOESPROC __glewClearDepthfOES;
extern PFNGLCLIPPLANEFOESPROC __glewClipPlanefOES;
extern PFNGLDEPTHRANGEFOESPROC __glewDepthRangefOES;
extern PFNGLFRUSTUMFOESPROC __glewFrustumfOES;
extern PFNGLGETCLIPPLANEFOESPROC __glewGetClipPlanefOES;
extern PFNGLORTHOFOESPROC __glewOrthofOES;

extern PFNGLDETAILTEXFUNCSGISPROC __glewDetailTexFuncSGIS;
extern PFNGLGETDETAILTEXFUNCSGISPROC __glewGetDetailTexFuncSGIS;

extern PFNGLFOGFUNCSGISPROC __glewFogFuncSGIS;
extern PFNGLGETFOGFUNCSGISPROC __glewGetFogFuncSGIS;

extern PFNGLSAMPLEMASKSGISPROC __glewSampleMaskSGIS;
extern PFNGLSAMPLEPATTERNSGISPROC __glewSamplePatternSGIS;

extern PFNGLGETSHARPENTEXFUNCSGISPROC __glewGetSharpenTexFuncSGIS;
extern PFNGLSHARPENTEXFUNCSGISPROC __glewSharpenTexFuncSGIS;

extern PFNGLTEXIMAGE4DSGISPROC __glewTexImage4DSGIS;
extern PFNGLTEXSUBIMAGE4DSGISPROC __glewTexSubImage4DSGIS;

extern PFNGLGETTEXFILTERFUNCSGISPROC __glewGetTexFilterFuncSGIS;
extern PFNGLTEXFILTERFUNCSGISPROC __glewTexFilterFuncSGIS;

extern PFNGLASYNCMARKERSGIXPROC __glewAsyncMarkerSGIX;
extern PFNGLDELETEASYNCMARKERSSGIXPROC __glewDeleteAsyncMarkersSGIX;
extern PFNGLFINISHASYNCSGIXPROC __glewFinishAsyncSGIX;
extern PFNGLGENASYNCMARKERSSGIXPROC __glewGenAsyncMarkersSGIX;
extern PFNGLISASYNCMARKERSGIXPROC __glewIsAsyncMarkerSGIX;
extern PFNGLPOLLASYNCSGIXPROC __glewPollAsyncSGIX;

extern PFNGLFLUSHRASTERSGIXPROC __glewFlushRasterSGIX;

extern PFNGLTEXTUREFOGSGIXPROC __glewTextureFogSGIX;

extern PFNGLFRAGMENTCOLORMATERIALSGIXPROC __glewFragmentColorMaterialSGIX;
extern PFNGLFRAGMENTLIGHTMODELFSGIXPROC __glewFragmentLightModelfSGIX;
extern PFNGLFRAGMENTLIGHTMODELFVSGIXPROC __glewFragmentLightModelfvSGIX;
extern PFNGLFRAGMENTLIGHTMODELISGIXPROC __glewFragmentLightModeliSGIX;
extern PFNGLFRAGMENTLIGHTMODELIVSGIXPROC __glewFragmentLightModelivSGIX;
extern PFNGLFRAGMENTLIGHTFSGIXPROC __glewFragmentLightfSGIX;
extern PFNGLFRAGMENTLIGHTFVSGIXPROC __glewFragmentLightfvSGIX;
extern PFNGLFRAGMENTLIGHTISGIXPROC __glewFragmentLightiSGIX;
extern PFNGLFRAGMENTLIGHTIVSGIXPROC __glewFragmentLightivSGIX;
extern PFNGLFRAGMENTMATERIALFSGIXPROC __glewFragmentMaterialfSGIX;
extern PFNGLFRAGMENTMATERIALFVSGIXPROC __glewFragmentMaterialfvSGIX;
extern PFNGLFRAGMENTMATERIALISGIXPROC __glewFragmentMaterialiSGIX;
extern PFNGLFRAGMENTMATERIALIVSGIXPROC __glewFragmentMaterialivSGIX;
extern PFNGLGETFRAGMENTLIGHTFVSGIXPROC __glewGetFragmentLightfvSGIX;
extern PFNGLGETFRAGMENTLIGHTIVSGIXPROC __glewGetFragmentLightivSGIX;
extern PFNGLGETFRAGMENTMATERIALFVSGIXPROC __glewGetFragmentMaterialfvSGIX;
extern PFNGLGETFRAGMENTMATERIALIVSGIXPROC __glewGetFragmentMaterialivSGIX;

extern PFNGLFRAMEZOOMSGIXPROC __glewFrameZoomSGIX;

extern PFNGLPIXELTEXGENSGIXPROC __glewPixelTexGenSGIX;

extern PFNGLREFERENCEPLANESGIXPROC __glewReferencePlaneSGIX;

extern PFNGLSPRITEPARAMETERFSGIXPROC __glewSpriteParameterfSGIX;
extern PFNGLSPRITEPARAMETERFVSGIXPROC __glewSpriteParameterfvSGIX;
extern PFNGLSPRITEPARAMETERISGIXPROC __glewSpriteParameteriSGIX;
extern PFNGLSPRITEPARAMETERIVSGIXPROC __glewSpriteParameterivSGIX;

extern PFNGLTAGSAMPLEBUFFERSGIXPROC __glewTagSampleBufferSGIX;

extern PFNGLCOLORTABLEPARAMETERFVSGIPROC __glewColorTableParameterfvSGI;
extern PFNGLCOLORTABLEPARAMETERIVSGIPROC __glewColorTableParameterivSGI;
extern PFNGLCOLORTABLESGIPROC __glewColorTableSGI;
extern PFNGLCOPYCOLORTABLESGIPROC __glewCopyColorTableSGI;
extern PFNGLGETCOLORTABLEPARAMETERFVSGIPROC __glewGetColorTableParameterfvSGI;
extern PFNGLGETCOLORTABLEPARAMETERIVSGIPROC __glewGetColorTableParameterivSGI;
extern PFNGLGETCOLORTABLESGIPROC __glewGetColorTableSGI;

extern PFNGLFINISHTEXTURESUNXPROC __glewFinishTextureSUNX;

extern PFNGLGLOBALALPHAFACTORBSUNPROC __glewGlobalAlphaFactorbSUN;
extern PFNGLGLOBALALPHAFACTORDSUNPROC __glewGlobalAlphaFactordSUN;
extern PFNGLGLOBALALPHAFACTORFSUNPROC __glewGlobalAlphaFactorfSUN;
extern PFNGLGLOBALALPHAFACTORISUNPROC __glewGlobalAlphaFactoriSUN;
extern PFNGLGLOBALALPHAFACTORSSUNPROC __glewGlobalAlphaFactorsSUN;
extern PFNGLGLOBALALPHAFACTORUBSUNPROC __glewGlobalAlphaFactorubSUN;
extern PFNGLGLOBALALPHAFACTORUISUNPROC __glewGlobalAlphaFactoruiSUN;
extern PFNGLGLOBALALPHAFACTORUSSUNPROC __glewGlobalAlphaFactorusSUN;

extern PFNGLREADVIDEOPIXELSSUNPROC __glewReadVideoPixelsSUN;

extern PFNGLREPLACEMENTCODEPOINTERSUNPROC __glewReplacementCodePointerSUN;
extern PFNGLREPLACEMENTCODEUBSUNPROC __glewReplacementCodeubSUN;
extern PFNGLREPLACEMENTCODEUBVSUNPROC __glewReplacementCodeubvSUN;
extern PFNGLREPLACEMENTCODEUISUNPROC __glewReplacementCodeuiSUN;
extern PFNGLREPLACEMENTCODEUIVSUNPROC __glewReplacementCodeuivSUN;
extern PFNGLREPLACEMENTCODEUSSUNPROC __glewReplacementCodeusSUN;
extern PFNGLREPLACEMENTCODEUSVSUNPROC __glewReplacementCodeusvSUN;

extern PFNGLCOLOR3FVERTEX3FSUNPROC __glewColor3fVertex3fSUN;
extern PFNGLCOLOR3FVERTEX3FVSUNPROC __glewColor3fVertex3fvSUN;
extern PFNGLCOLOR4FNORMAL3FVERTEX3FSUNPROC __glewColor4fNormal3fVertex3fSUN;
extern PFNGLCOLOR4FNORMAL3FVERTEX3FVSUNPROC __glewColor4fNormal3fVertex3fvSUN;
extern PFNGLCOLOR4UBVERTEX2FSUNPROC __glewColor4ubVertex2fSUN;
extern PFNGLCOLOR4UBVERTEX2FVSUNPROC __glewColor4ubVertex2fvSUN;
extern PFNGLCOLOR4UBVERTEX3FSUNPROC __glewColor4ubVertex3fSUN;
extern PFNGLCOLOR4UBVERTEX3FVSUNPROC __glewColor4ubVertex3fvSUN;
extern PFNGLNORMAL3FVERTEX3FSUNPROC __glewNormal3fVertex3fSUN;
extern PFNGLNORMAL3FVERTEX3FVSUNPROC __glewNormal3fVertex3fvSUN;
extern PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FSUNPROC __glewReplacementCodeuiColor3fVertex3fSUN;
extern PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FVSUNPROC __glewReplacementCodeuiColor3fVertex3fvSUN;
extern PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FSUNPROC __glewReplacementCodeuiColor4fNormal3fVertex3fSUN;
extern PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FVSUNPROC __glewReplacementCodeuiColor4fNormal3fVertex3fvSUN;
extern PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FSUNPROC __glewReplacementCodeuiColor4ubVertex3fSUN;
extern PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FVSUNPROC __glewReplacementCodeuiColor4ubVertex3fvSUN;
extern PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FSUNPROC __glewReplacementCodeuiNormal3fVertex3fSUN;
extern PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FVSUNPROC __glewReplacementCodeuiNormal3fVertex3fvSUN;
extern PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC __glewReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN;
extern PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC __glewReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN;
extern PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FSUNPROC __glewReplacementCodeuiTexCoord2fNormal3fVertex3fSUN;
extern PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FVSUNPROC __glewReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN;
extern PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FSUNPROC __glewReplacementCodeuiTexCoord2fVertex3fSUN;
extern PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FVSUNPROC __glewReplacementCodeuiTexCoord2fVertex3fvSUN;
extern PFNGLREPLACEMENTCODEUIVERTEX3FSUNPROC __glewReplacementCodeuiVertex3fSUN;
extern PFNGLREPLACEMENTCODEUIVERTEX3FVSUNPROC __glewReplacementCodeuiVertex3fvSUN;
extern PFNGLTEXCOORD2FCOLOR3FVERTEX3FSUNPROC __glewTexCoord2fColor3fVertex3fSUN;
extern PFNGLTEXCOORD2FCOLOR3FVERTEX3FVSUNPROC __glewTexCoord2fColor3fVertex3fvSUN;
extern PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC __glewTexCoord2fColor4fNormal3fVertex3fSUN;
extern PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC __glewTexCoord2fColor4fNormal3fVertex3fvSUN;
extern PFNGLTEXCOORD2FCOLOR4UBVERTEX3FSUNPROC __glewTexCoord2fColor4ubVertex3fSUN;
extern PFNGLTEXCOORD2FCOLOR4UBVERTEX3FVSUNPROC __glewTexCoord2fColor4ubVertex3fvSUN;
extern PFNGLTEXCOORD2FNORMAL3FVERTEX3FSUNPROC __glewTexCoord2fNormal3fVertex3fSUN;
extern PFNGLTEXCOORD2FNORMAL3FVERTEX3FVSUNPROC __glewTexCoord2fNormal3fVertex3fvSUN;
extern PFNGLTEXCOORD2FVERTEX3FSUNPROC __glewTexCoord2fVertex3fSUN;
extern PFNGLTEXCOORD2FVERTEX3FVSUNPROC __glewTexCoord2fVertex3fvSUN;
extern PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FSUNPROC __glewTexCoord4fColor4fNormal3fVertex4fSUN;
extern PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FVSUNPROC __glewTexCoord4fColor4fNormal3fVertex4fvSUN;
extern PFNGLTEXCOORD4FVERTEX4FSUNPROC __glewTexCoord4fVertex4fSUN;
extern PFNGLTEXCOORD4FVERTEX4FVSUNPROC __glewTexCoord4fVertex4fvSUN;

extern PFNGLADDSWAPHINTRECTWINPROC __glewAddSwapHintRectWIN;
extern GLboolean __GLEW_VERSION_1_1;
extern GLboolean __GLEW_VERSION_1_2;
extern GLboolean __GLEW_VERSION_1_2_1;
extern GLboolean __GLEW_VERSION_1_3;
extern GLboolean __GLEW_VERSION_1_4;
extern GLboolean __GLEW_VERSION_1_5;
extern GLboolean __GLEW_VERSION_2_0;
extern GLboolean __GLEW_VERSION_2_1;
extern GLboolean __GLEW_VERSION_3_0;
extern GLboolean __GLEW_VERSION_3_1;
extern GLboolean __GLEW_VERSION_3_2;
extern GLboolean __GLEW_VERSION_3_3;
extern GLboolean __GLEW_VERSION_4_0;
extern GLboolean __GLEW_VERSION_4_1;
extern GLboolean __GLEW_VERSION_4_2;
extern GLboolean __GLEW_3DFX_multisample;
extern GLboolean __GLEW_3DFX_tbuffer;
extern GLboolean __GLEW_3DFX_texture_compression_FXT1;
extern GLboolean __GLEW_AMD_blend_minmax_factor;
extern GLboolean __GLEW_AMD_conservative_depth;
extern GLboolean __GLEW_AMD_debug_output;
extern GLboolean __GLEW_AMD_depth_clamp_separate;
extern GLboolean __GLEW_AMD_draw_buffers_blend;
extern GLboolean __GLEW_AMD_multi_draw_indirect;
extern GLboolean __GLEW_AMD_name_gen_delete;
extern GLboolean __GLEW_AMD_performance_monitor;
extern GLboolean __GLEW_AMD_sample_positions;
extern GLboolean __GLEW_AMD_seamless_cubemap_per_texture;
extern GLboolean __GLEW_AMD_shader_stencil_export;
extern GLboolean __GLEW_AMD_texture_texture4;
extern GLboolean __GLEW_AMD_transform_feedback3_lines_triangles;
extern GLboolean __GLEW_AMD_vertex_shader_tessellator;
extern GLboolean __GLEW_APPLE_aux_depth_stencil;
extern GLboolean __GLEW_APPLE_client_storage;
extern GLboolean __GLEW_APPLE_element_array;
extern GLboolean __GLEW_APPLE_fence;
extern GLboolean __GLEW_APPLE_float_pixels;
extern GLboolean __GLEW_APPLE_flush_buffer_range;
extern GLboolean __GLEW_APPLE_object_purgeable;
extern GLboolean __GLEW_APPLE_pixel_buffer;
extern GLboolean __GLEW_APPLE_rgb_422;
extern GLboolean __GLEW_APPLE_row_bytes;
extern GLboolean __GLEW_APPLE_specular_vector;
extern GLboolean __GLEW_APPLE_texture_range;
extern GLboolean __GLEW_APPLE_transform_hint;
extern GLboolean __GLEW_APPLE_vertex_array_object;
extern GLboolean __GLEW_APPLE_vertex_array_range;
extern GLboolean __GLEW_APPLE_vertex_program_evaluators;
extern GLboolean __GLEW_APPLE_ycbcr_422;
extern GLboolean __GLEW_ARB_ES2_compatibility;
extern GLboolean __GLEW_ARB_base_instance;
extern GLboolean __GLEW_ARB_blend_func_extended;
extern GLboolean __GLEW_ARB_cl_event;
extern GLboolean __GLEW_ARB_color_buffer_float;
extern GLboolean __GLEW_ARB_compatibility;
extern GLboolean __GLEW_ARB_compressed_texture_pixel_storage;
extern GLboolean __GLEW_ARB_conservative_depth;
extern GLboolean __GLEW_ARB_copy_buffer;
extern GLboolean __GLEW_ARB_debug_output;
extern GLboolean __GLEW_ARB_depth_buffer_float;
extern GLboolean __GLEW_ARB_depth_clamp;
extern GLboolean __GLEW_ARB_depth_texture;
extern GLboolean __GLEW_ARB_draw_buffers;
extern GLboolean __GLEW_ARB_draw_buffers_blend;
extern GLboolean __GLEW_ARB_draw_elements_base_vertex;
extern GLboolean __GLEW_ARB_draw_indirect;
extern GLboolean __GLEW_ARB_draw_instanced;
extern GLboolean __GLEW_ARB_explicit_attrib_location;
extern GLboolean __GLEW_ARB_fragment_coord_conventions;
extern GLboolean __GLEW_ARB_fragment_program;
extern GLboolean __GLEW_ARB_fragment_program_shadow;
extern GLboolean __GLEW_ARB_fragment_shader;
extern GLboolean __GLEW_ARB_framebuffer_object;
extern GLboolean __GLEW_ARB_framebuffer_sRGB;
extern GLboolean __GLEW_ARB_geometry_shader4;
extern GLboolean __GLEW_ARB_get_program_binary;
extern GLboolean __GLEW_ARB_gpu_shader5;
extern GLboolean __GLEW_ARB_gpu_shader_fp64;
extern GLboolean __GLEW_ARB_half_float_pixel;
extern GLboolean __GLEW_ARB_half_float_vertex;
extern GLboolean __GLEW_ARB_imaging;
extern GLboolean __GLEW_ARB_instanced_arrays;
extern GLboolean __GLEW_ARB_internalformat_query;
extern GLboolean __GLEW_ARB_map_buffer_alignment;
extern GLboolean __GLEW_ARB_map_buffer_range;
extern GLboolean __GLEW_ARB_matrix_palette;
extern GLboolean __GLEW_ARB_multisample;
extern GLboolean __GLEW_ARB_multitexture;
extern GLboolean __GLEW_ARB_occlusion_query;
extern GLboolean __GLEW_ARB_occlusion_query2;
extern GLboolean __GLEW_ARB_pixel_buffer_object;
extern GLboolean __GLEW_ARB_point_parameters;
extern GLboolean __GLEW_ARB_point_sprite;
extern GLboolean __GLEW_ARB_provoking_vertex;
extern GLboolean __GLEW_ARB_robustness;
extern GLboolean __GLEW_ARB_sample_shading;
extern GLboolean __GLEW_ARB_sampler_objects;
extern GLboolean __GLEW_ARB_seamless_cube_map;
extern GLboolean __GLEW_ARB_separate_shader_objects;
extern GLboolean __GLEW_ARB_shader_atomic_counters;
extern GLboolean __GLEW_ARB_shader_bit_encoding;
extern GLboolean __GLEW_ARB_shader_image_load_store;
extern GLboolean __GLEW_ARB_shader_objects;
extern GLboolean __GLEW_ARB_shader_precision;
extern GLboolean __GLEW_ARB_shader_stencil_export;
extern GLboolean __GLEW_ARB_shader_subroutine;
extern GLboolean __GLEW_ARB_shader_texture_lod;
extern GLboolean __GLEW_ARB_shading_language_100;
extern GLboolean __GLEW_ARB_shading_language_420pack;
extern GLboolean __GLEW_ARB_shading_language_include;
extern GLboolean __GLEW_ARB_shading_language_packing;
extern GLboolean __GLEW_ARB_shadow;
extern GLboolean __GLEW_ARB_shadow_ambient;
extern GLboolean __GLEW_ARB_sync;
extern GLboolean __GLEW_ARB_tessellation_shader;
extern GLboolean __GLEW_ARB_texture_border_clamp;
extern GLboolean __GLEW_ARB_texture_buffer_object;
extern GLboolean __GLEW_ARB_texture_buffer_object_rgb32;
extern GLboolean __GLEW_ARB_texture_compression;
extern GLboolean __GLEW_ARB_texture_compression_bptc;
extern GLboolean __GLEW_ARB_texture_compression_rgtc;
extern GLboolean __GLEW_ARB_texture_cube_map;
extern GLboolean __GLEW_ARB_texture_cube_map_array;
extern GLboolean __GLEW_ARB_texture_env_add;
extern GLboolean __GLEW_ARB_texture_env_combine;
extern GLboolean __GLEW_ARB_texture_env_crossbar;
extern GLboolean __GLEW_ARB_texture_env_dot3;
extern GLboolean __GLEW_ARB_texture_float;
extern GLboolean __GLEW_ARB_texture_gather;
extern GLboolean __GLEW_ARB_texture_mirrored_repeat;
extern GLboolean __GLEW_ARB_texture_multisample;
extern GLboolean __GLEW_ARB_texture_non_power_of_two;
extern GLboolean __GLEW_ARB_texture_query_lod;
extern GLboolean __GLEW_ARB_texture_rectangle;
extern GLboolean __GLEW_ARB_texture_rg;
extern GLboolean __GLEW_ARB_texture_rgb10_a2ui;
extern GLboolean __GLEW_ARB_texture_storage;
extern GLboolean __GLEW_ARB_texture_swizzle;
extern GLboolean __GLEW_ARB_timer_query;
extern GLboolean __GLEW_ARB_transform_feedback2;
extern GLboolean __GLEW_ARB_transform_feedback3;
extern GLboolean __GLEW_ARB_transform_feedback_instanced;
extern GLboolean __GLEW_ARB_transpose_matrix;
extern GLboolean __GLEW_ARB_uniform_buffer_object;
extern GLboolean __GLEW_ARB_vertex_array_bgra;
extern GLboolean __GLEW_ARB_vertex_array_object;
extern GLboolean __GLEW_ARB_vertex_attrib_64bit;
extern GLboolean __GLEW_ARB_vertex_blend;
extern GLboolean __GLEW_ARB_vertex_buffer_object;
extern GLboolean __GLEW_ARB_vertex_program;
extern GLboolean __GLEW_ARB_vertex_shader;
extern GLboolean __GLEW_ARB_vertex_type_2_10_10_10_rev;
extern GLboolean __GLEW_ARB_viewport_array;
extern GLboolean __GLEW_ARB_window_pos;
extern GLboolean __GLEW_ATIX_point_sprites;
extern GLboolean __GLEW_ATIX_texture_env_combine3;
extern GLboolean __GLEW_ATIX_texture_env_route;
extern GLboolean __GLEW_ATIX_vertex_shader_output_point_size;
extern GLboolean __GLEW_ATI_draw_buffers;
extern GLboolean __GLEW_ATI_element_array;
extern GLboolean __GLEW_ATI_envmap_bumpmap;
extern GLboolean __GLEW_ATI_fragment_shader;
extern GLboolean __GLEW_ATI_map_object_buffer;
extern GLboolean __GLEW_ATI_meminfo;
extern GLboolean __GLEW_ATI_pn_triangles;
extern GLboolean __GLEW_ATI_separate_stencil;
extern GLboolean __GLEW_ATI_shader_texture_lod;
extern GLboolean __GLEW_ATI_text_fragment_shader;
extern GLboolean __GLEW_ATI_texture_compression_3dc;
extern GLboolean __GLEW_ATI_texture_env_combine3;
extern GLboolean __GLEW_ATI_texture_float;
extern GLboolean __GLEW_ATI_texture_mirror_once;
extern GLboolean __GLEW_ATI_vertex_array_object;
extern GLboolean __GLEW_ATI_vertex_attrib_array_object;
extern GLboolean __GLEW_ATI_vertex_streams;
extern GLboolean __GLEW_EXT_422_pixels;
extern GLboolean __GLEW_EXT_Cg_shader;
extern GLboolean __GLEW_EXT_abgr;
extern GLboolean __GLEW_EXT_bgra;
extern GLboolean __GLEW_EXT_bindable_uniform;
extern GLboolean __GLEW_EXT_blend_color;
extern GLboolean __GLEW_EXT_blend_equation_separate;
extern GLboolean __GLEW_EXT_blend_func_separate;
extern GLboolean __GLEW_EXT_blend_logic_op;
extern GLboolean __GLEW_EXT_blend_minmax;
extern GLboolean __GLEW_EXT_blend_subtract;
extern GLboolean __GLEW_EXT_clip_volume_hint;
extern GLboolean __GLEW_EXT_cmyka;
extern GLboolean __GLEW_EXT_color_subtable;
extern GLboolean __GLEW_EXT_compiled_vertex_array;
extern GLboolean __GLEW_EXT_convolution;
extern GLboolean __GLEW_EXT_coordinate_frame;
extern GLboolean __GLEW_EXT_copy_texture;
extern GLboolean __GLEW_EXT_cull_vertex;
extern GLboolean __GLEW_EXT_depth_bounds_test;
extern GLboolean __GLEW_EXT_direct_state_access;
extern GLboolean __GLEW_EXT_draw_buffers2;
extern GLboolean __GLEW_EXT_draw_instanced;
extern GLboolean __GLEW_EXT_draw_range_elements;
extern GLboolean __GLEW_EXT_fog_coord;
extern GLboolean __GLEW_EXT_fragment_lighting;
extern GLboolean __GLEW_EXT_framebuffer_blit;
extern GLboolean __GLEW_EXT_framebuffer_multisample;
extern GLboolean __GLEW_EXT_framebuffer_multisample_blit_scaled;
extern GLboolean __GLEW_EXT_framebuffer_object;
extern GLboolean __GLEW_EXT_framebuffer_sRGB;
extern GLboolean __GLEW_EXT_geometry_shader4;
extern GLboolean __GLEW_EXT_gpu_program_parameters;
extern GLboolean __GLEW_EXT_gpu_shader4;
extern GLboolean __GLEW_EXT_histogram;
extern GLboolean __GLEW_EXT_index_array_formats;
extern GLboolean __GLEW_EXT_index_func;
extern GLboolean __GLEW_EXT_index_material;
extern GLboolean __GLEW_EXT_index_texture;
extern GLboolean __GLEW_EXT_light_texture;
extern GLboolean __GLEW_EXT_misc_attribute;
extern GLboolean __GLEW_EXT_multi_draw_arrays;
extern GLboolean __GLEW_EXT_multisample;
extern GLboolean __GLEW_EXT_packed_depth_stencil;
extern GLboolean __GLEW_EXT_packed_float;
extern GLboolean __GLEW_EXT_packed_pixels;
extern GLboolean __GLEW_EXT_paletted_texture;
extern GLboolean __GLEW_EXT_pixel_buffer_object;
extern GLboolean __GLEW_EXT_pixel_transform;
extern GLboolean __GLEW_EXT_pixel_transform_color_table;
extern GLboolean __GLEW_EXT_point_parameters;
extern GLboolean __GLEW_EXT_polygon_offset;
extern GLboolean __GLEW_EXT_provoking_vertex;
extern GLboolean __GLEW_EXT_rescale_normal;
extern GLboolean __GLEW_EXT_scene_marker;
extern GLboolean __GLEW_EXT_secondary_color;
extern GLboolean __GLEW_EXT_separate_shader_objects;
extern GLboolean __GLEW_EXT_separate_specular_color;
extern GLboolean __GLEW_EXT_shader_image_load_store;
extern GLboolean __GLEW_EXT_shadow_funcs;
extern GLboolean __GLEW_EXT_shared_texture_palette;
extern GLboolean __GLEW_EXT_stencil_clear_tag;
extern GLboolean __GLEW_EXT_stencil_two_side;
extern GLboolean __GLEW_EXT_stencil_wrap;
extern GLboolean __GLEW_EXT_subtexture;
extern GLboolean __GLEW_EXT_texture;
extern GLboolean __GLEW_EXT_texture3D;
extern GLboolean __GLEW_EXT_texture_array;
extern GLboolean __GLEW_EXT_texture_buffer_object;
extern GLboolean __GLEW_EXT_texture_compression_dxt1;
extern GLboolean __GLEW_EXT_texture_compression_latc;
extern GLboolean __GLEW_EXT_texture_compression_rgtc;
extern GLboolean __GLEW_EXT_texture_compression_s3tc;
extern GLboolean __GLEW_EXT_texture_cube_map;
extern GLboolean __GLEW_EXT_texture_edge_clamp;
extern GLboolean __GLEW_EXT_texture_env;
extern GLboolean __GLEW_EXT_texture_env_add;
extern GLboolean __GLEW_EXT_texture_env_combine;
extern GLboolean __GLEW_EXT_texture_env_dot3;
extern GLboolean __GLEW_EXT_texture_filter_anisotropic;
extern GLboolean __GLEW_EXT_texture_integer;
extern GLboolean __GLEW_EXT_texture_lod_bias;
extern GLboolean __GLEW_EXT_texture_mirror_clamp;
extern GLboolean __GLEW_EXT_texture_object;
extern GLboolean __GLEW_EXT_texture_perturb_normal;
extern GLboolean __GLEW_EXT_texture_rectangle;
extern GLboolean __GLEW_EXT_texture_sRGB;
extern GLboolean __GLEW_EXT_texture_sRGB_decode;
extern GLboolean __GLEW_EXT_texture_shared_exponent;
extern GLboolean __GLEW_EXT_texture_snorm;
extern GLboolean __GLEW_EXT_texture_swizzle;
extern GLboolean __GLEW_EXT_timer_query;
extern GLboolean __GLEW_EXT_transform_feedback;
extern GLboolean __GLEW_EXT_vertex_array;
extern GLboolean __GLEW_EXT_vertex_array_bgra;
extern GLboolean __GLEW_EXT_vertex_attrib_64bit;
extern GLboolean __GLEW_EXT_vertex_shader;
extern GLboolean __GLEW_EXT_vertex_weighting;
extern GLboolean __GLEW_EXT_x11_sync_object;
extern GLboolean __GLEW_GREMEDY_frame_terminator;
extern GLboolean __GLEW_GREMEDY_string_marker;
extern GLboolean __GLEW_HP_convolution_border_modes;
extern GLboolean __GLEW_HP_image_transform;
extern GLboolean __GLEW_HP_occlusion_test;
extern GLboolean __GLEW_HP_texture_lighting;
extern GLboolean __GLEW_IBM_cull_vertex;
extern GLboolean __GLEW_IBM_multimode_draw_arrays;
extern GLboolean __GLEW_IBM_rasterpos_clip;
extern GLboolean __GLEW_IBM_static_data;
extern GLboolean __GLEW_IBM_texture_mirrored_repeat;
extern GLboolean __GLEW_IBM_vertex_array_lists;
extern GLboolean __GLEW_INGR_color_clamp;
extern GLboolean __GLEW_INGR_interlace_read;
extern GLboolean __GLEW_INTEL_parallel_arrays;
extern GLboolean __GLEW_INTEL_texture_scissor;
extern GLboolean __GLEW_KTX_buffer_region;
extern GLboolean __GLEW_MESAX_texture_stack;
extern GLboolean __GLEW_MESA_pack_invert;
extern GLboolean __GLEW_MESA_resize_buffers;
extern GLboolean __GLEW_MESA_window_pos;
extern GLboolean __GLEW_MESA_ycbcr_texture;
extern GLboolean __GLEW_NVX_gpu_memory_info;
extern GLboolean __GLEW_NV_blend_square;
extern GLboolean __GLEW_NV_conditional_render;
extern GLboolean __GLEW_NV_copy_depth_to_color;
extern GLboolean __GLEW_NV_copy_image;
extern GLboolean __GLEW_NV_depth_buffer_float;
extern GLboolean __GLEW_NV_depth_clamp;
extern GLboolean __GLEW_NV_depth_range_unclamped;
extern GLboolean __GLEW_NV_evaluators;
extern GLboolean __GLEW_NV_explicit_multisample;
extern GLboolean __GLEW_NV_fence;
extern GLboolean __GLEW_NV_float_buffer;
extern GLboolean __GLEW_NV_fog_distance;
extern GLboolean __GLEW_NV_fragment_program;
extern GLboolean __GLEW_NV_fragment_program2;
extern GLboolean __GLEW_NV_fragment_program4;
extern GLboolean __GLEW_NV_fragment_program_option;
extern GLboolean __GLEW_NV_framebuffer_multisample_coverage;
extern GLboolean __GLEW_NV_geometry_program4;
extern GLboolean __GLEW_NV_geometry_shader4;
extern GLboolean __GLEW_NV_gpu_program4;
extern GLboolean __GLEW_NV_gpu_program5;
extern GLboolean __GLEW_NV_gpu_program_fp64;
extern GLboolean __GLEW_NV_gpu_shader5;
extern GLboolean __GLEW_NV_half_float;
extern GLboolean __GLEW_NV_light_max_exponent;
extern GLboolean __GLEW_NV_multisample_coverage;
extern GLboolean __GLEW_NV_multisample_filter_hint;
extern GLboolean __GLEW_NV_occlusion_query;
extern GLboolean __GLEW_NV_packed_depth_stencil;
extern GLboolean __GLEW_NV_parameter_buffer_object;
extern GLboolean __GLEW_NV_parameter_buffer_object2;
extern GLboolean __GLEW_NV_path_rendering;
extern GLboolean __GLEW_NV_pixel_data_range;
extern GLboolean __GLEW_NV_point_sprite;
extern GLboolean __GLEW_NV_present_video;
extern GLboolean __GLEW_NV_primitive_restart;
extern GLboolean __GLEW_NV_register_combiners;
extern GLboolean __GLEW_NV_register_combiners2;
extern GLboolean __GLEW_NV_shader_buffer_load;
extern GLboolean __GLEW_NV_tessellation_program5;
extern GLboolean __GLEW_NV_texgen_emboss;
extern GLboolean __GLEW_NV_texgen_reflection;
extern GLboolean __GLEW_NV_texture_barrier;
extern GLboolean __GLEW_NV_texture_compression_vtc;
extern GLboolean __GLEW_NV_texture_env_combine4;
extern GLboolean __GLEW_NV_texture_expand_normal;
extern GLboolean __GLEW_NV_texture_multisample;
extern GLboolean __GLEW_NV_texture_rectangle;
extern GLboolean __GLEW_NV_texture_shader;
extern GLboolean __GLEW_NV_texture_shader2;
extern GLboolean __GLEW_NV_texture_shader3;
extern GLboolean __GLEW_NV_transform_feedback;
extern GLboolean __GLEW_NV_transform_feedback2;
extern GLboolean __GLEW_NV_vdpau_interop;
extern GLboolean __GLEW_NV_vertex_array_range;
extern GLboolean __GLEW_NV_vertex_array_range2;
extern GLboolean __GLEW_NV_vertex_attrib_integer_64bit;
extern GLboolean __GLEW_NV_vertex_buffer_unified_memory;
extern GLboolean __GLEW_NV_vertex_program;
extern GLboolean __GLEW_NV_vertex_program1_1;
extern GLboolean __GLEW_NV_vertex_program2;
extern GLboolean __GLEW_NV_vertex_program2_option;
extern GLboolean __GLEW_NV_vertex_program3;
extern GLboolean __GLEW_NV_vertex_program4;
extern GLboolean __GLEW_NV_video_capture;
extern GLboolean __GLEW_OES_byte_coordinates;
extern GLboolean __GLEW_OES_compressed_paletted_texture;
extern GLboolean __GLEW_OES_read_format;
extern GLboolean __GLEW_OES_single_precision;
extern GLboolean __GLEW_OML_interlace;
extern GLboolean __GLEW_OML_resample;
extern GLboolean __GLEW_OML_subsample;
extern GLboolean __GLEW_PGI_misc_hints;
extern GLboolean __GLEW_PGI_vertex_hints;
extern GLboolean __GLEW_REND_screen_coordinates;
extern GLboolean __GLEW_S3_s3tc;
extern GLboolean __GLEW_SGIS_color_range;
extern GLboolean __GLEW_SGIS_detail_texture;
extern GLboolean __GLEW_SGIS_fog_function;
extern GLboolean __GLEW_SGIS_generate_mipmap;
extern GLboolean __GLEW_SGIS_multisample;
extern GLboolean __GLEW_SGIS_pixel_texture;
extern GLboolean __GLEW_SGIS_point_line_texgen;
extern GLboolean __GLEW_SGIS_sharpen_texture;
extern GLboolean __GLEW_SGIS_texture4D;
extern GLboolean __GLEW_SGIS_texture_border_clamp;
extern GLboolean __GLEW_SGIS_texture_edge_clamp;
extern GLboolean __GLEW_SGIS_texture_filter4;
extern GLboolean __GLEW_SGIS_texture_lod;
extern GLboolean __GLEW_SGIS_texture_select;
extern GLboolean __GLEW_SGIX_async;
extern GLboolean __GLEW_SGIX_async_histogram;
extern GLboolean __GLEW_SGIX_async_pixel;
extern GLboolean __GLEW_SGIX_blend_alpha_minmax;
extern GLboolean __GLEW_SGIX_clipmap;
extern GLboolean __GLEW_SGIX_convolution_accuracy;
extern GLboolean __GLEW_SGIX_depth_texture;
extern GLboolean __GLEW_SGIX_flush_raster;
extern GLboolean __GLEW_SGIX_fog_offset;
extern GLboolean __GLEW_SGIX_fog_texture;
extern GLboolean __GLEW_SGIX_fragment_specular_lighting;
extern GLboolean __GLEW_SGIX_framezoom;
extern GLboolean __GLEW_SGIX_interlace;
extern GLboolean __GLEW_SGIX_ir_instrument1;
extern GLboolean __GLEW_SGIX_list_priority;
extern GLboolean __GLEW_SGIX_pixel_texture;
extern GLboolean __GLEW_SGIX_pixel_texture_bits;
extern GLboolean __GLEW_SGIX_reference_plane;
extern GLboolean __GLEW_SGIX_resample;
extern GLboolean __GLEW_SGIX_shadow;
extern GLboolean __GLEW_SGIX_shadow_ambient;
extern GLboolean __GLEW_SGIX_sprite;
extern GLboolean __GLEW_SGIX_tag_sample_buffer;
extern GLboolean __GLEW_SGIX_texture_add_env;
extern GLboolean __GLEW_SGIX_texture_coordinate_clamp;
extern GLboolean __GLEW_SGIX_texture_lod_bias;
extern GLboolean __GLEW_SGIX_texture_multi_buffer;
extern GLboolean __GLEW_SGIX_texture_range;
extern GLboolean __GLEW_SGIX_texture_scale_bias;
extern GLboolean __GLEW_SGIX_vertex_preclip;
extern GLboolean __GLEW_SGIX_vertex_preclip_hint;
extern GLboolean __GLEW_SGIX_ycrcb;
extern GLboolean __GLEW_SGI_color_matrix;
extern GLboolean __GLEW_SGI_color_table;
extern GLboolean __GLEW_SGI_texture_color_table;
extern GLboolean __GLEW_SUNX_constant_data;
extern GLboolean __GLEW_SUN_convolution_border_modes;
extern GLboolean __GLEW_SUN_global_alpha;
extern GLboolean __GLEW_SUN_mesh_array;
extern GLboolean __GLEW_SUN_read_video_pixels;
extern GLboolean __GLEW_SUN_slice_accum;
extern GLboolean __GLEW_SUN_triangle_list;
extern GLboolean __GLEW_SUN_vertex;
extern GLboolean __GLEW_WIN_phong_shading;
extern GLboolean __GLEW_WIN_specular_fog;
extern GLboolean __GLEW_WIN_swap_hint;

extern GLenum glewInit ();
extern GLboolean glewIsSupported (const char* name);

extern GLboolean glewExperimental;
extern GLboolean glewGetExtension (const char* name);
extern const GLubyte* glewGetErrorString (GLenum error);
extern const GLubyte* glewGetString (GLenum name);
}
using namespace std;
namespace enigma
{
  struct surface
  {
  GLuint tex, fbo;
  int width, height;
  };
}
int surface_create(int width, int height);
void surface_set_target(int id);
void surface_reset_target();
void surface_free(int id);
bool surface_exists(int id);
void draw_surface(int id, double x, double y);
void draw_surface_stretched(int id, double x, double y, double w, double h);
void draw_surface_part(int id,double left,double top,double width,double height,double x,double y);
void draw_surface_tiled(int id,double x,double y);
void draw_surface_tiled_area(int id,double x,double y,double x1,double y1,double x2,double y2);
void draw_surface_ext(int id,double x,double y,double xscale,double yscale,double rot,int color,double alpha);
void draw_surface_stretched_ext(int id,double x,double y,double w,double h,int color,double alpha);
void draw_surface_part_ext(int id,double left,double top,double width,double height,double x,double y,double xscale,double yscale,int color,double alpha);
void draw_surface_tiled_ext(int id,double x,double y,double xscale,double yscale,int color,double alpha);
void draw_surface_tiled_area_ext(int id,double x,double y,double x1,double y1,double x2,double y2, double xscale, double yscale, int color, double alpha);
void draw_surface_general(int id, double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a1,double a2,double a3,double a4);
int surface_get_bound();
int surface_get_texture(int id);
int surface_get_width(int id);
int surface_get_height(int id);
int surface_getpixel(int id, int x, int y);
int surface_getpixel_alpha(int id, int x, int y);
int surface_save(int id, string filename);
int surface_save_part(int id, string filename,unsigned x,unsigned y,unsigned w,unsigned h);
void surface_copy(int destination,double x,double y,int source);
void surface_copy_part(int destination,double x,double y,int source,int xs,int ys,int ws,int hs);
int sprite_create_from_surface(int id,int x,int y,int w,int h,bool removeback,bool smooth,int xorig,int yorig);
bool surface_is_supported();

int screen_save(string filename);
int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h);

void draw_sprite(int spr,int subimg,double x,double y);
void draw_sprite_stretched(int spr,int subimg,double x,double y,double w,double h);

void draw_sprite_part(int spr,int subimg,double left,double top,double width,double height,double x,double y);
void draw_sprite_part_offset(int spr,int subimg,double left,double top,double width,double height,double x,double y);
void draw_sprite_ext(int spr,int subimg,double x,double y,double xscale,double yscale,double rot,int color,double alpha);

void draw_sprite_stretched_ext(int spr,int subimg,double x,double y,double w,double h, int blend, double alpha);
void draw_sprite_part_ext(int spr,int subimg,double left,double top,double width,double height,double x,double y,double xscale,double yscale,int color,double alpha);
void draw_sprite_general(int spr,int subimg,double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a1, double a2, double a3, double a4);

void draw_sprite_tiled(int spr,int subimg,double x,double y);
void draw_sprite_tiled_ext(int spr,int subimg,double x,double y,double xscale,double yscale,int color,double alpha);

bool sprite_exists(int sprite);

void draw_background(int back, double x, double y);
void draw_background_stretched(int back, double x, double y, double w, double h);
void draw_background_part(int back,double left,double top,double width,double height,double x,double y);
void draw_background_tiled(int back,double x,double y);
void draw_background_tiled_area(int back,double x,double y,double x1,double y1,double x2,double y2);
void draw_background_ext(int back,double x,double y,double xscale,double yscale,double rot,int color,double alpha);
void draw_background_stretched_ext(int back,double x,double y,double w,double h,int color,double alpha);
void draw_background_part_ext(int back,double left,double top,double width,double height,double x,double y,double xscale,double yscale,int color,double alpha);
void draw_background_tiled_ext(int back,double x,double y,double xscale,double yscale,int color,double alpha);
void draw_background_tiled_area_ext(int back,double x,double y,double x1,double y1,double x2,double y2, double xscale, double yscale, int color, double alpha);
void draw_background_general(int back,double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a1,double a2,double a3,double a4);

int background_get_texture(int backId);
int background_get_width(int backId);
int background_get_height(int backId);
int texture_get_width(int texId);
int texture_get_height(int texId);
void texture_set_interpolation(int enable);
bool texture_get_interpolation();
void texture_set_blending(bool enable);
void texture_set_repeat(bool repeat);
void texture_preload(int texid);
void texture_set_priority(int texid, double prio);
const unsigned fa_left = 0;
const unsigned fa_center = 1;
const unsigned fa_right = 2;
const unsigned fa_top = 0;
const unsigned fa_middle = 1;
const unsigned fa_bottom = 2;

void draw_set_halign(unsigned align);
unsigned draw_get_halign();
void draw_set_valign(unsigned align);
unsigned draw_get_valign();
void draw_set_font(int fnt);
int draw_get_font();
void draw_text(int x,int y,variant str);
void draw_text_color(int x,int y,variant str,int c1,int c2,int c3,int c4,double a);

void draw_text_ext(int x,int y,variant str, int sep, int w);
void draw_text_ext_color(int x,int y,variant str, int sep,int w,int c1,int c2,int c3,int c4,double a);

void draw_text_transformed(double x,double y,variant str,double xscale,double yscale,double rot);
void draw_text_transformed_color(double x,double y,variant str,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a);

void draw_text_ext_transformed(double x,double y,variant str,int sep,int w, double xscale,double yscale,double rot);
void draw_text_ext_transformed_color(double x,double y,variant str,int sep,int w,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a);

unsigned int font_get_texture(int fnt);
unsigned int font_get_texture_width(int fnt);
unsigned int font_get_texture_height(int fnt);

unsigned int string_width(variant str);
unsigned int string_height(variant str);

unsigned int string_width_ext(variant str, int sep, int w);
unsigned int string_height_ext(variant str, int sep, int w);

unsigned int string_width_line(variant str, int line);
unsigned int string_width_ext_line(variant str, int w, int line);
unsigned int string_width_ext_line_count(variant str, int w);

void draw_bezier_quadratic(float x1,float y1,float x2,float y2,float x3,float y3);
void draw_bezier_quadratic_color(float x1,float y1,float x2,float y2,float x3,float y3,int c1,int c2,float a1,float a2);

void draw_bezier_cubic(float x1,float y1,float x2,float y2,float x3,float y3, float x4, float y4);
void draw_bezier_cubic_color(float x1,float y1,float x2,float y2,float x3,float y3, float x4, float y4, int c1, int c2, float a1, float a2);
void draw_set_curve_mode(int mode);
void draw_set_curve_detail(int detail);
void draw_set_curve_width(int width);
float draw_bezier_quadratic_x(float x1,float y1,float x2,float y2,float x3,float y3, float t);
float draw_bezier_quadratic_y(float x1,float y1,float x2,float y2,float x3,float y3, float t);
float draw_bezier_cubic_x(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float t);
float draw_bezier_cubic_y(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float t);
void draw_bezier_quadratic_spline_part(float x1, float y1, float x2, float y2, float x3, float y3, int c1, int c2, float a1, float a2);
void draw_spline_part(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int c1, int c2, float a1, float a2);
void draw_spline2c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void draw_spline2c_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int c1, int c2, float a1, float a2);
void draw_spline3(float x1, float y1, float x2, float y2, float x3, float y3);
void draw_spline3_color(float x1, float y1, float x2, float y2, float x3, float y3, int c1, int c2, float a1, float a2);
void draw_spline3c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5);
void draw_spline3c_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, int c1, int c2, float a1, float a2);
void draw_spline4(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void draw_spline4_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int c1, int c2, float a1, float a2);
void draw_spline4c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, float x6, float y6);
void draw_spline4c_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, float x6, float y6, int c1, int c2, float a1, float a2);
void draw_spline_part(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void draw_spline_part_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int c1, int c2, float a1, float a2);
void draw_spline_begin(int mode);
int draw_spline_vertex(float x, float y);
int draw_spline_vertex_color(float x, float y, int col, float alpha);
void draw_spline_end();
void draw_bezier_quadratic_spline_end();
int draw_spline_optimized_end();

bool sound_play(int sound);
bool sound_loop(int sound);
void sound_stop(int sound);
void sound_stop_all();
void sound_volume(int sound, float volume);
void sound_global_volume(float mastervolume);
void sound_delete(int sound);
bool sound_pause(int sound);
void sound_pause_all();
bool sound_resume(int sound);
void sound_resume_all();

float sound_get_length(int sound);
float sound_get_position(int sound);
void sound_seek(int sound, float position);
void sound_seek_all(float position);

bool sound_isplaying(int sound);
bool sound_ispaused(int sound);

void sound_pan(int sound, float value);

int sound_add(string fname, int kind, bool preload);

const char* sound_get_audio_error();

bool collide_rect_line(double rx1, double ry1, double rx2, double ry2,
                       double px1, double py1, double px2, double py2);
bool collide_rect_rect(double r1x1, double r1y1, double r1x2, double r1y2,
                       double r2x1, double r2y1, double r2x2, double r2y2);
bool collide_rect_point(double rx1, double ry1, double rx2, double ry2, double px, double py);
bool collide_bbox_rect(const enigma::object_collisions* inst, double ox, double oy, double x1, double y1, double x2, double y2);
bool collide_bbox_line(const enigma::object_collisions* inst, double ox, double oy, double x1, double y1, double x2, double y2);
bool collide_bbox_bbox(const enigma::object_collisions* inst1, double ox1, double oy1, const enigma::object_collisions* inst2, double ox2, double oy2);
bool collide_bbox_point(const enigma::object_collisions* inst, double ox, double oy, double x, double y);
bool collide_bbox_circle(const enigma::object_collisions* inst, double ox, double oy, double x, double y, double r);
bool collide_bbox_ellipse(const enigma::object_collisions* inst, double ox, double oy, double x, double y, double rx, double ry);
void destroy_bbox_point(const enigma::object_collisions* inst, double ox, double oy, double x, double y);

enigma::object_collisions* const collide_inst_inst(int object, bool solid_only, bool notme, double x, double y);
enigma::object_collisions* const collide_inst_rect(int object, bool solid_only, bool notme, int x1, int y1, int x2, int y2);
enigma::object_collisions* const collide_inst_line(int object, bool solid_only, bool notme, int x1, int y1, int x2, int y2);
enigma::object_collisions* const collide_inst_point(int object, bool solid_only, bool notme, int x1, int y1);
enigma::object_collisions* const collide_inst_circle(int object, bool solid_only, bool notme, int x1, int y1, double r);
enigma::object_collisions* const collide_inst_ellipse(int object, bool solid_only, bool notme, int x1, int y1, double rx, double ry);
void destroy_inst_point(int object, bool solid_only, int x1, int y1);

bool place_free(double x,double y);
bool place_empty(double x,double y);
bool place_meeting(double x, double y, int object);
bool position_free(double x,double y);
bool position_empty(double x, double y);
bool position_meeting(double x, double y, int object);
void position_destroy_object(double x, double y, int object, bool solid_only = false);
void position_destroy_solid(double x, double y);
void position_destroy(double x, double y);
int instance_place(double x, double y, int object);
int instance_position(double x, double y, int object);
int collision_rectangle(double x1, double y1, double x2, double y2, int obj, bool prec = true, bool notme = true);
int collision_line(double x1, double y1, double x2, double y2, int obj, bool prec = true, bool notme = true);
int collision_point(double x, double y, int obj, bool prec = true, bool notme = true);
int collision_circle(double x, double y, double radius, int obj, bool prec = true, bool notme = true);
int collision_ellipse(double x1, double y1, double x2, double y2, int obj, bool prec = true, bool notme = true);

double distance_to_object(int object);
double distance_to_point(double x, double y);

double move_contact_object(int object, double angle, double max_dist = 1000000, bool solid_only = false);

inline double move_contact_all(const double direction, const double max_dist = 1000000) {
    return move_contact_object(all, direction, max_dist);
}

inline double move_contact_solid(const double direction, const double max_dist = 1000000) {
    return move_contact_object(all, direction, max_dist, true);
}

double move_outside_object(int object, double angle, double max_dist = 1000000, bool solid_only = false);

inline double move_outside_all(const double direction, const double speed = 1000000) {
    return move_outside_object(all, direction, speed);
}

inline double move_ouside_solid(const double direction, const double speed = 1000000) {
    return move_outside_object(all, direction, speed, true);
}

bool move_bounce_object(int object, bool adv = true, bool solid_only = false);
bool move_bounce_object_inside(int object, bool adv = true, bool solid_only = false);
bool move_bounce_object_outside(int object, bool adv = true, bool solid_only = false);

inline bool move_bounce_all(bool adv = true) {
    return move_bounce_object(all, adv);
}

inline bool move_bounce_solid(bool adv = true) {
    return move_bounce_object(all, adv, true);
}

void instance_deactivate_region(int rleft, int rtop, int rwidth, int rheight, int inside = true, bool notme = true);
void instance_activate_region(int left, int top, int width, int height, int inside = true);
void instance_deactivate_circle(int x, int y, int r, int inside = true, bool notme = true);
void instance_activate_circle(int x, int y, int r, int inside = true);

void position_change(double x1, double y1, int obj, bool perf);

namespace enigma {
  struct iterator {
    struct inst_iter* it;
    bool temp;

    operator bool();
    object_basic* operator*();
    object_basic* operator->();

    iterator &operator++();
    iterator operator++(int);
    iterator &operator--();
    iterator operator--(int);

    const iterator &operator=(iterator&);
    const iterator &operator=(const iterator&);
    const iterator &operator=(inst_iter*);
    const iterator &operator=(object_basic*);

    iterator(iterator&);
    iterator(const iterator&);
    iterator(inst_iter*,bool=false);
    iterator(object_basic*);
    iterator();

    ~iterator();

    private: void addme();
  };

  void update_iterators_for_destroy(const inst_iter*);
  iterator instance_list_first();
  iterator fetch_inst_iter_by_id(int id);
  iterator fetch_inst_iter_by_int(int x);
}

namespace enigma
{
  struct inst_iter
  {
    object_basic* inst;
    inst_iter *next, *prev;
    bool dead;

    inst_iter(object_basic* i,inst_iter *n,inst_iter *p);
  };

  class temp_event_scope
  {
    object_basic *oinst;
    inst_iter *oiter;

    public:
    temp_event_scope(object_basic*);
    ~temp_event_scope();
  };
  struct event_iter: inst_iter
  {

    string name;
    inst_iter *add_inst(object_basic* inst);
    void unlink(inst_iter*);
    event_iter(string name);
    event_iter();
  };
  struct objectid_base: inst_iter
  {

    size_t count;
    inst_iter *add_inst(object_basic* inst);
    objectid_base();
  };

  struct iterator_level {
    inst_iter* it;
    object_basic* other;
    iterator_level *last;
    iterator_level(inst_iter*,object_basic* oth,iterator_level*);
    void push(inst_iter*,object_basic* oth);
    void pop();
  };
  extern iterator_level *il_base, *il_top;
  extern event_iter *events;
  extern objectid_base *objects;
  extern object_basic *ENIGMA_global_instance;
  extern inst_iter *instance_event_iterator;
  extern object_basic *instance_other;

  object_basic* fetch_instance_by_int(int x);
  object_basic* fetch_instance_by_id(int x);
}
extern int instance_count;
extern bool argument_relative;

inline bool action_if_object(const int object, const double xx, const double yy) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        return place_meeting(inst->x+xx,inst->y+yy,object);
    }
    else {
        return place_meeting(xx,yy,object);
    }
    return false;
}

inline bool action_if_empty(const double xx, const double yy, const int objects) {
    if (argument_relative) {
        if (objects == 0)
            return place_free(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x+xx,((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y+yy);
        else
            return place_empty(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x+xx,((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y+yy);
    }
    else {
        if (objects == 0)
            return place_free(xx,yy);
        else
            return place_empty(xx,yy);
    }
    return false;
}

inline void action_move_contact(const double direction, const double max_dist, const bool against) {
    move_contact_object(direction, max_dist, all, !against);
}

inline void action_bounce(int precisely, bool against) {
    move_bounce_object(precisely, all, !against);
}

inline void action_kill_position(double x, double y)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        position_destroy(inst->x+x, inst->y+y);
    }
    else
    {
        position_destroy(x, y);
    }
}

inline bool action_if_collision(const double x, const double y, const int object) {
  return !action_if_empty(x,y,object);
}

void action_set_alarm(int steps,int alarmno);
namespace enigma {
  struct extension_alarm
  {
    var alarm;
    extension_alarm();
  };
}
namespace enigma {
  struct extension_path
  {
    int path_index;
    int path_endaction;
    bool path_orientation;
    double path_position;
    double path_positionprevious;
    double path_scale;
    double path_speed;
    extension_path(): path_index(-1), path_endaction(0), path_orientation(0), path_position(0), path_positionprevious(0), path_scale(1), path_speed(0) {}
  };
}
unsigned mp_grid_create(int left,int top,int hcells,int vcells,int cellwidth,int cellheight, double speed_modifier = 1);
void mp_grid_destroy(unsigned id);
unsigned mp_grid_duplicate(unsigned id);
void mp_grid_copy(unsigned id, unsigned srcid);
unsigned mp_grid_get_cell(unsigned id,int h,int v);
void mp_grid_draw(unsigned id, unsigned mode = 0, unsigned color_mode = 0);
void mp_grid_draw_neighbours(unsigned id,int h,int v, int mode = 0);
bool mp_grid_path(unsigned id,unsigned path,double xstart,double ystart,double xgoal,double ygoal,bool allowdiag);
void mp_grid_clear_all(unsigned id, unsigned cost = 1);
void mp_grid_clear_cell(unsigned id,int h,int v, unsigned cost = 1);
void mp_grid_clear_rectangle(unsigned id,double x1,double y1,double x2,double y2, unsigned cost = 1);
void mp_grid_add_rectangle(unsigned id,double x1,double y1,double x2,double y2, unsigned cost = 50000);
void mp_grid_add_cell(unsigned id,int h,int v, unsigned cost = 50000);
void mp_grid_add_instances(unsigned id,int obj,bool prec, unsigned cost = 50000);
unsigned mp_grid_get_threshold(unsigned id);
void mp_grid_set_threshold(unsigned id, unsigned threshold = 1);
void mp_grid_reset_threshold(unsigned id);
double mp_grid_get_speed_modifier(unsigned id);
void mp_grid_get_speed_modifier(unsigned id, double value);
void mp_potential_settings(double maxrot, double rotstep, double ahead, double onspot);
bool mp_potential_step_object(const double x, const double y, const double stepsize, const int object, const bool solid_only = false);

inline bool mp_potential_step(const double x, const double y, const double stepsize, const bool checkall) {
    return mp_potential_step_object(x, y, stepsize, all, !checkall);
}

bool mp_potential_path_object(int path, const double x, const double y, const double stepsize, double factor, const int object, const bool solid_only = false);

inline bool mp_potential_path(int path, const double x, const double y, const double stepsize, double factor, const bool checkall, const bool solid_only = false) {
    return mp_potential_path_object(path, x, y, stepsize, factor, all, checkall ? false : true);
}

bool mp_linear_step_object(const double x, const double y, const double stepsize, const int object, const bool solid_only = false);

inline bool mp_linear_step(const double x, const double y, const double stepsize, const bool checkall) {
    return mp_linear_step_object(x, y, stepsize, all, !checkall);
}

bool mp_linear_path_object(int path, const double x, const double y, const double stepsize, const int object, const bool solid_only = false);

inline bool mp_linear_path(int path, const double x, const double y, const double stepsize, const bool checkall) {
    return mp_linear_path_object(path, x, y, stepsize, all, !checkall);
}
extern bool argument_relative;

inline void action_linear_step(double x, double y, double stepsize, bool solid_only)
{
    if (argument_relative)
    {
        mp_linear_step_object(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x+x, ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y+y, stepsize,all, solid_only);
    }
    else
    {
        mp_linear_step_object(x, y, stepsize,all, solid_only);
 }
}

inline void action_potential_step(double x, double y, double stepsize, bool solid_only)
{
    if (argument_relative)
    {
        mp_potential_step_object(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x+x, ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y+y, stepsize,all, solid_only);
    }
    else
    {
        mp_potential_step_object(x, y, stepsize,all, solid_only);
 }
}

namespace enigma {
  struct extension_mp
  {
  };
}

time_t date_current_datetime();
time_t date_current_date();
time_t date_current_time();
time_t date_create_datetime(int year,int month,int day,int hour,int minute,int second);
time_t date_create_date(int year,int month,int day);
time_t date_create_time(int hour,int minute,int second);
int date_get_year(time_t date);
int date_get_month(time_t date);
int date_get_week(time_t date);
int date_get_day(time_t date);
int date_get_hour(time_t date);
int date_get_minute(time_t date);
int date_get_second(time_t date);
int date_get_weekday(time_t date);
int date_get_day_of_year(time_t date);
int date_get_hour_of_year(time_t date);
int date_get_minute_of_year(time_t date);
int date_get_second_of_year(time_t date);

double date_year_span(time_t date1,time_t date2);
double date_month_span(time_t date1,time_t date2);
double date_week_span(time_t date1,time_t date2);
double date_day_span(time_t date1,time_t date2);
double date_hour_span(time_t date1,time_t date2);
double date_minute_span(time_t date1,time_t date2);
double date_second_span(time_t date1,time_t date2);

int date_compare_datetime(time_t date1,time_t date2);
int date_compare_date(time_t date1,time_t date2);
int date_compare_time(time_t date1,time_t date2);

time_t date_date_of(time_t date);
time_t date_time_of(time_t date);

int date_days_in_month(time_t date);
int date_days_in_year(time_t date);
bool date_leap_year(time_t date);
bool date_is_today(time_t date);

time_t date_inc_year(time_t date,int amount);
time_t date_inc_month(time_t date,int amount);
time_t date_inc_week(time_t date,int amount);
time_t date_inc_day(time_t date,int amount);
time_t date_inc_hour(time_t date,int amount);
time_t date_inc_minute(time_t date,int amount);
time_t date_inc_second(time_t date,int amount);

bool date_valid_datetime(int year,int month,int day,int hour,int minute,int second);
bool date_valid_date(int year,int month,int day);
bool date_valid_time(int hour,int minute,int second);

string date_datetime_string(time_t date);
string date_date_string(time_t date);
string date_time_string(time_t date);
string date_datetime_stringf(time_t date,string format);

namespace enigma {
  struct extension_datetime
  {
  };
}

unsigned int ds_grid_create(const unsigned int w, const unsigned int h);
void ds_grid_destroy(const unsigned int id);
void ds_grid_clear(const unsigned int id, const variant val);
void ds_grid_copy(const unsigned int id, const unsigned int source);
void ds_grid_resize(const unsigned int id, const unsigned int w, const unsigned int h);
unsigned int ds_grid_width(const unsigned int id);
unsigned int ds_grid_height(const unsigned int id);
void ds_grid_set(const unsigned int id, const unsigned int x, const unsigned int y, const variant val);
void ds_grid_add(const unsigned int id, const unsigned int x, const unsigned int y, const variant val);
void ds_grid_multiply(const unsigned int id, const unsigned int x, const unsigned int y, const double val);
void ds_grid_set_region(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const variant val);
void ds_grid_add_region(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const variant val);
void ds_grid_multiply_region(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const double val);
void ds_grid_set_disk(const unsigned int id, const double x, const double y, const double r, const variant val);
void ds_grid_add_disk(const unsigned int id, const double x, const double y, const double r, const variant val);
void ds_grid_multiply_disk(const unsigned int id, const double x, const double y, const double r, const double val);
void ds_grid_set_grid_region(const unsigned int id, const unsigned int source, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const unsigned int xpos, const unsigned int ypos);
void ds_grid_add_grid_region(const unsigned int id, const unsigned int source, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const unsigned int xpos, const unsigned int ypos);
void ds_grid_multiply_grid_region(const unsigned int id, const unsigned int source, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const unsigned int xpos, const unsigned int ypos);
variant ds_grid_get(const unsigned int id, const unsigned int x, const unsigned int y);
variant ds_grid_get_sum(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2);
variant ds_grid_get_max(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2);
variant ds_grid_get_min(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2);
variant ds_grid_get_mean(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2);
variant ds_grid_get_disk_sum(const unsigned int id, const double x, const double y, const double r);
variant ds_grid_get_disk_max(const unsigned int id, const double x, const double y, const double r);
variant ds_grid_get_disk_min(const unsigned int id, const double x, const double y, const double r);
variant ds_grid_get_disk_mean(const unsigned int id, const double x, const double y, const double r);
bool ds_grid_value_exists(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const variant val);
int ds_grid_value_x(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const variant val);
int ds_grid_value_y(const unsigned int id, const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2, const variant val);
bool ds_grid_value_disk_exists(const unsigned int id, const double x, const double y, const double r, const variant val);
bool ds_grid_value_disk_x(const unsigned int id, const double x, const double y, const double r, const variant val);
bool ds_grid_value_disk_y(const unsigned int id, const double x, const double y, const double r, const variant val);
void ds_grid_shuffle(const unsigned int id);
bool ds_grid_exists(const unsigned int id);
unsigned int ds_grid_duplicate(const unsigned int source);

unsigned int ds_map_create();
void ds_map_destroy(const unsigned int id);
void ds_map_clear(const unsigned int id);
void ds_map_copy(const unsigned int id, const unsigned int source);
unsigned int ds_map_size(const unsigned int id);
bool ds_map_empty(const unsigned int id);
void ds_map_add(const unsigned int id, const variant key, const variant val);
void ds_map_replace(const unsigned int id, const variant key, const variant val);
void ds_map_delete(const unsigned int id, const variant key);
void ds_map_delete(const unsigned int id, const variant first, const variant last);
bool ds_map_exists(const unsigned int id, const variant key);
variant ds_map_find_value(const unsigned int id, const variant key);
variant ds_map_find_previous(const unsigned int id, const variant key);
variant ds_map_find_next(const unsigned int id, const variant key);
variant ds_map_find_first(const unsigned int id);
variant ds_map_find_last(const unsigned int id);
bool ds_map_exists(const unsigned int id);
unsigned int ds_map_duplicate(const unsigned int source);

unsigned int ds_list_create();
void ds_list_destroy(const unsigned int id);
void ds_list_clear(const unsigned int id);
void ds_list_copy(const unsigned int id, const unsigned int source);
unsigned int ds_list_size(const unsigned int id);
bool ds_list_empty(const unsigned int id);
void ds_list_add(const unsigned int id, const variant val);
void ds_list_insert(const unsigned int id, const unsigned int pos, const variant val);
void ds_list_replace(const unsigned int id, const unsigned int pos, const variant val);
void ds_list_delete(const unsigned int id, const unsigned int pos);
void ds_list_delete(const unsigned int id, const unsigned int first, const unsigned int last);
int ds_list_find_index(const unsigned int id, const variant val);
variant ds_list_find_value(const unsigned int id, const unsigned int pos);
void ds_list_sort(const unsigned int id, const bool ascend);
void ds_list_shuffle(const unsigned int id);
bool ds_list_exists(const unsigned int id);
unsigned int ds_list_duplicate(const unsigned int source);

unsigned int ds_priority_create();
void ds_priority_destroy(const unsigned int id);
void ds_priority_clear(const unsigned int id);
void ds_priority_copy(const unsigned int id, const unsigned int source);
unsigned int ds_priority_size(const unsigned int id);
bool ds_priority_empty(const unsigned int id);
void ds_priority_add(const unsigned int id, const variant val, const variant prio);
void ds_priority_change_priority(const unsigned int id, const variant val, const variant prio);
variant ds_priority_find_priority(const unsigned int id, const variant val);
void ds_priority_delete_value(const unsigned int id, const variant val);
bool ds_priority_value_exists(const unsigned int id, const variant val);
variant ds_priority_delete_min(const unsigned int id);
variant ds_priority_find_min(const unsigned int id);
variant ds_priority_delete_max(const unsigned int id);
variant ds_priority_find_max(const unsigned int id);
bool ds_priority_exists(const unsigned int id);
unsigned int ds_priority_duplicate(const unsigned int source);

unsigned int ds_queue_create();
void ds_queue_destroy(const unsigned int id);
void ds_queue_clear(const unsigned int id);
void ds_queue_copy(const unsigned int id, const unsigned int source);
unsigned int ds_queue_size(const unsigned int id);
bool ds_queue_empty(const unsigned int id);
void ds_queue_enqueue(const unsigned int id, const variant val);
variant ds_queue_dequeue(const unsigned int id);
variant ds_queue_head(const unsigned int id);
variant ds_queue_tail(const unsigned int id);
bool ds_queue_exists(const unsigned int id);
unsigned int ds_queue_duplicate(const unsigned int source);

unsigned int ds_stack_create();
void ds_stack_destroy(const unsigned int id);
void ds_stack_clear(const unsigned int id);
void ds_stack_copy(const unsigned int id, const unsigned int source);
unsigned int ds_stack_size(const unsigned int id);
bool ds_stack_empty(const unsigned int id);
void ds_stack_push(const unsigned int id, const variant val);
variant ds_stack_pop(const unsigned int id);
variant ds_stack_top(const unsigned int id);
bool ds_stack_exists(const unsigned int id);
unsigned int ds_stack_duplicate(const unsigned int source);

namespace enigma {
  struct extension_ds
  {
  };
}
bool argument_relative=false;
string caption_score="Score:", caption_lives="Lives:", caption_health="Health:";

double fps;
unsigned int game_id;

double health=100;
int *instance_id;
int keyboard_key=0;
string keyboard_string="";

double lives=3;
double score=0;
bool secure_mode=false;
bool show_score=0, show_lives=0, show_health=0;
int transition_kind=0;
int transition_steps=80;
bool automatic_redraw = true;
string program_directory="";
int os_type;
int gamemaker_version=0;

int cursor_sprite;

namespace enigma
{
  extern int nodestroy;
}
using namespace std;

string toString(int);
string toString(long);
string toString(short);
string toString(unsigned);
string toString(unsigned long);
string toString(unsigned short);
string toString(long long n);
string toString(unsigned long long n);
string toString(char);
string toString(char*);
string toString(float);
string toString(double);

string toString();
string toString(const string& str);
string toString(const string& str, size_t pos, size_t n = string::npos);
string toString(const char *s, size_t n);
string toString(const char *s);
string toString(size_t n, char c);

string toString(const struct var&);
string toString(const struct variant&);
namespace enigma
{
  static inline int switch_hash(variant x) {
    if (!x.type)
      if (x == int(x))
        return int(x);
      else
        return int(x * 65536);
    else
    {
      int ret = 0;
      const string& n = x.sval;
      for (size_t i = 0; i < n.length(); i++)
        ret = 31*ret + n[i];
      return ret;
    }
  }
}

namespace enigma
{
  extern char mousestatus[3];
  extern char last_mousestatus[3];
  extern char last_keybdstatus[256];
  extern char keybdstatus[256];
}

enum {
  mb_any = -1,
  mb_none = 0,
  mb_left = 1,
  mb_right = 2,
  mb_middle = 3,
};

enum {
  vk_anykey = 1,
  vk_nokey = 0,

  vk_left = 37,
  vk_right = 39,
  vk_up = 38,
  vk_down = 40,

  vk_tab = 9,
  vk_enter = 13,
  vk_shift = 16,
  vk_control= 17,
  vk_alt = 18,
  vk_space = 32,

  vk_numpad0 = 96,
  vk_numpad1 = 97,
  vk_numpad2 = 98,
  vk_numpad3 = 99,
  vk_numpad4 = 100,
  vk_numpad5 = 101,
  vk_numpad6 = 102,
  vk_numpad7 = 103,
  vk_numpad8 = 104,
  vk_numpad9 = 105,

  vk_multiply = 106,
  vk_add = 107,
  vk_subtract = 109,
  vk_decimal = 110,
  vk_divide = 111,

  vk_f1 = 112,
  vk_f2 = 113,
  vk_f3 = 114,
  vk_f4 = 115,
  vk_f5 = 116,
  vk_f6 = 117,
  vk_f7 = 118,
  vk_f8 = 119,
  vk_f9 = 120,
  vk_f10 = 121,
  vk_f11 = 122,
  vk_f12 = 123,

  vk_backspace = 8,
  vk_escape = 27,
  vk_pageup = 33,
  vk_pagedown = 34,
  vk_end = 35,
  vk_home = 36,
  vk_insert = 45,
  vk_delete = 46,
  vk_lcontrol = 162,
  vk_rcontrol = 163,
  vk_lalt = 164,
  vk_ralt = 165,
  vk_printscreen = 42,
  vk_caps = 20,
  vk_scroll = 145,
  vk_pause = 19,
  vk_lsuper = 91,
  vk_rsuper = 92,
};
extern double mouse_x, mouse_y;
extern int mouse_button, mouse_lastbutton;
extern string keyboard_lastchar;

bool mouse_check_button(int button);
bool mouse_check_button_pressed(int button);
bool mouse_check_button_released(int button);

extern short mouse_hscrolls;
extern short mouse_vscrolls;

bool keyboard_check(int key);
bool keyboard_check_pressed(int key);
bool keyboard_check_released(int key);
bool mouse_wheel_up();
bool mouse_wheel_down();

void io_clear();
extern int amain();

namespace enigma{
 char* load_bitmap(string filename,int* width,int* height, int* fullwidth, int* fullheight);
}
namespace enigma {
  extern int destroycalls, createcalls;
}

int instance_create(int x,int y,int object);

namespace enigma {
  object_basic *instance_create_id(int x,int y,int object,int idg);
}

void instance_deactivate_all(bool notme);
void instance_activate_all();
void instance_activate_object(int obj);
void instance_deactivate_object(int obj);

void instance_destroy();
void instance_destroy(int id);
bool instance_exists (int obj);
int instance_find (int obj,int n);
int instance_number (int obj);
int instance_nearest (int x,int y,int obj,bool notme = false);
int instance_furthest(int x,int y,int obj,bool notme = false);
void instance_change(int obj, bool perf = false);
void instance_copy(bool perf = true);
inline void action_change_object(int obj, bool perf);

int room_goto(int roomind);
int room_restart();
string room_get_name(int index);
int room_goto_absolute(int index);
int room_goto_first();
int room_goto_previous();
int room_goto_next();
int room_next(int num);
int room_previous(int num);
bool room_exists(unsigned roomid);

extern int background_color;
extern int background_showcolor;

extern var background_visible, background_foreground, background_index, background_x, background_y, background_htiled,
background_vtiled, background_hspeed, background_vspeed,background_alpha;

extern int room_first;
extern int room_height;
extern int room_last;
extern int room_persistent;
extern int room_speed;
extern int room_width;

extern var room_caption;

int room_count();

extern int view_current;
extern int view_enabled;
typedef var rvt;
extern rvt view_hborder, view_hport, view_hspeed, view_hview, view_object, view_vborder, view_visible,
           view_vspeed, view_wport, view_wview, view_xport, view_xview, view_yport, view_yview,view_angle;
namespace enigma
{
  struct inst {
    int id,obj,x,y;
  };
    struct tile {
        int id,bckid,bgx,bgy,depth,height,width,roomX,roomY;
    };
  struct viewstruct
  {
    int start_vis;
    int area_x,area_y,area_w,area_h;
    int port_x,port_y,port_w,port_h;
    int object2follow;
    int hborder,vborder,hspd,vspd;
  };
  struct backstruct {
    int visible;
    int foreground;
    int background;
    int area_x, area_y, horSpeed, verSpeed;
    int tileHor, tileVert;
    int stretch;
  };
  struct roomstruct
  {
    int id;
    int order;
    string name;
    string cap;

    int backcolor;
    bool drawbackcolor;
    void(*createcode)();
    int width, height, spd;
    int views_enabled;
    viewstruct views[8];

    backstruct backs[8];

    int instancecount;
    inst *instances;
    int tilecount;
    tile *tiles;

    void gotome(bool=false);
  };
  void room_update();
  extern int room_max, maxid;
  void rooms_load();
  void game_start();
}
namespace enigma { struct roomv: multifunction_variant {
  using multifunction_variant::operator=; roomv &operator=(roomv&);;
  void function(variant oldval);
}; }
extern enigma::roomv room;
namespace enigma
{
  extern double mouse_xprevious, mouse_yprevious;
  void update_globals();
}
bool collision_bbox_rect(int object,double x1,double y1,double x2,double y2);
int collision_bbox_rect_first(int object,double x1,double y1,double x2,double y2);
namespace enigma
{

  typedef struct winstance_list_iterator *pinstance_list_iterator;
  void winstance_list_iterator_delete(pinstance_list_iterator);
  pinstance_list_iterator link_instance(object_basic* who);
  inst_iter *link_obj_instance(object_basic* who, int oid);
  void instance_iter_queue_for_destroy(pinstance_list_iterator whop);
  void dispose_destroyed_instances();
  void unlink_main(pinstance_list_iterator);
  void unlink_object_id_iter(inst_iter*,int);
}
namespace enigma {
  struct callable_script {
    variant (*base)();
    int argnum;
  };
  struct nameid_pair {
    string name; int id;
  };
  void map_resource_ids(nameid_pair* n);
  extern callable_script callable_scripts[];
  extern nameid_pair resource_nameids[];
};

int resource_get_id(string name);
variant script_execute(int scr, variant arg0 = 0, variant arg1 = 0, variant arg2 = 0, variant arg3 = 0, variant arg4 = 0, variant arg5 = 0, variant arg6 = 0, variant arg7 = 0, variant arg8 = 0, variant arg9 = 0, variant arg10 = 0, variant arg11 = 0, variant arg12 = 0, variant arg13 = 0, variant arg14 = 14, variant arg15 = 0);

namespace enigma {

class playerScore
{
public:
    std::string player_name;
    int player_score;

    playerScore();
    playerScore(std::string name, int score);
    bool operator<(const playerScore& other) const
    {
        return player_score < other.player_score;
    }
    bool operator>(const playerScore& other) const
    {
        return player_score > other.player_score;
    }
};

}

void highscore_show(int numb);

void action_highscore_show(int background,int border,double newColor,double otherColor,std::string font,int a,int b,int c,int d,int e,int f);

void highscore_set_background(int back) ;

void highscore_set_border(bool show) ;

void highscore_set_font(std::string name, int size, int style) ;

void highscore_set_colors(int back, int newcol, int othercol) ;

void highscore_set_strings(std::string caption, std::string nobody, std::string escape) ;
void highscore_show_ext(int numb, int back, int show, int newcol, int othercol, std::string name, int size) ;
void highscore_clear() ;
void highscore_add(std::string str, int numb) ;
void highscore_add_current() ;
int highscore_value(int place);
std::string highscore_name(int place) ;
void draw_highscore(int x1, int y1, int x2, int y2) ;namespace enigma {

    void highscore_init();
}

void path_start(unsigned pathid,double speed,unsigned endaction,bool absolute);
void path_end();
void path_set_position(double position, bool relative);
void path_set_speed(double speed, bool relative);
bool path_update();
bool path_exists(unsigned pathid);
void path_delete(unsigned pathid);
void path_assign(unsigned pathid,unsigned path);
void path_append(unsigned pathid,unsigned path);
int path_add();
int path_duplicate(unsigned pathid);
void path_copy(unsigned pathid,unsigned srcid);
int path_get_number(unsigned pathid);
double path_get_point_x(unsigned pathid, unsigned n);
double path_get_point_y(unsigned pathid, unsigned n);
double path_get_point_length(unsigned pathid, unsigned n);
double path_get_point_speed(unsigned pathid, unsigned n);
double path_get_center_x(unsigned pathid);
double path_get_center_y(unsigned pathid);
int path_get_precision(unsigned pathid);
double path_get_length(unsigned pathid);
bool path_get_kind(unsigned pathid);
bool path_get_closed(unsigned pathid);
void path_set_kind(unsigned pathid, bool val);
void path_set_closed(unsigned pathid, bool val);
void path_set_precision(unsigned pathid, int prec);
void path_clear_points(unsigned pathid);
void path_add_point(unsigned pathid,double x,double y,double speed);
void path_insert_point(unsigned pathid,unsigned n,double x,double y,double speed);
void path_change_point(unsigned pathid,unsigned n,double x,double y,double speed);
void path_delete_point(unsigned pathid,unsigned n);
void path_reverse(unsigned pathid);
void path_shift(unsigned pathid,double xshift,double yshift);
void path_flip(unsigned pathid);
void path_mirror(unsigned pathid);
void path_scale(unsigned pathid,double xscale,double yscale);
void path_rotate(unsigned pathid,double angle);
double path_get_x(unsigned pathid, double t);
double path_get_y(unsigned pathid, double t);
double path_get_speed(unsigned pathid, double t);
double path_get_direction(unsigned pathid, double t);
void draw_path(unsigned path,double x,double y,bool absolute);
void motion_set(int dir, double newspeed);
void motion_add(double newdirection, double newspeed);
void move_snap(const double hsnap, const double vsnap);

void move_wrap(const bool hor, const bool vert, const double margin);
void move_towards_point (const double point_x, const double point_y, const double newspeed);
bool place_snapped(int hsnap, int vsnap);
void move_random(const double snapHor, const double snapVer);
extern bool argument_relative;

inline bool action_if_variable(const variant& variable, const variant& value, int operation) {
    switch (operation)
    {
        case 0: return (variable==value); break;
        case 1: return (variable<value); break;
        case 2: return (variable>value); break;
        default: return false;
    }
}

inline void action_move_to(const double xx, const double yy) {
    if (argument_relative) {
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x+=xx;
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y+=yy;
    }
    else {
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->x=xx;
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->y=yy;
 }
}

inline void action_set_score(double newscore) {
    if (argument_relative) score+= (int)newscore;
    else score = (int)newscore;
}

inline void action_set_life(double newlives) {
    if (argument_relative) lives+= (int)newlives;
    else lives = newlives;
}

inline void action_set_caption(const int vscore, const string scoreCaption, const int vlives,string livesCaption, const int vhealth, const string healthCaption) {
    show_score=vscore;
    caption_score=scoreCaption;
    show_lives=vlives;
    caption_lives=livesCaption;
    show_health=vhealth;
    caption_health=healthCaption;
}

inline void action_sound(int snd, bool loop)
{
    (loop ? sound_loop:sound_play)(snd);
}
inline void action_color(const int color) {
 draw_set_color(color);
}

inline bool action_if_number(const int object, const double number, const int operation) {
 switch (operation)
 {
     case 0: return (instance_number(object) == number); break;
     case 1: return (instance_number(object) < number); break;
     case 2: return (instance_number(object) > number); break;
     default: return false;
  }
}

inline void action_kill_object() { instance_destroy(); }

inline void action_set_vspeed(const double newvspeed) {
    if (argument_relative) {
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed+=newvspeed;
    } else
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed=newvspeed;
}

inline void action_set_hspeed(const double newhspeed) {
 if (argument_relative) {
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->hspeed+=newhspeed;
    } else
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->hspeed=newhspeed;
}

inline void action_set_gravity(const double direction, const double newgravity) {
    if (argument_relative) {
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->gravity_direction+=direction;
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->gravity+=newgravity;
    } else {
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->gravity_direction=direction;
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->gravity=newgravity;
    }
}

inline void action_set_friction(const double newfriction) {
 if (argument_relative) {
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->friction+=newfriction;
    } else {
        ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->friction=newfriction;
    }
}

inline bool action_if_dice(double sides) {
    if (sides == 0) {return false;}
    return (random(1) < (double)1/sides);
}

inline void action_move_point(const double x, const double y, const double speed) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        move_towards_point(x+inst->x,y+inst->y,speed); }
    else move_towards_point(x,y,speed);
}

inline bool action_if(const double x) {
    return x != 0;
}

inline bool action_if_previous_room()
{
    return room_previous(room) != -1;
}

inline bool action_if_next_room()
{
 return room_next(room) != -1;
}
inline void action_move(const char dir[9], int argspeed) {
    int dirs[9] = { 225, 270, 315, 180, -1, 0, 135, 90, 45 };
    int chosendirs[9];
    int choices = 0;
    for (int i = 0; i < 9; i++)
        if (dir[i] == '1') chosendirs[choices++] = dirs[i];
    if (choices == 0) return;
    choices = int(random(choices));
    const double newdir =
    ((enigma::object_planar*)enigma::instance_event_iterator->inst)->direction.rval.d = chosendirs[choices];
    if (argument_relative)
        argspeed += ((enigma::object_planar*)enigma::instance_event_iterator->inst)->speed;
    const double newspd =
    ((enigma::object_planar*)enigma::instance_event_iterator->inst)->speed.rval.d = chosendirs[choices] == -1 ? 0 : argspeed;
    ((enigma::object_planar*)enigma::instance_event_iterator->inst)->hspeed.rval.d = newspd * cos(degtorad(newdir));
    ((enigma::object_planar*)enigma::instance_event_iterator->inst)->vspeed.rval.d = -newspd * sin(degtorad(newdir));
}

inline void action_reverse_xdir() {
    ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->hspeed=-((enigma::object_graphics*)enigma::instance_event_iterator->inst)->hspeed;
}

inline void action_reverse_ydir() {
 ((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed=-((enigma::object_graphics*)enigma::instance_event_iterator->inst)->vspeed;
}

inline bool action_if_aligned(const double snapHor, const double snapVer) {
 return place_snapped(snapHor, snapVer);
}

inline void action_move_start() {
    enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
 inst->x=inst->xstart;
 inst->y=inst->ystart;
}

inline void action_execute_script(string script,string argument0,string argument1,string argument2,string argument3,string argument4) {}
inline void action_draw_rectangle(const double x1, const double y1, const double x2, const double y2, const int filled) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_rectangle(x1+inst->x,y1+inst->y,x2+inst->x,y2+inst->y,filled);
    }
    else draw_rectangle(x1,y1,x2,y2,filled);
}

inline void action_sprite_set(const double spritep, const double subimage, const double speed) {
    enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);
    inst->sprite_index=spritep;
 if (subimage !=-1) inst->image_index=subimage;
 inst->image_speed=speed;
}

inline void action_draw_text(const string text, const double x, const double y) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text(x+inst->x,y+inst->y,text); }
    else {
        draw_text(x,y,text);
    }
}

inline void action_sleep(const double milliseconds, const int redraw) {
    if (redraw) {screen_redraw();}
    sleep(milliseconds/1000);
}

inline void action_current_room(const int transition) {

    room_restart();
}

inline void action_previous_room(const int transition) {

    room_goto_previous();
}

inline void action_next_room(const int transition) {

    room_goto_next();
}

inline void action_another_room(const int roomind, const int transition) {

 room_goto(roomind);
}

inline void action_font(const int font, const int align) {
    draw_set_font(font);

}
inline void action_wrap(const int direction) {
    switch (direction)
    {
        case 0: move_wrap(1,0,0); break;
        case 1: move_wrap(0,1,0); break;
        case 2: move_wrap(1,1,0); break;
    }
}

inline void action_set_motion(const double dir, const double nspeed) {
    enigma::object_graphics* const inst = ((enigma::object_graphics*)enigma::instance_event_iterator->inst);
    if (argument_relative) {
        inst->hspeed+= (nspeed) * cos(degtorad((dir)));
        inst->vspeed-= (nspeed) * sin(degtorad((dir)));
    }
    else {
        inst->direction=dir;
        inst->speed=nspeed;
    }
}

inline void game_restart() {
    room_goto_first();
}
static void show_info() {}
static inline void action_show_info() {show_info();}
inline void action_create_object(const int object, const double x, const double y)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        instance_create(inst->x+x, inst->y+y, object);
    }
    else
        instance_create(x, y, object);
}

void action_create_object_random(const int object1, const int object2, const int object3, const int object4, const double x, const double y);
void action_create_object_random(const int object1, const int object2, const int object3, const int object4, const double x, const double y)
{
    int obj_ar[4], obj_num = 0;
    if (object1 != -1)
        obj_ar[obj_num++] = object1;
    if (object2 != -1)
        obj_ar[obj_num++] = object2;
    if (object3 != -1)
        obj_ar[obj_num++] = object3;
    if (object4 != -1)
        obj_ar[obj_num++] = object4;
    const int object = obj_ar[int(random(obj_num))];
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        instance_create(inst->x+x, inst->y+y, object);
    }
    else
        instance_create(x, y, object);
}

inline void action_draw_arrow(const double x1, const double y1, const double x2, const double y2, const double tipSize) {
    draw_arrow(x1, y1, x2, y2, tipSize, 1, false);
}

inline void action_draw_background(const int background, const double x, const double y, const int tiled)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        (tiled ? draw_background_tiled : draw_background)(background, inst->x+x, inst->y+y);
    }
    else
      (tiled ? draw_background_tiled : draw_background)(background, x, y);
}

inline void action_draw_ellipse(const double x1, const double y1, const double x2, const double y2, const int filled)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_ellipse(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, filled);
    }
    else
        draw_ellipse(x1, y1, x2, y2, filled);
}

inline void action_draw_ellipse_gradient(const double x1, const double y1, const double x2, const double y2, const int color1, const int color2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_ellipse_color(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, color1, color2, false);
    }
    else
        draw_ellipse_color(x1, y1, x2, y2, color1, color2, false);
}

inline void action_draw_gradient_hor(const double x1, const double y1, const double x2, const double y2, const int color1, const int color2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_rectangle_color(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, color1, color2, color1, color2, false);
    }
    else
        draw_rectangle_color(x1, y1, x2, y2, color1, color2, color1, color2, false);
}

inline void action_draw_gradient_vert(const double x1, const double y1, const double x2, const double y2, const int color1, const int color2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_rectangle_color(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2, color1, color1, color2, color2, false);
    }
    else
        draw_rectangle_color(x1, y1, x2, y2, color1, color1, color2, color2, false);
}

inline void action_draw_score(const double x, const double y, const string caption) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text(x+inst->x,y+inst->y,caption+toString(score));
    } else draw_text(x,y,caption+toString(score));
}

inline void action_draw_sprite(const int sprite, const double x, const double y, const int subimage) {
    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_sprite(sprite,subimage,x+inst->x,y+inst->y);
    }
    else
        draw_sprite(sprite,subimage,x,y);
}

inline void action_set_health(double value) {
    if (argument_relative) health+= (int)value;
    else health = value;
}

void action_draw_health(const double x1, const double y1, const double x2, const double y2, const double backColor, const int barColor);
void action_draw_health(const double x1, const double y1, const double x2, const double y2, const double backColor, const int barColor) {
  double realbar1, realbar2;
  switch (barColor)
  {
      case 0: realbar1=c_green; realbar2=c_red; break;
      case 1: realbar1=c_white; realbar2=c_black; break;
      case 2: realbar1=c_black; realbar2=c_black; break;
      case 3: realbar1=c_gray; realbar2=c_gray; break;
      case 4: realbar1=c_silver; realbar2=c_silver; break;
      case 5: realbar1=c_white; realbar2=c_white; break;
      case 6: realbar1=c_maroon; realbar2=c_maroon; break;
      case 7: realbar1=c_green; realbar2=c_green; break;
      case 8: realbar1=c_olive; realbar2=c_olive; break;
      case 9: realbar1=c_navy; realbar2=c_navy; break;
      case 10: realbar1=c_purple; realbar2=c_purple; break;
      case 11: realbar1=c_teal; realbar2=c_teal; break;
      case 12: realbar1=c_red; realbar2=c_red; break;
      case 13: realbar1=c_lime; realbar2=c_lime; break;
      case 14: realbar1=c_yellow; realbar2=c_yellow; break;
      case 15: realbar1=c_blue; realbar2=c_blue; break;
      case 16: realbar1=c_fuchsia; realbar2=c_fuchsia; break;
      case 17: realbar1=c_aqua; realbar2=c_aqua; break;
      default: realbar1=c_green; realbar2=c_red;
  }
 if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_healthbar(x1+inst->x, y1+inst->y, x2+inst->x, y2+inst->y, health, backColor, realbar2, realbar1, 1, 1, 1);
 }
 else
        draw_healthbar(x1, y1, x2, y2, health, backColor, realbar2, realbar1, 1, 1, 1);
}

inline void action_draw_life(const double x, const double y, const string caption)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text(inst->x+x, inst->y+y, caption + toString(lives));
    }
    else
        draw_text(x, y, caption + toString(lives));
}

inline void action_draw_life_images(const double x, const double y, const int image) {
    int actualX=x, actualY=y;
    const int width = sprite_get_width(image);

    if (argument_relative) {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        actualX+=inst->x;
        actualY+=inst->y;
    }

    for (int i=0; i<lives; i++)
        draw_sprite(image,-1, actualX+(i*width), actualY);
}

inline void action_draw_line(const double x1, const double y1, const double x2, const double y2)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_line(inst->x+x1, inst->y+y1, inst->x+x2, inst->y+y2);
    }
    else
        draw_line(x1, y1, x2, y2);
}

inline void action_draw_text_transformed(const string text, const double x, const double y, const double horScale, const double verScale, const double angle)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text_transformed(inst->x+x, inst->y+y, text, horScale, verScale, angle);
    }
    else
        draw_text_transformed(x, y, text, horScale, verScale, angle);
}

inline void action_draw_variable(variant variable, const double x, const double y)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        draw_text(inst->x+x, inst->y+y, *variable);
    }
    else
        draw_text(x, y, toString(variable));
}

inline bool action_if_health(const double value, const int operation)
{
 switch (operation)
 {
     case 0: return (health == value); break;
     case 1: return (health < value); break;
     case 2: return (health > value); break;
     default: return false;
  }
}

inline bool action_if_life(const double value, const int operation)
{
 switch (operation)
 {
     case 0: return (lives == value); break;
     case 1: return (lives < value); break;
     case 2: return (lives > value); break;
     default: return false;
    }
}

inline bool action_if_score(const double value, const int operation)
{
 switch (operation)
 {
     case 0: return (score == value); break;
     case 1: return (score < value); break;
     case 2: return (score > value); break;
     default: return false;
  }
}

inline bool action_if_mouse(const int button) {
 return mouse_check_button(button);
}

inline void action_move_random(const double snapHor, const double snapVer) {
    move_random(snapHor, snapVer);
}

inline void action_snap(const double hor, const double vert) {
    move_snap(hor, vert);
}

inline void action_highscore_clear() {
    highscore_clear();
}

inline void action_create_object_motion(int object, double x, double y, double speed, double direction)
{
    if (argument_relative)
    {
        enigma::object_planar* const inst = ((enigma::object_planar*)enigma::instance_event_iterator->inst);
        enigma::object_planar* const ii = ((enigma::object_planar*)enigma::fetch_instance_by_int(instance_create(inst->x + x, inst->y + y, object)));
        ii->speed.rval.d = speed;
        ii->direction = direction;
    }
    else
    {
        enigma::object_planar* const ii = ((enigma::object_planar*)enigma::fetch_instance_by_int(instance_create(x, y, object)));
        ii->speed.rval.d = speed;
        ii->direction = direction;
    }
}

inline int draw_self()
{
    enigma::object_collisions* const inst = ((enigma::object_collisions*)enigma::instance_event_iterator->inst);
    draw_sprite_ext(inst->sprite_index, inst->image_index, inst->x, inst->y, inst->image_xscale, inst->image_yscale, inst->image_angle, inst->image_blend, inst->image_alpha);
    return 0;
}

inline void action_fullscreen(int action)
{
    switch (action)
    {
        case 0:
            window_set_fullscreen(!window_get_fullscreen());
            break;
        case 1:
            window_set_fullscreen(false);
            break;
        case 2:
            window_set_fullscreen(true);
            break;
    }
}

inline void set_automatic_draw(bool enable)
{
    automatic_redraw = enable;
}

inline void action_path(unsigned pathid,double speed,unsigned endaction,bool absolute)
{
    path_start(pathid, speed, endaction, absolute);
}
inline void action_path_end()
{
    path_end();
}
inline void action_path_position(double position, bool relative)
{
    path_set_position(position, relative);
}
inline void action_path_speed(double speed, bool relative)
{
    path_set_speed(speed, relative);
}

namespace enigma
{
  int game_ending();
  int game_ending()
  {
    for (enigma::iterator i = instance_list_first(); i; ++i)
      { i->unlink(); delete *i; }
    return 0;
  }
}
