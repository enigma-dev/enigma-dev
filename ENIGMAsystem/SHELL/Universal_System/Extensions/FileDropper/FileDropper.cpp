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

using std::string;
using std::size_t;
using std::vector;

static HHOOK hook = NULL;
static WNDPROC oldProc = NULL;
static bool file_dnd_enabled = false;
static HDROP hDrop = NULL;
static string fname;

using enigma_user::filename_name;
using enigma_user::filename_ext;
using enigma_user::string_replace_all;
using enigma_user::string_split;
using enigma_user::file_exists;
using enigma_user::directory_exists;
using enigma::hWnd;

static void UnInstallHook(HHOOK Hook) {
  UnhookWindowsHookEx(Hook);
}

static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  LRESULT rc = CallWindowProc(oldProc, hWnd, uMsg, wParam, lParam);
  
  if (uMsg == WM_DROPFILES) {
    hDrop = (HDROP)wParam;

    UINT nNumOfFiles = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, NULL);
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

    std::vector<string> nameVec = string_split(fname, '\n');
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
      oldProc = (WNDPROC)SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)HookWndProc);
      UnInstallHook(hook);
    }
  }

  return CallNextHookEx(hook, nCode, wParam, lParam);
}

static HHOOK InstallHook() {
  hook = SetWindowsHookExW(WH_CALLWNDPROC, (HOOKPROC)SetHook, NULL, GetWindowThreadProcessId(hWnd, NULL));
  return hook;
}

namespace enigma_user {

bool file_dnd_get_enabled() {
  return file_dnd_enabled;
}

void file_dnd_set_enabled(bool enable) {
  file_dnd_enabled = enable;
  DragAcceptFiles(hWnd, file_dnd_enabled);
  if (file_dnd_enabled && hook == NULL) 
  InstallHook(); else fname = "";
}

string file_dnd_get_files() {
  while (fname.back() == '\n')
    fname.pop_back();

  return fname;
}

void file_dnd_set_files(string pattern, double allowfiles, double allowdirs, double allowmulti) {
  if (pattern == "") { pattern = "."; }
  pattern = string_replace_all(pattern, " ", "");
  pattern = string_replace_all(pattern, "*", "");
  std::vector<string> extVec = string_split(pattern, ';');
  std::vector<string> nameVec = string_split(fname, '\n');
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

  nameVec = string_split(fname, '\n');
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

void file_dnd_add_files(string files) {
  if (files != "") {
    std::vector<string> pathVec = string_split(files, '\n');

    for (const string &path : pathVec) {
      tstring tstr_path = widen(path); wchar_t wstr_path[MAX_PATH];
      GetFullPathNameW(tstr_path.c_str(), MAX_PATH, wstr_path, NULL);
      string str_path = shorten(wstr_path);
      if (file_exists(str_path)) {
        if (fname != "") fname += "\n";
        fname += str_path;
      }
    }

    std::vector<string> nameVec = string_split(fname, '\n');
    sort(nameVec.begin(), nameVec.end());
    nameVec.erase(unique(nameVec.begin(), nameVec.end()), nameVec.end());
    std::vector<string>::size_type sz = nameVec.size();
    fname = "";

    for (std::vector<string>::size_type i = 0; i < sz; i += 1) {
      if (fname != "") fname += "\n";
      fname += nameVec[i];
    }
  }
}

void file_dnd_remove_files(string files) {
  std::vector<string> pathVec = string_split(files, '\n');

  for (const string &path : pathVec) {
    fname = string_replace_all(fname, path + "\n", "");
    fname = string_replace_all(fname, path, "");
  }
}

} // namespace enigma_user
