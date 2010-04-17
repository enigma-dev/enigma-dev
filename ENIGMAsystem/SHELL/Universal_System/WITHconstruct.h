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

//This is to be redone before official release.
/*
namespace enigma
{
  std::map<int,int> withid;
  int withpush=0;

  int pushwith(int level,double object2do);
  void popOtherObject();

  wildclass withcur[12];

  struct withiter
  {
    bool stillgoing;
    int level; wildclass mycur;
    std::map<int,object_basic*>::iterator instit;

    withiter(int a);

    void operator++ (int a);
  };
}



namespace enigma
{

  struct withstack { withstack* prev; object_basic* obj; } *withOTHERobject=NULL;
  int pushwith(int level,double object2do)
  {
    withpush=level;
    withid[withpush]=(int)object2do;
    withstack *lastoo=withOTHERobject;
    withOTHERobject=new withstack;
    withOTHERobject->prev=lastoo;
    return withpush;
  }
  void popOtherObject()
  {
    withstack *lastoo=withOTHERobject;
    if (lastoo==NULL) { show_error("Unexpected end of with clause",0); return; }
    withOTHERobject=lastoo->prev;
    delete []lastoo;
  }


  withiter::withiter(int a)
  {
    stillgoing=1;
    level=withpush; instit=enigma::instance_list.begin();
    if (instit == enigma::instance_list.end())
    stillgoing=0;
    else
    {
      mycur=(*instit).second;
      withcur[level]=mycur;
    }
  }

  void withiter::operator++ (int a)
  {
    instit++;
    if (instit == enigma::instance_list.end())
    stillgoing=0;
    else
    {
      mycur=(*instit).second;
      withcur[level]=mycur;
    }
  }
}
*/
