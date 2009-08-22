/*********************************************************************************\
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
\*********************************************************************************/

int idmax=0;

#define EV_STEP 0

struct objects:public parent
{
   int depth;
   fnode* fnode_step;
   void step() { printf("Step: ID %d, depth %d\r\n",id,depth); }
   objects(); void pre_dtor();
   #if INSTANCE_DEPTH_METHOD != 2
     void move_depth(int ndep);
   #endif
};
objects::objects()
{
  depth=0;
  id=idmax++;
  
  //add the object's event to the events list
  fnode* nfn=ENIGMACRO_fetchfnode(EV_STEP,depth);
  nfn->inst=this; fnode_step=nfn;
}
void objects::pre_dtor()
{
  fnode_step->erase();
  void move_depth();
}
#if INSTANCE_DEPTH_METHOD != 2
  void objects::move_depth(int ndep)
  {
    depth=ndep;
    fnode_step->movedepth(ndep);
  }
#endif

struct objects2:public parent
{
   int depth;
   fnode* fnode_step;
   void step() { printf("Step: ID %d, depth %d\r\n",id,depth); }
   objects2(); void pre_dtor();
   #if INSTANCE_DEPTH_METHOD != 2
     void move_depth(int ndep);
   #endif
};
objects2::objects2()
{
  depth=1; id=idmax++;
  //add the object's event to the events list
  fnode *nfn = ENIGMACRO_fetchfnode(EV_STEP,depth);
  nfn->inst=this; fnode_step=nfn;
}
void objects2::pre_dtor()
{
  fnode_step->erase();
}

#if INSTANCE_DEPTH_METHOD != 2
  void objects2::move_depth(int ndep)
  {
    depth=ndep;
    fnode_step->movedepth(ndep);
  }
#endif
