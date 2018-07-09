#include "dialogs.h"
#include "Universal_System/estring.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/Win32/WINDOWSmain.h"
#include "Widget_Systems/widgets_mandatory.h"
#include <windows.h>
#include <Commdlg.h>
#include <comutil.h>
#include <Shlobj.h>
#include <stdio.h>
#include <wchar.h>
#include <Dlgs.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using std::string;

using enigma::hWnd;
using enigma::hInstance;

using enigma_user::string_replace_all;

namespace enigma
{
  bool widget_system_initialize()
  {
    return true;
  }
}

WCHAR wstrPromptStr[4096];
WCHAR wstrTextEntry[MAX_PATH];
WCHAR wstrWindowStr[MAX_PATH];

void ClientResize(HWND hWnd, int nWidth, int nHeight)
{
  RECT rcClient, rcWind;
  POINT ptDiff;
  GetClientRect(hWnd, &rcClient);
  GetWindowRect(hWnd, &rcWind);
  ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
  ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
  MoveWindow(hWnd, rcWind.left, rcWind.top, nWidth + ptDiff.x, nHeight + ptDiff.y, TRUE);
}

LRESULT CALLBACK InputBoxHookProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  switch (Msg)
  {
  case WM_INITDIALOG:
    ClientResize(hWndDlg, 357, 128);
    RECT rect;
    GetWindowRect(hWndDlg, &rect);
    MoveWindow(hWndDlg,
      (GetSystemMetrics(SM_CXSCREEN) / 2) - ((rect.right - rect.left) / 2),
      (GetSystemMetrics(SM_CYSCREEN) / 3) - ((rect.bottom - rect.top) / 3),
      rect.right - rect.left, rect.bottom - rect.top, TRUE);
    MoveWindow(GetDlgItem(hWndDlg, IDOK), 272, 10, 75, 23, TRUE);
    MoveWindow(GetDlgItem(hWndDlg, IDCANCEL), 272, 39, 75, 23, TRUE);
    MoveWindow(GetDlgItem(hWndDlg, 990), 11, 94, 336, 23, TRUE);
    MoveWindow(GetDlgItem(hWndDlg, 992), 11, 11, 252, 66, TRUE);
    SetWindowLongPtr(hWndDlg, GWL_EXSTYLE, GetWindowLongPtr(hWndDlg, GWL_EXSTYLE) | WS_EX_DLGMODALFRAME);
    SetWindowPos(hWndDlg, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
    SetDlgItemTextW(hWndDlg, 992, wstrPromptStr);
    SetDlgItemTextW(hWndDlg, 990, wstrTextEntry);
    SetWindowTextW(hWndDlg, wstrWindowStr);
    SendDlgItemMessage(hWndDlg, 990, EM_SETSEL, '*', NULL);
    SendDlgItemMessage(hWndDlg, 990, WM_SETFOCUS, NULL, NULL);
    return TRUE;

  case WM_COMMAND:
    switch (wParam)
    {
    case IDOK:
      GetDlgItemTextW(hWndDlg, 990, wstrTextEntry, MAX_PATH);
      EndDialog(hWndDlg, 0);
      return TRUE;
    case IDCANCEL:
      tstring wstrEmpty = widen("");
      wcsncpy(wstrTextEntry, wstrEmpty.c_str(), MAX_PATH);
      EndDialog(hWndDlg, 0);
      return TRUE;
    }
    break;
  }

  return FALSE;
}

LRESULT CALLBACK PasswordBoxHookProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  switch (Msg)
  {
  case WM_INITDIALOG:
    ClientResize(hWndDlg, 357, 128);
    RECT rect;
    GetWindowRect(hWndDlg, &rect);
    MoveWindow(hWndDlg,
      (GetSystemMetrics(SM_CXSCREEN) / 2) - ((rect.right - rect.left) / 2),
      (GetSystemMetrics(SM_CYSCREEN) / 3) - ((rect.bottom - rect.top) / 3),
      rect.right - rect.left, rect.bottom - rect.top, TRUE);
    MoveWindow(GetDlgItem(hWndDlg, IDOK), 272, 10, 75, 23, TRUE);
    MoveWindow(GetDlgItem(hWndDlg, IDCANCEL), 272, 39, 75, 23, TRUE);
    MoveWindow(GetDlgItem(hWndDlg, 990), 11, 94, 336, 23, TRUE);
    MoveWindow(GetDlgItem(hWndDlg, 992), 11, 11, 252, 66, TRUE);
    SetWindowLongPtr(hWndDlg, GWL_EXSTYLE, GetWindowLongPtr(hWndDlg, GWL_EXSTYLE) | WS_EX_DLGMODALFRAME);
    SetWindowPos(hWndDlg, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
    SetDlgItemTextW(hWndDlg, 992, wstrPromptStr);
    SetDlgItemTextW(hWndDlg, 990, wstrTextEntry);
    SetWindowTextW(hWndDlg, wstrWindowStr);
    SendDlgItemMessage(hWndDlg, 990, EM_SETPASSWORDCHAR, '*', NULL);
    SendDlgItemMessage(hWndDlg, 990, EM_SETSEL , '*', NULL);
    SendDlgItemMessage(hWndDlg, 990, WM_SETFOCUS, NULL, NULL);
    return TRUE;

  case WM_COMMAND:
    switch (wParam)
    {
    case IDOK:
      GetDlgItemTextW(hWndDlg, 990, wstrTextEntry, MAX_PATH);
      EndDialog(hWndDlg, 0);
      return TRUE;
    case IDCANCEL:
      tstring wstrEmpty = widen("");
      wcsncpy(wstrTextEntry, wstrEmpty.c_str(), MAX_PATH);
      EndDialog(hWndDlg, 0);
      return TRUE;
    }
    break;
  }

  return FALSE;
}

WCHAR *LowerCaseToActualPathName(WCHAR *wstr_dname)
{
  LPITEMIDLIST pstr_dname;
  SHParseDisplayName(wstr_dname, 0, &pstr_dname, 0, 0);

  static WCHAR wstr_result[MAX_PATH];
  SHGetPathFromIDListW(pstr_dname, wstr_result);
  return wstr_result;
}

WCHAR wstr_dname[MAX_PATH];
WCHAR wstr_label5[MAX_PATH];
tstring ActualPath;
HWND label6;

UINT APIENTRY OFNHookProcOldStyle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_INITDIALOG)
  {
    ClientResize(hWnd, 424, 255);

    RECT rect;
    GetWindowRect(hWnd, &rect);
    MoveWindow(hWnd,
      (GetSystemMetrics(SM_CXSCREEN) / 2) - ((rect.right - rect.left) / 2),
      (GetSystemMetrics(SM_CYSCREEN) / 2) - ((rect.bottom - rect.top) / 2),
      rect.right - rect.left, rect.bottom - rect.top, TRUE);

    HWND bttn1 = GetDlgItem(hWnd, IDOK);
    HWND bttn2 = GetDlgItem(hWnd, IDCANCEL);
    HWND list1 = GetDlgItem(hWnd, lst1);
    HWND list2 = GetDlgItem(hWnd, lst2);
    HWND label1 = GetDlgItem(hWnd, stc3);
    HWND label2 = GetDlgItem(hWnd, 65535);
    HWND label3 = GetDlgItem(hWnd, stc2);
    HWND label4 = GetDlgItem(hWnd, stc4);
    HWND label5 = GetDlgItem(hWnd, stc1);
    label6 = GetDlgItem(hWnd, 991);
    HWND cmbbx1 = GetDlgItem(hWnd, cmb1);
    HWND cmbbx2 = GetDlgItem(hWnd, cmb2);
    HWND txtbx1 = GetDlgItem(hWnd, edt1);

    SetWindowText(label1, "&Files: (*.*)");
    SetWindowText(label2, "&Directories:");
    SetWindowText(label3, "Directory &Name:");
    SetWindowText(label4, "D&rives:");
    DestroyWindow(cmbbx1);
    DestroyWindow(txtbx1);

    MoveWindow(bttn1, 256, 224, 77, 27, TRUE);
    MoveWindow(bttn2, 340, 224, 77, 27, TRUE);
    MoveWindow(label1, 232, 56, 72, 16, TRUE);
    MoveWindow(label2, 8, 56, 72, 16, TRUE);
    MoveWindow(label3, 8, 8, 93, 16, TRUE);
    MoveWindow(label4, 232, 176, 50, 16, TRUE);
    MoveWindow(label5, 8, 24, 409 * 100, 16, TRUE);
    MoveWindow(label6, 8, 24, 409, 16, TRUE);
    MoveWindow(list1, 232, 72, 185, 93, TRUE);
    MoveWindow(list2, 8, 72, 213, 123, TRUE);
    MoveWindow(cmbbx2, 232, 192, 185, 19, TRUE);
    ShowWindow(label5, SW_HIDE);

    DlgDirListW(hWnd, wstr_dname, lst1, stc1, DDL_ARCHIVE | DDL_READWRITE | DDL_READONLY);

    GetDlgItemTextW(hWnd, stc1, wstr_label5, MAX_PATH);

    static string DlgItemText;
    DlgItemText = shorten(LowerCaseToActualPathName(wstr_label5));
    ActualPath = widen(string_replace_all(DlgItemText + "\\", "\\\\", "\\"));

    SetDlgItemTextW(hWnd, GetDlgCtrlID(label6), ActualPath.c_str());

    PostMessageW(hWnd, WM_SETFOCUS, 0, 0);
  }

  if (uMsg == WM_CREATE)
  {
    SetWindowLongPtr(hWnd, GWL_STYLE, GetWindowLongPtr(hWnd, GWL_STYLE) | DS_FIXEDSYS);
    SetWindowPos(hWnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
  }

  if (uMsg == WM_PAINT)
  {
    GetDlgItemTextW(hWnd, stc1, wstr_label5, MAX_PATH);

    static string DlgItemText;
    DlgItemText = shorten(LowerCaseToActualPathName(wstr_label5));
    ActualPath = widen(string_replace_all(DlgItemText + "\\", "\\\\", "\\"));

    SetDlgItemTextW(hWnd, GetDlgCtrlID(label6), ActualPath.c_str());
  }

  HWND list1 = GetDlgItem(hWnd, lst1);

  if (uMsg == WM_COMMAND && HIWORD(wParam) == LBN_DBLCLK && LOWORD(wParam) == lst1)
    return TRUE;

  if (uMsg == WM_COMMAND && HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDOK)
  {
    wcsncpy(wstr_dname, ActualPath.c_str(), MAX_PATH);
    PostMessageW(hWnd, WM_COMMAND, IDABORT, 0);
    return TRUE;
  }

  if (uMsg == WM_COMMAND && HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDCANCEL)
  {
    tstring tstr_dname = widen("");
    wcsncpy(wstr_dname, tstr_dname.c_str(), MAX_PATH);
    PostMessageW(hWnd, WM_COMMAND, IDABORT, 0);
    return TRUE;
  }

  if (uMsg == WM_CLOSE)
  {
    tstring tstr_dname = widen("");
    wcsncpy(wstr_dname, tstr_dname.c_str(), MAX_PATH);
    PostMessageW(hWnd, WM_COMMAND, IDABORT, 0);
    return TRUE;
  }

  return FALSE;
}

UINT_PTR CALLBACK CCHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
  if (uiMsg == WM_INITDIALOG)
  {
    RECT rect;
    GetWindowRect(hdlg, &rect);
    MoveWindow(hdlg,
      (GetSystemMetrics(SM_CXSCREEN) / 2) - ((rect.right - rect.left) / 2),
      (GetSystemMetrics(SM_CYSCREEN) / 3) - ((rect.bottom - rect.top) / 3),
      rect.right - rect.left, rect.bottom - rect.top, TRUE);

    PostMessageW(hdlg, WM_SETFOCUS, 0, 0);
  }

  return FALSE;
}

void show_error(string errortext, const bool fatal)
{
  string strStr = errortext;
  string strWindowCaption = "Error";

  if (strStr != "")
    strStr = strStr + "\n\n";

  if (fatal == 0)
    strStr = strStr + "Do you want to abort the application?";
  else
    strStr = strStr + "Click 'OK' to abort the application.";

  tstring tstrStr = widen(strStr);
  tstring tstrWindowCaption = widen(strWindowCaption);

  double result;

  if (fatal == 0)
    result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), tstrWindowCaption.c_str(), MB_YESNO | MB_ICONERROR | MB_DEFBUTTON1 | MB_APPLMODAL);
  else
    result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), tstrWindowCaption.c_str(), MB_OK | MB_ICONERROR | MB_DEFBUTTON1 | MB_APPLMODAL);

  if (result == IDOK || result == IDYES)
    exit(0);
}

namespace enigma_user
{

void show_info(string text, int bgcolor, int left, int top, int width, int height,
  bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop,
  bool pauseGame, string caption)
{

}

int show_message(const string &str)
{
  string strWindowCaption = window_get_caption();

  tstring tstrStr = widen(str);
  tstring tstrWindowCaption = widen(strWindowCaption);

  int result;
  result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), tstrWindowCaption.c_str(), MB_OK | MB_ICONINFORMATION | MB_DEFBUTTON1 | MB_APPLMODAL);

  if (result == IDOK)
    return 1;
  else
    return 0;
}

double show_question(string str)
{
  string strWindowCaption = window_get_caption();

  tstring tstrStr = widen(str);
  tstring tstrWindowCaption = widen(strWindowCaption);

  double result;
  result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), tstrWindowCaption.c_str(), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1 | MB_APPLMODAL);

  if (result == IDYES)
    return 1;
  else
    return 0;
}

string get_string(string str, string def)
{
  string strWindowCaption = window_get_caption();

  tstring tstrStr = widen(str);
  tstring tstrDef = widen(def);
  tstring tstrWindowCaption = widen(strWindowCaption);

  wcsncpy(wstrPromptStr, tstrStr.c_str(), 4096);
  wcsncpy(wstrTextEntry, tstrDef.c_str(), MAX_PATH);
  wcsncpy(wstrWindowStr, tstrWindowCaption.c_str(), MAX_PATH);

  DialogBoxW(enigma::hInstance, MAKEINTRESOURCEW(993), enigma::hWnd, reinterpret_cast<DLGPROC>(InputBoxHookProc));

  static string strResult;
  strResult = shorten(wstrTextEntry);
  return strResult;
}

string get_password(string str, string def)
{
  string strWindowCaption = window_get_caption();

  tstring tstrStr = widen(str);
  tstring tstrDef = widen(def);
  tstring tstrWindowCaption = widen(strWindowCaption);

  wcsncpy(wstrPromptStr, tstrStr.c_str(), 4096);
  wcsncpy(wstrTextEntry, tstrDef.c_str(), MAX_PATH);
  wcsncpy(wstrWindowStr, tstrWindowCaption.c_str(), MAX_PATH);

  DialogBoxW(enigma::hInstance, MAKEINTRESOURCEW(993), enigma::hWnd, reinterpret_cast<DLGPROC>(PasswordBoxHookProc));

  static string strResult;
  strResult = shorten(wstrTextEntry);
  return strResult;
}

double get_integer(string str, double def)
{
  std::ostringstream defInteger;
  defInteger << def;
  string strDef = defInteger.str();
  string strWindowCaption = window_get_caption();

  tstring tstrStr = widen(str);
  tstring tstrDef = widen(strDef);
  tstring tstrWindowCaption = widen(strWindowCaption);

  wcsncpy(wstrPromptStr, tstrStr.c_str(), 4096);
  wcsncpy(wstrTextEntry, tstrDef.c_str(), MAX_PATH);
  wcsncpy(wstrWindowStr, tstrWindowCaption.c_str(), MAX_PATH);

  DialogBoxW(enigma::hInstance, MAKEINTRESOURCEW(993), enigma::hWnd, reinterpret_cast<DLGPROC>(InputBoxHookProc));

  static string strResult;
  strResult = shorten(wstrTextEntry);
  char *cstrResult = (char *)strResult.c_str();

  return cstrResult ? strtod(cstrResult, NULL) : 0;
}

double get_passcode(string str, double def)
{
  std::ostringstream defInteger;
  defInteger << def;
  string strDef = defInteger.str();
  string strWindowCaption = window_get_caption();

  tstring tstrStr = widen(str);
  tstring tstrDef = widen(strDef);
  tstring tstrWindowCaption = widen(strWindowCaption);

  wcsncpy(wstrPromptStr, tstrStr.c_str(), 4096);
  wcsncpy(wstrTextEntry, tstrDef.c_str(), MAX_PATH);
  wcsncpy(wstrWindowStr, tstrWindowCaption.c_str(), MAX_PATH);

  DialogBoxW(enigma::hInstance, MAKEINTRESOURCEW(993), enigma::hWnd, reinterpret_cast<DLGPROC>(PasswordBoxHookProc));

  static string strResult;
  strResult = shorten(wstrTextEntry);
  char *cstrResult = (char *)strResult.c_str();

  return cstrResult ? strtod(cstrResult, NULL) : 0;
}

string get_open_filename(string filter, string fname)
{
  OPENFILENAMEW ofn;

  string str_filter = filter.append("||");

  tstring tstr_filter = widen(str_filter);
  replace(tstr_filter.begin(), tstr_filter.end(), '|', '\0');
  tstring tstr_fname = widen(fname);

  WCHAR wstr_fname[MAX_PATH];
  wcsncpy(wstr_fname, tstr_fname.c_str(), MAX_PATH);

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = enigma::hWnd;
  ofn.lpstrFile = wstr_fname;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = tstr_filter.c_str();
  ofn.nFilterIndex = 0;
  ofn.lpstrTitle = NULL;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

  if (GetOpenFileNameW(&ofn) != 0)
  {
    static string result;
    result = shorten(wstr_fname);
    return result;
  }

  return "";
}

string get_save_filename(string filter, string fname)
{
  OPENFILENAMEW ofn;

  string str_filter = filter.append("||");

  tstring tstr_filter = widen(str_filter);
  replace(tstr_filter.begin(), tstr_filter.end(), '|', '\0');
  tstring tstr_fname = widen(fname);

  WCHAR wstr_fname[MAX_PATH];
  wcsncpy(wstr_fname, tstr_fname.c_str(), MAX_PATH);

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = enigma::hWnd;
  ofn.lpstrFile = wstr_fname;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = tstr_filter.c_str();
  ofn.nFilterIndex = 0;
  ofn.lpstrTitle = NULL;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

  if (GetSaveFileNameW(&ofn) != 0)
  {
    static string result;
    result = shorten(wstr_fname);
    return result;
  }

  return "";
}

string get_open_filename_ext(string filter, string fname, string dir, string title)
{
  OPENFILENAMEW ofn;

  string str_filter = filter.append("||");

  tstring tstr_filter = widen(str_filter);
  replace(tstr_filter.begin(), tstr_filter.end(), '|', '\0');
  tstring tstr_fname = widen(fname);
  tstring tstr_dir = widen(dir);
  tstring tstr_title = widen(title);

  WCHAR wstr_fname[MAX_PATH];
  wcsncpy(wstr_fname, tstr_fname.c_str(), MAX_PATH);

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = enigma::hWnd;
  ofn.lpstrFile = wstr_fname;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = tstr_filter.c_str();
  ofn.nFilterIndex = 0;
  ofn.lpstrTitle = tstr_title.c_str();
  ofn.lpstrInitialDir = tstr_dir.c_str();
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

  if (GetOpenFileNameW(&ofn) != 0)
  {
    static string result;
    result = shorten(wstr_fname);
    return result;
  }

  return "";
}

string get_save_filename_ext(string filter, string fname, string dir, string title)
{
  OPENFILENAMEW ofn;

  string str_filter = filter.append("||");

  tstring tstr_filter = widen(str_filter);
  replace(tstr_filter.begin(), tstr_filter.end(), '|', '\0');
  tstring tstr_fname = widen(fname);
  tstring tstr_dir = widen(dir);
  tstring tstr_title = widen(title);

  WCHAR wstr_fname[MAX_PATH];
  wcsncpy(wstr_fname, tstr_fname.c_str(), MAX_PATH);

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = enigma::hWnd;
  ofn.lpstrFile = wstr_fname;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = tstr_filter.c_str();
  ofn.nFilterIndex = 0;
  ofn.lpstrTitle = tstr_title.c_str();
  ofn.lpstrInitialDir = tstr_dir.c_str();
  ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

  if (GetSaveFileNameW(&ofn) != 0)
  {
    static string result;
    result = shorten(wstr_fname);
    return result;
  }

  return "";
}

string get_directory(string dname)
{
  OPENFILENAMEW ofn;

  tstring tstr_filter = widen("*.*|*.*|");
  replace(tstr_filter.begin(), tstr_filter.end(), '|', '\0');
  tstring tstr_dname = widen(dname);
  tstring tstr_title = widen("Select Directory");
  tstring tstr_empty = widen("");

  if (tstr_dname == tstr_empty)
    GetCurrentDirectoryW(MAX_PATH, wstr_dname);
  else
    wcsncpy(wstr_dname, tstr_dname.c_str(), MAX_PATH);

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = enigma::hWnd;
  ofn.lpstrFile = NULL;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrFilter = tstr_filter.c_str();
  ofn.nFilterIndex = 0;
  ofn.lpstrTitle = tstr_title.c_str();
  ofn.lpstrInitialDir = wstr_dname;
  ofn.Flags = OFN_ENABLETEMPLATE | OFN_ENABLEHOOK | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_LONGNAMES;
  ofn.lpTemplateName = (LPCWSTR)MAKEINTRESOURCE(FILEOPENORD);
  ofn.lpfnHook = (LPOFNHOOKPROC)OFNHookProcOldStyle;
  ofn.hInstance = enigma::hInstance;

  GetOpenFileNameW(&ofn);

  DWORD attrib = GetFileAttributesW(wstr_dname);

  if (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY))
  {
    static string result;
    result = shorten(wstr_dname);
    return result;
  }

  return "";
}

string get_directory_alt(string capt, string root)
{
  BROWSEINFOW bi;

  tstring tstr_capt = widen(capt);
  tstring tstr_root = widen(root);
  tstring tstr_slash = widen("\\");
  tstring tstr_empty = widen("");
  tstring tstr_zero = widen("0");

  LPITEMIDLIST pstr_root;

  if (tstr_root == tstr_empty)
    SHParseDisplayName(tstr_zero.c_str(), 0, &pstr_root, 0, 0);
  else
    SHParseDisplayName(tstr_root.c_str(), 0, &pstr_root, 0, 0);

  WCHAR wstr_dir[MAX_PATH];

  ZeroMemory(&bi, sizeof(bi));
  bi.hwndOwner = enigma::hWnd;
  bi.pidlRoot = pstr_root;
  bi.pszDisplayName = wstr_dir;
  bi.lpszTitle = tstr_capt.c_str();
  bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_NONEWFOLDERBUTTON;

  LPITEMIDLIST lpItem = SHBrowseForFolderW(&bi);
  if (lpItem != NULL)
  {
    if (SHGetPathFromIDListW(lpItem, wstr_dir) == 0)
      return "";
    else
    {
      static string result;
      result = string_replace_all(shorten(wstr_dir) + shorten(tstr_slash), "\\\\", "\\");
      return result;
    }
  }

  return "";
}

double get_color(double defcol)
{
  CHOOSECOLOR cc;

  COLORREF DefColor = (int)defcol;
  static COLORREF CustColors[16];

  ZeroMemory(&cc, sizeof(cc));
  cc.lStructSize = sizeof(CHOOSECOLOR);
  cc.hwndOwner = enigma::hWnd;
  cc.rgbResult = DefColor;
  cc.lpCustColors = CustColors;
  cc.Flags = CC_RGBINIT | CC_ENABLEHOOK;
  cc.lpfnHook = CCHookProc;

  if (ChooseColor(&cc) != 0)
    return (int)cc.rgbResult;
  else
    return -1;
}

}
