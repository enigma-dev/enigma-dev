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


int instance_create(int x,int y,int object)
{
	int idn = enigma::maxid++;
  enigma::object_basic* ob;
	switch((int)object)
	{
	  #define NEW_OBJ_PREFIX ob =
    #include "Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h"
    default:
        #if SHOWERRORS
        show_error("Object doesn't exist",0);
        #endif
      return -1;
  }
  ob->myevent_create();
  return idn;
}

namespace enigma
{
  object_basic* instance_create_id(int x,int y,int object,int idn)
  { //This is for use by the system only. Please leave be.
    if (enigma::maxid<idn)
      enigma::maxid = idn;
    enigma::object_basic* ob;
    switch (object)
    {
	    #define NEW_OBJ_PREFIX ob =
      #include "Preprocessor_Environment_Editable/IDE_EDIT_object_switch.h"
      default:
          #if SHOWERRORS
          show_error("Object doesn't exist",0);
          #endif
        ;
    }
    return ob;
  }
}
