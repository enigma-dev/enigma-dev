/*

 MIT License
 
 Copyright © 2020 Samuel Venable
 
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

#include <algorithm>
#include <climits>
#include <cwchar>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <map>

#include "Platforms/General/PFwindow.h"
#include "Platforms/General/PFfilemanip.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/estring.h"
#include "Universal_System/Extensions/VideoPlayer/insecure_bullshit.h"
#include "Universal_System/Extensions/VideoPlayer/videoplayer.h"

#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

#include <dshow.h>

#ifdef _MSC_VER
#pragma comment(lib, "strmiids.lib")
#endif

#define ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, func, hr) if (FAILED(hr)) {\
  DEBUG_MESSAGE(GetErrorText(func, hr), MESSAGE_TYPE::M_ERROR);\
  break;\
}

#define RELEASE_IF_DIRECTSHOW_SUCCEEDED(ptr, hr, wid) if (ptr != NULL) {\
  hr = ptr->Release();\
  ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "Release", hr);\
}

using std::string;
using std::wstring;
using std::vector;

static video_t id = 1; // starts at one because zero is reserved
// using "UINT_MAX - id" for index to avoid conflicting procinfo

static inline DWORD Win32FromHResult(HRESULT hr) {
  if ((hr & 0xFFFF0000) == MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, 0)) {
    return HRESULT_CODE(hr);
  }
  if (hr == S_OK) {
    return ERROR_SUCCESS;
  }
  return ERROR_CAN_NOT_COMPLETE;
}

static inline string GetErrorText(string func, HRESULT hr) {
  void *lpMsgBuf;
  void *lpDisplayBuf;
  wstring wstr = widen(func);
  const wchar_t *lpszFunction = wstr.c_str();
  DWORD dw = Win32FromHResult(hr); 
  FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
  FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
  (wchar_t *)&lpMsgBuf, 0, NULL);
  lpDisplayBuf = (void *)LocalAlloc(LMEM_ZEROINIT, 
  (wcslen((wchar_t *)lpMsgBuf) + wcslen((wchar_t *)lpszFunction) + 40) * sizeof(wchar_t)); 
  StringCchPrintfW((wchar_t *)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(wchar_t), 
  L"%s failed with error %d: %s", lpszFunction, dw, (wchar_t *)lpMsgBuf); 
  string errortext = shorten((wchar_t *)lpDisplayBuf); 
  LocalFree(lpMsgBuf);
  LocalFree(lpDisplayBuf);
  return errortext;
}

namespace enigma_user {

static std::map<video_t, string> vidmap;
static std::map<video_t, wid_t>  widmap;
static std::map<video_t, wid_t> cwidmap;
static std::map<video_t, string> updmap;

static std::mutex window_mutex;
static std::mutex update_mutex;

static inline void update_thread(video_t ind) {
  while (video_is_playing(ind)) {
    HWND window = (HWND)stoull(widmap.find(ind)->second, nullptr, 10);
    HWND cwindow = (HWND)stoull(cwidmap.find(ind)->second, nullptr, 10);
    if (!IsWindow(cwindow)) {
      std::lock_guard<std::mutex> guard2(update_mutex);
      updmap.erase(ind);
      updmap.insert(std::make_pair(ind, std::to_string(0)));
      return;
    }
    LONG_PTR style = GetWindowLongPtr(window, GWL_STYLE);
    if (!(style & (WS_CLIPCHILDREN | WS_CLIPSIBLINGS))) {
      SetWindowLongPtr(window, GWL_STYLE, style | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    }
    RECT rect; GetClientRect(window, &rect);
    MoveWindow(cwindow, 0, 0, rect.right - rect.left, rect.bottom - rect.top, true);
  }
}

// fuck joo bitch !!!
static inline void video_thread(video_t ind, wid_t wid) {
  // MPEG-2 vidoe codec + MP2 audio codec + *.MPG = werk!
  HRESULT                 hr             = S_OK;
  IGraphBuilder          *pGraph         = NULL;
  IBaseFilter            *pVideoRenderer = NULL;
  IVMRAspectRatioControl *pAspectRatio   = NULL;
  IVideoWindow           *pVidWin        = NULL;
  IPin                   *pPin           = NULL;
  IMediaControl          *pControl       = NULL;
  IOverlay               *pOverlay       = NULL;
  IMediaEvent            *pEvent         = NULL;
  switch (0) default: {
    hr = CoInitialize(NULL);
    ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "CoInitialize", hr);
    HWND vidwin = NULL;
    RECT rect; long evCode;
    widmap.insert(std::make_pair(ind, wid));
    HWND window = (HWND)stoull(wid, nullptr, 10);
    wstring wstr_fname = widen(vidmap.find(ind)->second);
    switch (0) default: {
      hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pGraph);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "CoCreateInstance", hr);
      hr = pGraph->RenderFile(wstr_fname.c_str(), NULL);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "RenderFile", hr);
      hr = CoCreateInstance(CLSID_VideoMixingRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pVideoRenderer);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "CoCreateInstance", hr);
      hr = pGraph->FindFilterByName(L"Video Renderer", &pVideoRenderer);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "FindFilterByName", hr);
      hr = pVideoRenderer->QueryInterface(IID_IVMRAspectRatioControl, (void **)&pAspectRatio);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "QueryInterface", hr);
      hr = pAspectRatio->SetAspectRatioMode(VMR_ARMODE_LETTER_BOX);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "SetAspectRatioMode", hr);
      hr = pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "QueryInterface", hr);
      SetWindowLongPtr(window, GWL_STYLE, GetWindowLongPtr(window, GWL_STYLE) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
      hr = pVidWin->put_Owner((OAHWND)window);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "put_Owner", hr);
      GetClientRect(window, &rect);
      hr = pVidWin->SetWindowPosition(0, 0, rect.right - rect.left, rect.bottom - rect.top);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "SetWindowPosition", hr);
      hr = pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "put_WindowStyle", hr);
      hr = pVidWin->SetWindowForeground(OATRUE);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "SetWindowForeground", hr);
      hr = pVidWin->HideCursor(OATRUE);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "HideCursor", hr);
      hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "QueryInterface", hr);
      hr = pVideoRenderer->FindPin(L"VMR Input0", &pPin);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "FindPin", hr);
      hr = pPin->QueryInterface(IID_IOverlay, (void **)&pOverlay);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "QueryInterface", hr);
      hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "QueryInterface", hr);
      hr = pOverlay->GetWindowHandle(&vidwin);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "GetWindowHandle", hr);
      std::lock_guard<std::mutex> guard1(window_mutex);
      cwidmap.insert(std::make_pair(ind, std::to_string((unsigned long long)vidwin)));
      std::thread updthread(update_thread, ind);
      switch (0) default: {
        hr = pControl->Run();
        ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "Run", hr);
        while (ShowCursor(false) >= 0);
        switch (0) default: {
          hr = pEvent->WaitForCompletion(INFINITE, &evCode);
          ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "WaitForCompletion", hr);
          hr = pControl->Stop();
          ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "Stop", hr);
          hr = pVidWin->put_Visible(OAFALSE);
          ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "put_Visible", hr);
          hr = pVidWin->put_Owner((OAHWND)NULL);
          ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(wid, "put_Owner", hr);
        }
        while (ShowCursor(true) < 0);
      }
      updthread.join();
    }
    RELEASE_IF_DIRECTSHOW_SUCCEEDED(pGraph, hr, wid);
    RELEASE_IF_DIRECTSHOW_SUCCEEDED(pVideoRenderer, hr, wid);
    RELEASE_IF_DIRECTSHOW_SUCCEEDED(pAspectRatio, hr, wid);
    RELEASE_IF_DIRECTSHOW_SUCCEEDED(pVidWin, hr, wid);
    RELEASE_IF_DIRECTSHOW_SUCCEEDED(pPin, hr, wid);
    RELEASE_IF_DIRECTSHOW_SUCCEEDED(pControl, hr, wid);
    RELEASE_IF_DIRECTSHOW_SUCCEEDED(pOverlay, hr, wid);
    RELEASE_IF_DIRECTSHOW_SUCCEEDED(pEvent, hr, wid);
    CoUninitialize();
  }
  std::lock_guard<std::mutex> guard2(update_mutex);
  updmap.erase(ind);
}

video_t video_add(std::string fname) {
  video_t ind = UINT_MAX - id;
  fname = enigma_insecure::echo(UINT_MAX, fname); // expand environment variables
  enigma_insecure::process_clear_out(UINT_MAX); // grab output right by the pussy
  enigma_insecure::process_clear_pid(UINT_MAX); // grab procid right by the pussy
  if (file_exists(fname)) {
    vidmap.insert(std::make_pair(ind, fname)); 
    id++;
  }
  return ind;
}

void video_play(video_t ind, wid_t wid) {
  std::lock_guard<std::mutex> guard(update_mutex);
  updmap.insert(std::make_pair(ind, std::to_string(1)));
  std::thread vidthread(video_thread, ind, wid);  
  vidthread.detach();
}

void video_stop(video_t ind) {
  ShowWindow((HWND)stoull(cwidmap.find(ind)->second), SW_HIDE);
  SetParent((HWND)stoull(cwidmap.find(ind)->second), NULL);
  DestroyWindow((HWND)stoull(cwidmap.find(ind)->second));
}

bool video_is_playing(video_t ind) {
  if (!updmap.find(ind)->second.empty()) {
    return (bool)stoul(updmap.find(ind)->second, nullptr, 10);
  }
  return false;
}

wid_t video_get_winid(video_t ind) {
  return widmap.find(ind)->second;
}

bool video_exists(video_t ind) {
  return (!vidmap.find(ind)->second.empty());
}

void video_delete(video_t ind) {
  vidmap.erase(ind);
  widmap.erase(ind);
  cwidmap.erase(ind);
  widmap.insert(std::make_pair(ind, std::to_string(0)));
  cwidmap.insert(std::make_pair(ind, std::to_string(0)));
}

} // namespace enigma_user
