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

#if INSTANCE_STORAGE_METHOD==1
  struct instnode
  {
    parent *object;
    instnode *next,*prev;
    
    instnode();
    ~instnode();
    
    instnode* destroy(); //it's more like "orphan" than "destroy";
  };
  
  struct instlist
  {
    instnode *list, *last;
    instnode* appendinst();
    instnode* findbyid(int id);
    instlist(); ~instlist();
  };
  
  #define foreach(a) for (a=instance_list->list->next; a.current!=NULL; a++)
  #define ENIGMACRO_ACCESSOBJPRT(itr) itr.current->object
  #define ENIGMACRO_ACCESSINSTNODE(itr) itr.current
  #define ENIGMACRO_APPENDINST(id) appendinst()
  
#elif INSTANCE_STORAGE_METHOD==0
  struct instnode
  {
    parent *object;
    std::map<int,instnode*> *mymap;
    std::map<int,instnode*>::iterator me;
    
    instnode();
    ~instnode();
    instnode* destroy();
  };
  
  struct instlist
  {
    std::map<int,instnode*> list;
    instnode* appendinst(int id);
    instnode* findbyid(int id);
    instlist(); ~instlist();
  };
  
  #define foreach(a) for (a=instance_list->list.begin(); a!=instance_list->list.end(); a++) 
  #define ENIGMACRO_ACCESSOBJPRT(itr) itr->second->object
  #define ENIGMACRO_ACCESSINSTNODE(itr) itr->second
  #define ENIGMACRO_APPENDINST(id) appendinst(id)
  
#endif


#if INSTANCE_STORAGE_METHOD==1
  struct institer
  {
    instnode *current;
    institer& operator= (instnode *node);
    institer& operator++ (int nothing);
  };
#elif INSTANCE_STORAGE_METHOD==0
  typedef std::map<int,instnode*>::iterator institer;
#endif
