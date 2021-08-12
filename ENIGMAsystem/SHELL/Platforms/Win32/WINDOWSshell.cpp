/** Copyright (C) 2008-2017 Josh Ventura
*** Copyright (C) 2013-2014 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

#include "Bridges/Win32/WINDOWShandle.h" // enigma::hWnd
#include "Platforms/General/PFmain.h"
#include "Universal_System/estring.h"

using std::string;

namespace enigma_user {

void url_open(string url) {
  tstring tstr_url = widen(url);
  ShellExecuteW(NULL, L"open", tstr_url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void action_webpage(const string &url) {
  url_open(url);
}

} // namespace enigma_user
