/** Copyright (C) 2019 Samuel Venable
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

#include "FileDropper.h"
#include <windows.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <cwchar>

#include "Universal_System/estring.h"
#include "Platforms/General/PFfilemanip.h"
#include "Platforms/Win32/WINDOWSmain.h"
#include "strings_util.h"

using std::string;
using std::size_t;
using std::vector;

static HHOOK hook = NULL;
static WNDPROC oldProc = NULL;
static bool file_dnd_enabled = false;
static HDROP hDrop = NULL;
static string fname;

static string def_pattern;
static bool def_allowfiles = true;
static bool def_allowdirs = true;
static bool def_allowmulti = true;

using enigma_user::filename_name;
using enigma_user::filename_ext;
using enigma_user::file_exists;
using enigma_user::directory_exists;

static void UnInstallHook(HHOOK Hook) {
  UnhookWindowsHookEx(Hook);
}

static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  LRESULT rc = CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);

  if (uMsg == WM_DROPFILES) {
    if (!def_allowmulti) fname = "";
    hDrop = (HDROP)wParam;

    UINT nNumOfFiles = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
    if (nNumOfFiles > 0) {
      for (UINT i = 0; i < nNumOfFiles; i += 1) {
        UINT nBufSize = DragQueryFileW(hDrop, i, NULL, 0) + 1;
        wchar_t *fName = new wchar_t[nBufSize];
        DragQueryFileW(hDrop, i, fName, nBufSize);
        if (fname != "") fname += "\n";
        fname += shorten(fName);
        delete[] fName;
      }
    }

    std::vector<string> nameVec = split_string(fname, '\n');
    sort(nameVec.begin(), nameVec.end());
    nameVec.erase(unique(nameVec.begin(), nameVec.end()), nameVec.end());
    fname = "";

    std::vector<string>::size_type sz = nameVec.size();
    for (std::vector<string>::size_type i = 0; i < sz; i += 1) {
      if (fname != "") fname += "\n";
      fname += nameVec[i];
    }

    DragFinish(hDrop);
  }

  return rc;
}

static LRESULT CALLBACK SetHook(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    CWPSTRUCT *pwp = (CWPSTRUCT *)lParam;

    if (pwp->message == WM_KILLFOCUS) {
      oldProc = (WNDPROC)SetWindowLongPtrW(enigma::hWnd, GWLP_WNDPROC, (LONG_PTR)HookWndProc);
      UnInstallHook(hook);
    }
  }

  return CallNextHookEx(hook, nCode, wParam, lParam);
}

static HHOOK InstallHook() {
  hook = SetWindowsHookExW(WH_CALLWNDPROC, (HOOKPROC)SetHook, NULL, GetWindowThreadProcessId(enigma::hWnd, NULL));
  return hook;
}

static void file_dnd_apply_filter(string pattern, bool allowfiles, bool allowdirs, bool allowmulti) {
  if (pattern == "") { pattern = "."; }
  pattern = string_replace_all(pattern, " ", "");
  pattern = string_replace_all(pattern, "*", "");
  std::vector<string> extVec = split_string(pattern, ';');
  std::vector<string> nameVec = split_string(fname, '\n');
  std::vector<string>::size_type sz1 = nameVec.size();
  std::vector<string>::size_type sz2 = extVec.size();
  fname = "";

  for (std::vector<string>::size_type i2 = 0; i2 < sz2; i2 += 1) {
    for (std::vector<string>::size_type i1 = 0; i1 < sz1; i1 += 1) {
      if (extVec[i2] == "." || extVec[i2] == filename_ext(nameVec[i1])) {
        if (fname != "") fname += "\n";
        fname += nameVec[i1];
      }
    }
  }

  nameVec = split_string(fname, '\n');
  sz1 = nameVec.size();
  fname = "";

  if (allowmulti) {
    for (std::vector<string>::size_type i = 0; i < sz1; i += 1) {
      if (allowfiles && file_exists(nameVec[i])) {
        if (fname != "") fname += "\n";
        fname += nameVec[i];
      } else if (allowdirs && directory_exists(nameVec[i])) {
        if (fname != "") fname += "\n";
        fname += nameVec[i];
      }
    }
  } else {
    if (!nameVec.empty()) {
      if (allowfiles && file_exists(nameVec[0])) {
        if (fname != "") fname += "\n";
        fname += nameVec[0];
      } else if (allowdirs && directory_exists(nameVec[0])) {
        if (fname != "") fname += "\n";
        fname += nameVec[0];
      }
    }
  }
}

namespace enigma_user {

bool file_dnd_get_enabled() {
  return file_dnd_enabled;
}

void file_dnd_set_enabled(bool enable) {
  file_dnd_enabled = enable;
  DragAcceptFiles(enigma::hWnd, file_dnd_enabled);
  if (file_dnd_enabled && hook == NULL)
  InstallHook(); else fname = "";
}

void file_dnd_set_files(string pattern, bool allowfiles, bool allowdirs, bool allowmulti) {
  def_pattern = pattern;
  def_allowfiles = allowfiles;
  def_allowdirs = allowdirs;
  def_allowmulti = allowmulti;
}

string file_dnd_get_files() {
  if (fname != "") {
    file_dnd_apply_filter(def_pattern, def_allowfiles, def_allowdirs, def_allowmulti);
    while (fname.back() == '\n')
      fname.pop_back();
  }

  return fname;
}

} // namespace enigma_user
