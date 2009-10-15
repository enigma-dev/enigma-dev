struct a
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

#define captain(planet) planet.save()

#define ZE_END_OF_INCLUDE winnage
