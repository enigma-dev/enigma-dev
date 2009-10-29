//#include <stdio.h>
//#include <wchar.h>
namespace namespacicle
{
  int this_does_work();
  int *this_doesnt_work();
  int this_is_cut();
}

/*
union pascal
{
  int something;
  double ovaries;
}*/

/*
#if 1
  #if 0
  typedef long int __quad_t;
  typedef unsigned long int __u_quad_t;
  #elif 0
  __extension__ typedef long long int __quad_t;
  __extension__ typedef unsigned long long int __u_quad_t;
  #else
  typedef struct
  {
    long __val[2];
  } __quad_t;
  typedef struct
  {
    long __val[2];
  } __u_quad_t;
  #endif
#endif
*/

/*
struct salad;
typedef struct salad dressing;*/

/*#define WORDSIZE 32

#if WORDSIZE == 32
# define Successish
#elif WORDSIZE == 64
# define Absolute_failure
#else
# define failure
# error "unexpected value for __WORDSIZE macro"
#endif
*/
/*
#if defined TACOS
  int should_not_be_here;
#else
  int should_be_here;
#endif

#define TACOS 1

#if defined TACOS
  int passing_grade;
#else
  int failing_grade;
#endif

/*struct a
{
  struct a
  {
    int a;
  };
  #if 1 + 16 - (2 << 3) - 1
    int millennium_falcon;
  #endif
  int this_should_be_there;
  #ifndef TACOS
    int this_should_be_there_too;
  #endif
  #ifdef TACOS
    int this_shouldnt_be_there;
  #endif
  #define TACOS 1
  #ifndef TACOS
    int this_shouldnt_be_there2;
  #endif
  #ifdef TACOS
    int this_should_be_there_too2;
  #endif
  
  #define TROO 1
  #define FALZ 0
  #if FALZ
    int should_not_be_here;
    #if TROO
      int ultimately_should_not_be_here;
    #endif
  #else
    int should_be_here;
    #if TROO
      int should_be_here2;
    #else
      double shouldnt_be_here_either;
    #endif
  #endif
};

#include "cfile_parse/expression_evaluator_settings.h"

#define captain(planet) int planet[32];

#define ZE_END_OF_INCLUDE winnage

int ordinal();
captain(chalupa);

*/
