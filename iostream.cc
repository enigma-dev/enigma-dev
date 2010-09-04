# 1 "in.cpp"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "in.cpp"
# 1 "/usr/include/c++/4.2.1/iostream" 1 3
# 42 "/usr/include/c++/4.2.1/iostream" 3
       
# 43 "/usr/include/c++/4.2.1/iostream" 3

# 1 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++config.h" 1 3
# 41 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++config.h" 3
# 1 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/os_defines.h" 1 3
# 61 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/os_defines.h" 3
# 1 "/usr/include/unistd.h" 1 3 4
# 71 "/usr/include/unistd.h" 3 4
# 1 "/usr/include/_types.h" 1 3 4
# 27 "/usr/include/_types.h" 3 4
# 1 "/usr/include/sys/_types.h" 1 3 4
# 32 "/usr/include/sys/_types.h" 3 4
# 1 "/usr/include/sys/cdefs.h" 1 3 4
# 33 "/usr/include/sys/_types.h" 2 3 4
# 1 "/usr/include/machine/_types.h" 1 3 4
# 34 "/usr/include/machine/_types.h" 3 4
# 1 "/usr/include/i386/_types.h" 1 3 4
# 37 "/usr/include/i386/_types.h" 3 4
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
# 70 "/usr/include/i386/_types.h" 3 4
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
# 35 "/usr/include/machine/_types.h" 2 3 4
# 34 "/usr/include/sys/_types.h" 2 3 4
# 58 "/usr/include/sys/_types.h" 3 4
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
# 94 "/usr/include/sys/_types.h" 3 4
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
# 28 "/usr/include/_types.h" 2 3 4
# 39 "/usr/include/_types.h" 3 4
typedef int __darwin_nl_item;
typedef int __darwin_wctrans_t;

typedef __uint32_t __darwin_wctype_t;
# 72 "/usr/include/unistd.h" 2 3 4
# 1 "/usr/include/sys/unistd.h" 1 3 4
# 138 "/usr/include/sys/unistd.h" 3 4
struct accessx_descriptor {
 unsigned int ad_name_offset;
 int ad_flags;
 int ad_pad[2];
};
# 73 "/usr/include/unistd.h" 2 3 4




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
# 414 "/usr/include/unistd.h" 3 4
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


# 1 "/usr/include/sys/select.h" 1 3 4
# 66 "/usr/include/sys/select.h" 3 4
# 1 "/usr/include/sys/appleapiopts.h" 1 3 4
# 67 "/usr/include/sys/select.h" 2 3 4
# 78 "/usr/include/sys/select.h" 3 4
# 1 "/usr/include/sys/_structs.h" 1 3 4
# 88 "/usr/include/sys/_structs.h" 3 4
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
# 183 "/usr/include/sys/_structs.h" 3 4
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
# 79 "/usr/include/sys/select.h" 2 3 4
# 87 "/usr/include/sys/select.h" 3 4
typedef __darwin_time_t time_t;




typedef __darwin_suseconds_t suseconds_t;




typedef __darwin_sigset_t sigset_t;
# 134 "/usr/include/sys/select.h" 3 4
extern "C" {


int pselect(int, fd_set * , fd_set * ,
  fd_set * , const struct timespec * ,
  const sigset_t * )




  __asm("_" "pselect" "$1050")




  ;


# 1 "/usr/include/sys/_select.h" 1 3 4
# 39 "/usr/include/sys/_select.h" 3 4
int select(int, fd_set * , fd_set * ,
  fd_set * , struct timeval * )




  __asm("_" "select" "$1050")




  ;
# 153 "/usr/include/sys/select.h" 2 3 4

}
# 520 "/usr/include/unistd.h" 2 3 4

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
# 611 "/usr/include/unistd.h" 3 4
struct fssearchblock;
struct searchstate;

int searchfs(const char *, struct fssearchblock *, unsigned long *, unsigned int, unsigned int, struct searchstate *);
int fsctl(const char *,unsigned long,void*,unsigned int);
int ffsctl(int,unsigned long,void*,unsigned int);

extern int optreset;


}
# 62 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/os_defines.h" 2 3


extern "C" {
# 89 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/os_defines.h" 3
extern void __dtrace_probe$cxa_runtime$cxa_exception_rethrow$v1(void);
extern int __dtrace_isenabled$cxa_runtime$cxa_exception_rethrow$v1(void);
extern void __dtrace_probe$cxa_runtime$cxa_exception_throw$v1$766f6964202a(void *);
extern int __dtrace_isenabled$cxa_runtime$cxa_exception_throw$v1(void);


}
# 42 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++config.h" 2 3


# 1 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/cpu_defines.h" 1 3
# 45 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++config.h" 2 3
# 153 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++config.h" 3
namespace std {
# 165 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++config.h" 3
}
# 45 "/usr/include/c++/4.2.1/iostream" 2 3
# 1 "/usr/include/c++/4.2.1/ostream" 1 3
# 43 "/usr/include/c++/4.2.1/ostream" 3
       
# 44 "/usr/include/c++/4.2.1/ostream" 3

# 1 "/usr/include/c++/4.2.1/ios" 1 3
# 41 "/usr/include/c++/4.2.1/ios" 3
       
# 42 "/usr/include/c++/4.2.1/ios" 3

# 1 "/usr/include/c++/4.2.1/iosfwd" 1 3
# 42 "/usr/include/c++/4.2.1/iosfwd" 3
       
# 43 "/usr/include/c++/4.2.1/iosfwd" 3


# 1 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++locale.h" 1 3
# 45 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++locale.h" 3
       
# 46 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++locale.h" 3

# 1 "/usr/include/c++/4.2.1/clocale" 1 3
# 48 "/usr/include/c++/4.2.1/clocale" 3
       
# 49 "/usr/include/c++/4.2.1/clocale" 3


# 1 "/usr/include/locale.h" 1 3 4
# 40 "/usr/include/locale.h" 3 4
# 1 "/usr/include/_locale.h" 1 3 4
# 43 "/usr/include/_locale.h" 3 4
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
# 41 "/usr/include/locale.h" 2 3 4
# 52 "/usr/include/locale.h" 3 4
extern "C" {
char *setlocale(int, const char *);
}
# 52 "/usr/include/c++/4.2.1/clocale" 2 3





namespace std {

  using ::lconv;
  using ::setlocale;
  using ::localeconv;

}
# 48 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++locale.h" 2 3
# 1 "/usr/include/c++/4.2.1/cstring" 1 3
# 48 "/usr/include/c++/4.2.1/cstring" 3
       
# 49 "/usr/include/c++/4.2.1/cstring" 3


# 1 "/usr/include/c++/4.2.1/cstddef" 1 3
# 48 "/usr/include/c++/4.2.1/cstddef" 3
       
# 49 "/usr/include/c++/4.2.1/cstddef" 3


# 1 "/usr/lib/gcc/i686-apple-darwin10/4.2.1/include/stddef.h" 1 3 4
# 152 "/usr/lib/gcc/i686-apple-darwin10/4.2.1/include/stddef.h" 3 4
typedef long int ptrdiff_t;
# 52 "/usr/include/c++/4.2.1/cstddef" 2 3

namespace std {

  using ::ptrdiff_t;
  using ::size_t;

}
# 52 "/usr/include/c++/4.2.1/cstring" 2 3
# 1 "/usr/include/string.h" 1 3 4
# 80 "/usr/include/string.h" 3 4
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
# 53 "/usr/include/c++/4.2.1/cstring" 2 3
# 78 "/usr/include/c++/4.2.1/cstring" 3
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
# 49 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++locale.h" 2 3
# 1 "/usr/include/c++/4.2.1/cstdio" 1 3
# 48 "/usr/include/c++/4.2.1/cstdio" 3
       
# 49 "/usr/include/c++/4.2.1/cstdio" 3




# 1 "/usr/include/stdio.h" 1 3 4
# 70 "/usr/include/stdio.h" 3 4
typedef __darwin_va_list va_list;
# 87 "/usr/include/stdio.h" 3 4
typedef __darwin_off_t fpos_t;
# 98 "/usr/include/stdio.h" 3 4
struct __sbuf {
 unsigned char *_base;
 int _size;
};


struct __sFILEX;
# 132 "/usr/include/stdio.h" 3 4
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
# 248 "/usr/include/stdio.h" 3 4
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
# 383 "/usr/include/stdio.h" 3 4
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
# 54 "/usr/include/c++/4.2.1/cstdio" 2 3
# 98 "/usr/include/c++/4.2.1/cstdio" 3
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
# 155 "/usr/include/c++/4.2.1/cstdio" 3
namespace __gnu_cxx {
# 170 "/usr/include/c++/4.2.1/cstdio" 3
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
# 50 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++locale.h" 2 3
# 1 "/usr/include/c++/4.2.1/cstdarg" 1 3
# 48 "/usr/include/c++/4.2.1/cstdarg" 3
       
# 49 "/usr/include/c++/4.2.1/cstdarg" 3


# 1 "/usr/lib/gcc/i686-apple-darwin10/4.2.1/include/stdarg.h" 1 3 4
# 43 "/usr/lib/gcc/i686-apple-darwin10/4.2.1/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 52 "/usr/include/c++/4.2.1/cstdarg" 2 3






namespace std {

  using ::va_list;

}
# 51 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++locale.h" 2 3



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
# 46 "/usr/include/c++/4.2.1/iosfwd" 2 3
# 1 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++io.h" 1 3
# 43 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++io.h" 3
# 1 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/gthr.h" 1 3
# 33 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/gthr.h" 3
#pragma GCC visibility push(default)
# 114 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/gthr.h" 3
# 1 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/gthr-default.h" 1 3
# 43 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/gthr-default.h" 3
# 1 "/usr/include/pthread.h" 1 3 4
# 57 "/usr/include/pthread.h" 3 4
# 1 "/usr/include/pthread_impl.h" 1 3 4
# 58 "/usr/include/pthread.h" 2 3 4

# 1 "/usr/include/sched.h" 1 3 4
# 30 "/usr/include/sched.h" 3 4
extern "C" {




struct sched_param { int sched_priority; char __opaque[4]; };


extern int sched_yield(void);
extern int sched_get_priority_min(int);
extern int sched_get_priority_max(int);
}
# 60 "/usr/include/pthread.h" 2 3 4
# 1 "/usr/include/time.h" 1 3 4
# 69 "/usr/include/time.h" 3 4
# 1 "/usr/include/_structs.h" 1 3 4
# 24 "/usr/include/_structs.h" 3 4
# 1 "/usr/include/sys/_structs.h" 1 3 4
# 25 "/usr/include/_structs.h" 2 3 4
# 70 "/usr/include/time.h" 2 3 4







typedef __darwin_clock_t clock_t;
# 90 "/usr/include/time.h" 3 4
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
# 113 "/usr/include/time.h" 3 4
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
# 61 "/usr/include/pthread.h" 2 3 4



typedef __darwin_pthread_attr_t pthread_attr_t;




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
# 148 "/usr/include/pthread.h" 3 4
extern "C" {
# 244 "/usr/include/pthread.h" 3 4
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
# 44 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/gthr-default.h" 2 3


typedef pthread_key_t __gthread_key_t;
typedef pthread_once_t __gthread_once_t;
typedef pthread_mutex_t __gthread_mutex_t;
typedef pthread_mutex_t __gthread_recursive_mutex_t;
# 92 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/gthr-default.h" 3
















# 214 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/gthr-default.h" 3
static inline int
__gthread_active_p (void)
{
  return 1;
}
# 571 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/gthr-default.h" 3
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
# 115 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/gthr.h" 2 3







#pragma GCC visibility pop
# 44 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++io.h" 2 3

namespace std {

  typedef __gthread_mutex_t __c_lock;


  typedef FILE __c_file;

}
# 47 "/usr/include/c++/4.2.1/iosfwd" 2 3
# 1 "/usr/include/c++/4.2.1/cctype" 1 3
# 48 "/usr/include/c++/4.2.1/cctype" 3
       
# 49 "/usr/include/c++/4.2.1/cctype" 3


# 1 "/usr/include/ctype.h" 1 3 4
# 69 "/usr/include/ctype.h" 3 4
# 1 "/usr/include/runetype.h" 1 3 4
# 53 "/usr/include/runetype.h" 3 4
typedef __darwin_ct_rune_t ct_rune_t;




typedef __darwin_rune_t rune_t;
# 70 "/usr/include/runetype.h" 3 4
typedef __darwin_wint_t wint_t;
# 81 "/usr/include/runetype.h" 3 4
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
# 70 "/usr/include/ctype.h" 2 3 4
# 145 "/usr/include/ctype.h" 3 4
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
# 164 "/usr/include/ctype.h" 3 4
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
# 204 "/usr/include/ctype.h" 3 4
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
# 52 "/usr/include/c++/4.2.1/cctype" 2 3
# 68 "/usr/include/c++/4.2.1/cctype" 3
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
# 48 "/usr/include/c++/4.2.1/iosfwd" 2 3
# 1 "/usr/include/c++/4.2.1/bits/stringfwd.h" 1 3
# 42 "/usr/include/c++/4.2.1/bits/stringfwd.h" 3
       
# 43 "/usr/include/c++/4.2.1/bits/stringfwd.h" 3



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
# 49 "/usr/include/c++/4.2.1/iosfwd" 2 3
# 1 "/usr/include/c++/4.2.1/bits/postypes.h" 1 3
# 44 "/usr/include/c++/4.2.1/bits/postypes.h" 3
       
# 45 "/usr/include/c++/4.2.1/bits/postypes.h" 3

# 1 "/usr/include/c++/4.2.1/cwchar" 1 3
# 48 "/usr/include/c++/4.2.1/cwchar" 3
       
# 49 "/usr/include/c++/4.2.1/cwchar" 3



# 1 "/usr/include/c++/4.2.1/ctime" 1 3
# 48 "/usr/include/c++/4.2.1/ctime" 3
       
# 49 "/usr/include/c++/4.2.1/ctime" 3
# 64 "/usr/include/c++/4.2.1/ctime" 3
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
# 53 "/usr/include/c++/4.2.1/cwchar" 2 3


# 1 "/usr/include/wchar.h" 1 3 4
# 83 "/usr/include/wchar.h" 3 4
typedef __darwin_mbstate_t mbstate_t;
# 114 "/usr/include/wchar.h" 3 4
# 1 "/usr/include/_wctype.h" 1 3 4
# 52 "/usr/include/_wctype.h" 3 4
typedef __darwin_wctype_t wctype_t;
# 71 "/usr/include/_wctype.h" 3 4
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
# 176 "/usr/include/_wctype.h" 3 4
extern "C" {
wctype_t
 wctype(const char *);
}
# 115 "/usr/include/wchar.h" 2 3 4

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
# 56 "/usr/include/c++/4.2.1/cwchar" 2 3
# 70 "/usr/include/c++/4.2.1/cwchar" 3
namespace std {

  using ::mbstate_t;

}
# 144 "/usr/include/c++/4.2.1/cwchar" 3
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
# 262 "/usr/include/c++/4.2.1/cwchar" 3
  using ::wcstoll;
  using ::wcstoull;


}

namespace std {

  using ::__gnu_cxx::wcstold;
  using ::__gnu_cxx::wcstoll;
  using ::__gnu_cxx::wcstoull;

}
# 47 "/usr/include/c++/4.2.1/bits/postypes.h" 2 3


# 1 "/usr/lib/gcc/i686-apple-darwin10/4.2.1/include/stdint.h" 1 3 4
# 20 "/usr/lib/gcc/i686-apple-darwin10/4.2.1/include/stdint.h" 3 4
typedef signed char int8_t;




typedef short int16_t;




typedef int int32_t;




typedef long long int64_t;




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
# 89 "/usr/lib/gcc/i686-apple-darwin10/4.2.1/include/stdint.h" 3 4
typedef unsigned long uintptr_t;







typedef long int intmax_t;
# 106 "/usr/lib/gcc/i686-apple-darwin10/4.2.1/include/stdint.h" 3 4
typedef long unsigned int uintmax_t;
# 50 "/usr/include/c++/4.2.1/bits/postypes.h" 2 3


namespace std {
# 72 "/usr/include/c++/4.2.1/bits/postypes.h" 3
  typedef int64_t streamoff;





  typedef ptrdiff_t streamsize;

  template<typename _StateT>
    class fpos;
# 94 "/usr/include/c++/4.2.1/bits/postypes.h" 3
  template<typename _StateT>
    class fpos
    {
    private:
      streamoff _M_off;
      _StateT _M_state;

    public:




      fpos()
      : _M_off(0), _M_state() { }
# 116 "/usr/include/c++/4.2.1/bits/postypes.h" 3
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
# 50 "/usr/include/c++/4.2.1/iosfwd" 2 3
# 1 "/usr/include/c++/4.2.1/bits/functexcept.h" 1 3
# 42 "/usr/include/c++/4.2.1/bits/functexcept.h" 3
# 1 "/usr/include/c++/4.2.1/exception_defines.h" 1 3
# 43 "/usr/include/c++/4.2.1/bits/functexcept.h" 2 3

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
# 51 "/usr/include/c++/4.2.1/iosfwd" 2 3

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
# 135 "/usr/include/c++/4.2.1/iosfwd" 3
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
# 44 "/usr/include/c++/4.2.1/ios" 2 3
# 1 "/usr/include/c++/4.2.1/exception" 1 3
# 40 "/usr/include/c++/4.2.1/exception" 3
#pragma GCC visibility push(default)



extern "C++" {

namespace std
{
# 56 "/usr/include/c++/4.2.1/exception" 3
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
# 112 "/usr/include/c++/4.2.1/exception" 3
  bool uncaught_exception() throw();
}

namespace __gnu_cxx {
# 127 "/usr/include/c++/4.2.1/exception" 3
  void __verbose_terminate_handler ();

}

}

#pragma GCC visibility pop
# 45 "/usr/include/c++/4.2.1/ios" 2 3
# 1 "/usr/include/c++/4.2.1/bits/char_traits.h" 1 3
# 43 "/usr/include/c++/4.2.1/bits/char_traits.h" 3
       
# 44 "/usr/include/c++/4.2.1/bits/char_traits.h" 3


# 1 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 1 3
# 67 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
# 1 "/usr/include/c++/4.2.1/climits" 1 3
# 48 "/usr/include/c++/4.2.1/climits" 3
       
# 49 "/usr/include/c++/4.2.1/climits" 3

# 1 "/usr/lib/gcc/i686-apple-darwin10/4.2.1/include/limits.h" 1 3 4






# 1 "/usr/lib/gcc/i686-apple-darwin10/4.2.1/include/syslimits.h" 1 3 4
# 8 "/usr/lib/gcc/i686-apple-darwin10/4.2.1/include/limits.h" 2 3 4


# 1 "/usr/include/limits.h" 1 3 4
# 64 "/usr/include/limits.h" 3 4
# 1 "/usr/include/machine/limits.h" 1 3 4







# 1 "/usr/include/i386/limits.h" 1 3 4
# 40 "/usr/include/i386/limits.h" 3 4
# 1 "/usr/include/i386/_limits.h" 1 3 4
# 41 "/usr/include/i386/limits.h" 2 3 4
# 9 "/usr/include/machine/limits.h" 2 3 4
# 65 "/usr/include/limits.h" 2 3 4
# 1 "/usr/include/sys/syslimits.h" 1 3 4
# 66 "/usr/include/limits.h" 2 3 4
# 11 "/usr/lib/gcc/i686-apple-darwin10/4.2.1/include/limits.h" 2 3 4
# 51 "/usr/include/c++/4.2.1/climits" 2 3
# 68 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 2 3
# 1 "/usr/include/c++/4.2.1/cstdlib" 1 3
# 48 "/usr/include/c++/4.2.1/cstdlib" 3
       
# 49 "/usr/include/c++/4.2.1/cstdlib" 3
# 72 "/usr/include/c++/4.2.1/cstdlib" 3
# 1 "/usr/include/stdlib.h" 1 3 4
# 61 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/Availability.h" 1 3 4
# 126 "/usr/include/Availability.h" 3 4
# 1 "/usr/include/AvailabilityInternal.h" 1 3 4
# 127 "/usr/include/Availability.h" 2 3 4
# 62 "/usr/include/stdlib.h" 2 3 4



# 1 "/usr/include/sys/wait.h" 1 3 4
# 79 "/usr/include/sys/wait.h" 3 4
typedef enum {
 P_ALL,
 P_PID,
 P_PGID
} idtype_t;
# 95 "/usr/include/sys/wait.h" 3 4
typedef __darwin_id_t id_t;
# 116 "/usr/include/sys/wait.h" 3 4
# 1 "/usr/include/sys/signal.h" 1 3 4
# 81 "/usr/include/sys/signal.h" 3 4
# 1 "/usr/include/machine/signal.h" 1 3 4
# 34 "/usr/include/machine/signal.h" 3 4
# 1 "/usr/include/i386/signal.h" 1 3 4
# 39 "/usr/include/i386/signal.h" 3 4
typedef int sig_atomic_t;
# 55 "/usr/include/i386/signal.h" 3 4
# 1 "/usr/include/i386/_structs.h" 1 3 4
# 56 "/usr/include/i386/signal.h" 2 3 4
# 35 "/usr/include/machine/signal.h" 2 3 4
# 82 "/usr/include/sys/signal.h" 2 3 4
# 154 "/usr/include/sys/signal.h" 3 4
# 1 "/usr/include/sys/_structs.h" 1 3 4
# 57 "/usr/include/sys/_structs.h" 3 4
# 1 "/usr/include/machine/_structs.h" 1 3 4
# 31 "/usr/include/machine/_structs.h" 3 4
# 1 "/usr/include/i386/_structs.h" 1 3 4
# 38 "/usr/include/i386/_structs.h" 3 4
# 1 "/usr/include/mach/i386/_structs.h" 1 3 4
# 43 "/usr/include/mach/i386/_structs.h" 3 4
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
# 89 "/usr/include/mach/i386/_structs.h" 3 4
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
# 147 "/usr/include/mach/i386/_structs.h" 3 4
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
# 191 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_mmst_reg
{
 char __mmst_reg[10];
 char __mmst_rsrv[6];
};
# 210 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_xmm_reg
{
 char __xmm_reg[16];
};
# 232 "/usr/include/mach/i386/_structs.h" 3 4
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
# 308 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_i386_exception_state
{
    unsigned int __trapno;
    unsigned int __err;
    unsigned int __faultvaddr;
};
# 326 "/usr/include/mach/i386/_structs.h" 3 4
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
# 358 "/usr/include/mach/i386/_structs.h" 3 4
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
# 413 "/usr/include/mach/i386/_structs.h" 3 4
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
# 517 "/usr/include/mach/i386/_structs.h" 3 4
struct __darwin_x86_exception_state64
{
    unsigned int __trapno;
    unsigned int __err;
    __uint64_t __faultvaddr;
};
# 535 "/usr/include/mach/i386/_structs.h" 3 4
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
# 39 "/usr/include/i386/_structs.h" 2 3 4
# 48 "/usr/include/i386/_structs.h" 3 4
struct __darwin_mcontext32
{
 struct __darwin_i386_exception_state __es;
 struct __darwin_i386_thread_state __ss;
 struct __darwin_i386_float_state __fs;
};
# 68 "/usr/include/i386/_structs.h" 3 4
struct __darwin_mcontext64
{
 struct __darwin_x86_exception_state64 __es;
 struct __darwin_x86_thread_state64 __ss;
 struct __darwin_x86_float_state64 __fs;
};
# 91 "/usr/include/i386/_structs.h" 3 4
typedef struct __darwin_mcontext64 *mcontext_t;
# 32 "/usr/include/machine/_structs.h" 2 3 4
# 58 "/usr/include/sys/_structs.h" 2 3 4
# 75 "/usr/include/sys/_structs.h" 3 4
struct __darwin_sigaltstack
{
 void *ss_sp;
 __darwin_size_t ss_size;
 int ss_flags;
};
# 128 "/usr/include/sys/_structs.h" 3 4
struct __darwin_ucontext
{
 int uc_onstack;
 __darwin_sigset_t uc_sigmask;
 struct __darwin_sigaltstack uc_stack;
 struct __darwin_ucontext *uc_link;
 __darwin_size_t uc_mcsize;
 struct __darwin_mcontext64 *uc_mcontext;



};
# 218 "/usr/include/sys/_structs.h" 3 4
typedef struct __darwin_sigaltstack stack_t;
# 227 "/usr/include/sys/_structs.h" 3 4
typedef struct __darwin_ucontext ucontext_t;
# 155 "/usr/include/sys/signal.h" 2 3 4
# 181 "/usr/include/sys/signal.h" 3 4
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
# 292 "/usr/include/sys/signal.h" 3 4
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
# 354 "/usr/include/sys/signal.h" 3 4
typedef void (*sig_t)(int);
# 371 "/usr/include/sys/signal.h" 3 4
struct sigvec {
 void (*sv_handler)(int);
 int sv_mask;
 int sv_flags;
};
# 390 "/usr/include/sys/signal.h" 3 4
struct sigstack {
 char *ss_sp;
 int ss_onstack;
};
# 412 "/usr/include/sys/signal.h" 3 4
extern "C" {
void (*signal(int, void (*)(int)))(int);
}
# 117 "/usr/include/sys/wait.h" 2 3 4
# 1 "/usr/include/sys/resource.h" 1 3 4
# 76 "/usr/include/sys/resource.h" 3 4
# 1 "/usr/include/sys/_structs.h" 1 3 4
# 77 "/usr/include/sys/resource.h" 2 3 4
# 88 "/usr/include/sys/resource.h" 3 4
typedef __uint64_t rlim_t;
# 142 "/usr/include/sys/resource.h" 3 4
struct rusage {
 struct timeval ru_utime;
 struct timeval ru_stime;
# 153 "/usr/include/sys/resource.h" 3 4
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
# 213 "/usr/include/sys/resource.h" 3 4
struct rlimit {
 rlim_t rlim_cur;
 rlim_t rlim_max;
};
# 235 "/usr/include/sys/resource.h" 3 4
extern "C" {
int getpriority(int, id_t);

int getiopolicy_np(int, int);

int getrlimit(int, struct rlimit *) __asm("_" "getrlimit" );
int getrusage(int, struct rusage *);
int setpriority(int, id_t, int);

int setiopolicy_np(int, int, int);

int setrlimit(int, const struct rlimit *) __asm("_" "setrlimit" );
}
# 118 "/usr/include/sys/wait.h" 2 3 4
# 193 "/usr/include/sys/wait.h" 3 4
# 1 "/usr/include/machine/endian.h" 1 3 4
# 37 "/usr/include/machine/endian.h" 3 4
# 1 "/usr/include/i386/endian.h" 1 3 4
# 99 "/usr/include/i386/endian.h" 3 4
# 1 "/usr/include/sys/_endian.h" 1 3 4
# 124 "/usr/include/sys/_endian.h" 3 4
# 1 "/usr/include/libkern/_OSByteOrder.h" 1 3 4
# 66 "/usr/include/libkern/_OSByteOrder.h" 3 4
# 1 "/usr/include/libkern/i386/_OSByteOrder.h" 1 3 4
# 44 "/usr/include/libkern/i386/_OSByteOrder.h" 3 4
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



    __asm__ ("bswap   %0" : "+r" (_data));
    return _data;

}
# 91 "/usr/include/libkern/i386/_OSByteOrder.h" 3 4
static inline
__uint64_t
_OSSwapInt64(
    __uint64_t _data
)
{
    __asm__ ("bswap   %0" : "+r" (_data));
    return _data;
}
# 67 "/usr/include/libkern/_OSByteOrder.h" 2 3 4
# 125 "/usr/include/sys/_endian.h" 2 3 4
# 100 "/usr/include/i386/endian.h" 2 3 4
# 38 "/usr/include/machine/endian.h" 2 3 4
# 194 "/usr/include/sys/wait.h" 2 3 4







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
# 254 "/usr/include/sys/wait.h" 3 4
extern "C" {
pid_t wait(int *) __asm("_" "wait" );
pid_t waitpid(pid_t, int *, int) __asm("_" "waitpid" );

int waitid(idtype_t, id_t, siginfo_t *, int) __asm("_" "waitid" );


pid_t wait3(int *, int, struct rusage *);
pid_t wait4(pid_t, int *, int, struct rusage *);

}
# 66 "/usr/include/stdlib.h" 2 3 4

# 1 "/usr/include/alloca.h" 1 3 4
# 35 "/usr/include/alloca.h" 3 4
extern "C" {
void *alloca(size_t);
}
# 68 "/usr/include/stdlib.h" 2 3 4
# 97 "/usr/include/stdlib.h" 3 4
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
# 134 "/usr/include/stdlib.h" 3 4
extern int __mb_cur_max;
# 144 "/usr/include/stdlib.h" 3 4
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






# 1 "/usr/include/machine/types.h" 1 3 4
# 37 "/usr/include/machine/types.h" 3 4
# 1 "/usr/include/i386/types.h" 1 3 4
# 70 "/usr/include/i386/types.h" 3 4
# 1 "/usr/include/i386/_types.h" 1 3 4
# 71 "/usr/include/i386/types.h" 2 3 4
# 80 "/usr/include/i386/types.h" 3 4
typedef unsigned char u_int8_t;




typedef unsigned short u_int16_t;




typedef unsigned int u_int32_t;




typedef unsigned long long u_int64_t;


typedef int64_t register_t;
# 114 "/usr/include/i386/types.h" 3 4
typedef u_int64_t user_addr_t;
typedef u_int64_t user_size_t;
typedef int64_t user_ssize_t;
typedef int64_t user_long_t;
typedef u_int64_t user_ulong_t;
typedef int64_t user_time_t;
typedef int64_t user_off_t;







typedef u_int64_t syscall_arg_t;
# 38 "/usr/include/machine/types.h" 2 3 4
# 256 "/usr/include/stdlib.h" 2 3 4
# 267 "/usr/include/stdlib.h" 3 4
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
# 73 "/usr/include/c++/4.2.1/cstdlib" 2 3
# 104 "/usr/include/c++/4.2.1/cstdlib" 3
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
# 161 "/usr/include/c++/4.2.1/cstdlib" 3
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
# 194 "/usr/include/c++/4.2.1/cstdlib" 3
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
# 69 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 2 3


# 1 "/usr/include/c++/4.2.1/bits/stl_pair.h" 1 3
# 64 "/usr/include/c++/4.2.1/bits/stl_pair.h" 3
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
# 142 "/usr/include/c++/4.2.1/bits/stl_pair.h" 3
  template<class _T1, class _T2>
    inline pair<_T1, _T2>
    make_pair(_T1 __x, _T2 __y)
    { return pair<_T1, _T2>(__x, __y); }

}
# 72 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 2 3
# 1 "/usr/include/c++/4.2.1/bits/cpp_type_traits.h" 1 3
# 41 "/usr/include/c++/4.2.1/bits/cpp_type_traits.h" 3
       
# 42 "/usr/include/c++/4.2.1/bits/cpp_type_traits.h" 3
# 74 "/usr/include/c++/4.2.1/bits/cpp_type_traits.h" 3
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
# 73 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 2 3
# 1 "/usr/include/c++/4.2.1/ext/type_traits.h" 1 3
# 38 "/usr/include/c++/4.2.1/ext/type_traits.h" 3
       
# 39 "/usr/include/c++/4.2.1/ext/type_traits.h" 3


# 1 "/usr/include/c++/4.2.1/utility" 1 3
# 63 "/usr/include/c++/4.2.1/utility" 3
       
# 64 "/usr/include/c++/4.2.1/utility" 3


# 1 "/usr/include/c++/4.2.1/bits/stl_relops.h" 1 3
# 74 "/usr/include/c++/4.2.1/bits/stl_relops.h" 3
namespace std {

  namespace rel_ops
  {
# 90 "/usr/include/c++/4.2.1/bits/stl_relops.h" 3
    template <class _Tp>
      inline bool
      operator!=(const _Tp& __x, const _Tp& __y)
      { return !(__x == __y); }
# 103 "/usr/include/c++/4.2.1/bits/stl_relops.h" 3
    template <class _Tp>
      inline bool
      operator>(const _Tp& __x, const _Tp& __y)
      { return __y < __x; }
# 116 "/usr/include/c++/4.2.1/bits/stl_relops.h" 3
    template <class _Tp>
      inline bool
      operator<=(const _Tp& __x, const _Tp& __y)
      { return !(__y < __x); }
# 129 "/usr/include/c++/4.2.1/bits/stl_relops.h" 3
    template <class _Tp>
      inline bool
      operator>=(const _Tp& __x, const _Tp& __y)
      { return !(__x < __y); }

  }

}
# 67 "/usr/include/c++/4.2.1/utility" 2 3
# 42 "/usr/include/c++/4.2.1/ext/type_traits.h" 2 3


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
# 74 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 2 3
# 1 "/usr/include/c++/4.2.1/bits/stl_iterator_base_types.h" 1 3
# 67 "/usr/include/c++/4.2.1/bits/stl_iterator_base_types.h" 3
       
# 68 "/usr/include/c++/4.2.1/bits/stl_iterator_base_types.h" 3

namespace std {
# 80 "/usr/include/c++/4.2.1/bits/stl_iterator_base_types.h" 3
  struct input_iterator_tag {};

  struct output_iterator_tag {};

  struct forward_iterator_tag : public input_iterator_tag {};


  struct bidirectional_iterator_tag : public forward_iterator_tag {};


  struct random_access_iterator_tag : public bidirectional_iterator_tag {};
# 104 "/usr/include/c++/4.2.1/bits/stl_iterator_base_types.h" 3
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
# 75 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 2 3
# 1 "/usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h" 1 3
# 68 "/usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h" 3
       
# 69 "/usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h" 3
# 1 "/usr/include/c++/4.2.1/bits/concept_check.h" 1 3
# 38 "/usr/include/c++/4.2.1/bits/concept_check.h" 3
       
# 39 "/usr/include/c++/4.2.1/bits/concept_check.h" 3
# 70 "/usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h" 2 3

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
# 113 "/usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h" 3
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
# 171 "/usr/include/c++/4.2.1/bits/stl_iterator_base_funcs.h" 3
  template<typename _InputIterator, typename _Distance>
    inline void
    advance(_InputIterator& __i, _Distance __n)
    {

      typename iterator_traits<_InputIterator>::difference_type __d = __n;
      std::__advance(__i, __d, std::__iterator_category(__i));
    }

}
# 76 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 2 3
# 1 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 1 3
# 72 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 3
namespace std {
# 93 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 3
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
# 280 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 3
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
# 384 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 3
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
# 410 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 3
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
# 444 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 3
  template<typename _Container>
    inline back_insert_iterator<_Container>
    back_inserter(_Container& __x)
    { return back_insert_iterator<_Container>(__x); }
# 459 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 3
  template<typename _Container>
    class front_insert_iterator
    : public iterator<output_iterator_tag, void, void, void, void>
    {
    protected:
      _Container* container;

    public:

      typedef _Container container_type;


      explicit front_insert_iterator(_Container& __x) : container(&__x) { }
# 484 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 3
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
# 518 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 3
  template<typename _Container>
    inline front_insert_iterator<_Container>
    front_inserter(_Container& __x)
    { return front_insert_iterator<_Container>(__x); }
# 537 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 3
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
# 579 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 3
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
# 614 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 3
  template<typename _Container, typename _Iterator>
    inline insert_iterator<_Container>
    inserter(_Container& __x, _Iterator __i)
    {
      return insert_iterator<_Container>(__x,
      typename _Container::iterator(__i));
    }

}

namespace __gnu_cxx {
# 633 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 3
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
# 730 "/usr/include/c++/4.2.1/bits/stl_iterator.h" 3
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
# 77 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 2 3

# 1 "/usr/include/c++/4.2.1/debug/debug.h" 1 3
# 47 "/usr/include/c++/4.2.1/debug/debug.h" 3
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
# 79 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 2 3

namespace std {
# 91 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
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
# 141 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
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
# 180 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
  template<typename _Tp>
    inline const _Tp&
    min(const _Tp& __a, const _Tp& __b)
    {

     

      if (__b < __a)
 return __b;
      return __a;
    }
# 202 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
  template<typename _Tp>
    inline const _Tp&
    max(const _Tp& __a, const _Tp& __b)
    {

     

      if (__a < __b)
 return __b;
      return __a;
    }
# 224 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
  template<typename _Tp, typename _Compare>
    inline const _Tp&
    min(const _Tp& __a, const _Tp& __b, _Compare __comp)
    {

      if (__comp(__b, __a))
 return __b;
      return __a;
    }
# 244 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
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
# 387 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
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
# 524 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
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
# 581 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
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
# 657 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
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
# 704 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
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
# 739 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
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
# 769 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
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
# 801 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
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
# 833 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
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
# 873 "/usr/include/c++/4.2.1/bits/stl_algobase.h" 3
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
# 47 "/usr/include/c++/4.2.1/bits/char_traits.h" 2 3


namespace __gnu_cxx {
# 61 "/usr/include/c++/4.2.1/bits/char_traits.h" 3
  template <class _CharT>
    struct _Char_types
    {
      typedef unsigned long int_type;
      typedef std::streampos pos_type;
      typedef std::streamoff off_type;
      typedef std::mbstate_t state_type;
    };
# 86 "/usr/include/c++/4.2.1/bits/char_traits.h" 3
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
# 225 "/usr/include/c++/4.2.1/bits/char_traits.h" 3
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
# 46 "/usr/include/c++/4.2.1/ios" 2 3

# 1 "/usr/include/c++/4.2.1/bits/localefwd.h" 1 3
# 43 "/usr/include/c++/4.2.1/bits/localefwd.h" 3
       
# 44 "/usr/include/c++/4.2.1/bits/localefwd.h" 3






namespace std {


  class locale;


  template<typename _CharT>
    inline bool
    isspace(_CharT, const locale&);

  template<typename _CharT>
    inline bool
    isprint(_CharT, const locale&);

  template<typename _CharT>
    inline bool
    iscntrl(_CharT, const locale&);

  template<typename _CharT>
    inline bool
    isupper(_CharT, const locale&);

  template<typename _CharT>
    inline bool
    islower(_CharT, const locale&);

  template<typename _CharT>
    inline bool
    isalpha(_CharT, const locale&);

  template<typename _CharT>
    inline bool
    isdigit(_CharT, const locale&);

  template<typename _CharT>
    inline bool
    ispunct(_CharT, const locale&);

  template<typename _CharT>
    inline bool
    isxdigit(_CharT, const locale&);

  template<typename _CharT>
    inline bool
    isalnum(_CharT, const locale&);

  template<typename _CharT>
    inline bool
    isgraph(_CharT, const locale&);

  template<typename _CharT>
    inline _CharT
    toupper(_CharT, const locale&);

  template<typename _CharT>
    inline _CharT
    tolower(_CharT, const locale&);


  class ctype_base;
  template<typename _CharT>
    class ctype;
  template<> class ctype<char>;

  template<> class ctype<wchar_t>;

  template<typename _CharT>
    class ctype_byname;


  class codecvt_base;
  class __enc_traits;
  template<typename _InternT, typename _ExternT, typename _StateT>
    class codecvt;
  template<> class codecvt<char, char, mbstate_t>;

  template<> class codecvt<wchar_t, char, mbstate_t>;

  template<typename _InternT, typename _ExternT, typename _StateT>
    class codecvt_byname;



  template<typename _CharT, typename _InIter = istreambuf_iterator<_CharT> >
    class num_get;
  template<typename _CharT, typename _OutIter = ostreambuf_iterator<_CharT> >
    class num_put;

  template<typename _CharT> class numpunct;
  template<typename _CharT> class numpunct_byname;


  template<typename _CharT>
    class collate;
  template<typename _CharT> class
    collate_byname;


  class time_base;
  template<typename _CharT, typename _InIter = istreambuf_iterator<_CharT> >
    class time_get;
  template<typename _CharT, typename _InIter = istreambuf_iterator<_CharT> >
    class time_get_byname;
  template<typename _CharT, typename _OutIter = ostreambuf_iterator<_CharT> >
    class time_put;
  template<typename _CharT, typename _OutIter = ostreambuf_iterator<_CharT> >
    class time_put_byname;


  class money_base;

  template<typename _CharT, typename _InIter = istreambuf_iterator<_CharT> >
    class money_get;
  template<typename _CharT, typename _OutIter = ostreambuf_iterator<_CharT> >
    class money_put;

  template<typename _CharT, bool _Intl = false>
    class moneypunct;
  template<typename _CharT, bool _Intl = false>
    class moneypunct_byname;


  class messages_base;
  template<typename _CharT>
    class messages;
  template<typename _CharT>
    class messages_byname;

  template<typename _Facet>
    bool
    has_facet(const locale& __loc) throw();

  template<typename _Facet>
    const _Facet&
    use_facet(const locale& __loc);

  template<typename _Facet>
    inline const _Facet&
    __check_facet(const _Facet* __f)
    {
      if (!__f)
 __throw_bad_cast();
      return *__f;
    }

}
# 48 "/usr/include/c++/4.2.1/ios" 2 3
# 1 "/usr/include/c++/4.2.1/bits/ios_base.h" 1 3
# 43 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
       
# 44 "/usr/include/c++/4.2.1/bits/ios_base.h" 3

# 1 "/usr/include/c++/4.2.1/ext/atomicity.h" 1 3
# 40 "/usr/include/c++/4.2.1/ext/atomicity.h" 3
# 1 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/atomic_word.h" 1 3
# 37 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/atomic_word.h" 3
typedef int _Atomic_word;
# 41 "/usr/include/c++/4.2.1/ext/atomicity.h" 2 3

namespace __gnu_cxx {






  static inline _Atomic_word
  __exchange_and_add(volatile _Atomic_word* __mem, int __val)
  { return __sync_fetch_and_add(__mem, __val); }

  static inline void
  __atomic_add(volatile _Atomic_word* __mem, int __val)
  { __sync_fetch_and_add(__mem, __val); }
# 66 "/usr/include/c++/4.2.1/ext/atomicity.h" 3
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
# 46 "/usr/include/c++/4.2.1/bits/ios_base.h" 2 3

# 1 "/usr/include/c++/4.2.1/bits/locale_classes.h" 1 3
# 43 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
       
# 44 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3



# 1 "/usr/include/c++/4.2.1/string" 1 3
# 43 "/usr/include/c++/4.2.1/string" 3
       
# 44 "/usr/include/c++/4.2.1/string" 3




# 1 "/usr/include/c++/4.2.1/memory" 1 3
# 51 "/usr/include/c++/4.2.1/memory" 3
       
# 52 "/usr/include/c++/4.2.1/memory" 3


# 1 "/usr/include/c++/4.2.1/bits/allocator.h" 1 3
# 53 "/usr/include/c++/4.2.1/bits/allocator.h" 3
# 1 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++allocator.h" 1 3
# 39 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++allocator.h" 3
# 1 "/usr/include/c++/4.2.1/ext/new_allocator.h" 1 3
# 37 "/usr/include/c++/4.2.1/ext/new_allocator.h" 3
# 1 "/usr/include/c++/4.2.1/new" 1 3
# 47 "/usr/include/c++/4.2.1/new" 3
#pragma GCC visibility push(default)

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
# 95 "/usr/include/c++/4.2.1/new" 3
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

#pragma GCC visibility pop
# 38 "/usr/include/c++/4.2.1/ext/new_allocator.h" 2 3


namespace __gnu_cxx {

  using std::size_t;
  using std::ptrdiff_t;
# 52 "/usr/include/c++/4.2.1/ext/new_allocator.h" 3
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
# 40 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/c++allocator.h" 2 3
# 54 "/usr/include/c++/4.2.1/bits/allocator.h" 2 3



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
# 55 "/usr/include/c++/4.2.1/memory" 2 3
# 1 "/usr/include/c++/4.2.1/bits/stl_construct.h" 1 3
# 67 "/usr/include/c++/4.2.1/bits/stl_construct.h" 3
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
# 116 "/usr/include/c++/4.2.1/bits/stl_construct.h" 3
  template<typename _ForwardIterator>
    inline void
    __destroy_aux(_ForwardIterator __first, _ForwardIterator __last,
    __false_type)
    {
      for (; __first != __last; ++__first)
 std::_Destroy(&*__first);
    }
# 134 "/usr/include/c++/4.2.1/bits/stl_construct.h" 3
  template<typename _ForwardIterator>
    inline void
    __destroy_aux(_ForwardIterator, _ForwardIterator, __true_type)
    { }
# 146 "/usr/include/c++/4.2.1/bits/stl_construct.h" 3
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
# 166 "/usr/include/c++/4.2.1/bits/stl_construct.h" 3
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
# 56 "/usr/include/c++/4.2.1/memory" 2 3

# 1 "/usr/include/c++/4.2.1/bits/stl_uninitialized.h" 1 3
# 67 "/usr/include/c++/4.2.1/bits/stl_uninitialized.h" 3
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
# 106 "/usr/include/c++/4.2.1/bits/stl_uninitialized.h" 3
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
# 168 "/usr/include/c++/4.2.1/bits/stl_uninitialized.h" 3
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
# 213 "/usr/include/c++/4.2.1/bits/stl_uninitialized.h" 3
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
# 318 "/usr/include/c++/4.2.1/bits/stl_uninitialized.h" 3
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
# 58 "/usr/include/c++/4.2.1/memory" 2 3
# 1 "/usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h" 1 3
# 64 "/usr/include/c++/4.2.1/bits/stl_raw_storage_iter.h" 3
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
# 59 "/usr/include/c++/4.2.1/memory" 2 3

# 1 "/usr/include/c++/4.2.1/limits" 1 3
# 46 "/usr/include/c++/4.2.1/limits" 3
       
# 47 "/usr/include/c++/4.2.1/limits" 3
# 150 "/usr/include/c++/4.2.1/limits" 3
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
# 192 "/usr/include/c++/4.2.1/limits" 3
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
# 285 "/usr/include/c++/4.2.1/limits" 3
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
# 61 "/usr/include/c++/4.2.1/memory" 2 3

namespace std {
# 72 "/usr/include/c++/4.2.1/memory" 3
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
# 108 "/usr/include/c++/4.2.1/memory" 3
  template<typename _Tp>
    inline pair<_Tp*, ptrdiff_t>
    get_temporary_buffer(ptrdiff_t __len)
    { return std::__get_temporary_buffer(__len, static_cast<_Tp*>(0)); }
# 120 "/usr/include/c++/4.2.1/memory" 3
  template<typename _Tp>
    void
    return_temporary_buffer(_Tp* __p)
    { ::operator delete(__p, nothrow); }
# 132 "/usr/include/c++/4.2.1/memory" 3
  template<typename _Tp1>
    struct auto_ptr_ref
    {
      _Tp1* _M_ptr;

      explicit
      auto_ptr_ref(_Tp1* __p): _M_ptr(__p) { }
    };
# 173 "/usr/include/c++/4.2.1/memory" 3
  template<typename _Tp>
    class auto_ptr
    {
    private:
      _Tp* _M_ptr;

    public:

      typedef _Tp element_type;







      explicit
      auto_ptr(element_type* __p = 0) throw() : _M_ptr(__p) { }
# 199 "/usr/include/c++/4.2.1/memory" 3
      auto_ptr(auto_ptr& __a) throw() : _M_ptr(__a.release()) { }
# 211 "/usr/include/c++/4.2.1/memory" 3
      template<typename _Tp1>
        auto_ptr(auto_ptr<_Tp1>& __a) throw() : _M_ptr(__a.release()) { }
# 222 "/usr/include/c++/4.2.1/memory" 3
      auto_ptr&
      operator=(auto_ptr& __a) throw()
      {
 reset(__a.release());
 return *this;
      }
# 239 "/usr/include/c++/4.2.1/memory" 3
      template<typename _Tp1>
        auto_ptr&
        operator=(auto_ptr<_Tp1>& __a) throw()
        {
   reset(__a.release());
   return *this;
 }
# 259 "/usr/include/c++/4.2.1/memory" 3
      ~auto_ptr() { delete _M_ptr; }
# 269 "/usr/include/c++/4.2.1/memory" 3
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
# 299 "/usr/include/c++/4.2.1/memory" 3
      element_type*
      get() const throw() { return _M_ptr; }
# 313 "/usr/include/c++/4.2.1/memory" 3
      element_type*
      release() throw()
      {
 element_type* __tmp = _M_ptr;
 _M_ptr = 0;
 return __tmp;
      }
# 328 "/usr/include/c++/4.2.1/memory" 3
      void
      reset(element_type* __p = 0) throw()
      {
 if (__p != _M_ptr)
   {
     delete _M_ptr;
     _M_ptr = __p;
   }
      }
# 349 "/usr/include/c++/4.2.1/memory" 3
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
# 49 "/usr/include/c++/4.2.1/string" 2 3


# 1 "/usr/include/c++/4.2.1/bits/ostream_insert.h" 1 3
# 38 "/usr/include/c++/4.2.1/bits/ostream_insert.h" 3
       
# 39 "/usr/include/c++/4.2.1/bits/ostream_insert.h" 3



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
# 52 "/usr/include/c++/4.2.1/string" 2 3

# 1 "/usr/include/c++/4.2.1/bits/stl_function.h" 1 3
# 64 "/usr/include/c++/4.2.1/bits/stl_function.h" 3
namespace std {
# 101 "/usr/include/c++/4.2.1/bits/stl_function.h" 3
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
# 133 "/usr/include/c++/4.2.1/bits/stl_function.h" 3
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
# 195 "/usr/include/c++/4.2.1/bits/stl_function.h" 3
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
# 256 "/usr/include/c++/4.2.1/bits/stl_function.h" 3
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
# 311 "/usr/include/c++/4.2.1/bits/stl_function.h" 3
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
# 391 "/usr/include/c++/4.2.1/bits/stl_function.h" 3
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
# 480 "/usr/include/c++/4.2.1/bits/stl_function.h" 3
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
# 582 "/usr/include/c++/4.2.1/bits/stl_function.h" 3
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
# 54 "/usr/include/c++/4.2.1/string" 2 3
# 1 "/usr/include/c++/4.2.1/bits/basic_string.h" 1 3
# 44 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
       
# 45 "/usr/include/c++/4.2.1/bits/basic_string.h" 3




namespace std {
# 110 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 147 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      struct _Rep_base
      {
 size_type _M_length;
 size_type _M_capacity;
 _Atomic_word _M_refcount;
      };

      struct _Rep : _Rep_base
      {

 typedef typename _Alloc::template rebind<char>::other _Raw_bytes_alloc;
# 172 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 463 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 518 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 629 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      void
      resize(size_type __n, _CharT __c);
# 642 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      void
      resize(size_type __n)
      { this->resize(__n, _CharT()); }





      size_type
      capacity() const
      { return _M_rep()->_M_capacity; }
# 671 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      void
      reserve(size_type __res_arg = 0);




      void
      clear()
      { _M_mutate(0, this->size(), 0); }




      bool
      empty() const
      { return this->size() == 0; }
# 699 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      const_reference
      operator[] (size_type __pos) const
      {
 ;
 return _M_data()[__pos];
      }
# 716 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      reference
      operator[](size_type __pos)
      {

 ;

 ;
 _M_leak();
 return _M_data()[__pos];
      }
# 737 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      const_reference
      at(size_type __n) const
      {
 if (__n >= this->size())
   __throw_out_of_range(("basic_string::at"));
 return _M_data()[__n];
      }
# 756 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 816 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 848 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      append(size_type __n, _CharT __c);
# 859 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 898 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      assign(const basic_string& __str, size_type __pos, size_type __n)
      { return this->assign(__str._M_data()
       + __str._M_check(__pos, "basic_string::assign"),
       __str._M_limit(__pos, __n)); }
# 914 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      assign(const _CharT* __s, size_type __n);
# 926 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      assign(const _CharT* __s)
      {
 ;
 return this->assign(__s, traits_type::length(__s));
      }
# 942 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      assign(size_type __n, _CharT __c)
      { return _M_replace_aux(size_type(0), this->size(), __n, __c); }
# 954 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      template<class _InputIterator>
        basic_string&
        assign(_InputIterator __first, _InputIterator __last)
        { return this->replace(_M_ibegin(), _M_iend(), __first, __last); }
# 971 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      void
      insert(iterator __p, size_type __n, _CharT __c)
      { this->replace(__p, __p, __n, __c); }
# 986 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      template<class _InputIterator>
        void
        insert(iterator __p, _InputIterator __beg, _InputIterator __end)
        { this->replace(__p, __p, __beg, __end); }
# 1002 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      insert(size_type __pos1, const basic_string& __str)
      { return this->insert(__pos1, __str, size_type(0), __str.size()); }
# 1024 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      insert(size_type __pos1, const basic_string& __str,
      size_type __pos2, size_type __n)
      { return this->insert(__pos1, __str._M_data()
       + __str._M_check(__pos2, "basic_string::insert"),
       __str._M_limit(__pos2, __n)); }
# 1047 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      insert(size_type __pos, const _CharT* __s, size_type __n);
# 1065 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      insert(size_type __pos, const _CharT* __s)
      {
 ;
 return this->insert(__pos, __s, traits_type::length(__s));
      }
# 1088 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      insert(size_type __pos, size_type __n, _CharT __c)
      { return _M_replace_aux(_M_check(__pos, "basic_string::insert"),
         size_type(0), __n, __c); }
# 1105 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      iterator
      insert(iterator __p, _CharT __c)
      {
 ;
 const size_type __pos = __p - _M_ibegin();
 _M_replace_aux(__pos, size_type(0), size_type(1), __c);
 _M_rep()->_M_set_leaked();
 return iterator(_M_data() + __pos);
      }
# 1129 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      erase(size_type __pos = 0, size_type __n = npos)
      {
 _M_mutate(_M_check(__pos, "basic_string::erase"),
    _M_limit(__pos, __n), size_type(0));
 return *this;
      }
# 1145 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      iterator
      erase(iterator __position)
      {
 ;

 const size_type __pos = __position - _M_ibegin();
 _M_mutate(__pos, size_type(1), size_type(0));
 _M_rep()->_M_set_leaked();
 return iterator(_M_data() + __pos);
      }
# 1165 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      iterator
      erase(iterator __first, iterator __last)
      {
 ;

        const size_type __pos = __first - _M_ibegin();
 _M_mutate(__pos, __last - __first, size_type(0));
 _M_rep()->_M_set_leaked();
 return iterator(_M_data() + __pos);
      }
# 1192 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      replace(size_type __pos, size_type __n, const basic_string& __str)
      { return this->replace(__pos, __n, __str._M_data(), __str.size()); }
# 1214 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      replace(size_type __pos1, size_type __n1, const basic_string& __str,
       size_type __pos2, size_type __n2)
      { return this->replace(__pos1, __n1, __str._M_data()
        + __str._M_check(__pos2, "basic_string::replace"),
        __str._M_limit(__pos2, __n2)); }
# 1238 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      replace(size_type __pos, size_type __n1, const _CharT* __s,
       size_type __n2);
# 1257 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      replace(size_type __pos, size_type __n1, const _CharT* __s)
      {
 ;
 return this->replace(__pos, __n1, __s, traits_type::length(__s));
      }
# 1280 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      replace(size_type __pos, size_type __n1, size_type __n2, _CharT __c)
      { return _M_replace_aux(_M_check(__pos, "basic_string::replace"),
         _M_limit(__pos, __n1), __n2, __c); }
# 1298 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      replace(iterator __i1, iterator __i2, const basic_string& __str)
      { return this->replace(__i1, __i2, __str._M_data(), __str.size()); }
# 1316 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      replace(iterator __i1, iterator __i2, const _CharT* __s, size_type __n)
      {
 ;

 return this->replace(__i1 - _M_ibegin(), __i2 - __i1, __s, __n);
      }
# 1337 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      replace(iterator __i1, iterator __i2, const _CharT* __s)
      {
 ;
 return this->replace(__i1, __i2, __s, traits_type::length(__s));
      }
# 1358 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string&
      replace(iterator __i1, iterator __i2, size_type __n, _CharT __c)
      {
 ;

 return _M_replace_aux(__i1 - _M_ibegin(), __i2 - __i1, __n, __c);
      }
# 1380 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 1511 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      copy(_CharT* __s, size_type __n, size_type __pos = 0) const;
# 1521 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      void
      swap(basic_string& __s);
# 1531 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      const _CharT*
      c_str() const
      { return _M_data(); }







      const _CharT*
      data() const
      { return _M_data(); }




      allocator_type
      get_allocator() const
      { return _M_dataplus; }
# 1563 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find(const _CharT* __s, size_type __pos, size_type __n) const;
# 1576 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find(const basic_string& __str, size_type __pos = 0) const
      { return this->find(__str.data(), __pos, __str.size()); }
# 1590 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find(const _CharT* __s, size_type __pos = 0) const
      {
 ;
 return this->find(__s, __pos, traits_type::length(__s));
      }
# 1607 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find(_CharT __c, size_type __pos = 0) const;
# 1620 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      rfind(const basic_string& __str, size_type __pos = npos) const
      { return this->rfind(__str.data(), __pos, __str.size()); }
# 1635 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      rfind(const _CharT* __s, size_type __pos, size_type __n) const;
# 1648 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      rfind(const _CharT* __s, size_type __pos = npos) const
      {
 ;
 return this->rfind(__s, __pos, traits_type::length(__s));
      }
# 1665 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      rfind(_CharT __c, size_type __pos = npos) const;
# 1678 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_first_of(const basic_string& __str, size_type __pos = 0) const
      { return this->find_first_of(__str.data(), __pos, __str.size()); }
# 1693 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_first_of(const _CharT* __s, size_type __pos, size_type __n) const;
# 1706 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_first_of(const _CharT* __s, size_type __pos = 0) const
      {
 ;
 return this->find_first_of(__s, __pos, traits_type::length(__s));
      }
# 1725 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_first_of(_CharT __c, size_type __pos = 0) const
      { return this->find(__c, __pos); }
# 1739 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_last_of(const basic_string& __str, size_type __pos = npos) const
      { return this->find_last_of(__str.data(), __pos, __str.size()); }
# 1754 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_last_of(const _CharT* __s, size_type __pos, size_type __n) const;
# 1767 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_last_of(const _CharT* __s, size_type __pos = npos) const
      {
 ;
 return this->find_last_of(__s, __pos, traits_type::length(__s));
      }
# 1786 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_last_of(_CharT __c, size_type __pos = npos) const
      { return this->rfind(__c, __pos); }
# 1800 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_first_not_of(const basic_string& __str, size_type __pos = 0) const
      { return this->find_first_not_of(__str.data(), __pos, __str.size()); }
# 1815 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_first_not_of(const _CharT* __s, size_type __pos,
   size_type __n) const;
# 1829 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_first_not_of(const _CharT* __s, size_type __pos = 0) const
      {
 ;
 return this->find_first_not_of(__s, __pos, traits_type::length(__s));
      }
# 1846 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_first_not_of(_CharT __c, size_type __pos = 0) const;
# 1859 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_last_not_of(const basic_string& __str, size_type __pos = npos) const
      { return this->find_last_not_of(__str.data(), __pos, __str.size()); }
# 1875 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_last_not_of(const _CharT* __s, size_type __pos,
         size_type __n) const;
# 1888 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_last_not_of(const _CharT* __s, size_type __pos = npos) const
      {
 ;
 return this->find_last_not_of(__s, __pos, traits_type::length(__s));
      }
# 1905 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      size_type
      find_last_not_of(_CharT __c, size_type __pos = npos) const;
# 1920 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      basic_string
      substr(size_type __pos = 0, size_type __n = npos) const
      { return basic_string(*this,
       _M_check(__pos, "basic_string::substr"), __n); }
# 1938 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 1968 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      int
      compare(size_type __pos, size_type __n, const basic_string& __str) const;
# 1992 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      int
      compare(size_type __pos1, size_type __n1, const basic_string& __str,
       size_type __pos2, size_type __n2) const;
# 2010 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      int
      compare(const _CharT* __s) const;
# 2033 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      int
      compare(size_type __pos, size_type __n1, const _CharT* __s) const;
# 2058 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
      int
      compare(size_type __pos, size_type __n1, const _CharT* __s,
       size_type __n2) const;
  };

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline basic_string<_CharT, _Traits, _Alloc>::
    basic_string()

    : _M_dataplus(_S_empty_rep()._M_refdata(), _Alloc()) { }
# 2079 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 2150 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 2187 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 2224 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 2261 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 2298 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 2335 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 2372 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
  template<typename _CharT, typename _Traits, typename _Alloc>
    inline void
    swap(basic_string<_CharT, _Traits, _Alloc>& __lhs,
  basic_string<_CharT, _Traits, _Alloc>& __rhs)
    { __lhs.swap(__rhs); }
# 2389 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_istream<_CharT, _Traits>&
    operator>>(basic_istream<_CharT, _Traits>& __is,
        basic_string<_CharT, _Traits, _Alloc>& __str);

  template<>
    basic_istream<char>&
    operator>>(basic_istream<char>& __is, basic_string<char>& __str);
# 2407 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
  template<typename _CharT, typename _Traits, typename _Alloc>
    inline basic_ostream<_CharT, _Traits>&
    operator<<(basic_ostream<_CharT, _Traits>& __os,
        const basic_string<_CharT, _Traits, _Alloc>& __str)
    {


      return __ostream_insert(__os, __str.data(), __str.size());
    }
# 2430 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_istream<_CharT, _Traits>&
    getline(basic_istream<_CharT, _Traits>& __is,
     basic_string<_CharT, _Traits, _Alloc>& __str, _CharT __delim);
# 2447 "/usr/include/c++/4.2.1/bits/basic_string.h" 3
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
# 55 "/usr/include/c++/4.2.1/string" 2 3


# 1 "/usr/include/c++/4.2.1/algorithm" 1 3
# 63 "/usr/include/c++/4.2.1/algorithm" 3
       
# 64 "/usr/include/c++/4.2.1/algorithm" 3




# 1 "/usr/include/c++/4.2.1/bits/stl_algo.h" 1 3
# 65 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
# 1 "/usr/include/c++/4.2.1/bits/stl_heap.h" 1 3
# 65 "/usr/include/c++/4.2.1/bits/stl_heap.h" 3
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
# 139 "/usr/include/c++/4.2.1/bits/stl_heap.h" 3
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
# 187 "/usr/include/c++/4.2.1/bits/stl_heap.h" 3
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
# 251 "/usr/include/c++/4.2.1/bits/stl_heap.h" 3
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
# 317 "/usr/include/c++/4.2.1/bits/stl_heap.h" 3
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
# 342 "/usr/include/c++/4.2.1/bits/stl_heap.h" 3
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
# 382 "/usr/include/c++/4.2.1/bits/stl_heap.h" 3
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
# 420 "/usr/include/c++/4.2.1/bits/stl_heap.h" 3
  template<typename _RandomAccessIterator>
    void
    sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
    {

     

     

      ;


      while (__last - __first > 1)
 std::pop_heap(__first, _RandomAccessIterator(__last--));
    }
# 446 "/usr/include/c++/4.2.1/bits/stl_heap.h" 3
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
# 66 "/usr/include/c++/4.2.1/bits/stl_algo.h" 2 3
# 1 "/usr/include/c++/4.2.1/bits/stl_tempbuf.h" 1 3
# 67 "/usr/include/c++/4.2.1/bits/stl_tempbuf.h" 3
namespace std {
# 76 "/usr/include/c++/4.2.1/bits/stl_tempbuf.h" 3
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
# 67 "/usr/include/c++/4.2.1/bits/stl_algo.h" 2 3




namespace std {
# 85 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 119 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 151 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 316 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
  template<typename _InputIterator, typename _Tp>
    inline _InputIterator
    find(_InputIterator __first, _InputIterator __last,
  const _Tp& __val)
    {

     
     

      ;
      return std::__find(__first, __last, __val,
           std::__iterator_category(__first));
    }
# 338 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
  template<typename _InputIterator, typename _Predicate>
    inline _InputIterator
    find_if(_InputIterator __first, _InputIterator __last,
     _Predicate __pred)
    {

     
     

      ;
      return std::__find_if(__first, __last, __pred,
       std::__iterator_category(__first));
    }
# 360 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 391 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 422 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 446 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 485 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 556 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 628 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 662 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 720 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 747 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 787 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 848 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 884 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 922 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 957 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 989 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1020 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1053 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1089 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1123 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
  template<typename _ForwardIterator, typename _Generator>
    void
    generate(_ForwardIterator __first, _ForwardIterator __last,
      _Generator __gen)
    {

     
     

      ;

      for ( ; __first != __last; ++__first)
 *__first = __gen();
    }
# 1149 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
  template<typename _OutputIterator, typename _Size, typename _Generator>
    _OutputIterator
    generate_n(_OutputIterator __first, _Size __n, _Generator __gen)
    {

     



      for ( ; __n > 0; --__n, ++__first)
 *__first = __gen();
      return __first;
    }
# 1176 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1212 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1251 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1286 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1312 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1337 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1362 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1384 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1415 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1446 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1487 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1527 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1560 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1599 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1633 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1655 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1682 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
  template<typename _BidirectionalIterator>
    inline void
    reverse(_BidirectionalIterator __first, _BidirectionalIterator __last)
    {

     

      ;
      std::__reverse(__first, __last, std::__iterator_category(__first));
    }
# 1708 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1736 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1915 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1948 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 1974 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
  template<typename _RandomAccessIterator>
    inline void
    random_shuffle(_RandomAccessIterator __first, _RandomAccessIterator __last)
    {

     

      ;

      if (__first != __last)
 for (_RandomAccessIterator __i = __first + 1; __i != __last; ++__i)
   std::iter_swap(__i, __first + (std::rand() % ((__i - __first) + 1)));
    }
# 2001 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 2095 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 2206 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 2535 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 2573 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 2612 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 2677 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 2814 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 2849 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 2882 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 2932 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 2979 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 3029 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 3227 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 3288 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 3711 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 3765 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 3874 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 3915 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4030 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4068 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4107 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4168 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4226 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4258 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4297 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4345 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4394 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4460 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4526 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4582 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4639 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4699 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4756 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4821 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4879 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4906 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4958 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 4993 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 5049 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 5104 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 5160 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 5219 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 5255 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 5285 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 5435 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 5480 "/usr/include/c++/4.2.1/bits/stl_algo.h" 3
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
# 69 "/usr/include/c++/4.2.1/algorithm" 2 3
# 58 "/usr/include/c++/4.2.1/string" 2 3
# 1 "/usr/include/c++/4.2.1/bits/basic_string.tcc" 1 3
# 47 "/usr/include/c++/4.2.1/bits/basic_string.tcc" 3
       
# 48 "/usr/include/c++/4.2.1/bits/basic_string.tcc" 3

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
# 559 "/usr/include/c++/4.2.1/bits/basic_string.tcc" 3
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
# 59 "/usr/include/c++/4.2.1/string" 2 3
# 48 "/usr/include/c++/4.2.1/bits/locale_classes.h" 2 3


namespace std {
# 66 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
  class locale
  {
  public:


    typedef int category;


    class facet;
    class id;
    class _Impl;

    friend class facet;
    friend class _Impl;

    template<typename _Facet>
      friend bool
      has_facet(const locale&) throw();

    template<typename _Facet>
      friend const _Facet&
      use_facet(const locale&);

    template<typename _Cache>
      friend struct __use_cache;
# 104 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
    static const category none = 0;
    static const category ctype = 1L << 0;
    static const category numeric = 1L << 1;
    static const category collate = 1L << 2;
    static const category time = 1L << 3;
    static const category monetary = 1L << 4;
    static const category messages = 1L << 5;
    static const category all = (ctype | numeric | collate |
        time | monetary | messages);
# 123 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
    locale() throw();
# 132 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
    locale(const locale& __other) throw();
# 142 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
    explicit
    locale(const char* __s);
# 157 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
    locale(const locale& __base, const char* __s, category __cat);
# 170 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
    locale(const locale& __base, const locale& __add, category __cat);
# 182 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
    template<typename _Facet>
      locale(const locale& __other, _Facet* __f);


    ~locale() throw();
# 196 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
    const locale&
    operator=(const locale& __other) throw();
# 211 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
    template<typename _Facet>
      locale
      combine(const locale& __other) const;






    string
    name() const;
# 230 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
    bool
    operator==(const locale& __other) const throw ();







    inline bool
    operator!=(const locale& __other) const throw ()
    { return !(this->operator==(__other)); }
# 258 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
    template<typename _Char, typename _Traits, typename _Alloc>
      bool
      operator()(const basic_string<_Char, _Traits, _Alloc>& __s1,
   const basic_string<_Char, _Traits, _Alloc>& __s2) const;
# 274 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
    static locale
    global(const locale&);




    static const locale&
    classic();

  private:

    _Impl* _M_impl;


    static _Impl* _S_classic;


    static _Impl* _S_global;





    static const char* const* const _S_categories;
# 309 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
    enum { _S_categories_size = 6 + 0 };


    static __gthread_once_t _S_once;


    explicit
    locale(_Impl*) throw();

    static void
    _S_initialize();

    static void
    _S_initialize_once();

    static category
    _S_normalize_category(category);

    void
    _M_coalesce(const locale& __base, const locale& __add, category __cat);
  };
# 342 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
  class locale::facet
  {
  private:
    friend class locale;
    friend class locale::_Impl;

    mutable _Atomic_word _M_refcount;


    static __c_locale _S_c_locale;


    static const char _S_c_name[2];


    static __gthread_once_t _S_once;


    static void
    _S_initialize_once();

  protected:
# 373 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
    explicit
    facet(size_t __refs = 0) throw() : _M_refcount(__refs ? 1 : 0)
    { }


    virtual
    ~facet();

    static void
    _S_create_c_locale(__c_locale& __cloc, const char* __s,
         __c_locale __old = 0);

    static __c_locale
    _S_clone_c_locale(__c_locale& __cloc);

    static void
    _S_destroy_c_locale(__c_locale& __cloc);



    static __c_locale
    _S_get_c_locale();

    static const char*
    _S_get_c_name();

  private:
    inline void
    _M_add_reference() const throw()
    { __gnu_cxx::__atomic_add_dispatch(&_M_refcount, 1); }

    inline void
    _M_remove_reference() const throw()
    {
      if (__gnu_cxx::__exchange_and_add_dispatch(&_M_refcount, -1) == 1)
 {
   try
     { delete this; }
   catch (...)
     { }
 }
    }

    facet(const facet&);

    facet&
    operator=(const facet&);
  };
# 433 "/usr/include/c++/4.2.1/bits/locale_classes.h" 3
  class locale::id
  {
  private:
    friend class locale;
    friend class locale::_Impl;

    template<typename _Facet>
      friend const _Facet&
      use_facet(const locale&);

    template<typename _Facet>
      friend bool
      has_facet(const locale&) throw ();




    mutable size_t _M_index;


    static _Atomic_word _S_refcount;

    void
    operator=(const id&);

    id(const id&);

  public:



    id() { }

    size_t
    _M_id() const;
  };



  class locale::_Impl
  {
  public:

    friend class locale;
    friend class locale::facet;

    template<typename _Facet>
      friend bool
      has_facet(const locale&) throw();

    template<typename _Facet>
      friend const _Facet&
      use_facet(const locale&);

    template<typename _Cache>
      friend struct __use_cache;

  private:

    _Atomic_word _M_refcount;
    const facet** _M_facets;
    size_t _M_facets_size;
    const facet** _M_caches;
    char** _M_names;
    static const locale::id* const _S_id_ctype[];
    static const locale::id* const _S_id_numeric[];
    static const locale::id* const _S_id_collate[];
    static const locale::id* const _S_id_time[];
    static const locale::id* const _S_id_monetary[];
    static const locale::id* const _S_id_messages[];
    static const locale::id* const* const _S_facet_categories[];

    inline void
    _M_add_reference() throw()
    { __gnu_cxx::__atomic_add_dispatch(&_M_refcount, 1); }

    inline void
    _M_remove_reference() throw()
    {
      if (__gnu_cxx::__exchange_and_add_dispatch(&_M_refcount, -1) == 1)
 {
   try
     { delete this; }
   catch(...)
     { }
 }
    }

    _Impl(const _Impl&, size_t);
    _Impl(const char*, size_t);
    _Impl(size_t) throw();

   ~_Impl() throw();

    _Impl(const _Impl&);

    void
    operator=(const _Impl&);

    inline bool
    _M_check_same_name()
    {
      bool __ret = true;
      if (_M_names[1])

 for (size_t __i = 0; __ret && __i < _S_categories_size - 1; ++__i)
   __ret = std::strcmp(_M_names[__i], _M_names[__i + 1]) == 0;
      return __ret;
    }

    void
    _M_replace_categories(const _Impl*, category);

    void
    _M_replace_category(const _Impl*, const locale::id* const*);

    void
    _M_replace_facet(const _Impl*, const locale::id*);

    void
    _M_install_facet(const locale::id*, const facet*);

    template<typename _Facet>
      inline void
      _M_init_facet(_Facet* __facet)
      { _M_install_facet(&_Facet::id, __facet); }

    void
    _M_install_cache(const facet*, size_t);
  };

  template<typename _Facet>
    locale::locale(const locale& __other, _Facet* __f)
    {
      _M_impl = new _Impl(*__other._M_impl, 1);

      try
 { _M_impl->_M_install_facet(&_Facet::id, __f); }
      catch(...)
 {
   _M_impl->_M_remove_reference();
   throw;
 }
      delete [] _M_impl->_M_names[0];
      _M_impl->_M_names[0] = 0;
    }

}
# 48 "/usr/include/c++/4.2.1/bits/ios_base.h" 2 3

namespace std {





  enum _Ios_Fmtflags
    {
      _S_boolalpha = 1L << 0,
      _S_dec = 1L << 1,
      _S_fixed = 1L << 2,
      _S_hex = 1L << 3,
      _S_internal = 1L << 4,
      _S_left = 1L << 5,
      _S_oct = 1L << 6,
      _S_right = 1L << 7,
      _S_scientific = 1L << 8,
      _S_showbase = 1L << 9,
      _S_showpoint = 1L << 10,
      _S_showpos = 1L << 11,
      _S_skipws = 1L << 12,
      _S_unitbuf = 1L << 13,
      _S_uppercase = 1L << 14,
      _S_adjustfield = _S_left | _S_right | _S_internal,
      _S_basefield = _S_dec | _S_oct | _S_hex,
      _S_floatfield = _S_scientific | _S_fixed,
      _S_ios_fmtflags_end = 1L << 16
    };

  inline _Ios_Fmtflags
  operator&(_Ios_Fmtflags __a, _Ios_Fmtflags __b)
  { return _Ios_Fmtflags(static_cast<int>(__a) & static_cast<int>(__b)); }

  inline _Ios_Fmtflags
  operator|(_Ios_Fmtflags __a, _Ios_Fmtflags __b)
  { return _Ios_Fmtflags(static_cast<int>(__a) | static_cast<int>(__b)); }

  inline _Ios_Fmtflags
  operator^(_Ios_Fmtflags __a, _Ios_Fmtflags __b)
  { return _Ios_Fmtflags(static_cast<int>(__a) ^ static_cast<int>(__b)); }

  inline _Ios_Fmtflags&
  operator|=(_Ios_Fmtflags& __a, _Ios_Fmtflags __b)
  { return __a = __a | __b; }

  inline _Ios_Fmtflags&
  operator&=(_Ios_Fmtflags& __a, _Ios_Fmtflags __b)
  { return __a = __a & __b; }

  inline _Ios_Fmtflags&
  operator^=(_Ios_Fmtflags& __a, _Ios_Fmtflags __b)
  { return __a = __a ^ __b; }

  inline _Ios_Fmtflags
  operator~(_Ios_Fmtflags __a)
  { return _Ios_Fmtflags(~static_cast<int>(__a)); }


  enum _Ios_Openmode
    {
      _S_app = 1L << 0,
      _S_ate = 1L << 1,
      _S_bin = 1L << 2,
      _S_in = 1L << 3,
      _S_out = 1L << 4,
      _S_trunc = 1L << 5,
      _S_ios_openmode_end = 1L << 16
    };

  inline _Ios_Openmode
  operator&(_Ios_Openmode __a, _Ios_Openmode __b)
  { return _Ios_Openmode(static_cast<int>(__a) & static_cast<int>(__b)); }

  inline _Ios_Openmode
  operator|(_Ios_Openmode __a, _Ios_Openmode __b)
  { return _Ios_Openmode(static_cast<int>(__a) | static_cast<int>(__b)); }

  inline _Ios_Openmode
  operator^(_Ios_Openmode __a, _Ios_Openmode __b)
  { return _Ios_Openmode(static_cast<int>(__a) ^ static_cast<int>(__b)); }

  inline _Ios_Openmode&
  operator|=(_Ios_Openmode& __a, _Ios_Openmode __b)
  { return __a = __a | __b; }

  inline _Ios_Openmode&
  operator&=(_Ios_Openmode& __a, _Ios_Openmode __b)
  { return __a = __a & __b; }

  inline _Ios_Openmode&
  operator^=(_Ios_Openmode& __a, _Ios_Openmode __b)
  { return __a = __a ^ __b; }

  inline _Ios_Openmode
  operator~(_Ios_Openmode __a)
  { return _Ios_Openmode(~static_cast<int>(__a)); }


  enum _Ios_Iostate
    {
      _S_goodbit = 0,
      _S_badbit = 1L << 0,
      _S_eofbit = 1L << 1,
      _S_failbit = 1L << 2,
      _S_ios_iostate_end = 1L << 16
    };

  inline _Ios_Iostate
  operator&(_Ios_Iostate __a, _Ios_Iostate __b)
  { return _Ios_Iostate(static_cast<int>(__a) & static_cast<int>(__b)); }

  inline _Ios_Iostate
  operator|(_Ios_Iostate __a, _Ios_Iostate __b)
  { return _Ios_Iostate(static_cast<int>(__a) | static_cast<int>(__b)); }

  inline _Ios_Iostate
  operator^(_Ios_Iostate __a, _Ios_Iostate __b)
  { return _Ios_Iostate(static_cast<int>(__a) ^ static_cast<int>(__b)); }

  inline _Ios_Iostate&
  operator|=(_Ios_Iostate& __a, _Ios_Iostate __b)
  { return __a = __a | __b; }

  inline _Ios_Iostate&
  operator&=(_Ios_Iostate& __a, _Ios_Iostate __b)
  { return __a = __a & __b; }

  inline _Ios_Iostate&
  operator^=(_Ios_Iostate& __a, _Ios_Iostate __b)
  { return __a = __a ^ __b; }

  inline _Ios_Iostate
  operator~(_Ios_Iostate __a)
  { return _Ios_Iostate(~static_cast<int>(__a)); }

  enum _Ios_Seekdir
    {
      _S_beg = 0,
      _S_cur = 1,
      _S_end = 2,
      _S_ios_seekdir_end = 1L << 16
    };
# 201 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
  class ios_base
  {
  public:



    class failure : public exception
    {
    public:


      explicit
      failure(const string& __str) throw();



      virtual
      ~failure() throw();

      virtual const char*
      what() const throw();

    private:
      string _M_msg;
    };
# 253 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    typedef _Ios_Fmtflags fmtflags;


    static const fmtflags boolalpha = _S_boolalpha;


    static const fmtflags dec = _S_dec;


    static const fmtflags fixed = _S_fixed;


    static const fmtflags hex = _S_hex;




    static const fmtflags internal = _S_internal;



    static const fmtflags left = _S_left;


    static const fmtflags oct = _S_oct;



    static const fmtflags right = _S_right;


    static const fmtflags scientific = _S_scientific;



    static const fmtflags showbase = _S_showbase;



    static const fmtflags showpoint = _S_showpoint;


    static const fmtflags showpos = _S_showpos;


    static const fmtflags skipws = _S_skipws;


    static const fmtflags unitbuf = _S_unitbuf;



    static const fmtflags uppercase = _S_uppercase;


    static const fmtflags adjustfield = _S_adjustfield;


    static const fmtflags basefield = _S_basefield;


    static const fmtflags floatfield = _S_floatfield;
# 328 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    typedef _Ios_Iostate iostate;



    static const iostate badbit = _S_badbit;


    static const iostate eofbit = _S_eofbit;




    static const iostate failbit = _S_failbit;


    static const iostate goodbit = _S_goodbit;
# 359 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    typedef _Ios_Openmode openmode;


    static const openmode app = _S_app;


    static const openmode ate = _S_ate;





    static const openmode binary = _S_bin;


    static const openmode in = _S_in;


    static const openmode out = _S_out;


    static const openmode trunc = _S_trunc;
# 392 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    typedef _Ios_Seekdir seekdir;


    static const seekdir beg = _S_beg;


    static const seekdir cur = _S_cur;


    static const seekdir end = _S_end;


    typedef int io_state;
    typedef int open_mode;
    typedef int seek_dir;

    typedef std::streampos streampos;
    typedef std::streamoff streamoff;
# 418 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    enum event
    {
      erase_event,
      imbue_event,
      copyfmt_event
    };
# 435 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    typedef void (*event_callback) (event, ios_base&, int);
# 447 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    void
    register_callback(event_callback __fn, int __index);

  protected:






    streamsize _M_precision;
    streamsize _M_width;
    fmtflags _M_flags;
    iostate _M_exception;
    iostate _M_streambuf_state;




    struct _Callback_list
    {

      _Callback_list* _M_next;
      ios_base::event_callback _M_fn;
      int _M_index;
      _Atomic_word _M_refcount;

      _Callback_list(ios_base::event_callback __fn, int __index,
       _Callback_list* __cb)
      : _M_next(__cb), _M_fn(__fn), _M_index(__index), _M_refcount(0) { }

      void
      _M_add_reference() { __gnu_cxx::__atomic_add_dispatch(&_M_refcount, 1); }


      int
      _M_remove_reference()
      { return __gnu_cxx::__exchange_and_add_dispatch(&_M_refcount, -1); }
    };

     _Callback_list* _M_callbacks;

    void
    _M_call_callbacks(event __ev) throw();

    void
    _M_dispose_callbacks(void);


    struct _Words
    {
      void* _M_pword;
      long _M_iword;
      _Words() : _M_pword(0), _M_iword(0) { }
    };


    _Words _M_word_zero;



    enum { _S_local_word_size = 8 };
    _Words _M_local_word[_S_local_word_size];


    int _M_word_size;
    _Words* _M_word;

    _Words&
    _M_grow_words(int __index, bool __iword);


    locale _M_ios_locale;

    void
    _M_init();

  public:





    class Init
    {
      friend class ios_base;
    public:
      Init();
      ~Init();

    private:
      static _Atomic_word _S_refcount;
      static bool _S_synced_with_stdio;
    };






    inline fmtflags
    flags() const { return _M_flags; }
# 557 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    inline fmtflags
    flags(fmtflags __fmtfl)
    {
      fmtflags __old = _M_flags;
      _M_flags = __fmtfl;
      return __old;
    }
# 573 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    inline fmtflags
    setf(fmtflags __fmtfl)
    {
      fmtflags __old = _M_flags;
      _M_flags |= __fmtfl;
      return __old;
    }
# 590 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    inline fmtflags
    setf(fmtflags __fmtfl, fmtflags __mask)
    {
      fmtflags __old = _M_flags;
      _M_flags &= ~__mask;
      _M_flags |= (__fmtfl & __mask);
      return __old;
    }







    inline void
    unsetf(fmtflags __mask) { _M_flags &= ~__mask; }
# 617 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    inline streamsize
    precision() const { return _M_precision; }






    inline streamsize
    precision(streamsize __prec)
    {
      streamsize __old = _M_precision;
      _M_precision = __prec;
      return __old;
    }







    inline streamsize
    width() const { return _M_width; }






    inline streamsize
    width(streamsize __wide)
    {
      streamsize __old = _M_width;
      _M_width = __wide;
      return __old;
    }
# 666 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    static bool
    sync_with_stdio(bool __sync = true);
# 678 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    locale
    imbue(const locale& __loc);
# 689 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    inline locale
    getloc() const { return _M_ios_locale; }
# 699 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    inline const locale&
    _M_getloc() const { return _M_ios_locale; }
# 717 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    static int
    xalloc() throw();
# 733 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    inline long&
    iword(int __ix)
    {
      _Words& __word = (__ix < _M_word_size)
   ? _M_word[__ix] : _M_grow_words(__ix, true);
      return __word._M_iword;
    }
# 754 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    inline void*&
    pword(int __ix)
    {
      _Words& __word = (__ix < _M_word_size)
   ? _M_word[__ix] : _M_grow_words(__ix, false);
      return __word._M_pword;
    }
# 771 "/usr/include/c++/4.2.1/bits/ios_base.h" 3
    virtual ~ios_base();

  protected:
    ios_base();



  private:
    ios_base(const ios_base&);

    ios_base&
    operator=(const ios_base&);
  };



  inline ios_base&
  boolalpha(ios_base& __base)
  {
    __base.setf(ios_base::boolalpha);
    return __base;
  }


  inline ios_base&
  noboolalpha(ios_base& __base)
  {
    __base.unsetf(ios_base::boolalpha);
    return __base;
  }


  inline ios_base&
  showbase(ios_base& __base)
  {
    __base.setf(ios_base::showbase);
    return __base;
  }


  inline ios_base&
  noshowbase(ios_base& __base)
  {
    __base.unsetf(ios_base::showbase);
    return __base;
  }


  inline ios_base&
  showpoint(ios_base& __base)
  {
    __base.setf(ios_base::showpoint);
    return __base;
  }


  inline ios_base&
  noshowpoint(ios_base& __base)
  {
    __base.unsetf(ios_base::showpoint);
    return __base;
  }


  inline ios_base&
  showpos(ios_base& __base)
  {
    __base.setf(ios_base::showpos);
    return __base;
  }


  inline ios_base&
  noshowpos(ios_base& __base)
  {
    __base.unsetf(ios_base::showpos);
    return __base;
  }


  inline ios_base&
  skipws(ios_base& __base)
  {
    __base.setf(ios_base::skipws);
    return __base;
  }


  inline ios_base&
  noskipws(ios_base& __base)
  {
    __base.unsetf(ios_base::skipws);
    return __base;
  }


  inline ios_base&
  uppercase(ios_base& __base)
  {
    __base.setf(ios_base::uppercase);
    return __base;
  }


  inline ios_base&
  nouppercase(ios_base& __base)
  {
    __base.unsetf(ios_base::uppercase);
    return __base;
  }


  inline ios_base&
  unitbuf(ios_base& __base)
  {
     __base.setf(ios_base::unitbuf);
     return __base;
  }


  inline ios_base&
  nounitbuf(ios_base& __base)
  {
     __base.unsetf(ios_base::unitbuf);
     return __base;
  }



  inline ios_base&
  internal(ios_base& __base)
  {
     __base.setf(ios_base::internal, ios_base::adjustfield);
     return __base;
  }


  inline ios_base&
  left(ios_base& __base)
  {
    __base.setf(ios_base::left, ios_base::adjustfield);
    return __base;
  }


  inline ios_base&
  right(ios_base& __base)
  {
    __base.setf(ios_base::right, ios_base::adjustfield);
    return __base;
  }



  inline ios_base&
  dec(ios_base& __base)
  {
    __base.setf(ios_base::dec, ios_base::basefield);
    return __base;
  }


  inline ios_base&
  hex(ios_base& __base)
  {
    __base.setf(ios_base::hex, ios_base::basefield);
    return __base;
  }


  inline ios_base&
  oct(ios_base& __base)
  {
    __base.setf(ios_base::oct, ios_base::basefield);
    return __base;
  }



  inline ios_base&
  fixed(ios_base& __base)
  {
    __base.setf(ios_base::fixed, ios_base::floatfield);
    return __base;
  }


  inline ios_base&
  scientific(ios_base& __base)
  {
    __base.setf(ios_base::scientific, ios_base::floatfield);
    return __base;
  }

}
# 49 "/usr/include/c++/4.2.1/ios" 2 3
# 1 "/usr/include/c++/4.2.1/streambuf" 1 3
# 42 "/usr/include/c++/4.2.1/streambuf" 3
       
# 43 "/usr/include/c++/4.2.1/streambuf" 3
# 51 "/usr/include/c++/4.2.1/streambuf" 3
namespace std {






  template<typename _CharT, typename _Traits>
    streamsize
    __copy_streambufs_eof(basic_streambuf<_CharT, _Traits>*,
     basic_streambuf<_CharT, _Traits>*, bool&);
# 123 "/usr/include/c++/4.2.1/streambuf" 3
  template<typename _CharT, typename _Traits>
    class basic_streambuf
    {
    public:






      typedef _CharT char_type;
      typedef _Traits traits_type;
      typedef typename traits_type::int_type int_type;
      typedef typename traits_type::pos_type pos_type;
      typedef typename traits_type::off_type off_type;
# 146 "/usr/include/c++/4.2.1/streambuf" 3
      typedef basic_streambuf<char_type, traits_type> __streambuf_type;


      friend class basic_ios<char_type, traits_type>;
      friend class basic_istream<char_type, traits_type>;
      friend class basic_ostream<char_type, traits_type>;
      friend class istreambuf_iterator<char_type, traits_type>;
      friend class ostreambuf_iterator<char_type, traits_type>;

      friend streamsize
      __copy_streambufs_eof<>(__streambuf_type*, __streambuf_type*, bool&);

      template<typename _CharT2>
        friend typename __gnu_cxx::__enable_if<__is_char<_CharT2>::__value,
            _CharT2*>::__type
        __copy_aux(istreambuf_iterator<_CharT2>,
     istreambuf_iterator<_CharT2>, _CharT2*);

      template<typename _CharT2>
        friend typename __gnu_cxx::__enable_if<__is_char<_CharT2>::__value,
      istreambuf_iterator<_CharT2> >::__type
        find(istreambuf_iterator<_CharT2>, istreambuf_iterator<_CharT2>,
      const _CharT2&);

      template<typename _CharT2, typename _Traits2>
        friend basic_istream<_CharT2, _Traits2>&
        operator>>(basic_istream<_CharT2, _Traits2>&, _CharT2*);

      template<typename _CharT2, typename _Traits2, typename _Alloc>
        friend basic_istream<_CharT2, _Traits2>&
        operator>>(basic_istream<_CharT2, _Traits2>&,
     basic_string<_CharT2, _Traits2, _Alloc>&);

      template<typename _CharT2, typename _Traits2, typename _Alloc>
        friend basic_istream<_CharT2, _Traits2>&
        getline(basic_istream<_CharT2, _Traits2>&,
  basic_string<_CharT2, _Traits2, _Alloc>&, _CharT2);

    protected:
# 195 "/usr/include/c++/4.2.1/streambuf" 3
      char_type* _M_in_beg;
      char_type* _M_in_cur;
      char_type* _M_in_end;
      char_type* _M_out_beg;
      char_type* _M_out_cur;
      char_type* _M_out_end;






      locale _M_buf_locale;

  public:

      virtual
      ~basic_streambuf()
      { }
# 223 "/usr/include/c++/4.2.1/streambuf" 3
      locale
      pubimbue(const locale &__loc)
      {
 locale __tmp(this->getloc());
 this->imbue(__loc);
 _M_buf_locale = __loc;
 return __tmp;
      }
# 240 "/usr/include/c++/4.2.1/streambuf" 3
      locale
      getloc() const
      { return _M_buf_locale; }
# 253 "/usr/include/c++/4.2.1/streambuf" 3
      __streambuf_type*
      pubsetbuf(char_type* __s, streamsize __n)
      { return this->setbuf(__s, __n); }

      pos_type
      pubseekoff(off_type __off, ios_base::seekdir __way,
   ios_base::openmode __mode = ios_base::in | ios_base::out)
      { return this->seekoff(__off, __way, __mode); }

      pos_type
      pubseekpos(pos_type __sp,
   ios_base::openmode __mode = ios_base::in | ios_base::out)
      { return this->seekpos(__sp, __mode); }

      int
      pubsync() { return this->sync(); }
# 280 "/usr/include/c++/4.2.1/streambuf" 3
      streamsize
      in_avail()
      {
 const streamsize __ret = this->egptr() - this->gptr();
 return __ret ? __ret : this->showmanyc();
      }
# 294 "/usr/include/c++/4.2.1/streambuf" 3
      int_type
      snextc()
      {
 int_type __ret = traits_type::eof();
 if (__builtin_expect(!traits_type::eq_int_type(this->sbumpc(),
             __ret), true))
   __ret = this->sgetc();
 return __ret;
      }
# 312 "/usr/include/c++/4.2.1/streambuf" 3
      int_type
      sbumpc()
      {
 int_type __ret;
 if (__builtin_expect(this->gptr() < this->egptr(), true))
   {
     __ret = traits_type::to_int_type(*this->gptr());
     this->gbump(1);
   }
 else
   __ret = this->uflow();
 return __ret;
      }
# 334 "/usr/include/c++/4.2.1/streambuf" 3
      int_type
      sgetc()
      {
 int_type __ret;
 if (__builtin_expect(this->gptr() < this->egptr(), true))
   __ret = traits_type::to_int_type(*this->gptr());
 else
   __ret = this->underflow();
 return __ret;
      }
# 353 "/usr/include/c++/4.2.1/streambuf" 3
      streamsize
      sgetn(char_type* __s, streamsize __n)
      { return this->xsgetn(__s, __n); }
# 367 "/usr/include/c++/4.2.1/streambuf" 3
      int_type
      sputbackc(char_type __c)
      {
 int_type __ret;
 const bool __testpos = this->eback() < this->gptr();
 if (__builtin_expect(!__testpos ||
        !traits_type::eq(__c, this->gptr()[-1]), false))
   __ret = this->pbackfail(traits_type::to_int_type(__c));
 else
   {
     this->gbump(-1);
     __ret = traits_type::to_int_type(*this->gptr());
   }
 return __ret;
      }
# 392 "/usr/include/c++/4.2.1/streambuf" 3
      int_type
      sungetc()
      {
 int_type __ret;
 if (__builtin_expect(this->eback() < this->gptr(), true))
   {
     this->gbump(-1);
     __ret = traits_type::to_int_type(*this->gptr());
   }
 else
   __ret = this->pbackfail();
 return __ret;
      }
# 419 "/usr/include/c++/4.2.1/streambuf" 3
      int_type
      sputc(char_type __c)
      {
 int_type __ret;
 if (__builtin_expect(this->pptr() < this->epptr(), true))
   {
     *this->pptr() = __c;
     this->pbump(1);
     __ret = traits_type::to_int_type(__c);
   }
 else
   __ret = this->overflow(traits_type::to_int_type(__c));
 return __ret;
      }
# 445 "/usr/include/c++/4.2.1/streambuf" 3
      streamsize
      sputn(const char_type* __s, streamsize __n)
      { return this->xsputn(__s, __n); }

    protected:
# 459 "/usr/include/c++/4.2.1/streambuf" 3
      basic_streambuf()
      : _M_in_beg(0), _M_in_cur(0), _M_in_end(0),
      _M_out_beg(0), _M_out_cur(0), _M_out_end(0),
      _M_buf_locale(locale())
      { }
# 477 "/usr/include/c++/4.2.1/streambuf" 3
      char_type*
      eback() const { return _M_in_beg; }

      char_type*
      gptr() const { return _M_in_cur; }

      char_type*
      egptr() const { return _M_in_end; }
# 493 "/usr/include/c++/4.2.1/streambuf" 3
      void
      gbump(int __n) { _M_in_cur += __n; }
# 504 "/usr/include/c++/4.2.1/streambuf" 3
      void
      setg(char_type* __gbeg, char_type* __gnext, char_type* __gend)
      {
 _M_in_beg = __gbeg;
 _M_in_cur = __gnext;
 _M_in_end = __gend;
      }
# 524 "/usr/include/c++/4.2.1/streambuf" 3
      char_type*
      pbase() const { return _M_out_beg; }

      char_type*
      pptr() const { return _M_out_cur; }

      char_type*
      epptr() const { return _M_out_end; }
# 540 "/usr/include/c++/4.2.1/streambuf" 3
      void
      pbump(int __n) { _M_out_cur += __n; }
# 550 "/usr/include/c++/4.2.1/streambuf" 3
      void
      setp(char_type* __pbeg, char_type* __pend)
      {
 _M_out_beg = _M_out_cur = __pbeg;
 _M_out_end = __pend;
      }
# 571 "/usr/include/c++/4.2.1/streambuf" 3
      virtual void
      imbue(const locale&)
      { }
# 586 "/usr/include/c++/4.2.1/streambuf" 3
      virtual basic_streambuf<char_type,_Traits>*
      setbuf(char_type*, streamsize)
      { return this; }
# 597 "/usr/include/c++/4.2.1/streambuf" 3
      virtual pos_type
      seekoff(off_type, ios_base::seekdir,
       ios_base::openmode = ios_base::in | ios_base::out)
      { return pos_type(off_type(-1)); }
# 609 "/usr/include/c++/4.2.1/streambuf" 3
      virtual pos_type
      seekpos(pos_type,
       ios_base::openmode = ios_base::in | ios_base::out)
      { return pos_type(off_type(-1)); }
# 622 "/usr/include/c++/4.2.1/streambuf" 3
      virtual int
      sync() { return 0; }
# 644 "/usr/include/c++/4.2.1/streambuf" 3
      virtual streamsize
      showmanyc() { return 0; }
# 660 "/usr/include/c++/4.2.1/streambuf" 3
      virtual streamsize
      xsgetn(char_type* __s, streamsize __n);
# 682 "/usr/include/c++/4.2.1/streambuf" 3
      virtual int_type
      underflow()
      { return traits_type::eof(); }
# 695 "/usr/include/c++/4.2.1/streambuf" 3
      virtual int_type
      uflow()
      {
 int_type __ret = traits_type::eof();
 const bool __testeof = traits_type::eq_int_type(this->underflow(),
       __ret);
 if (!__testeof)
   {
     __ret = traits_type::to_int_type(*this->gptr());
     this->gbump(1);
   }
 return __ret;
      }
# 719 "/usr/include/c++/4.2.1/streambuf" 3
      virtual int_type
      pbackfail(int_type = traits_type::eof())
      { return traits_type::eof(); }
# 737 "/usr/include/c++/4.2.1/streambuf" 3
      virtual streamsize
      xsputn(const char_type* __s, streamsize __n);
# 762 "/usr/include/c++/4.2.1/streambuf" 3
      virtual int_type
      overflow(int_type = traits_type::eof())
      { return traits_type::eof(); }
# 791 "/usr/include/c++/4.2.1/streambuf" 3
    private:


      basic_streambuf(const __streambuf_type& __sb)
      : _M_in_beg(__sb._M_in_beg), _M_in_cur(__sb._M_in_cur),
      _M_in_end(__sb._M_in_end), _M_out_beg(__sb._M_out_beg),
      _M_out_cur(__sb._M_out_cur), _M_out_end(__sb._M_out_cur),
      _M_buf_locale(__sb._M_buf_locale)
      { }

      __streambuf_type&
      operator=(const __streambuf_type&) { return *this; };
    };


  template<>
    streamsize
    __copy_streambufs_eof(basic_streambuf<char>* __sbin,
     basic_streambuf<char>* __sbout, bool& __ineof);

  template<>
    streamsize
    __copy_streambufs_eof(basic_streambuf<wchar_t>* __sbin,
     basic_streambuf<wchar_t>* __sbout, bool& __ineof);


}


# 1 "/usr/include/c++/4.2.1/bits/streambuf.tcc" 1 3
# 43 "/usr/include/c++/4.2.1/bits/streambuf.tcc" 3
       
# 44 "/usr/include/c++/4.2.1/bits/streambuf.tcc" 3

namespace std {

  template<typename _CharT, typename _Traits>
    streamsize
    basic_streambuf<_CharT, _Traits>::
    xsgetn(char_type* __s, streamsize __n)
    {
      streamsize __ret = 0;
      while (__ret < __n)
 {
   const streamsize __buf_len = this->egptr() - this->gptr();
   if (__buf_len)
     {
       const streamsize __remaining = __n - __ret;
       const streamsize __len = std::min(__buf_len, __remaining);
       traits_type::copy(__s, this->gptr(), __len);
       __ret += __len;
       __s += __len;
       this->gbump(__len);
     }

   if (__ret < __n)
     {
       const int_type __c = this->uflow();
       if (!traits_type::eq_int_type(__c, traits_type::eof()))
  {
    traits_type::assign(*__s++, traits_type::to_char_type(__c));
    ++__ret;
  }
       else
  break;
     }
 }
      return __ret;
    }

  template<typename _CharT, typename _Traits>
    streamsize
    basic_streambuf<_CharT, _Traits>::
    xsputn(const char_type* __s, streamsize __n)
    {
      streamsize __ret = 0;
      while (__ret < __n)
 {
   const streamsize __buf_len = this->epptr() - this->pptr();
   if (__buf_len)
     {
       const streamsize __remaining = __n - __ret;
       const streamsize __len = std::min(__buf_len, __remaining);
       traits_type::copy(this->pptr(), __s, __len);
       __ret += __len;
       __s += __len;
       this->pbump(__len);
     }

   if (__ret < __n)
     {
       int_type __c = this->overflow(traits_type::to_int_type(*__s));
       if (!traits_type::eq_int_type(__c, traits_type::eof()))
  {
    ++__ret;
    ++__s;
  }
       else
  break;
     }
 }
      return __ret;
    }




  template<typename _CharT, typename _Traits>
    streamsize
    __copy_streambufs_eof(basic_streambuf<_CharT, _Traits>* __sbin,
     basic_streambuf<_CharT, _Traits>* __sbout,
     bool& __ineof)
    {
      streamsize __ret = 0;
      __ineof = true;
      typename _Traits::int_type __c = __sbin->sgetc();
      while (!_Traits::eq_int_type(__c, _Traits::eof()))
 {
   __c = __sbout->sputc(_Traits::to_char_type(__c));
   if (_Traits::eq_int_type(__c, _Traits::eof()))
     {
       __ineof = false;
       break;
     }
   ++__ret;
   __c = __sbin->snextc();
 }
      return __ret;
    }

  template<typename _CharT, typename _Traits>
    inline streamsize
    __copy_streambufs(basic_streambuf<_CharT, _Traits>* __sbin,
        basic_streambuf<_CharT, _Traits>* __sbout)
    {
      bool __ineof;
      return __copy_streambufs_eof(__sbin, __sbout, __ineof);
    }





  extern template class basic_streambuf<char>;
  extern template
    streamsize
    __copy_streambufs(basic_streambuf<char>*,
        basic_streambuf<char>*);
  extern template
    streamsize
    __copy_streambufs_eof(basic_streambuf<char>*,
     basic_streambuf<char>*, bool&);


  extern template class basic_streambuf<wchar_t>;
  extern template
    streamsize
    __copy_streambufs(basic_streambuf<wchar_t>*,
        basic_streambuf<wchar_t>*);
  extern template
    streamsize
    __copy_streambufs_eof(basic_streambuf<wchar_t>*,
     basic_streambuf<wchar_t>*, bool&);



}
# 821 "/usr/include/c++/4.2.1/streambuf" 2 3
# 50 "/usr/include/c++/4.2.1/ios" 2 3
# 1 "/usr/include/c++/4.2.1/bits/basic_ios.h" 1 3
# 39 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
       
# 40 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3

# 1 "/usr/include/c++/4.2.1/bits/streambuf_iterator.h" 1 3
# 39 "/usr/include/c++/4.2.1/bits/streambuf_iterator.h" 3
       
# 40 "/usr/include/c++/4.2.1/bits/streambuf_iterator.h" 3




namespace std {



  template<typename _CharT, typename _Traits>
    class istreambuf_iterator
    : public iterator<input_iterator_tag, _CharT, typename _Traits::off_type,
        _CharT*, _CharT&>
    {
    public:



      typedef _CharT char_type;
      typedef _Traits traits_type;
      typedef typename _Traits::int_type int_type;
      typedef basic_streambuf<_CharT, _Traits> streambuf_type;
      typedef basic_istream<_CharT, _Traits> istream_type;


      template<typename _CharT2>
 friend typename __gnu_cxx::__enable_if<__is_char<_CharT2>::__value,
                      ostreambuf_iterator<_CharT2> >::__type
 copy(istreambuf_iterator<_CharT2>, istreambuf_iterator<_CharT2>,
      ostreambuf_iterator<_CharT2>);

      template<typename _CharT2>
 friend typename __gnu_cxx::__enable_if<__is_char<_CharT2>::__value,
            _CharT2*>::__type
 __copy_aux(istreambuf_iterator<_CharT2>, istreambuf_iterator<_CharT2>,
     _CharT2*);

      template<typename _CharT2>
 friend typename __gnu_cxx::__enable_if<__is_char<_CharT2>::__value,
               istreambuf_iterator<_CharT2> >::__type
 find(istreambuf_iterator<_CharT2>, istreambuf_iterator<_CharT2>,
      const _CharT2&);

    private:







      mutable streambuf_type* _M_sbuf;
      mutable int_type _M_c;

    public:

      istreambuf_iterator() throw()
      : _M_sbuf(0), _M_c(traits_type::eof()) { }


      istreambuf_iterator(istream_type& __s) throw()
      : _M_sbuf(__s.rdbuf()), _M_c(traits_type::eof()) { }


      istreambuf_iterator(streambuf_type* __s) throw()
      : _M_sbuf(__s), _M_c(traits_type::eof()) { }




      char_type
      operator*() const
      {







 return traits_type::to_char_type(_M_get());
      }


      istreambuf_iterator&
      operator++()
      {
 ;


 if (_M_sbuf)
   {
     _M_sbuf->sbumpc();
     _M_c = traits_type::eof();
   }
 return *this;
      }


      istreambuf_iterator
      operator++(int)
      {
 ;



 istreambuf_iterator __old = *this;
 if (_M_sbuf)
   {
     __old._M_c = _M_sbuf->sbumpc();
     _M_c = traits_type::eof();
   }
 return __old;
      }





      bool
      equal(const istreambuf_iterator& __b) const
      {
 const bool __thiseof = _M_at_eof();
 const bool __beof = __b._M_at_eof();
 return (__thiseof && __beof || (!__thiseof && !__beof));
      }

    private:
      int_type
      _M_get() const
      {
 const int_type __eof = traits_type::eof();
 int_type __ret = __eof;
 if (_M_sbuf)
   {
     if (!traits_type::eq_int_type(_M_c, __eof))
       __ret = _M_c;
     else if (!traits_type::eq_int_type((__ret = _M_sbuf->sgetc()),
            __eof))
       _M_c = __ret;
     else
       _M_sbuf = 0;
   }
 return __ret;
      }

      bool
      _M_at_eof() const
      {
 const int_type __eof = traits_type::eof();
 return traits_type::eq_int_type(_M_get(), __eof);
      }
    };

  template<typename _CharT, typename _Traits>
    inline bool
    operator==(const istreambuf_iterator<_CharT, _Traits>& __a,
        const istreambuf_iterator<_CharT, _Traits>& __b)
    { return __a.equal(__b); }

  template<typename _CharT, typename _Traits>
    inline bool
    operator!=(const istreambuf_iterator<_CharT, _Traits>& __a,
        const istreambuf_iterator<_CharT, _Traits>& __b)
    { return !__a.equal(__b); }


  template<typename _CharT, typename _Traits>
    class ostreambuf_iterator
    : public iterator<output_iterator_tag, void, void, void, void>
    {
    public:



      typedef _CharT char_type;
      typedef _Traits traits_type;
      typedef basic_streambuf<_CharT, _Traits> streambuf_type;
      typedef basic_ostream<_CharT, _Traits> ostream_type;


      template<typename _CharT2>
 friend typename __gnu_cxx::__enable_if<__is_char<_CharT2>::__value,
                      ostreambuf_iterator<_CharT2> >::__type
 copy(istreambuf_iterator<_CharT2>, istreambuf_iterator<_CharT2>,
      ostreambuf_iterator<_CharT2>);

    private:
      streambuf_type* _M_sbuf;
      bool _M_failed;

    public:

      ostreambuf_iterator(ostream_type& __s) throw ()
      : _M_sbuf(__s.rdbuf()), _M_failed(!_M_sbuf) { }


      ostreambuf_iterator(streambuf_type* __s) throw ()
      : _M_sbuf(__s), _M_failed(!_M_sbuf) { }


      ostreambuf_iterator&
      operator=(_CharT __c)
      {
 if (!_M_failed &&
     _Traits::eq_int_type(_M_sbuf->sputc(__c), _Traits::eof()))
   _M_failed = true;
 return *this;
      }


      ostreambuf_iterator&
      operator*()
      { return *this; }


      ostreambuf_iterator&
      operator++(int)
      { return *this; }


      ostreambuf_iterator&
      operator++()
      { return *this; }


      bool
      failed() const throw()
      { return _M_failed; }

      ostreambuf_iterator&
      _M_put(const _CharT* __ws, streamsize __len)
      {
 if (__builtin_expect(!_M_failed, true)
     && __builtin_expect(this->_M_sbuf->sputn(__ws, __len) != __len,
    false))
   _M_failed = true;
 return *this;
      }
    };


  template<typename _CharT>
    typename __gnu_cxx::__enable_if<__is_char<_CharT>::__value,
                           ostreambuf_iterator<_CharT> >::__type
    copy(istreambuf_iterator<_CharT> __first,
  istreambuf_iterator<_CharT> __last,
  ostreambuf_iterator<_CharT> __result)
    {
      if (__first._M_sbuf && !__last._M_sbuf && !__result._M_failed)
 {
   bool __ineof;
   __copy_streambufs_eof(__first._M_sbuf, __result._M_sbuf, __ineof);
   if (!__ineof)
     __result._M_failed = true;
 }
      return __result;
    }

  template<typename _CharT>
    typename __gnu_cxx::__enable_if<__is_char<_CharT>::__value,
            ostreambuf_iterator<_CharT> >::__type
    __copy_aux(_CharT* __first, _CharT* __last,
        ostreambuf_iterator<_CharT> __result)
    {
      const streamsize __num = __last - __first;
      if (__num > 0)
 __result._M_put(__first, __num);
      return __result;
    }

  template<typename _CharT>
    typename __gnu_cxx::__enable_if<__is_char<_CharT>::__value,
        ostreambuf_iterator<_CharT> >::__type
    __copy_aux(const _CharT* __first, const _CharT* __last,
        ostreambuf_iterator<_CharT> __result)
    {
      const streamsize __num = __last - __first;
      if (__num > 0)
 __result._M_put(__first, __num);
      return __result;
    }

  template<typename _CharT>
    typename __gnu_cxx::__enable_if<__is_char<_CharT>::__value,
            _CharT*>::__type
    __copy_aux(istreambuf_iterator<_CharT> __first,
        istreambuf_iterator<_CharT> __last, _CharT* __result)
    {
      typedef istreambuf_iterator<_CharT> __is_iterator_type;
      typedef typename __is_iterator_type::traits_type traits_type;
      typedef typename __is_iterator_type::streambuf_type streambuf_type;
      typedef typename traits_type::int_type int_type;

      if (__first._M_sbuf && !__last._M_sbuf)
 {
   streambuf_type* __sb = __first._M_sbuf;
   int_type __c = __sb->sgetc();
   while (!traits_type::eq_int_type(__c, traits_type::eof()))
     {
       const streamsize __n = __sb->egptr() - __sb->gptr();
       if (__n > 1)
  {
    traits_type::copy(__result, __sb->gptr(), __n);
    __sb->gbump(__n);
    __result += __n;
    __c = __sb->underflow();
  }
       else
  {
    *__result++ = traits_type::to_char_type(__c);
    __c = __sb->snextc();
  }
     }
 }
      return __result;
    }

  template<typename _CharT>
    typename __gnu_cxx::__enable_if<__is_char<_CharT>::__value,
          istreambuf_iterator<_CharT> >::__type
    find(istreambuf_iterator<_CharT> __first,
  istreambuf_iterator<_CharT> __last, const _CharT& __val)
    {
      typedef istreambuf_iterator<_CharT> __is_iterator_type;
      typedef typename __is_iterator_type::traits_type traits_type;
      typedef typename __is_iterator_type::streambuf_type streambuf_type;
      typedef typename traits_type::int_type int_type;

      if (__first._M_sbuf && !__last._M_sbuf)
 {
   const int_type __ival = traits_type::to_int_type(__val);
   streambuf_type* __sb = __first._M_sbuf;
   int_type __c = __sb->sgetc();
   while (!traits_type::eq_int_type(__c, traits_type::eof())
   && !traits_type::eq_int_type(__c, __ival))
     {
       streamsize __n = __sb->egptr() - __sb->gptr();
       if (__n > 1)
  {
    const _CharT* __p = traits_type::find(__sb->gptr(),
       __n, __val);
    if (__p)
      __n = __p - __sb->gptr();
    __sb->gbump(__n);
    __c = __sb->sgetc();
  }
       else
  __c = __sb->snextc();
     }

   if (!traits_type::eq_int_type(__c, traits_type::eof()))
     __first._M_c = __c;
   else
     __first._M_sbuf = 0;
 }
      return __first;
    }

}
# 42 "/usr/include/c++/4.2.1/bits/basic_ios.h" 2 3


# 1 "/usr/include/c++/4.2.1/bits/locale_facets.h" 1 3
# 43 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
       
# 44 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3


# 1 "/usr/include/c++/4.2.1/cwctype" 1 3
# 48 "/usr/include/c++/4.2.1/cwctype" 3
       
# 49 "/usr/include/c++/4.2.1/cwctype" 3




# 1 "/usr/include/wctype.h" 1 3 4
# 39 "/usr/include/wctype.h" 3 4
typedef __darwin_wctrans_t wctrans_t;
# 62 "/usr/include/wctype.h" 3 4
static inline int
iswblank(wint_t _wc)
{
 return (__istype(_wc, 0x00020000L));
}


static inline int
iswascii(wint_t _wc)
{
 return ((_wc & ~0x7F) == 0);
}

static inline int
iswhexnumber(wint_t _wc)
{
 return (__istype(_wc, 0x00010000L));
}

static inline int
iswideogram(wint_t _wc)
{
 return (__istype(_wc, 0x00080000L));
}

static inline int
iswnumber(wint_t _wc)
{
 return (__istype(_wc, 0x00000400L));
}

static inline int
iswphonogram(wint_t _wc)
{
 return (__istype(_wc, 0x00200000L));
}

static inline int
iswrune(wint_t _wc)
{
 return (__istype(_wc, 0xFFFFFFF0L));
}

static inline int
iswspecial(wint_t _wc)
{
 return (__istype(_wc, 0x00100000L));
}
# 130 "/usr/include/wctype.h" 3 4
extern "C" {

wint_t nextwctype(wint_t, wctype_t);

wint_t towctrans(wint_t, wctrans_t);
wctrans_t
 wctrans(const char *);
}
# 54 "/usr/include/c++/4.2.1/cwctype" 2 3
# 80 "/usr/include/c++/4.2.1/cwctype" 3
namespace std {

  using ::wint_t;

  using ::wctype_t;
  using ::wctrans_t;

  using ::iswalnum;
  using ::iswalpha;

  using ::iswblank;

  using ::iswcntrl;
  using ::iswctype;
  using ::iswdigit;
  using ::iswgraph;
  using ::iswlower;
  using ::iswprint;
  using ::iswpunct;
  using ::iswspace;
  using ::iswupper;
  using ::iswxdigit;
  using ::towctrans;
  using ::towlower;
  using ::towupper;
  using ::wctrans;
  using ::wctype;

}
# 47 "/usr/include/c++/4.2.1/bits/locale_facets.h" 2 3
# 1 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/ctype_base.h" 1 3
# 38 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/ctype_base.h" 3
namespace std {


  struct ctype_base
  {

    typedef const int* __to_type;

    typedef unsigned long mask;


    static const mask upper = 0x00008000L;
    static const mask lower = 0x00001000L;
    static const mask alpha = 0x00000100L;
    static const mask digit = 0x00000400L;
    static const mask xdigit = 0x00010000L;
    static const mask space = 0x00004000L;
    static const mask print = 0x00040000L;
    static const mask graph = 0x00000100L | 0x00000400L | 0x00002000L;
    static const mask cntrl = 0x00000200L;
    static const mask punct = 0x00002000L;
    static const mask alnum = 0x00000100L | 0x00000400L;
# 74 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/ctype_base.h" 3
  };

}
# 48 "/usr/include/c++/4.2.1/bits/locale_facets.h" 2 3





namespace std {
# 65 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  template<typename _Tv>
    void
    __convert_to_v(const char* __in, _Tv& __out, ios_base::iostate& __err,
     const __c_locale& __cloc);


  template<>
    void
    __convert_to_v(const char*, float&, ios_base::iostate&,
     const __c_locale&);

  template<>
    void
    __convert_to_v(const char*, double&, ios_base::iostate&,
     const __c_locale&);

  template<>
    void
    __convert_to_v(const char*, long double&, ios_base::iostate&,
     const __c_locale&);



  template<typename _CharT, typename _Traits>
    struct __pad
    {
      static void
      _S_pad(ios_base& __io, _CharT __fill, _CharT* __news,
      const _CharT* __olds, const streamsize __newlen,
      const streamsize __oldlen, const bool __num);
    };






  template<typename _CharT>
    _CharT*
    __add_grouping(_CharT* __s, _CharT __sep,
     const char* __gbeg, size_t __gsize,
     const _CharT* __first, const _CharT* __last);




  template<typename _CharT>
    inline
    ostreambuf_iterator<_CharT>
    __write(ostreambuf_iterator<_CharT> __s, const _CharT* __ws, int __len)
    {
      __s._M_put(__ws, __len);
      return __s;
    }


  template<typename _CharT, typename _OutIter>
    inline
    _OutIter
    __write(_OutIter __s, const _CharT* __ws, int __len)
    {
      for (int __j = 0; __j < __len; __j++, ++__s)
 *__s = __ws[__j];
      return __s;
    }
# 145 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  template<typename _CharT>
    class __ctype_abstract_base : public locale::facet, public ctype_base
    {
    public:


      typedef _CharT char_type;
# 163 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      bool
      is(mask __m, char_type __c) const
      { return this->do_is(__m, __c); }
# 180 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      const char_type*
      is(const char_type *__lo, const char_type *__hi, mask *__vec) const
      { return this->do_is(__lo, __hi, __vec); }
# 196 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      const char_type*
      scan_is(mask __m, const char_type* __lo, const char_type* __hi) const
      { return this->do_scan_is(__m, __lo, __hi); }
# 212 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      const char_type*
      scan_not(mask __m, const char_type* __lo, const char_type* __hi) const
      { return this->do_scan_not(__m, __lo, __hi); }
# 226 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      char_type
      toupper(char_type __c) const
      { return this->do_toupper(__c); }
# 241 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      const char_type*
      toupper(char_type *__lo, const char_type* __hi) const
      { return this->do_toupper(__lo, __hi); }
# 255 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      char_type
      tolower(char_type __c) const
      { return this->do_tolower(__c); }
# 270 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      const char_type*
      tolower(char_type* __lo, const char_type* __hi) const
      { return this->do_tolower(__lo, __hi); }
# 287 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      char_type
      widen(char __c) const
      { return this->do_widen(__c); }
# 306 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      const char*
      widen(const char* __lo, const char* __hi, char_type* __to) const
      { return this->do_widen(__lo, __hi, __to); }
# 325 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      char
      narrow(char_type __c, char __dfault) const
      { return this->do_narrow(__c, __dfault); }
# 347 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      const char_type*
      narrow(const char_type* __lo, const char_type* __hi,
       char __dfault, char *__to) const
      { return this->do_narrow(__lo, __hi, __dfault, __to); }

    protected:
      explicit
      __ctype_abstract_base(size_t __refs = 0): facet(__refs) { }

      virtual
      ~__ctype_abstract_base() { }
# 372 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual bool
      do_is(mask __m, char_type __c) const = 0;
# 391 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_is(const char_type* __lo, const char_type* __hi,
     mask* __vec) const = 0;
# 410 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_scan_is(mask __m, const char_type* __lo,
   const char_type* __hi) const = 0;
# 429 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_scan_not(mask __m, const char_type* __lo,
    const char_type* __hi) const = 0;
# 447 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char_type
      do_toupper(char_type) const = 0;
# 464 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_toupper(char_type* __lo, const char_type* __hi) const = 0;
# 480 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char_type
      do_tolower(char_type) const = 0;
# 497 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_tolower(char_type* __lo, const char_type* __hi) const = 0;
# 516 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char_type
      do_widen(char) const = 0;
# 537 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char*
      do_widen(const char* __lo, const char* __hi,
        char_type* __dest) const = 0;
# 559 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char
      do_narrow(char_type, char __dfault) const = 0;
# 583 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_narrow(const char_type* __lo, const char_type* __hi,
  char __dfault, char* __dest) const = 0;
    };
# 606 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  template<typename _CharT>
    class ctype : public __ctype_abstract_base<_CharT>
    {
    public:

      typedef _CharT char_type;
      typedef typename __ctype_abstract_base<_CharT>::mask mask;


      static locale::id id;

      explicit
      ctype(size_t __refs = 0) : __ctype_abstract_base<_CharT>(__refs) { }

   protected:
      virtual
      ~ctype();

      virtual bool
      do_is(mask __m, char_type __c) const;

      virtual const char_type*
      do_is(const char_type* __lo, const char_type* __hi, mask* __vec) const;

      virtual const char_type*
      do_scan_is(mask __m, const char_type* __lo, const char_type* __hi) const;

      virtual const char_type*
      do_scan_not(mask __m, const char_type* __lo,
    const char_type* __hi) const;

      virtual char_type
      do_toupper(char_type __c) const;

      virtual const char_type*
      do_toupper(char_type* __lo, const char_type* __hi) const;

      virtual char_type
      do_tolower(char_type __c) const;

      virtual const char_type*
      do_tolower(char_type* __lo, const char_type* __hi) const;

      virtual char_type
      do_widen(char __c) const;

      virtual const char*
      do_widen(const char* __lo, const char* __hi, char_type* __dest) const;

      virtual char
      do_narrow(char_type, char __dfault) const;

      virtual const char_type*
      do_narrow(const char_type* __lo, const char_type* __hi,
  char __dfault, char* __dest) const;
    };

  template<typename _CharT>
    locale::id ctype<_CharT>::id;
# 675 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  template<>
    class ctype<char> : public locale::facet, public ctype_base
    {
    public:


      typedef char char_type;

    protected:

      __c_locale _M_c_locale_ctype;
      bool _M_del;
      __to_type _M_toupper;
      __to_type _M_tolower;
      const mask* _M_table;
      mutable char _M_widen_ok;
      mutable char _M_widen[1 + static_cast<unsigned char>(-1)];
      mutable char _M_narrow[1 + static_cast<unsigned char>(-1)];
      mutable char _M_narrow_ok;


    public:

      static locale::id id;

      static const size_t table_size = 1 + static_cast<unsigned char>(-1);
# 712 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      ctype(const mask* __table = 0, bool __del = false, size_t __refs = 0);
# 725 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      ctype(__c_locale __cloc, const mask* __table = 0, bool __del = false,
     size_t __refs = 0);
# 738 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      inline bool
      is(mask __m, char __c) const;
# 753 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      inline const char*
      is(const char* __lo, const char* __hi, mask* __vec) const;
# 767 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      inline const char*
      scan_is(mask __m, const char* __lo, const char* __hi) const;
# 781 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      inline const char*
      scan_not(mask __m, const char* __lo, const char* __hi) const;
# 796 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      char_type
      toupper(char_type __c) const
      { return this->do_toupper(__c); }
# 813 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      const char_type*
      toupper(char_type *__lo, const char_type* __hi) const
      { return this->do_toupper(__lo, __hi); }
# 829 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      char_type
      tolower(char_type __c) const
      { return this->do_tolower(__c); }
# 846 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      const char_type*
      tolower(char_type* __lo, const char_type* __hi) const
      { return this->do_tolower(__lo, __hi); }
# 866 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      char_type
      widen(char __c) const
      {
 if (_M_widen_ok)
   return _M_widen[static_cast<unsigned char>(__c)];
 this->_M_widen_init();
 return this->do_widen(__c);
      }
# 893 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      const char*
      widen(const char* __lo, const char* __hi, char_type* __to) const
      {
 if (_M_widen_ok == 1)
   {
     memcpy(__to, __lo, __hi - __lo);
     return __hi;
   }
 if (!_M_widen_ok)
   _M_widen_init();
 return this->do_widen(__lo, __hi, __to);
      }
# 924 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      char
      narrow(char_type __c, char __dfault) const
      {
 if (_M_narrow[static_cast<unsigned char>(__c)])
   return _M_narrow[static_cast<unsigned char>(__c)];
 const char __t = do_narrow(__c, __dfault);
 if (__t != __dfault)
   _M_narrow[static_cast<unsigned char>(__c)] = __t;
 return __t;
      }
# 957 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      const char_type*
      narrow(const char_type* __lo, const char_type* __hi,
      char __dfault, char *__to) const
      {
 if (__builtin_expect(_M_narrow_ok == 1, true))
   {
     memcpy(__to, __lo, __hi - __lo);
     return __hi;
   }
 if (!_M_narrow_ok)
   _M_narrow_init();
 return this->do_narrow(__lo, __hi, __dfault, __to);
      }

    protected:


      const mask*
      table() const throw()
      { return _M_table; }


      static const mask*
      classic_table() throw();







      virtual
      ~ctype();
# 1004 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char_type
      do_toupper(char_type) const;
# 1021 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_toupper(char_type* __lo, const char_type* __hi) const;
# 1037 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char_type
      do_tolower(char_type) const;
# 1054 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_tolower(char_type* __lo, const char_type* __hi) const;
# 1074 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char_type
      do_widen(char __c) const
      { return __c; }
# 1097 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char*
      do_widen(const char* __lo, const char* __hi, char_type* __dest) const
      {
 memcpy(__dest, __lo, __hi - __lo);
 return __hi;
      }
# 1123 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char
      do_narrow(char_type __c, char) const
      { return __c; }
# 1149 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_narrow(const char_type* __lo, const char_type* __hi,
  char, char* __dest) const
      {
 memcpy(__dest, __lo, __hi - __lo);
 return __hi;
      }

    private:

      void _M_widen_init() const
      {
 char __tmp[sizeof(_M_widen)];
 for (size_t __i = 0; __i < sizeof(_M_widen); ++__i)
   __tmp[__i] = __i;
 do_widen(__tmp, __tmp + sizeof(__tmp), _M_widen);

 _M_widen_ok = 1;

 if (memcmp(__tmp, _M_widen, sizeof(_M_widen)))
   _M_widen_ok = 2;
      }




      void _M_narrow_init() const
      {
 char __tmp[sizeof(_M_narrow)];
 for (size_t __i = 0; __i < sizeof(_M_narrow); ++__i)
   __tmp[__i] = __i;
 do_narrow(__tmp, __tmp + sizeof(__tmp), 0, _M_narrow);

 _M_narrow_ok = 1;
 if (memcmp(__tmp, _M_narrow, sizeof(_M_narrow)))
   _M_narrow_ok = 2;
 else
   {


     char __c;
     do_narrow(__tmp, __tmp + 1, 1, &__c);
     if (__c == 1)
       _M_narrow_ok = 2;
   }
      }
    };

  template<>
    const ctype<char>&
    use_facet<ctype<char> >(const locale& __loc);
# 1213 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  template<>
    class ctype<wchar_t> : public __ctype_abstract_base<wchar_t>
    {
    public:


      typedef wchar_t char_type;
      typedef wctype_t __wmask_type;

    protected:
      __c_locale _M_c_locale_ctype;


      bool _M_narrow_ok;
      char _M_narrow[128];
      wint_t _M_widen[1 + static_cast<unsigned char>(-1)];


      mask _M_bit[16];
      __wmask_type _M_wmask[16];

    public:


      static locale::id id;
# 1246 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      ctype(size_t __refs = 0);
# 1257 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      ctype(__c_locale __cloc, size_t __refs = 0);

    protected:
      __wmask_type
      _M_convert_to_wmask(const mask __m) const;


      virtual
      ~ctype();
# 1281 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual bool
      do_is(mask __m, char_type __c) const;
# 1300 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_is(const char_type* __lo, const char_type* __hi, mask* __vec) const;
# 1318 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_scan_is(mask __m, const char_type* __lo, const char_type* __hi) const;
# 1336 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_scan_not(mask __m, const char_type* __lo,
    const char_type* __hi) const;
# 1353 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char_type
      do_toupper(char_type) const;
# 1370 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_toupper(char_type* __lo, const char_type* __hi) const;
# 1386 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char_type
      do_tolower(char_type) const;
# 1403 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_tolower(char_type* __lo, const char_type* __hi) const;
# 1423 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char_type
      do_widen(char) const;
# 1445 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char*
      do_widen(const char* __lo, const char* __hi, char_type* __dest) const;
# 1468 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char
      do_narrow(char_type, char __dfault) const;
# 1494 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual const char_type*
      do_narrow(const char_type* __lo, const char_type* __hi,
  char __dfault, char* __dest) const;


      void
      _M_initialize_ctype();
    };

  template<>
    const ctype<wchar_t>&
    use_facet<ctype<wchar_t> >(const locale& __loc);



  template<typename _CharT>
    class ctype_byname : public ctype<_CharT>
    {
    public:
      typedef _CharT char_type;

      explicit
      ctype_byname(const char* __s, size_t __refs = 0);

    protected:
      virtual
      ~ctype_byname() { };
    };


  template<>
    ctype_byname<char>::ctype_byname(const char*, size_t refs);

  template<>
    ctype_byname<wchar_t>::ctype_byname(const char*, size_t refs);

}


# 1 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/ctype_inline.h" 1 3
# 42 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/ctype_inline.h" 3
namespace std {

  bool
  ctype<char>::
  is(mask __m, char __c) const
  {
    if (_M_table)
      return _M_table[static_cast<unsigned char>(__c)] & __m;
    else
      return __istype(__c, __m);
  }

  const char*
  ctype<char>::
  is(const char* __low, const char* __high, mask* __vec) const
  {
    if (_M_table)
      while (__low < __high)
 *__vec++ = _M_table[static_cast<unsigned char>(*__low++)];
    else
      for (;__low < __high; ++__vec, ++__low)
 {

   *__vec = __maskrune (*__low, upper | lower | alpha | digit | xdigit
          | space | print | graph | cntrl | punct | alnum);
# 83 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/ctype_inline.h" 3
 }
    return __high;
  }

  const char*
  ctype<char>::
  scan_is(mask __m, const char* __low, const char* __high) const
  {
    if (_M_table)
      while (__low < __high
      && !(_M_table[static_cast<unsigned char>(*__low)] & __m))
 ++__low;
    else
      while (__low < __high && !this->is(__m, *__low))
 ++__low;
    return __low;
  }

  const char*
  ctype<char>::
  scan_not(mask __m, const char* __low, const char* __high) const
  {
    if (_M_table)
      while (__low < __high
      && (_M_table[static_cast<unsigned char>(*__low)] & __m) != 0)
 ++__low;
    else
      while (__low < __high && this->is(__m, *__low) != 0)
 ++__low;
    return __low;
  }


  inline bool
  ctype<wchar_t>::
  do_is(mask __m, wchar_t __c) const
  {
    return __istype (__c, __m);
  }

  inline const wchar_t*
  ctype<wchar_t>::
  do_is(const wchar_t* __lo, const wchar_t* __hi, mask* __vec) const
  {
    for (; __lo < __hi; ++__vec, ++__lo)
      *__vec = __maskrune (*__lo, upper | lower | alpha | digit | xdigit
      | space | print | graph | cntrl | punct | alnum);
    return __hi;
  }

  inline const wchar_t*
  ctype<wchar_t>::
  do_scan_is(mask __m, const wchar_t* __lo, const wchar_t* __hi) const
  {
    while (__lo < __hi && ! __istype (*__lo, __m))
      ++__lo;
    return __lo;
  }

  inline const wchar_t*
  ctype<wchar_t>::
  do_scan_not(mask __m, const char_type* __lo, const char_type* __hi) const
  {
    while (__lo < __hi && __istype (*__lo, __m))
      ++__lo;
    return __lo;
  }


}
# 1534 "/usr/include/c++/4.2.1/bits/locale_facets.h" 2 3


# 1 "/usr/include/c++/4.2.1/bits/codecvt.h" 1 3
# 45 "/usr/include/c++/4.2.1/bits/codecvt.h" 3
       
# 46 "/usr/include/c++/4.2.1/bits/codecvt.h" 3

namespace std {


  class codecvt_base
  {
  public:
    enum result
    {
      ok,
      partial,
      error,
      noconv
    };
  };
# 71 "/usr/include/c++/4.2.1/bits/codecvt.h" 3
  template<typename _InternT, typename _ExternT, typename _StateT>
    class __codecvt_abstract_base
    : public locale::facet, public codecvt_base
    {
    public:

      typedef codecvt_base::result result;
      typedef _InternT intern_type;
      typedef _ExternT extern_type;
      typedef _StateT state_type;
# 119 "/usr/include/c++/4.2.1/bits/codecvt.h" 3
      result
      out(state_type& __state, const intern_type* __from,
   const intern_type* __from_end, const intern_type*& __from_next,
   extern_type* __to, extern_type* __to_end,
   extern_type*& __to_next) const
      {
 return this->do_out(__state, __from, __from_end, __from_next,
       __to, __to_end, __to_next);
      }
# 158 "/usr/include/c++/4.2.1/bits/codecvt.h" 3
      result
      unshift(state_type& __state, extern_type* __to, extern_type* __to_end,
       extern_type*& __to_next) const
      { return this->do_unshift(__state, __to,__to_end,__to_next); }
# 199 "/usr/include/c++/4.2.1/bits/codecvt.h" 3
      result
      in(state_type& __state, const extern_type* __from,
  const extern_type* __from_end, const extern_type*& __from_next,
  intern_type* __to, intern_type* __to_end,
  intern_type*& __to_next) const
      {
 return this->do_in(__state, __from, __from_end, __from_next,
      __to, __to_end, __to_next);
      }

      int
      encoding() const throw()
      { return this->do_encoding(); }

      bool
      always_noconv() const throw()
      { return this->do_always_noconv(); }

      int
      length(state_type& __state, const extern_type* __from,
      const extern_type* __end, size_t __max) const
      { return this->do_length(__state, __from, __end, __max); }

      int
      max_length() const throw()
      { return this->do_max_length(); }

    protected:
      explicit
      __codecvt_abstract_base(size_t __refs = 0) : locale::facet(__refs) { }

      virtual
      ~__codecvt_abstract_base() { }
# 240 "/usr/include/c++/4.2.1/bits/codecvt.h" 3
      virtual result
      do_out(state_type& __state, const intern_type* __from,
      const intern_type* __from_end, const intern_type*& __from_next,
      extern_type* __to, extern_type* __to_end,
      extern_type*& __to_next) const = 0;

      virtual result
      do_unshift(state_type& __state, extern_type* __to,
   extern_type* __to_end, extern_type*& __to_next) const = 0;

      virtual result
      do_in(state_type& __state, const extern_type* __from,
     const extern_type* __from_end, const extern_type*& __from_next,
     intern_type* __to, intern_type* __to_end,
     intern_type*& __to_next) const = 0;

      virtual int
      do_encoding() const throw() = 0;

      virtual bool
      do_always_noconv() const throw() = 0;

      virtual int
      do_length(state_type&, const extern_type* __from,
  const extern_type* __end, size_t __max) const = 0;

      virtual int
      do_max_length() const throw() = 0;
    };



  template<typename _InternT, typename _ExternT, typename _StateT>
    class codecvt
    : public __codecvt_abstract_base<_InternT, _ExternT, _StateT>
    {
    public:

      typedef codecvt_base::result result;
      typedef _InternT intern_type;
      typedef _ExternT extern_type;
      typedef _StateT state_type;

    protected:
      __c_locale _M_c_locale_codecvt;

    public:
      static locale::id id;

      explicit
      codecvt(size_t __refs = 0)
      : __codecvt_abstract_base<_InternT, _ExternT, _StateT> (__refs) { }

      explicit
      codecvt(__c_locale __cloc, size_t __refs = 0);

    protected:
      virtual
      ~codecvt() { }

      virtual result
      do_out(state_type& __state, const intern_type* __from,
      const intern_type* __from_end, const intern_type*& __from_next,
      extern_type* __to, extern_type* __to_end,
      extern_type*& __to_next) const;

      virtual result
      do_unshift(state_type& __state, extern_type* __to,
   extern_type* __to_end, extern_type*& __to_next) const;

      virtual result
      do_in(state_type& __state, const extern_type* __from,
     const extern_type* __from_end, const extern_type*& __from_next,
     intern_type* __to, intern_type* __to_end,
     intern_type*& __to_next) const;

      virtual int
      do_encoding() const throw();

      virtual bool
      do_always_noconv() const throw();

      virtual int
      do_length(state_type&, const extern_type* __from,
  const extern_type* __end, size_t __max) const;

      virtual int
      do_max_length() const throw();
    };

  template<typename _InternT, typename _ExternT, typename _StateT>
    locale::id codecvt<_InternT, _ExternT, _StateT>::id;


  template<>
    class codecvt<char, char, mbstate_t>
    : public __codecvt_abstract_base<char, char, mbstate_t>
    {
    public:

      typedef char intern_type;
      typedef char extern_type;
      typedef mbstate_t state_type;

    protected:
      __c_locale _M_c_locale_codecvt;

    public:
      static locale::id id;

      explicit
      codecvt(size_t __refs = 0);

      explicit
      codecvt(__c_locale __cloc, size_t __refs = 0);

    protected:
      virtual
      ~codecvt();

      virtual result
      do_out(state_type& __state, const intern_type* __from,
      const intern_type* __from_end, const intern_type*& __from_next,
      extern_type* __to, extern_type* __to_end,
      extern_type*& __to_next) const;

      virtual result
      do_unshift(state_type& __state, extern_type* __to,
   extern_type* __to_end, extern_type*& __to_next) const;

      virtual result
      do_in(state_type& __state, const extern_type* __from,
     const extern_type* __from_end, const extern_type*& __from_next,
     intern_type* __to, intern_type* __to_end,
     intern_type*& __to_next) const;

      virtual int
      do_encoding() const throw();

      virtual bool
      do_always_noconv() const throw();

      virtual int
      do_length(state_type&, const extern_type* __from,
  const extern_type* __end, size_t __max) const;

      virtual int
      do_max_length() const throw();
  };



  template<>
    class codecvt<wchar_t, char, mbstate_t>
    : public __codecvt_abstract_base<wchar_t, char, mbstate_t>
    {
    public:

      typedef wchar_t intern_type;
      typedef char extern_type;
      typedef mbstate_t state_type;

    protected:
      __c_locale _M_c_locale_codecvt;

    public:
      static locale::id id;

      explicit
      codecvt(size_t __refs = 0);

      explicit
      codecvt(__c_locale __cloc, size_t __refs = 0);

    protected:
      virtual
      ~codecvt();

      virtual result
      do_out(state_type& __state, const intern_type* __from,
      const intern_type* __from_end, const intern_type*& __from_next,
      extern_type* __to, extern_type* __to_end,
      extern_type*& __to_next) const;

      virtual result
      do_unshift(state_type& __state,
   extern_type* __to, extern_type* __to_end,
   extern_type*& __to_next) const;

      virtual result
      do_in(state_type& __state,
      const extern_type* __from, const extern_type* __from_end,
      const extern_type*& __from_next,
      intern_type* __to, intern_type* __to_end,
      intern_type*& __to_next) const;

      virtual
      int do_encoding() const throw();

      virtual
      bool do_always_noconv() const throw();

      virtual
      int do_length(state_type&, const extern_type* __from,
      const extern_type* __end, size_t __max) const;

      virtual int
      do_max_length() const throw();
    };



  template<typename _InternT, typename _ExternT, typename _StateT>
    class codecvt_byname : public codecvt<_InternT, _ExternT, _StateT>
    {
    public:
      explicit
      codecvt_byname(const char* __s, size_t __refs = 0)
      : codecvt<_InternT, _ExternT, _StateT>(__refs)
      {
 if (std::strcmp(__s, "C") != 0 && std::strcmp(__s, "POSIX") != 0)
   {
     this->_S_destroy_c_locale(this->_M_c_locale_codecvt);
     this->_S_create_c_locale(this->_M_c_locale_codecvt, __s);
   }
      }

    protected:
      virtual
      ~codecvt_byname() { }
    };

}
# 1537 "/usr/include/c++/4.2.1/bits/locale_facets.h" 2 3

namespace std {


  class __num_base
  {
  public:


    enum
      {
        _S_ominus,
        _S_oplus,
        _S_ox,
        _S_oX,
        _S_odigits,
        _S_odigits_end = _S_odigits + 16,
        _S_oudigits = _S_odigits_end,
        _S_oudigits_end = _S_oudigits + 16,
        _S_oe = _S_odigits + 14,
        _S_oE = _S_oudigits + 14,
 _S_oend = _S_oudigits_end
      };






    static const char* _S_atoms_out;



    static const char* _S_atoms_in;

    enum
    {
      _S_iminus,
      _S_iplus,
      _S_ix,
      _S_iX,
      _S_izero,
      _S_ie = _S_izero + 14,
      _S_iE = _S_izero + 20,
      _S_iend = 26
    };



    static void
    _S_format_float(const ios_base& __io, char* __fptr, char __mod);
  };

  template<typename _CharT>
    struct __numpunct_cache : public locale::facet
    {
      const char* _M_grouping;
      size_t _M_grouping_size;
      bool _M_use_grouping;
      const _CharT* _M_truename;
      size_t _M_truename_size;
      const _CharT* _M_falsename;
      size_t _M_falsename_size;
      _CharT _M_decimal_point;
      _CharT _M_thousands_sep;





      _CharT _M_atoms_out[__num_base::_S_oend];





      _CharT _M_atoms_in[__num_base::_S_iend];

      bool _M_allocated;

      __numpunct_cache(size_t __refs = 0) : facet(__refs),
      _M_grouping(__null), _M_grouping_size(0), _M_use_grouping(false),
      _M_truename(__null), _M_truename_size(0), _M_falsename(__null),
      _M_falsename_size(0), _M_decimal_point(_CharT()),
      _M_thousands_sep(_CharT()), _M_allocated(false)
      { }

      ~__numpunct_cache();

      void
      _M_cache(const locale& __loc);

    private:
      __numpunct_cache&
      operator=(const __numpunct_cache&);

      explicit
      __numpunct_cache(const __numpunct_cache&);
    };

  template<typename _CharT>
    __numpunct_cache<_CharT>::~__numpunct_cache()
    {
      if (_M_allocated)
 {
   delete [] _M_grouping;
   delete [] _M_truename;
   delete [] _M_falsename;
 }
    }
# 1661 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  template<typename _CharT>
    class numpunct : public locale::facet
    {
    public:



      typedef _CharT char_type;
      typedef basic_string<_CharT> string_type;

      typedef __numpunct_cache<_CharT> __cache_type;

    protected:
      __cache_type* _M_data;

    public:

      static locale::id id;






      explicit
      numpunct(size_t __refs = 0) : facet(__refs), _M_data(__null)
      { _M_initialize_numpunct(); }
# 1698 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      numpunct(__cache_type* __cache, size_t __refs = 0)
      : facet(__refs), _M_data(__cache)
      { _M_initialize_numpunct(); }
# 1712 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      numpunct(__c_locale __cloc, size_t __refs = 0)
      : facet(__refs), _M_data(__null)
      { _M_initialize_numpunct(__cloc); }
# 1726 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      char_type
      decimal_point() const
      { return this->do_decimal_point(); }
# 1739 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      char_type
      thousands_sep() const
      { return this->do_thousands_sep(); }
# 1770 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      string
      grouping() const
      { return this->do_grouping(); }
# 1783 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      string_type
      truename() const
      { return this->do_truename(); }
# 1796 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      string_type
      falsename() const
      { return this->do_falsename(); }

    protected:

      virtual
      ~numpunct();
# 1813 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char_type
      do_decimal_point() const
      { return _M_data->_M_decimal_point; }
# 1825 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char_type
      do_thousands_sep() const
      { return _M_data->_M_thousands_sep; }
# 1838 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual string
      do_grouping() const
      { return _M_data->_M_grouping; }
# 1851 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual string_type
      do_truename() const
      { return _M_data->_M_truename; }
# 1864 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual string_type
      do_falsename() const
      { return _M_data->_M_falsename; }


      void
      _M_initialize_numpunct(__c_locale __cloc = __null);
    };

  template<typename _CharT>
    locale::id numpunct<_CharT>::id;

  template<>
    numpunct<char>::~numpunct();

  template<>
    void
    numpunct<char>::_M_initialize_numpunct(__c_locale __cloc);


  template<>
    numpunct<wchar_t>::~numpunct();

  template<>
    void
    numpunct<wchar_t>::_M_initialize_numpunct(__c_locale __cloc);



  template<typename _CharT>
    class numpunct_byname : public numpunct<_CharT>
    {
    public:
      typedef _CharT char_type;
      typedef basic_string<_CharT> string_type;

      explicit
      numpunct_byname(const char* __s, size_t __refs = 0)
      : numpunct<_CharT>(__refs)
      {
 if (std::strcmp(__s, "C") != 0 && std::strcmp(__s, "POSIX") != 0)
   {
     __c_locale __tmp;
     this->_S_create_c_locale(__tmp, __s);
     this->_M_initialize_numpunct(__tmp);
     this->_S_destroy_c_locale(__tmp);
   }
      }

    protected:
      virtual
      ~numpunct_byname() { }
    };


# 1931 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  template<typename _CharT, typename _InIter>
    class num_get : public locale::facet
    {
    public:



      typedef _CharT char_type;
      typedef _InIter iter_type;



      static locale::id id;
# 1952 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      num_get(size_t __refs = 0) : facet(__refs) { }
# 1978 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      get(iter_type __in, iter_type __end, ios_base& __io,
   ios_base::iostate& __err, bool& __v) const
      { return this->do_get(__in, __end, __io, __err, __v); }
# 2014 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      get(iter_type __in, iter_type __end, ios_base& __io,
   ios_base::iostate& __err, long& __v) const
      { return this->do_get(__in, __end, __io, __err, __v); }

      iter_type
      get(iter_type __in, iter_type __end, ios_base& __io,
   ios_base::iostate& __err, unsigned short& __v) const
      { return this->do_get(__in, __end, __io, __err, __v); }

      iter_type
      get(iter_type __in, iter_type __end, ios_base& __io,
   ios_base::iostate& __err, unsigned int& __v) const
      { return this->do_get(__in, __end, __io, __err, __v); }

      iter_type
      get(iter_type __in, iter_type __end, ios_base& __io,
   ios_base::iostate& __err, unsigned long& __v) const
      { return this->do_get(__in, __end, __io, __err, __v); }


      iter_type
      get(iter_type __in, iter_type __end, ios_base& __io,
   ios_base::iostate& __err, long long& __v) const
      { return this->do_get(__in, __end, __io, __err, __v); }

      iter_type
      get(iter_type __in, iter_type __end, ios_base& __io,
   ios_base::iostate& __err, unsigned long long& __v) const
      { return this->do_get(__in, __end, __io, __err, __v); }
# 2073 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      get(iter_type __in, iter_type __end, ios_base& __io,
   ios_base::iostate& __err, float& __v) const
      { return this->do_get(__in, __end, __io, __err, __v); }

      iter_type
      get(iter_type __in, iter_type __end, ios_base& __io,
   ios_base::iostate& __err, double& __v) const
      { return this->do_get(__in, __end, __io, __err, __v); }

      iter_type
      get(iter_type __in, iter_type __end, ios_base& __io,
   ios_base::iostate& __err, long double& __v) const
      { return this->do_get(__in, __end, __io, __err, __v); }
# 2115 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      get(iter_type __in, iter_type __end, ios_base& __io,
   ios_base::iostate& __err, void*& __v) const
      { return this->do_get(__in, __end, __io, __err, __v); }

    protected:

      virtual ~num_get() { }

      iter_type
      _M_extract_float(iter_type, iter_type, ios_base&, ios_base::iostate&,
         string& __xtrc) const;

      template<typename _ValueT>
        iter_type
        _M_extract_int(iter_type, iter_type, ios_base&, ios_base::iostate&,
         _ValueT& __v) const;

      template<typename _CharT2>
      typename __gnu_cxx::__enable_if<__is_char<_CharT2>::__value, int>::__type
        _M_find(const _CharT2*, size_t __len, _CharT2 __c) const
        {
   int __ret = -1;
   if (__len <= 10)
     {
       if (__c >= _CharT2('0') && __c < _CharT2(_CharT2('0') + __len))
  __ret = __c - _CharT2('0');
     }
   else
     {
       if (__c >= _CharT2('0') && __c <= _CharT2('9'))
  __ret = __c - _CharT2('0');
       else if (__c >= _CharT2('a') && __c <= _CharT2('f'))
  __ret = 10 + (__c - _CharT2('a'));
       else if (__c >= _CharT2('A') && __c <= _CharT2('F'))
  __ret = 10 + (__c - _CharT2('A'));
     }
   return __ret;
 }

      template<typename _CharT2>
      typename __gnu_cxx::__enable_if<!__is_char<_CharT2>::__value,
          int>::__type
        _M_find(const _CharT2* __zero, size_t __len, _CharT2 __c) const
        {
   int __ret = -1;
   const char_type* __q = char_traits<_CharT2>::find(__zero, __len, __c);
   if (__q)
     {
       __ret = __q - __zero;
       if (__ret > 15)
  __ret -= 6;
     }
   return __ret;
 }
# 2186 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual iter_type
      do_get(iter_type, iter_type, ios_base&, ios_base::iostate&, bool&) const;


      virtual iter_type
      do_get(iter_type, iter_type, ios_base&, ios_base::iostate&, long&) const;

      virtual iter_type
      do_get(iter_type, iter_type, ios_base&, ios_base::iostate& __err,
       unsigned short&) const;

      virtual iter_type
      do_get(iter_type, iter_type, ios_base&, ios_base::iostate& __err,
      unsigned int&) const;

      virtual iter_type
      do_get(iter_type, iter_type, ios_base&, ios_base::iostate& __err,
      unsigned long&) const;


      virtual iter_type
      do_get(iter_type, iter_type, ios_base&, ios_base::iostate& __err,
      long long&) const;

      virtual iter_type
      do_get(iter_type, iter_type, ios_base&, ios_base::iostate& __err,
      unsigned long long&) const;


      virtual iter_type
      do_get(iter_type, iter_type, ios_base&, ios_base::iostate& __err,
      float&) const;

      virtual iter_type
      do_get(iter_type, iter_type, ios_base&, ios_base::iostate& __err,
      double&) const;







      virtual iter_type
      do_get(iter_type, iter_type, ios_base&, ios_base::iostate& __err,
      long double&) const;


      virtual iter_type
      do_get(iter_type, iter_type, ios_base&, ios_base::iostate& __err,
      void*&) const;
# 2245 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
    };

  template<typename _CharT, typename _InIter>
    locale::id num_get<_CharT, _InIter>::id;
# 2262 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  template<typename _CharT, typename _OutIter>
    class num_put : public locale::facet
    {
    public:



      typedef _CharT char_type;
      typedef _OutIter iter_type;



      static locale::id id;
# 2283 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      num_put(size_t __refs = 0) : facet(__refs) { }
# 2301 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      put(iter_type __s, ios_base& __f, char_type __fill, bool __v) const
      { return this->do_put(__s, __f, __fill, __v); }
# 2343 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      put(iter_type __s, ios_base& __f, char_type __fill, long __v) const
      { return this->do_put(__s, __f, __fill, __v); }

      iter_type
      put(iter_type __s, ios_base& __f, char_type __fill,
   unsigned long __v) const
      { return this->do_put(__s, __f, __fill, __v); }


      iter_type
      put(iter_type __s, ios_base& __f, char_type __fill, long long __v) const
      { return this->do_put(__s, __f, __fill, __v); }

      iter_type
      put(iter_type __s, ios_base& __f, char_type __fill,
   unsigned long long __v) const
      { return this->do_put(__s, __f, __fill, __v); }
# 2406 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      put(iter_type __s, ios_base& __f, char_type __fill, double __v) const
      { return this->do_put(__s, __f, __fill, __v); }

      iter_type
      put(iter_type __s, ios_base& __f, char_type __fill,
   long double __v) const
      { return this->do_put(__s, __f, __fill, __v); }
# 2431 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      put(iter_type __s, ios_base& __f, char_type __fill,
   const void* __v) const
      { return this->do_put(__s, __f, __fill, __v); }

    protected:
      template<typename _ValueT>
        iter_type
        _M_insert_float(iter_type, ios_base& __io, char_type __fill,
   char __mod, _ValueT __v) const;

      void
      _M_group_float(const char* __grouping, size_t __grouping_size,
       char_type __sep, const char_type* __p, char_type* __new,
       char_type* __cs, int& __len) const;

      template<typename _ValueT>
        iter_type
        _M_insert_int(iter_type, ios_base& __io, char_type __fill,
        _ValueT __v) const;

      void
      _M_group_int(const char* __grouping, size_t __grouping_size,
     char_type __sep, ios_base& __io, char_type* __new,
     char_type* __cs, int& __len) const;

      void
      _M_pad(char_type __fill, streamsize __w, ios_base& __io,
      char_type* __new, const char_type* __cs, int& __len) const;


      virtual
      ~num_put() { };
# 2479 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual iter_type
      do_put(iter_type, ios_base&, char_type __fill, bool __v) const;

      virtual iter_type
      do_put(iter_type, ios_base&, char_type __fill, long __v) const;

      virtual iter_type
      do_put(iter_type, ios_base&, char_type __fill, unsigned long) const;


      virtual iter_type
      do_put(iter_type, ios_base&, char_type __fill, long long __v) const;

      virtual iter_type
      do_put(iter_type, ios_base&, char_type __fill, unsigned long long) const;


      virtual iter_type
      do_put(iter_type, ios_base&, char_type __fill, double __v) const;






      virtual iter_type
      do_put(iter_type, ios_base&, char_type __fill, long double __v) const;


      virtual iter_type
      do_put(iter_type, ios_base&, char_type __fill, const void* __v) const;







    };

  template <typename _CharT, typename _OutIter>
    locale::id num_put<_CharT, _OutIter>::id;


# 2536 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  template<typename _CharT>
    class collate : public locale::facet
    {
    public:



      typedef _CharT char_type;
      typedef basic_string<_CharT> string_type;


    protected:


      __c_locale _M_c_locale_collate;

    public:

      static locale::id id;
# 2563 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      collate(size_t __refs = 0)
      : facet(__refs), _M_c_locale_collate(_S_get_c_locale())
      { }
# 2577 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      collate(__c_locale __cloc, size_t __refs = 0)
      : facet(__refs), _M_c_locale_collate(_S_clone_c_locale(__cloc))
      { }
# 2594 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      int
      compare(const _CharT* __lo1, const _CharT* __hi1,
       const _CharT* __lo2, const _CharT* __hi2) const
      { return this->do_compare(__lo1, __hi1, __lo2, __hi2); }
# 2613 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      string_type
      transform(const _CharT* __lo, const _CharT* __hi) const
      { return this->do_transform(__lo, __hi); }
# 2627 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      long
      hash(const _CharT* __lo, const _CharT* __hi) const
      { return this->do_hash(__lo, __hi); }


      int
      _M_compare(const _CharT*, const _CharT*) const;

      size_t
      _M_transform(_CharT*, const _CharT*, size_t) const;

  protected:

      virtual
      ~collate()
      { _S_destroy_c_locale(_M_c_locale_collate); }
# 2656 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual int
      do_compare(const _CharT* __lo1, const _CharT* __hi1,
   const _CharT* __lo2, const _CharT* __hi2) const;
# 2672 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual string_type
      do_transform(const _CharT* __lo, const _CharT* __hi) const;
# 2685 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual long
      do_hash(const _CharT* __lo, const _CharT* __hi) const;
    };

  template<typename _CharT>
    locale::id collate<_CharT>::id;


  template<>
    int
    collate<char>::_M_compare(const char*, const char*) const;

  template<>
    size_t
    collate<char>::_M_transform(char*, const char*, size_t) const;


  template<>
    int
    collate<wchar_t>::_M_compare(const wchar_t*, const wchar_t*) const;

  template<>
    size_t
    collate<wchar_t>::_M_transform(wchar_t*, const wchar_t*, size_t) const;



  template<typename _CharT>
    class collate_byname : public collate<_CharT>
    {
    public:


      typedef _CharT char_type;
      typedef basic_string<_CharT> string_type;


      explicit
      collate_byname(const char* __s, size_t __refs = 0)
      : collate<_CharT>(__refs)
      {
 if (std::strcmp(__s, "C") != 0 && std::strcmp(__s, "POSIX") != 0)
   {
     this->_S_destroy_c_locale(this->_M_c_locale_collate);
     this->_S_create_c_locale(this->_M_c_locale_collate, __s);
   }
      }

    protected:
      virtual
      ~collate_byname() { }
    };
# 2745 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  class time_base
  {
  public:
    enum dateorder { no_order, dmy, mdy, ymd, ydm };
  };

  template<typename _CharT>
    struct __timepunct_cache : public locale::facet
    {

      static const _CharT* _S_timezones[14];

      const _CharT* _M_date_format;
      const _CharT* _M_date_era_format;
      const _CharT* _M_time_format;
      const _CharT* _M_time_era_format;
      const _CharT* _M_date_time_format;
      const _CharT* _M_date_time_era_format;
      const _CharT* _M_am;
      const _CharT* _M_pm;
      const _CharT* _M_am_pm_format;


      const _CharT* _M_day1;
      const _CharT* _M_day2;
      const _CharT* _M_day3;
      const _CharT* _M_day4;
      const _CharT* _M_day5;
      const _CharT* _M_day6;
      const _CharT* _M_day7;


      const _CharT* _M_aday1;
      const _CharT* _M_aday2;
      const _CharT* _M_aday3;
      const _CharT* _M_aday4;
      const _CharT* _M_aday5;
      const _CharT* _M_aday6;
      const _CharT* _M_aday7;


      const _CharT* _M_month01;
      const _CharT* _M_month02;
      const _CharT* _M_month03;
      const _CharT* _M_month04;
      const _CharT* _M_month05;
      const _CharT* _M_month06;
      const _CharT* _M_month07;
      const _CharT* _M_month08;
      const _CharT* _M_month09;
      const _CharT* _M_month10;
      const _CharT* _M_month11;
      const _CharT* _M_month12;


      const _CharT* _M_amonth01;
      const _CharT* _M_amonth02;
      const _CharT* _M_amonth03;
      const _CharT* _M_amonth04;
      const _CharT* _M_amonth05;
      const _CharT* _M_amonth06;
      const _CharT* _M_amonth07;
      const _CharT* _M_amonth08;
      const _CharT* _M_amonth09;
      const _CharT* _M_amonth10;
      const _CharT* _M_amonth11;
      const _CharT* _M_amonth12;

      bool _M_allocated;

      __timepunct_cache(size_t __refs = 0) : facet(__refs),
      _M_date_format(__null), _M_date_era_format(__null), _M_time_format(__null),
      _M_time_era_format(__null), _M_date_time_format(__null),
      _M_date_time_era_format(__null), _M_am(__null), _M_pm(__null),
      _M_am_pm_format(__null), _M_day1(__null), _M_day2(__null), _M_day3(__null),
      _M_day4(__null), _M_day5(__null), _M_day6(__null), _M_day7(__null),
      _M_aday1(__null), _M_aday2(__null), _M_aday3(__null), _M_aday4(__null),
      _M_aday5(__null), _M_aday6(__null), _M_aday7(__null), _M_month01(__null),
      _M_month02(__null), _M_month03(__null), _M_month04(__null), _M_month05(__null),
      _M_month06(__null), _M_month07(__null), _M_month08(__null), _M_month09(__null),
      _M_month10(__null), _M_month11(__null), _M_month12(__null), _M_amonth01(__null),
      _M_amonth02(__null), _M_amonth03(__null), _M_amonth04(__null),
      _M_amonth05(__null), _M_amonth06(__null), _M_amonth07(__null),
      _M_amonth08(__null), _M_amonth09(__null), _M_amonth10(__null),
      _M_amonth11(__null), _M_amonth12(__null), _M_allocated(false)
      { }

      ~__timepunct_cache();

      void
      _M_cache(const locale& __loc);

    private:
      __timepunct_cache&
      operator=(const __timepunct_cache&);

      explicit
      __timepunct_cache(const __timepunct_cache&);
    };

  template<typename _CharT>
    __timepunct_cache<_CharT>::~__timepunct_cache()
    {
      if (_M_allocated)
 {

 }
    }


  template<>
    const char*
    __timepunct_cache<char>::_S_timezones[14];


  template<>
    const wchar_t*
    __timepunct_cache<wchar_t>::_S_timezones[14];



  template<typename _CharT>
    const _CharT* __timepunct_cache<_CharT>::_S_timezones[14];

  template<typename _CharT>
    class __timepunct : public locale::facet
    {
    public:

      typedef _CharT __char_type;
      typedef basic_string<_CharT> __string_type;
      typedef __timepunct_cache<_CharT> __cache_type;

    protected:
      __cache_type* _M_data;
      __c_locale _M_c_locale_timepunct;
      const char* _M_name_timepunct;

    public:

      static locale::id id;

      explicit
      __timepunct(size_t __refs = 0);

      explicit
      __timepunct(__cache_type* __cache, size_t __refs = 0);
# 2903 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      __timepunct(__c_locale __cloc, const char* __s, size_t __refs = 0);



      void
      _M_put(_CharT* __s, size_t __maxlen, const _CharT* __format,
      const tm* __tm) const;

      void
      _M_date_formats(const _CharT** __date) const
      {

 __date[0] = _M_data->_M_date_format;
 __date[1] = _M_data->_M_date_era_format;
      }

      void
      _M_time_formats(const _CharT** __time) const
      {

 __time[0] = _M_data->_M_time_format;
 __time[1] = _M_data->_M_time_era_format;
      }

      void
      _M_date_time_formats(const _CharT** __dt) const
      {

 __dt[0] = _M_data->_M_date_time_format;
 __dt[1] = _M_data->_M_date_time_era_format;
      }

      void
      _M_am_pm_format(const _CharT* __ampm) const
      { __ampm = _M_data->_M_am_pm_format; }

      void
      _M_am_pm(const _CharT** __ampm) const
      {
 __ampm[0] = _M_data->_M_am;
 __ampm[1] = _M_data->_M_pm;
      }

      void
      _M_days(const _CharT** __days) const
      {
 __days[0] = _M_data->_M_day1;
 __days[1] = _M_data->_M_day2;
 __days[2] = _M_data->_M_day3;
 __days[3] = _M_data->_M_day4;
 __days[4] = _M_data->_M_day5;
 __days[5] = _M_data->_M_day6;
 __days[6] = _M_data->_M_day7;
      }

      void
      _M_days_abbreviated(const _CharT** __days) const
      {
 __days[0] = _M_data->_M_aday1;
 __days[1] = _M_data->_M_aday2;
 __days[2] = _M_data->_M_aday3;
 __days[3] = _M_data->_M_aday4;
 __days[4] = _M_data->_M_aday5;
 __days[5] = _M_data->_M_aday6;
 __days[6] = _M_data->_M_aday7;
      }

      void
      _M_months(const _CharT** __months) const
      {
 __months[0] = _M_data->_M_month01;
 __months[1] = _M_data->_M_month02;
 __months[2] = _M_data->_M_month03;
 __months[3] = _M_data->_M_month04;
 __months[4] = _M_data->_M_month05;
 __months[5] = _M_data->_M_month06;
 __months[6] = _M_data->_M_month07;
 __months[7] = _M_data->_M_month08;
 __months[8] = _M_data->_M_month09;
 __months[9] = _M_data->_M_month10;
 __months[10] = _M_data->_M_month11;
 __months[11] = _M_data->_M_month12;
      }

      void
      _M_months_abbreviated(const _CharT** __months) const
      {
 __months[0] = _M_data->_M_amonth01;
 __months[1] = _M_data->_M_amonth02;
 __months[2] = _M_data->_M_amonth03;
 __months[3] = _M_data->_M_amonth04;
 __months[4] = _M_data->_M_amonth05;
 __months[5] = _M_data->_M_amonth06;
 __months[6] = _M_data->_M_amonth07;
 __months[7] = _M_data->_M_amonth08;
 __months[8] = _M_data->_M_amonth09;
 __months[9] = _M_data->_M_amonth10;
 __months[10] = _M_data->_M_amonth11;
 __months[11] = _M_data->_M_amonth12;
      }

    protected:
      virtual
      ~__timepunct();


      void
      _M_initialize_timepunct(__c_locale __cloc = __null);
    };

  template<typename _CharT>
    locale::id __timepunct<_CharT>::id;


  template<>
    void
    __timepunct<char>::_M_initialize_timepunct(__c_locale __cloc);

  template<>
    void
    __timepunct<char>::_M_put(char*, size_t, const char*, const tm*) const;


  template<>
    void
    __timepunct<wchar_t>::_M_initialize_timepunct(__c_locale __cloc);

  template<>
    void
    __timepunct<wchar_t>::_M_put(wchar_t*, size_t, const wchar_t*,
     const tm*) const;


}


# 1 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/time_members.h" 1 3
# 42 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/time_members.h" 3
namespace std {

  template<typename _CharT>
    __timepunct<_CharT>::__timepunct(size_t __refs)
    : facet(__refs), _M_data(__null)
    {
      _M_name_timepunct = _S_get_c_name();
      _M_initialize_timepunct();
    }

  template<typename _CharT>
    __timepunct<_CharT>::__timepunct(__cache_type* __cache, size_t __refs)
    : facet(__refs), _M_data(__cache)
    {
      _M_name_timepunct = _S_get_c_name();
      _M_initialize_timepunct();
    }

  template<typename _CharT>
    __timepunct<_CharT>::__timepunct(__c_locale __cloc, const char* __s,
         size_t __refs)
    : facet(__refs), _M_data(__null)
    {
      const size_t __len = std::strlen(__s) + 1;
      char* __tmp = new char[__len];
      std::memcpy(__tmp, __s, __len);
      _M_name_timepunct = __tmp;

      try
 { _M_initialize_timepunct(__cloc); }
      catch(...)
 {
   delete [] _M_name_timepunct;
   throw;
 }
    }

  template<typename _CharT>
    __timepunct<_CharT>::~__timepunct()
    {
      if (_M_name_timepunct != _S_get_c_name())
 delete [] _M_name_timepunct;
      delete _M_data;
      _S_destroy_c_locale(_M_c_locale_timepunct);
    }

}
# 3041 "/usr/include/c++/4.2.1/bits/locale_facets.h" 2 3

namespace std {
# 3056 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  template<typename _CharT, typename _InIter>
    class time_get : public locale::facet, public time_base
    {
    public:



      typedef _CharT char_type;
      typedef _InIter iter_type;

      typedef basic_string<_CharT> __string_type;


      static locale::id id;
# 3078 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      time_get(size_t __refs = 0)
      : facet (__refs) { }
# 3095 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      dateorder
      date_order() const
      { return this->do_date_order(); }
# 3119 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      get_time(iter_type __beg, iter_type __end, ios_base& __io,
        ios_base::iostate& __err, tm* __tm) const
      { return this->do_get_time(__beg, __end, __io, __err, __tm); }
# 3144 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      get_date(iter_type __beg, iter_type __end, ios_base& __io,
        ios_base::iostate& __err, tm* __tm) const
      { return this->do_get_date(__beg, __end, __io, __err, __tm); }
# 3172 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      get_weekday(iter_type __beg, iter_type __end, ios_base& __io,
    ios_base::iostate& __err, tm* __tm) const
      { return this->do_get_weekday(__beg, __end, __io, __err, __tm); }
# 3201 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      get_monthname(iter_type __beg, iter_type __end, ios_base& __io,
      ios_base::iostate& __err, tm* __tm) const
      { return this->do_get_monthname(__beg, __end, __io, __err, __tm); }
# 3227 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      get_year(iter_type __beg, iter_type __end, ios_base& __io,
        ios_base::iostate& __err, tm* __tm) const
      { return this->do_get_year(__beg, __end, __io, __err, __tm); }

    protected:

      virtual
      ~time_get() { }
# 3247 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual dateorder
      do_date_order() const;
# 3265 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual iter_type
      do_get_time(iter_type __beg, iter_type __end, ios_base& __io,
    ios_base::iostate& __err, tm* __tm) const;
# 3284 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual iter_type
      do_get_date(iter_type __beg, iter_type __end, ios_base& __io,
    ios_base::iostate& __err, tm* __tm) const;
# 3303 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual iter_type
      do_get_weekday(iter_type __beg, iter_type __end, ios_base&,
       ios_base::iostate& __err, tm* __tm) const;
# 3322 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual iter_type
      do_get_monthname(iter_type __beg, iter_type __end, ios_base&,
         ios_base::iostate& __err, tm* __tm) const;
# 3341 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual iter_type
      do_get_year(iter_type __beg, iter_type __end, ios_base& __io,
    ios_base::iostate& __err, tm* __tm) const;


      iter_type
      _M_extract_num(iter_type __beg, iter_type __end, int& __member,
       int __min, int __max, size_t __len,
       ios_base& __io, ios_base::iostate& __err) const;



      iter_type
      _M_extract_name(iter_type __beg, iter_type __end, int& __member,
        const _CharT** __names, size_t __indexlen,
        ios_base& __io, ios_base::iostate& __err) const;


      iter_type
      _M_extract_via_format(iter_type __beg, iter_type __end, ios_base& __io,
       ios_base::iostate& __err, tm* __tm,
       const _CharT* __format) const;
    };

  template<typename _CharT, typename _InIter>
    locale::id time_get<_CharT, _InIter>::id;


  template<typename _CharT, typename _InIter>
    class time_get_byname : public time_get<_CharT, _InIter>
    {
    public:

      typedef _CharT char_type;
      typedef _InIter iter_type;

      explicit
      time_get_byname(const char*, size_t __refs = 0)
      : time_get<_CharT, _InIter>(__refs) { }

    protected:
      virtual
      ~time_get_byname() { }
    };
# 3397 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  template<typename _CharT, typename _OutIter>
    class time_put : public locale::facet
    {
    public:



      typedef _CharT char_type;
      typedef _OutIter iter_type;



      static locale::id id;
# 3418 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      time_put(size_t __refs = 0)
      : facet(__refs) { }
# 3437 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      put(iter_type __s, ios_base& __io, char_type __fill, const tm* __tm,
   const _CharT* __beg, const _CharT* __end) const;
# 3457 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      put(iter_type __s, ios_base& __io, char_type __fill,
   const tm* __tm, char __format, char __mod = 0) const
      { return this->do_put(__s, __io, __fill, __tm, __format, __mod); }

    protected:

      virtual
      ~time_put()
      { }
# 3484 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual iter_type
      do_put(iter_type __s, ios_base& __io, char_type __fill, const tm* __tm,
      char __format, char __mod) const;
    };

  template<typename _CharT, typename _OutIter>
    locale::id time_put<_CharT, _OutIter>::id;


  template<typename _CharT, typename _OutIter>
    class time_put_byname : public time_put<_CharT, _OutIter>
    {
    public:

      typedef _CharT char_type;
      typedef _OutIter iter_type;

      explicit
      time_put_byname(const char*, size_t __refs = 0)
      : time_put<_CharT, _OutIter>(__refs)
      { };

    protected:
      virtual
      ~time_put_byname() { }
    };
# 3522 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  class money_base
  {
  public:
    enum part { none, space, symbol, sign, value };
    struct pattern { char field[4]; };

    static const pattern _S_default_pattern;

    enum
    {
      _S_minus,
      _S_zero,
      _S_end = 11
    };



    static const char* _S_atoms;



    static pattern
    _S_construct_pattern(char __precedes, char __space, char __posn);
  };

  template<typename _CharT, bool _Intl>
    struct __moneypunct_cache : public locale::facet
    {
      const char* _M_grouping;
      size_t _M_grouping_size;
      bool _M_use_grouping;
      _CharT _M_decimal_point;
      _CharT _M_thousands_sep;
      const _CharT* _M_curr_symbol;
      size_t _M_curr_symbol_size;
      const _CharT* _M_positive_sign;
      size_t _M_positive_sign_size;
      const _CharT* _M_negative_sign;
      size_t _M_negative_sign_size;
      int _M_frac_digits;
      money_base::pattern _M_pos_format;
      money_base::pattern _M_neg_format;




      _CharT _M_atoms[money_base::_S_end];

      bool _M_allocated;

      __moneypunct_cache(size_t __refs = 0) : facet(__refs),
      _M_grouping(__null), _M_grouping_size(0), _M_use_grouping(false),
      _M_decimal_point(_CharT()), _M_thousands_sep(_CharT()),
      _M_curr_symbol(__null), _M_curr_symbol_size(0),
      _M_positive_sign(__null), _M_positive_sign_size(0),
      _M_negative_sign(__null), _M_negative_sign_size(0),
      _M_frac_digits(0),
      _M_pos_format(money_base::pattern()),
      _M_neg_format(money_base::pattern()), _M_allocated(false)
      { }

      ~__moneypunct_cache();

      void
      _M_cache(const locale& __loc);

    private:
      __moneypunct_cache&
      operator=(const __moneypunct_cache&);

      explicit
      __moneypunct_cache(const __moneypunct_cache&);
    };

  template<typename _CharT, bool _Intl>
    __moneypunct_cache<_CharT, _Intl>::~__moneypunct_cache()
    {
      if (_M_allocated)
 {
   delete [] _M_grouping;
   delete [] _M_curr_symbol;
   delete [] _M_positive_sign;
   delete [] _M_negative_sign;
 }
    }







  template<typename _CharT, bool _Intl>
    class moneypunct : public locale::facet, public money_base
    {
    public:



      typedef _CharT char_type;
      typedef basic_string<_CharT> string_type;

      typedef __moneypunct_cache<_CharT, _Intl> __cache_type;

    private:
      __cache_type* _M_data;

    public:


      static const bool intl = _Intl;

      static locale::id id;
# 3643 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      moneypunct(size_t __refs = 0) : facet(__refs), _M_data(__null)
      { _M_initialize_moneypunct(); }
# 3655 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      moneypunct(__cache_type* __cache, size_t __refs = 0)
      : facet(__refs), _M_data(__cache)
      { _M_initialize_moneypunct(); }
# 3670 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      moneypunct(__c_locale __cloc, const char* __s, size_t __refs = 0)
      : facet(__refs), _M_data(__null)
      { _M_initialize_moneypunct(__cloc, __s); }
# 3684 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      char_type
      decimal_point() const
      { return this->do_decimal_point(); }
# 3697 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      char_type
      thousands_sep() const
      { return this->do_thousands_sep(); }
# 3726 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      string
      grouping() const
      { return this->do_grouping(); }
# 3739 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      string_type
      curr_symbol() const
      { return this->do_curr_symbol(); }
# 3756 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      string_type
      positive_sign() const
      { return this->do_positive_sign(); }
# 3773 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      string_type
      negative_sign() const
      { return this->do_negative_sign(); }
# 3789 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      int
      frac_digits() const
      { return this->do_frac_digits(); }
# 3824 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      pattern
      pos_format() const
      { return this->do_pos_format(); }

      pattern
      neg_format() const
      { return this->do_neg_format(); }


    protected:

      virtual
      ~moneypunct();
# 3846 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char_type
      do_decimal_point() const
      { return _M_data->_M_decimal_point; }
# 3858 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual char_type
      do_thousands_sep() const
      { return _M_data->_M_thousands_sep; }
# 3871 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual string
      do_grouping() const
      { return _M_data->_M_grouping; }
# 3884 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual string_type
      do_curr_symbol() const
      { return _M_data->_M_curr_symbol; }
# 3897 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual string_type
      do_positive_sign() const
      { return _M_data->_M_positive_sign; }
# 3910 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual string_type
      do_negative_sign() const
      { return _M_data->_M_negative_sign; }
# 3924 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual int
      do_frac_digits() const
      { return _M_data->_M_frac_digits; }
# 3938 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual pattern
      do_pos_format() const
      { return _M_data->_M_pos_format; }
# 3952 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual pattern
      do_neg_format() const
      { return _M_data->_M_neg_format; }


       void
       _M_initialize_moneypunct(__c_locale __cloc = __null,
    const char* __name = __null);
    };

  template<typename _CharT, bool _Intl>
    locale::id moneypunct<_CharT, _Intl>::id;

  template<typename _CharT, bool _Intl>
    const bool moneypunct<_CharT, _Intl>::intl;

  template<>
    moneypunct<char, true>::~moneypunct();

  template<>
    moneypunct<char, false>::~moneypunct();

  template<>
    void
    moneypunct<char, true>::_M_initialize_moneypunct(__c_locale, const char*);

  template<>
    void
    moneypunct<char, false>::_M_initialize_moneypunct(__c_locale, const char*);


  template<>
    moneypunct<wchar_t, true>::~moneypunct();

  template<>
    moneypunct<wchar_t, false>::~moneypunct();

  template<>
    void
    moneypunct<wchar_t, true>::_M_initialize_moneypunct(__c_locale,
       const char*);

  template<>
    void
    moneypunct<wchar_t, false>::_M_initialize_moneypunct(__c_locale,
        const char*);



  template<typename _CharT, bool _Intl>
    class moneypunct_byname : public moneypunct<_CharT, _Intl>
    {
    public:
      typedef _CharT char_type;
      typedef basic_string<_CharT> string_type;

      static const bool intl = _Intl;

      explicit
      moneypunct_byname(const char* __s, size_t __refs = 0)
      : moneypunct<_CharT, _Intl>(__refs)
      {
 if (std::strcmp(__s, "C") != 0 && std::strcmp(__s, "POSIX") != 0)
   {
     __c_locale __tmp;
     this->_S_create_c_locale(__tmp, __s);
     this->_M_initialize_moneypunct(__tmp);
     this->_S_destroy_c_locale(__tmp);
   }
      }

    protected:
      virtual
      ~moneypunct_byname() { }
    };

  template<typename _CharT, bool _Intl>
    const bool moneypunct_byname<_CharT, _Intl>::intl;


# 4044 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  template<typename _CharT, typename _InIter>
    class money_get : public locale::facet
    {
    public:



      typedef _CharT char_type;
      typedef _InIter iter_type;
      typedef basic_string<_CharT> string_type;



      static locale::id id;
# 4066 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      money_get(size_t __refs = 0) : facet(__refs) { }
# 4096 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      get(iter_type __s, iter_type __end, bool __intl, ios_base& __io,
   ios_base::iostate& __err, long double& __units) const
      { return this->do_get(__s, __end, __intl, __io, __err, __units); }
# 4126 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      get(iter_type __s, iter_type __end, bool __intl, ios_base& __io,
   ios_base::iostate& __err, string_type& __digits) const
      { return this->do_get(__s, __end, __intl, __io, __err, __digits); }

    protected:

      virtual
      ~money_get() { }
# 4149 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual iter_type
      do_get(iter_type __s, iter_type __end, bool __intl, ios_base& __io,
      ios_base::iostate& __err, long double& __units) const;
# 4161 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual iter_type
      do_get(iter_type __s, iter_type __end, bool __intl, ios_base& __io,
      ios_base::iostate& __err, string_type& __digits) const;
# 4172 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      template<bool _Intl>
        iter_type
        _M_extract(iter_type __s, iter_type __end, ios_base& __io,
     ios_base::iostate& __err, string& __digits) const;
    };

  template<typename _CharT, typename _InIter>
    locale::id money_get<_CharT, _InIter>::id;
# 4193 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  template<typename _CharT, typename _OutIter>
    class money_put : public locale::facet
    {
    public:


      typedef _CharT char_type;
      typedef _OutIter iter_type;
      typedef basic_string<_CharT> string_type;



      static locale::id id;
# 4214 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      money_put(size_t __refs = 0) : facet(__refs) { }
# 4234 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      put(iter_type __s, bool __intl, ios_base& __io,
   char_type __fill, long double __units) const
      { return this->do_put(__s, __intl, __io, __fill, __units); }
# 4256 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      iter_type
      put(iter_type __s, bool __intl, ios_base& __io,
   char_type __fill, const string_type& __digits) const
      { return this->do_put(__s, __intl, __io, __fill, __digits); }

    protected:

      virtual
      ~money_put() { }
# 4290 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual iter_type
      do_put(iter_type __s, bool __intl, ios_base& __io, char_type __fill,
      long double __units) const;
# 4313 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual iter_type
      do_put(iter_type __s, bool __intl, ios_base& __io, char_type __fill,
      const string_type& __digits) const;
# 4324 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      template<bool _Intl>
        iter_type
        _M_insert(iter_type __s, ios_base& __io, char_type __fill,
    const string_type& __digits) const;
    };

  template<typename _CharT, typename _OutIter>
    locale::id money_put<_CharT, _OutIter>::id;






  struct messages_base
  {
    typedef int catalog;
  };
# 4363 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
  template<typename _CharT>
    class messages : public locale::facet, public messages_base
    {
    public:



      typedef _CharT char_type;
      typedef basic_string<_CharT> string_type;


    protected:


      __c_locale _M_c_locale_messages;
      const char* _M_name_messages;

    public:

      static locale::id id;
# 4391 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      messages(size_t __refs = 0);
# 4405 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      explicit
      messages(__c_locale __cloc, const char* __s, size_t __refs = 0);
# 4418 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      catalog
      open(const basic_string<char>& __s, const locale& __loc) const
      { return this->do_open(__s, __loc); }
# 4436 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      catalog
      open(const basic_string<char>&, const locale&, const char*) const;
# 4454 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      string_type
      get(catalog __c, int __set, int __msgid, const string_type& __s) const
      { return this->do_get(__c, __set, __msgid, __s); }
# 4465 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      void
      close(catalog __c) const
      { return this->do_close(__c); }

    protected:

      virtual
      ~messages();
# 4485 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual catalog
      do_open(const basic_string<char>&, const locale&) const;
# 4504 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
      virtual string_type
      do_get(catalog, int, int, const string_type& __dfault) const;






      virtual void
      do_close(catalog) const;


      char*
      _M_convert_to_char(const string_type& __msg) const
      {

 return reinterpret_cast<char*>(const_cast<_CharT*>(__msg.c_str()));
      }


      string_type
      _M_convert_from_char(char*) const
      {
# 4561 "/usr/include/c++/4.2.1/bits/locale_facets.h" 3
 return string_type();
      }
     };

  template<typename _CharT>
    locale::id messages<_CharT>::id;


  template<>
    string
    messages<char>::do_get(catalog, int, int, const string&) const;


  template<>
    wstring
    messages<wchar_t>::do_get(catalog, int, int, const wstring&) const;



   template<typename _CharT>
    class messages_byname : public messages<_CharT>
    {
    public:
      typedef _CharT char_type;
      typedef basic_string<_CharT> string_type;

      explicit
      messages_byname(const char* __s, size_t __refs = 0);

    protected:
      virtual
      ~messages_byname()
      { }
    };

}


# 1 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/messages_members.h" 1 3
# 41 "/usr/include/c++/4.2.1/i686-apple-darwin10/x86_64/bits/messages_members.h" 3
namespace std {


  template<typename _CharT>
     messages<_CharT>::messages(size_t __refs)
     : facet(__refs)
     { _M_c_locale_messages = _S_get_c_locale(); }

  template<typename _CharT>
     messages<_CharT>::messages(__c_locale, const char*, size_t __refs)
     : facet(__refs)
     { _M_c_locale_messages = _S_get_c_locale(); }

  template<typename _CharT>
    typename messages<_CharT>::catalog
    messages<_CharT>::open(const basic_string<char>& __s, const locale& __loc,
      const char*) const
    { return this->do_open(__s, __loc); }


  template<typename _CharT>
    messages<_CharT>::~messages()
    { _S_destroy_c_locale(_M_c_locale_messages); }

  template<typename _CharT>
    typename messages<_CharT>::catalog
    messages<_CharT>::do_open(const basic_string<char>&, const locale&) const
    { return 0; }

  template<typename _CharT>
    typename messages<_CharT>::string_type
    messages<_CharT>::do_get(catalog, int, int,
        const string_type& __dfault) const
    { return __dfault; }

  template<typename _CharT>
    void
    messages<_CharT>::do_close(catalog) const
    { }


   template<typename _CharT>
     messages_byname<_CharT>::messages_byname(const char* __s, size_t __refs)
     : messages<_CharT>(__refs)
     {
 if (std::strcmp(__s, "C") != 0 && std::strcmp(__s, "POSIX") != 0)
   {
     this->_S_destroy_c_locale(this->_M_c_locale_messages);
     this->_S_create_c_locale(this->_M_c_locale_messages, __s);
   }
     }

}
# 4600 "/usr/include/c++/4.2.1/bits/locale_facets.h" 2 3

namespace std {







  template<typename _CharT>
    inline bool
    isspace(_CharT __c, const locale& __loc)
    { return use_facet<ctype<_CharT> >(__loc).is(ctype_base::space, __c); }


  template<typename _CharT>
    inline bool
    isprint(_CharT __c, const locale& __loc)
    { return use_facet<ctype<_CharT> >(__loc).is(ctype_base::print, __c); }


  template<typename _CharT>
    inline bool
    iscntrl(_CharT __c, const locale& __loc)
    { return use_facet<ctype<_CharT> >(__loc).is(ctype_base::cntrl, __c); }


  template<typename _CharT>
    inline bool
    isupper(_CharT __c, const locale& __loc)
    { return use_facet<ctype<_CharT> >(__loc).is(ctype_base::upper, __c); }


  template<typename _CharT>
    inline bool
    islower(_CharT __c, const locale& __loc)
    { return use_facet<ctype<_CharT> >(__loc).is(ctype_base::lower, __c); }


  template<typename _CharT>
    inline bool
    isalpha(_CharT __c, const locale& __loc)
    { return use_facet<ctype<_CharT> >(__loc).is(ctype_base::alpha, __c); }


  template<typename _CharT>
    inline bool
    isdigit(_CharT __c, const locale& __loc)
    { return use_facet<ctype<_CharT> >(__loc).is(ctype_base::digit, __c); }


  template<typename _CharT>
    inline bool
    ispunct(_CharT __c, const locale& __loc)
    { return use_facet<ctype<_CharT> >(__loc).is(ctype_base::punct, __c); }


  template<typename _CharT>
    inline bool
    isxdigit(_CharT __c, const locale& __loc)
    { return use_facet<ctype<_CharT> >(__loc).is(ctype_base::xdigit, __c); }


  template<typename _CharT>
    inline bool
    isalnum(_CharT __c, const locale& __loc)
    { return use_facet<ctype<_CharT> >(__loc).is(ctype_base::alnum, __c); }


  template<typename _CharT>
    inline bool
    isgraph(_CharT __c, const locale& __loc)
    { return use_facet<ctype<_CharT> >(__loc).is(ctype_base::graph, __c); }


  template<typename _CharT>
    inline _CharT
    toupper(_CharT __c, const locale& __loc)
    { return use_facet<ctype<_CharT> >(__loc).toupper(__c); }


  template<typename _CharT>
    inline _CharT
    tolower(_CharT __c, const locale& __loc)
    { return use_facet<ctype<_CharT> >(__loc).tolower(__c); }

}
# 45 "/usr/include/c++/4.2.1/bits/basic_ios.h" 2 3

namespace std {
# 55 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
  template<typename _CharT, typename _Traits>
    class basic_ios : public ios_base
    {
    public:






      typedef _CharT char_type;
      typedef typename _Traits::int_type int_type;
      typedef typename _Traits::pos_type pos_type;
      typedef typename _Traits::off_type off_type;
      typedef _Traits traits_type;
# 78 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
      typedef ctype<_CharT> __ctype_type;
      typedef num_put<_CharT, ostreambuf_iterator<_CharT, _Traits> >
           __num_put_type;
      typedef num_get<_CharT, istreambuf_iterator<_CharT, _Traits> >
           __num_get_type;



    protected:
      basic_ostream<_CharT, _Traits>* _M_tie;
      mutable char_type _M_fill;
      mutable bool _M_fill_init;
      basic_streambuf<_CharT, _Traits>* _M_streambuf;


      const __ctype_type* _M_ctype;

      const __num_put_type* _M_num_put;

      const __num_get_type* _M_num_get;

    public:







      operator void*() const
      { return this->fail() ? 0 : const_cast<basic_ios*>(this); }

      bool
      operator!() const
      { return this->fail(); }
# 122 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
      iostate
      rdstate() const
      { return _M_streambuf_state; }
# 133 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
      void
      clear(iostate __state = goodbit);







      void
      setstate(iostate __state)
      { this->clear(this->rdstate() | __state); }




      void
      _M_setstate(iostate __state)
      {


 _M_streambuf_state |= __state;
 if (this->exceptions() & __state)
   throw;
      }







      bool
      good() const
      { return this->rdstate() == 0; }







      bool
      eof() const
      { return (this->rdstate() & eofbit) != 0; }
# 186 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
      bool
      fail() const
      { return (this->rdstate() & (badbit | failbit)) != 0; }







      bool
      bad() const
      { return (this->rdstate() & badbit) != 0; }
# 207 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
      iostate
      exceptions() const
      { return _M_exception; }
# 242 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
      void
      exceptions(iostate __except)
      {
        _M_exception = __except;
        this->clear(_M_streambuf_state);
      }







      explicit
      basic_ios(basic_streambuf<_CharT, _Traits>* __sb)
      : ios_base(), _M_tie(0), _M_fill(), _M_fill_init(false), _M_streambuf(0),
      _M_ctype(0), _M_num_put(0), _M_num_get(0)
      { this->init(__sb); }







      virtual
      ~basic_ios() { }
# 280 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
      basic_ostream<_CharT, _Traits>*
      tie() const
      { return _M_tie; }
# 292 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
      basic_ostream<_CharT, _Traits>*
      tie(basic_ostream<_CharT, _Traits>* __tiestr)
      {
        basic_ostream<_CharT, _Traits>* __old = _M_tie;
        _M_tie = __tiestr;
        return __old;
      }







      basic_streambuf<_CharT, _Traits>*
      rdbuf() const
      { return _M_streambuf; }
# 332 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
      basic_streambuf<_CharT, _Traits>*
      rdbuf(basic_streambuf<_CharT, _Traits>* __sb);
# 346 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
      basic_ios&
      copyfmt(const basic_ios& __rhs);







      char_type
      fill() const
      {
 if (!_M_fill_init)
   {
     _M_fill = this->widen(' ');
     _M_fill_init = true;
   }
 return _M_fill;
      }
# 375 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
      char_type
      fill(char_type __ch)
      {
 char_type __old = this->fill();
 _M_fill = __ch;
 return __old;
      }
# 395 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
      locale
      imbue(const locale& __loc);
# 415 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
      char
      narrow(char_type __c, char __dfault) const;
# 433 "/usr/include/c++/4.2.1/bits/basic_ios.h" 3
      char_type
      widen(char __c) const;

    protected:







      basic_ios()
      : ios_base(), _M_tie(0), _M_fill(char_type()), _M_fill_init(false),
      _M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
      { }







      void
      init(basic_streambuf<_CharT, _Traits>* __sb);

      void
      _M_cache_locale(const locale& __loc);
    };

}


# 1 "/usr/include/c++/4.2.1/bits/basic_ios.tcc" 1 3
# 39 "/usr/include/c++/4.2.1/bits/basic_ios.tcc" 3
       
# 40 "/usr/include/c++/4.2.1/bits/basic_ios.tcc" 3

namespace std {

  template<typename _CharT, typename _Traits>
    void
    basic_ios<_CharT, _Traits>::clear(iostate __state)
    {
      if (this->rdbuf())
 _M_streambuf_state = __state;
      else
   _M_streambuf_state = __state | badbit;
      if (this->exceptions() & this->rdstate())
 __throw_ios_failure(("basic_ios::clear"));
    }

  template<typename _CharT, typename _Traits>
    basic_streambuf<_CharT, _Traits>*
    basic_ios<_CharT, _Traits>::rdbuf(basic_streambuf<_CharT, _Traits>* __sb)
    {
      basic_streambuf<_CharT, _Traits>* __old = _M_streambuf;
      _M_streambuf = __sb;
      this->clear();
      return __old;
    }

  template<typename _CharT, typename _Traits>
    basic_ios<_CharT, _Traits>&
    basic_ios<_CharT, _Traits>::copyfmt(const basic_ios& __rhs)
    {


      if (this != &__rhs)
 {




   _Words* __words = (__rhs._M_word_size <= _S_local_word_size) ?
                      _M_local_word : new _Words[__rhs._M_word_size];


   _Callback_list* __cb = __rhs._M_callbacks;
   if (__cb)
     __cb->_M_add_reference();
   _M_call_callbacks(erase_event);
   if (_M_word != _M_local_word)
     {
       delete [] _M_word;
       _M_word = 0;
     }
   _M_dispose_callbacks();


   _M_callbacks = __cb;
   for (int __i = 0; __i < __rhs._M_word_size; ++__i)
     __words[__i] = __rhs._M_word[__i];
   _M_word = __words;
   _M_word_size = __rhs._M_word_size;

   this->flags(__rhs.flags());
   this->width(__rhs.width());
   this->precision(__rhs.precision());
   this->tie(__rhs.tie());
   this->fill(__rhs.fill());
   _M_ios_locale = __rhs.getloc();
   _M_cache_locale(_M_ios_locale);

   _M_call_callbacks(copyfmt_event);


   this->exceptions(__rhs.exceptions());
 }
      return *this;
    }

  template<typename _CharT, typename _Traits>
    char
    basic_ios<_CharT, _Traits>::narrow(char_type __c, char __dfault) const
    { return __check_facet(_M_ctype).narrow(__c, __dfault); }

  template<typename _CharT, typename _Traits>
    _CharT
    basic_ios<_CharT, _Traits>::widen(char __c) const
    { return __check_facet(_M_ctype).widen(__c); }


  template<typename _CharT, typename _Traits>
    locale
    basic_ios<_CharT, _Traits>::imbue(const locale& __loc)
    {
      locale __old(this->getloc());
      ios_base::imbue(__loc);
      _M_cache_locale(__loc);
      if (this->rdbuf() != 0)
 this->rdbuf()->pubimbue(__loc);
      return __old;
    }

  template<typename _CharT, typename _Traits>
    void
    basic_ios<_CharT, _Traits>::init(basic_streambuf<_CharT, _Traits>* __sb)
    {

      ios_base::_M_init();


      _M_cache_locale(_M_ios_locale);
# 160 "/usr/include/c++/4.2.1/bits/basic_ios.tcc" 3
      _M_fill = _CharT();
      _M_fill_init = false;

      _M_tie = 0;
      _M_exception = goodbit;
      _M_streambuf = __sb;
      _M_streambuf_state = __sb ? goodbit : badbit;
    }

  template<typename _CharT, typename _Traits>
    void
    basic_ios<_CharT, _Traits>::_M_cache_locale(const locale& __loc)
    {
      if (__builtin_expect(has_facet<__ctype_type>(__loc), true))
 _M_ctype = &use_facet<__ctype_type>(__loc);
      else
 _M_ctype = 0;

      if (__builtin_expect(has_facet<__num_put_type>(__loc), true))
 _M_num_put = &use_facet<__num_put_type>(__loc);
      else
 _M_num_put = 0;

      if (__builtin_expect(has_facet<__num_get_type>(__loc), true))
 _M_num_get = &use_facet<__num_get_type>(__loc);
      else
 _M_num_get = 0;
    }





  extern template class basic_ios<char>;


  extern template class basic_ios<wchar_t>;



}
# 466 "/usr/include/c++/4.2.1/bits/basic_ios.h" 2 3
# 51 "/usr/include/c++/4.2.1/ios" 2 3
# 46 "/usr/include/c++/4.2.1/ostream" 2 3


namespace std {
# 58 "/usr/include/c++/4.2.1/ostream" 3
  template<typename _CharT, typename _Traits>
    class basic_ostream : virtual public basic_ios<_CharT, _Traits>
    {
    public:

      typedef _CharT char_type;
      typedef typename _Traits::int_type int_type;
      typedef typename _Traits::pos_type pos_type;
      typedef typename _Traits::off_type off_type;
      typedef _Traits traits_type;


      typedef basic_streambuf<_CharT, _Traits> __streambuf_type;
      typedef basic_ios<_CharT, _Traits> __ios_type;
      typedef basic_ostream<_CharT, _Traits> __ostream_type;
      typedef num_put<_CharT, ostreambuf_iterator<_CharT, _Traits> >
             __num_put_type;
      typedef ctype<_CharT> __ctype_type;
# 85 "/usr/include/c++/4.2.1/ostream" 3
      explicit
      basic_ostream(__streambuf_type* __sb)
      { this->init(__sb); }






      virtual
      ~basic_ostream() { }


      class sentry;
      friend class sentry;
# 111 "/usr/include/c++/4.2.1/ostream" 3
      __ostream_type&
      operator<<(__ostream_type& (*__pf)(__ostream_type&))
      {



 return __pf(*this);
      }

      __ostream_type&
      operator<<(__ios_type& (*__pf)(__ios_type&))
      {



 __pf(*this);
 return *this;
      }

      __ostream_type&
      operator<<(ios_base& (*__pf) (ios_base&))
      {



 __pf(*this);
 return *this;
      }
# 168 "/usr/include/c++/4.2.1/ostream" 3
      __ostream_type&
      operator<<(long __n)
      { return _M_insert(__n); }

      __ostream_type&
      operator<<(unsigned long __n)
      { return _M_insert(__n); }

      __ostream_type&
      operator<<(bool __n)
      { return _M_insert(__n); }

      __ostream_type&
      operator<<(short __n);

      __ostream_type&
      operator<<(unsigned short __n)
      {


 return _M_insert(static_cast<unsigned long>(__n));
      }

      __ostream_type&
      operator<<(int __n);

      __ostream_type&
      operator<<(unsigned int __n)
      {


 return _M_insert(static_cast<unsigned long>(__n));
      }


      __ostream_type&
      operator<<(long long __n)
      { return _M_insert(__n); }

      __ostream_type&
      operator<<(unsigned long long __n)
      { return _M_insert(__n); }


      __ostream_type&
      operator<<(double __f)
      { return _M_insert(__f); }

      __ostream_type&
      operator<<(float __f)
      {


 return _M_insert(static_cast<double>(__f));
      }

      __ostream_type&
      operator<<(long double __f)
      { return _M_insert(__f); }

      __ostream_type&
      operator<<(const void* __p)
      { return _M_insert(__p); }
# 253 "/usr/include/c++/4.2.1/ostream" 3
      __ostream_type&
      operator<<(__streambuf_type* __sb);
# 286 "/usr/include/c++/4.2.1/ostream" 3
      __ostream_type&
      put(char_type __c);


      void
      _M_write(const char_type* __s, streamsize __n)
      {
 const streamsize __put = this->rdbuf()->sputn(__s, __n);
 if (__put != __n)
   this->setstate(ios_base::badbit);
      }
# 314 "/usr/include/c++/4.2.1/ostream" 3
      __ostream_type&
      write(const char_type* __s, streamsize __n);
# 327 "/usr/include/c++/4.2.1/ostream" 3
      __ostream_type&
      flush();
# 338 "/usr/include/c++/4.2.1/ostream" 3
      pos_type
      tellp();
# 349 "/usr/include/c++/4.2.1/ostream" 3
      __ostream_type&
      seekp(pos_type);
# 361 "/usr/include/c++/4.2.1/ostream" 3
       __ostream_type&
      seekp(off_type, ios_base::seekdir);

    protected:
      explicit
      basic_ostream() { }

      template<typename _ValueT>
        __ostream_type&
        _M_insert(_ValueT __v);
    };
# 383 "/usr/include/c++/4.2.1/ostream" 3
  template <typename _CharT, typename _Traits>
    class basic_ostream<_CharT, _Traits>::sentry
    {

      bool _M_ok;
      basic_ostream<_CharT, _Traits>& _M_os;

    public:
# 402 "/usr/include/c++/4.2.1/ostream" 3
      explicit
      sentry(basic_ostream<_CharT, _Traits>& __os);
# 412 "/usr/include/c++/4.2.1/ostream" 3
      ~sentry()
      {

 if (_M_os.flags() & ios_base::unitbuf && !uncaught_exception())
   {

     if (_M_os.rdbuf() && _M_os.rdbuf()->pubsync() == -1)
       _M_os.setstate(ios_base::badbit);
   }
      }
# 430 "/usr/include/c++/4.2.1/ostream" 3
      operator bool() const
      { return _M_ok; }
    };
# 451 "/usr/include/c++/4.2.1/ostream" 3
  template<typename _CharT, typename _Traits>
    inline basic_ostream<_CharT, _Traits>&
    operator<<(basic_ostream<_CharT, _Traits>& __out, _CharT __c)
    { return __ostream_insert(__out, &__c, 1); }

  template<typename _CharT, typename _Traits>
    inline basic_ostream<_CharT, _Traits>&
    operator<<(basic_ostream<_CharT, _Traits>& __out, char __c)
    { return (__out << __out.widen(__c)); }


  template <class _Traits>
    inline basic_ostream<char, _Traits>&
    operator<<(basic_ostream<char, _Traits>& __out, char __c)
    { return __ostream_insert(__out, &__c, 1); }


  template<class _Traits>
    inline basic_ostream<char, _Traits>&
    operator<<(basic_ostream<char, _Traits>& __out, signed char __c)
    { return (__out << static_cast<char>(__c)); }

  template<class _Traits>
    inline basic_ostream<char, _Traits>&
    operator<<(basic_ostream<char, _Traits>& __out, unsigned char __c)
    { return (__out << static_cast<char>(__c)); }
# 493 "/usr/include/c++/4.2.1/ostream" 3
  template<typename _CharT, typename _Traits>
    inline basic_ostream<_CharT, _Traits>&
    operator<<(basic_ostream<_CharT, _Traits>& __out, const _CharT* __s)
    {
      if (!__s)
 __out.setstate(ios_base::badbit);
      else
 __ostream_insert(__out, __s,
    static_cast<streamsize>(_Traits::length(__s)));
      return __out;
    }

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits> &
    operator<<(basic_ostream<_CharT, _Traits>& __out, const char* __s);


  template<class _Traits>
    inline basic_ostream<char, _Traits>&
    operator<<(basic_ostream<char, _Traits>& __out, const char* __s)
    {
      if (!__s)
 __out.setstate(ios_base::badbit);
      else
 __ostream_insert(__out, __s,
    static_cast<streamsize>(_Traits::length(__s)));
      return __out;
    }


  template<class _Traits>
    inline basic_ostream<char, _Traits>&
    operator<<(basic_ostream<char, _Traits>& __out, const signed char* __s)
    { return (__out << reinterpret_cast<const char*>(__s)); }

  template<class _Traits>
    inline basic_ostream<char, _Traits> &
    operator<<(basic_ostream<char, _Traits>& __out, const unsigned char* __s)
    { return (__out << reinterpret_cast<const char*>(__s)); }
# 543 "/usr/include/c++/4.2.1/ostream" 3
  template<typename _CharT, typename _Traits>
    inline basic_ostream<_CharT, _Traits>&
    endl(basic_ostream<_CharT, _Traits>& __os)
    { return flush(__os.put(__os.widen('\n'))); }







  template<typename _CharT, typename _Traits>
    inline basic_ostream<_CharT, _Traits>&
    ends(basic_ostream<_CharT, _Traits>& __os)
    { return __os.put(_CharT()); }






  template<typename _CharT, typename _Traits>
    inline basic_ostream<_CharT, _Traits>&
    flush(basic_ostream<_CharT, _Traits>& __os)
    { return __os.flush(); }

}


# 1 "/usr/include/c++/4.2.1/bits/ostream.tcc" 1 3
# 44 "/usr/include/c++/4.2.1/bits/ostream.tcc" 3
       
# 45 "/usr/include/c++/4.2.1/bits/ostream.tcc" 3

# 1 "/usr/include/c++/4.2.1/locale" 1 3
# 41 "/usr/include/c++/4.2.1/locale" 3
       
# 42 "/usr/include/c++/4.2.1/locale" 3




# 1 "/usr/include/c++/4.2.1/bits/locale_facets.tcc" 1 3
# 39 "/usr/include/c++/4.2.1/bits/locale_facets.tcc" 3
       
# 40 "/usr/include/c++/4.2.1/bits/locale_facets.tcc" 3


# 1 "/usr/include/c++/4.2.1/typeinfo" 1 3
# 41 "/usr/include/c++/4.2.1/typeinfo" 3
#pragma GCC visibility push(default)

extern "C++" {

namespace __cxxabiv1
{
  class __class_type_info;
}
# 60 "/usr/include/c++/4.2.1/typeinfo" 3
namespace std
{






  class type_info
  {
  public:




    virtual ~type_info();



    const char* name() const
    { return __name; }
# 94 "/usr/include/c++/4.2.1/typeinfo" 3
    bool before(const type_info& __arg) const
    { return __name < __arg.__name; }

    bool operator==(const type_info& __arg) const
    { return __name == __arg.__name; }

    bool operator!=(const type_info& __arg) const
    { return !operator==(__arg); }





    virtual bool __is_pointer_p() const;


    virtual bool __is_function_p() const;







    virtual bool __do_catch(const type_info *__thr_type, void **__thr_obj,
       unsigned __outer) const;


    virtual bool __do_upcast(const __cxxabiv1::__class_type_info *__target,
        void **__obj_ptr) const;



  protected:
    const char *__name;

    explicit type_info(const char *__n): __name(__n) { }

  private:

    type_info& operator=(const type_info&);
    type_info(const type_info&);
  };






  class bad_cast : public exception
  {
  public:
    bad_cast() throw() { }



    virtual ~bad_cast() throw();


    virtual const char* what() const throw();
  };


  class bad_typeid : public exception
  {
  public:
    bad_typeid () throw() { }



    virtual ~bad_typeid() throw();


    virtual const char* what() const throw();
  };
}

#pragma GCC visibility pop

}
# 43 "/usr/include/c++/4.2.1/bits/locale_facets.tcc" 2 3



namespace std {

  template<typename _Facet>
    locale
    locale::combine(const locale& __other) const
    {
      _Impl* __tmp = new _Impl(*_M_impl, 1);
      try
 {
   __tmp->_M_replace_facet(__other._M_impl, &_Facet::id);
 }
      catch(...)
 {
   __tmp->_M_remove_reference();
   throw;
 }
      return locale(__tmp);
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    bool
    locale::operator()(const basic_string<_CharT, _Traits, _Alloc>& __s1,
                       const basic_string<_CharT, _Traits, _Alloc>& __s2) const
    {
      typedef std::collate<_CharT> __collate_type;
      const __collate_type& __collate = use_facet<__collate_type>(*this);
      return (__collate.compare(__s1.data(), __s1.data() + __s1.length(),
    __s2.data(), __s2.data() + __s2.length()) < 0);
    }
# 87 "/usr/include/c++/4.2.1/bits/locale_facets.tcc" 3
  template<typename _Facet>
    inline bool
    has_facet(const locale& __loc) throw()
    {
      const size_t __i = _Facet::id._M_id();
      const locale::facet** __facets = __loc._M_impl->_M_facets;
      return (__i < __loc._M_impl->_M_facets_size && __facets[__i]);
    }
# 109 "/usr/include/c++/4.2.1/bits/locale_facets.tcc" 3
  template<typename _Facet>
    inline const _Facet&
    use_facet(const locale& __loc)
    {
      const size_t __i = _Facet::id._M_id();
      const locale::facet** __facets = __loc._M_impl->_M_facets;
      if (!(__i < __loc._M_impl->_M_facets_size && __facets[__i]))
        __throw_bad_cast();
      return static_cast<const _Facet&>(*__facets[__i]);
    }



  template<typename _Facet>
    struct __use_cache
    {
      const _Facet*
      operator() (const locale& __loc) const;
    };


  template<typename _CharT>
    struct __use_cache<__numpunct_cache<_CharT> >
    {
      const __numpunct_cache<_CharT>*
      operator() (const locale& __loc) const
      {
 const size_t __i = numpunct<_CharT>::id._M_id();
 const locale::facet** __caches = __loc._M_impl->_M_caches;
 if (!__caches[__i])
   {
     __numpunct_cache<_CharT>* __tmp = __null;
     try
       {
  __tmp = new __numpunct_cache<_CharT>;
  __tmp->_M_cache(__loc);
       }
     catch(...)
       {
  delete __tmp;
  throw;
       }
     __loc._M_impl->_M_install_cache(__tmp, __i);
   }
 return static_cast<const __numpunct_cache<_CharT>*>(__caches[__i]);
      }
    };

  template<typename _CharT, bool _Intl>
    struct __use_cache<__moneypunct_cache<_CharT, _Intl> >
    {
      const __moneypunct_cache<_CharT, _Intl>*
      operator() (const locale& __loc) const
      {
 const size_t __i = moneypunct<_CharT, _Intl>::id._M_id();
 const locale::facet** __caches = __loc._M_impl->_M_caches;
 if (!__caches[__i])
   {
     __moneypunct_cache<_CharT, _Intl>* __tmp = __null;
     try
       {
  __tmp = new __moneypunct_cache<_CharT, _Intl>;
  __tmp->_M_cache(__loc);
       }
     catch(...)
       {
  delete __tmp;
  throw;
       }
     __loc._M_impl->_M_install_cache(__tmp, __i);
   }
 return static_cast<
   const __moneypunct_cache<_CharT, _Intl>*>(__caches[__i]);
      }
    };

  template<typename _CharT>
    void
    __numpunct_cache<_CharT>::_M_cache(const locale& __loc)
    {
      _M_allocated = true;

      const numpunct<_CharT>& __np = use_facet<numpunct<_CharT> >(__loc);

      _M_grouping_size = __np.grouping().size();
      char* __grouping = new char[_M_grouping_size];
      __np.grouping().copy(__grouping, _M_grouping_size);
      _M_grouping = __grouping;
      _M_use_grouping = (_M_grouping_size
    && static_cast<signed char>(__np.grouping()[0]) > 0);

      _M_truename_size = __np.truename().size();
      _CharT* __truename = new _CharT[_M_truename_size];
      __np.truename().copy(__truename, _M_truename_size);
      _M_truename = __truename;

      _M_falsename_size = __np.falsename().size();
      _CharT* __falsename = new _CharT[_M_falsename_size];
      __np.falsename().copy(__falsename, _M_falsename_size);
      _M_falsename = __falsename;

      _M_decimal_point = __np.decimal_point();
      _M_thousands_sep = __np.thousands_sep();

      const ctype<_CharT>& __ct = use_facet<ctype<_CharT> >(__loc);
      __ct.widen(__num_base::_S_atoms_out,
   __num_base::_S_atoms_out + __num_base::_S_oend, _M_atoms_out);
      __ct.widen(__num_base::_S_atoms_in,
   __num_base::_S_atoms_in + __num_base::_S_iend, _M_atoms_in);
    }

  template<typename _CharT, bool _Intl>
    void
    __moneypunct_cache<_CharT, _Intl>::_M_cache(const locale& __loc)
    {
      _M_allocated = true;

      const moneypunct<_CharT, _Intl>& __mp =
 use_facet<moneypunct<_CharT, _Intl> >(__loc);

      _M_grouping_size = __mp.grouping().size();
      char* __grouping = new char[_M_grouping_size];
      __mp.grouping().copy(__grouping, _M_grouping_size);
      _M_grouping = __grouping;
      _M_use_grouping = (_M_grouping_size
    && static_cast<signed char>(__mp.grouping()[0]) > 0);

      _M_decimal_point = __mp.decimal_point();
      _M_thousands_sep = __mp.thousands_sep();
      _M_frac_digits = __mp.frac_digits();

      _M_curr_symbol_size = __mp.curr_symbol().size();
      _CharT* __curr_symbol = new _CharT[_M_curr_symbol_size];
      __mp.curr_symbol().copy(__curr_symbol, _M_curr_symbol_size);
      _M_curr_symbol = __curr_symbol;

      _M_positive_sign_size = __mp.positive_sign().size();
      _CharT* __positive_sign = new _CharT[_M_positive_sign_size];
      __mp.positive_sign().copy(__positive_sign, _M_positive_sign_size);
      _M_positive_sign = __positive_sign;

      _M_negative_sign_size = __mp.negative_sign().size();
      _CharT* __negative_sign = new _CharT[_M_negative_sign_size];
      __mp.negative_sign().copy(__negative_sign, _M_negative_sign_size);
      _M_negative_sign = __negative_sign;

      _M_pos_format = __mp.pos_format();
      _M_neg_format = __mp.neg_format();

      const ctype<_CharT>& __ct = use_facet<ctype<_CharT> >(__loc);
      __ct.widen(money_base::_S_atoms,
   money_base::_S_atoms + money_base::_S_end, _M_atoms);
    }
# 272 "/usr/include/c++/4.2.1/bits/locale_facets.tcc" 3
  static bool
  __verify_grouping(const char* __grouping, size_t __grouping_size,
      const string& __grouping_tmp);



  template<typename _CharT, typename _InIter>
    _InIter
    num_get<_CharT, _InIter>::
    _M_extract_float(_InIter __beg, _InIter __end, ios_base& __io,
       ios_base::iostate& __err, string& __xtrc) const
    {
      typedef char_traits<_CharT> __traits_type;
      typedef __numpunct_cache<_CharT> __cache_type;
      __use_cache<__cache_type> __uc;
      const locale& __loc = __io._M_getloc();
      const __cache_type* __lc = __uc(__loc);
      const _CharT* __lit = __lc->_M_atoms_in;
      char_type __c = char_type();


      bool __testeof = __beg == __end;


      if (!__testeof)
 {
   __c = *__beg;
   const bool __plus = __c == __lit[__num_base::_S_iplus];
   if ((__plus || __c == __lit[__num_base::_S_iminus])
       && !(__lc->_M_use_grouping && __c == __lc->_M_thousands_sep)
       && !(__c == __lc->_M_decimal_point))
     {
       __xtrc += __plus ? '+' : '-';
       if (++__beg != __end)
  __c = *__beg;
       else
  __testeof = true;
     }
 }


      bool __found_mantissa = false;
      int __sep_pos = 0;
      while (!__testeof)
 {
   if (__lc->_M_use_grouping && __c == __lc->_M_thousands_sep
       || __c == __lc->_M_decimal_point)
     break;
   else if (__c == __lit[__num_base::_S_izero])
     {
       if (!__found_mantissa)
  {
    __xtrc += '0';
    __found_mantissa = true;
  }
       ++__sep_pos;

       if (++__beg != __end)
  __c = *__beg;
       else
  __testeof = true;
     }
   else
     break;
 }


      bool __found_dec = false;
      bool __found_sci = false;
      string __found_grouping;
      if (__lc->_M_use_grouping)
 __found_grouping.reserve(32);
      const char_type* __lit_zero = __lit + __num_base::_S_izero;

      if (!__lc->_M_allocated)

 while (!__testeof)
   {
     const int __digit = _M_find(__lit_zero, 10, __c);
     if (__digit != -1)
       {
  __xtrc += '0' + __digit;
  __found_mantissa = true;
       }
     else if (__c == __lc->_M_decimal_point
       && !__found_dec && !__found_sci)
       {
  __xtrc += '.';
  __found_dec = true;
       }
     else if ((__c == __lit[__num_base::_S_ie]
        || __c == __lit[__num_base::_S_iE])
       && !__found_sci && __found_mantissa)
       {

  __xtrc += 'e';
  __found_sci = true;


  if (++__beg != __end)
    {
      __c = *__beg;
      const bool __plus = __c == __lit[__num_base::_S_iplus];
      if (__plus || __c == __lit[__num_base::_S_iminus])
        __xtrc += __plus ? '+' : '-';
      else
        continue;
    }
  else
    {
      __testeof = true;
      break;
    }
       }
     else
       break;

     if (++__beg != __end)
       __c = *__beg;
     else
       __testeof = true;
   }
      else
 while (!__testeof)
   {


     if (__lc->_M_use_grouping && __c == __lc->_M_thousands_sep)
       {
  if (!__found_dec && !__found_sci)
    {


      if (__sep_pos)
        {
   __found_grouping += static_cast<char>(__sep_pos);
   __sep_pos = 0;
        }
      else
        {


   __xtrc.clear();
   break;
        }
    }
  else
    break;
       }
     else if (__c == __lc->_M_decimal_point)
       {
  if (!__found_dec && !__found_sci)
    {



      if (__found_grouping.size())
        __found_grouping += static_cast<char>(__sep_pos);
      __xtrc += '.';
      __found_dec = true;
    }
  else
    break;
       }
     else
       {
  const char_type* __q =
    __traits_type::find(__lit_zero, 10, __c);
  if (__q)
    {
      __xtrc += '0' + (__q - __lit_zero);
      __found_mantissa = true;
      ++__sep_pos;
    }
  else if ((__c == __lit[__num_base::_S_ie]
     || __c == __lit[__num_base::_S_iE])
    && !__found_sci && __found_mantissa)
    {

      if (__found_grouping.size() && !__found_dec)
        __found_grouping += static_cast<char>(__sep_pos);
      __xtrc += 'e';
      __found_sci = true;


      if (++__beg != __end)
        {
   __c = *__beg;
   const bool __plus = __c == __lit[__num_base::_S_iplus];
   if ((__plus || __c == __lit[__num_base::_S_iminus])
       && !(__lc->_M_use_grouping
     && __c == __lc->_M_thousands_sep)
       && !(__c == __lc->_M_decimal_point))
        __xtrc += __plus ? '+' : '-';
   else
     continue;
        }
      else
        {
   __testeof = true;
   break;
        }
    }
  else
    break;
       }

     if (++__beg != __end)
       __c = *__beg;
     else
       __testeof = true;
   }



      if (__found_grouping.size())
        {

   if (!__found_dec && !__found_sci)
     __found_grouping += static_cast<char>(__sep_pos);

          if (!std::__verify_grouping(__lc->_M_grouping,
          __lc->_M_grouping_size,
          __found_grouping))
     __err |= ios_base::failbit;
        }


      if (__testeof)
        __err |= ios_base::eofbit;
      return __beg;
    }





  template<typename _CharT, typename _InIter>
    template<typename _ValueT>
      _InIter
      num_get<_CharT, _InIter>::
      _M_extract_int(_InIter __beg, _InIter __end, ios_base& __io,
       ios_base::iostate& __err, _ValueT& __v) const
      {
        typedef char_traits<_CharT> __traits_type;
 using __gnu_cxx::__add_unsigned;
 typedef typename __add_unsigned<_ValueT>::__type __unsigned_type;
 typedef __numpunct_cache<_CharT> __cache_type;
 __use_cache<__cache_type> __uc;
 const locale& __loc = __io._M_getloc();
 const __cache_type* __lc = __uc(__loc);
 const _CharT* __lit = __lc->_M_atoms_in;
 char_type __c = char_type();


 const ios_base::fmtflags __basefield = __io.flags()
                                        & ios_base::basefield;
 const bool __oct = __basefield == ios_base::oct;
 int __base = __oct ? 8 : (__basefield == ios_base::hex ? 16 : 10);


 bool __testeof = __beg == __end;


 bool __negative = false;
 if (!__testeof)
   {
     __c = *__beg;
     if (numeric_limits<_ValueT>::is_signed)
       __negative = __c == __lit[__num_base::_S_iminus];
     if ((__negative || __c == __lit[__num_base::_S_iplus])
  && !(__lc->_M_use_grouping && __c == __lc->_M_thousands_sep)
  && !(__c == __lc->_M_decimal_point))
       {
  if (++__beg != __end)
    __c = *__beg;
  else
    __testeof = true;
       }
   }



 bool __found_zero = false;
 int __sep_pos = 0;
 while (!__testeof)
   {
     if (__lc->_M_use_grouping && __c == __lc->_M_thousands_sep
  || __c == __lc->_M_decimal_point)
       break;
     else if (__c == __lit[__num_base::_S_izero]
       && (!__found_zero || __base == 10))
       {
  __found_zero = true;
  ++__sep_pos;
  if (__basefield == 0)
    __base = 8;
  if (__base == 8)
    __sep_pos = 0;
       }
     else if (__found_zero
       && (__c == __lit[__num_base::_S_ix]
    || __c == __lit[__num_base::_S_iX]))
       {
  if (__basefield == 0)
    __base = 16;
  if (__base == 16)
    {
      __found_zero = false;
      __sep_pos = 0;
    }
  else
    break;
       }
     else
       break;

     if (++__beg != __end)
       {
  __c = *__beg;
  if (!__found_zero)
    break;
       }
     else
       __testeof = true;
   }



 const size_t __len = (__base == 16 ? __num_base::_S_iend
         - __num_base::_S_izero : __base);


 string __found_grouping;
 if (__lc->_M_use_grouping)
   __found_grouping.reserve(32);
 bool __testfail = false;
 const __unsigned_type __max = __negative ?
   -numeric_limits<_ValueT>::min() : numeric_limits<_ValueT>::max();
 const __unsigned_type __smax = __max / __base;
 __unsigned_type __result = 0;
 int __digit = 0;
 const char_type* __lit_zero = __lit + __num_base::_S_izero;

 if (!__lc->_M_allocated)

   while (!__testeof)
     {
       __digit = _M_find(__lit_zero, __len, __c);
       if (__digit == -1)
  break;

       if (__result > __smax)
  __testfail = true;
       else
  {
    __result *= __base;
    __testfail |= __result > __max - __digit;
    __result += __digit;
    ++__sep_pos;
  }

       if (++__beg != __end)
  __c = *__beg;
       else
  __testeof = true;
     }
 else
   while (!__testeof)
     {


       if (__lc->_M_use_grouping && __c == __lc->_M_thousands_sep)
  {


    if (__sep_pos)
      {
        __found_grouping += static_cast<char>(__sep_pos);
        __sep_pos = 0;
      }
    else
      {
        __testfail = true;
        break;
      }
  }
       else if (__c == __lc->_M_decimal_point)
  break;
       else
  {
    const char_type* __q =
      __traits_type::find(__lit_zero, __len, __c);
    if (!__q)
      break;

    __digit = __q - __lit_zero;
    if (__digit > 15)
      __digit -= 6;
    if (__result > __smax)
      __testfail = true;
    else
      {
        __result *= __base;
        __testfail |= __result > __max - __digit;
        __result += __digit;
        ++__sep_pos;
      }
  }

       if (++__beg != __end)
  __c = *__beg;
       else
  __testeof = true;
     }



 if (__found_grouping.size())
   {

     __found_grouping += static_cast<char>(__sep_pos);

     if (!std::__verify_grouping(__lc->_M_grouping,
     __lc->_M_grouping_size,
     __found_grouping))
       __err |= ios_base::failbit;
   }

 if (!__testfail && (__sep_pos || __found_zero
       || __found_grouping.size()))
   __v = __negative ? -__result : __result;
 else
   __err |= ios_base::failbit;

 if (__testeof)
   __err |= ios_base::eofbit;
 return __beg;
      }



  template<typename _CharT, typename _InIter>
    _InIter
    num_get<_CharT, _InIter>::
    do_get(iter_type __beg, iter_type __end, ios_base& __io,
           ios_base::iostate& __err, bool& __v) const
    {
      if (!(__io.flags() & ios_base::boolalpha))
        {



   long __l = -1;
          __beg = _M_extract_int(__beg, __end, __io, __err, __l);
   if (__l == 0 || __l == 1)
     __v = __l;
   else
            __err |= ios_base::failbit;
        }
      else
        {

   typedef __numpunct_cache<_CharT> __cache_type;
   __use_cache<__cache_type> __uc;
   const locale& __loc = __io._M_getloc();
   const __cache_type* __lc = __uc(__loc);

   bool __testf = true;
   bool __testt = true;
   size_t __n;
   bool __testeof = __beg == __end;
          for (__n = 0; !__testeof; ++__n)
            {
       const char_type __c = *__beg;

       if (__testf)
  if (__n < __lc->_M_falsename_size)
    __testf = __c == __lc->_M_falsename[__n];
  else
    break;

       if (__testt)
  if (__n < __lc->_M_truename_size)
    __testt = __c == __lc->_M_truename[__n];
  else
    break;

       if (!__testf && !__testt)
  break;

       if (++__beg == __end)
  __testeof = true;
            }
   if (__testf && __n == __lc->_M_falsename_size)
     __v = 0;
   else if (__testt && __n == __lc->_M_truename_size)
     __v = 1;
   else
     __err |= ios_base::failbit;

          if (__testeof)
            __err |= ios_base::eofbit;
        }
      return __beg;
    }

  template<typename _CharT, typename _InIter>
    _InIter
    num_get<_CharT, _InIter>::
    do_get(iter_type __beg, iter_type __end, ios_base& __io,
           ios_base::iostate& __err, long& __v) const
    { return _M_extract_int(__beg, __end, __io, __err, __v); }

  template<typename _CharT, typename _InIter>
    _InIter
    num_get<_CharT, _InIter>::
    do_get(iter_type __beg, iter_type __end, ios_base& __io,
           ios_base::iostate& __err, unsigned short& __v) const
    { return _M_extract_int(__beg, __end, __io, __err, __v); }

  template<typename _CharT, typename _InIter>
    _InIter
    num_get<_CharT, _InIter>::
    do_get(iter_type __beg, iter_type __end, ios_base& __io,
           ios_base::iostate& __err, unsigned int& __v) const
    { return _M_extract_int(__beg, __end, __io, __err, __v); }

  template<typename _CharT, typename _InIter>
    _InIter
    num_get<_CharT, _InIter>::
    do_get(iter_type __beg, iter_type __end, ios_base& __io,
           ios_base::iostate& __err, unsigned long& __v) const
    { return _M_extract_int(__beg, __end, __io, __err, __v); }


  template<typename _CharT, typename _InIter>
    _InIter
    num_get<_CharT, _InIter>::
    do_get(iter_type __beg, iter_type __end, ios_base& __io,
           ios_base::iostate& __err, long long& __v) const
    { return _M_extract_int(__beg, __end, __io, __err, __v); }

  template<typename _CharT, typename _InIter>
    _InIter
    num_get<_CharT, _InIter>::
    do_get(iter_type __beg, iter_type __end, ios_base& __io,
           ios_base::iostate& __err, unsigned long long& __v) const
    { return _M_extract_int(__beg, __end, __io, __err, __v); }


  template<typename _CharT, typename _InIter>
    _InIter
    num_get<_CharT, _InIter>::
    do_get(iter_type __beg, iter_type __end, ios_base& __io,
    ios_base::iostate& __err, float& __v) const
    {
      string __xtrc;
      __xtrc.reserve(32);
      __beg = _M_extract_float(__beg, __end, __io, __err, __xtrc);
      std::__convert_to_v(__xtrc.c_str(), __v, __err, _S_get_c_locale());
      return __beg;
    }

  template<typename _CharT, typename _InIter>
    _InIter
    num_get<_CharT, _InIter>::
    do_get(iter_type __beg, iter_type __end, ios_base& __io,
           ios_base::iostate& __err, double& __v) const
    {
      string __xtrc;
      __xtrc.reserve(32);
      __beg = _M_extract_float(__beg, __end, __io, __err, __xtrc);
      std::__convert_to_v(__xtrc.c_str(), __v, __err, _S_get_c_locale());
      return __beg;
    }
# 864 "/usr/include/c++/4.2.1/bits/locale_facets.tcc" 3
  template<typename _CharT, typename _InIter>
    _InIter
    num_get<_CharT, _InIter>::
    do_get(iter_type __beg, iter_type __end, ios_base& __io,
           ios_base::iostate& __err, long double& __v) const
    {
      string __xtrc;
      __xtrc.reserve(32);
      __beg = _M_extract_float(__beg, __end, __io, __err, __xtrc);
      std::__convert_to_v(__xtrc.c_str(), __v, __err, _S_get_c_locale());
      return __beg;
    }

  template<typename _CharT, typename _InIter>
    _InIter
    num_get<_CharT, _InIter>::
    do_get(iter_type __beg, iter_type __end, ios_base& __io,
           ios_base::iostate& __err, void*& __v) const
    {

      typedef ios_base::fmtflags fmtflags;
      const fmtflags __fmt = __io.flags();
      __io.flags(__fmt & ~ios_base::basefield | ios_base::hex);

      unsigned long __ul;
      __beg = _M_extract_int(__beg, __end, __io, __err, __ul);


      __io.flags(__fmt);

      if (!(__err & ios_base::failbit))
 __v = reinterpret_cast<void*>(__ul);
      return __beg;
    }



  template<typename _CharT, typename _OutIter>
    void
    num_put<_CharT, _OutIter>::
    _M_pad(_CharT __fill, streamsize __w, ios_base& __io,
    _CharT* __new, const _CharT* __cs, int& __len) const
    {


      __pad<_CharT, char_traits<_CharT> >::_S_pad(__io, __fill, __new, __cs,
        __w, __len, true);
      __len = static_cast<int>(__w);
    }



  template<typename _CharT, typename _ValueT>
    int
    __int_to_char(_CharT* __bufend, _ValueT __v, const _CharT* __lit,
    ios_base::fmtflags __flags, bool __dec)
    {
      _CharT* __buf = __bufend;
      if (__builtin_expect(__dec, true))
 {

   do
     {
       *--__buf = __lit[(__v % 10) + __num_base::_S_odigits];
       __v /= 10;
     }
   while (__v != 0);
 }
      else if ((__flags & ios_base::basefield) == ios_base::oct)
 {

   do
     {
       *--__buf = __lit[(__v & 0x7) + __num_base::_S_odigits];
       __v >>= 3;
     }
   while (__v != 0);
 }
      else
 {

   const bool __uppercase = __flags & ios_base::uppercase;
   const int __case_offset = __uppercase ? __num_base::_S_oudigits
                                         : __num_base::_S_odigits;
   do
     {
       *--__buf = __lit[(__v & 0xf) + __case_offset];
       __v >>= 4;
     }
   while (__v != 0);
 }
      return __bufend - __buf;
    }



  template<typename _CharT, typename _OutIter>
    void
    num_put<_CharT, _OutIter>::
    _M_group_int(const char* __grouping, size_t __grouping_size, _CharT __sep,
   ios_base&, _CharT* __new, _CharT* __cs, int& __len) const
    {
      _CharT* __p = std::__add_grouping(__new, __sep, __grouping,
     __grouping_size, __cs, __cs + __len);
      __len = __p - __new;
    }

  template<typename _CharT, typename _OutIter>
    template<typename _ValueT>
      _OutIter
      num_put<_CharT, _OutIter>::
      _M_insert_int(_OutIter __s, ios_base& __io, _CharT __fill,
      _ValueT __v) const
      {
 using __gnu_cxx::__add_unsigned;
 typedef typename __add_unsigned<_ValueT>::__type __unsigned_type;
 typedef __numpunct_cache<_CharT> __cache_type;
 __use_cache<__cache_type> __uc;
 const locale& __loc = __io._M_getloc();
 const __cache_type* __lc = __uc(__loc);
 const _CharT* __lit = __lc->_M_atoms_out;
 const ios_base::fmtflags __flags = __io.flags();


 const int __ilen = 5 * sizeof(_ValueT);
 _CharT* __cs = static_cast<_CharT*>(__builtin_alloca(sizeof(_CharT)
            * __ilen));



 const ios_base::fmtflags __basefield = __flags & ios_base::basefield;
 const bool __dec = (__basefield != ios_base::oct
       && __basefield != ios_base::hex);
 const __unsigned_type __u = (__v > 0 || !__dec) ? __v : -__v;
  int __len = __int_to_char(__cs + __ilen, __u, __lit, __flags, __dec);
 __cs += __ilen - __len;


 if (__lc->_M_use_grouping)
   {


     _CharT* __cs2 = static_cast<_CharT*>(__builtin_alloca(sizeof(_CharT)
          * (__len + 1)
          * 2));
     _M_group_int(__lc->_M_grouping, __lc->_M_grouping_size,
    __lc->_M_thousands_sep, __io, __cs2 + 2, __cs, __len);
     __cs = __cs2 + 2;
   }


 if (__builtin_expect(__dec, true))
   {

     if (__v > 0)
       {
  if (__flags & ios_base::showpos
      && numeric_limits<_ValueT>::is_signed)
    *--__cs = __lit[__num_base::_S_oplus], ++__len;
       }
     else if (__v)
       *--__cs = __lit[__num_base::_S_ominus], ++__len;
   }
 else if (__flags & ios_base::showbase && __v)
   {
     if (__basefield == ios_base::oct)
       *--__cs = __lit[__num_base::_S_odigits], ++__len;
     else
       {

  const bool __uppercase = __flags & ios_base::uppercase;
  *--__cs = __lit[__num_base::_S_ox + __uppercase];

  *--__cs = __lit[__num_base::_S_odigits];
  __len += 2;
       }
   }


 const streamsize __w = __io.width();
 if (__w > static_cast<streamsize>(__len))
   {
     _CharT* __cs3 = static_cast<_CharT*>(__builtin_alloca(sizeof(_CharT)
          * __w));
     _M_pad(__fill, __w, __io, __cs3, __cs, __len);
     __cs = __cs3;
   }
 __io.width(0);



 return std::__write(__s, __cs, __len);
      }

  template<typename _CharT, typename _OutIter>
    void
    num_put<_CharT, _OutIter>::
    _M_group_float(const char* __grouping, size_t __grouping_size,
     _CharT __sep, const _CharT* __p, _CharT* __new,
     _CharT* __cs, int& __len) const
    {



      const int __declen = __p ? __p - __cs : __len;
      _CharT* __p2 = std::__add_grouping(__new, __sep, __grouping,
      __grouping_size,
      __cs, __cs + __declen);


      int __newlen = __p2 - __new;
      if (__p)
 {
   char_traits<_CharT>::copy(__p2, __p, __len - __declen);
   __newlen += __len - __declen;
 }
      __len = __newlen;
    }
# 1093 "/usr/include/c++/4.2.1/bits/locale_facets.tcc" 3
  template<typename _CharT, typename _OutIter>
    template<typename _ValueT>
      _OutIter
      num_put<_CharT, _OutIter>::
      _M_insert_float(_OutIter __s, ios_base& __io, _CharT __fill, char __mod,
         _ValueT __v) const
      {
 typedef __numpunct_cache<_CharT> __cache_type;
 __use_cache<__cache_type> __uc;
 const locale& __loc = __io._M_getloc();
 const __cache_type* __lc = __uc(__loc);


 const streamsize __prec = __io.precision() < 0 ? 6 : __io.precision();

 const int __max_digits = numeric_limits<_ValueT>::digits10;


 int __len;

 char __fbuf[16];
 __num_base::_S_format_float(__io, __fbuf, __mod);




 int __cs_size = __max_digits * 3;
 char* __cs = static_cast<char*>(__builtin_alloca(__cs_size));
 __len = std::__convert_from_v(_S_get_c_locale(), __cs, __cs_size,
          __fbuf, __prec, __v);


 if (__len >= __cs_size)
   {
     __cs_size = __len + 1;
     __cs = static_cast<char*>(__builtin_alloca(__cs_size));
     __len = std::__convert_from_v(_S_get_c_locale(), __cs, __cs_size,
       __fbuf, __prec, __v);
   }
# 1152 "/usr/include/c++/4.2.1/bits/locale_facets.tcc" 3
 const ctype<_CharT>& __ctype = use_facet<ctype<_CharT> >(__loc);

 _CharT* __ws = static_cast<_CharT*>(__builtin_alloca(sizeof(_CharT)
            * __len));
 __ctype.widen(__cs, __cs + __len, __ws);


 _CharT* __wp = 0;
 const char* __p = char_traits<char>::find(__cs, __len, '.');
 if (__p)
   {
     __wp = __ws + (__p - __cs);
     *__wp = __lc->_M_decimal_point;
   }




 if (__lc->_M_use_grouping
     && (__wp || __len < 3 || (__cs[1] <= '9' && __cs[2] <= '9'
          && __cs[1] >= '0' && __cs[2] >= '0')))
   {


     _CharT* __ws2 = static_cast<_CharT*>(__builtin_alloca(sizeof(_CharT)
          * __len * 2));

     streamsize __off = 0;
     if (__cs[0] == '-' || __cs[0] == '+')
       {
  __off = 1;
  __ws2[0] = __ws[0];
  __len -= 1;
       }

     _M_group_float(__lc->_M_grouping, __lc->_M_grouping_size,
      __lc->_M_thousands_sep, __wp, __ws2 + __off,
      __ws + __off, __len);
     __len += __off;

     __ws = __ws2;
   }


 const streamsize __w = __io.width();
 if (__w > static_cast<streamsize>(__len))
   {
     _CharT* __ws3 = static_cast<_CharT*>(__builtin_alloca(sizeof(_CharT)
          * __w));
     _M_pad(__fill, __w, __io, __ws3, __ws, __len);
     __ws = __ws3;
   }
 __io.width(0);



 return std::__write(__s, __ws, __len);
      }

  template<typename _CharT, typename _OutIter>
    _OutIter
    num_put<_CharT, _OutIter>::
    do_put(iter_type __s, ios_base& __io, char_type __fill, bool __v) const
    {
      const ios_base::fmtflags __flags = __io.flags();
      if ((__flags & ios_base::boolalpha) == 0)
        {
          const long __l = __v;
          __s = _M_insert_int(__s, __io, __fill, __l);
        }
      else
        {
   typedef __numpunct_cache<_CharT> __cache_type;
   __use_cache<__cache_type> __uc;
   const locale& __loc = __io._M_getloc();
   const __cache_type* __lc = __uc(__loc);

   const _CharT* __name = __v ? __lc->_M_truename
                              : __lc->_M_falsename;
   int __len = __v ? __lc->_M_truename_size
                   : __lc->_M_falsename_size;

   const streamsize __w = __io.width();
   if (__w > static_cast<streamsize>(__len))
     {
       _CharT* __cs
  = static_cast<_CharT*>(__builtin_alloca(sizeof(_CharT)
       * __w));
       _M_pad(__fill, __w, __io, __cs, __name, __len);
       __name = __cs;
     }
   __io.width(0);
   __s = std::__write(__s, __name, __len);
 }
      return __s;
    }

  template<typename _CharT, typename _OutIter>
    _OutIter
    num_put<_CharT, _OutIter>::
    do_put(iter_type __s, ios_base& __io, char_type __fill, long __v) const
    { return _M_insert_int(__s, __io, __fill, __v); }

  template<typename _CharT, typename _OutIter>
    _OutIter
    num_put<_CharT, _OutIter>::
    do_put(iter_type __s, ios_base& __io, char_type __fill,
           unsigned long __v) const
    { return _M_insert_int(__s, __io, __fill, __v); }


  template<typename _CharT, typename _OutIter>
    _OutIter
    num_put<_CharT, _OutIter>::
    do_put(iter_type __s, ios_base& __io, char_type __fill, long long __v) const
    { return _M_insert_int(__s, __io, __fill, __v); }

  template<typename _CharT, typename _OutIter>
    _OutIter
    num_put<_CharT, _OutIter>::
    do_put(iter_type __s, ios_base& __io, char_type __fill,
           unsigned long long __v) const
    { return _M_insert_int(__s, __io, __fill, __v); }


  template<typename _CharT, typename _OutIter>
    _OutIter
    num_put<_CharT, _OutIter>::
    do_put(iter_type __s, ios_base& __io, char_type __fill, double __v) const
    { return _M_insert_float(__s, __io, __fill, char(), __v); }
# 1291 "/usr/include/c++/4.2.1/bits/locale_facets.tcc" 3
  template<typename _CharT, typename _OutIter>
    _OutIter
    num_put<_CharT, _OutIter>::
    do_put(iter_type __s, ios_base& __io, char_type __fill,
    long double __v) const
    { return _M_insert_float(__s, __io, __fill, 'L', __v); }

  template<typename _CharT, typename _OutIter>
    _OutIter
    num_put<_CharT, _OutIter>::
    do_put(iter_type __s, ios_base& __io, char_type __fill,
           const void* __v) const
    {
      const ios_base::fmtflags __flags = __io.flags();
      const ios_base::fmtflags __fmt = ~(ios_base::basefield
      | ios_base::uppercase
      | ios_base::internal);
      __io.flags(__flags & __fmt | (ios_base::hex | ios_base::showbase));

      __s = _M_insert_int(__s, __io, __fill,
     reinterpret_cast<unsigned long>(__v));
      __io.flags(__flags);
      return __s;
    }

  template<typename _CharT, typename _InIter>
    template<bool _Intl>
      _InIter
      money_get<_CharT, _InIter>::
      _M_extract(iter_type __beg, iter_type __end, ios_base& __io,
   ios_base::iostate& __err, string& __units) const
      {
 typedef char_traits<_CharT> __traits_type;
 typedef typename string_type::size_type size_type;
 typedef money_base::part part;
 typedef __moneypunct_cache<_CharT, _Intl> __cache_type;

 const locale& __loc = __io._M_getloc();
 const ctype<_CharT>& __ctype = use_facet<ctype<_CharT> >(__loc);

 __use_cache<__cache_type> __uc;
 const __cache_type* __lc = __uc(__loc);
 const char_type* __lit = __lc->_M_atoms;


 bool __negative = false;

 size_type __sign_size = 0;

 const bool __mandatory_sign = (__lc->_M_positive_sign_size
           && __lc->_M_negative_sign_size);

 string __grouping_tmp;
 if (__lc->_M_use_grouping)
   __grouping_tmp.reserve(32);

 int __last_pos = 0;

 int __n = 0;

 bool __testvalid = true;

 bool __testdecfound = false;


 string __res;
 __res.reserve(32);

 const char_type* __lit_zero = __lit + money_base::_S_zero;
 const money_base::pattern __p = __lc->_M_neg_format;
 for (int __i = 0; __i < 4 && __testvalid; ++__i)
   {
     const part __which = static_cast<part>(__p.field[__i]);
     switch (__which)
       {
       case money_base::symbol:




  if (__io.flags() & ios_base::showbase || __sign_size > 1
      || __i == 0
      || (__i == 1 && (__mandatory_sign
         || (static_cast<part>(__p.field[0])
      == money_base::sign)
         || (static_cast<part>(__p.field[2])
      == money_base::space)))
      || (__i == 2 && ((static_cast<part>(__p.field[3])
          == money_base::value)
         || __mandatory_sign
         && (static_cast<part>(__p.field[3])
      == money_base::sign))))
    {
      const size_type __len = __lc->_M_curr_symbol_size;
      size_type __j = 0;
      for (; __beg != __end && __j < __len
      && *__beg == __lc->_M_curr_symbol[__j];
    ++__beg, ++__j);
      if (__j != __len
   && (__j || __io.flags() & ios_base::showbase))
        __testvalid = false;
    }
  break;
       case money_base::sign:

  if (__lc->_M_positive_sign_size && __beg != __end
      && *__beg == __lc->_M_positive_sign[0])
    {
      __sign_size = __lc->_M_positive_sign_size;
      ++__beg;
    }
  else if (__lc->_M_negative_sign_size && __beg != __end
    && *__beg == __lc->_M_negative_sign[0])
    {
      __negative = true;
      __sign_size = __lc->_M_negative_sign_size;
      ++__beg;
    }
  else if (__lc->_M_positive_sign_size
    && !__lc->_M_negative_sign_size)


    __negative = true;
  else if (__mandatory_sign)
    __testvalid = false;
  break;
       case money_base::value:


  for (; __beg != __end; ++__beg)
    {
      const char_type __c = *__beg;
      const char_type* __q = __traits_type::find(__lit_zero,
              10, __c);
      if (__q != 0)
        {
   __res += money_base::_S_atoms[__q - __lit];
   ++__n;
        }
      else if (__c == __lc->_M_decimal_point
        && !__testdecfound)
        {
   __last_pos = __n;
   __n = 0;
   __testdecfound = true;
        }
      else if (__lc->_M_use_grouping
        && __c == __lc->_M_thousands_sep
        && !__testdecfound)
        {
   if (__n)
     {

       __grouping_tmp += static_cast<char>(__n);
       __n = 0;
     }
   else
     {
       __testvalid = false;
       break;
     }
        }
      else
        break;
    }
  if (__res.empty())
    __testvalid = false;
  break;
       case money_base::space:

  if (__beg != __end && __ctype.is(ctype_base::space, *__beg))
    ++__beg;
  else
    __testvalid = false;
       case money_base::none:

  if (__i != 3)
    for (; __beg != __end
    && __ctype.is(ctype_base::space, *__beg); ++__beg);
  break;
       }
   }


 if (__sign_size > 1 && __testvalid)
   {
     const char_type* __sign = __negative ? __lc->_M_negative_sign
                                          : __lc->_M_positive_sign;
     size_type __i = 1;
     for (; __beg != __end && __i < __sign_size
     && *__beg == __sign[__i]; ++__beg, ++__i);

     if (__i != __sign_size)
       __testvalid = false;
   }

 if (__testvalid)
   {

     if (__res.size() > 1)
       {
  const size_type __first = __res.find_first_not_of('0');
  const bool __only_zeros = __first == string::npos;
  if (__first)
    __res.erase(0, __only_zeros ? __res.size() - 1 : __first);
       }


     if (__negative && __res[0] != '0')
       __res.insert(__res.begin(), '-');


     if (__grouping_tmp.size())
       {

  __grouping_tmp += static_cast<char>(__testdecfound ? __last_pos
                         : __n);
  if (!std::__verify_grouping(__lc->_M_grouping,
         __lc->_M_grouping_size,
         __grouping_tmp))
    __err |= ios_base::failbit;
       }


     if (__testdecfound && __lc->_M_frac_digits > 0
  && __n != __lc->_M_frac_digits)
       __testvalid = false;
   }


 if (!__testvalid)
   __err |= ios_base::failbit;
 else
   __units.swap(__res);


 if (__beg == __end)
   __err |= ios_base::eofbit;
 return __beg;
      }
# 1547 "/usr/include/c++/4.2.1/bits/locale_facets.tcc" 3
  template<typename _CharT, typename _InIter>
    _InIter
    money_get<_CharT, _InIter>::
    do_get(iter_type __beg, iter_type __end, bool __intl, ios_base& __io,
    ios_base::iostate& __err, long double& __units) const
    {
      string __str;
      __beg = __intl ? _M_extract<true>(__beg, __end, __io, __err, __str)
              : _M_extract<false>(__beg, __end, __io, __err, __str);
      std::__convert_to_v(__str.c_str(), __units, __err, _S_get_c_locale());
      return __beg;
    }

  template<typename _CharT, typename _InIter>
    _InIter
    money_get<_CharT, _InIter>::
    do_get(iter_type __beg, iter_type __end, bool __intl, ios_base& __io,
    ios_base::iostate& __err, string_type& __digits) const
    {
      typedef typename string::size_type size_type;

      const locale& __loc = __io._M_getloc();
      const ctype<_CharT>& __ctype = use_facet<ctype<_CharT> >(__loc);

      string __str;
      __beg = __intl ? _M_extract<true>(__beg, __end, __io, __err, __str)
              : _M_extract<false>(__beg, __end, __io, __err, __str);
      const size_type __len = __str.size();
      if (__len)
 {
   __digits.resize(__len);
   __ctype.widen(__str.data(), __str.data() + __len, &__digits[0]);
 }
      return __beg;
    }

  template<typename _CharT, typename _OutIter>
    template<bool _Intl>
      _OutIter
      money_put<_CharT, _OutIter>::
      _M_insert(iter_type __s, ios_base& __io, char_type __fill,
  const string_type& __digits) const
      {
 typedef typename string_type::size_type size_type;
 typedef money_base::part part;
 typedef __moneypunct_cache<_CharT, _Intl> __cache_type;

 const locale& __loc = __io._M_getloc();
 const ctype<_CharT>& __ctype = use_facet<ctype<_CharT> >(__loc);

 __use_cache<__cache_type> __uc;
 const __cache_type* __lc = __uc(__loc);
 const char_type* __lit = __lc->_M_atoms;



 const char_type* __beg = __digits.data();

 money_base::pattern __p;
 const char_type* __sign;
 size_type __sign_size;
 if (!(*__beg == __lit[money_base::_S_minus]))
   {
     __p = __lc->_M_pos_format;
     __sign = __lc->_M_positive_sign;
     __sign_size = __lc->_M_positive_sign_size;
   }
 else
   {
     __p = __lc->_M_neg_format;
     __sign = __lc->_M_negative_sign;
     __sign_size = __lc->_M_negative_sign_size;
     if (__digits.size())
       ++__beg;
   }


 size_type __len = __ctype.scan_not(ctype_base::digit, __beg,
        __beg + __digits.size()) - __beg;
 if (__len)
   {



     string_type __value;
     __value.reserve(2 * __len);



     long __paddec = __len - __lc->_M_frac_digits;
     if (__paddec > 0)
         {
  if (__lc->_M_frac_digits < 0)
    __paddec = __len;
    if (__lc->_M_grouping_size)
      {
      __value.assign(2 * __paddec, char_type());
       _CharT* __vend =
        std::__add_grouping(&__value[0], __lc->_M_thousands_sep,
       __lc->_M_grouping,
       __lc->_M_grouping_size,
       __beg, __beg + __paddec);
      __value.erase(__vend - &__value[0]);
      }
    else
    __value.assign(__beg, __paddec);
       }


     if (__lc->_M_frac_digits > 0)
       {
  __value += __lc->_M_decimal_point;
  if (__paddec >= 0)
    __value.append(__beg + __paddec, __lc->_M_frac_digits);
  else
    {

      __value.append(-__paddec, __lit[money_base::_S_zero]);
      __value.append(__beg, __len);
    }
         }


     const ios_base::fmtflags __f = __io.flags()
                                    & ios_base::adjustfield;
     __len = __value.size() + __sign_size;
     __len += ((__io.flags() & ios_base::showbase)
        ? __lc->_M_curr_symbol_size : 0);

     string_type __res;
     __res.reserve(2 * __len);

     const size_type __width = static_cast<size_type>(__io.width());
     const bool __testipad = (__f == ios_base::internal
         && __len < __width);

     for (int __i = 0; __i < 4; ++__i)
       {
  const part __which = static_cast<part>(__p.field[__i]);
  switch (__which)
    {
    case money_base::symbol:
      if (__io.flags() & ios_base::showbase)
        __res.append(__lc->_M_curr_symbol,
       __lc->_M_curr_symbol_size);
      break;
    case money_base::sign:



      if (__sign_size)
        __res += __sign[0];
      break;
    case money_base::value:
      __res += __value;
      break;
    case money_base::space:



      if (__testipad)
        __res.append(__width - __len, __fill);
      else
        __res += __fill;
      break;
    case money_base::none:
      if (__testipad)
        __res.append(__width - __len, __fill);
      break;
    }
       }


     if (__sign_size > 1)
       __res.append(__sign + 1, __sign_size - 1);


     __len = __res.size();
     if (__width > __len)
       {
  if (__f == ios_base::left)

    __res.append(__width - __len, __fill);
  else

    __res.insert(0, __width - __len, __fill);
  __len = __width;
       }


     __s = std::__write(__s, __res.data(), __len);
   }
 __io.width(0);
 return __s;
      }
# 1752 "/usr/include/c++/4.2.1/bits/locale_facets.tcc" 3
  template<typename _CharT, typename _OutIter>
    _OutIter
    money_put<_CharT, _OutIter>::
    do_put(iter_type __s, bool __intl, ios_base& __io, char_type __fill,
    long double __units) const
    {
      const locale __loc = __io.getloc();
      const ctype<_CharT>& __ctype = use_facet<ctype<_CharT> >(__loc);


      int __cs_size = 64;
      char* __cs = static_cast<char*>(__builtin_alloca(__cs_size));


      int __len = std::__convert_from_v(_S_get_c_locale(), __cs, __cs_size,
     "%.*Lf", 0, __units);

      if (__len >= __cs_size)
 {
   __cs_size = __len + 1;
   __cs = static_cast<char*>(__builtin_alloca(__cs_size));
   __len = std::__convert_from_v(_S_get_c_locale(), __cs, __cs_size,
     "%.*Lf", 0, __units);
 }







      string_type __digits(__len, char_type());
      __ctype.widen(__cs, __cs + __len, &__digits[0]);
      return __intl ? _M_insert<true>(__s, __io, __fill, __digits)
             : _M_insert<false>(__s, __io, __fill, __digits);
    }

  template<typename _CharT, typename _OutIter>
    _OutIter
    money_put<_CharT, _OutIter>::
    do_put(iter_type __s, bool __intl, ios_base& __io, char_type __fill,
    const string_type& __digits) const
    { return __intl ? _M_insert<true>(__s, __io, __fill, __digits)
             : _M_insert<false>(__s, __io, __fill, __digits); }






  template<typename _CharT, typename _InIter>
    time_base::dateorder
    time_get<_CharT, _InIter>::do_date_order() const
    { return time_base::no_order; }



  template<typename _CharT, typename _InIter>
    _InIter
    time_get<_CharT, _InIter>::
    _M_extract_via_format(iter_type __beg, iter_type __end, ios_base& __io,
     ios_base::iostate& __err, tm* __tm,
     const _CharT* __format) const
    {
      const locale& __loc = __io._M_getloc();
      const __timepunct<_CharT>& __tp = use_facet<__timepunct<_CharT> >(__loc);
      const ctype<_CharT>& __ctype = use_facet<ctype<_CharT> >(__loc);
      const size_t __len = char_traits<_CharT>::length(__format);

      ios_base::iostate __tmperr = ios_base::goodbit;
      for (size_t __i = 0; __beg != __end && __i < __len && !__tmperr; ++__i)
 {
   if (__ctype.narrow(__format[__i], 0) == '%')
     {

       char __c = __ctype.narrow(__format[++__i], 0);
       int __mem = 0;
       if (__c == 'E' || __c == 'O')
  __c = __ctype.narrow(__format[++__i], 0);
       switch (__c)
  {
    const char* __cs;
    _CharT __wcs[10];
  case 'a':

    const char_type* __days1[7];
    __tp._M_days_abbreviated(__days1);
    __beg = _M_extract_name(__beg, __end, __tm->tm_wday, __days1,
       7, __io, __tmperr);
    break;
  case 'A':

    const char_type* __days2[7];
    __tp._M_days(__days2);
    __beg = _M_extract_name(__beg, __end, __tm->tm_wday, __days2,
       7, __io, __tmperr);
    break;
  case 'h':
  case 'b':

    const char_type* __months1[12];
    __tp._M_months_abbreviated(__months1);
    __beg = _M_extract_name(__beg, __end, __tm->tm_mon,
       __months1, 12, __io, __tmperr);
    break;
  case 'B':

    const char_type* __months2[12];
    __tp._M_months(__months2);
    __beg = _M_extract_name(__beg, __end, __tm->tm_mon,
       __months2, 12, __io, __tmperr);
    break;
  case 'c':

    const char_type* __dt[2];
    __tp._M_date_time_formats(__dt);
    __beg = _M_extract_via_format(__beg, __end, __io, __tmperr,
      __tm, __dt[0]);
    break;
  case 'd':

    __beg = _M_extract_num(__beg, __end, __tm->tm_mday, 1, 31, 2,
      __io, __tmperr);
    break;
  case 'e':


    if (__ctype.is(ctype_base::space, *__beg))
      __beg = _M_extract_num(++__beg, __end, __tm->tm_mday, 1, 9,
        1, __io, __tmperr);
    else
      __beg = _M_extract_num(__beg, __end, __tm->tm_mday, 10, 31,
        2, __io, __tmperr);
    break;
  case 'D':

    __cs = "%m/%d/%y";
    __ctype.widen(__cs, __cs + 9, __wcs);
    __beg = _M_extract_via_format(__beg, __end, __io, __tmperr,
      __tm, __wcs);
    break;
  case 'H':

    __beg = _M_extract_num(__beg, __end, __tm->tm_hour, 0, 23, 2,
      __io, __tmperr);
    break;
  case 'I':

    __beg = _M_extract_num(__beg, __end, __tm->tm_hour, 1, 12, 2,
      __io, __tmperr);
    break;
  case 'm':

    __beg = _M_extract_num(__beg, __end, __mem, 1, 12, 2,
      __io, __tmperr);
    if (!__tmperr)
      __tm->tm_mon = __mem - 1;
    break;
  case 'M':

    __beg = _M_extract_num(__beg, __end, __tm->tm_min, 0, 59, 2,
      __io, __tmperr);
    break;
  case 'n':
    if (__ctype.narrow(*__beg, 0) == '\n')
      ++__beg;
    else
      __tmperr |= ios_base::failbit;
    break;
  case 'R':

    __cs = "%H:%M";
    __ctype.widen(__cs, __cs + 6, __wcs);
    __beg = _M_extract_via_format(__beg, __end, __io, __tmperr,
      __tm, __wcs);
    break;
  case 'S':



    __beg = _M_extract_num(__beg, __end, __tm->tm_sec, 0, 60, 2,



      __io, __tmperr);
    break;
  case 't':
    if (__ctype.narrow(*__beg, 0) == '\t')
      ++__beg;
    else
      __tmperr |= ios_base::failbit;
    break;
  case 'T':

    __cs = "%H:%M:%S";
    __ctype.widen(__cs, __cs + 9, __wcs);
    __beg = _M_extract_via_format(__beg, __end, __io, __tmperr,
      __tm, __wcs);
    break;
  case 'x':

    const char_type* __dates[2];
    __tp._M_date_formats(__dates);
    __beg = _M_extract_via_format(__beg, __end, __io, __tmperr,
      __tm, __dates[0]);
    break;
  case 'X':

    const char_type* __times[2];
    __tp._M_time_formats(__times);
    __beg = _M_extract_via_format(__beg, __end, __io, __tmperr,
      __tm, __times[0]);
    break;
  case 'y':
  case 'C':

    __beg = _M_extract_num(__beg, __end, __tm->tm_year, 0, 99, 2,
      __io, __tmperr);
    break;
  case 'Y':

    __beg = _M_extract_num(__beg, __end, __mem, 0, 9999, 4,
      __io, __tmperr);
    if (!__tmperr)
      __tm->tm_year = __mem - 1900;
    break;
  case 'Z':

    if (__ctype.is(ctype_base::upper, *__beg))
      {
        int __tmp;
        __beg = _M_extract_name(__beg, __end, __tmp,
           __timepunct_cache<_CharT>::_S_timezones,
           14, __io, __tmperr);


        if (__beg != __end && !__tmperr && __tmp == 0
     && (*__beg == __ctype.widen('-')
         || *__beg == __ctype.widen('+')))
   {
     __beg = _M_extract_num(__beg, __end, __tmp, 0, 23, 2,
       __io, __tmperr);
     __beg = _M_extract_num(__beg, __end, __tmp, 0, 59, 2,
       __io, __tmperr);
   }
      }
    else
      __tmperr |= ios_base::failbit;
    break;
  default:

    __tmperr |= ios_base::failbit;
  }
     }
   else
     {

       if (__format[__i] == *__beg)
  ++__beg;
       else
  __tmperr |= ios_base::failbit;
     }
 }

      if (__tmperr)
 __err |= ios_base::failbit;

      return __beg;
    }

  template<typename _CharT, typename _InIter>
    _InIter
    time_get<_CharT, _InIter>::
    _M_extract_num(iter_type __beg, iter_type __end, int& __member,
     int __min, int __max, size_t __len,
     ios_base& __io, ios_base::iostate& __err) const
    {
      const locale& __loc = __io._M_getloc();
      const ctype<_CharT>& __ctype = use_facet<ctype<_CharT> >(__loc);


      int __mult = __len == 2 ? 10 : (__len == 4 ? 1000 : 1);

      ++__min;
      size_t __i = 0;
      int __value = 0;
      for (; __beg != __end && __i < __len; ++__beg, ++__i)
 {
   const char __c = __ctype.narrow(*__beg, '*');
   if (__c >= '0' && __c <= '9')
     {
       __value = __value * 10 + (__c - '0');
       const int __valuec = __value * __mult;
       if (__valuec > __max || __valuec + __mult < __min)
  break;
       __mult /= 10;
     }
   else
     break;
 }
      if (__i == __len)
 __member = __value;
      else
 __err |= ios_base::failbit;

      return __beg;
    }



  template<typename _CharT, typename _InIter>
    _InIter
    time_get<_CharT, _InIter>::
    _M_extract_name(iter_type __beg, iter_type __end, int& __member,
      const _CharT** __names, size_t __indexlen,
      ios_base& __io, ios_base::iostate& __err) const
    {
      typedef char_traits<_CharT> __traits_type;
      const locale& __loc = __io._M_getloc();
      const ctype<_CharT>& __ctype = use_facet<ctype<_CharT> >(__loc);

      int* __matches = static_cast<int*>(__builtin_alloca(sizeof(int)
         * __indexlen));
      size_t __nmatches = 0;
      size_t __pos = 0;
      bool __testvalid = true;
      const char_type* __name;





      if (__beg != __end)
 {
   const char_type __c = *__beg;
   for (size_t __i1 = 0; __i1 < __indexlen; ++__i1)
     if (__c == __names[__i1][0]
  || __c == __ctype.toupper(__names[__i1][0]))
       __matches[__nmatches++] = __i1;
 }

      while (__nmatches > 1)
 {

   size_t __minlen = __traits_type::length(__names[__matches[0]]);
   for (size_t __i2 = 1; __i2 < __nmatches; ++__i2)
     __minlen = std::min(__minlen,
         __traits_type::length(__names[__matches[__i2]]));
   ++__beg, ++__pos;
   if (__pos < __minlen && __beg != __end)
     for (size_t __i3 = 0; __i3 < __nmatches;)
       {
  __name = __names[__matches[__i3]];
  if (!(__name[__pos] == *__beg))
    __matches[__i3] = __matches[--__nmatches];
  else
    ++__i3;
       }
   else
     break;
 }

      if (__nmatches == 1)
 {

   ++__beg, ++__pos;
   __name = __names[__matches[0]];
   const size_t __len = __traits_type::length(__name);
   while (__pos < __len && __beg != __end && __name[__pos] == *__beg)
     ++__beg, ++__pos;

   if (__len == __pos)
     __member = __matches[0];
   else
     __testvalid = false;
 }
      else
 __testvalid = false;
      if (!__testvalid)
 __err |= ios_base::failbit;

      return __beg;
    }

  template<typename _CharT, typename _InIter>
    _InIter
    time_get<_CharT, _InIter>::
    do_get_time(iter_type __beg, iter_type __end, ios_base& __io,
  ios_base::iostate& __err, tm* __tm) const
    {
      const locale& __loc = __io._M_getloc();
      const __timepunct<_CharT>& __tp = use_facet<__timepunct<_CharT> >(__loc);
      const char_type* __times[2];
      __tp._M_time_formats(__times);
      __beg = _M_extract_via_format(__beg, __end, __io, __err,
        __tm, __times[0]);
      if (__beg == __end)
 __err |= ios_base::eofbit;
      return __beg;
    }

  template<typename _CharT, typename _InIter>
    _InIter
    time_get<_CharT, _InIter>::
    do_get_date(iter_type __beg, iter_type __end, ios_base& __io,
  ios_base::iostate& __err, tm* __tm) const
    {
      const locale& __loc = __io._M_getloc();
      const __timepunct<_CharT>& __tp = use_facet<__timepunct<_CharT> >(__loc);
      const char_type* __dates[2];
      __tp._M_date_formats(__dates);
      __beg = _M_extract_via_format(__beg, __end, __io, __err,
        __tm, __dates[0]);
      if (__beg == __end)
 __err |= ios_base::eofbit;
      return __beg;
    }

  template<typename _CharT, typename _InIter>
    _InIter
    time_get<_CharT, _InIter>::
    do_get_weekday(iter_type __beg, iter_type __end, ios_base& __io,
     ios_base::iostate& __err, tm* __tm) const
    {
      typedef char_traits<_CharT> __traits_type;
      const locale& __loc = __io._M_getloc();
      const __timepunct<_CharT>& __tp = use_facet<__timepunct<_CharT> >(__loc);
      const ctype<_CharT>& __ctype = use_facet<ctype<_CharT> >(__loc);
      const char_type* __days[7];
      __tp._M_days_abbreviated(__days);
      int __tmpwday;
      ios_base::iostate __tmperr = ios_base::goodbit;
      __beg = _M_extract_name(__beg, __end, __tmpwday, __days, 7,
         __io, __tmperr);







      if (!__tmperr && __beg != __end)
 {
   size_t __pos = __traits_type::length(__days[__tmpwday]);
   __tp._M_days(__days);
   const char_type* __name = __days[__tmpwday];
   if (__name[__pos] == *__beg)
     {

       const size_t __len = __traits_type::length(__name);
       while (__pos < __len && __beg != __end
       && __name[__pos] == *__beg)
  ++__beg, ++__pos;
       if (__len != __pos)
  __tmperr |= ios_base::failbit;
     }
 }
      if (!__tmperr)
 __tm->tm_wday = __tmpwday;
      else
 __err |= ios_base::failbit;

      if (__beg == __end)
 __err |= ios_base::eofbit;
      return __beg;
     }

  template<typename _CharT, typename _InIter>
    _InIter
    time_get<_CharT, _InIter>::
    do_get_monthname(iter_type __beg, iter_type __end,
                     ios_base& __io, ios_base::iostate& __err, tm* __tm) const
    {
      typedef char_traits<_CharT> __traits_type;
      const locale& __loc = __io._M_getloc();
      const __timepunct<_CharT>& __tp = use_facet<__timepunct<_CharT> >(__loc);
      const ctype<_CharT>& __ctype = use_facet<ctype<_CharT> >(__loc);
      const char_type* __months[12];
      __tp._M_months_abbreviated(__months);
      int __tmpmon;
      ios_base::iostate __tmperr = ios_base::goodbit;
      __beg = _M_extract_name(__beg, __end, __tmpmon, __months, 12,
         __io, __tmperr);







      if (!__tmperr && __beg != __end)
 {
   size_t __pos = __traits_type::length(__months[__tmpmon]);
   __tp._M_months(__months);
   const char_type* __name = __months[__tmpmon];
   if (__name[__pos] == *__beg)
     {

       const size_t __len = __traits_type::length(__name);
       while (__pos < __len && __beg != __end
       && __name[__pos] == *__beg)
  ++__beg, ++__pos;
       if (__len != __pos)
  __tmperr |= ios_base::failbit;
     }
 }
      if (!__tmperr)
 __tm->tm_mon = __tmpmon;
      else
 __err |= ios_base::failbit;

      if (__beg == __end)
 __err |= ios_base::eofbit;
      return __beg;
    }

  template<typename _CharT, typename _InIter>
    _InIter
    time_get<_CharT, _InIter>::
    do_get_year(iter_type __beg, iter_type __end, ios_base& __io,
  ios_base::iostate& __err, tm* __tm) const
    {
      const locale& __loc = __io._M_getloc();
      const ctype<_CharT>& __ctype = use_facet<ctype<_CharT> >(__loc);

      size_t __i = 0;
      int __value = 0;
      for (; __beg != __end && __i < 4; ++__beg, ++__i)
 {
   const char __c = __ctype.narrow(*__beg, '*');
   if (__c >= '0' && __c <= '9')
     __value = __value * 10 + (__c - '0');
   else
     break;
 }
      if (__i == 2 || __i == 4)
 __tm->tm_year = __i == 2 ? __value : __value - 1900;
      else
 __err |= ios_base::failbit;

      if (__beg == __end)
 __err |= ios_base::eofbit;
      return __beg;
    }

  template<typename _CharT, typename _OutIter>
    _OutIter
    time_put<_CharT, _OutIter>::
    put(iter_type __s, ios_base& __io, char_type __fill, const tm* __tm,
 const _CharT* __beg, const _CharT* __end) const
    {
      const locale& __loc = __io._M_getloc();
      ctype<_CharT> const& __ctype = use_facet<ctype<_CharT> >(__loc);
      for (; __beg != __end; ++__beg)
 if (__ctype.narrow(*__beg, 0) != '%')
   {
     *__s = *__beg;
     ++__s;
   }
 else if (++__beg != __end)
   {
     char __format;
     char __mod = 0;
     const char __c = __ctype.narrow(*__beg, 0);
     if (__c != 'E' && __c != 'O')
       __format = __c;
     else if (++__beg != __end)
       {
  __mod = __c;
  __format = __ctype.narrow(*__beg, 0);
       }
     else
       break;
     __s = this->do_put(__s, __io, __fill, __tm, __format, __mod);
   }
 else
   break;
      return __s;
    }

  template<typename _CharT, typename _OutIter>
    _OutIter
    time_put<_CharT, _OutIter>::
    do_put(iter_type __s, ios_base& __io, char_type, const tm* __tm,
    char __format, char __mod) const
    {
      const locale& __loc = __io._M_getloc();
      ctype<_CharT> const& __ctype = use_facet<ctype<_CharT> >(__loc);
      __timepunct<_CharT> const& __tp = use_facet<__timepunct<_CharT> >(__loc);



      const size_t __maxlen = 128;
      char_type* __res =
       static_cast<char_type*>(__builtin_alloca(sizeof(char_type) * __maxlen));






      char_type __fmt[4];
      __fmt[0] = __ctype.widen('%');
      if (!__mod)
 {
   __fmt[1] = __format;
   __fmt[2] = char_type();
 }
      else
 {
   __fmt[1] = __mod;
   __fmt[2] = __format;
   __fmt[3] = char_type();
 }

      __tp._M_put(__res, __maxlen, __fmt, __tm);


      return std::__write(__s, __res, char_traits<char_type>::length(__res));
    }


  template<typename _CharT>
    int
    collate<_CharT>::_M_compare(const _CharT*, const _CharT*) const
    { return 0; }


  template<typename _CharT>
    size_t
    collate<_CharT>::_M_transform(_CharT*, const _CharT*, size_t) const
    { return 0; }

  template<typename _CharT>
    int
    collate<_CharT>::
    do_compare(const _CharT* __lo1, const _CharT* __hi1,
        const _CharT* __lo2, const _CharT* __hi2) const
    {


      const string_type __one(__lo1, __hi1);
      const string_type __two(__lo2, __hi2);

      const _CharT* __p = __one.c_str();
      const _CharT* __pend = __one.data() + __one.length();
      const _CharT* __q = __two.c_str();
      const _CharT* __qend = __two.data() + __two.length();




      for (;;)
 {
   const int __res = _M_compare(__p, __q);
   if (__res)
     return __res;

   __p += char_traits<_CharT>::length(__p);
   __q += char_traits<_CharT>::length(__q);
   if (__p == __pend && __q == __qend)
     return 0;
   else if (__p == __pend)
     return -1;
   else if (__q == __qend)
     return 1;

   __p++;
   __q++;
 }
    }

  template<typename _CharT>
    typename collate<_CharT>::string_type
    collate<_CharT>::
    do_transform(const _CharT* __lo, const _CharT* __hi) const
    {
      string_type __ret;


      const string_type __str(__lo, __hi);

      const _CharT* __p = __str.c_str();
      const _CharT* __pend = __str.data() + __str.length();

      size_t __len = (__hi - __lo) * 2;

      _CharT* __c = new _CharT[__len];

      try
 {



   for (;;)
     {

       size_t __res = _M_transform(__c, __p, __len);


       if (__res >= __len)
  {
    __len = __res + 1;
    delete [] __c, __c = 0;
    __c = new _CharT[__len];
    __res = _M_transform(__c, __p, __len);
  }

       __ret.append(__c, __res);
       __p += char_traits<_CharT>::length(__p);
       if (__p == __pend)
  break;

       __p++;
       __ret.push_back(_CharT());
     }
 }
      catch(...)
 {
   delete [] __c;
   throw;
 }

      delete [] __c;

      return __ret;
    }

  template<typename _CharT>
    long
    collate<_CharT>::
    do_hash(const _CharT* __lo, const _CharT* __hi) const
    {
      unsigned long __val = 0;
      for (; __lo < __hi; ++__lo)
 __val = *__lo + ((__val << 7) |
         (__val >> (numeric_limits<unsigned long>::digits - 7)));
      return static_cast<long>(__val);
    }
# 2503 "/usr/include/c++/4.2.1/bits/locale_facets.tcc" 3
  template<typename _CharT, typename _Traits>
    void
    __pad<_CharT, _Traits>::_S_pad(ios_base& __io, _CharT __fill,
       _CharT* __news, const _CharT* __olds,
       const streamsize __newlen,
       const streamsize __oldlen, const bool __num)
    {
      const size_t __plen = static_cast<size_t>(__newlen - __oldlen);
      const ios_base::fmtflags __adjust = __io.flags() & ios_base::adjustfield;


      if (__adjust == ios_base::left)
 {
   _Traits::copy(__news, const_cast<_CharT*>(__olds), __oldlen);
   _Traits::assign(__news + __oldlen, __plen, __fill);
   return;
 }

      size_t __mod = 0;
      if (__adjust == ios_base::internal && __num)
 {



          const locale& __loc = __io._M_getloc();
   const ctype<_CharT>& __ctype = use_facet<ctype<_CharT> >(__loc);

   const bool __testsign = (__ctype.widen('-') == __olds[0]
       || __ctype.widen('+') == __olds[0]);
   const bool __testhex = (__ctype.widen('0') == __olds[0]
      && __oldlen > 1
      && (__ctype.widen('x') == __olds[1]
          || __ctype.widen('X') == __olds[1]));
   if (__testhex)
     {
       __news[0] = __olds[0];
       __news[1] = __olds[1];
       __mod = 2;
       __news += 2;
     }
   else if (__testsign)
     {
       __news[0] = __olds[0];
       __mod = 1;
       ++__news;
     }

 }
      _Traits::assign(__news, __plen, __fill);
      _Traits::copy(__news + __plen, const_cast<_CharT*>(__olds + __mod),
      __oldlen - __mod);
    }

  bool
  __verify_grouping(const char* __grouping, size_t __grouping_size,
      const string& __grouping_tmp)
  {
    const size_t __n = __grouping_tmp.size() - 1;
    const size_t __min = std::min(__n, size_t(__grouping_size - 1));
    size_t __i = __n;
    bool __test = true;




    for (size_t __j = 0; __j < __min && __test; --__i, ++__j)
      __test = __grouping_tmp[__i] == __grouping[__j];
    for (; __i && __test; --__i)
      __test = __grouping_tmp[__i] == __grouping[__min];



    if (static_cast<signed char>(__grouping[__min]) > 0)
      __test &= __grouping_tmp[0] <= __grouping[__min];
    return __test;
  }

  template<typename _CharT>
    _CharT*
    __add_grouping(_CharT* __s, _CharT __sep,
     const char* __gbeg, size_t __gsize,
     const _CharT* __first, const _CharT* __last)
    {
      size_t __idx = 0;
      size_t __ctr = 0;

      while (__last - __first > __gbeg[__idx]
      && static_cast<signed char>(__gbeg[__idx]) > 0)
 {
   __last -= __gbeg[__idx];
   __idx < __gsize - 1 ? ++__idx : ++__ctr;
 }

      while (__first != __last)
 *__s++ = *__first++;

      while (__ctr--)
 {
   *__s++ = __sep;
   for (char __i = __gbeg[__idx]; __i > 0; --__i)
     *__s++ = *__first++;
 }

      while (__idx--)
 {
   *__s++ = __sep;
   for (char __i = __gbeg[__idx]; __i > 0; --__i)
     *__s++ = *__first++;
 }

      return __s;
    }





  extern template class moneypunct<char, false>;
  extern template class moneypunct<char, true>;
  extern template class moneypunct_byname<char, false>;
  extern template class moneypunct_byname<char, true>;
  extern template class money_get<char>;
  extern template class money_put<char>;
  extern template class numpunct<char>;
  extern template class numpunct_byname<char>;
  extern template class num_get<char>;
  extern template class num_put<char>;
  extern template class __timepunct<char>;
  extern template class time_put<char>;
  extern template class time_put_byname<char>;
  extern template class time_get<char>;
  extern template class time_get_byname<char>;
  extern template class messages<char>;
  extern template class messages_byname<char>;
  extern template class ctype_byname<char>;
  extern template class codecvt_byname<char, char, mbstate_t>;
  extern template class collate<char>;
  extern template class collate_byname<char>;

  extern template
    const codecvt<char, char, mbstate_t>&
    use_facet<codecvt<char, char, mbstate_t> >(const locale&);

  extern template
    const collate<char>&
    use_facet<collate<char> >(const locale&);

  extern template
    const numpunct<char>&
    use_facet<numpunct<char> >(const locale&);

  extern template
    const num_put<char>&
    use_facet<num_put<char> >(const locale&);

  extern template
    const num_get<char>&
    use_facet<num_get<char> >(const locale&);

  extern template
    const moneypunct<char, true>&
    use_facet<moneypunct<char, true> >(const locale&);

  extern template
    const moneypunct<char, false>&
    use_facet<moneypunct<char, false> >(const locale&);

  extern template
    const money_put<char>&
    use_facet<money_put<char> >(const locale&);

  extern template
    const money_get<char>&
    use_facet<money_get<char> >(const locale&);

  extern template
    const __timepunct<char>&
    use_facet<__timepunct<char> >(const locale&);

  extern template
    const time_put<char>&
    use_facet<time_put<char> >(const locale&);

  extern template
    const time_get<char>&
    use_facet<time_get<char> >(const locale&);

  extern template
    const messages<char>&
    use_facet<messages<char> >(const locale&);

  extern template
    bool
    has_facet<ctype<char> >(const locale&);

  extern template
    bool
    has_facet<codecvt<char, char, mbstate_t> >(const locale&);

  extern template
    bool
    has_facet<collate<char> >(const locale&);

  extern template
    bool
    has_facet<numpunct<char> >(const locale&);

  extern template
    bool
    has_facet<num_put<char> >(const locale&);

  extern template
    bool
    has_facet<num_get<char> >(const locale&);

  extern template
    bool
    has_facet<moneypunct<char> >(const locale&);

  extern template
    bool
    has_facet<money_put<char> >(const locale&);

  extern template
    bool
    has_facet<money_get<char> >(const locale&);

  extern template
    bool
    has_facet<__timepunct<char> >(const locale&);

  extern template
    bool
    has_facet<time_put<char> >(const locale&);

  extern template
    bool
    has_facet<time_get<char> >(const locale&);

  extern template
    bool
    has_facet<messages<char> >(const locale&);


  extern template class moneypunct<wchar_t, false>;
  extern template class moneypunct<wchar_t, true>;
  extern template class moneypunct_byname<wchar_t, false>;
  extern template class moneypunct_byname<wchar_t, true>;
  extern template class money_get<wchar_t>;
  extern template class money_put<wchar_t>;
  extern template class numpunct<wchar_t>;
  extern template class numpunct_byname<wchar_t>;
  extern template class num_get<wchar_t>;
  extern template class num_put<wchar_t>;
  extern template class __timepunct<wchar_t>;
  extern template class time_put<wchar_t>;
  extern template class time_put_byname<wchar_t>;
  extern template class time_get<wchar_t>;
  extern template class time_get_byname<wchar_t>;
  extern template class messages<wchar_t>;
  extern template class messages_byname<wchar_t>;
  extern template class ctype_byname<wchar_t>;
  extern template class codecvt_byname<wchar_t, char, mbstate_t>;
  extern template class collate<wchar_t>;
  extern template class collate_byname<wchar_t>;

  extern template
    const codecvt<wchar_t, char, mbstate_t>&
    use_facet<codecvt<wchar_t, char, mbstate_t> >(locale const&);

  extern template
    const collate<wchar_t>&
    use_facet<collate<wchar_t> >(const locale&);

  extern template
    const numpunct<wchar_t>&
    use_facet<numpunct<wchar_t> >(const locale&);

  extern template
    const num_put<wchar_t>&
    use_facet<num_put<wchar_t> >(const locale&);

  extern template
    const num_get<wchar_t>&
    use_facet<num_get<wchar_t> >(const locale&);

  extern template
    const moneypunct<wchar_t, true>&
    use_facet<moneypunct<wchar_t, true> >(const locale&);

  extern template
    const moneypunct<wchar_t, false>&
    use_facet<moneypunct<wchar_t, false> >(const locale&);

  extern template
    const money_put<wchar_t>&
    use_facet<money_put<wchar_t> >(const locale&);

  extern template
    const money_get<wchar_t>&
    use_facet<money_get<wchar_t> >(const locale&);

  extern template
    const __timepunct<wchar_t>&
    use_facet<__timepunct<wchar_t> >(const locale&);

  extern template
    const time_put<wchar_t>&
    use_facet<time_put<wchar_t> >(const locale&);

  extern template
    const time_get<wchar_t>&
    use_facet<time_get<wchar_t> >(const locale&);

  extern template
    const messages<wchar_t>&
    use_facet<messages<wchar_t> >(const locale&);

 extern template
    bool
    has_facet<ctype<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<codecvt<wchar_t, char, mbstate_t> >(const locale&);

  extern template
    bool
    has_facet<collate<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<numpunct<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<num_put<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<num_get<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<moneypunct<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<money_put<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<money_get<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<__timepunct<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<time_put<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<time_get<wchar_t> >(const locale&);

  extern template
    bool
    has_facet<messages<wchar_t> >(const locale&);



}
# 47 "/usr/include/c++/4.2.1/locale" 2 3
# 47 "/usr/include/c++/4.2.1/bits/ostream.tcc" 2 3

namespace std {

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>::sentry::
    sentry(basic_ostream<_CharT, _Traits>& __os)
    : _M_ok(false), _M_os(__os)
    {

      if (__os.tie() && __os.good())
 __os.tie()->flush();

      if (__os.good())
 _M_ok = true;
      else
 __os.setstate(ios_base::failbit);
    }

  template<typename _CharT, typename _Traits>
    template<typename _ValueT>
      basic_ostream<_CharT, _Traits>&
      basic_ostream<_CharT, _Traits>::
      _M_insert(_ValueT __v)
      {
 sentry __cerb(*this);
 if (__cerb)
   {
     ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
     try
       {
  const __num_put_type& __np = __check_facet(this->_M_num_put);
  if (__np.put(*this, *this, this->fill(), __v).failed())
    __err |= ios_base::badbit;
       }
     catch(...)
       { this->_M_setstate(ios_base::badbit); }
     if (__err)
       this->setstate(__err);
   }
 return *this;
      }

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>&
    basic_ostream<_CharT, _Traits>::
    operator<<(short __n)
    {


      const ios_base::fmtflags __fmt = this->flags() & ios_base::basefield;
      if (__fmt == ios_base::oct || __fmt == ios_base::hex)
 return _M_insert(static_cast<long>(static_cast<unsigned short>(__n)));
      else
 return _M_insert(static_cast<long>(__n));
    }

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>&
    basic_ostream<_CharT, _Traits>::
    operator<<(int __n)
    {


      const ios_base::fmtflags __fmt = this->flags() & ios_base::basefield;
      if (__fmt == ios_base::oct || __fmt == ios_base::hex)
 return _M_insert(static_cast<long>(static_cast<unsigned int>(__n)));
      else
 return _M_insert(static_cast<long>(__n));
    }

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>&
    basic_ostream<_CharT, _Traits>::
    operator<<(__streambuf_type* __sbin)
    {
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      sentry __cerb(*this);
      if (__cerb && __sbin)
 {
   try
     {
       if (!__copy_streambufs(__sbin, this->rdbuf()))
  __err |= ios_base::failbit;
     }
   catch(...)
     { this->_M_setstate(ios_base::failbit); }
 }
      else if (!__sbin)
 __err |= ios_base::badbit;
      if (__err)
 this->setstate(__err);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>&
    basic_ostream<_CharT, _Traits>::
    put(char_type __c)
    {






      sentry __cerb(*this);
      if (__cerb)
 {
   ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
   try
     {
       const int_type __put = this->rdbuf()->sputc(__c);
       if (traits_type::eq_int_type(__put, traits_type::eof()))
  __err |= ios_base::badbit;
     }
   catch (...)
     { this->_M_setstate(ios_base::badbit); }
   if (__err)
     this->setstate(__err);
 }
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>&
    basic_ostream<_CharT, _Traits>::
    write(const _CharT* __s, streamsize __n)
    {







      sentry __cerb(*this);
      if (__cerb)
 {
   try
     { _M_write(__s, __n); }
   catch (...)
     { this->_M_setstate(ios_base::badbit); }
 }
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>&
    basic_ostream<_CharT, _Traits>::
    flush()
    {



      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      try
 {
   if (this->rdbuf() && this->rdbuf()->pubsync() == -1)
     __err |= ios_base::badbit;
 }
      catch(...)
 { this->_M_setstate(ios_base::badbit); }
      if (__err)
 this->setstate(__err);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    typename basic_ostream<_CharT, _Traits>::pos_type
    basic_ostream<_CharT, _Traits>::
    tellp()
    {
      pos_type __ret = pos_type(-1);
      try
 {
   if (!this->fail())
     __ret = this->rdbuf()->pubseekoff(0, ios_base::cur, ios_base::out);
 }
      catch(...)
 { this->_M_setstate(ios_base::badbit); }
      return __ret;
    }

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>&
    basic_ostream<_CharT, _Traits>::
    seekp(pos_type __pos)
    {
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      try
 {
   if (!this->fail())
     {


       const pos_type __p = this->rdbuf()->pubseekpos(__pos,
            ios_base::out);


       if (__p == pos_type(off_type(-1)))
  __err |= ios_base::failbit;
     }
 }
      catch(...)
 { this->_M_setstate(ios_base::badbit); }
      if (__err)
 this->setstate(__err);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>&
    basic_ostream<_CharT, _Traits>::
    seekp(off_type __off, ios_base::seekdir __dir)
    {
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      try
 {
   if (!this->fail())
     {


       const pos_type __p = this->rdbuf()->pubseekoff(__off, __dir,
            ios_base::out);


       if (__p == pos_type(off_type(-1)))
  __err |= ios_base::failbit;
     }
 }
      catch(...)
 { this->_M_setstate(ios_base::badbit); }
      if (__err)
 this->setstate(__err);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_ostream<_CharT, _Traits>&
    operator<<(basic_ostream<_CharT, _Traits>& __out, const char* __s)
    {
      if (!__s)
 __out.setstate(ios_base::badbit);
      else
 {


   const size_t __clen = char_traits<char>::length(__s);
   _CharT* __ws = 0;
   try
     {
       __ws = new _CharT[__clen];
       for (size_t __i = 0; __i < __clen; ++__i)
  __ws[__i] = __out.widen(__s[__i]);
     }
   catch(...)
     {
       delete [] __ws;
       __out._M_setstate(ios_base::badbit);
       return __out;
     }

   try
     {
       __ostream_insert(__out, __ws, __clen);
       delete [] __ws;
     }
   catch(...)
     {
       delete [] __ws;
       throw;
     }
 }
      return __out;
    }





  extern template class basic_ostream<char>;
  extern template ostream& endl(ostream&);
  extern template ostream& ends(ostream&);
  extern template ostream& flush(ostream&);
  extern template ostream& operator<<(ostream&, char);
  extern template ostream& operator<<(ostream&, unsigned char);
  extern template ostream& operator<<(ostream&, signed char);
  extern template ostream& operator<<(ostream&, const char*);
  extern template ostream& operator<<(ostream&, const unsigned char*);
  extern template ostream& operator<<(ostream&, const signed char*);

  extern template ostream& ostream::_M_insert(long);
  extern template ostream& ostream::_M_insert(unsigned long);
  extern template ostream& ostream::_M_insert(bool);

  extern template ostream& ostream::_M_insert(long long);
  extern template ostream& ostream::_M_insert(unsigned long long);

  extern template ostream& ostream::_M_insert(double);
  extern template ostream& ostream::_M_insert(long double);
  extern template ostream& ostream::_M_insert(const void*);


  extern template class basic_ostream<wchar_t>;
  extern template wostream& endl(wostream&);
  extern template wostream& ends(wostream&);
  extern template wostream& flush(wostream&);
  extern template wostream& operator<<(wostream&, wchar_t);
  extern template wostream& operator<<(wostream&, char);
  extern template wostream& operator<<(wostream&, const wchar_t*);
  extern template wostream& operator<<(wostream&, const char*);

  extern template wostream& wostream::_M_insert(long);
  extern template wostream& wostream::_M_insert(unsigned long);
  extern template wostream& wostream::_M_insert(bool);

  extern template wostream& wostream::_M_insert(long long);
  extern template wostream& wostream::_M_insert(unsigned long long);

  extern template wostream& wostream::_M_insert(double);
  extern template wostream& wostream::_M_insert(long double);
  extern template wostream& wostream::_M_insert(const void*);



}
# 573 "/usr/include/c++/4.2.1/ostream" 2 3
# 46 "/usr/include/c++/4.2.1/iostream" 2 3
# 1 "/usr/include/c++/4.2.1/istream" 1 3
# 42 "/usr/include/c++/4.2.1/istream" 3
       
# 43 "/usr/include/c++/4.2.1/istream" 3




namespace std {
# 57 "/usr/include/c++/4.2.1/istream" 3
  template<typename _CharT, typename _Traits>
    class basic_istream : virtual public basic_ios<_CharT, _Traits>
    {
    public:

      typedef _CharT char_type;
      typedef typename _Traits::int_type int_type;
      typedef typename _Traits::pos_type pos_type;
      typedef typename _Traits::off_type off_type;
      typedef _Traits traits_type;


      typedef basic_streambuf<_CharT, _Traits> __streambuf_type;
      typedef basic_ios<_CharT, _Traits> __ios_type;
      typedef basic_istream<_CharT, _Traits> __istream_type;
      typedef num_get<_CharT, istreambuf_iterator<_CharT, _Traits> >
        __num_get_type;
      typedef ctype<_CharT> __ctype_type;

      template<typename _CharT2, typename _Traits2>
        friend basic_istream<_CharT2, _Traits2>&
        operator>>(basic_istream<_CharT2, _Traits2>&, _CharT2&);

      template<typename _CharT2, typename _Traits2>
        friend basic_istream<_CharT2, _Traits2>&
        operator>>(basic_istream<_CharT2, _Traits2>&, _CharT2*);

    protected:







      streamsize _M_gcount;

    public:
# 103 "/usr/include/c++/4.2.1/istream" 3
      explicit
      basic_istream(__streambuf_type* __sb): _M_gcount(streamsize(0))
      { this->init(__sb); }






      virtual
      ~basic_istream()
      { _M_gcount = streamsize(0); }


      class sentry;
      friend class sentry;
# 130 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      operator>>(__istream_type& (*__pf)(__istream_type&))
      { return __pf(*this); }

      __istream_type&
      operator>>(__ios_type& (*__pf)(__ios_type&))
      {
 __pf(*this);
 return *this;
      }

      __istream_type&
      operator>>(ios_base& (*__pf)(ios_base&))
      {
 __pf(*this);
 return *this;
      }
# 177 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      operator>>(bool& __n)
      { return _M_extract(__n); }

      __istream_type&
      operator>>(short& __n);

      __istream_type&
      operator>>(unsigned short& __n)
      { return _M_extract(__n); }

      __istream_type&
      operator>>(int& __n);

      __istream_type&
      operator>>(unsigned int& __n)
      { return _M_extract(__n); }

      __istream_type&
      operator>>(long& __n)
      { return _M_extract(__n); }

      __istream_type&
      operator>>(unsigned long& __n)
      { return _M_extract(__n); }


      __istream_type&
      operator>>(long long& __n)
      { return _M_extract(__n); }

      __istream_type&
      operator>>(unsigned long long& __n)
      { return _M_extract(__n); }


      __istream_type&
      operator>>(float& __f)
      { return _M_extract(__f); }

      __istream_type&
      operator>>(double& __f)
      { return _M_extract(__f); }

      __istream_type&
      operator>>(long double& __f)
      { return _M_extract(__f); }

      __istream_type&
      operator>>(void*& __p)
      { return _M_extract(__p); }
# 249 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      operator>>(__streambuf_type* __sb);
# 259 "/usr/include/c++/4.2.1/istream" 3
      streamsize
      gcount() const
      { return _M_gcount; }
# 291 "/usr/include/c++/4.2.1/istream" 3
      int_type
      get();
# 305 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      get(char_type& __c);
# 332 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      get(char_type* __s, streamsize __n, char_type __delim);
# 343 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      get(char_type* __s, streamsize __n)
      { return this->get(__s, __n, this->widen('\n')); }
# 366 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      get(__streambuf_type& __sb, char_type __delim);
# 376 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      get(__streambuf_type& __sb)
      { return this->get(__sb, this->widen('\n')); }
# 405 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      getline(char_type* __s, streamsize __n, char_type __delim);
# 416 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      getline(char_type* __s, streamsize __n)
      { return this->getline(__s, __n, this->widen('\n')); }
# 440 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      ignore();

      __istream_type&
      ignore(streamsize __n);

      __istream_type&
      ignore(streamsize __n, int_type __delim);
# 457 "/usr/include/c++/4.2.1/istream" 3
      int_type
      peek();
# 475 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      read(char_type* __s, streamsize __n);
# 494 "/usr/include/c++/4.2.1/istream" 3
      streamsize
      readsome(char_type* __s, streamsize __n);
# 510 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      putback(char_type __c);
# 525 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      unget();
# 543 "/usr/include/c++/4.2.1/istream" 3
      int
      sync();
# 557 "/usr/include/c++/4.2.1/istream" 3
      pos_type
      tellg();
# 572 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      seekg(pos_type);
# 588 "/usr/include/c++/4.2.1/istream" 3
      __istream_type&
      seekg(off_type, ios_base::seekdir);


    protected:
      explicit
      basic_istream(): _M_gcount(streamsize(0)) { }

      template<typename _ValueT>
        __istream_type&
        _M_extract(_ValueT& __v);
    };


  template<>
    basic_istream<char>&
    basic_istream<char>::
    getline(char_type* __s, streamsize __n, char_type __delim);

  template<>
    basic_istream<char>&
    basic_istream<char>::
    ignore(streamsize __n);

  template<>
    basic_istream<char>&
    basic_istream<char>::
    ignore(streamsize __n, int_type __delim);


  template<>
    basic_istream<wchar_t>&
    basic_istream<wchar_t>::
    getline(char_type* __s, streamsize __n, char_type __delim);

  template<>
    basic_istream<wchar_t>&
    basic_istream<wchar_t>::
    ignore(streamsize __n);

  template<>
    basic_istream<wchar_t>&
    basic_istream<wchar_t>::
    ignore(streamsize __n, int_type __delim);
# 645 "/usr/include/c++/4.2.1/istream" 3
  template<typename _CharT, typename _Traits>
    class basic_istream<_CharT, _Traits>::sentry
    {
    public:

      typedef _Traits traits_type;
      typedef basic_streambuf<_CharT, _Traits> __streambuf_type;
      typedef basic_istream<_CharT, _Traits> __istream_type;
      typedef typename __istream_type::__ctype_type __ctype_type;
      typedef typename _Traits::int_type __int_type;
# 677 "/usr/include/c++/4.2.1/istream" 3
      explicit
      sentry(basic_istream<_CharT, _Traits>& __is, bool __noskipws = false);
# 687 "/usr/include/c++/4.2.1/istream" 3
      operator bool() const
      { return _M_ok; }

    private:
      bool _M_ok;
    };
# 707 "/usr/include/c++/4.2.1/istream" 3
  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    operator>>(basic_istream<_CharT, _Traits>& __in, _CharT& __c);

  template<class _Traits>
    inline basic_istream<char, _Traits>&
    operator>>(basic_istream<char, _Traits>& __in, unsigned char& __c)
    { return (__in >> reinterpret_cast<char&>(__c)); }

  template<class _Traits>
    inline basic_istream<char, _Traits>&
    operator>>(basic_istream<char, _Traits>& __in, signed char& __c)
    { return (__in >> reinterpret_cast<char&>(__c)); }
# 748 "/usr/include/c++/4.2.1/istream" 3
  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    operator>>(basic_istream<_CharT, _Traits>& __in, _CharT* __s);


  template<>
    basic_istream<char>&
    operator>>(basic_istream<char>& __in, char* __s);

  template<class _Traits>
    inline basic_istream<char, _Traits>&
    operator>>(basic_istream<char, _Traits>& __in, unsigned char* __s)
    { return (__in >> reinterpret_cast<char*>(__s)); }

  template<class _Traits>
    inline basic_istream<char, _Traits>&
    operator>>(basic_istream<char, _Traits>& __in, signed char* __s)
    { return (__in >> reinterpret_cast<char*>(__s)); }
# 775 "/usr/include/c++/4.2.1/istream" 3
  template<typename _CharT, typename _Traits>
    class basic_iostream
    : public basic_istream<_CharT, _Traits>,
      public basic_ostream<_CharT, _Traits>
    {
    public:



      typedef _CharT char_type;
      typedef typename _Traits::int_type int_type;
      typedef typename _Traits::pos_type pos_type;
      typedef typename _Traits::off_type off_type;
      typedef _Traits traits_type;


      typedef basic_istream<_CharT, _Traits> __istream_type;
      typedef basic_ostream<_CharT, _Traits> __ostream_type;







      explicit
      basic_iostream(basic_streambuf<_CharT, _Traits>* __sb)
      : __istream_type(), __ostream_type()
      { this->init(__sb); }




      virtual
      ~basic_iostream() { }

    protected:
      explicit
      basic_iostream() : __istream_type(), __ostream_type()
      { }
    };
# 838 "/usr/include/c++/4.2.1/istream" 3
  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    ws(basic_istream<_CharT, _Traits>& __is);

}


# 1 "/usr/include/c++/4.2.1/bits/istream.tcc" 1 3
# 44 "/usr/include/c++/4.2.1/bits/istream.tcc" 3
       
# 45 "/usr/include/c++/4.2.1/bits/istream.tcc" 3




namespace std {

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>::sentry::
    sentry(basic_istream<_CharT, _Traits>& __in, bool __noskip) : _M_ok(false)
    {
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      if (__in.good())
 {
   if (__in.tie())
     __in.tie()->flush();
   if (!__noskip && (__in.flags() & ios_base::skipws))
     {
       const __int_type __eof = traits_type::eof();
       __streambuf_type* __sb = __in.rdbuf();
       __int_type __c = __sb->sgetc();

       const __ctype_type& __ct = __check_facet(__in._M_ctype);
       while (!traits_type::eq_int_type(__c, __eof)
       && __ct.is(ctype_base::space,
    traits_type::to_char_type(__c)))
  __c = __sb->snextc();




       if (traits_type::eq_int_type(__c, __eof))
  __err |= ios_base::eofbit;
     }
 }

      if (__in.good() && __err == ios_base::goodbit)
 _M_ok = true;
      else
 {
   __err |= ios_base::failbit;
   __in.setstate(__err);
 }
    }

  template<typename _CharT, typename _Traits>
    template<typename _ValueT>
      basic_istream<_CharT, _Traits>&
      basic_istream<_CharT, _Traits>::
      _M_extract(_ValueT& __v)
      {
 sentry __cerb(*this, false);
 if (__cerb)
   {
     ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
     try
       {
  const __num_get_type& __ng = __check_facet(this->_M_num_get);
  __ng.get(*this, 0, *this, __err, __v);
       }
     catch(...)
       { this->_M_setstate(ios_base::badbit); }
     if (__err)
       this->setstate(__err);
   }
 return *this;
      }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    operator>>(short& __n)
    {


      long __l;
      _M_extract(__l);
      if (!this->fail())
 {
   if (numeric_limits<short>::min() <= __l
       && __l <= numeric_limits<short>::max())
     __n = __l;
   else
     this->setstate(ios_base::failbit);
 }
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    operator>>(int& __n)
    {


      long __l;
      _M_extract(__l);
      if (!this->fail())
 {
   if (numeric_limits<int>::min() <= __l
       && __l <= numeric_limits<int>::max())
     __n = __l;
   else
     this->setstate(ios_base::failbit);
 }
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    operator>>(__streambuf_type* __sbout)
    {
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      sentry __cerb(*this, false);
      if (__cerb && __sbout)
 {
   try
     {
       bool __ineof;
       if (!__copy_streambufs_eof(this->rdbuf(), __sbout, __ineof))
  __err |= ios_base::failbit;
       if (__ineof)
  __err |= ios_base::eofbit;
     }
   catch(...)
     { this->_M_setstate(ios_base::failbit); }
 }
      else if (!__sbout)
 __err |= ios_base::failbit;
      if (__err)
 this->setstate(__err);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    typename basic_istream<_CharT, _Traits>::int_type
    basic_istream<_CharT, _Traits>::
    get(void)
    {
      const int_type __eof = traits_type::eof();
      int_type __c = __eof;
      _M_gcount = 0;
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      sentry __cerb(*this, true);
      if (__cerb)
 {
   try
     {
       __c = this->rdbuf()->sbumpc();

       if (!traits_type::eq_int_type(__c, __eof))
  _M_gcount = 1;
       else
  __err |= ios_base::eofbit;
     }
   catch(...)
     { this->_M_setstate(ios_base::badbit); }
 }
      if (!_M_gcount)
 __err |= ios_base::failbit;
      if (__err)
 this->setstate(__err);
      return __c;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    get(char_type& __c)
    {
      _M_gcount = 0;
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      sentry __cerb(*this, true);
      if (__cerb)
 {
   try
     {
       const int_type __cb = this->rdbuf()->sbumpc();

       if (!traits_type::eq_int_type(__cb, traits_type::eof()))
  {
    _M_gcount = 1;
    __c = traits_type::to_char_type(__cb);
  }
       else
  __err |= ios_base::eofbit;
     }
   catch(...)
     { this->_M_setstate(ios_base::badbit); }
 }
      if (!_M_gcount)
 __err |= ios_base::failbit;
      if (__err)
 this->setstate(__err);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    get(char_type* __s, streamsize __n, char_type __delim)
    {
      _M_gcount = 0;
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      sentry __cerb(*this, true);
      if (__cerb)
 {
   try
     {
       const int_type __idelim = traits_type::to_int_type(__delim);
       const int_type __eof = traits_type::eof();
       __streambuf_type* __sb = this->rdbuf();
       int_type __c = __sb->sgetc();

       while (_M_gcount + 1 < __n
       && !traits_type::eq_int_type(__c, __eof)
       && !traits_type::eq_int_type(__c, __idelim))
  {
    *__s++ = traits_type::to_char_type(__c);
    ++_M_gcount;
    __c = __sb->snextc();
  }
       if (traits_type::eq_int_type(__c, __eof))
  __err |= ios_base::eofbit;
     }
   catch(...)
     { this->_M_setstate(ios_base::badbit); }
 }


      if (__n > 0)
 *__s = char_type();
      if (!_M_gcount)
 __err |= ios_base::failbit;
      if (__err)
 this->setstate(__err);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    get(__streambuf_type& __sb, char_type __delim)
    {
      _M_gcount = 0;
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      sentry __cerb(*this, true);
      if (__cerb)
 {
   try
     {
       const int_type __idelim = traits_type::to_int_type(__delim);
       const int_type __eof = traits_type::eof();
       __streambuf_type* __this_sb = this->rdbuf();
       int_type __c = __this_sb->sgetc();
       char_type __c2 = traits_type::to_char_type(__c);

       while (!traits_type::eq_int_type(__c, __eof)
       && !traits_type::eq_int_type(__c, __idelim)
       && !traits_type::eq_int_type(__sb.sputc(__c2), __eof))
  {
    ++_M_gcount;
    __c = __this_sb->snextc();
    __c2 = traits_type::to_char_type(__c);
  }
       if (traits_type::eq_int_type(__c, __eof))
  __err |= ios_base::eofbit;
     }
   catch(...)
     { this->_M_setstate(ios_base::badbit); }
 }
      if (!_M_gcount)
 __err |= ios_base::failbit;
      if (__err)
 this->setstate(__err);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    getline(char_type* __s, streamsize __n, char_type __delim)
    {
      _M_gcount = 0;
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      sentry __cerb(*this, true);
      if (__cerb)
        {
          try
            {
              const int_type __idelim = traits_type::to_int_type(__delim);
              const int_type __eof = traits_type::eof();
              __streambuf_type* __sb = this->rdbuf();
              int_type __c = __sb->sgetc();

              while (_M_gcount + 1 < __n
                     && !traits_type::eq_int_type(__c, __eof)
                     && !traits_type::eq_int_type(__c, __idelim))
                {
                  *__s++ = traits_type::to_char_type(__c);
                  __c = __sb->snextc();
                  ++_M_gcount;
                }
              if (traits_type::eq_int_type(__c, __eof))
                __err |= ios_base::eofbit;
              else
                {
                  if (traits_type::eq_int_type(__c, __idelim))
                    {
                      __sb->sbumpc();
                      ++_M_gcount;
                    }
                  else
                    __err |= ios_base::failbit;
                }
            }
          catch(...)
            { this->_M_setstate(ios_base::badbit); }
        }


      if (__n > 0)
 *__s = char_type();
      if (!_M_gcount)
        __err |= ios_base::failbit;
      if (__err)
        this->setstate(__err);
      return *this;
    }




  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    ignore(void)
    {
      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb)
 {
   ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
   try
     {
       const int_type __eof = traits_type::eof();
       __streambuf_type* __sb = this->rdbuf();

       if (traits_type::eq_int_type(__sb->sbumpc(), __eof))
  __err |= ios_base::eofbit;
       else
  _M_gcount = 1;
     }
   catch(...)
     { this->_M_setstate(ios_base::badbit); }
   if (__err)
     this->setstate(__err);
 }
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    ignore(streamsize __n)
    {
      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb && __n > 0)
        {
          ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
          try
            {
              const int_type __eof = traits_type::eof();
              __streambuf_type* __sb = this->rdbuf();
              int_type __c = __sb->sgetc();
# 429 "/usr/include/c++/4.2.1/bits/istream.tcc" 3
       bool __large_ignore = false;
       while (true)
  {
    while (_M_gcount < __n
    && !traits_type::eq_int_type(__c, __eof))
      {
        ++_M_gcount;
        __c = __sb->snextc();
      }
    if (__n == numeric_limits<streamsize>::max()
        && !traits_type::eq_int_type(__c, __eof))
      {
        _M_gcount = numeric_limits<streamsize>::min();
        __large_ignore = true;
      }
    else
      break;
  }

       if (__large_ignore)
  _M_gcount = numeric_limits<streamsize>::max();

       if (traits_type::eq_int_type(__c, __eof))
                __err |= ios_base::eofbit;
            }
          catch(...)
            { this->_M_setstate(ios_base::badbit); }
          if (__err)
            this->setstate(__err);
        }
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    ignore(streamsize __n, int_type __delim)
    {
      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb && __n > 0)
        {
          ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
          try
            {
              const int_type __eof = traits_type::eof();
              __streambuf_type* __sb = this->rdbuf();
              int_type __c = __sb->sgetc();


       bool __large_ignore = false;
       while (true)
  {
    while (_M_gcount < __n
    && !traits_type::eq_int_type(__c, __eof)
    && !traits_type::eq_int_type(__c, __delim))
      {
        ++_M_gcount;
        __c = __sb->snextc();
      }
    if (__n == numeric_limits<streamsize>::max()
        && !traits_type::eq_int_type(__c, __eof)
        && !traits_type::eq_int_type(__c, __delim))
      {
        _M_gcount = numeric_limits<streamsize>::min();
        __large_ignore = true;
      }
    else
      break;
  }

       if (__large_ignore)
  _M_gcount = numeric_limits<streamsize>::max();

              if (traits_type::eq_int_type(__c, __eof))
                __err |= ios_base::eofbit;
       else if (traits_type::eq_int_type(__c, __delim))
  {
    if (_M_gcount < numeric_limits<streamsize>::max())
      ++_M_gcount;
    __sb->sbumpc();
  }
            }
          catch(...)
            { this->_M_setstate(ios_base::badbit); }
          if (__err)
            this->setstate(__err);
        }
      return *this;
    }

  template<typename _CharT, typename _Traits>
    typename basic_istream<_CharT, _Traits>::int_type
    basic_istream<_CharT, _Traits>::
    peek(void)
    {
      int_type __c = traits_type::eof();
      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb)
 {
   ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
   try
     {
       __c = this->rdbuf()->sgetc();
       if (traits_type::eq_int_type(__c, traits_type::eof()))
  __err |= ios_base::eofbit;
     }
   catch(...)
     { this->_M_setstate(ios_base::badbit); }
   if (__err)
     this->setstate(__err);
 }
      return __c;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    read(char_type* __s, streamsize __n)
    {
      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb)
 {
   ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
   try
     {
       _M_gcount = this->rdbuf()->sgetn(__s, __n);
       if (_M_gcount != __n)
  __err |= (ios_base::eofbit | ios_base::failbit);
     }
   catch(...)
     { this->_M_setstate(ios_base::badbit); }
   if (__err)
     this->setstate(__err);
 }
      return *this;
    }

  template<typename _CharT, typename _Traits>
    streamsize
    basic_istream<_CharT, _Traits>::
    readsome(char_type* __s, streamsize __n)
    {
      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb)
 {
   ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
   try
     {

       const streamsize __num = this->rdbuf()->in_avail();
       if (__num > 0)
  _M_gcount = this->rdbuf()->sgetn(__s, std::min(__num, __n));
       else if (__num == -1)
  __err |= ios_base::eofbit;
     }
   catch(...)
     { this->_M_setstate(ios_base::badbit); }
   if (__err)
     this->setstate(__err);
 }
      return _M_gcount;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    putback(char_type __c)
    {


      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb)
 {
   ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
   try
     {
       const int_type __eof = traits_type::eof();
       __streambuf_type* __sb = this->rdbuf();
       if (!__sb
    || traits_type::eq_int_type(__sb->sputbackc(__c), __eof))
  __err |= ios_base::badbit;
     }
   catch(...)
     { this->_M_setstate(ios_base::badbit); }
   if (__err)
     this->setstate(__err);
 }
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    unget(void)
    {


      _M_gcount = 0;
      sentry __cerb(*this, true);
      if (__cerb)
 {
   ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
   try
     {
       const int_type __eof = traits_type::eof();
       __streambuf_type* __sb = this->rdbuf();
       if (!__sb
    || traits_type::eq_int_type(__sb->sungetc(), __eof))
  __err |= ios_base::badbit;
     }
   catch(...)
     { this->_M_setstate(ios_base::badbit); }
   if (__err)
     this->setstate(__err);
 }
      return *this;
    }

  template<typename _CharT, typename _Traits>
    int
    basic_istream<_CharT, _Traits>::
    sync(void)
    {


      int __ret = -1;
      sentry __cerb(*this, true);
      if (__cerb)
 {
   ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
   try
     {
       __streambuf_type* __sb = this->rdbuf();
       if (__sb)
  {
    if (__sb->pubsync() == -1)
      __err |= ios_base::badbit;
    else
      __ret = 0;
  }
     }
   catch(...)
     { this->_M_setstate(ios_base::badbit); }
   if (__err)
     this->setstate(__err);
 }
      return __ret;
    }

  template<typename _CharT, typename _Traits>
    typename basic_istream<_CharT, _Traits>::pos_type
    basic_istream<_CharT, _Traits>::
    tellg(void)
    {


      pos_type __ret = pos_type(-1);
      try
 {
   if (!this->fail())
     __ret = this->rdbuf()->pubseekoff(0, ios_base::cur,
           ios_base::in);
 }
      catch(...)
 { this->_M_setstate(ios_base::badbit); }
      return __ret;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    seekg(pos_type __pos)
    {


      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      try
 {
   if (!this->fail())
     {

       const pos_type __p = this->rdbuf()->pubseekpos(__pos,
            ios_base::in);


       if (__p == pos_type(off_type(-1)))
  __err |= ios_base::failbit;
     }
 }
      catch(...)
 { this->_M_setstate(ios_base::badbit); }
      if (__err)
 this->setstate(__err);
      return *this;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    seekg(off_type __off, ios_base::seekdir __dir)
    {


      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      try
 {
   if (!this->fail())
     {

       const pos_type __p = this->rdbuf()->pubseekoff(__off, __dir,
            ios_base::in);


       if (__p == pos_type(off_type(-1)))
  __err |= ios_base::failbit;
     }
 }
      catch(...)
 { this->_M_setstate(ios_base::badbit); }
      if (__err)
 this->setstate(__err);
      return *this;
    }


  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    operator>>(basic_istream<_CharT, _Traits>& __in, _CharT& __c)
    {
      typedef basic_istream<_CharT, _Traits> __istream_type;
      typedef typename __istream_type::int_type __int_type;

      typename __istream_type::sentry __cerb(__in, false);
      if (__cerb)
 {
   ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
   try
     {
       const __int_type __cb = __in.rdbuf()->sbumpc();
       if (!_Traits::eq_int_type(__cb, _Traits::eof()))
  __c = _Traits::to_char_type(__cb);
       else
  __err |= (ios_base::eofbit | ios_base::failbit);
     }
   catch(...)
     { __in._M_setstate(ios_base::badbit); }
   if (__err)
     __in.setstate(__err);
 }
      return __in;
    }

  template<typename _CharT, typename _Traits>
    basic_istream<_CharT, _Traits>&
    operator>>(basic_istream<_CharT, _Traits>& __in, _CharT* __s)
    {
      typedef basic_istream<_CharT, _Traits> __istream_type;
      typedef typename __istream_type::__streambuf_type __streambuf_type;
      typedef typename _Traits::int_type int_type;
      typedef _CharT char_type;
      typedef ctype<_CharT> __ctype_type;

      streamsize __extracted = 0;
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      typename __istream_type::sentry __cerb(__in, false);
      if (__cerb)
 {
   try
     {

       streamsize __num = __in.width();
       if (__num <= 0)
  __num = numeric_limits<streamsize>::max();

       const __ctype_type& __ct = use_facet<__ctype_type>(__in.getloc());

       const int_type __eof = _Traits::eof();
       __streambuf_type* __sb = __in.rdbuf();
       int_type __c = __sb->sgetc();

       while (__extracted < __num - 1
       && !_Traits::eq_int_type(__c, __eof)
       && !__ct.is(ctype_base::space,
     _Traits::to_char_type(__c)))
  {
    *__s++ = _Traits::to_char_type(__c);
    ++__extracted;
    __c = __sb->snextc();
  }
       if (_Traits::eq_int_type(__c, __eof))
  __err |= ios_base::eofbit;



       *__s = char_type();
       __in.width(0);
     }
   catch(...)
     { __in._M_setstate(ios_base::badbit); }
 }
      if (!__extracted)
 __err |= ios_base::failbit;
      if (__err)
 __in.setstate(__err);
      return __in;
    }


  template<typename _CharT, typename _Traits>
    basic_istream<_CharT,_Traits>&
    ws(basic_istream<_CharT,_Traits>& __in)
    {
      typedef basic_istream<_CharT, _Traits> __istream_type;
      typedef typename __istream_type::__streambuf_type __streambuf_type;
      typedef typename __istream_type::__ctype_type __ctype_type;
      typedef typename __istream_type::int_type __int_type;

      const __ctype_type& __ct = use_facet<__ctype_type>(__in.getloc());
      const __int_type __eof = _Traits::eof();
      __streambuf_type* __sb = __in.rdbuf();
      __int_type __c = __sb->sgetc();

      while (!_Traits::eq_int_type(__c, __eof)
      && __ct.is(ctype_base::space, _Traits::to_char_type(__c)))
 __c = __sb->snextc();

       if (_Traits::eq_int_type(__c, __eof))
  __in.setstate(ios_base::eofbit);
      return __in;
    }


  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_istream<_CharT, _Traits>&
    operator>>(basic_istream<_CharT, _Traits>& __in,
        basic_string<_CharT, _Traits, _Alloc>& __str)
    {
      typedef basic_istream<_CharT, _Traits> __istream_type;
      typedef typename __istream_type::int_type __int_type;
      typedef typename __istream_type::__streambuf_type __streambuf_type;
      typedef typename __istream_type::__ctype_type __ctype_type;
      typedef basic_string<_CharT, _Traits, _Alloc> __string_type;
      typedef typename __string_type::size_type __size_type;

      __size_type __extracted = 0;
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      typename __istream_type::sentry __cerb(__in, false);
      if (__cerb)
 {
   try
     {

       __str.erase();
       _CharT __buf[128];
       __size_type __len = 0;
       const streamsize __w = __in.width();
       const __size_type __n = __w > 0 ? static_cast<__size_type>(__w)
                                : __str.max_size();
       const __ctype_type& __ct = use_facet<__ctype_type>(__in.getloc());
       const __int_type __eof = _Traits::eof();
       __streambuf_type* __sb = __in.rdbuf();
       __int_type __c = __sb->sgetc();

       while (__extracted < __n
       && !_Traits::eq_int_type(__c, __eof)
       && !__ct.is(ctype_base::space, _Traits::to_char_type(__c)))
  {
    if (__len == sizeof(__buf) / sizeof(_CharT))
      {
        __str.append(__buf, sizeof(__buf) / sizeof(_CharT));
        __len = 0;
      }
    __buf[__len++] = _Traits::to_char_type(__c);
    ++__extracted;
    __c = __sb->snextc();
  }
       __str.append(__buf, __len);

       if (_Traits::eq_int_type(__c, __eof))
  __err |= ios_base::eofbit;
       __in.width(0);
     }
   catch(...)
     {



       __in._M_setstate(ios_base::badbit);
     }
 }

      if (!__extracted)
 __err |= ios_base::failbit;
      if (__err)
 __in.setstate(__err);
      return __in;
    }

  template<typename _CharT, typename _Traits, typename _Alloc>
    basic_istream<_CharT, _Traits>&
    getline(basic_istream<_CharT, _Traits>& __in,
     basic_string<_CharT, _Traits, _Alloc>& __str, _CharT __delim)
    {
      typedef basic_istream<_CharT, _Traits> __istream_type;
      typedef typename __istream_type::int_type __int_type;
      typedef typename __istream_type::__streambuf_type __streambuf_type;
      typedef typename __istream_type::__ctype_type __ctype_type;
      typedef basic_string<_CharT, _Traits, _Alloc> __string_type;
      typedef typename __string_type::size_type __size_type;

      __size_type __extracted = 0;
      const __size_type __n = __str.max_size();
      ios_base::iostate __err = ios_base::iostate(ios_base::goodbit);
      typename __istream_type::sentry __cerb(__in, true);
      if (__cerb)
 {
   try
     {
       __str.erase();
       const __int_type __idelim = _Traits::to_int_type(__delim);
       const __int_type __eof = _Traits::eof();
       __streambuf_type* __sb = __in.rdbuf();
       __int_type __c = __sb->sgetc();

       while (__extracted < __n
       && !_Traits::eq_int_type(__c, __eof)
       && !_Traits::eq_int_type(__c, __idelim))
  {
    __str += _Traits::to_char_type(__c);
    ++__extracted;
    __c = __sb->snextc();
  }

       if (_Traits::eq_int_type(__c, __eof))
  __err |= ios_base::eofbit;
       else if (_Traits::eq_int_type(__c, __idelim))
  {
    ++__extracted;
    __sb->sbumpc();
  }
       else
  __err |= ios_base::failbit;
     }
   catch(...)
     {



       __in._M_setstate(ios_base::badbit);
     }
 }
      if (!__extracted)
 __err |= ios_base::failbit;
      if (__err)
 __in.setstate(__err);
      return __in;
    }





  extern template class basic_istream<char>;
  extern template istream& ws(istream&);
  extern template istream& operator>>(istream&, char&);
  extern template istream& operator>>(istream&, char*);
  extern template istream& operator>>(istream&, unsigned char&);
  extern template istream& operator>>(istream&, signed char&);
  extern template istream& operator>>(istream&, unsigned char*);
  extern template istream& operator>>(istream&, signed char*);

  extern template istream& istream::_M_extract(unsigned short&);
  extern template istream& istream::_M_extract(unsigned int&);
  extern template istream& istream::_M_extract(long&);
  extern template istream& istream::_M_extract(unsigned long&);
  extern template istream& istream::_M_extract(bool&);

  extern template istream& istream::_M_extract(long long&);
  extern template istream& istream::_M_extract(unsigned long long&);

  extern template istream& istream::_M_extract(float&);
  extern template istream& istream::_M_extract(double&);
  extern template istream& istream::_M_extract(long double&);
  extern template istream& istream::_M_extract(void*&);

  extern template class basic_iostream<char>;


  extern template class basic_istream<wchar_t>;
  extern template wistream& ws(wistream&);
  extern template wistream& operator>>(wistream&, wchar_t&);
  extern template wistream& operator>>(wistream&, wchar_t*);

  extern template wistream& wistream::_M_extract(unsigned short&);
  extern template wistream& wistream::_M_extract(unsigned int&);
  extern template wistream& wistream::_M_extract(long&);
  extern template wistream& wistream::_M_extract(unsigned long&);
  extern template wistream& wistream::_M_extract(bool&);

  extern template wistream& wistream::_M_extract(long long&);
  extern template wistream& wistream::_M_extract(unsigned long long&);

  extern template wistream& wistream::_M_extract(float&);
  extern template wistream& wistream::_M_extract(double&);
  extern template wistream& wistream::_M_extract(long double&);
  extern template wistream& wistream::_M_extract(void*&);

  extern template class basic_iostream<wchar_t>;



}
# 846 "/usr/include/c++/4.2.1/istream" 2 3
# 47 "/usr/include/c++/4.2.1/iostream" 2 3

namespace std {
# 63 "/usr/include/c++/4.2.1/iostream" 3
  extern istream cin;
  extern ostream cout;
  extern ostream cerr;
  extern ostream clog;


  extern wistream wcin;
  extern wostream wcout;
  extern wostream wcerr;
  extern wostream wclog;




  static ios_base::Init __ioinit;

}
# 1 "in.cpp" 2
