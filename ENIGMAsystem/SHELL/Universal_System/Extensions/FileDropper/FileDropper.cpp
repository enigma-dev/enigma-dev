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
#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

#include <set>
#include <string>
#include <vector>
#include <cwchar>

#include "Universal_System/estring.h"
#include "Platforms/General/PFfilemanip.h"
#include "Platforms/Win32/WINDOWSmain.h"
#include "strings_util.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

namespace {

HHOOK hook = NULL;
WNDPROC oldProc = NULL;
bool file_dnd_enabled = false;
HDROP hDrop = NULL;
set<string> dropped_files;

string def_pattern;
bool def_allowfiles = true;
bool def_allowdirs = true;
bool def_allowmulti = true;

using enigma_user::filename_ext;
using enigma_user::file_exists;
using enigma_user::directory_exists;

void UnInstallHook(HHOOK Hook) {
  UnhookWindowsHookEx(Hook);
}

LRESULT CALLBACK HookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  LRESULT rc = CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);
  if (uMsg == WM_DROPFILES) {
    if (!def_allowmulti) dropped_files.clear();
    hDrop = (HDROP)wParam;
    UINT nNumOfFiles = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
    vector<wchar_t> fName;
    for (UINT i = 0; i < nNumOfFiles; i += 1) {
      fName.resize(DragQueryFileW(hDrop, i, NULL, 0) + 1);
      DragQueryFileW(hDrop, i, fName.data(), fName.size());
      dropped_files.insert(shorten({fName.data(), fName.size() - 1}));
    }
    DragFinish(hDrop); DWORD dwProcessId;
    GetWindowThreadProcessId(enigma::hWnd, &dwProcessId);
    AllowSetForegroundWindow(dwProcessId);
    SetForegroundWindow(enigma::hWnd);
    return 0;
  }
  return rc;
}

LRESULT CALLBACK SetHook(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode == HC_ACTION) {
    CWPSTRUCT *pwp = (CWPSTRUCT *)lParam;
    if (pwp->message == WM_KILLFOCUS) {
      oldProc = (WNDPROC)SetWindowLongPtrW(enigma::hWnd, GWLP_WNDPROC, (LONG_PTR)HookWndProc);
      UnInstallHook(hook);
    }
  }
  return CallNextHookEx(hook, nCode, wParam, lParam);
}

HHOOK InstallHook() {
  ChangeWindowMessageFilter(WM_DROPFILES, 1 /* = MSGFLT_ADD */);
  ChangeWindowMessageFilter(WM_COPYDATA, 1 /* = MSGFLT_ADD */);
  ChangeWindowMessageFilter(0x0049 /* = WM_COPYGLOBALDATA */, 1 /* = MSGFLT_ADD */);
  hook = SetWindowsHookExW(WH_CALLWNDPROC, (HOOKPROC)SetHook, NULL, GetWindowThreadProcessId(enigma::hWnd, NULL));
  return hook;
}

string file_dnd_apply_filter(string pattern, bool allowfiles, bool allowdirs, bool allowmulti) {
  if (pattern == "") { pattern = "."; }
  pattern = string_replace_all(pattern, " ", "");
  pattern = string_replace_all(pattern, "*", "");
  vector<string> extVec = split_string(pattern, ';');
  set<string> filteredNames;
  for (const string &droppedFile : dropped_files) {
    for (const string &ext : extVec) {
      if (ext == "." || ext == filename_ext(droppedFile)) {
        if ((allowfiles && file_exists(droppedFile))
           || (allowdirs && directory_exists(droppedFile)))
          filteredNames.insert(droppedFile);
        break;
      }
    }
  }
  string fname = "";
  if (filteredNames.empty()) return fname;
  for (const string &filteredName : filteredNames) {
    fname += filteredName + "\n";
    if (!def_allowmulti) break;
  }
  fname.pop_back();
  return fname;
}

}  // namespace

namespace enigma_user {

bool file_dnd_get_enabled() {
  return file_dnd_enabled;
}

void file_dnd_set_enabled(bool enable) {
  file_dnd_enabled = enable;
  DragAcceptFiles(enigma::hWnd, file_dnd_enabled);
  if (file_dnd_enabled && hook == NULL)
  InstallHook(); else dropped_files.clear();
}

void file_dnd_set_files(string pattern, bool allowfiles, bool allowdirs, bool allowmulti) {
  def_pattern = pattern;
  def_allowfiles = allowfiles;
  def_allowdirs = allowdirs;
  def_allowmulti = allowmulti;
}

string file_dnd_get_files() {
  return file_dnd_apply_filter(def_pattern, def_allowfiles, def_allowdirs, def_allowmulti);
}

} // namespace enigma_user
