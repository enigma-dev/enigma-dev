/**
  @file  run_cpp_game.cpp
  @brief Implements a method to run a recently-built game.
  
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

#include <languages/lang_CPP.h>
#include <settings-parse/crawler.h>
#include <general/bettersystem.h>
#include <backend/ideprint.h>
#include <general/estring.h>

void lang_CPP::run_game(compile_context&, resource_writer *reswg) {
  resource_writer_cpp *resw = (resource_writer_cpp*)reswg;
  string rprog = extensions::targetOS.runprog, rparam = extensions::targetOS.runparam;
  rprog = string_replace_all(rprog,"$game",resw->gameFname);
  rparam = string_replace_all(rparam,"$game",resw->gameFname);
  user << "Running \"" << rprog << "\" " << rparam << flushl;
  int gameres = e_execs(rprog, rparam);
  user << "Game returned " << gameres << "\n";
}
