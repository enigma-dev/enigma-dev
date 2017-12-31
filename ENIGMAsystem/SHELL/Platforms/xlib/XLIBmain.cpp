/** Copyright (C) 2008-2011 IsmAvatar <cmagicj@nni.com>, Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2014 Seth N. Hetu
*** Copyright (C) 2015 Harijs Grinbergs
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "XLIBmain.h"
#include "XLIBwindow.h"
#include "LINUXjoystick.h"

#include "Platforms/platforms_mandatory.h"
#include "Platforms/POSIX/GameLoop.h"

#include "Universal_System/var4.h"
#include "Universal_System/CallbackArrays.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/loading.h"

#include <unistd.h> //getcwd

namespace enigma
{
  void (*WindowResizedCallback)();
  XVisualInfo* CreateVisualInfo();

  namespace x11
  {
    Display *disp;
    Screen *screen;
    Window win;
    Atom wm_delwin;

    void handleEvents()
    {
      XEvent e;
      XNextEvent(disp, &e);
      int gk; unsigned char actualKey;
      switch (e.type)
      {
        case KeyPress: {
              gk=XLookupKeysym(&e.xkey,0);
              if (gk==NoSymbol)
                break;

              if (!(gk & 0xFF00)) actualKey = enigma_user::keyboard_get_map((int)enigma::keymap[gk & 0xFF]);
              else actualKey = enigma_user::keyboard_get_map((int)enigma::keymap[gk & 0x1FF]);
              { // Set keyboard_lastchar. Seems to work without
                char str[1];
                int len = XLookupString(&e.xkey, str, 1, NULL, NULL);
                if (len > 0) {
                  enigma_user::keyboard_lastchar = string(1,str[0]);
                  if (actualKey == enigma_user::vk_backspace) {
                    enigma_user::keyboard_string = enigma_user::keyboard_string.substr(0, enigma_user::keyboard_string.length() - 1);
                  } else {
                    enigma_user::keyboard_string += enigma_user::keyboard_lastchar;
                  }
                }
              }
              enigma_user::keyboard_lastkey = actualKey;
              enigma_user::keyboard_key = actualKey;
              if (enigma::last_keybdstatus[actualKey]==1 && enigma::keybdstatus[actualKey]==0) {
                enigma::keybdstatus[actualKey]=1;
                break;
              }
              enigma::last_keybdstatus[actualKey]=enigma::keybdstatus[actualKey];
              enigma::keybdstatus[actualKey]=1;
              break;
        }
        case KeyRelease: {
            enigma_user::keyboard_key = 0;
            gk=XLookupKeysym(&e.xkey,0);
            if (gk == NoSymbol)
              break;

            if (!(gk & 0xFF00)) actualKey = enigma_user::keyboard_get_map((int)enigma::keymap[gk & 0xFF]);
            else actualKey = enigma_user::keyboard_get_map((int)enigma::keymap[gk & 0x1FF]);

            enigma::last_keybdstatus[actualKey]=enigma::keybdstatus[actualKey];
            enigma::keybdstatus[actualKey]=0;
          break;
        }
        case ButtonPress: {
            if (e.xbutton.button < 4) enigma::mousestatus[e.xbutton.button == 1 ? 0 : 4-e.xbutton.button] = 1;
            else switch (e.xbutton.button) {
              case 4: enigma_user::mouse_vscrolls++; break;
              case 5: enigma_user::mouse_vscrolls--; break;
              case 6: enigma_user::mouse_hscrolls++; break;
              case 7: enigma_user::mouse_hscrolls--; break;
              default: ;
            }
          break;
        }
        case ButtonRelease: {
            if (e.xbutton.button < 4) enigma::mousestatus[e.xbutton.button == 1 ? 0 : 4-e.xbutton.button] = 0;
            else switch (e.xbutton.button) {
              case 4: enigma_user::mouse_vscrolls++; break;
              case 5: enigma_user::mouse_vscrolls--; break;
              case 6: enigma_user::mouse_hscrolls++; break;
              case 7: enigma_user::mouse_hscrolls--; break;
              default: ;
            }
          break;
        }
        case ConfigureNotify: {
          enigma::windowWidth = e.xconfigure.width;
          enigma::windowHeight = e.xconfigure.height;

          //NOTE: This will lead to a loop, and it seems superfluous.
          //enigma::setwindowsize();

          if (WindowResizedCallback != NULL) {
            WindowResizedCallback();
          }
          break;
        }
        case FocusIn:
          gameWindowFocused = true;
          pausedSteps = 0;
          break;
        case FocusOut:
          gameWindowFocused = false;
          break;
        case ClientMessage:
          if ((Atom)e.xclient.data.l[0] == wm_delwin) //For some reason, this line warns whether we cast to unsigned or not.
          {
            enigma_user::game_end(0);
            break;
          }
          //else fall through
        default:
            //printf("%d\n",e.type);
          break;
      }
      //Move/Resize = ConfigureNotify
      //Min = UnmapNotify
      //Restore = MapNotify
    }
  }

void platformEvents()
{
  while (XQLength(enigma::x11::disp) || XPending(enigma::x11::disp))
    enigma::x11::handleEvents();

  enigma::handle_joysticks();
}

}
using namespace enigma::x11;

int main(int argc,char** argv)
{
    // Set the working_directory
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) != NULL)
       fprintf(stdout, "Current working dir: %s\n", buffer);
    else
       perror("getcwd() error");
    enigma_user::working_directory = string( buffer );

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

    // any normal person would know that this should be deleted but the OpenGL bridge does not want it deleted, please beware
    XVisualInfo* vi = enigma::CreateVisualInfo();

    // Window event listening and coloring
    XSetWindowAttributes swa;
    swa.border_pixel = 0;
    swa.background_pixel = (enigma::windowColor & 0xFF000000) | ((enigma::windowColor & 0xFF0000) >> 16) | (enigma::windowColor & 0xFF00) | ((enigma::windowColor & 0xFF) << 16);
    swa.colormap = XCreateColormap(disp,root,vi->visual,AllocNone);
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | FocusChangeMask | StructureNotifyMask;
    unsigned long valmask = CWColormap | CWEventMask | CWBackPixel; // | CWBorderPixel;

    //prepare window for display (center, caption, etc)
    screen = DefaultScreenOfDisplay(disp);
    //default window size
    int winw = enigma_user::room_width;
    int winh = enigma_user::room_height;
    // By default if the room is too big instead of creating a gigantic ass window
    // make it not bigger than the screen to full screen it, this is what 8.1 and Studio
    // do, if the user wants to manually override this they can using
    // views/screen_set_viewport or window_set_size/window_set_region_size
    // We won't limit those functions like GM, just the default.
    if (winw > screen->width) winw = screen->width;
    if (winh > screen->height) winh = screen->height;

    //Make the window
    win = XCreateWindow(disp,root,0,0,winw,winh,0,vi->depth,InputOutput,vi->visual,valmask,&swa);
    XMapRaised(disp,win); //request visible

    //printf("Screen: %d %d %d %d\n",s->width/2,s->height/2,winw,winh);
    XMoveWindow(disp,win,(screen->width-winw)/2,(screen->height-winh)/2);

    enigma::EnableDrawing();

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
    
   enigma::gmw_init(); //init gm window functions, flushes
  //#include "initialize.h"

  int ret = enigma::gameLoop(argc, argv);

  XCloseDisplay(disp);

  return ret;
  
}