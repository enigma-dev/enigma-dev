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

#include <stdio.h> //printf, NULL
#include <stdlib.h> //malloc
#include <unistd.h> //usleep
#include <time.h> //clock
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/Xos.h>
//#include <X11/Xatom.h>
//#include <X11/keysym.h>
#include <GL/glx.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

Display *disp;
Window win;
GC gc;
Atom wm_delwin;

#include "xwindow_gml.h"

void repaint() {
 XWindowAttributes wa;
 XGetWindowAttributes(disp,win,&wa);
 glViewport(0,0,wa.width,wa.height);
 glClearColor(0,1,0,1); //rgba
 glClear(GL_COLOR_BUFFER_BIT);

 glBegin(GL_LINES);
  glColor4f(1,0,0,1);
  glVertex2f(-1,-1);
  glColor4f(1,1,0,1);
  glVertex2f(1,1);
  glColor4f(0,0,1,1);
  glVertex2f(1,-1);
  glColor4f(0,1,0,1);
  glVertex2f(-1,1);
 glEnd();
 glXSwapBuffers(disp,win);
}

int curse = 0;
void nextCurse() {
 XUndefineCursor(disp,win);
 char str[16];
 sprintf(str,"%d",curse);
 window_set_caption(str);
 XDefineCursor(disp,win,XCreateFontCursor(disp,curse++));
}

char exit_on_esc = 1;
int room_speed = 30;
void processEvents() {

  XEvent e;
  while (1) // FIXME: WHAT THE HELL
  {
    if (XQLength(disp) == 0)
    {
      int wait = 1000 / room_speed;
      while (wait > 100)
      {
        int cl = clock() * 1000 / CLOCKS_PER_SEC;
        glXSwapBuffers(disp,win);
        cl = clock() * 1000 / CLOCKS_PER_SEC - cl;
        Sleep(100 - cl);
        wait -= 100;
      }
      
      glViewport(0,0,window_get_width(),window_get_height());
      glClearColor(0,1,0,1); //rgba
      glClear(GL_COLOR_BUFFER_BIT);
      
      usleep(10000);
      glBegin(GL_LINES);
      glColor4f(1,0,0,1);
      glVertex2f(-1,-1);
      glColor4f(1,1,0,1);
      glVertex2f(1,1);
      glColor4f(0,0,1,1);
      glVertex2f(1,-1);
      glColor4f(0,1,0,1);
      glVertex2f(-1,1);
      glEnd();
      
      glXSwapBuffers(disp,win);
      XFlush(disp);
      usleep(1000000/30);
      Sleep(wait);
      continue;
    }
    XNextEvent(disp, &e);
    switch (e.type) 
    {
      case KeyPress: {
        printf("Keypress. Scan: %d;  XLookupKeysym: %d\n",e.xkey.keycode,XLookupKeysym(&e.xkey,0));
        nextCurse();
        break;
      }
      case Expose: {
        repaint();
        break;
      }
      case ClientMessage:
      if (e.xclient.data.l[0] == wm_delwin)
        return;
      //else overflow to default
      default:
        printf("%d\n",e.type);
      break;
    }
    //Move/Resize = ConfigureNotify
    //Min = UnmapNotify
    //Restore = MapNotify
  }
}

int main() {
 //initiate display
 disp = XOpenDisplay(NULL);
 if (disp == NULL) {
  printf("Display failed\n");
  return -1;
 }

 //identify components (close button, root pane)
 wm_delwin = XInternAtom(disp,"WM_DELETE_WINDOW",False);
 Window root = DefaultRootWindow(disp);

 //Prepare openGL
 GLint att[] = { GLX_RGBA, GLX_DOUBLEBUFFER, None };
 XVisualInfo *vi = glXChooseVisual(disp,0,att);
 if (vi == NULL) {
  printf("GLFail\n");
  return -2;
 }

 //window event listening and coloring
 XSetWindowAttributes swa;
 swa.border_pixel = 0;
 swa.background_pixel = 0;
 swa.colormap = XCreateColormap(disp,root,vi->visual,AllocNone);
 swa.event_mask = ExposureMask | KeyPressMask;// | StructureNotifyMask;
 unsigned long valmask = CWColormap | CWEventMask;
                     //  | CWBackPixel | CWBorderPixel;

 //default window size
 int winw = 200;
 int winh = 100;

 win = XCreateWindow(disp,root,0,0,winw,winh,0,vi->depth,
                     InputOutput,vi->visual,valmask,&swa);
 XMapRaised(disp,win); //request visible

 //prepare window for display (center, caption, etc)
 Screen *s = DefaultScreenOfDisplay(disp);
 XMoveWindow(disp,win,s->width/2-winw/2,s->height/2-winh/2);
 window_set_caption("Happy");
 geom();





 //give us a GL context
 GLXContext glxc = glXCreateContext(disp, vi, NULL, True);
 if (!glxc) {
  printf("NoContext.\n");
  return -3;
 }

 //apply context
 glXMakeCurrent(disp,win,glxc); //flushes

 //wait for server to report our display request
/* XEvent e;
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

//
  printf("Going process...\n");
 processEvents();

 glxc = glXGetCurrentContext();
 glXDestroyContext(disp,glxc);
 XCloseDisplay(disp);

 return 0;
}

