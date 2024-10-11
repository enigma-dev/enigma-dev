/*

 MIT License

 Copyright © 2021-2024 Samuel Venable

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*/

#define _WIN32_IE 0x0900

#include <cstdlib>
#include <cstdio>
#include <cwchar>

#include <sstream>
#include <vector>
#include <string>

#include <libdlgmod/libdlgmod.h>

#include <windows.h>
#include <gdiplus.h>
#include <shobjidl.h>
#include <shlwapi.h>
#include <commdlg.h>
#include <commctrl.h>
#include <comdef.h>
#ifdef _MSC_VER
#include <atlbase.h>
#include <activscp.h>
#else
#include <sys/stat.h>
#include <fcntl.h>
#include <share.h>
#include <io.h>
#endif
#include <objbase.h>
#include <shlobj.h>

#if !defined(_MSC_VER)
#include <xprocess.hpp>
#endif

using namespace Gdiplus;
using std::basic_string;
using std::to_string;
using std::wstring;
using std::string;
using std::vector;
using std::size_t;

#ifdef _MSC_VER
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "comctl32.lib")
#endif

namespace dialog_module {

  namespace {

    // window handles
    void *owner = nullptr;
    HWND dlg = nullptr;
    HWND win = nullptr;

    // hook procs
    bool init = false;
    HHOOK hhook = nullptr;

    // error msgs
    bool fatal = false;

    // input boxes
    bool hidden = false;

    // get color
    string tstr_gctitle;
    wstring cpp_wstr_gctitle;

    // file dialogs
    OPENFILENAMEW ofn; 
    bool cancel_pressed = false;
    wchar_t *wstr_filter = nullptr;
    wchar_t wstr_fname[32767];
    wstring cpp_wstr_dir;
    wstring cpp_wstr_title;
    wstring pipefilter;
    HWND textbox;
    string files;

    // misc
    string caption;
    string tstr_icon;

    enum BUTTON_TYPES {
      BUTTON_ABORT,
      BUTTON_IGNORE,
      BUTTON_OK,
      BUTTON_CANCEL,
      BUTTON_YES,
      BUTTON_NO,
      BUTTON_RETRY
    };

    int const btn_array_len = 7;
    string btn_array[btn_array_len] = { "Abort", "Ignore", "OK", "Cancel", "Yes", "No", "Retry" };

    wstring widen(string tstr) {
      if (tstr.empty()) return L"";
      size_t wchar_count = tstr.size() + 1; 
      vector<wchar_t> buf(wchar_count);
      return wstring{ buf.data(), (size_t)MultiByteToWideChar(CP_UTF8, 0, tstr.c_str(), -1, buf.data(), (int)wchar_count) };
    }

    string narrow(wstring wstr) {
      if (wstr.empty()) return "";
      int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), nullptr, 0, nullptr, nullptr); 
      vector<char> buf(nbytes);
      return string{ buf.data(), (size_t)WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, nullptr, nullptr) };
    }


    string string_replace_all(string str, string substr, string newstr) {
      size_t pos = 0;
      const size_t sublen = substr.length(), newlen = newstr.length();
      while ((pos = str.find(substr, pos)) != string::npos) {
        str.replace(pos, sublen, newstr);
        pos += newlen;
      }
      return str;
    }

    HWND owner_window() {
      win = owner ? (HWND)owner : GetForegroundWindow();
      win = (unsigned long long)win ? win : GetDesktopWindow();
      return win;
    }

    int show_message_helper(char *str, bool cancelable) {
      string tstr = str; wstring wstr = widen(tstr);

      string title = (caption == "") ? (cancelable ? "Question" : "Information") : caption;
      wstring wtitle = widen(title);

      UINT flags = MB_DEFBUTTON1 | MB_APPLMODAL;
      flags |= cancelable ? (MB_OKCANCEL | MB_ICONQUESTION) : (MB_OK | MB_ICONINFORMATION);

      int result = MessageBoxW(owner_window(), wstr.c_str(), wtitle.c_str(), flags | MB_TOPMOST);
      return cancelable ? ((result == IDOK) ? 1 : -1) : 1;
    }

    int show_question_helper(char *str, bool cancelable) {
      string tstr = str; wstring wstr = widen(tstr);

      string title = (caption == "") ? "Question" : caption;
      wstring wtitle = widen(title);

      UINT flags = MB_DEFBUTTON1 | MB_APPLMODAL | MB_ICONQUESTION;
      flags |= cancelable ? MB_YESNOCANCEL : MB_YESNO;

      int result = MessageBoxW(owner_window(), wstr.c_str(), wtitle.c_str(), flags | MB_TOPMOST);
      return cancelable ? ((result == IDYES) ? 1 : ((result == IDNO) ? 0 : -1)) : (result == IDYES);
    }

    int show_error_helper(char *str, bool abort, bool attempt) {
      string tstr = str; wstring wstr = widen(tstr);

      string title = (caption == "") ? "Error" : caption;
      wstring wtitle = widen(title);

      if (attempt) {
        UINT flags = MB_RETRYCANCEL | MB_ICONERROR | MB_DEFBUTTON1 | MB_APPLMODAL;
        int result = MessageBoxW(owner_window(), wstr.c_str(), wtitle.c_str(), flags | MB_TOPMOST);
        return (result == IDRETRY) ? 0 : -1;
      }

      UINT flags = (abort ? MB_OK : MB_OKCANCEL) | MB_ICONERROR | MB_DEFBUTTON1 | MB_APPLMODAL;
      int result = MessageBoxW(owner_window(), wstr.c_str(), wtitle.c_str(), flags | MB_TOPMOST);
      result = abort ? 1 : ((result == IDOK) ? 1 : -1);

      if (result == 1) exit(0);
      return result;
    }


    #ifdef _MSC_VER
    class CSimpleScriptSite :
      public IActiveScriptSite,
      public IActiveScriptSiteWindow {
    public:
      CSimpleScriptSite() : m_cRefCount(1), m_hWnd(nullptr) { }

      // IUnknown

      STDMETHOD_(ULONG, AddRef)();
      STDMETHOD_(ULONG, Release)();
      STDMETHOD(QueryInterface)(REFIID riid, void **ppvObject);

      // IActiveScriptSite

      STDMETHOD(GetLCID)(LCID *plcid) { *plcid = 0; return S_OK; }
      STDMETHOD(GetItemInfo)(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppiunkItem, ITypeInfo **ppti) { return TYPE_E_ELEMENTNOTFOUND; }
      STDMETHOD(GetDocVersionString)(BSTR *pbstrVersion) { *pbstrVersion = SysAllocString(L"1.0"); return S_OK; }
      STDMETHOD(OnScriptTerminate)(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo) { return S_OK; }
      STDMETHOD(OnStateChange)(SCRIPTSTATE ssScriptState) { return S_OK; }
      STDMETHOD(OnScriptError)(IActiveScriptError *pIActiveScriptError) { return S_OK; }
      STDMETHOD(OnEnterScript)(void) { return S_OK; }
      STDMETHOD(OnLeaveScript)(void) { return S_OK; }

      // IActiveScriptSiteWindow

      STDMETHOD(GetWindow)(HWND *phWnd) { *phWnd = m_hWnd; return S_OK; }
      STDMETHOD(EnableModeless)(BOOL fEnable) { return S_OK; }

      // Miscellaneous

      STDMETHOD(SetWindow)(HWND hWnd) { m_hWnd = hWnd; return S_OK; }

    public:
      LONG m_cRefCount;
      HWND m_hWnd;
    };

    STDMETHODIMP_(ULONG) CSimpleScriptSite::AddRef() {
      return InterlockedIncrement(&m_cRefCount);
    }

    STDMETHODIMP_(ULONG) CSimpleScriptSite::Release() {
      if (!InterlockedDecrement(&m_cRefCount)) {
        delete this;
        return 0;
      }
      return m_cRefCount;
    }

    STDMETHODIMP CSimpleScriptSite::QueryInterface(REFIID riid, void **ppvObject) {
      if (riid == IID_IUnknown || riid == IID_IActiveScriptSiteWindow) {
        *ppvObject = (IActiveScriptSiteWindow *)this;
        AddRef();
        return NOERROR;
      }
      if (riid == IID_IActiveScriptSite) {
        *ppvObject = (IActiveScriptSite *)this;
        AddRef();
        return NOERROR;
      }
      return E_NOINTERFACE;
    }
    #endif

    HICON GetIcon(HWND hwnd) {
      HICON icon = (HICON)SendMessageW(hwnd, WM_GETICON, ICON_SMALL, 0);
      if (icon == nullptr)
        icon = (HICON)GetClassLongPtrW(hwnd, GCLP_HICONSM);
      if (icon == nullptr)
        icon = LoadIcon(GetModuleHandleW(nullptr), MAKEINTRESOURCE(0));
      if (icon == nullptr)
        icon = LoadIcon(nullptr, IDI_APPLICATION);
      return icon;
    }

    UINT_PTR CALLBACK GetColorProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
      if (uiMsg == WM_INITDIALOG) {
        cancel_pressed = false;
        if (tstr_gctitle != "")
          SetWindowTextW(hdlg, cpp_wstr_gctitle.c_str());
        PostMessageW(hdlg, WM_SETFOCUS, 0, 0);
      }
      return false;
    }

    #ifdef _MSC_VER
    LRESULT CALLBACK InputBoxProc(int nCode, WPARAM wParam, LPARAM lParam) {
      if (nCode < HC_ACTION)
        return CallNextHookEx(hhook, nCode, wParam, lParam);

      if (nCode == HCBT_CREATEWND) {
        cancel_pressed = false;
        CBT_CREATEWNDW *cbtcr = (CBT_CREATEWNDW *)lParam;
        if (win == GetDesktopWindow() ||
          (win != (HWND)wParam && cbtcr->lpcs->hwndParent == win)) {
          dlg = (HWND)wParam;
          init = true;
        }
      }

      if (nCode == HCBT_SETFOCUS) {
        POINT pt;
        if (GetCursorPos(&pt) && ScreenToClient(dlg, &pt) && 
          GetDlgItem(dlg, 2) == ChildWindowFromPoint(dlg, pt)) {
          cancel_pressed = true;
        } else {
          cancel_pressed = false;
        }
      }

      if (dlg != nullptr) {
        if (nCode == HCBT_ACTIVATE && init == true) {
          SetWindowLongPtr(dlg, GWLP_HWNDPARENT, (LONG_PTR)GetDesktopWindow());
          ShowWindow(dlg, SW_SHOW);
          SetWindowLongPtr(dlg, GWL_EXSTYLE, GetWindowLongPtr(dlg, GWL_EXSTYLE) | WS_EX_APPWINDOW);
          SetWindowPos(dlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
          EnableWindow(win, true);
          if (hidden == true)
            SendDlgItemMessageW(dlg, 1000, EM_SETPASSWORDCHAR, L'\x25cf', 0);
          init = false;
        }
        wstring cpp_wstr_icon = widen(tstr_icon);
        if (PathFileExistsW(cpp_wstr_icon.c_str())) {
          HICON hIcon;
          ULONG_PTR m_gdiplusToken;
          Gdiplus::GdiplusStartupInput gdiplusStartupInput;
          Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);
          Bitmap *png = Bitmap::FromFile(cpp_wstr_icon.c_str());
          png->GetHICON(&hIcon);
          PostMessage(dlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
          delete png;
          Gdiplus::GdiplusShutdown(m_gdiplusToken);
        } else {
          HICON hIcon = GetIcon(win);
          PostMessage(dlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }
      }
      return CallNextHookEx(hhook, nCode, wParam, lParam);
    }
    #endif

    LRESULT CALLBACK MessageBoxProc(int nCode, WPARAM wParam, LPARAM lParam) {
      if (nCode < HC_ACTION)
        return CallNextHookEx(hhook, nCode, wParam, lParam);
      if (nCode == HCBT_CREATEWND) {
        cancel_pressed = false;
        CBT_CREATEWNDW *cbtcr = (CBT_CREATEWNDW *)lParam;
        if (win == GetDesktopWindow() ||
          (win != (HWND)wParam && cbtcr->lpcs->hwndParent == win)) {
          dlg = (HWND)wParam;
          init = true;
        }
        if (dlg != nullptr && init) {
          SetWindowLongPtr(dlg, GWLP_HWNDPARENT, (LONG_PTR)GetDesktopWindow());
          ShowWindow(dlg, SW_SHOW);
          SetWindowLongPtr(dlg, GWL_EXSTYLE, GetWindowLongPtr(dlg, GWL_EXSTYLE) | WS_EX_APPWINDOW);
          SetWindowPos(dlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
          EnableWindow(win, true);
          RECT wrect1; GetWindowRect(dlg, &wrect1);
          RECT wrect2; GetWindowRect(win, &wrect2);
          unsigned width1 = wrect1.right - wrect1.left;
          unsigned height1 = wrect1.bottom - wrect1.top;
          unsigned width2 = wrect2.right - wrect2.left;
          unsigned height2 = wrect2.bottom - wrect2.top;
          int xpos = wrect2.left + (width2 / 2) - (width1 / 2);
          int ypos = wrect2.top + (height2 / 2) - (height1 / 2);
          MoveWindow(dlg, xpos, ypos, width1, height1, true);
          wstring wstr_ok = widen(btn_array[BUTTON_OK]);
          wstring wstr_yes = widen(btn_array[BUTTON_YES]);
          wstring wstr_no = widen(btn_array[BUTTON_NO]);
          wstring wstr_retry = widen(btn_array[BUTTON_RETRY]);
          wstring wstr_cancel = widen(btn_array[BUTTON_CANCEL]);
          SetDlgItemTextW(dlg, IDOK, wstr_ok.c_str());
          SetDlgItemTextW(dlg, IDYES, wstr_yes.c_str());
          SetDlgItemTextW(dlg, IDNO, wstr_no.c_str());
          SetDlgItemTextW(dlg, IDRETRY, wstr_retry.c_str());
          SetDlgItemTextW(dlg, IDCANCEL, wstr_cancel.c_str());
          wstring cpp_wstr_icon = widen(tstr_icon);
          if (PathFileExistsW(cpp_wstr_icon.c_str())) {
            HICON hIcon;
            ULONG_PTR m_gdiplusToken;
            Gdiplus::GdiplusStartupInput gdiplusStartupInput;
            Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);
            Bitmap *png = Bitmap::FromFile(cpp_wstr_icon.c_str());
            png->GetHICON(&hIcon);
            PostMessage(dlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            delete png;
            Gdiplus::GdiplusShutdown(m_gdiplusToken);
          } else {
            HICON hIcon = GetIcon(win);
            PostMessage(dlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
          }
        }
      }
      return CallNextHookEx(hhook, nCode, wParam, lParam);
    }

    LRESULT CALLBACK ShowErrorProc(int nCode, WPARAM wParam, LPARAM lParam) {
      if (nCode < HC_ACTION)
        return CallNextHookEx(hhook, nCode, wParam, lParam);
      if (nCode == HCBT_CREATEWND) {
        cancel_pressed = false;
        CBT_CREATEWNDW *cbtcr = (CBT_CREATEWNDW *)lParam;
        if (win == GetDesktopWindow() ||
          (win != (HWND)wParam && cbtcr->lpcs->hwndParent == win)) {
          dlg = (HWND)wParam;
          init = true;
        }
        if (dlg != nullptr && init) {
          SetWindowLongPtr(dlg, GWLP_HWNDPARENT, (LONG_PTR)GetDesktopWindow());
          ShowWindow(dlg, SW_SHOW);
          SetWindowLongPtr(dlg, GWL_EXSTYLE, GetWindowLongPtr(dlg, GWL_EXSTYLE) | WS_EX_APPWINDOW);
          SetWindowPos(dlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
          EnableWindow(win, true);
          RECT wrect1; GetWindowRect(dlg, &wrect1);
          RECT wrect2; GetWindowRect(win, &wrect2);
          unsigned width1 = wrect1.right - wrect1.left;
          unsigned height1 = wrect1.bottom - wrect1.top;
          unsigned width2 = wrect2.right - wrect2.left;
          unsigned height2 = wrect2.bottom - wrect2.top;
          int xpos = wrect2.left + (width2 / 2) - (width1 / 2);
          int ypos = wrect2.top + (height2 / 2) - (height1 / 2);
          MoveWindow(dlg, xpos, ypos, width1, height1, true);
          wstring wstr_abort = widen(btn_array[BUTTON_ABORT]);
          wstring wstr_ignore = widen(btn_array[BUTTON_IGNORE]);
          SetDlgItemTextW(dlg, IDOK, wstr_abort.c_str());
          SetDlgItemTextW(dlg, IDCANCEL, wstr_ignore.c_str());
          wstring cpp_wstr_icon = widen(tstr_icon);
          if (PathFileExistsW(cpp_wstr_icon.c_str())) {
            HICON hIcon;
            ULONG_PTR m_gdiplusToken;
            Gdiplus::GdiplusStartupInput gdiplusStartupInput;
            Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);
            Bitmap *png = Bitmap::FromFile(cpp_wstr_icon.c_str());
            png->GetHICON(&hIcon);
            PostMessage(dlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            delete png;
            Gdiplus::GdiplusShutdown(m_gdiplusToken);
          } else {
            HICON hIcon = GetIcon(win);
            PostMessage(dlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
          }
        }
      }
      return CallNextHookEx(hhook, nCode, wParam, lParam);
    }

    LRESULT CALLBACK DialogProc(int nCode, WPARAM wParam, LPARAM lParam) {
      if (nCode < HC_ACTION)
        return CallNextHookEx(hhook, nCode, wParam, lParam);

      if (nCode == HCBT_CREATEWND) {
        cancel_pressed = false;
        CBT_CREATEWNDW *cbtcr = (CBT_CREATEWNDW *)lParam;
        if (win == GetDesktopWindow() ||
          (win != (HWND)wParam && cbtcr->lpcs->hwndParent == win)) {
          dlg = (HWND)wParam;
          init = true;
        }
      }

      if (nCode == HCBT_ACTIVATE && init) {
        SetWindowLongPtr(dlg, GWLP_HWNDPARENT, (LONG_PTR)GetDesktopWindow());
        ShowWindow(dlg, SW_SHOW);
        SetWindowLongPtr(dlg, GWL_EXSTYLE, GetWindowLongPtr(dlg, GWL_EXSTYLE) | WS_EX_APPWINDOW);
        SetWindowPos(dlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        EnableWindow(win, true);
        RECT wrect1; GetWindowRect(dlg, &wrect1);
        RECT wrect2; GetWindowRect(win, &wrect2);
        unsigned width1 = wrect1.right - wrect1.left;
        unsigned height1 = wrect1.bottom - wrect1.top;
        unsigned width2 = wrect2.right - wrect2.left;
        unsigned height2 = wrect2.bottom - wrect2.top;
        int xpos = wrect2.left + (width2 / 2) - (width1 / 2);
        int ypos = wrect2.top + (height2 / 2) - (height1 / 2);
        MoveWindow(dlg, xpos, ypos, width1, height1, true);
        init = false;
      }
      wstring cpp_wstr_icon = widen(tstr_icon);
      if (PathFileExistsW(cpp_wstr_icon.c_str())) {
        HICON hIcon;
        ULONG_PTR m_gdiplusToken;
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);
        Bitmap *png = Bitmap::FromFile(cpp_wstr_icon.c_str());
        png->GetHICON(&hIcon);
        PostMessage(dlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        delete png;
        Gdiplus::GdiplusShutdown(m_gdiplusToken);
      } else {
        HICON hIcon = GetIcon(win);
        PostMessage(dlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
      }
      return CallNextHookEx(hhook, nCode, wParam, lParam);
    }

    vector<string> string_split(string str, char delimiter) {
      vector<string> vec;
      std::stringstream sstr(str);
      string tmp;
      while (std::getline(sstr, tmp, delimiter)) {
        vec.push_back(tmp);
      }
      return vec;
    }

    string filename_change_ext(string fname, string newext) {
      size_t fp = fname.find_last_of(".");
      if (fp == string::npos) return fname + newext;
      return fname.replace(fp, fname.length(), newext);
    }

    string filename_name(string fname) {
      size_t fp = fname.find_last_of("\\/");
      if (fp == string::npos) return fname;
      return fname.substr(fp + 1);
    }

    string filename_ext(string fname) {
      fname = filename_name(fname);
      size_t fp = fname.find_last_of(".");
      if (fp == string::npos) return "";
      return fname.substr(fp);
    }

    LRESULT CALLBACK SaveAsProc(int nCode, WPARAM wParam, LPARAM lParam) {
      if (nCode < HC_ACTION)
        return CallNextHookEx(hhook, nCode, wParam, lParam);

      if (nCode == HCBT_CREATEWND) {
        CBT_CREATEWNDW *cbtcr = (CBT_CREATEWNDW *)lParam;
        if (win == GetDesktopWindow() ||
          (win != (HWND)wParam && cbtcr->lpcs->hwndParent == win)) {
          dlg = (HWND)wParam;
          init = true;
        }
      }

      if (nCode == HCBT_ACTIVATE && init) {
        SetWindowLongPtr(dlg, GWLP_HWNDPARENT, (LONG_PTR)GetDesktopWindow());
        ShowWindow(dlg, SW_SHOW);
        SetWindowLongPtr(dlg, GWL_EXSTYLE, GetWindowLongPtr(dlg, GWL_EXSTYLE) | WS_EX_APPWINDOW);
        SetWindowPos(dlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        EnableWindow(win, true);
        textbox = FindWindowEx(dlg, nullptr, "DUIViewWndClassName", nullptr);
        textbox = FindWindowEx(textbox, nullptr, "DirectUIHWND", nullptr);
        textbox = FindWindowEx(textbox, nullptr, "FloatNotifySink", nullptr);
        textbox = FindWindowEx(textbox, nullptr, "ComboBox", nullptr);
        textbox = FindWindowEx(textbox, nullptr, "Edit", nullptr);
        wchar_t *textstr = new wchar_t[MAX_PATH]();
        GetWindowTextW(textbox, textstr, MAX_PATH);
        bool equalsext = false;
        vector<string> pipesplit = string_split(narrow(pipefilter), '|');
        for (unsigned i = 0; i < pipesplit.size(); i++) {
          if (i % 2 != 0) {
            vector<string> semicolonsplit = string_split(pipesplit[i], ';');
            for (int j = 0; j < semicolonsplit.size(); j++) {
              semicolonsplit[j] = string_replace_all(semicolonsplit[j], "*.*", "");
              semicolonsplit[j] = string_replace_all(semicolonsplit[j], "*", "");
              if (filename_ext(narrow(textstr)) == semicolonsplit[j]) {
                equalsext = true;
              }
            }
            if (!equalsext && filename_ext(narrow(textstr)) == "" && semicolonsplit.size()) {
              wstring textwstr = widen(filename_change_ext(narrow(textstr), semicolonsplit[0]));
              SetWindowTextW(textbox, textwstr.c_str());
            }
          }
        }
        delete[] textstr; 
        textstr = nullptr;
        RECT wrect1; GetWindowRect(dlg, &wrect1);
        RECT wrect2; GetWindowRect(win, &wrect2);
        unsigned width1 = wrect1.right - wrect1.left;
        unsigned height1 = wrect1.bottom - wrect1.top;
        unsigned width2 = wrect2.right - wrect2.left;
        unsigned height2 = wrect2.bottom - wrect2.top;
        int xpos = wrect2.left + (width2 / 2) - (width1 / 2);
        int ypos = wrect2.top + (height2 / 2) - (height1 / 2);
        MoveWindow(dlg, xpos, ypos, width1, height1, true);
        init = false;
      }
      wstring cpp_wstr_icon = widen(tstr_icon);
      if (PathFileExistsW(cpp_wstr_icon.c_str())) {
        HICON hIcon;
        ULONG_PTR m_gdiplusToken;
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);
        Bitmap *png = Bitmap::FromFile(cpp_wstr_icon.c_str());
        png->GetHICON(&hIcon);
        PostMessage(dlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        delete png;
        Gdiplus::GdiplusShutdown(m_gdiplusToken);
      } else {
        HICON hIcon = GetIcon(win);
        PostMessage(dlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
      }
      return CallNextHookEx(hhook, nCode, wParam, lParam);
    }

    #ifndef _MSC_VER
    std::string InputBoxResult;
    #endif
    char *InputBox(char *Prompt, char *Title, char *Default) {
      #ifdef _MSC_VER
      HRESULT hr = S_OK;
      hr = CoInitialize(nullptr);

      // Initialize
      CSimpleScriptSite *pScriptSite = new CSimpleScriptSite();
      CComPtr<IActiveScript> spVBScript;
      CComPtr<IActiveScriptParse> spVBScriptParse;
      #endif
      HWND parent_window = owner_window();
      #ifdef _MSC_VER
      hr = pScriptSite->SetWindow(parent_window);
      hr = spVBScript.CoCreateInstance(OLESTR("VBScript"));
      hr = spVBScript->SetScriptSite(pScriptSite);
      hr = spVBScript->QueryInterface(&spVBScriptParse);
      hr = spVBScriptParse->InitNew();
      #endif

      // Replace quotes with double quotes
      string strPrompt = string_replace_all(Prompt, "\"", "\"\"");
      string strTitle = string_replace_all(Title, "\"", "\"\"");
      string strDefault = string_replace_all(Default, "\"", "\"\"");

      // Create evaluation string
      string Evaluation = "InputBox(\"" + strPrompt + "\", \"" + strTitle + "\", \"" + strDefault + "\")";
      Evaluation = string_replace_all(Evaluation, "\r", "");
      Evaluation = string_replace_all(Evaluation, "\n", "\" + vbNewLine + \"");
      wstring WideEval = widen(Evaluation);

      #ifdef _MSC_VER
      // Run InpuBox
      CComVariant result;
      EXCEPINFO ei = {};
      #endif

      #ifdef _MSC_VER
      DWORD ThreadID = GetCurrentThreadId();
      HINSTANCE ModHwnd = GetModuleHandle(nullptr);
      hhook = SetWindowsHookEx(WH_CBT, &InputBoxProc, ModHwnd, ThreadID);
      hr = spVBScriptParse->ParseScriptText(WideEval.c_str(), nullptr, nullptr, nullptr, 0, 0, SCRIPTTEXT_ISEXPRESSION, &result, &ei);
      UnhookWindowsHookEx(hhook);
      #else
      FILE *fp = nullptr;
      wstring wfname = widen("C:\\Windows\\Temp\\temp.XXXXXX"); 
      wchar_t *wbuff = wfname.data(); if (_wmktemp_s(wbuff, wfname.length() + 1)) return (char *)"";
      if (_wfopen_s(&fp, wbuff, L"wb, ccs=UTF-8" )) {
        return (char *)"";
      }
      if (!fp) return (char *)"";
      Evaluation = "WScript.Echo " + Evaluation;
      std::size_t result = fwrite(Evaluation.data(), sizeof(char), Evaluation.length(), fp);
      if (result < Evaluation.length()) { fclose(fp); return (char *)""; }
      else { fclose(fp); }
      MoveFileW(wbuff, (wbuff + std::wstring(L".vbs")).c_str());
      ngs::ps::NGS_PROCID proc_id = ngs::ps::spawn_child_proc_id((std::string("cscript.exe /nologo \"") + narrow(wbuff) + std::string(".vbs\"")).c_str(), false);
      int window_ids_length = 0;
      char **window_ids = nullptr;
      xprocess::window_id_from_proc_id(proc_id, &window_ids, &window_ids_length);
      for (int i = 0; i < window_ids_length; i++) {
        HWND dlg = (HWND)(void *)strtoull(window_ids[i], nullptr, 10);
        if (IsWindow(dlg) && IsWindowVisible(dlg)) {
          SetWindowLongPtr(dlg, GWLP_HWNDPARENT, (LONG_PTR)owner_window());
          POINT pt;
          if (GetCursorPos(&pt) && ScreenToClient(dlg, &pt) && 
            GetDlgItem(dlg, 2) == ChildWindowFromPoint(dlg, pt)) {
            cancel_pressed = true;
          } else {
            cancel_pressed = false;
          }
          if (hidden == true) {
            SendDlgItemMessageW(dlg, 1000, EM_SETPASSWORDCHAR, L'\x25cf', 0);
            SendDlgItemMessageW(dlg, 1000, WM_LBUTTONDOWN, 0, 0);
          }
          wstring cpp_wstr_icon = widen(tstr_icon);
          if (PathFileExistsW(cpp_wstr_icon.c_str())) {
            HICON hIcon;
            ULONG_PTR m_gdiplusToken;
            Gdiplus::GdiplusStartupInput gdiplusStartupInput;
            Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);
            Bitmap *png = Bitmap::FromFile(cpp_wstr_icon.c_str());
            png->GetHICON(&hIcon);
            PostMessage(dlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            delete png;
            Gdiplus::GdiplusShutdown(m_gdiplusToken);
          } else {
            HICON hIcon = GetIcon(win);
            PostMessage(dlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
          }
          
          break;
        }
      }
      if (window_ids) xprocess::free_window_id(window_ids);
      EnableWindow(owner_window(), false);
      while (proc_id != 0 && !ngs::ps::child_proc_id_is_complete(proc_id)) {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }  
      }
      InputBoxResult.clear();
      InputBoxResult = ngs::ps::read_from_stdout_for_child_proc_id(proc_id);
      while (!InputBoxResult.empty() && (InputBoxResult.back() == ' ' || 
        InputBoxResult.back() == '\t' || InputBoxResult.back() == '\r' || InputBoxResult.back() == '\n'))
        InputBoxResult.pop_back();
      static string strResult;
      strResult = InputBoxResult;
      ngs::ps::free_stdout_for_child_proc_id(proc_id);
      ngs::ps::free_stdin_for_child_proc_id(proc_id);
      DeleteFileW((wbuff + std::wstring(L".vbs")).c_str());
      EnableWindow(owner_window(), true);
      #endif
      #ifdef _MSC_VER
      // Cleanup
      spVBScriptParse = nullptr;
      spVBScript = nullptr;
      pScriptSite->Release();
      pScriptSite = nullptr;

      ::CoUninitialize();
      static string strResult;
      _bstr_t bstrResult = (_bstr_t)result;
      strResult = narrow((wchar_t *)bstrResult);
      #endif
      if (strResult.empty()) {
        cancel_pressed = true;
      }
      return (char *)strResult.c_str();
    }

    char *get_string_helper(char *str, char *def, bool hide) {
      hidden = hide; string title = (caption == "") ? "Input Query" : caption;
      return InputBox(str, (char *)title.c_str(), def);
    }

    string remove_trailing_zeros(double numb) {
      string strnumb = to_string(numb);

      while (!strnumb.empty() && strnumb.find('.') != string::npos && (strnumb.back() == '.' || strnumb.back() == '0'))
        strnumb.pop_back();

      return strnumb;
    }

    double get_integer_helper(char *str, double def, bool hide) {
      double DIGITS_MIN = -999999999999999;
      double DIGITS_MAX = 999999999999999;

      if (def < DIGITS_MIN) def = DIGITS_MIN;
      if (def > DIGITS_MAX) def = DIGITS_MAX;

      string cpp_tdef = remove_trailing_zeros(def);
      double result = strtod(get_string_helper(str, (char *)cpp_tdef.c_str(), hide), nullptr);

      if (result < DIGITS_MIN) result = DIGITS_MIN;
      if (result > DIGITS_MAX) result = DIGITS_MAX;

      return result;
    }

    string add_slash(const string &dir) {
      if (dir.empty() || *dir.rbegin() != '\\') return dir + '\\';
      return dir;
    }

    string remove_slash(string dir) {
      while (!dir.empty() && (dir.back() == '\\' || dir.back() == '/'))
        dir.pop_back();
      return dir;
    }

    
    OPENFILENAMEW get_filename_or_filenames_helper(string filter, string fname, string dir, string title, DWORD flags) {
      filter = filter.append("||");
      fname = remove_slash(fname);

      wstring cpp_wstr_filter = widen(filter);
      wstring cpp_wstr_fname = widen(fname);
      cpp_wstr_dir = widen(dir);
      cpp_wstr_title = widen(title);
      pipefilter = cpp_wstr_filter;

      wstr_filter = new wchar_t[cpp_wstr_filter.length() + 1]();
      wcsncpy_s(wstr_filter, cpp_wstr_filter.length() + 1,
      cpp_wstr_filter.c_str(), cpp_wstr_filter.length() + 1);
      wcsncpy_s(wstr_fname, 32767, cpp_wstr_fname.c_str(), 32767);

      wstring defext;
      vector<string> pipesplit = string_split(narrow(pipefilter), '|');
      for (unsigned i = 0; i < pipesplit.size(); i++) {
        if (i % 2 != 0) {
          vector<string> semicolonsplit = string_split(pipesplit[i], ';');
          for (int j = 0; j < semicolonsplit.size(); j++) {
            semicolonsplit[j] = string_replace_all(semicolonsplit[j], "*.*", "");
            semicolonsplit[j] = string_replace_all(semicolonsplit[j], "*.", "");
            defext = widen(semicolonsplit[0]);
            break;
          }
        }
      }

      int i = 0;
      while (wstr_filter[i] != L'\0') {
        if (wstr_filter[i] == L'|') {
          wstr_filter[i] = L'\0';
        }
        i++;
      }

      ZeroMemory(&ofn, sizeof(ofn));
      ofn.lStructSize = sizeof(ofn);
      ofn.hwndOwner = owner_window();
      ofn.lpstrFile = wstr_fname;
      ofn.nMaxFile = 32767;
      ofn.lpstrFilter = wstr_filter;
      ofn.nMaxCustFilter = sizeof(wstr_filter) / sizeof(*wstr_filter);
      ofn.nFilterIndex = 0;
      ofn.lpstrDefExt = defext.c_str();
      ofn.lpstrTitle = cpp_wstr_title.c_str();
      ofn.lpstrInitialDir = cpp_wstr_dir.c_str();
      ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | flags;

      return ofn;
    }

    string get_open_filename_helper(string filter, string fname, string dir, string title) {
      ofn = get_filename_or_filenames_helper(filter, fname, dir, title, 0);

      if (GetOpenFileNameW(&ofn) != 0) {
        string result = narrow(wstr_fname);
        delete[] wstr_filter;
        wstr_filter = nullptr;
        return result;
      }

      return "";
    }
    
    string filename_path(string fname) {
      size_t fp = fname.find_last_of("\\/");
      if (fp == string::npos) return fname;
      return fname.substr(0, fp + 1);
    }

    string get_open_filenames_helper(string filter, string fname, string dir, string title) {
      files = ""; ofn = get_filename_or_filenames_helper(filter, fname, dir, title, OFN_ALLOWMULTISELECT);
      if (GetOpenFileNameW(&ofn) != 0) {
        size_t pos = 0, i = 0;
        while (wstr_fname[pos] != L'\0') {
          if (pos > 0) {
            files += add_slash(narrow(&wstr_fname[0])) +
            narrow(&wstr_fname[pos]) + "\n"; i++;
          }
          pos += wcslen(wstr_fname + pos) + 1;
        }
        if (!files.empty()) {
          files.pop_back();
        }
        if (i == 0) {
          files = narrow(&wstr_fname[0]);
        }
        delete[] wstr_filter;
        wstr_filter = nullptr;
        return files;
      }
      delete[] wstr_filter;
      wstr_filter = nullptr;
      return "";
    }

    string get_save_filename_helper(string filter, string fname, string dir, string title) {
      ofn = get_filename_or_filenames_helper(filter, fname, dir, title, OFN_OVERWRITEPROMPT);

      if (GetSaveFileNameW(&ofn) != 0) {
        string result = narrow(wstr_fname);
        delete[] wstr_filter;
        wstr_filter = nullptr;
        return result;
      }

      return "";
    }

    string get_directory_helper(string dname, string title) {
      cpp_wstr_title = widen(title);
      cpp_wstr_dir = (!dname.empty()) ? widen(dname) : L""; 
      IFileDialog *selectDirectory = nullptr;
      CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&selectDirectory));
      DWORD options = 0; selectDirectory->GetOptions(&options);
      selectDirectory->SetOptions(options | FOS_PICKFOLDERS | FOS_NOCHANGEDIR | FOS_FORCEFILESYSTEM);
      wchar_t *szFilePath = (wchar_t *)cpp_wstr_dir.c_str(); IShellItem *pItem = nullptr;
      HRESULT hr = SHCreateItemFromParsingName(szFilePath, nullptr, IID_PPV_ARGS(&pItem));
      if (SUCCEEDED(hr)) {
        wchar_t *szName = nullptr;
        hr = pItem->GetDisplayName(SIGDN_NORMALDISPLAY, &szName);
        if (SUCCEEDED(hr)) {
          selectDirectory->SetFolder(pItem);
          CoTaskMemFree(szName);
        }
        pItem->Release();
      }
      selectDirectory->SetTitle(cpp_wstr_title.c_str());
      selectDirectory->Show(owner_window()); 
      pItem = nullptr;
      hr = selectDirectory->GetResult(&pItem);
      if (SUCCEEDED(hr)) {
        wchar_t *wstr_result = nullptr;
        pItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &wstr_result);
        pItem->Release(); string str_result;
        str_result = add_slash(narrow(wstr_result));
        return str_result;
      }
      return "";
    }

    int get_color_helper(int defcol, string title) {
      CHOOSECOLORW cc;

      COLORREF DefColor = defcol;
      static COLORREF CustColors[16];

      tstr_gctitle = title;
      cpp_wstr_gctitle = widen(tstr_gctitle);

      ZeroMemory(&cc, sizeof(cc));
      cc.lStructSize = sizeof(CHOOSECOLORW);
      cc.hwndOwner = owner_window();
      cc.rgbResult = DefColor;
      cc.lpCustColors = CustColors;
      cc.Flags = CC_RGBINIT | CC_ENABLEHOOK;
      cc.lpfnHook = GetColorProc;

      return (ChooseColorW(&cc) != 0) ? cc.rgbResult : -1;
    }

  } // anonymous namespace

  int show_message(char *str) {
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &MessageBoxProc, ModHwnd, ThreadID);
    int result = show_message_helper(str, false);
    UnhookWindowsHookEx(hhook);
    return result;
  }

  int show_message_cancelable(char *str) {
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &MessageBoxProc, ModHwnd, ThreadID);
    int result = show_message_helper(str, true);
    UnhookWindowsHookEx(hhook);
    return result;
  }

  int show_question(char *str) {
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &MessageBoxProc, ModHwnd, ThreadID);
    int result = show_question_helper(str, false);
    UnhookWindowsHookEx(hhook);
    return result;
  }

  int show_question_cancelable(char *str) {
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &MessageBoxProc, ModHwnd, ThreadID);
    int result = show_question_helper(str, true);
    UnhookWindowsHookEx(hhook);
    return result;
  }

  int show_attempt(char *str) {
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &MessageBoxProc, ModHwnd, ThreadID);
    int result = show_error_helper(str, false, true);
    UnhookWindowsHookEx(hhook);
    return result;
  }

  int show_error(char *str, bool abort) {
    fatal = abort;
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &ShowErrorProc, ModHwnd, ThreadID);
    int result = show_error_helper(str, abort, false);
    UnhookWindowsHookEx(hhook);
    return result;
  }

  char *get_string(char *str, char *def) {
    return get_string_helper(str, def, false);
  }

  char *get_password(char *str, char *def) {
    return get_string_helper(str, def, true);
  }

  double get_integer(char *str, double def) {
    return get_integer_helper(str, def, false);
  }

  double get_passcode(char *str, double def) {
    return get_integer_helper(str, def, true);
  }

  char *get_open_filename(char *filter, char *fname) {
    string str_filter = filter; string str_fname = fname; static string result;
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &DialogProc, ModHwnd, ThreadID);
    result = get_open_filename_helper(str_filter, str_fname, "", "");
    UnhookWindowsHookEx(hhook);
    return (char *)result.c_str();
  }

  char *get_open_filename_ext(char *filter, char *fname, char *dir, char *title) {
    string str_filter = filter; string str_fname = fname;
    string str_dir = dir; string str_title = title; static string result;
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &DialogProc, ModHwnd, ThreadID);
    result = get_open_filename_helper(str_filter, str_fname, str_dir, str_title);
    UnhookWindowsHookEx(hhook);
    return (char *)result.c_str();
  }

  char *get_open_filenames(char *filter, char *fname) {
    string str_filter = filter; string str_fname = fname; static string result;
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &DialogProc, ModHwnd, ThreadID);
    result = get_open_filenames_helper(str_filter, str_fname, "", "");
    UnhookWindowsHookEx(hhook);
    return (char *)result.c_str();
  }

  char *get_open_filenames_ext(char *filter, char *fname, char *dir, char *title) {
    string str_filter = filter; string str_fname = fname;
    string str_dir = dir; string str_title = title; static string result;
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &DialogProc, ModHwnd, ThreadID);
    result = get_open_filenames_helper(str_filter, str_fname, str_dir, str_title);
    UnhookWindowsHookEx(hhook);
    return (char *)result.c_str();
  }

  char *get_save_filename(char *filter, char *fname) {
    string str_filter = filter; string str_fname = fname; static string result;
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &SaveAsProc, ModHwnd, ThreadID);
    result = get_save_filename_helper(str_filter, str_fname, "", "");
    UnhookWindowsHookEx(hhook);
    return (char *)result.c_str();
  }

  char *get_save_filename_ext(char *filter, char *fname, char *dir, char *title) {
    string str_filter = filter; string str_fname = fname;
    string str_dir = dir; string str_title = title; static string result;
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &SaveAsProc, ModHwnd, ThreadID);
    result = get_save_filename_helper(str_filter, str_fname, str_dir, str_title);
    UnhookWindowsHookEx(hhook);
    return (char *)result.c_str();
  }

  char *get_directory(char *dname) {
    string str_dname = dname;  static string result;
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &DialogProc, ModHwnd, ThreadID);
    result = get_directory_helper(str_dname, "");
    UnhookWindowsHookEx(hhook);
    return (char *)result.c_str();
  }

  char *get_directory_alt(char *capt, char *root) {
    string str_dname = root; string str_title = capt; static string result;
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &DialogProc, ModHwnd, ThreadID);
    result = get_directory_helper(str_dname, str_title);
    UnhookWindowsHookEx(hhook);
    return (char *)result.c_str();
  }

  int get_color(int defcol) {
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &DialogProc, ModHwnd, ThreadID);
    int result = get_color_helper(defcol, "");
    UnhookWindowsHookEx(hhook);
    return result;
  }

  int get_color_ext(int defcol, char *title) {
    string str_title = title;
    DWORD ThreadID = GetCurrentThreadId();
    HINSTANCE ModHwnd = GetModuleHandle(nullptr);
    hhook = SetWindowsHookEx(WH_CBT, &DialogProc, ModHwnd, ThreadID);
    int result = get_color_helper(defcol, str_title);
    UnhookWindowsHookEx(hhook);
    return result;
  }

  char *widget_get_caption() {
    return (char *)caption.c_str();
  }

  void widget_set_caption(char *str) {
    caption = str;
  }

  char *widget_get_owner() {
    static string hwnd;
    hwnd = std::to_string((unsigned long long)owner);
    return (char *)hwnd.c_str();
  }

  void widget_set_owner(char *hwnd) {
    owner = (void *)strtoull(hwnd, nullptr, 10);
  }

  char *widget_get_icon() {
    wchar_t wstr_icon[MAX_PATH];
    wstring cpp_wstr_icon = widen(tstr_icon);
    GetFullPathNameW(cpp_wstr_icon.c_str(), MAX_PATH, wstr_icon, nullptr);
    static string tstr_result; tstr_result = narrow(wstr_icon);
    return (char *)tstr_result.c_str();
  }

  void widget_set_icon(char *icon) {
    wchar_t wstr_icon[MAX_PATH];
    wstring cpp_wstr_icon = widen(icon);
    GetFullPathNameW(cpp_wstr_icon.c_str(), MAX_PATH, wstr_icon, nullptr);
    if (PathFileExistsW(wstr_icon)) tstr_icon = icon;
  }

  char *widget_get_system() {
    return (char *)"Win32";
  }

  void widget_set_system(char *sys) {

  }

  void widget_set_button_name(int type, char *name) {
    string str_name = name;
    btn_array[type] = str_name;
  }

  char *widget_get_button_name(int type) {
    return (char *)btn_array[type].c_str();
  }

  bool widget_get_canceled() {
    return cancel_pressed;
  }

} // namespace dialog_module
