/*

 MIT License

 Copyright © 2021-2022 Samuel Venable

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

#include <climits>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <map>

#include "SDL.h"
#include "SDL_syswm.h"
#include "filedialogs.h"
#if defined(__APPLE__) && defined(__MACH__)
#include "imgui_impl_sdlrenderer.h"
#else
#include "imgui_impl_opengl2.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include "SDL_opengles2.h"
#else
#include "SDL_opengl.h"
#endif
#endif
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "ImFileDialog.h"
#include "filesystem.hpp"
#include "filesystem.h"

#include <sys/stat.h>
#if defined(_WIN32) 
#include <windows.h>
#include <dwmapi.h>
#define STR_SLASH "\\"
#define CHR_SLASH '\\'
#define HOME_PATH "USERPROFILE"
#else
#if defined(__APPLE__) && defined(__MACH__)
#include <AppKit/AppKit.h>
#endif
#include <unistd.h>
#define STR_SLASH "/"
#define CHR_SLASH '/'
#define HOME_PATH "HOME"
#endif

#if defined(_MSC_VER)
#if defined(_WIN32) && !defined(_WIN64)
#pragma comment(lib, __FILE__"\\..\\lib\\x86\\SDL2.lib")
#elif defined(_WIN32) && defined(_WIN64)
#pragma comment(lib, __FILE__"\\..\\lib\\x64\\SDL2.lib")
#endif
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

  string imgui_filter(string input, bool is_folder) {
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
    if (!is_folder) {
      string_output += ".*";
    }
    return string_output;
  }

  enum {
    openFile,
    openFiles,
    saveFile,
    selectFolder
  };

  void ApplyDefaultStyle() {
    ImVec4 *colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]               = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border]                = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg]               = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg]               = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive]         = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark]             = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab]            = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header]                = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered]         = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive]          = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip]            = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab]                   = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered]            = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive]             = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram]         = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight]      = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget]        = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight]          = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding                    = ImVec2(8.00f, 8.00f);
    style.FramePadding                     = ImVec2(5.00f, 2.00f);
    style.CellPadding                      = ImVec2(6.00f, 6.00f);
    style.ItemSpacing                      = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing                 = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding                = ImVec2(0.00f, 0.00f);
    style.IndentSpacing                    = 25;
    style.ScrollbarSize                    = 15;
    style.GrabMinSize                      = 10;
    style.WindowBorderSize                 = 1;
    style.ChildBorderSize                  = 1;
    style.PopupBorderSize                  = 1;
    style.FrameBorderSize                  = 1;
    style.TabBorderSize                    = 1;
    style.WindowRounding                   = 0;
    style.ChildRounding                    = 4;
    style.FrameRounding                    = 3;
    style.PopupRounding                    = 0;
    style.ScrollbarRounding                = 9;
    style.GrabRounding                     = 3;
    style.LogSliderDeadzone                = 4;
    style.TabRounding                      = 4;
  }

  string expand_without_trailing_slash(string dname) {
    std::error_code ec;
    dname = ngs::fs::environment_expand_variables(dname);
    ghc::filesystem::path p = ghc::filesystem::path(dname);
    p = ghc::filesystem::absolute(p, ec);
    if (ec.value() != 0) return "";
    dname = p.string();
    #if defined(_WIN32)
    while ((dname.back() == '\\' || dname.back() == '/') && 
      (p.root_name().string() + "\\" != dname && p.root_name().string() + "/" != dname)) {
      message_pump(); p = ghc::filesystem::path(dname); dname.pop_back();
    }
    #else
    while (dname.back() == '/' && (!dname.empty() && dname[0] != '/' && dname.length() != 1)) {
      dname.pop_back();
    }
    #endif
    return dname;
  }

  vector<string> fonts;
  #if (defined(__MACH__) && defined(__APPLE__))
  SDL_Renderer *renderer = nullptr;
  SDL_Surface *surf = nullptr;
  #endif

  void ifd_load_fonts() {
    if (ngs::fs::environment_get_variable("IMGUI_FONT_PATH").empty()) {
      if (!fonts.empty()) fonts.clear();
      ngs::fs::environment_set_variable("IMGUI_FONT_PATH", ngs::fs::executable_get_directory() + "fonts");
      fonts.push_back(ngs::fs::directory_contents_first(ngs::fs::environment_get_variable("IMGUI_FONT_PATH"), "*.ttf;*.otf", false, false));
      while (!fonts[fonts.size() - 1].empty()) {
        message_pump();
        fonts.push_back(ngs::fs::directory_contents_next());
      }
      if (!fonts.empty()) fonts.pop_back();
      ngs::fs::directory_contents_close();
    } else {
      #if !defined(IFD_SHARED_LIBRARY)
      ngs::imgui::ifd_load_fonts();
      #endif
    }
  }

  string file_dialog_helper(string filter, string fname, string dir, string title, int type) {
    SDL_Window *window;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
      return "";
    }
    #if (!defined(__MACH__) && !defined(__APPLE__))
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    #endif
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
    #if (!defined(__MACH__) && !defined(__APPLE__))
    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | 
    SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR | SDL_WINDOW_HIDDEN);
    #else
    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_ALWAYS_ON_TOP | 
    SDL_WINDOW_SKIP_TASKBAR | SDL_WINDOW_HIDDEN);
    #endif
    if (ngs::fs::environment_get_variable("IMGUI_DIALOG_WIDTH").empty())
    ngs::fs::environment_set_variable("IMGUI_DIALOG_WIDTH", std::to_string(640));
    if (ngs::fs::environment_get_variable("IMGUI_DIALOG_HEIGHT").empty())
    ngs::fs::environment_set_variable("IMGUI_DIALOG_HEIGHT", std::to_string(360));
    window = SDL_CreateWindow(title.c_str(), 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, IFD_DIALOG_WIDTH, IFD_DIALOG_HEIGHT, windowFlags);
    if (window == nullptr) return "";
    #if (defined(__MACH__) && defined(__APPLE__))
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) return "";
    #else
    if (type == selectFolder) {
      SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *)ifd::folder_icon, 32, 32, 32, 32 * 4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
      SDL_SetWindowIcon(window, surface);
      SDL_FreeSurface(surface);
    } else {
      SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *)ifd::file_icon, 32, 32, 32, 32 * 4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
      SDL_SetWindowIcon(window, surface);
      SDL_FreeSurface(surface);
    }
    #endif
    #if defined(_WIN32)
    SDL_SysWMinfo system_info;
    SDL_VERSION(&system_info.version);
    if (!SDL_GetWindowWMInfo(window, &system_info)) return "";
    HWND hWnd = system_info.info.win.window;
    SetWindowLongPtrW(hWnd, GWL_STYLE, GetWindowLongPtrW(hWnd, GWL_STYLE) & ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX));
    SetWindowLongPtrW(hWnd, GWL_EXSTYLE, GetWindowLongPtrW(hWnd, GWL_EXSTYLE) | WS_EX_TOPMOST);
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
    #if (!defined(__MACH__) && !defined(__APPLE__))
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);
    #endif
    IMGUI_CHECKVERSION();
    ImGui::CreateContext(); ifd_load_fonts();
    if (ngs::fs::environment_get_variable("IMGUI_FONT_SIZE").empty())
    ngs::fs::environment_set_variable("IMGUI_FONT_SIZE", std::to_string(20));
    ImGuiIO& io = ImGui::GetIO(); (void)io; ImFontConfig config; io.IniFilename = nullptr;
    config.MergeMode = true; ImFont *font = nullptr; ImWchar ranges[] = { 0x0020, 0xFFFF, 0 }; 
    float fontSize = (float)strtod(ngs::fs::environment_get_variable("IMGUI_FONT_SIZE").c_str(), nullptr);
    for (unsigned i = 0; i < fonts.size(); i++) if (ngs::fs::file_exists(fonts[i])) io.Fonts->AddFontFromFileTTF(fonts[i].c_str(), fontSize, (!i) ? nullptr : &config, ranges);
    io.Fonts->Build(); ApplyDefaultStyle();
    #if (!defined(__MACH__) && !defined(__APPLE__))
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL2_Init();
    #else
    ImGui_ImplSDL2_InitForSDLRenderer(window);
    ImGui_ImplSDLRenderer_Init(renderer); 
    #endif
    ifd::FileDialog::Instance().CreateTexture = [](uint8_t *data, int w, int h, char fmt) -> void * {
      #if (!defined(__MACH__) && !defined(__APPLE__))
      GLuint tex;
      glGenTextures(1, &tex);
      glBindTexture(GL_TEXTURE_2D, tex);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      #if defined(IMGUI_IMPL_OPENGL_ES2)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      #else
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
      #endif
      glBindTexture(GL_TEXTURE_2D, 0);
      return (void *)(uintptr_t)tex;
      #else
      if (surf) SDL_FreeSurface(surf);
      surf = SDL_CreateRGBSurfaceFrom((void *)data, w, h, 32, w * 4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
      SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
      return (void *)tex;
      #endif
    };
    ifd::FileDialog::Instance().DeleteTexture = [](void *tex) {
      #if (!defined(__MACH__) && !defined(__APPLE__))
      GLuint texID = (GLuint)(uintptr_t)tex;
      glDeleteTextures(1, &texID);
      #else
      SDL_DestroyTexture((SDL_Texture *)tex);
      #endif
    };
    ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    string filterNew = imgui_filter(filter, (type == selectFolder)); 
    bool quit = false; SDL_Event e;
    string result; while (!quit) {
      while (SDL_PollEvent(&e)) {
        ImGui_ImplSDL2_ProcessEvent(&e);
        if (e.type == SDL_QUIT) {
          quit = true;
        }
      }
      #if (!defined(__MACH__) && !defined(__APPLE__))
      ImGui_ImplOpenGL2_NewFrame();
      #else
      ImGui_ImplSDLRenderer_NewFrame();
      #endif 
      ImGui_ImplSDL2_NewFrame(); ImGui::NewFrame(); ImGui::SetNextWindowPos(ImVec2(0, 0)); dir = expand_without_trailing_slash(dir);
      if (type == openFile) ifd::FileDialog::Instance().Open("GetOpenFileName", "Open", filterNew.c_str(), false, fname.c_str(), dir.c_str());
      if (type == openFiles) ifd::FileDialog::Instance().Open("GetOpenFileNames", "Open", filterNew.c_str(), true, fname.c_str(), dir.c_str());
      if (type == selectFolder) ifd::FileDialog::Instance().Open("GetDirectory", "Select Directory", "", false, fname.c_str(), dir.c_str());
      if (type == saveFile) ifd::FileDialog::Instance().Save("GetSaveFileName", "Save As", filterNew.c_str(), fname.c_str(), dir.c_str());
      if (ifd::FileDialog::Instance().IsDone("GetOpenFileName")) {
        if (ifd::FileDialog::Instance().HasResult()) {
          result = ifd::FileDialog::Instance().GetResult().string();
        }
        ifd::FileDialog::Instance().Close();
        goto finish;
      }
      if (ifd::FileDialog::Instance().IsDone("GetOpenFileNames")) {
        if (ifd::FileDialog::Instance().HasResult()) {
          const std::vector<ghc::filesystem::path>& res = ifd::FileDialog::Instance().GetResults();
          for (const auto& r : res) result += r.string() + "\n";
          if (!result.empty()) result.pop_back();
        }
        ifd::FileDialog::Instance().Close();
        goto finish;
      }
      if (ifd::FileDialog::Instance().IsDone("GetDirectory")) {
        if (ifd::FileDialog::Instance().HasResult()) {
          result = ifd::FileDialog::Instance().GetResult().string();
          if (!result.empty() && result.back() != CHR_SLASH) result.push_back(CHR_SLASH);
        }
        ifd::FileDialog::Instance().Close();
        goto finish;
      }
      if (ifd::FileDialog::Instance().IsDone("GetSaveFileName")) {
        if (ifd::FileDialog::Instance().HasResult()) {
          result = ifd::FileDialog::Instance().GetResult().string();
        }
        ifd::FileDialog::Instance().Close();
        goto finish;
      }
      ImGui::Render();
      #if (!defined(__MACH__) && !defined(__APPLE__))
      glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
      glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
      SDL_GL_SwapWindow(window);
      #else
      SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
      SDL_RenderClear(renderer);
      ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
      SDL_RenderPresent(renderer);
      #endif
      if (SDL_GetWindowFlags(window) & SDL_WINDOW_HIDDEN) {
        SDL_ShowWindow(window);
      }
    }
    finish:
    #if (!defined(__MACH__) && !defined(__APPLE__))
    ImGui_ImplOpenGL2_Shutdown();
    #else
    ImGui_ImplSDLRenderer_Shutdown();
    #endif
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    #if (!defined(__MACH__) && !defined(__APPLE__))
    SDL_GL_DeleteContext(gl_context);
    #endif
    SDL_DestroyWindow(window);
    SDL_Quit();
    return result;
  }

} // anonymous namespace

namespace ngs::imgui {

  void ifd_load_fonts() {
    if (!fonts.empty()) fonts.clear();
    if (!ngs::fs::environment_get_variable("IMGUI_FONT_PATH").empty()) {
      fonts.push_back(ngs::fs::directory_contents_first(ngs::fs::environment_get_variable("IMGUI_FONT_PATH"), "*.ttf;*.otf", false, false));
      while (!fonts[fonts.size() - 1].empty()) {
        message_pump();
        fonts.push_back(ngs::fs::directory_contents_next());
      }
      if (!fonts.empty()) fonts.pop_back();
      ngs::fs::directory_contents_close();
    }
  }

  string get_open_filename(string filter, string fname) {
    return file_dialog_helper(((filter.empty()) ? "*.*" : filter), fname, ngs::fs::environment_get_variable(HOME_PATH), "Open", openFile);
  }

  string get_open_filename_ext(string filter, string fname, string dir, string title) {
    return file_dialog_helper(((filter.empty()) ? "*.*" : filter), fname, ((dir.empty()) ? ngs::fs::environment_get_variable(HOME_PATH) : dir), title, openFile);
  }

  string get_open_filenames(string filter, string fname) {
    return file_dialog_helper(((filter.empty()) ? "*.*" : filter), fname, ngs::fs::environment_get_variable(HOME_PATH), "Open", openFiles);
  }
 
  string get_open_filenames_ext(string filter, string fname, string dir, string title) {
    return file_dialog_helper(((filter.empty()) ? "*.*" : filter), fname, ((dir.empty()) ? ngs::fs::environment_get_variable(HOME_PATH) : dir), title, openFiles);
  }

  string get_save_filename(string filter, string fname) {
    return file_dialog_helper(((filter.empty()) ? "*.*" : filter), fname, ngs::fs::environment_get_variable(HOME_PATH), "Save As", saveFile);
  }

  string get_save_filename_ext(string filter, string fname, string dir, string title) {
    return file_dialog_helper(((filter.empty()) ? "*.*" : filter), fname, ((dir.empty()) ? ngs::fs::environment_get_variable(HOME_PATH) : dir), title, saveFile);
  }

  string get_directory(string dname) {
    return file_dialog_helper("", "", ((dname.empty()) ? ngs::fs::environment_get_variable(HOME_PATH) : dname), "Select Directory", selectFolder);
  }

  string get_directory_alt(string capt, string root) {
    return file_dialog_helper("", "", ((root.empty()) ? ngs::fs::environment_get_variable(HOME_PATH) : root), capt, selectFolder);
  }

} // namespace ngs::imgui

#if defined(IFD_SHARED_LIBRARY)
void ifd_load_fonts() {
  ngs::imgui::ifd_load_fonts();
}

const char *get_open_filename(const char *filter, const char *fname) {
  static string result;
  result = ngs::imgui::get_open_filename(filter, fname);
  return result.c_str();
}

const char *get_open_filename_ext(const char *filter, const char *fname, const char *dir, const char *title) {
  static string result;
  result = ngs::imgui::get_open_filename_ext(filter, fname, dir, title);
  return result.c_str();
}

const char *get_open_filenames(const char *filter, const char *fname) {
  static string result;
  result = ngs::imgui::get_open_filenames(filter, fname);
  return result.c_str();
}

const char *get_open_filenames_ext(const char *filter, const char *fname, const char *dir, const char *title) {
  static string result;
  result = ngs::imgui::get_open_filenames_ext(filter, fname, dir, title);
  return result.c_str();
}

const char *get_save_filename(const char *filter, const char *fname) {
  static string result;
  result = ngs::imgui::get_save_filename(filter, fname);
  return result.c_str();
}

const char *get_save_filename_ext(const char *filter, const char *fname, const char *dir, const char *title) {
  static string result;
  result = ngs::imgui::get_save_filename_ext(filter, fname, dir, title);
  return result.c_str();
}

const char *get_directory(const char *dname) {
  static string result;
  result = ngs::imgui::get_directory(dname);
  return result.c_str();
}

const char *get_directory_alt(const char *capt, const char *root) {
  static string result;
  result = ngs::imgui::get_directory_alt(capt, root);
  return result.c_str();
}
#endif
