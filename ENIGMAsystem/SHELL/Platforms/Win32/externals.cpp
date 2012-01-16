/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <map>
#include <string>
#include <windows.h>

#include "ffi/ffi.h"
#include "Universal_System/var4.h"

#include "Platforms/platforms_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "externals.h"

using namespace std;

struct external
{
  ffi_cif cif;
  int argc,restype;
  ffi_type **arg_type;
  void (*functionptr)();
  
  external(int acount,int returntype)
  {
    argc=acount;
    restype=returntype;
    arg_type=new ffi_type*[acount];
  }
  ~external()
  {
    delete[] arg_type;
  }
};

map<int,external*> externals;
int external_count=0;

int external_define(string dll,string func,int calltype,bool returntype,int argcount,
                    bool t01,bool t02,bool t03,bool t04,bool t05,bool t06,bool t07,bool t08,
                    bool t09,bool t10,bool t11,bool t12,bool t13,bool t14,bool t15,bool t16)
{
  ffi_status status;
  
  int ac=(argcount>16)?16:((int)argcount);
  external *a = new external(ac,(int)returntype);
  
  switch (ac)
  {
    case 16: a->arg_type[15] = (t16==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 15: a->arg_type[14] = (t15==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 14: a->arg_type[13] = (t14==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 13: a->arg_type[12] = (t13==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 12: a->arg_type[11] = (t12==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 11: a->arg_type[10] = (t11==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 10: a->arg_type[ 9] = (t10==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 9:  a->arg_type[ 8] = (t09==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 8:  a->arg_type[ 7] = (t08==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 7:  a->arg_type[ 6] = (t07==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 6:  a->arg_type[ 5] = (t06==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 5:  a->arg_type[ 4] = (t05==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 4:  a->arg_type[ 3] = (t04==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 3:  a->arg_type[ 2] = (t03==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 2:  a->arg_type[ 1] = (t02==ty_string?(&ffi_type_pointer):(&ffi_type_double));
    case 1:  a->arg_type[ 0] = (t01==ty_string?(&ffi_type_pointer):(&ffi_type_double));
  }
  
  ffi_type *restype = ((returntype==ty_string)?(&ffi_type_pointer):(&ffi_type_double));
  status=ffi_prep_cif(&(a->cif), ((calltype==dll_stdcall)?FFI_STDCALL:FFI_DEFAULT_ABI), ac, restype, a->arg_type);
  
  if (status != FFI_OK)
  {
    show_error("Defining DLL failed.",0);
    return -1;
  }
  
  HMODULE dllmod = LoadLibrary(dll.c_str());
  FARPROC funcptr = GetProcAddress(dllmod,func.c_str());
  if (funcptr==NULL) return 0;
  a->functionptr=(void(*)())funcptr;
  
  int ind=external_count++;
  externals[ind]=a;
  return ind;
}

using namespace enigma;
union ambiguous { double d; const char* s; };
variant external_call(int id,variant a1,variant a2, variant a3, variant a4, variant a5, variant a6, variant a7, variant a8,
                             variant a9,variant a10,variant a11,variant a12,variant a13,variant a14,variant a15,variant a16)
{
  map<int,external*>::iterator it;
  if ((it=externals.find(id)) == externals.end())
    return 0;
  external* a=it->second;
  
  ambiguous array[a->argc];
  void *arg_values[a->argc];
  
  variant args[] = { a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16 };
  for (int i=0;i<a->argc;i++)
  {
    if (a->arg_type[i]==&ffi_type_double)
      array[i].d=(double)args[i];
    else
      array[i].s=((string)args[i]).c_str();
    arg_values[i]=&array[i];
  }
  
  ambiguous result;
  ffi_call(&(a->cif), a->functionptr, &result, arg_values);
  if (a->restype==ty_string) return result.s;
  return result.d;
}
