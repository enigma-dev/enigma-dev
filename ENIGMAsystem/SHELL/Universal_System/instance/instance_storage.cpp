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

#include <map>
#include <stdio.h>
#include "GLOBAL_GAME_SETTINGS.h"

#include "INSTANCE_PARENT.h"
#include "instances.h"
#include "events.h"
#include "cleanup.h"


  /* Actual instance storage
  An unsorted list that passes all cleanup duties to
  a special structure that will handle them later on
  */
     #if INSTANCE_STORAGE_METHOD==1
       instnode::instnode()
        { next=prev=NULL; object=NULL; }
       instnode::~instnode()
        { delete object; } //deletes events
     #elif INSTANCE_STORAGE_METHOD==0
       instnode::instnode()
        { object=NULL; }
       instnode::~instnode()
        { delete object; mymap->erase(me); } //deletes object, which deletes events
     #endif
     
     #if INSTANCE_STORAGE_METHOD==1
       instlist::instlist()
       {
         list=last=new instnode; //root
       }
       instlist::~instlist()
       { /*shouldn't happen*/ }
     #elif INSTANCE_STORAGE_METHOD==0
       instlist::instlist()
       { /*nothing to do, really */ }
       instlist::~instlist()
       { /*shouldn't happen*/ }
     #endif
    
    #if INSTANCE_STORAGE_METHOD==1
      instnode* instlist::appendinst()
      {
        instnode* node=new instnode; //allocate the new node
        last->next=node; //inform the *old* last node,
        node->prev=last; //inform the *new* last node,
        last=node;       //then burn the bridge
        node->next=NULL; 
        return node;
      }
    #elif INSTANCE_STORAGE_METHOD==0
      instnode* instlist::appendinst(int id)
      {
        instnode* node=new instnode; //allocate the new node
        
        #if DEBUGMODE
          if (list.find(id) != list.end())
          show_error("Trying to create multiple instances of same id",0);
        #endif
        
        list[id]=node;
        std::map<int,instnode*>::iterator lit=list.find(id);
        
        #if DEBUGMODE
          if (lit->second!=node)
          show_error("Found wrong instnode during creation",0);
        #endif
        
        node->me=lit;
        node->mymap=&list;
        return node;
      }
    #endif
    
    #if INSTANCE_STORAGE_METHOD==1
      extern instlist* instance_list;
      instnode* instnode::destroy() //it's more like "orphan" than "destroy"
      {
        if (prev!=NULL) prev->next = next; //This is essencially cutting out the
        if (next!=NULL) next->prev = prev; // current node and healing the wound
        if (object!=NULL) object->pre_dtor();
        if (instance_list->last==this) instance_list->last=prev;
        return this; //Don't destroy it now; this might offend the iterators.
      }
    #elif INSTANCE_STORAGE_METHOD==0
      instnode* instnode::destroy()
      {
        //do not call mymap->erase(me) or it will lead to the destruction of the world
        if (object!=NULL) object->pre_dtor();
        return this; //Return for later destroy
      }
    #endif
    
    #if INSTANCE_STORAGE_METHOD==1
      instnode* instlist::findbyid(int id)
      {
         instnode *pointer = list->next;
         while (pointer!=NULL)
         {
           if (pointer->object->id==id)
           return pointer; 
           pointer=pointer->next;
         }
         return NULL;
      }
    #elif INSTANCE_STORAGE_METHOD==0
      instnode* instlist::findbyid(int id)
      {
         std::map<int,instnode*>::iterator lit=list.find(id);
         if (lit!=list.end()) return lit->second;
         return NULL;
      }
    #endif
    
    #if INSTANCE_STORAGE_METHOD==1
      institer& institer::operator= (instnode *node)
      {
        #if DEBUGMODE
        if (node==NULL)
        show_error("Trying to iterate a bad or nonexistant instance list.",0);
        #endif
        current=node;
        return *this;
      }
      institer& institer::operator++ (int nothing)
      {
        #if DEBUGMODE
        if (current==NULL)
        show_error("Trying to iterate past the end of the instance list.",1);
        #endif
        current=current->next;
        return *this;
      }
    #endif
    
    
  /* Cleanup
  A structure that will take care of cleanup
  after it's all said and done.
  */
    
    #if INSTANCE_STORAGE_METHOD==0
    
    //Because STL is simply not meant for this kind of abuse, we have to make sure we don't kill it
    
      combuststruct& combuststruct::operator+=(instnode* node) 
      {
         combustmap[node->object]=node;
         return *this;
      }
      void combuststruct::combust()
      {
        if (combustmap.empty()) printf("Nothing to combust\r\n");
        for (std::map<parent*,instnode*>::iterator it=combustmap.begin(); 
             it!=combustmap.end(); it++)
          delete it->second; //delete it->first;
        combustmap.clear();
      }
      /*
      combuststruct::combuststruct() { }
      combuststruct::~combuststruct() { }*/
    #else
      combuststruct& combuststruct::operator+=(instnode* node) 
      {
         combuststruct* ni=new combuststruct;
         ni->next=next; next=ni; ni->delptr=node;
         return *this;
      }
      void combuststruct::combust()
      {
        while (next!=NULL) //extreneous !=NULL, but I'm keeping it
        {
          combuststruct* pr=next;
          next=next->next;
          delete pr;
        }
      }
      
      combuststruct::combuststruct()
      {
        next=NULL;
        delptr=NULL;
      }
      combuststruct::~combuststruct()
      {
        delete delptr;
      }
    #endif
    
    
  /* Events *
  Storing events for in-order execution with no
  additional lookup for next viable event
  */


   #if INSTANCE_DEPTH_METHOD!=2
    fnode* depthnode::appendfnode(fnode* nta)
    {
      fnode* nnode;
      if (nta==NULL) nnode=new fnode;
      else           nnode=nta;
      nnode->mydepth=this;
      
      if (start==NULL) //the thing was just created, or maybe was destroyed earlier
      {
        start=end=nnode;
        
        nnode->next=NULL;
        #if INSTANCE_DEPTH_METHOD==0
        std::map<int,depthnode*>::iterator looking=me;
        for (looking++; looking != mymap->end(); looking++)
         if (looking->second->start != NULL)
            { nnode->prev=looking->second->end; looking->second->end->next=nnode; break; } 
        #elif INSTANCE_DEPTH_METHOD==1
        for (depthnode* looking=next; looking != NULL; looking=looking->next) //Find next node
         if (looking->start != NULL)
            { nnode->next=looking->start; looking->start->prev=nnode; break; }
        #endif
        nnode->prev=NULL;
        #if INSTANCE_DEPTH_METHOD==0
        looking=me;
        if (looking!=mymap->begin())
        for (looking--; 1; looking--) {
         if (looking->second->start != NULL)
            { nnode->next=looking->second->start; looking->second->start->prev=nnode; break; }
         if (looking==mymap->begin()) break; }
        #elif INSTANCE_DEPTH_METHOD==1
        for (depthnode* looking=prev; looking != NULL; looking=looking->prev) //Find next node
         if (looking->end != NULL)
            { nnode->prev=looking->end; looking->end->next=nnode; break; }
        #endif
        return nnode;
      }
      else
      {
        nnode->next=end->next; 
        nnode->prev=end; 
        end->next=nnode;
        end=nnode;
        nnode->mydepth=this;
        return nnode;
      }
    }

    depthnode* eventnode::getdepth(int n_depth)
    {
      #if INSTANCE_DEPTH_METHOD==0
        std::map<int,depthnode*>::iterator exs = depths.find(n_depth);
        if (exs!=depths.end()) 
        return exs->second;
        
        depthnode* nnode=new depthnode;
        depths[n_depth]=nnode;
        nnode->me=depths.find(n_depth);
        nnode->mymap=&depths;
        nnode->myevent=this;
        return nnode;
      #elif INSTANCE_DEPTH_METHOD==1
        depthnode *looking=depths->next,*prevlook=depths,*nnode;
        
        while (looking!=NULL)
        {
          if (looking->depth==n_depth) return looking;
          if (looking->depth<n_depth) //Wait until smaller depth than this to add before
          {
            nnode=new depthnode; nnode->depth=n_depth;
            nnode->next=looking;
            prevlook->next=nnode;
            return nnode;
          }
          prevlook=looking; looking=looking->next;
        }
        //nothing smaller found, add to end
        nnode=new depthnode; nnode->depth=n_depth;
        nnode->next=NULL;
        nnode->prev=prevlook;
        prevlook->next=nnode;
        nnode->myevent=this;
        return nnode;
      #endif
    }
   #else
    fnode* eventnode::appendfnode()
    {
      fnode* nnode=new fnode;
      nnode->next=NULL;
      nnode->prev=end;
      if (end!=NULL) end->next=nnode;
      else start=nnode; //can assume what has no end has no beginning
      end=nnode;
      
      nnode->myevent=this;
      return nnode;
    }
   #endif

    eventnode* eventnode::getevent(int n_id)
    {
      eventnode *prev=this;
      eventnode *looking=next;
      eventnode *nnode;
      
      while (looking!=NULL)
      {
        if (looking->evid==n_id) return looking;
        if (looking->evid>n_id) //Wait until larger ID than event to add before
        {
          nnode=new eventnode; nnode->evid=n_id;
          nnode->next=looking;
          prev->next=nnode;
          return nnode;
        }
        prev=looking; looking=looking->next;
      }
      //nothing larger found, add to end
      nnode=new eventnode; nnode->evid=n_id;
      nnode->next=looking;
      prev->next=nnode;
      return nnode;
    }
    
    #if INSTANCE_DEPTH_METHOD!=2 
      depthnode::depthnode()
      {
        #if INSTANCE_DEPTH_METHOD==1 
        depth=-1;
        prev=NULL; next=NULL;
        #endif
        start=NULL; end=NULL;
      }
    #endif
    eventnode::eventnode()
    {
      evid=-1;
      #if INSTANCE_DEPTH_METHOD==1 
        depths=new depthnode; //allocate the root of the list it points to
      #elif INSTANCE_DEPTH_METHOD==2
        start=end=NULL;
      #endif
      next=NULL; //this indicates end-of-list
    }
    #if INSTANCE_DEPTH_METHOD!=2
      depthnode::~depthnode()
      {
      }
    #endif
    eventnode::~eventnode()
    { /* lolz, this shouldn't happen :3 */ }
    
    #if INSTANCE_DEPTH_METHOD!=2
      void depthnode::erase()
      {
        #if INSTANCE_DEPTH_METHOD==0
          mymap->erase(me);
        #elif INSTANCE_DEPTH_METHOD==1
          if (next!=NULL) next->prev=prev;
          if (prev!=NULL) prev->next=next;
        #endif
        start=end=NULL;
      }
    #endif
    
    void fnode::erase()
    {
      #if INSTANCE_DEPTH_METHOD!=2
        //unsassociate with depthnodes
        if (mydepth->start==this)
        {
          if (mydepth->end==this)
          {
            mydepth->erase();
          }
          else //fix mydepth->start
          {
            //(next->mydepth==mydepth) has to be true
            mydepth->start=next; 
          }
        }
        else if (mydepth->end==this)
        {
          //(prev->mydepth==mydepth) has to be true
          mydepth->end=prev;
        }
      #else
        if (myevent->start==this)
        {
          myevent->start=myevent->end=NULL;
        }
        else if (myevent->end==this)
        myevent->end=prev;
      #endif
      //patch hole in the spacetime
      if (next!=NULL) next->prev=prev;
      if (prev!=NULL) prev->next=next;
    }
    
    #if INSTANCE_DEPTH_METHOD!=2
      void fnode::movedepth(int ndep)
      {
        #if INSTANCE_DEPTH_METHOD==0
          if (ndep==mydepth->me->first) return;
          erase();
          mydepth->myevent->getdepth(ndep)->appendfnode(this);
        #elif INSTANCE_DEPTH_METHOD==1
          if (ndep==mydepth->depth) return;
          erase();
          mydepth->myevent->getdepth(ndep)->appendfnode(this);
        #endif
      }
    #endif
    
    fnode::~fnode()
    {
      #if INSTANCE_DEPTH_METHOD!=2
        //free my depth's node if I unlinked it at erase()
        if (mydepth->start==NULL)
        {
          delete mydepth;
        }
      #else
        if (myevent->start==NULL)
        {
          delete myevent;
        }
      #endif
    }
