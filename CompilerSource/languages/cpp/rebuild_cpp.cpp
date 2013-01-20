/**
  @file  rebuild_cpp.cpp
  @brief Source implementing the C++ plugin's method for rebuilding the whole engine.
  
  @section License
    Copyright (C) 2010-2013 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <languages/lang_CPP.h>
#include <settings-parse/crawler.h>
#include <general/bettersystem.h>
#include <backend/ideprint.h>
#include <OS_Switchboard.h>

int lang_CPP::rebuild()
{
  edbg << "Cleaning..." << flushl;

  string make = "clean-game ";
  make += "COMPILEPATH=" CURRENT_PLATFORM_NAME "/" + extensions::targetOS.identifier + " ";
  make += "eTCpath=\"" + MAKE_tcpaths + "\"";

  edbg << "Full command line: " << MAKE_location << " " << make << flushl;
  e_execs(MAKE_location,make);

  edbg << "Done.\n" << flushl;
  return 0;
}
