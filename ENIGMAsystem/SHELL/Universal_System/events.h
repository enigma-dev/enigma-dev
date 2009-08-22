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

/*****************************************************

int show_error(std::string errortext,double fatal)

******************************************************/

namespace enigma
{
   int event_current=-1;

   std::string evname(int evnumb)
   {
     switch (evnumb)
     {
        case -3: return "Game initialize";
        case -2: return "Initialize";
        case -1: return "No particular";
        case 0: return "Parent Begin Step";
        case 1: return "Room Start";
        case 2: return "Begin Step";
        case 3: return "Alarm";
        case 4: return "Keyboard";
        case 5: return "Key Press";
        case 6: return "Key Release";
        case 7: return "Mouse";
        case 8: return "Step";
        case 9: return "Path End";
        case 10: return "Outside Room";
        case 11: return "Instersect Boundary";
        case 12: return "Collision";
        case 13: return "No More Lives";
        case 14: return "No More Health";
        case 15: return "End Step";
        case 16: return "Draw";
        case 17: return "Animation End";
        case 18: return "Room End";
        case 19: return "Game End";
        case 20: return "Parent End Step";
     }
     return "Unknown";
   }

   int getID()
   {
      if (instance_event_iterator != NULL)
      {
         if ((*instance_event_iterator).second != NULL)
         return (int)(*instance_event_iterator).second->object_index;
         else return -2;
      }
      return -1;
   }
}


int show_error(std::string errortext,double fatal)
{
    #ifdef ENIGMA_WS_WINDOWS
    if (MessageBox(NULL,("Error in "+enigma::evname(enigma::event_current)+" event for object number "+string(enigma::getID())+": \r\n"+errortext).c_str(),"Error",MB_ABORTRETRYIGNORE | MB_ICONERROR)==IDABORT)
    exit(0);
    #endif
    if (!fatal)
    printf("ERROR: %s\n",errortext.c_str());
    else { printf("FATAL ERROR: %s\n",errortext.c_str()); exit(0); }
    #if ABORTONALL
    exit(0);
    #endif

    return 0;
}


void ENIGMA_events(void)
{
 #if BUILDMODE
 if (!buildmode::checkpause())
 {
 #endif
 #if DEBUGMODE
 try
 {
 #endif

      enigma::update_globals();

      #ifdef ENIGMAEVENT_parent_bstep
      enigma::event_current=0;
         enigma::parent_beginstep();
      #endif

            #ifdef ENIGMAEVENT_roomstart
            enigma::event_current=1;
            for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
            {
              (*enigma::instance_event_iterator).second->myevent_roomstart;
            }
            enigma::update_globals();
            #endif

            #ifdef ENIGMAEVENT_beginstep
            enigma::event_current=2;
            for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
            {
              (*enigma::instance_event_iterator).second->myevent_beginstep();
            }
            enigma::update_globals();
            #endif

            #ifdef ENIGMAEVENT_alarm
            enigma::event_current=3;
            for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
            {
              (*enigma::instance_event_iterator).second->myevent_alarm();
            }
            enigma::update_globals();
            #endif
            
            {
                 #ifdef ENIGMAEVENT_keyboard
                 enigma::event_current=4;
                 for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
                 {
                   if ((*enigma::instance_event_iterator).second != NULL)
                   (*enigma::instance_event_iterator).second->myevent_keyboard();
                 }
                 enigma::update_globals();
                 #endif

                 #ifdef ENIGMAEVENT_keypress
                 enigma::event_current=5;
                 for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
                 {
                   (*enigma::instance_event_iterator).second->myevent_keypress ();
                 }
                 enigma::update_globals();
                 #endif

                 #ifdef ENIGMAEVENT_keyrelease
                 enigma::event_current=6;
                 for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
                 {
                   (*enigma::instance_event_iterator).second->myevent_keyrelease();
                 }
                 #endif
            }

            enigmacatchmouse();
            {
                 #ifdef ENIGMAEVENT_mouse
                 enigma::event_current=7;
                 for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
                 {
                   (*enigma::instance_event_iterator).second->myevent_mouse();
                 }
                 enigma::update_globals();
                 #endif
            }

            enigma::event_current=8;
            for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
            {
              (*enigma::instance_event_iterator).second->myevent_step();
            }
            enigma::update_globals();

            #ifdef ENIGMAEVENT_pathend
            enigma::event_current=9;
            for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
            {
              (*enigma::instance_event_iterator).second->myevent_pathend();
            }
            enigma::update_globals();
            #endif

            #ifdef ENIGMAEVENT_outsideroom
            enigma::event_current=10;
            for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
            {
              (*enigma::instance_event_iterator).second->myevent_outsideroom();
            }
            enigma::update_globals();
            #endif

            #ifdef ENIGMAEVENT_boundary
            enigma::event_current=11;
            for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
            {
              (*enigma::instance_event_iterator).second->myevent_boundary();
            }
            enigma::update_globals();
            #endif

            #ifdef ENIGMAEVENT_collision
            enigma::event_current=12;
            for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
            {
              (*enigma::instance_event_iterator).second->myevent_collision();
            }
            enigma::update_globals();
            #endif

            #ifdef ENIGMAEVENT_nomorelives
            enigma::event_current=13;
            for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
            {
              (*enigma::instance_event_iterator).second->myevent_nomorelives();
            }
            enigma::update_globals();
            #endif

            #ifdef ENIGMAEVENT_nomorehealth
            enigma::event_current=14;
            for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
            {
              (*enigma::instance_event_iterator).second->myevent_nomorehealth();
            }
            enigma::update_globals();
            #endif

            #ifdef ENIGMAEVENT_endstep
            enigma::event_current=15;
            for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
            {
              (*enigma::instance_event_iterator).second->myevent_endstep();
            }
            enigma::update_globals();
            #endif


//printtime(3);
            enigma::update_globals();
            enigma::event_current=16;
            screen_redraw();
            screen_refresh();
//printtime(4);


            #ifdef ENIGMAEVENT_endanimation
            enigma::event_current=17;
            for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
            {
              (*enigma::instance_event_iterator).second->myevent_endanimation();
            }
            enigma::update_globals();
            #endif

            #ifdef ENIGMAEVENT_roomend
            enigma::event_current=18;
            for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
            {
              (*enigma::instance_event_iterator).second->myevent_roomend();
            }
            enigma::update_globals();
            #endif

            #ifdef ENIGMAEVENT_gameend
            enigma::event_current=19;
            for (enigma::instance_event_iterator=enigma::instance_list.begin(); enigma::instance_event_iterator != enigma::instance_list.end(); enigma::instance_event_iterator++)
            {
              (*enigma::instance_event_iterator).second->myevent_gameend();
            }
            enigma::update_globals();
            #endif


      #ifdef ENIGMAEVENT_parent_estep
         enigma::parent_endstep();
      #endif

 #if DEBUGMODE
 }
 catch (int   a) { show_error("Exception thrown with id "+string(a),0); }
 catch (char* a) { show_error(a,0); }
 #endif
 #if BUILDMODE
 }
 else buildmode::bmain();
 #endif


 //(*enigma::instance_list).cleanup(); //Kill destroyed objects now that we are quite done with them.

 for (int i=0;i<256;i++)
 { enigma::last_keybdstatus[i]=enigma::keybdstatus[i]; }

 for (int i=0;i<3;i++) { enigma::last_mousestatus[i]=enigma::mousestatus[i]; }
 enigma::mousewheel=0;

 //fps=enigma::getfps();

 window_set_caption(room_caption);

 for (int i=0; i<enigma::cleancount; i++)
 {
    delete enigma::instance_list[enigma::cleanups[i]];
    enigma::instance_list.erase(enigma::cleanups[i]);
 }
 enigma::cleanups.clear(); enigma::cleancount=0;

 instance_count=enigma::instance_list.size();

   static int __framecount=0, __lfps, __ltime;
    	__framecount++;
    	if (__framecount==room_speed)
    	{
            __framecount=0;
            int thistime=(int)(clock()*1000.0/CLOCKS_PER_SEC);
            __lfps=(int) (1000*room_speed/(thistime-__ltime));
            __ltime=thistime;
        }

        fps= __lfps;

//printtime(5);//printf("%d fps! \n",fps);

 double a=1000.0/room_speed;
 static int clockl=clock();
 int tsclock=clock();
 int b=(int)(a-(tsclock-clockl)*1000.0/CLOCKS_PER_SEC);
 if (b<1) b=1;
 Sleep(b);
 clockl=clock();
}
