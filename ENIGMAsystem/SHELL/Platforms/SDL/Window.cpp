#include "Window.h"
#include "Event.h"

#include "Platforms/General/PFwindow.h"
#include "Platforms/platforms_mandatory.h"

#include <string>

using enigma::windowHandle;

namespace enigma {
SDL_Window* windowHandle = nullptr;
static SDL_Event_Handler eventHandler;

void initInput() {}

void handleInput() { input_push(); }

void showWindow() { SDL_ShowWindow(windowHandle); }

bool initGameWindow() {
  windowHandle = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_HIDDEN);

  return (windowHandle != nullptr);
}

void destroyWindow() {
  SDL_DestroyWindow(windowHandle);
  SDL_Quit();
}

int handleEvents() { return eventHandler.processEvents(); }

}  // namespace enigma

namespace enigma_user {

void io_handle() {}

int window_get_visible() {
  Uint32 flags = SDL_GetWindowFlags(windowHandle);
  return (flags & SDL_WINDOW_SHOWN);
}

void window_set_visible(bool visible) { (visible) ? SDL_ShowWindow(windowHandle) : SDL_HideWindow(windowHandle); }

std::string window_get_caption() { return SDL_GetWindowTitle(windowHandle); }

void window_set_caption(std::string caption) { SDL_SetWindowTitle(windowHandle, caption.c_str()); }

int window_mouse_get_x() {
  int x;
  SDL_GetMouseState(&x, nullptr);
  return x;
}

int window_mouse_get_y() {
  int y;
  SDL_GetMouseState(nullptr, &y);
  return y;
}

void window_mouse_set(int x, int y) { SDL_WarpMouseInWindow(windowHandle, x, y); }

int display_mouse_get_x() {
  int x;
  SDL_GetGlobalMouseState(&x, nullptr);
  return x;
}

int display_mouse_get_y() {
  int y;
  SDL_GetGlobalMouseState(nullptr, &y);
  return y;
}

void display_mouse_set(int x, int y) { SDL_WarpMouseGlobal(x, y); }

bool window_get_stayontop() {
  Uint32 flags = SDL_GetWindowFlags(windowHandle);
  return (flags & SDL_WINDOW_ALWAYS_ON_TOP);
}

void window_set_stayontop(bool stay) {
  /* SDL (as of 2.0.5) can set SDL_WINDOW_ALWAYS_ON_TOP 
  *  upon creation but not after. Also, this only works 
  * in X11
  */
}

bool window_get_sizeable() {
  Uint32 flags = SDL_GetWindowFlags(windowHandle);
  return (flags & SDL_WINDOW_RESIZABLE);
}

void window_set_sizeable(bool resize) { SDL_SetWindowResizable(windowHandle, (resize) ? SDL_TRUE : SDL_FALSE); }

bool window_get_showborder() {
  Uint32 flags = SDL_GetWindowFlags(windowHandle);
  return !(flags & SDL_WINDOW_BORDERLESS);
}

void window_set_showborder(bool show) { SDL_SetWindowBordered(windowHandle, (show) ? SDL_TRUE : SDL_FALSE); }

bool window_get_minimized() {
  Uint32 flags = SDL_GetWindowFlags(windowHandle);
  return (flags & SDL_WINDOW_MINIMIZED);
}

void window_set_minimized(bool min) {
  if (min != window_get_minimized()) (min) ? SDL_MinimizeWindow(windowHandle) : SDL_RestoreWindow(windowHandle);
}

bool window_get_maximized() {
  Uint32 flags = SDL_GetWindowFlags(windowHandle);
  return (flags & SDL_WINDOW_MAXIMIZED);
}

void window_set_maximized(bool max) {
  if (max != window_get_maximized()) (max) ? SDL_MaximizeWindow(windowHandle) : SDL_RestoreWindow(windowHandle);
}

int window_get_x() {
  int x;
  SDL_GetWindowPosition(windowHandle, &x, nullptr);
  return x;
}

int window_get_y() {
  int y;
  SDL_GetWindowPosition(windowHandle, nullptr, &y);
  return y;
}

void window_set_position(int x, int y) { SDL_SetWindowPosition(windowHandle, x, y); }

void window_set_rectangle(int x, int y, int w, int h) {
  window_set_size(w, h);
  window_set_position(x, y);
}

int window_get_width() {
  int w;
  SDL_GetWindowSize(windowHandle, &w, nullptr);
  return w;
}

int window_get_height() {
  int h;
  SDL_GetWindowSize(windowHandle, nullptr, &h);
  return h;
}

void window_set_size(unsigned w, unsigned h) { SDL_SetWindowSize(windowHandle, w, h); }

bool window_get_fullscreen() {
  Uint32 flags = SDL_GetWindowFlags(windowHandle);
  return (flags & SDL_WINDOW_FULLSCREEN || flags & SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void window_set_fullscreen(bool fullscreen) {
  if (fullscreen) {
    int r = SDL_SetWindowFullscreen(windowHandle, SDL_WINDOW_FULLSCREEN);

    if (r != 0) r = SDL_SetWindowFullscreen(windowHandle, SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (r != 0) printf("Could not set window to fullscreen! SDL Error: %s\n", SDL_GetError());
  } else {
    int r = SDL_SetWindowFullscreen(windowHandle, 0);
    if (r != 0) printf("Could not unset window fullscreen! SDL Error: %s\n", SDL_GetError());
  }
}

int window_set_cursor(int cursorID) {
  SDL_Cursor* cursor;

  if (cursorID == cr_none)
    SDL_ShowCursor(SDL_DISABLE);
  else
    SDL_ShowCursor(SDL_ENABLE);

  switch (cursorID) {
    case cr_default:
    case cr_arrow:
      cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
      break;

    case cr_cross:
      cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
      break;

    case cr_beam:
      cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
      break;

    case cr_size_nesw:
      cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
      break;

    case cr_size_ns:
      cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
      break;

    case cr_size_nwse:
      cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
      break;

    case cr_size_we:
      cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
      break;

    case cr_nodrop:
    case cr_no:
      cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
      break;

    case cr_handpoint:
      cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
      break;

    case cr_size_all:
      cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
      break;

    default:
      printf("Cursor lookup failure\n");
      break;
  }

  if (cursor != nullptr) {
    //enigma::cursorInt = cursor;
    SDL_SetCursor(cursor);
    return 1;
  }

  return 0;
}

bool os_is_paused() {
  Uint32 flags = SDL_GetWindowFlags(windowHandle);
  return enigma::freezeOnLoseFocus && flags & SDL_WINDOW_INPUT_FOCUS;
}

int display_get_width() {
  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
  return DM.w;
}

int display_get_height() {
  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
  return DM.h;
}

bool keyboard_check_direct(int key) {
  const Uint8* state = SDL_GetKeyboardState(nullptr);
  return state[key];
}
}  // namespace enigma_user
