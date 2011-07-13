/*
 * Copyright (C) 2008 IsmAvatar <cmagicj@nni.com>
 *
 * This file is part of ENIGMA.
 *
 * ENIGMA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ENIGMA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License (COPYING) for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

//This file has been modified beyond recognition by Josh @ Dreamland
//under the pretense that it would be better compatible with ENIGMA

#include <X11/Xlib.h>
#include <GL/glx.h>
#include <stdio.h>

#include "XLIBwindow.h"
#include "LINUXjoystick.h"

#include "../../Universal_System/var4.h"
#include "../../Universal_System/CallbackArrays.h"
#include "../../Universal_System/roomsystem.h"

namespace enigma
{
  extern char keymap[256];
  extern char usermap[256];
  void ENIGMA_events(void); //TODO: Synchronize this with Windows by putting these two in a single header.
  extern int initialize_everything();
  
  namespace x11
  {
    extern Display *disp;
    extern GLXDrawable win;
    extern Atom wm_delwin;
    
    int handleEvents()
    {
      XEvent e;
      XNextEvent(disp, &e);
      int gk; unsigned char actualKey;
      switch (e.type)
      {
        case KeyPress: {
              gk=XLookupKeysym(&e.xkey,0);
              if (gk==NoSymbol)
                return 0;

              if (!(gk & 0xFF00)) actualKey = enigma::usermap[gk];
              else actualKey = enigma::usermap[(int)enigma::keymap[gk & 0xFF]];

              if (enigma::last_keybdstatus[actualKey]==1 && enigma::keybdstatus[actualKey]==0) {
                enigma::keybdstatus[actualKey]=1;
                return 0;
              }
              enigma::last_keybdstatus[actualKey]=enigma::keybdstatus[actualKey];
              enigma::keybdstatus[actualKey]=1;
            return 0;
        }
        case KeyRelease: {
            gk=XLookupKeysym(&e.xkey,0);
            printf("Pressed a key: %d : %d\n", gk, gk & 0xFF);
            if (gk == NoSymbol)
              return 0;
            
            if (!(gk & 0xFF00)) actualKey = enigma::usermap[gk];
            else actualKey = enigma::usermap[(int)enigma::keymap[gk & 0xFF]];
            
            enigma::last_keybdstatus[actualKey]=enigma::keybdstatus[actualKey];
            enigma::keybdstatus[actualKey]=0;
          return 0;
        }
        case ButtonPress: {
            if (e.xbutton.button < 4) enigma::mousestatus[e.xbutton.button == 1 ? 0 : 4-e.xbutton.button] = 1;
            else switch (e.xbutton.button) {
              case 4: mouse_vscrolls++; break;
              case 5: mouse_vscrolls--; break;
              case 6: mouse_hscrolls++; break;
              case 7: mouse_hscrolls--; break;
            }
          return 0;
        }
        case ButtonRelease: {
            if (e.xbutton.button < 4) enigma::mousestatus[e.xbutton.button == 1 ? 0 : 4-e.xbutton.button] = 0;
            else switch (e.xbutton.button) {
              case 4: mouse_vscrolls++; break;
              case 5: mouse_vscrolls--; break;
              case 6: mouse_hscrolls++; break;
              case 7: mouse_hscrolls--; break;
            }
          return 0;
        }
        case Expose: {
            //screen_refresh();
          return 0;
        }
        case ClientMessage:
          if ((unsigned)e.xclient.data.l[0] == (unsigned)wm_delwin) //For some reason, this line warns whether we cast to unsigned or not.
            return 1;
          //else fall through
        default:
            printf("%d\n",e.type);
          return 0;
      }
      //Move/Resize = ConfigureNotify
      //Min = UnmapNotify
      //Restore = MapNotify
    }
  }
}

using namespace enigma::x11;

namespace enigma
{
  void initkeymap();
  void input_initialize()
  {
    //Clear the input arrays
    for(int i=0;i<3;i++){
      last_mousestatus[i]=0;
      mousestatus[i]=0;
    }
    for(int i=0;i<256;i++){
      last_keybdstatus[i]=0;
      keybdstatus[i]=0;
    }
    
    init_joysticks();
  }
  
  void input_push()
  {
    for(int i=0;i<3;i++){
      last_mousestatus[i] = mousestatus[i];
    }
    for(int i=0;i<256;i++){
      last_keybdstatus[i] = keybdstatus[i];
    }
    mouse_hscrolls = mouse_vscrolls = 0;
  }
  
  int game_ending();
}

int main(int argc,char** argv)
{
  // Copy our parameters
	enigma::parameters=new char* [argc];
	for (int i=0; i<argc; i++)
		enigma::parameters[i]=argv[i];
	enigma::initkeymap();


	// Initiate display
	disp = XOpenDisplay(NULL);
	if(!disp){
		printf("Display failed\n");
		return -1;
	}
	

	// Identify components (close button, root pane)
	wm_delwin = XInternAtom(disp,"WM_DELETE_WINDOW",False);
	Window root = DefaultRootWindow(disp);

	// Prepare openGL
	GLint att[] = { GLX_RGBA, GLX_DOUBLEBUFFER, None };
	XVisualInfo *vi = glXChooseVisual(disp,0,att);
	if(!vi){
		printf("GLFail\n");
		return -2;
	}

	// Window event listening and coloring
	XSetWindowAttributes swa;
	swa.border_pixel = 0;
	swa.background_pixel = 0;
	swa.colormap = XCreateColormap(disp,root,vi->visual,AllocNone);
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;// | StructureNotifyMask;
	unsigned long valmask = CWColormap | CWEventMask; //  | CWBackPixel | CWBorderPixel;

	//default window size
	int winw = room_width;
	int winh = room_height;
	win = XCreateWindow(disp,root,0,0,winw,winh,0,vi->depth,InputOutput,vi->visual,valmask,&swa);
	XMapRaised(disp,win); //request visible

	//prepare window for display (center, caption, etc)
	Screen *s = DefaultScreenOfDisplay(disp);
	//printf("Screen: %d %d %d %d\n",s->width/2,s->height/2,winw,winh);
	XMoveWindow(disp,win,(s->width-winw)/2,(s->height-winh)/2);

	//geom();
	//give us a GL context
	GLXContext glxc = glXCreateContext(disp, vi, NULL, True);
	if (!glxc){
		printf("NoContext\n");
		return -3;
	}

	//apply context
	glXMakeCurrent(disp,win,glxc); //flushes

	/* XEvent e;//wait for server to report our display request
	do {
	XNextEvent(disp, &e); //auto-flush
	} while (e.type != MapNotify);*/

	//register CloseButton listener
	Atom prots[] = {wm_delwin};
	if (!XSetWMProtocols(disp,win,prots,1)) {
		printf("NoClose\n");
		return -4;
	}
	gmw_init(); //init gm window functions, flushes
	//#include "../../initialize.h"

	//Call ENIGMA system initializers; sprites, audio, and what have you
	enigma::initialize_everything();

	/*
	for(char q=1;q;ENIGMA_events())
		while(XQLength(disp))
			if(handleEvents()>0) q=0;
	glxc = glXGetCurrentContext();
	glXDestroyContext(disp,glxc);
	XCloseDisplay(disp);
	return 0;*/
	
	for(;;)
	{
		while(XQLength(disp))
			if(handleEvents() > 0)
				goto end;
    enigma::handle_joysticks();
		enigma::ENIGMA_events();
		enigma::input_push();
	}
	
	end:
	enigma::game_ending();
  glXDestroyContext(disp,glxc);
  XCloseDisplay(disp);
	return 0;
}

