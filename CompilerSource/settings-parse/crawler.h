/**
  @file  crawler.h
  @brief Declares methods for reading in extension skeletons.
  
  Most of the work reading extensions is ultimately done by the language plugin.
  See the corresponding source file for more information.
  
  @section License
    Copyright (C) 2008-2013 Josh Ventura
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

#include <list>
#include <vector>
#include <map>

#include "settings.h"
#include <languages/language_adapter.h>

namespace settings
{
  struct system_descriptor
  {
    string name;
  };
  extern list<string> systems;
  int scour_settings();
}

namespace extensions
{
  void dump_read_locals(map<string,definition*>&);
  /// Parses the requested_extensions of the language adapter into its parsed_extensions.
  void parse_extensions(language_adapter *lang);
  void crawl_for_locals(language_adapter *lang);
  void determine_target();
}
