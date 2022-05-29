/**
  @file  language_adapter.cpp
  @brief Implements a class by which ENIGMA can interface with and output to other languages.
  
  In actuality, only the virtual destructor and a few globals need defined.
  
  @section License
    Copyright (C) 2008-2012 Josh Ventura
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

#include "language_adapter.h"
language_adapter::~language_adapter() {}

map<string,language_adapter*> languages;
language_adapter *current_language;
string current_language_name;

jdi::Context *main_context = nullptr;
