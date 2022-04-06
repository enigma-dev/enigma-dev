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

#include "sys/stat.h"
#include "SDL.h"
#include "SDL_syswm.h"
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

#if defined(_WIN32) 
#include <windows.h>
#include <dwmapi.h>
#define STR_SLASH "\\"
#define CHR_SLASH '\\'
#else
#if defined(__APPLE__) && defined(__MACH__)
#include <AppKit/AppKit.h>
#endif
#include <unistd.h>
#define STR_SLASH "/"
#define CHR_SLASH '/'
#endif

#if defined(_MSC_VER)
#if defined(_WIN32) && !defined(_WIN64)
#pragma comment(lib, __FILE__"\\..\\lib\\x86\\SDL2.lib")
#elif defined(_WIN32) && defined(_WIN64)
#pragma comment(lib, __FILE__"\\..\\lib\\x64\\SDL2.lib")
#endif
#pragma comment(lib, "Dwmapi.lib")
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
      string_output.push_back('.');
      string_output.push_back('*');
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
    colors[ImGuiCol_NavHighlight]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);
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

  #if defined(_WIN32)
  BOOL is_light_theme() {
    auto buffer = std::vector<char>(4);
    auto cbData = static_cast<DWORD>(buffer.size() * sizeof(char));
    auto res = RegGetValueW(HKEY_CURRENT_USER, 
      L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", L"AppsUseLightTheme",
      RRF_RT_REG_DWORD, nullptr, buffer.data(), &cbData);
    if (res != ERROR_SUCCESS) return TRUE;
    auto i = int(buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0]);
    return (BOOL)(i == 1);
  }
  #endif

  #if (defined(__MACH__) && defined(__APPLE__))
  SDL_Renderer *renderer = nullptr;
  SDL_Surface *surf = nullptr;
  #endif

  string file_dialog_helper(string filter, string dir, string title, int type) {
    SDL_Window *window;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
      return "";
    }
    #if (!defined(__MACH__) && !defined(__APPLE__))
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    #endif
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
    #if (!defined(__MACH__) && !defined(__APPLE__))
    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI |
    SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR);
    #else
    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_ALLOW_HIGHDPI |
    SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR);
    #endif
    window = SDL_CreateWindow(title.c_str(), 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, windowFlags);
    if (window == nullptr) return "";
    #if (defined(__MACH__) && defined(__APPLE__))
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) return "";
    #endif
    if (type == selectFolder) {
      SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *)ifd::folder_icon, 32, 32, 32, 32 * 4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
      SDL_SetWindowIcon(window, surface);
      SDL_FreeSurface(surface);
    } else {
      SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *)ifd::file_icon, 32, 32, 32, 32 * 4, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
      SDL_SetWindowIcon(window, surface);
      SDL_FreeSurface(surface);
    }
    #if defined(_WIN32)
    SDL_SysWMinfo system_info;
    SDL_VERSION(&system_info.version);
    if (!SDL_GetWindowWMInfo(window, &system_info)) return "";
    HWND hWnd = system_info.info.win.window;
    SetWindowLongPtrW(hWnd, GWL_STYLE, GetWindowLongPtrW(hWnd, GWL_STYLE) & ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX));
    SetWindowLongPtrW(hWnd, GWL_EXSTYLE, GetWindowLongPtrW(hWnd, GWL_EXSTYLE) | WS_EX_TOPMOST);
    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    BOOL b = !is_light_theme(); DwmSetWindowAttribute(hWnd, 20, &b, sizeof(BOOL));
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
    ImGui::CreateContext(); if (ngs::fs::environment_get_variable("IMGUI_FONT_PATH").empty())
    ngs::fs::environment_set_variable("IMGUI_FONT_PATH", ngs::fs::executable_get_directory() + "fonts");
    if (ngs::fs::environment_get_variable("IMGUI_FONT_SIZE").empty())
    ngs::fs::environment_set_variable("IMGUI_FONT_SIZE", std::to_string(18));
    ImGuiIO& io = ImGui::GetIO(); (void)io; ImFontConfig config; 
    config.MergeMode = true; ImFont *font = nullptr; ImWchar ranges[] = { 0x0020, 0xFFFF, 0 }; 
    vector<string> fonts; fonts.push_back(ngs::fs::directory_contents_first(ngs::fs::environment_get_variable("IMGUI_FONT_PATH"), "*.ttf;*.otf", false, false));
    unsigned i = 0; while (!fonts[fonts.size() - 1].empty()) { fonts.push_back(ngs::fs::directory_contents_next()); message_pump(); } 
    while (!fonts[fonts.size() - 1].empty()) { fonts.pop_back(); message_pump(); }
    ngs::fs::directory_contents_close(); float fontSize = (float)strtod(ngs::fs::environment_get_variable("IMGUI_FONT_SIZE").c_str(), nullptr);
    // for (unsigned i = 0; i < fonts.size(); i++) { if (ngs::fs::file_exists(fonts[i])) { printf("%s\n", fonts[i].c_str()); } message_pump(); }
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
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
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
    string filterNew = imgui_filter(filter); bool quit = false; SDL_Event e;
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
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame(); ImGui::SetNextWindowPos(ImVec2(0, 0));
      if (!dir.empty() && dir.back() != CHR_SLASH) dir.push_back(CHR_SLASH);
      if (type == openFile) ifd::FileDialog::Instance().Open("GetOpenFileName", "Open", filterNew.c_str(), false, dir.c_str());
      if (type == openFiles) ifd::FileDialog::Instance().Open("GetOpenFileNames", "Open", filterNew.c_str(), true, dir.c_str());
      if (type == selectFolder) ifd::FileDialog::Instance().Open("GetDirectory", "Select Directory", "", false, dir.c_str());
      if (type == saveFile) ifd::FileDialog::Instance().Save("GetSaveFileName", "Save As", filterNew.c_str(), dir.c_str());
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

  string get_open_filename(string filter) {
    return file_dialog_helper(filter, ngs::fs::executable_get_directory(), "Open", openFile);
  }

  string get_open_filename_ext(string filter, string dir, string title) {
    return file_dialog_helper(filter, dir, title, openFile);
  }

  string get_open_filenames(string filter) {
    return file_dialog_helper(filter, ngs::fs::executable_get_directory(), "Open", openFiles);
  }
 
  string get_open_filenames_ext(string filter, string dir, string title) {
    return file_dialog_helper(filter, dir, title, openFiles);
  }

  string get_save_filename(string filter) {
    return file_dialog_helper(filter, ngs::fs::executable_get_directory(), "Save As", saveFile);
  }

  string get_save_filename_ext(string filter, string dir, string title) {
    return file_dialog_helper(filter, dir, title, saveFile);
  }

  string get_directory(string dname) {
    return file_dialog_helper("", dname, "Select Directory", selectFolder);
  }

  string get_directory_alt(string capt, string root) {
    return file_dialog_helper("", root, capt, selectFolder);
  }

} // namespace ngs::imgui

