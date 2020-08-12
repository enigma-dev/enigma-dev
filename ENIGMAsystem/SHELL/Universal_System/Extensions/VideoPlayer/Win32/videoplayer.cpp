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
#include "Universal_System/estring.h"

#include "Universal_System/Extensions/VideoPlayer/insecure_bullshit.h"
#include "Universal_System/Extensions/VideoPlayer/videoplayer.h"

#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

#include <dshow.h>

using std::string;
using std::wstring;
using std::vector;

static video_t id = 1; // starts at one because zero is reserved
// using "UINT_MAX - id" for index to avoid conflicting procinfo

namespace enigma_user {

static std::map<video_t, string> vidmap;
static std::map<video_t, wid_t> widmap;
static std::map<video_t, wid_t> cwidmap;

static std::map<video_t, bool> finmap;

static std::mutex window_mutex;
static std::mutex finish_mutex;

static IPin *pin = NULL;
static IOverlay *pOverlay = NULL;
static IGraphBuilder *pGraph = NULL;
static IVMRAspectRatioControl *pAspectRatio = NULL;
static IBaseFilter *pVideoRenderer = NULL;
static IVideoWindow *pVidWin = NULL;
static IMediaControl *pControl = NULL;
static IMediaEvent *pEvent = NULL;

static inline void finish_thread(video_t ind) {
  while (video_is_playing(ind)) {
    RECT rect;
    HWND window = (HWND)stoull(widmap.find(ind)->second;
	HWND cwindow = (HWND)stoull(cwidmap.find(ind)->second;
    GetClientRect(window, nullptr, 10), &rect);
    SetWindowLongPtr(window, GWL_STYLE, GetWindowLongPtr(window, GWL_STYLE) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    MoveWindow(cwindow, nullptr, 10), 0, 0, rect.right - rect.left, rect.bottom - rect.top, true);
  }
}

// fuck joo bitch !!!
static inline void video_thread(video_t ind, wid_t wid) {
  // MPEG-2 vidoe codec + MP2 audio codec + *.MPG = werk!
  HRESULT hr = S_OK;
  CoInitialize(NULL);
  HWND vidwin = NULL;
  RECT rect; long evCode;
  widmap.insert(std::make_pair(ind, wid));
  HWND window = (HWND)stoull(wid, nullptr, 10);
  wstring wstr_fname = widen(vidmap.find(ind)->second);
  hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&pGraph);
  if (SUCCEEDED(hr)) {
    hr = pGraph->RenderFile(wstr_fname.c_str(), NULL);
    if (SUCCEEDED(hr)) {
      hr = CoCreateInstance(CLSID_VideoMixingRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void **)&pVideoRenderer);
      if (SUCCEEDED(hr)) {
        hr = pGraph->FindFilterByName(L"Video Renderer", &pVideoRenderer);
        if (SUCCEEDED(hr)) {
          hr = pVideoRenderer->QueryInterface(IID_IVMRAspectRatioControl, (void **)&pAspectRatio);
          if (SUCCEEDED(hr)) {
            hr = pAspectRatio->SetAspectRatioMode(VMR_ARMODE_LETTER_BOX);
            if (SUCCEEDED(hr)) {
              hr = pGraph->QueryInterface(IID_IVideoWindow, (void **)&pVidWin);
              if (SUCCEEDED(hr)) {
                SetWindowLongPtr(window, GWL_STYLE, GetWindowLongPtr(window, GWL_STYLE) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
                hr = pVidWin->put_Owner((OAHWND)window);
                if (SUCCEEDED(hr)) {
                  GetClientRect(window, &rect);
                  hr = pVidWin->SetWindowPosition(0, 0, rect.right - rect.left, rect.bottom - rect.top);
                  if (SUCCEEDED(hr)) {
                    hr = pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
                    if (SUCCEEDED(hr)) {
                      hr = pVidWin->SetWindowForeground(OATRUE);
                      if (SUCCEEDED(hr)) {
                        hr = pVidWin->HideCursor(OATRUE);
                        if (SUCCEEDED(hr)) {
                          hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
                          if (SUCCEEDED(hr)) {
                            hr = pVideoRenderer->FindPin(L"VMR Input0", &pin);
                            if (SUCCEEDED(hr)) {
                              hr = pin->QueryInterface(IID_IOverlay, (void **)&pOverlay);
                              if (SUCCEEDED(hr)) {
                                hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
                                if (SUCCEEDED(hr)) {
                                  hr = pOverlay->GetWindowHandle(&vidwin);
                                  if (SUCCEEDED(hr)) {
                                    std::lock_guard<std::mutex> guard1(window_mutex);
                                    cwidmap.insert(std::make_pair(ind, std::to_string((unsigned long long)vidwin)));
                                    std::thread finthread(finish_thread, ind);
                                    hr = pControl->Run();
                                    if (SUCCEEDED(hr)) {
                                      while (ShowCursor(false) >= 0);
                                      hr = pEvent->WaitForCompletion(INFINITE, &evCode);
                                      if (SUCCEEDED(hr)) {
                                        hr = pControl->Stop();
                                        if (SUCCEEDED(hr)) {
                                          hr = pVidWin->put_Visible(OAFALSE);
                                          if (SUCCEEDED(hr)) {
                                            pVidWin->put_Owner((OAHWND)NULL);
                                          }
                                        }
                                      }
                                      while (ShowCursor(true) < 0);
                                    }
                                    std::lock_guard<std::mutex> guard2(finish_mutex);
                                    finmap[ind] = false;
                                    finthread.join();
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  if (pEvent != NULL) { pEvent->Release(); }
  if (pControl != NULL) { pControl->Release(); }
  if (pVidWin != NULL) { pVidWin->Release(); }
  if (pAspectRatio != NULL) { pAspectRatio->Release(); }
  if (pVideoRenderer != NULL) { pVideoRenderer->Release(); }
  if (pGraph != NULL) { pGraph->Release(); }
  if (pin != NULL) { pin->Release(); }
  CoUninitialize();
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
  std::lock_guard<std::mutex> guard(finish_mutex);
  finmap[ind] = true;
  std::thread vidthread(video_thread, ind, wid);  
  vidthread.detach();
}

void video_stop(video_t ind) {
  DestroyWindow((HWND)stoull(cwidmap.find(ind)->second));
  ShowWindow((HWND)stoull(cwidmap.find(ind)->second), SW_HIDE);
}

bool video_is_playing(video_t ind) {
  return finmap.find(ind)->second;
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
  finmap.erase(ind);
}

} // namespace enigma_user
