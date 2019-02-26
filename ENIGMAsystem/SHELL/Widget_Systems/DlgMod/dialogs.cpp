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

#include "Platforms/General/PFwindow.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/estring.h"

// Windows Vista or later for IFileDialog
#define NTDDI_VERSION NTDDI_VISTA
#define _WIN32_WINNT _WIN32_WINNT_VISTA

#include <windows.h>
#include <shobjidl.h> // for IFileDialog
#include <Commdlg.h>
#include <comdef.h>
#include <atlbase.h>
#include <activscp.h>
#include <Shlobj.h>
#include <cstdlib>
#include <cstdio>
#include <cwchar>
#include <algorithm>
#include <string>
#include <vector>

#ifdef DEBUG_MODE
#include "Universal_System/var4.h"
#include "Universal_System/resource_data.h"
#include "Universal_System/object.h"
#include "Universal_System/debugscope.h"
#endif

using enigma_user::string_replace_all;

using std::string;

namespace enigma {

  extern HWND hWnd;

} // namespace enigma

namespace enigma {
  
bool widget_system_initialize() {
  return true;
}
  
} // namespace enigma

static HHOOK hHook = 0;
static bool HideInput = false;
static string str_cctitle;
static tstring tstr_cctitle;

static inline tstring tstring_replace_all(tstring str, tstring substr, tstring newstr)
{
	size_t pos = 0;
	const size_t sublen = substr.length(), newlen = newstr.length();

	while ((pos = str.find(substr, pos)) != tstring::npos)
	{
		str.replace(pos, sublen, newstr);
		pos += newlen;
	}

	return str;
}

static inline string remove_trailing_zeros(double numb) 
{
	string strnumb = std::to_string(numb);

	while (!strnumb.empty() && strnumb.find('.') != string::npos && (strnumb.back() == '.' || strnumb.back() == '0'))
		strnumb.pop_back();

	return strnumb;
}

static inline string CPPNewLineToVBSNewLine(string NewLine)
{
	size_t pos = 0;

	while (pos < NewLine.length())
	{
		if (NewLine[pos] == '\n' || NewLine[pos] == '\r')
			NewLine.replace(pos, 2, "\" + vbNewLine + \"");

		pos += 1;
	}

	return NewLine;
}

static inline void CenterRectToMonitor(LPRECT prc, UINT flags)
{
	HMONITOR hMonitor;
	MONITORINFO mi;
	RECT        rc;
	int         w = prc->right - prc->left;
	int         h = prc->bottom - prc->top;

	hMonitor = MonitorFromRect(prc, MONITOR_DEFAULTTONEAREST);

	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonitor, &mi);
	rc = mi.rcMonitor;

	if (flags & MONITOR_CENTER)
	{
		prc->left = rc.left + (rc.right - rc.left - w) / 2;
		prc->top = rc.top + (rc.bottom - rc.top - h) / 2;
		prc->right = prc->left + w;
		prc->bottom = prc->top + h;
	}
	else
	{
		prc->left = rc.left + (rc.right - rc.left - w) / 2;
		prc->top = rc.top + (rc.bottom - rc.top - h) / 3;
		prc->right = prc->left + w;
		prc->bottom = prc->top + h;
	}
}

static inline void CenterWindowToMonitor(HWND hwnd, UINT flags)
{
	RECT rc;
	GetWindowRect(hwnd, &rc);
	CenterRectToMonitor(&rc, flags);
	SetWindowPos(hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

class CSimpleScriptSite :
	public IActiveScriptSite,
	public IActiveScriptSiteWindow
{
public:
	CSimpleScriptSite() : m_cRefCount(1), m_hWnd(NULL) { }

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

STDMETHODIMP_(ULONG) CSimpleScriptSite::AddRef()
{
	return InterlockedIncrement(&m_cRefCount);
}

STDMETHODIMP_(ULONG) CSimpleScriptSite::Release()
{
	if (!InterlockedDecrement(&m_cRefCount))
	{
		delete this;
		return 0;
	}
	return m_cRefCount;
}

STDMETHODIMP CSimpleScriptSite::QueryInterface(REFIID riid, void **ppvObject)
{
	if (riid == IID_IUnknown || riid == IID_IActiveScriptSiteWindow)
	{
		*ppvObject = (IActiveScriptSiteWindow *)this;
		AddRef();
		return NOERROR;
	}
	if (riid == IID_IActiveScriptSite)
	{
		*ppvObject = (IActiveScriptSite *)this;
		AddRef();
		return NOERROR;
	}
	return E_NOINTERFACE;
}

static inline LRESULT CALLBACK InputBoxProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < HC_ACTION)
		return CallNextHookEx(hHook, nCode, wParam, lParam);

	if (nCode = HCBT_ACTIVATE)
	{
		if (HideInput == true)
		{
			HWND TextBox = FindWindowEx((HWND)wParam, NULL, "Edit", NULL);
			SendDlgItemMessage((HWND)wParam, GetDlgCtrlID(TextBox), EM_SETPASSWORDCHAR, '*', 0);
		}
	}

	if (nCode = HCBT_CREATEWND)
	{
		if (!(GetWindowLongPtr((HWND)wParam, GWL_STYLE) & WS_CHILD))
			SetWindowLongPtr((HWND)wParam, GWL_EXSTYLE, GetWindowLongPtr((HWND)wParam, GWL_EXSTYLE) | WS_EX_DLGMODALFRAME);
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

static inline char *InputBox(char *Prompt, char *Title, char *Default)
{
	HRESULT hr = S_OK;
	hr = CoInitialize(NULL);

	// Initialize
	CSimpleScriptSite *pScriptSite = new CSimpleScriptSite();
	CComPtr<IActiveScript> spVBScript;
	CComPtr<IActiveScriptParse> spVBScriptParse;
	hr = spVBScript.CoCreateInstance(OLESTR("VBScript"));
	hr = spVBScript->SetScriptSite(pScriptSite);
	hr = spVBScript->QueryInterface(&spVBScriptParse);
	hr = spVBScriptParse->InitNew();

	// Replace quotes with double quotes
	string strPrompt = string_replace_all(Prompt, "\"", "\"\"");
	string strTitle = string_replace_all(Title, "\"", "\"\"");
	string strDefault = string_replace_all(Default, "\"", "\"\"");

	// Create evaluation string
	string Evaluation = "InputBox(\"" + strPrompt + "\", \"" + strTitle + "\", \"" + strDefault + "\")";
	Evaluation = CPPNewLineToVBSNewLine(Evaluation);
	tstring WideEval = enigma_user::widen(Evaluation);

	// Run InpuBox
	CComVariant result;
	EXCEPINFO ei = {};

	DWORD ThreadID = GetCurrentThreadId();
	HINSTANCE ModHwnd = GetModuleHandle(NULL);
	hr = pScriptSite->SetWindow(enigma::hWnd);
	hHook = SetWindowsHookEx(WH_CBT, &InputBoxProc, ModHwnd, ThreadID);
	hr = spVBScriptParse->ParseScriptText(WideEval.c_str(), NULL, NULL, NULL, 0, 0, SCRIPTTEXT_ISEXPRESSION, &result, &ei);
	UnhookWindowsHookEx(hHook);


	// Cleanup
	spVBScriptParse = NULL;
	spVBScript = NULL;
	pScriptSite->Release();
	pScriptSite = NULL;

	::CoUninitialize();
	static string strResult;
	_bstr_t bstrResult = (_bstr_t)result;
	strResult = enigma_user::shorten((wchar_t *)bstrResult);
	return (char *)strResult.c_str();
}

static inline UINT_PTR CALLBACK CCHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (uiMsg == WM_INITDIALOG)
	{
		CenterWindowToMonitor(hdlg, 0);
		if (str_cctitle != "") SetWindowTextW(hdlg, tstr_cctitle.c_str());
		PostMessageW(hdlg, WM_SETFOCUS, 0, 0);
	}

	return false;
}

static inline string remove_slash(string dir) {
	while (!dir.empty() && (dir.back() == '\\' || dir.back() == '/'))
		dir.pop_back();

	return dir;
}

static inline string get_open_filename_helper(string filter, string fname, string dir, string title) {
	OPENFILENAMEW ofn;

	filter = filter.append("||");
	fname = remove_slash(fname);

	tstring tstr_filter = enigma_user::widen(filter);
	replace(tstr_filter.begin(), tstr_filter.end(), '|', '\0');
	tstring tstr_fname = enigma_user::widen(fname);
	tstring tstr_dir = enigma_user::widen(dir);
	tstring tstr_title = enigma_user::widen(title);

	wchar_t wstr_fname[MAX_PATH + 1];
	wcsncpy_s(wstr_fname, tstr_fname.c_str(), MAX_PATH + 1);

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = enigma::hWnd;
	ofn.lpstrFile = wstr_fname;
	ofn.nMaxFile = MAX_PATH + 1;
	ofn.lpstrFilter = tstr_filter.c_str();
	ofn.nFilterIndex = 0;
	ofn.lpstrTitle = tstr_title.c_str();
	ofn.lpstrInitialDir = tstr_dir.c_str();
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;

	if (GetOpenFileNameW(&ofn) != 0)
		return enigma_user::shorten(wstr_fname);

	return "";
}

static inline string get_open_filenames_helper(string filter, string fname, string dir, string title) {
	OPENFILENAMEW ofn;

	filter = filter.append("||");
	fname = remove_slash(fname);

	tstring tstr_filter = enigma_user::widen(filter);
	replace(tstr_filter.begin(), tstr_filter.end(), '|', '\0');
	tstring tstr_fname = enigma_user::widen(fname);
	tstring tstr_dir = enigma_user::widen(dir);
	tstring tstr_title = enigma_user::widen(title);

	wchar_t wstr_fname1[4096];
	wcsncpy_s(wstr_fname1, tstr_fname.c_str(), 4096);

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = enigma::hWnd;
	ofn.lpstrFile = wstr_fname1;
	ofn.nMaxFile = 4096;
	ofn.lpstrFilter = tstr_filter.c_str();
	ofn.nFilterIndex = 0;
	ofn.lpstrTitle = tstr_title.c_str();
	ofn.lpstrInitialDir = tstr_dir.c_str();
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT;

	if (GetOpenFileNameW(&ofn) != 0) {
		tstring tstr_fname1 = wstr_fname1;
		tstr_fname1 += '\\';

		size_t pos = 0;
		size_t prevlen = pos;
		size_t len = wcslen(wstr_fname1);

		while (pos < len) {
			if (wstr_fname1[len - 1] != '\n' && wstr_fname1[len] == '\0')
				wstr_fname1[len] = '\n';

			prevlen = len;
			len = wcslen(wstr_fname1);
			pos += (len - prevlen) + 1;
		}

		tstring tstr_fname2 = wstr_fname1;
		if (tstr_fname2[len - 1] == '\n') tstr_fname2[len - 1] = '\0';
		if (tstr_fname2[len - 2] == '\\') tstr_fname2[len - 2] = '\0';
		tstr_fname2 = tstring_replace_all(tstr_fname2, L"\n", L"\n" + tstr_fname1);
		size_t rm = tstr_fname2.find_first_of(L'\n');

		if (rm != string::npos)
			tstr_fname2 = tstr_fname2.substr(rm + 1, tstr_fname2.length() - (rm + 1));

		if (tstr_fname2.length() >= 4095) {
			tstr_fname2 = tstr_fname2.substr(0, 4095);
			size_t end = tstr_fname2.find_last_of(L"\n");
			tstr_fname2 = tstr_fname2.substr(0, end);
		}

		return string_replace_all(enigma_user::shorten(tstr_fname2), "\\\\", "\\");
	}

	return "";
}

static inline string get_save_filename_helper(string filter, string fname, string dir, string title) {
	OPENFILENAMEW ofn;

	filter = filter.append("||");
	fname = remove_slash(fname);

	tstring tstr_filter = enigma_user::widen(filter);
	replace(tstr_filter.begin(), tstr_filter.end(), '|', '\0');
	tstring tstr_fname = enigma_user::widen(fname);
	tstring tstr_dir = enigma_user::widen(dir);
	tstring tstr_title = enigma_user::widen(title);

	wchar_t wstr_fname[MAX_PATH + 1];
	wcsncpy_s(wstr_fname, tstr_fname.c_str(), MAX_PATH + 1);

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = enigma::hWnd;
	ofn.lpstrFile = wstr_fname;
	ofn.nMaxFile = MAX_PATH + 1;
	ofn.lpstrFilter = tstr_filter.c_str();
	ofn.nFilterIndex = 0;
	ofn.lpstrTitle = tstr_title.c_str();
	ofn.lpstrInitialDir = tstr_dir.c_str();
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	if (GetSaveFileNameW(&ofn) != 0)
		return enigma_user::shorten(wstr_fname);

	return "";
}

static inline string get_directory_helper(string capt, string root) {
	IFileDialog *SelectDirectory;
	CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&SelectDirectory));

	DWORD options;
	SelectDirectory->GetOptions(&options);
	SelectDirectory->SetOptions(options | FOS_PICKFOLDERS | FOS_NOCHANGEDIR | FOS_FORCEFILESYSTEM);

	tstring tstr_dname = enigma_user::widen(root);
	LPWSTR szFilePath = (wchar_t *)tstr_dname.c_str();

	IShellItem* pItem = nullptr;
	HRESULT hr = ::SHCreateItemFromParsingName(szFilePath, nullptr, IID_PPV_ARGS(&pItem));

	if (SUCCEEDED(hr))
	{
		LPWSTR szName = nullptr;
		hr = pItem->GetDisplayName(SIGDN_NORMALDISPLAY, &szName);
		if (SUCCEEDED(hr))
		{
			SelectDirectory->SetFolder(pItem);
			::CoTaskMemFree(szName);
		}
		pItem->Release();
	}

	if (capt == "") capt = "Select Directory";
	tstring tstr_capt = enigma_user::widen(capt);

	SelectDirectory->SetTitle(tstr_capt.c_str());
	SelectDirectory->Show(enigma::hWnd);

	pItem = nullptr;
	hr = SelectDirectory->GetResult(&pItem);

	if (SUCCEEDED(hr)) {
		LPWSTR wstr_result;
		pItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &wstr_result);
		pItem->Release();

		return string_replace_all(enigma_user::shorten(wstr_result) + "\\", "\\\\", "\\");
	}

	return "";
}

static inline int get_color_helper(int defcol, string title) {
	CHOOSECOLORW cc;

	COLORREF DefColor = (int)defcol;
	static COLORREF CustColors[16];

	str_cctitle = title;
	tstr_cctitle = enigma_user::widen(str_cctitle);

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(CHOOSECOLORW);
	cc.hwndOwner = enigma::hWnd;
	cc.rgbResult = DefColor;
	cc.lpCustColors = CustColors;
	cc.Flags = CC_RGBINIT | CC_ENABLEHOOK;
	cc.lpfnHook = CCHookProc;

	if (ChooseColorW(&cc) != 0)
		return cc.rgbResult;

	return -1;
}

void show_error(string errortext, const bool fatal) {
  #ifdef DEBUG_MODE
  errortext = enigma::debug_scope::GetErrors() + "\n\n" + errortext;
  #else
  errortext = "Error in some event or another for some object: \r\n\r\n" + errortext;
  #endif

	string strWindowCaption = "Error";
	errortext = errortext + "\n\n";

	if (abort == 0)
		errortext = errortext + "Do you want to abort the application?";
	else
		errortext = errortext + "Click 'OK' to abort the application.";

	tstring tstrStr = enigma_user::widen(errortext);
	tstring tstrWindowCaption = enigma_user::widen(strWindowCaption);
	int result;

	if (abort == 0)
		result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), tstrWindowCaption.c_str(), MB_YESNO | MB_ICONERROR | MB_DEFBUTTON1 | MB_APPLMODAL);
	else
		result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), tstrWindowCaption.c_str(), MB_OK | MB_ICONERROR | MB_DEFBUTTON1 | MB_APPLMODAL);

	if (result == IDOK || result == IDYES)
		exit(0);
}

namespace enigma_user {

void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) {

}

int show_message(string str) {
	tstring tstrStr = enigma_user::widen(str);
	tstring tstrWindowCaption = enigma_user::widen(window_get_caption());
	
	int result;
	result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), tstrWindowCaption.c_str(), MB_OK | MB_ICONINFORMATION | MB_DEFBUTTON1 | MB_APPLMODAL);

	if (result == IDOK)
		return 1;

	return 0;
}

bool show_question(string str) {
	tstring tstrStr = enigma_user::widen(str);
	tstring tstrWindowCaption = enigma_user::widen(window_get_caption());
	
	int result;
	result = MessageBoxW(enigma::hWnd, tstrStr.c_str(), tstrWindowCaption.c_str(), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1 | MB_APPLMODAL);

	if (result == IDYES)
		return true;

	return false;
}

string get_string(string str, string def) {
	HideInput = 0;
  string window_caption = window_get_caption();
	char *result = InputBox(str, (char *)window_caption.c_str(), def);
	return result;
}

double get_integer(string str, double def) {
	HideInput = 1;
  string window_caption = window_get_caption();
	char *result = InputBox(str, (char *)window_caption.c_str(), def);
	return result;
}

string get_password(string str, string def) {
	string strStr = str;
	string strDef = remove_trailing_zeros(def);

	HideInput = 0;
  string window_caption = window_get_caption();
	char *result = InputBox(str, (char *)window_caption.c_str(), (char *)strDef.c_str());
	return strtod(result, NULL);
}

double get_passcode(string str, double def) {
	string strStr = str;
	string strDef = remove_trailing_zeros(def);

	HideInput = 1;
  string window_caption = window_get_caption();
	char *result = InputBox(str, (char *)window_caption.c_str(), (char *)strDef.c_str());
	return strtod(result, NULL);
}

string get_open_filename(string filter, string fname) {
  return get_open_filename_helper(filter, fname, "", "");
}

string get_open_filenames(string filter, string fname) {
  return get_open_filenames_helper(filter, fname, "", "");
}

string get_save_filename(string filter, string fname) {
  return get_save_filename_helper(filter, fname, "", "");
}

string get_open_filename_ext(string filter, string fname, string dir, string title) {
  return get_open_filename_helper(filter, fname, dir, title);
}

string get_open_filenames_ext(string filter, string fname, string dir, string title) {
  return get_open_filenames_helper(filter, fname, dir, title);
}

string get_save_filename_ext(string filter, string fname, string dir, string title) {
  return get_save_filename_helper(filter, fname, dir, title);
}

string get_directory(string dname) {
  return get_directory_helper("", dname);
}

string get_directory_alt(string capt, string root) {
  return get_directory_helper(capt, root);
}

int get_color(int defcol) {
  return get_color_helper(defcol, "");
}
  
int get_color_ext(int defcol, string title) {
  return get_color_helper(defcol, title);
}
  
} // namespace enigma_user
