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

enum e_type {
  et_inline,
  et_stacked,
  et_special,
  et_spec_sys,
  et_system,
  et_none,
  et_error
};

enum p_type {
  p2t_sprite,
  p2t_sound,
  p2t_background,
  p2t_path,
  p2t_script,
  p2t_font,
  p2t_timeline,
  p2t_object,
  p2t_room,
  p2t_key,
  p2t_error
};

#include <string>
#include <map>

struct event_info
{
  string name;
  int    gmid;
  
  string humanname;
  p_type par2type;
  
  e_type mode;
  int    mmod;
  
  string def;
  string cons;
  string super;
  string sub;
  string instead;
  
  event_info();
  event_info(string n,int i);
};

struct main_event_info
{
  string name;
  bool is_group;
  map<int,event_info*> specs;
  typedef map<int,event_info*>::iterator iter;
  main_event_info();
};

int event_parse_resourcefile();
void event_info_clear();
