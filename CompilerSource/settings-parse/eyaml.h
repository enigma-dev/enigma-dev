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

#include <string>
#include <fstream>
using std::string;

typedef map<string, struct ey_base*>::iterator eyit;
typedef pair<string, struct ey_base*> eypair;

struct ey_base {
  string name;
  const bool is_scalar;
  string gets(string);
  eyit getit(string);
  eyit itend(void);
  ey_base(bool);
  ey_base(string,bool);
};

struct ey_string: ey_base {
  string value;
  ey_string();
  ey_string(string);
  ey_string(string, string);
};

struct ey_data: ey_base
{
  map<string, ey_base*> values;
  struct eylist {
    ey_base *value; eylist *next;
    eylist(); eylist(eylist*);
  } values_order, *values_order_last;
  
  operator char*();
  operator ey_data*();
  
  string gets(string);
  eyit getit(string);
  eyit itend(void);
  
  ey_data();
  ey_data(string);
  ~ey_data();
};

typedef ey_data::eylist *eycit;

ey_data parse_eyaml(istream &file, string fname = "");
ey_data parse_eyaml_str(string);

eyit eyaml_ci_find(ey_data &dat, string str);

string eycit_str(eycit);
string eyit_str(eyit);
