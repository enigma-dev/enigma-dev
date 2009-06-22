#define EXTFLAG_FUNCTION 1
#define EXTFLAG_TEMPLATE 2
#define EXTFLAG_TYPENAME 4
#define EXTFLAG_MACRO 8
#define EXTFLAG_CLASS 16
#define EXTFLAG_ENUM 32
#define EXTFLAG_STRUCT 64
#define EXTFLAG_NAMESPACE 128
//#define EXTFLAG_NAMESPACE 256

struct externs
{
  unsigned int flags;
  darray<char> fargs;
  darray<char> targs;
  string name;
  
  externs* parent;
  map<string, externs*> members;
};

extern map<string,string> macros;
typedef map<string,externs*>::iterator extiter;
typedef map<string,string>::iterator maciter;
extern externs global_scope,*current_scope;
