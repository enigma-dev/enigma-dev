//This file is entirely for debug.
//Same GPL copyright applies as in header.

#include <iostream>
using namespace std;
void post_access_watch(unsigned i,unsigned sz,unsigned all)
{
  if ((signed)(i - sz) > 0)
    cout <<
    "***************************************************\n" <<
    "Though this is not a fatal error, its occurrance is\n" <<
    "potentially dangerous. Please report the occurrence\n" <<
    "***************************************************\n";
}
