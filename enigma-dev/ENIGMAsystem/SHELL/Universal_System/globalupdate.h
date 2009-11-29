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

namespace enigma
{
  void update_globals()
  {
    //Update mouse coordinates
    //////////////////////////
        mouse_xprevious=mouse_x;
        mouse_yprevious=mouse_y;
        mouse_x=window_mouse_get_x();
        mouse_y=window_mouse_get_y(); 
        
        if (view_enabled)
   	    {
         for (int i=0;i<7;i++)
          {
            if((mouse_x>=view_xport[i] && mouse_x<view_xport[i]+view_wport[i])
            && (mouse_y>=view_yport[i] && mouse_y<view_yport[i]+view_hport[i]))
            {
              mouse_x=view_xview[i]+((mouse_x-view_xport[i])/(double)view_wport[i])*view_wview[i];
              mouse_y=view_yview[i]+((mouse_y-view_yport[i])/(double)view_hport[i])*view_hview[i];
              break;
            }
          }
        }
    //end mouse coords
    
  }
}

