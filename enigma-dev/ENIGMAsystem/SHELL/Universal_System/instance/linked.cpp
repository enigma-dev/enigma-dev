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


//int ocount=0,ncount=0,ccount=0;
#include <stdlib.h>
#include <stdio.h>
#include "GLOBAL_GAME_SETTINGS.h"
#include <map>

using namespace std;

#include "INSTANCE_PARENT.h"
#include "instances.h"
#include "cleanup.h"
#include "events.h"

eventnode* event_list;
instlist* instance_list;

#include "IDE_edit_instancode.h"

/*
Test code: test the operability and flexibility of the system
*/





#if INSTANCE_DEPTH_METHOD==1
#define evtest \
  printf("Event test. Old method, go\r\n");\
  foreach(it) { ENIGMACRO_ACCESSOBJPRT(it)->step(); }\
  printf("New method, go\r\n");\
  if (event_list->next->depths->next!=NULL)\
  for(fnode* cur=event_list->next->depths->next->start;cur!=NULL;cur=cur->next)\
  { cur->inst->step(); lcur=cur; }
#elif INSTANCE_DEPTH_METHOD==0
#define evtest \
  printf("Event test. Old method, go\r\n");\
  foreach(it) { ENIGMACRO_ACCESSOBJPRT(it)->step(); }\
  printf("New method, go\r\n");\
  if (!event_list->next->depths.empty())\
  for(fnode* cur=event_list->next->depths.rbegin()->second->start;cur!=NULL;cur=cur->next)\
  { cur->inst->step(); lcur=cur; }
#elif INSTANCE_DEPTH_METHOD==2
#define evtest \
  printf("Event test. Old method, go\r\n");\
  foreach(it) { ENIGMACRO_ACCESSOBJPRT(it)->step(); }\
  printf("New method, go\r\n");\
  for(fnode* cur=event_list->next->start;cur!=NULL;cur=cur->next)\
  { cur->inst->step(); lcur=cur; }
#endif


int main(int argc, char *argv[])
{
    event_list = new eventnode;
    instance_list = new instlist;
    
    printf("Count to ten \r\n");
    instnode* aft;
    for (int i=1;i<=10;i++)
    {
      aft=instance_list->ENIGMACRO_APPENDINST(i);
      aft->object=new objects;
      aft->object->id=i;
    }
    
    institer it; fnode*lcur;
    
    foreach(it) 
    printf("%d\r\n",ENIGMACRO_ACCESSOBJPRT(it)->id);
    
    
    
    evtest
    
    
    printf("Erase 6, 1, and 10\r\n");
    combuststruct clean;
    printf("Things I'm about to erase are the same: %d\r\n",((objects*)instance_list->findbyid(1)->object)->fnode_step==((objects*)instance_list->findbyid(6)->object)->fnode_step);
    clean+= instance_list->findbyid(6)->destroy();
    clean+= instance_list->findbyid(1)->destroy();
    clean+= instance_list->findbyid(10)->destroy();
    foreach(it)
    printf("%d\r\n",ENIGMACRO_ACCESSOBJPRT(it)->id);
    
    printf("Asploding\r\n");
    clean.combust();
    
    
    
    printf("Attendance\r\n");
    foreach(it)
    printf("%d\r\n",ENIGMACRO_ACCESSOBJPRT(it)->id);
    
    printf("Memory check: (y/n)");//(o%d, n%d, c%d) ",ocount,ncount,ccount);
    if (getchar()=='y')
    {
      for (int i=11;i<1000;i++)
      {
        instnode*a=instance_list->ENIGMACRO_APPENDINST(i);
        a->object=new objects;
        clean+=a->destroy();
      }
      printf("Memory check: Memory should be at peak. "/*(o%d, n%d, c%d) ",ocount,ncount,ccount*/); system("PAUSE");
      clean.combust();
      printf("Memory check: Memory should be at minimum. "/*(o%d, n%d, c%d) ",ocount,ncount,ccount*/); system("PAUSE");
    }
    
    
    printf("\r\nWipe all\r\n");
    foreach(it)
        clean+=ENIGMACRO_ACCESSINSTNODE(it)->destroy();
    printf("End wipe\r\n");
    
    evtest
    clean.combust();
    printf("Combust:\r\n");
    evtest
    
    printf("\r\nDo this \r\n");
    for (int i=1;i<=10;i+=2) {
    ((aft=instance_list->ENIGMACRO_APPENDINST(i))->object=new objects)->id=i;
    ((aft=instance_list->ENIGMACRO_APPENDINST(i+1))->object=new objects2)->id=i+1; }
    
    evtest
    
    printf("\r\n\r\nDelete 6\r\n");
    clean+= instance_list->findbyid(6)->destroy();
    evtest  printf("\r\n\rCombust\r\n");
    clean.combust();
    evtest
    
    
    
    printf("\r\nKey Test: Deletion while iterating\r\n"); system("Pause"); int asd=0;
#if INSTANCE_DEPTH_METHOD==1
me1:
  if (event_list->next->depths->next!=NULL)
  for(fnode* cur=event_list->next->depths->next->start;cur!=NULL;cur=cur->next)
  { if (cur->inst->id==4) clean+=instance_list->findbyid(4)->destroy(); cur->inst->step(); lcur=cur; }
  if (asd==0) { asd=1; printf("Reiterate\r\n"); goto me1; }
#elif INSTANCE_DEPTH_METHOD==0
me1:
  if (!event_list->next->depths.empty())
  for(fnode* cur=event_list->next->depths.rbegin()->second->start;cur!=NULL;cur=cur->next)
  { if (cur->inst->id==4) clean+=instance_list->findbyid(4)->destroy(); cur->inst->step(); lcur=cur; }
  if (asd==0) { asd=1; printf("Reiterate\r\n"); goto me1; }
#elif INSTANCE_DEPTH_METHOD==2
me1:
  for(fnode* cur=event_list->next->start;cur!=NULL;cur=cur->next)
  { if (cur->inst->id==4) clean+=instance_list->findbyid(4)->destroy(); cur->inst->step(); lcur=cur; }
  if (asd==0) { asd=1; printf("Reiterate\r\n"); goto me1; }
#endif
    
    
    clean.combust();
    
    
    #if INSTANCE_DEPTH_METHOD != 2
      printf("\r\nKey Test: Moving from depth to depth: node five will be moved to depth 1\r\n"); system("Pause");
      instance_list->findbyid(5)->object->move_depth(1);
      #if INSTANCE_DEPTH_METHOD==1
        if (event_list->next->depths->next!=NULL)
        for(fnode* cur=event_list->next->depths->next->start;cur!=NULL;cur=cur->next)
        { if (cur->inst->id==4) clean+=instance_list->findbyid(4)->destroy(); cur->inst->step(); lcur=cur; }
      #elif INSTANCE_DEPTH_METHOD==0
        if (!event_list->next->depths.empty())
        for(fnode* cur=event_list->next->depths.rbegin()->second->start;cur!=NULL;cur=cur->next)
        { if (cur->inst->id==4) clean+=instance_list->findbyid(4)->destroy(); cur->inst->step(); lcur=cur; }
      #elif INSTANCE_DEPTH_METHOD==2
        for(fnode* cur=event_list->next->start;cur!=NULL;cur=cur->next)
        { if (cur->inst->id==4) clean+=instance_list->findbyid(4)->destroy(); cur->inst->step(); lcur=cur; }
      #endif
    #endif
    
    
    printf("\r\n\r\n------------------------------------\r\nTest over. ");
    system("PAUSE");
    return EXIT_SUCCESS;
}

