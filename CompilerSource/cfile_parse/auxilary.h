/*template <typename a> struct b {
  typedef typename a::template c<int>
  #pragma printlast
  ::d e;
};
*/

/*
struct a { a(); a(int); };
a::a() {};
a::a(int) {};
*/


/*template <typename a> struct b { };
template <typename c> typename b<c>::d monster();*/

/*
template <typename b> struct a {
  typedef typename b::template e f;
  typedef typename b::template c<int> d;
};
template <typename aa> typename a<aa>::bb cc;
*/

/*
template <typename,typename,typename> struct basic_string { basic_string(); };

  template<typename _CharT, typename _Traits, typename _Alloc>
    inline basic_string<_CharT, _Traits, _Alloc>::basic_string
#pragma debug_entry_point
(
  #pragma printlast
):_M_dataplus(_S_empty_rep()._M_refdata(), _Alloc()) { }
*/


/*
#include <map>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <list>
#include <set>
#include <iostream>*/

//CUsersJoshENIGMAtrunkENIGMAsystemSHELL
#include "C:/Users/Josh/ENIGMA/trunk/ENIGMAsystem/SHELL/SHELLmain.cpp"

/* 
enum ass { a };
#pragma printlast
typedef enum 
#pragma printlast
ass 
#pragma printlast
ass
#pragma printlast
;

/* */
/*
struct a;
typedef struct a { int c; } b;
typedef const b *c;
*/
/*struct a;
typedef struct a * b;
typedef const b c;*/

//#include <windows.h>
//using namespace std;


/* */
/*
static struct a { };
static struct a b;

enum c { er };
enum c d;*/

/*
struct HDDEDATA__ { int i; };
typedef HDDEDATA__ HDDEDATA;
#define CALLBACK __stdcall
//typedef HDDEDATA CALLBACK FNCALLBACK
//#pragma debug_entry_point
//(UINT,UINT);

typedef HDDEDATA 
#pragma debug_entry_point
(
#pragma printlast
CALLBACK *PFNCALLBACK)(UINT,UINT);
HDDEDATA(*a) (int);*/


/*

extern int a;
extern int a;
int a;
extern int a;
extern int a;
extern int a;
int a;

template <typename a> int b(int a::* A);
template <class a> struct c { template<class b = double a::* B> struct d {}; };
int main() {}

*/
