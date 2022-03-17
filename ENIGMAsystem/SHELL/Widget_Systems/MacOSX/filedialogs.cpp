/*

 MIT License

 Copyright © 2021 Samuel Venable

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

#include <clocale>
#include <climits>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <map>

#include <sys/stat.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#if defined(__APPLE__) && defined(__MACH__)
#include <AppKit/AppKit.h>
#include <imgui_impl_sdlrenderer.h>
#define USE_SDL_RENDERER
#else
#define USE_OGL_RENDERER
#include <imgui_impl_opengl2.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif
#endif
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include "lib/ImGuiFileDialog/ImGuiFileDialog.h"
#include "filesystem.hpp"
#include "filesystem.h"
#include <unistd.h>
#if defined(_WIN32) 
#include <windows.h>
#define STR_SLASH "\\"
#define CHR_SLASH '\\'
#else
#define STR_SLASH "/"
#define CHR_SLASH '/'
#endif

using std::string;
using std::wstring;
using std::vector;

namespace {

  void message_pump() {
    #if defined(_WIN32) 
    MSG msg; while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    #endif
  }
  
  #if defined(_WIN32) 
  wstring widen(string str) {
    size_t wchar_count = str.size() + 1; vector<wchar_t> buf(wchar_count);
    return wstring { buf.data(), (size_t)MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf.data(), (int)wchar_count) };
  }

  string narrow(wstring wstr) {
    int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), nullptr, 0, nullptr, nullptr); vector<char> buf(nbytes);
    return string { buf.data(), (size_t)WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, nullptr, nullptr) };
  }
  #endif

  string string_replace_all(string str, string substr, string nstr) {
    size_t pos = 0;
    while ((pos = str.find(substr, pos)) != string::npos) {
      message_pump();
      str.replace(pos, substr.length(), nstr);
      pos += nstr.length();
    }
    return str;
  }

  vector<string> string_split(string str, char delimiter) {
    vector<string> vec;
    std::stringstream sstr(str); string tmp;
    while (std::getline(sstr, tmp, delimiter)) {
       message_pump(); vec.push_back(tmp);
    }
    return vec;
  }

  string imgui_filter(string input) {
    input = string_replace_all(input, "\r", "");
    input = string_replace_all(input, "\n", "");
    input = string_replace_all(input, "{", "");
    input = string_replace_all(input, "}", "");
    input = string_replace_all(input, ",", "");
    vector<string> stringVec = string_split(input, '|');
    string string_output;
    unsigned index = 0;
    for (string str : stringVec) {
      message_pump();
      if (index % 2 == 0)
        string_output += str + string("{");
      else {
        std::replace(str.begin(), str.end(), ';', ',');
        string_output += string_replace_all(str, "*.", ".") + string("},");
      }
      index += 1;
    }
    if (!string_output.empty() && string_output.back() == ',') {
      string_output.pop_back();
    } else if (string_output.empty()) {
      string_output = ".*";
    }
    return string_output;
  }

  enum {
    openFile,
    openFiles,
    saveFile,
    selectFolder
  };

  string file_dialog_helper(string filter, string fname, string dir, string title, int type) {
    std::setlocale(LC_ALL, ".UTF8");
    SDL_Window *window;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
      return "";
    }
    #if defined(USE_OGL_RENDERER)
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    #endif
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
    #if defined(USE_OGL_RENDERER)
    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI |
    SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR);
    #elif defined(USE_SDL_RENDERER)
    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_ALLOW_HIGHDPI |
    SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR);
    #endif
    window = SDL_CreateWindow(title.c_str(), 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 348, windowFlags);
    if (window == nullptr) return "";
    #if defined(USE_SDL_RENDERER)
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) return "";
    #endif
    #if defined(_WIN32)
    SDL_SysWMinfo system_info;
    SDL_VERSION(&system_info.version);
    if (!SDL_GetWindowWMInfo(window, &system_info)) return "";
    HWND hWnd = system_info.info.win.window;
    SetWindowLongPtrW(hWnd, GWL_EXSTYLE, GetWindowLongPtrW(hWnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    #elif defined(__APPLE__) && defined(__MACH__)
    SDL_SysWMinfo system_info;
    SDL_VERSION(&system_info.version);
    if (!SDL_GetWindowWMInfo(window, &system_info)) return "";
    NSWindow *nsWnd = system_info.info.cocoa.window;
    [[nsWnd standardWindowButton:NSWindowCloseButton] setHidden:NO];
    [[nsWnd standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
    [[nsWnd standardWindowButton:NSWindowZoomButton] setHidden:YES];
    [[nsWnd standardWindowButton:NSWindowCloseButton] setEnabled:YES];
    [[nsWnd standardWindowButton:NSWindowMiniaturizeButton] setEnabled:NO];
    [[nsWnd standardWindowButton:NSWindowZoomButton] setEnabled:NO];
    #endif
    #if defined(USE_OGL_RENDERER)
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);
    #endif
    IMGUI_CHECKVERSION();
    ImGui::CreateContext(); if (ngs::fs::environment_get_variable("IMGUI_FONT_PATH").empty())
    ngs::fs::environment_set_variable("IMGUI_FONT_PATH", ngs::fs::executable_get_directory() + "fonts");
    if (ngs::fs::environment_get_variable("IMGUI_FONT_SIZE").empty())
    ngs::fs::environment_set_variable("IMGUI_FONT_SIZE", std::to_string(30));
    ImGuiIO& io = ImGui::GetIO(); (void)io; ImFontConfig config; 
    config.MergeMode = true; ImFont *font = nullptr; ImWchar ranges[] = { 0x0020, 0xFFFF, 0 }; 
    vector<string> fonts; fonts.push_back(ngs::fs::directory_contents_first(ngs::fs::filename_absolute(ngs::fs::environment_get_variable("IMGUI_FONT_PATH")), "*.ttf;*.otf", false, false));
    unsigned i = 0; while (!fonts[fonts.size() - 1].empty()) { fonts.push_back(ngs::fs::directory_contents_next()); message_pump(); } 
    while (!fonts[fonts.size() - 1].empty()) { fonts.pop_back(); message_pump(); }
    ngs::fs::directory_contents_close(); unsigned long long fontSize = strtoull(ngs::fs::environment_get_variable("IMGUI_FONT_SIZE").c_str(), nullptr, 10);
    // for (unsigned i = 0; i < fonts.size(); i++) { if (ngs::fs::file_exists(fonts[i])) { printf("%s\n", fonts[i].c_str()); } message_pump(); }
    for (unsigned i = 0; i < fonts.size(); i++) if (ngs::fs::file_exists(fonts[i])) io.Fonts->AddFontFromFileTTF(fonts[i].c_str(), fontSize, (!i) ? nullptr : &config, ranges);
    io.Fonts->Build(); ImGui::StyleColorsDark();
    #if defined(USE_OGL_RENDERER)
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL2_Init();
    #elif defined(USE_SDL_RENDERER)
    ImGui_ImplSDL2_InitForSDLRenderer(window);
    ImGui_ImplSDLRenderer_Init(renderer); 
    #endif
    ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    string filterNew = imgui_filter(filter); bool quit = false; SDL_Event e;
    string result; while (!quit) {
      while (SDL_PollEvent(&e)) {
        ImGui_ImplSDL2_ProcessEvent(&e);
        if (e.type == SDL_QUIT) {
          quit = true;
        }
      }
      #if defined(USE_OGL_RENDERER)
      ImGui_ImplOpenGL2_NewFrame();
      #elif defined(USE_SDL_RENDERER)
      ImGui_ImplSDLRenderer_NewFrame();
      #endif 
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame(); ImGui::SetNextWindowPos(ImVec2(0, 0));
      if (!dir.empty() && dir.back() != CHR_SLASH) dir.push_back(CHR_SLASH);
      if (type == openFile) ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "", filterNew.c_str(), dir.c_str(), fname.c_str(), 1, nullptr, 0);
      if (type == openFiles) ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "", filterNew.c_str(), dir.c_str(), fname.c_str(), 0, nullptr, 0);
      if (type == saveFile) ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "", filterNew.c_str(), dir.c_str(), fname.c_str(), 1, nullptr, 
      ImGuiFileDialogFlags_ConfirmOverwrite); if (type == selectFolder) ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "", nullptr, dir.c_str());
      int display_w, display_h; SDL_GetWindowSize(window, &display_w, &display_h);
      ImVec2 maxSize = ImVec2((float)display_w, (float)display_h); ImVec2 minSize = maxSize;
      if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey",
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove, minSize, maxSize)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
          if (type == openFile || type == openFiles) {
            auto selection = ImGuiFileDialog::Instance()->GetSelection();
            for (auto const& [key, val] : selection) {
              result += ngs::fs::filename_absolute(val) + string("\n");
            }
            if (!result.empty() && result.back() == '\n') {
              result.pop_back();
            }
          } else if (type == saveFile || type == selectFolder) {
            result = ImGuiFileDialog::Instance()->GetFilePathName();
            if (type == saveFile) {
              result = string_replace_all(ngs::fs::filename_canonical(result), ".*", "");
            } else {
              while (!result.empty() && result.back() == CHR_SLASH) {
                result.pop_back();
              }
              #if defined(_WIN32)
              if (!result.empty()) {
                result.push_back(CHR_SLASH);
              }
              #else
              result.push_back(CHR_SLASH);
              #endif
              result = ngs::fs::filename_canonical(result);
              ghc::filesystem::path respath = ghc::filesystem::path(result);
              bool ret = (ngs::fs::directory_exists(result) || strcmp(result.c_str(), 
              (respath.root_name().string() + STR_SLASH).c_str()) == 0);
              if (!ret) ret = ngs::fs::directory_create(result);
              if (!ret) result = "";
            }
          }
        }
        ImGuiFileDialog::Instance()->Close();
        quit = true;
      }

      ImGui::Render();
      #if defined(USE_OGL_RENDERER)
      glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
      glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
      SDL_GL_SwapWindow(window);
      #elif defined(USE_SDL_RENDERER)
      SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
      SDL_RenderClear(renderer);
      ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
      SDL_RenderPresent(renderer);
      #endif
    }
    #if defined(USE_OGL_RENDERER)
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    #elif defined(USE_SDL_RENDERER)
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    #endif
    SDL_DestroyWindow(window);
    SDL_Quit();
    return result;
  }

} // anonymous namespace

namespace ngs::imgui {

  string get_open_filename(string filter, string fname) {
    return file_dialog_helper(filter, fname, ngs::fs::executable_get_directory(), "Open", openFile);
  }

  string get_open_filename_ext(string filter, string fname, string dir, string title) {
    return file_dialog_helper(filter, fname, dir, title, openFile);
  }

  string get_open_filenames(string filter, string fname) {
    return file_dialog_helper(filter, fname, ngs::fs::executable_get_directory(), "Open", openFiles);
  }
 
  string get_open_filenames_ext(string filter, string fname, string dir, string title) {
    return file_dialog_helper(filter, fname, dir, title, openFiles);
  }

  string get_save_filename(string filter, string fname) {
    return file_dialog_helper(filter, fname, ngs::fs::executable_get_directory(), "Save As", saveFile);
  }

  string get_save_filename_ext(string filter, string fname, string dir, string title) {
    return file_dialog_helper(filter, fname, dir, title, saveFile);
  }

  string get_directory(string dname) {
    return file_dialog_helper("", "", dname, "Select Directory", selectFolder);
  }

  string get_directory_alt(string capt, string root) {
    return file_dialog_helper("", "", root, capt, selectFolder);
  }

} // namespace ngs::imgui

