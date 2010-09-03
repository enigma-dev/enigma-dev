#define __MAC_10_0      1000

#define __AVAILABILITY_INTERNAL_DEPRECATED         __attribute__((deprecated,visibility("default")))
#define __AVAILABILITY_INTERNAL_UNAVAILABLE        __attribute__((unavailable,visibility("default")))
#define __AVAILABILITY_INTERNAL_WEAK_IMPORT        __attribute__((weak_import,visibility("default")))
#define __AVAILABILITY_INTERNAL_REGULAR            __attribute__((visibility("default")))
#define __AVAILABILITY_INTERNAL__MAC_10_0 __AVAILABILITY_INTERNAL_REGULAR
#define __AVAILABILITY_INTERNAL__MAC_10_0_DEP__MAC_10_5 __AVAILABILITY_INTERNAL__MAC_10_0

#define	__CONCAT(x,y)	x/**/y
#define	__STRING(x)	"x"

#define __OSX_AVAILABLE_STARTING(_mac, _iphone) __AVAILABILITY_INTERNAL##_mac
#define __OSX_AVAILABLE_BUT_DEPRECATED(_macIntro, _macDep, _iphoneIntro, _iphoneDep) __AVAILABILITY_INTERNAL##_macIntro##_DEP##_macDep

#    define __DARWIN_SUF_1050

#define __DARWIN_1050(sym)		__asm("_" __STRING(sym) __DARWIN_SUF_1050)
int	 daemon(int, int) __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_5,__IPHONE_2_0,__IPHONE_2_0);

#define __DARWIN_SUF_UNIX03 /* Nothing */
#define __DARWIN_10_6_AND_LATER_ALIAS(x) x
#define __DARWIN_ALIAS(sym) __asm("_" __STRING(sym) __DARWIN_SUF_UNIX03)
__DARWIN_10_6_AND_LATER_ALIAS(__DARWIN_ALIAS(fopen));

//template<class a> struct b {};
//typedef b<int> c;

#define SUFFIXTHING /* NOTHING */
__asm__ ("addl %%ebx, %%eax;":"=a"(add):"a"(arg1),"b"(arg2)SUFFIXTHING);

/* union for signal handlers */
struct ass {
};

/* Signal vector template for Kernel user boundary */
struct	cunt {
	struct ass 
	#pragma debug_entry_point
	ass;  /* signal handler */
};

#define __STRING(x) #x

__asm("asm" "asskaec" __STRING(ass) /* fuck all */ "ASS" //Fuck ALL!
);

#include <sys/signal.h>

#include <cpio.h>
#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pthread.h>
#include <pwd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <tar.h>
#include <termios.h>
#include <unistd.h>
#include <utime.h>

#include <map>
#include <list>
#include <stack>
#include <string>
#include <vector>
#include <limits>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#include "../../ENIGMAsystem/SHELL/SHELLmain.cpp"

/*#define concat(x,y) x ## y
#define conc(x,y) concat(x,y)
#define conc3(x,y,z) conc(conc(x,y),z)
#define mc(w,x,y,z); w conc3(x,y,z);


//mc(int,a,b1,qqq0);
int conc(conc(a,b1),c1);
/*
#include <math.h>


#include <string>
#include <list>
#include <stack>
#include <vector>
#include <map>
*

#define deca() \
  double a;
#define decb() \
  double b()  {}\
  double bb() {}
#define decc() \
  double c;

#define macrothing()\
 deca()\
 decb()\
 decc()

macrothing();*/
