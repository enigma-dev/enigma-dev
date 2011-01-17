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

struct ey_base { // A very vague container, representing something
  string name;  // Each one has a name, mostly to preserve case
  const bool is_scalar; // Tells whether this contains a single data entry (including [] lists) or more attributes
  struct ey_data &data(); // Treat as more data
  struct ey_string &scalar(); // Treat as a scalar
  ey_base(string,bool); ey_base(bool);
};

struct ey_string: ey_base { // If it doesn't contain more named members, it's a scalar
  string value; // Scalars have a single value
  operator string&(); // Implicitly behave as a string
  int toInt(); double toDouble(); // Offer methods to convert to reals
  string toString(); // Offer a method to parse the string
  long long toLong(); char toByte(); // And oddly-sized reals
  ey_string(string, string);
  ey_string(string); ey_string();
};

struct ey_data: ey_base // Contains multiple members
{
  map<string, ey_base*> values; // Members stored here
  struct eylist { // Chronological storage and iteration
    ey_base *value; eylist *next; // Store a value and forward-pointer
    eylist(); eylist(eylist*);
  } values_order, *values_order_last; // We link to the start and end of the chrono list
  
  operator const char*(); // Implicitly return name when char* casted, i.e. for print
  struct ey_string &get(string); // Get the value of a scalar
  eyit find(string), // Get an iterator to an element by that name
       begin(), end(); // Iterator to first element and to end of data map (an invalid iterator)
  eylist* first(); // Get the first element defined chronologically
  
  ey_data(string); // Construct with a name
  ey_data(); ~ey_data(); // Background work
};

typedef ey_data::eylist *eycit; // Quick access of chronological iterators

ey_data parse_eyaml(istream &file, string fname = "");
ey_data parse_eyaml_str(string);

ey_string &eyscalar(ey_base* x);
ey_string &eyscalar(eycit x);
ey_string &eyscalar(eyit x); 
