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

#include <windowsx.h>//Cross plat this

namespace buildmode{
char paused=0,shifted=0;
char unpausing=0,pausing=0,stepping=0,stepdown=0;
int take_a_step=-1;
int gridx=16,gridy=16;
unsigned char* scrpixels;
int dragid=-4;
WNDCLASS wbuild; HWND buildhwnd; HINSTANCE Bhinst;
HWND freeze,pause,undo,redo,step,object,gxin,gyin,grid,resume;
struct instchng{int x,y,id,type;};
std::map<int,int> changecount;
std::map<int,std::map<int,instchng> > changes;
std::map<int,std::map<int,instchng> >::iterator rciter;
std::map<int,instchng>::iterator citer;
void bmain()
{
	enigma::update_globals();
	int w=window_get_width(),h=window_get_height();
	if(pausing)
	{
		if(enigma::keybdstatus[vk_space]) return;
	pausing=0;}
    if(enigma::keybdstatus[vk_enter]==1 || SendMessage(resume,BM_GETSTATE,0,0))
		unpausing=1;
    else if (unpausing)
    {
		paused=0;
		unpausing=0;
		delete[] scrpixels;
		return;
	}
	if((enigma::last_keybdstatus[vk_right]==1 && enigma::keybdstatus[vk_right]==0))
		stepping=1;
	else if (stepping) 
	{ stepping=0; take_a_step=1; }
	if((stepdown=SendMessage(step,BM_GETSTATE,0,0)==108)&& !stepdown) take_a_step=1;
	if(shifted)
	{
		draw_clear(255);
		glRasterPos2f(0,w);
		glDrawPixels(w,h,GL_RGBA,GL_UNSIGNED_BYTE,scrpixels);
  }
	else screen_redraw(1);
	char itemtext[512];
	GetWindowText(object,itemtext,512);
	int objnum=-1;
	for (enigma::objiter=enigma::objectdata.begin();enigma::objiter!=enigma::objectdata.end();enigma::objiter++)
		if ((*enigma::objiter).second.name==itemtext)
		{
			objnum=(*enigma::objiter).first;
			break;
		}
	room_caption=string(objnum)+" "+itemtext;
	if(!enigma::keybdstatus[vk_control])
	{
		dragid=-4;
		if(objnum != -1)
		{
			if(!enigma::mousestatus[0] && enigma::objectdata[objnum].sprite_index != -1)
			{
				int xx=mouse_x,yy=mouse_y;
				if(!enigma::keybdstatus[vk_alt])
				{
					xx=gridx*(int)(xx/gridx);
					yy=gridy*(int)(yy/gridy);
				}
				draw_sprite(enigma::objectdata[objnum].sprite_index,0,xx,yy);
			}
		} else 
		{
			if(!enigma::last_mousestatus[0] || enigma::keybdstatus[vk_shift])
			{
				int xx=mouse_x,yy=mouse_y;
				if(!enigma::keybdstatus[vk_alt])
				{
					xx=gridx*(int)(xx/gridx);
					yy=gridy*(int)(yy/gridy);
				}
				int insthere=0;
				for(int i=0;i<changecount[room];i++)
					if(changes[room][i].x==xx && changes[room][i].y==yy)
					{
						insthere=1;
						break;
					}
				if(!insthere)
				{
          //add to room changes
          int nobj=instance_create(xx,yy,objnum);
          changes[room][changecount[room]].x=xx;
          changes[room][changecount[room]].y=yy;
          changes[room][changecount[room]].id=objnum;
          changes[room][changecount[room]].type=0;
          changecount[room]++;
          //add to room data
          enigma::roomdata[room].instances[enigma::roomdata[room].instancecount++].x=xx;
          enigma::roomdata[room].instances[enigma::roomdata[room].instancecount++].y=yy;
          enigma::roomdata[room].instances[enigma::roomdata[room].instancecount++].id=nobj;
          enigma::roomdata[room].instances[enigma::roomdata[room].instancecount++].obj=objnum;
				}
			}
		}
	}
	else
	{
		if(!enigma::last_mousestatus[0])
		{
			dragid=-4;
			for(enigma::instance_iterator= enigma::instance_list.begin();enigma::instance_iterator!=enigma::instance_list.end();enigma::instance_iterator++)
			{
				enigma::object *fobj=(*enigma::instance_iterator).second;
				if(fobj)
				{
					int bx=fobj->sprite_width,by=fobj->sprite_height;
					if(bx<16) bx=16;
					if(by<16) by=16;
					if(mouse_x>fobj->x && mouse_y>fobj->y && mouse_x<fobj->x+bx && mouse_y<fobj->x+by)
					{
						dragid=(*enigma::instance_iterator).first;
						break;
					}
				}
			}
		}else if (dragid>0)
		{
			enigma::instance_iterator=enigma::instance_list.find(dragid);
			if (enigma::instance_iterator==enigma::instance_list.end())
				dragid=-4;
			else
			{
				enigma::object *fobj=(*enigma::instance_iterator).second;
				fobj->x=mouse_x; fobj->y=mouse_y;
				bool instthere=0;
				for(int i=0; i<enigma::roomdata[room].instancecount;i++)
					if(enigma::roomdata[room].instances[i].id==dragid)
					{
						instthere=1;
						break;
					}
				if(instthere)
				{
					int armoved=0;
					for(int i=0; i<changecount[room];i++)
						if (changes[room][i].id==dragid)
						{
							changes[room][i].x=(int)mouse_x;
							changes[room][i].y=(int)mouse_y;
							armoved=1;
							break;
						}
					if(!armoved)
					{
						changes[room][changecount[room]].x=(int)mouse_x;
						changes[room][changecount[room]].y=(int)mouse_y;
						changes[room][changecount[room]].id=dragid;
						changes[room][changecount[room]].type=1;
						changecount[room]++;
					}
				}else
				{
					for (int i=0; i<changecount[room];i++)
						if (changes[room][i].id==dragid && changes[room][i].type==0)
						{
							changes[room][i].x=(int)mouse_x;
							changes[room][i].y=(int)mouse_y;
							break;
						}
				}
			}
		}
	}
    if (!enigma::last_mousestatus[1] && enigma::mousestatus[1])
    {
		int fnd=-4;
		for (enigma::instance_iterator= enigma::instance_list.begin();enigma::instance_iterator!=enigma::instance_list.end();enigma::instance_iterator++)
		{
			enigma::object *fobj=(*enigma::instance_iterator).second;
			if (fobj)
			{
				int bx=fobj->sprite_width,by=fobj->sprite_height;
				if (bx<16) bx=16; if (by<16) by=16;
				if ((mouse_x>fobj->x) && (mouse_y>fobj->y) && (mouse_x<(fobj->x+bx)) && (mouse_y<(fobj->x+by)))
				{
					fnd=(*enigma::instance_iterator).first;
					break;
				}
			}
		}
		if (fnd>0)
		{
			instance_destroy(fnd);
			changes[room][changecount[room]].x=0;
			changes[room][changecount[room]].y=0;
			changes[room][changecount[room]].id=fnd;
			changes[room][changecount[room]].type=2;
			changecount[room]++;
		}
	}
    char gintx[16];
    GetWindowText(gxin,gintx,16);
    gridx=atoi(gintx);
    GetWindowText(gyin,gintx,16);
    gridy=atoi(gintx);

    if (SendMessage(grid,BM_GETCHECK,0,0))
    {
      glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT);
      glColor4f(1,1,1,1);
      if (view_enabled)
      {
        for (int vc=0; vc<7; vc++)
        if (view_visible[vc])
        {
           glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO);
           if (gridx>0) for (int i=(int)((double)view_xview[vc]/gridx)*gridx;i<view_xview[vc]+view_wview[vc];i+=gridx) draw_line(i,view_yview[vc],i,view_yview[vc]+view_hview[vc]);
           if (gridy>0) for (int i=(int)((double)view_yview[vc]/gridx)*gridy;i<view_yview[vc]+view_hview[vc];i+=gridy) draw_line(view_xview[vc],i,view_xview[vc]+view_wview[vc],i);
        }
      }
      else
      {
         glBlendFunc(GL_ONE_MINUS_DST_COLOR,GL_ZERO);
         if (gridx>0) for (int i=0;i<room_width;i+=gridx) draw_line(i,0,i,h);
         if (gridy>0) for (int i=0;i<room_height;i+=gridy) draw_line(0,i,w,i);
      }

      glPopAttrib();
    }

    SwapBuffers(enigma::window_hDC);
  }







  bool checkpause()
  {
    int w=window_get_width(),h=window_get_height();
    
    if (take_a_step>0)
    {
      take_a_step--;
      return 0;
    }
    if (take_a_step==0)
    {
      take_a_step--;
      delete[]scrpixels;
      scrpixels=new unsigned char[w*h*4+1];
      glReadPixels(0,0,w,h,GL_RGBA,GL_UNSIGNED_BYTE,scrpixels);
      return 1;
    }
    if (paused==64)
      return 1;
    if ((enigma::keybdstatus[vk_control]==1 && enigma::last_keybdstatus[vk_space]==0 && enigma::keybdstatus[vk_space]==1)
    ||  ((SendMessage(pause,BM_GETSTATE,0,0)) || (SendMessage(freeze,BM_GETSTATE,0,0))))
    {
      pausing=1;

      scrpixels=new unsigned char[w*h*4+1];
      glReadPixels(0,0,w,h,GL_RGBA,GL_UNSIGNED_BYTE,scrpixels);

      shifted=(enigma::keybdstatus[vk_shift] || (SendMessage(freeze,BM_GETSTATE,0,0)));

      paused=64;
      return 1;
    }
    return 0;
  }







  LRESULT CALLBACK buildProc(HWND h,UINT u,WPARAM w, LPARAM l)
  {
    switch (u)
    {
        case WM_CREATE:
            return 0;
        default:
            return DefWindowProc (h, u, w, l);
    }
  }

  void buildinit()
  {
     //Register window class
        wbuild.style = CS_OWNDC;
        wbuild.lpfnWndProc = buildProc;
        wbuild.cbClsExtra = 0;
        wbuild.cbWndExtra = 0;
        wbuild.hInstance = Bhinst;
        wbuild.hIcon = LoadIcon (NULL, IDI_APPLICATION);
        wbuild.hCursor = LoadCursor (NULL, IDC_ARROW);
        wbuild.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
        wbuild.lpszMenuName = NULL;
        wbuild.lpszClassName = "TBuild";
        RegisterClass (&wbuild);

     RECT r;
     r.left=64; r.top=48; r.right=320+64; r.bottom=96+48;
     AdjustWindowRect(&r, WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE | WS_SIZEBOX, FALSE);


     //Create the build mode main window
     buildhwnd = CreateWindow ("TBuild", "Build Mode Access Panel", WS_VISIBLE | DS_CONTROL, r.left, r.top, r.right-r.left, r.bottom-r.top, enigma::hWndParent, NULL, Bhinst, NULL);

     //Add a combo box to that with the object data in it
     object = CreateWindow ("COMBOBOX", "Combo1", WS_VISIBLE | CBS_DROPDOWNLIST | WS_CHILD, 8, 8, 304, 620, buildhwnd, NULL, Bhinst, NULL);
       for (unsigned int i=0;i<enigma::objectdata.size();i++)
       SendMessage((object),CB_ADDSTRING,0,(LPARAM)(LPCTSTR)(enigma::objectdata[i].name.c_str()));

     //Add various buttons
     pause = CreateWindow ("BUTTON", "Pause", WS_VISIBLE | WS_CHILD, 8, 40, 64, 24, buildhwnd, NULL, Bhinst, NULL);
     freeze= CreateWindow ("BUTTON", "Freeze", WS_VISIBLE | WS_CHILD, 8, 72, 64, 24, buildhwnd, NULL, Bhinst, NULL);

     undo = CreateWindow ("BUTTON", "Undo", WS_VISIBLE | WS_CHILD, 96, 40, 48, 24, buildhwnd, NULL, Bhinst, NULL);
     redo = CreateWindow ("BUTTON", "Redo", WS_VISIBLE | WS_CHILD, 96, 72, 48, 24, buildhwnd, NULL, Bhinst, NULL);


     gxin = CreateWindow ("EDIT", "16", WS_VISIBLE | WS_CHILD, 152, 40, 28, 16, buildhwnd, NULL, Bhinst, NULL);
     gyin = CreateWindow ("EDIT", "16", WS_VISIBLE | WS_CHILD, 184, 40, 28, 16, buildhwnd, NULL, Bhinst, NULL);

     grid = CreateWindow ("BUTTON", "Grid", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 152, 64, 60, 24, buildhwnd, NULL, Bhinst, NULL);
     SendMessage(grid,BM_SETCHECK,1,0);

     step = CreateWindow ("BUTTON", "Step", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 312-48, 40, 48, 56, buildhwnd, NULL, Bhinst, NULL);

  }





  void finishup()
  {
    FILE* chout=fopen(BUILDFILE,"wb");

    int version=1;
    fwrite(&version,4,1,chout);

    for (rciter=changes.begin();rciter!=changes.end();rciter++)
    {
       int rm=(*rciter).first;
       fwrite(&rm,4,1,chout); fwrite(&changecount[rm],4,1,chout);
       for (int i=0;i<changecount[rm];i++)
       {
          fwrite(&(changes[rm][i].type),4,1,chout);
          fwrite(&(changes[rm][i].id),4,1,chout);
          fwrite(&(changes[rm][i].x),4,1,chout);
          fwrite(&(changes[rm][i].y),4,1,chout);
       }
    }
    fclose(chout);
  }
}
