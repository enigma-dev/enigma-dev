#include <map>
#include <string>
#include "darray.h"
using namespace std;
#include "externs.h"

//Map to sort, darray for polymorphic things
map<string, varray<externs> > extarray;
externs global_scope,*current_scope;
map<string,string> macros;
