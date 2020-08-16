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
#include <chrono>
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

#define ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS(func, ...) {\
  HRESULT hr = (__VA_ARGS__);\
  if (FAILED(hr)) {\
    DEBUG_MESSAGE(GetErrorText(func, hr), MESSAGE_TYPE::M_ERROR);\
    break;\
  }\
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
  wid_t wid = widmap.find(ind)->second;
  wid_t cwid = cwidmap.find(ind)->second;
  while (!cwid.empty() && video_is_playing(ind)) {
    HWND window = (HWND)stoull(wid, nullptr, 10);
    HWND cwindow = (HWND)stoull(cwid, nullptr, 10);
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
  IGraphBuilder          *pGraph         = NULL;
  IBaseFilter            *pVideoRenderer = NULL;
  IVMRAspectRatioControl *pAspectRatio   = NULL;
  IVideoWindow           *pVidWin        = NULL;
  IPin                   *pPin           = NULL;
  IMediaControl          *pControl       = NULL;
  IOverlay               *pOverlay       = NULL;
  IMediaEvent            *pEvent         = NULL;
  switch (0) default: {
    ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("CoInitialize", CoInitialize(NULL));
    HWND vidwin = NULL;
    RECT rect; long evCode;
    widmap.erase(ind);
    widmap.insert(std::make_pair(ind, wid));
    HWND window = (HWND)stoull(wid, nullptr, 10);
    wstring wstr_fname = widen(vidmap.find(ind)->second);
    switch (0) default: {
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("CoCreateInstance", CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pGraph));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("RenderFile", pGraph->RenderFile(wstr_fname.c_str(), NULL));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("CoCreateInstance", CoCreateInstance(CLSID_VideoMixingRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pVideoRenderer));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("FindFilterByName", pGraph->FindFilterByName(L"Video Renderer", &pVideoRenderer));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("QueryInterface", pVideoRenderer->QueryInterface(IID_IVMRAspectRatioControl, (void **)&pAspectRatio));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("SetAspectRatioMode", pAspectRatio->SetAspectRatioMode(VMR_ARMODE_LETTER_BOX));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("QueryInterface", pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin));
      SetWindowLongPtr(window, GWL_STYLE, GetWindowLongPtr(window, GWL_STYLE) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("put_Owner", pVidWin->put_Owner((OAHWND)window));
      GetClientRect(window, &rect);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("SetWindowPosition", pVidWin->SetWindowPosition(0, 0, rect.right - rect.left, rect.bottom - rect.top));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("put_WindowStyle", pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("SetWindowForeground", pVidWin->SetWindowForeground(OATRUE));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("HideCursor", pVidWin->HideCursor(OATRUE));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("QueryInterface", pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("FindPin", pVideoRenderer->FindPin(L"VMR Input0", &pPin));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("QueryInterface", pPin->QueryInterface(IID_IOverlay, (void **)&pOverlay));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("QueryInterface", pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("GetWindowHandle", pOverlay->GetWindowHandle(&vidwin));
      cwidmap.erase(ind);
      cwidmap.insert(std::make_pair(ind, std::to_string((unsigned long long)vidwin)));
      std::thread updthread(update_thread, ind);
      updthread.detach();
      switch (0) default: {
        ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("Run", pControl->Run());
        switch (0) default: {
          ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("WaitForCompletion", pEvent->WaitForCompletion(INFINITE, &evCode));
        }
        ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("Stop", pControl->Stop());
      }
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("put_Visible", pVidWin->put_Visible(OAFALSE));
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("put_Owner", pVidWin->put_Owner((OAHWND)NULL));
    }
    if (pGraph != NULL) ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("Release", pGraph->Release());
    if (pVideoRenderer != NULL) ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("Release", pVideoRenderer->Release());
    if (pAspectRatio != NULL) ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("Release", pAspectRatio->Release());
    if (pVidWin != NULL) ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("Release", pVidWin->Release());
    if (pPin != NULL) ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("Release", pPin->Release());
    if (pControl != NULL) ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("Release", pControl->Release());
    if (pOverlay != NULL) ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("Release", pOverlay->Release());
    if (pEvent != NULL) ERROR_AND_BREAK_IF_DIRECTSHOW_FAILS("Release", pEvent->Release());
    CoUninitialize();
  }
  video_stop(ind);
}

video_t video_add(std::string fname) {
  video_t ind = UINT_MAX - id;
  fname = enigma_insecure::echo(UINT_MAX, fname); // expand environment variables
  enigma_insecure::process_clear_out(UINT_MAX); // grab output right by the pussy
  enigma_insecure::process_clear_pid(UINT_MAX); // grab procid right by the pussy
  if (file_exists(fname)) {
    vidmap.erase(ind);
    vidmap.insert(std::make_pair(ind, fname)); 
    id++;
  }
  return ind;
}

void video_play(video_t ind, wid_t wid) {
  std::thread vidthread(video_thread, ind, wid);  
  vidthread.detach();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::lock_guard<std::mutex> guard(update_mutex);
  updmap.erase(ind);
  updmap.insert(std::make_pair(ind, std::to_string(1)));
}

void video_stop(video_t ind) {
  wid_t cwid = cwidmap.find(ind)->second;
  if (!cwid.empty() && video_is_playing(ind)) {
    HWND cwindow = (HWND)stoull(cwid, nullptr, 10);
    ShowWindow(cwindow, SW_HIDE);
    SetParent(cwindow, NULL);
    DestroyWindow(cwindow);
    std::lock_guard<std::mutex> guard(update_mutex);
    updmap.erase(ind);
    updmap.insert(std::make_pair(ind, std::to_string(0)));
  }
}

bool video_is_playing(video_t ind) {
  std::lock_guard<std::mutex> guard(update_mutex);
  string condition = updmap.find(ind)->second;
  if (!condition.empty()) {
    return (condition == std::to_string(1));
  }
  return false;
}

wid_t video_get_winid(video_t ind) {
  if (video_is_playing(ind)) {
    std::lock_guard<std::mutex> guard(window_mutex);
    return widmap.find(ind)->second;
  }
  return std::to_string(0);
}

bool video_exists(video_t ind) {
  return (!vidmap.find(ind)->second.empty());
}

void video_delete(video_t ind) {
  if (video_exists(ind)) {
    vidmap.erase(ind);
    std::lock_guard<std::mutex> guard(window_mutex);
    widmap.erase(ind);
    widmap.insert(std::make_pair(ind, std::to_string(0)));
  }
}

} // namespace enigma_user
