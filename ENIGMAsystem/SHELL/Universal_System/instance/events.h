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


#if INSTANCE_DEPTH_METHOD!=2
struct depthnode;
#endif
struct eventnode;

struct fnode  //structure points to an instance containing the event
{
  parent* inst; //Pointer to the instance
  fnode *next, *prev; //used in linking
  
  #if INSTANCE_DEPTH_METHOD!=2
    depthnode *mydepth;
    void movedepth(int ndep);
  #else
    eventnode *myevent;
  #endif
  
  ~fnode();
  void erase();
};

#if INSTANCE_DEPTH_METHOD!=2
  #define ENIGMACRO_fetchfnode(x,y) event_list->getevent(x)->getdepth(y)->appendfnode();
  struct depthnode  //structure points to a list of instances by depth 
  {
    #if INSTANCE_DEPTH_METHOD==0
     std::map<int,depthnode*> *mymap;
     std::map<int,depthnode*>::iterator me;
     eventnode *myevent;
    #elif INSTANCE_DEPTH_METHOD==1
     int depth;
     eventnode *myevent;
     depthnode *next, *prev;
    #endif
    
    fnode *end, *start;
    
    fnode* appendfnode(fnode* nta=NULL);
    void erase();
    
    depthnode();
    ~depthnode();
  };
#else
  #define ENIGMACRO_fetchfnode(x,y) event_list->getevent(x)->appendfnode();
#endif

struct eventnode  //structure points to a list of depths by event id 
{
  int evid;
  #if INSTANCE_DEPTH_METHOD==0
    std::map<int,depthnode*> depths;
    depthnode* getdepth(int n_depth);
  #elif INSTANCE_DEPTH_METHOD==1
    depthnode *depths;
    depthnode* getdepth(int n_depth);
  #elif INSTANCE_DEPTH_METHOD==2
    fnode *start, *end;
    fnode *appendfnode();
  #endif
  
  eventnode *next;
  eventnode* getevent(int n_id);
  
  eventnode();
  ~eventnode();
};










