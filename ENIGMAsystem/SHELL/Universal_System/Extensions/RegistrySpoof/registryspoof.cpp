// 
// Copyright (C) 2014 Seth N. Hetu
// 
// This file is a part of the ENIGMA Development Environment.
// 
// ENIGMA is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, version 3 of the license or any later version.
// 
// This application and its source code is distributed AS-IS, WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along
// with this code. If not, see <http://www.gnu.org/licenses/>
//

#include <fstream>
#include <sstream>

#include "Platforms/General/PFregistry.h"
#include "Universal_System/Extensions/IniFilesystem/UNIXiniindex.h"

#include "Universal_System/estring.h"


namespace {

//Convert 0 to HKEY_CURRENT_USER, etc. Return the default value on error.
std::string TranslateRoot(int root, std::string defValue) {
  if (root==0) {
    return "HKEY_CURRENT_USER";
  } else if (root==1) {
    return "HKEY_LOCAL_MACHINE";
  } else if (root==2) {
    return "HKEY_CLASSES_ROOT";
  } else if (root==3) {
    return "HKEY_USERS";
  }
  return defValue;
}

//The name of the ini file used to store the faux registry.
const std::string RegistryFilename = "registry.ini";

//The registry as an ini file, and whether or not it has been opened.
enigma::IniFileIndex regIni;
bool regOpen = false;

//The unique string used to represent our game (since the registry file is unique, this doesn't need to be unique per-game).
const std::string GameKey = "MyEnigmaGame";

//The current root.
std::string CurrRoot = TranslateRoot(0, "UNKNOWN");

//Open the ini file if it's not already opened.
void open_ini() {
  if (regOpen) { return; }
  std::ifstream infile(RegistryFilename.c_str());
  regIni.load(infile, ';');
  regOpen = true;
}
void save_ini() {
  regIni.saveToFile(RegistryFilename);
}

} //End un-named namespace


namespace enigma_user {

void registry_write_string(std::string name, std::string str)
{
  registry_write_string_ext(GameKey, name, str);
}
void registry_write_string_ext(std::string key, std::string name, std::string str)
{
  open_ini();
  regIni.write(CurrRoot+"\\"+key, name, str);
  save_ini();
}

void registry_write_real(std::string name, int x)
{
  registry_write_real_ext(GameKey, name, x);
}
void registry_write_real_ext(std::string key, std::string name, int x)
{
  open_ini();
  regIni.write(CurrRoot+"\\"+key, name, x);
  save_ini();
}

std::string registry_read_string(std::string name)
{
  return registry_read_string_ext(GameKey, name);
}
std::string registry_read_string_ext(std::string key, std::string name)
{
  open_ini();
  return regIni.read(CurrRoot+"\\"+key, name, "");
}

int registry_read_real(std::string name)
{
  return registry_read_real_ext(GameKey, name);
}
int registry_read_real_ext(std::string key, std::string name)
{
  open_ini();
  return regIni.read(CurrRoot+"\\"+key, name, 0);
}

bool registry_exists(std::string name) 
{
  return registry_exists_ext(GameKey, name);
}
bool registry_exists_ext(std::string key, std::string name)
{
  open_ini();
  return regIni.keyExists(CurrRoot+"\\"+key, name);
}

void registry_set_root(int root)
{
  //For invalid roots, keep the same value.
  CurrRoot = TranslateRoot(root, CurrRoot);
}

}

