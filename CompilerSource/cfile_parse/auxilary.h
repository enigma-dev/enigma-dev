#define concat(x,y) x ## y
#define conc(x,y) concat(x,y)
#define conc3(x,y,z) conc(conc(x,y),z)
#define mc(w,x,y,z); w conc3(x,y,z);


//mc(int,a,b1,qqq0);
int conc(conc(a,b1),c1);

#include <math.h>
