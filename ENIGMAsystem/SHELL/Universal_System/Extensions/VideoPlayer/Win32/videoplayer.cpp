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
#define ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED(func, hr) if (FAILED(hr)) {\
  DEBUG_MESSAGE(GetErrorText(func, hr), MESSAGE_TYPE::M_ERROR);\
  break;\
}\

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
static std::map<video_t, wid_t> widmap;
static std::map<video_t, wid_t> cwidmap;

static std::map<video_t, bool> updmap;

static std::mutex window_mutex;
static std::mutex update_mutex;

static IPin *pin = NULL;
static IOverlay *pOverlay = NULL;
static IGraphBuilder *pGraph = NULL;
static IVMRAspectRatioControl *pAspectRatio = NULL;
static IBaseFilter *pVideoRenderer = NULL;
static IVideoWindow *pVidWin = NULL;
static IMediaControl *pControl = NULL;
static IMediaEvent *pEvent = NULL;

static inline void update_thread(video_t ind) {
  while (video_is_playing(ind)) {
    HWND window = (HWND)stoull(widmap.find(ind)->second, nullptr, 10);
    HWND cwindow = (HWND)stoull(cwidmap.find(ind)->second, nullptr, 10);
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
  HRESULT hr = S_OK;
  switch (0) default: {
    hr = CoInitialize(NULL);
    ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("CoInitialize", hr);
    HWND vidwin = NULL;
    RECT rect; long evCode;
    widmap.insert(std::make_pair(ind, wid));
    HWND window = (HWND)stoull(wid, nullptr, 10);
    wstring wstr_fname = widen(vidmap.find(ind)->second);
    switch (0) default: {
      hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pGraph);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("CoCreateInstance", hr);
      hr = pGraph->RenderFile(wstr_fname.c_str(), NULL);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("RenderFile", hr);
      hr = CoCreateInstance(CLSID_VideoMixingRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pVideoRenderer);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("CoCreateInstance", hr);
      hr = pGraph->FindFilterByName(L"Video Renderer", &pVideoRenderer);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("FindFilterByName", hr);
      hr = pVideoRenderer->QueryInterface(IID_IVMRAspectRatioControl, (void **)&pAspectRatio);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("QueryInterface", hr);
      hr = pAspectRatio->SetAspectRatioMode(VMR_ARMODE_LETTER_BOX);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("SetAspectRatioMode", hr);
      hr = pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("QueryInterface", hr);
      SetWindowLongPtr(window, GWL_STYLE, GetWindowLongPtr(window, GWL_STYLE) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
      hr = pVidWin->put_Owner((OAHWND)window);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("put_Owner", hr);
      GetClientRect(window, &rect);
      hr = pVidWin->SetWindowPosition(0, 0, rect.right - rect.left, rect.bottom - rect.top);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("SetWindowPosition", hr);
      hr = pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("put_WindowStyle", hr);
      hr = pVidWin->SetWindowForeground(OATRUE);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("SetWindowForeground", hr);
      hr = pVidWin->HideCursor(OATRUE);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("HideCursor", hr);
      hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("QueryInterface", hr);
      hr = pVideoRenderer->FindPin(L"VMR Input0", &pin);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("FindPin", hr);
      hr = pin->QueryInterface(IID_IOverlay, (void **)&pOverlay);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("QueryInterface", hr);
      hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("QueryInterface", hr);
      hr = pOverlay->GetWindowHandle(&vidwin);
      ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("GetWindowHandle", hr);
      std::lock_guard<std::mutex> guard1(window_mutex);
      cwidmap.insert(std::make_pair(ind, std::to_string((unsigned long long)vidwin)));
      std::thread updthread(update_thread, ind);
      switch (0) default: {
        hr = pControl->Run();
        ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("Run", hr);
        while (ShowCursor(false) >= 0);
        switch (0) default: {
          hr = pEvent->WaitForCompletion(INFINITE, &evCode);
          ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("WaitForCompletion", hr);
          hr = pControl->Stop();
          ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("Stop", hr);
          hr = pVidWin->put_Visible(OAFALSE);
          ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("put_Visible", hr);
          hr = pVidWin->put_Owner((OAHWND)NULL);
          ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("put_Owner", hr);
        }
        while (ShowCursor(true) < 0);
      }
      updthread.join();
      if (pEvent != NULL) {
        hr = pEvent->Release();
        ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("Release", hr);
      }
      if (pControl != NULL) {
        hr = pControl->Release();
        ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("Release", hr);
      }
      if (pVidWin != NULL) {
        hr = pVidWin->Release();
        ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("Release", hr);
      }
      if (pAspectRatio != NULL) {
        hr = pAspectRatio->Release();
        ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("Release", hr);
      }
      if (pVideoRenderer != NULL) {
        hr = pVideoRenderer->Release();
        ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("Release", hr);
      }
      if (pGraph != NULL) {
        hr = pGraph->Release();
        ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("Release", hr);
      }
      if (pin != NULL) {
        hr = pin->Release();
        ERROR_AND_BREAK_IF_DIRECTSHOW_FAILED("Release", hr);
      }
    }
    CoUninitialize();
  }
  std::lock_guard<std::mutex> guard2(update_mutex);
  updmap[ind] = false;
}

video_t video_add(std::string fname) {
  video_t ind = UINT_MAX - id;
  fname = enigma_insecure::echo(UINT_MAX, fname); // expand environment variables
  enigma_insecure::process_clear_out(UINT_MAX); // grab output right by the pussy
  enigma_insecure::process_clear_pid(UINT_MAX); // grab procid right by the pussy
  if (file_exists(fname)) {
    vidmap.insert(std::make_pair(ind, fname)); 
    id++;
  } else {
    DEBUG_MESSAGE("The specified video file does not exist.", MESSAGE_TYPE::M_ERROR);
  }
  return ind;
}

void video_play(video_t ind, wid_t wid) {
  std::lock_guard<std::mutex> guard(update_mutex);
  updmap[ind] = true;
  std::thread vidthread(video_thread, ind, wid);  
  vidthread.detach();
}

void video_stop(video_t ind) {
  DestroyWindow((HWND)stoull(cwidmap.find(ind)->second));
  ShowWindow((HWND)stoull(cwidmap.find(ind)->second), SW_HIDE);
}

bool video_is_playing(video_t ind) {
  return updmap.find(ind)->second;
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
  updmap.erase(ind);
}

} // namespace enigma_user
