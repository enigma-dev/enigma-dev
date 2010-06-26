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
#include <math.h>
#include <string>

#include "var4.h"
#include "reflexive_types.h"

//#include "GAME_GLOBALS.h"
#include "ENIGMA_GLOBALS.h"
//#include "EGMstd.h"

#include "../Platforms/windows/WINDOWSwindow.h"
#include "../Platforms/windows/WINDOWSStd.h"
#include "../libEGMstd.h"
#include "instance_system.h"
#include "instance.h"
#include "planar_object.h"

extern std::string string(double); //TODO: Clean these up without EGMstd.


double room_first  = 0;
double room_last   = 0;
double room_speed  = 60;
double room_width  = 640;
double room_height = 480;
double room_persistent = 0;
std::string room_caption = "ENIGMA Engine";

double background_color=16777215;
double background_showcolor=1;

double view_angle   = 0;
double view_current = 0;
double view_enabled = 0;
double view_hborder[9] = {0,0,0,0,0,0,0,0};
double view_hport[9]   = {0,0,0,0,0,0,0,0};
double view_hspeed[9]  = {0,0,0,0,0,0,0,0};
double view_hview[9]   = {0,0,0,0,0,0,0,0};
double view_object[9]  = {0,0,0,0,0,0,0,0};
double view_vborder[9] = {0,0,0,0,0,0,0,0};
double view_visible[9] = {0,0,0,0,0,0,0,0};
double view_vspeed[9]  = {0,0,0,0,0,0,0,0};
double view_wport[9]   = {0,0,0,0,0,0,0,0};
double view_wview[9]   = {0,0,0,0,0,0,0,0};
double view_xport[9]   = {0,0,0,0,0,0,0,0};
double view_xview[9]   = {0,0,0,0,0,0,0,0};
double view_yport[9]   = {0,0,0,0,0,0,0,0};
double view_yview[9]   = {0,0,0,0,0,0,0,0};

namespace enigma
{
  struct inst {
    int x,y,id,obj;
  };
  struct viewstruct
  {
    int start_vis;
    int area_x,area_y,area_w,area_h;
    int port_x,port_y,port_w,port_h;
    int object2follow;
    int hborder,vborder,hspd,vspd;
  };
  struct roomstruct
  {
    std::string name;
    int backcolor;
    void(*createcode)();
    std::string cap;
    int width, height, spd;
    int views_enabled;
    viewstruct views[8];
    int instancecount;
    
    std::map<int,inst> instances;
    void gotome();
    roomstruct(): createcode(NULL) {}
  };
  
  int room_max;
  std::map<int,roomstruct> roomdata;
  std::map<int,enigma::roomstruct>::iterator roomiter;
  
  void roomstruct::gotome()
  {
    room_caption = cap;
    room_width   = width;
    room_height  = height;
    room_speed   = spd;
    
    background_color = backcolor;
    background_showcolor = (backcolor!=-1);
    
    view_enabled = views_enabled;
    
    for(int i=0;i<7;i++)
    {
      view_xview[i] = views[i].area_x; view_yview[i] = views[i].area_y; view_wview[i] = views[i].area_w; view_hview[i] = views[i].area_h;
      view_xport[i] = views[i].port_x; view_yport[i] = views[i].port_y; view_wport[i] = views[i].port_w; view_hport[i] = views[i].port_h;
      view_object[i] = views[i].object2follow;
      view_hborder[i] = views[i].hborder; view_vborder[i] = views[i].vborder; view_hspeed[i] = views[i].hspd; view_vspeed[i] = views[i].vspd;
      view_visible[i] = views[i].start_vis;
    }
    
    double xm=0,ym=0;
    if (view_enabled)
    {
      for (int i=0;i<7;i++)
        if (view_visible[i])
        {
          if(view_xview[i]+view_wview[i]>xm)
            xm=(int)(view_xview[i]+view_wview[i]);
          if(view_yview[i]+view_hview[i]>ym)
            ym=(int)(view_yview[i]+view_hview[i]);
        }
    }
    else
    {
      xm=room_width;
      ym=room_height;
    }
    window_set_size(xm,ym);
    for (int i=0; i<instancecount; i++)
    {
      inst obj=instances[i];
      if (obj.id>enigma::maxid)
        enigma::maxid=obj.id;
      instance_create_id(obj.x,obj.y,obj.obj,obj.id);
    }
    createcode();
  }
}


//Implement the "room" global before we continue
int room_goto(double roomind);
  #define TYPEPURPOSE roomv
  #define TYPEVARIABLES
  #define TYPEFUNCTION room_goto((int)rval.d);
  #include "multifunction_variant_source.h"
  #undef TYPEFUNCTION
  #undef TYPEVARIABLES
  #undef TYPEPURPOSE
enigma::roomv room;


int room_goto(double roomind)
{
	int indx=(int)roomind;
	
	#if SHOWERRORS
	if(enigma::roomdata.find(indx)==enigma::roomdata.end())
	{
		show_error("Attempting to go to nonexisting room",0);
		return 0;
	}
	#endif
	
	//Destroy all objects
	enigma::nodestroy=1;
	for (enigma::inst_iter *it = enigma::instance_list_first(); it != NULL; it = it->next)
	{
		it->inst->myevent_roomend();
		if(!((enigma::object_planar*)(it->inst))->persistent)
      instance_destroy(it->inst->id);
	}
	enigma::nodestroy = 0;
	
	room.rval.d = indx;
	
	enigma::roomdata[indx].gotome();
	return 0;
}

int room_restart()
{
	int indx=(int)room.rval.d;
	
	#if SHOWERRORS
	if (enigma::roomdata.find(indx) == enigma::roomdata.end()) {
		show_error("Is this some kind of joke? <__<",0);
		return 0;
	}
  #endif
	
	//Destroy all objects
	enigma::nodestroy=1;
	for(enigma::inst_iter *it = enigma::instance_list_first(); it != NULL; it = it->next)
	{
		it->inst->myevent_roomend();
		#ifdef ISCONTROLLER_persistent
		if (!it->inst->persistent)
		#endif
		instance_destroy(it->inst->id);
	}
	enigma::nodestroy=0;
	enigma::roomdata[indx].gotome();
	return 0;
}

int room_goto_absolute(double index)
{
	enigma::roomiter = enigma::roomdata.begin();
	//for (int ii=0; ii<(int)index; ii++) if (enigma::roomiter==enigma::roomdata.end()) { break; } else enigma::roomiter++;
  
  #ifdef DEBUGMODE
	if (enigma::roomiter == enigma::roomdata.end())
	{
		if (enigma::roomdata.empty())
			show_error("Game must have at least one room to run",0);
		else show_error("Attempting to go to nonexisting room",0);
		return 0;
	}
  #endif
	int indx=(*enigma::roomiter).first;
	//Destroy all objects
	enigma::nodestroy=1;
	for (enigma::inst_iter *it = enigma::instance_list_first(); it != NULL; it = it->next)
	{
		it->inst->myevent_roomend();
		#ifdef ISCONTROLLER_persistent
		if(!it->inst->persistent)
		#endif
		instance_destroy(it->inst->id);
	}
	enigma::nodestroy=0;
	room.rval.d=indx;
	enigma::roomdata[indx].gotome();
	return 0;
}



int room_goto_first()
{
    enigma::roomiter=enigma::roomdata.begin();
    #ifdef DEBUGMODE
    if (enigma::roomiter==enigma::roomdata.end())
    {
        show_error("Game must have at least one room to run",0);
        return 0;
    }
    #endif

    int indx=(*enigma::roomiter).first;

    //Destroy all objects
    enigma::nodestroy=1;
    for (enigma::inst_iter *it = enigma::instance_list_first(); it != NULL; it = it->next)
    {
      it->inst->myevent_roomend();
      #ifdef ISCONTROLLER_persistent
      if (!it->inst->persistent)
      #endif
      instance_destroy(it->inst->id);
    }
    enigma::nodestroy=0;

    room.rval.d=indx;
    enigma::roomdata[indx].gotome();

    return 0;
}


int room_goto_next()
{
    enigma::roomiter=enigma::roomdata.find(int(room));
    #ifdef DEBUGMODE
    if (enigma::roomiter==enigma::roomdata.end())
    {
      show_error("Going to next room from unexisting room (?)",0);
      return 0;
    } //error like GM here
    #endif
    
    enigma::roomiter++;

    #ifdef DEBUGMODE
    if (enigma::roomiter==enigma::roomdata.end())
    {
      show_error("Going to next room after last",0);
      return 0;
    } //error like GM here
    #endif
    
    //Destroy all objects
    enigma::nodestroy = 1;
    for (enigma::inst_iter *it = enigma::instance_list_first(); it != NULL; it = it->next)
    {
      it->inst->myevent_roomend();
      #ifdef ISCONTROLLER_persistent
      if (!it->inst->persistent)
      #endif
      instance_destroy(it->inst->id);
    }
    enigma::nodestroy=0;

    room.rval.d = enigma::roomiter->first;
    enigma::roomiter->second.gotome();
    return 0;
}


int room_next(int num)
{
    enigma::roomiter=enigma::roomdata.find((int)num);
    if (enigma::roomiter == enigma::roomdata.end())
      return -1;
    enigma::roomiter++;
    if (enigma::roomiter==enigma::roomdata.end())
      return -1;
    return enigma::roomiter->first;
}
int room_previous(int num)
{
    enigma::roomiter=enigma::roomdata.find((int)num);
    if (enigma::roomiter==enigma::roomdata.end())
      return -1;
    enigma::roomiter--; 
    if (enigma::roomiter==enigma::roomdata.end())
      return -1;
    return enigma::roomiter->first;
}

#include "key_game_globals.h" //TODO: Remove all instances of this line. It's just sloppy. Figure out the dependencies manually.

namespace enigma
{
  void room_update()
  {
    window_set_caption(room_caption);
    if(view_enabled)
    {
      for(int i=0;i<7;i++)
      {
        if(mouse_x>=view_xport[i] && mouse_x<view_xport[i]+view_wport[i]
        && mouse_y>=view_yport[i] && mouse_y<view_yport[i]+view_hport[i]){
          mouse_x=view_xview[i]+((mouse_x-view_xport[i])/(double)view_wport[i])*view_wview[i];
          mouse_y=view_yview[i]+((mouse_y-view_yport[i])/(double)view_hport[i])*view_hview[i];
          break;
        }
      }
    }
  }
}
