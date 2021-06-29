#include "Window.h"
#include "Event.h"
#include "Joystick.h"
#include "Gamepad.h"
#include "Icon.h"

#include "Platforms/General/PFwindow.h"
#include "Platforms/platforms_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/estring.h" // ord
#include "Universal_System/roomsystem.h" // room_caption, update_mouse_variables

#include <array>
#include <string>
#include <algorithm>

template<typename K, typename V>
static std::unordered_map<V,K> inverse_map(std::unordered_map<K,V> &map) {
    std::unordered_map<V,K> inv;
    std::for_each(map.begin(), map.end(),
                [&inv] (const std::pair<K,V> &p)
                {
                    inv.insert(std::make_pair(p.second, p.first));
                });
    return inv;
}


using enigma::windowHandle;

namespace enigma {
void (*WindowResizedCallback)();

SDL_Window* windowHandle = nullptr;
unsigned sdl_window_flags = SDL_WINDOW_HIDDEN;

// this is to be implemented by an SDL bridge
// it is for setting any attributes on the
// window before it is created
// (e.g, SDL's OpenGL context attributes)
void init_sdl_window_bridge_attributes();


bool initGameWindow() {
  SDL_Init(SDL_INIT_VIDEO);
  if (isSizeable) sdl_window_flags |= SDL_WINDOW_RESIZABLE;
  if (!showBorder) sdl_window_flags |= SDL_WINDOW_BORDERLESS;
  if (isFullScreen) sdl_window_flags |= SDL_WINDOW_FULLSCREEN;
  init_sdl_window_bridge_attributes();
  windowHandle = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, sdl_window_flags);
  bool notnull = (windowHandle != nullptr);
  if (notnull) window_init();
  return notnull;
}
namespace keyboard {
  using namespace enigma_user;
  std::unordered_map<int,SDL_Keycode> keymap = {
    {SDLK_LEFT, vk_left}, {SDLK_RIGHT, vk_right}, {SDLK_UP, vk_up}, {SDLK_DOWN, vk_down},
    {SDLK_TAB, vk_tab}, {SDLK_RETURN, vk_enter}, {SDLK_SPACE, vk_space},
    {SDLK_LSHIFT, vk_shift}, {SDLK_LCTRL, vk_control}, {SDLK_LALT, vk_alt},
    {SDLK_RSHIFT, vk_shift}, {SDLK_RCTRL, vk_control}, {SDLK_RALT, vk_alt},
    {SDLK_0, vk_numpad0}, {SDLK_1, vk_numpad1}, {SDLK_2, vk_numpad2}, {SDLK_3, vk_numpad3},
    {SDLK_4, vk_numpad4}, {SDLK_5, vk_numpad5}, {SDLK_6, vk_numpad6}, {SDLK_7, vk_numpad7},
    {SDLK_8, vk_numpad8}, {SDLK_9, vk_numpad9},
    {SDLK_KP_MULTIPLY, vk_multiply}, {SDLK_KP_PLUS, vk_add}, {SDLK_KP_MINUS, vk_subtract},
    {SDLK_KP_DECIMAL, vk_decimal}, {SDLK_KP_DIVIDE, vk_divide},
    {SDLK_F1, vk_f1}, {SDLK_F2, vk_f2}, {SDLK_F3, vk_f3}, {SDLK_F4, vk_f4}, {SDLK_F5, vk_f5},
    {SDLK_F6, vk_f6}, {SDLK_F7, vk_f7}, {SDLK_F8, vk_f8}, {SDLK_F9, vk_f9}, {SDLK_F10, vk_f10},
    {SDLK_F11, vk_f11}, {SDLK_F12, vk_f12},
    {SDLK_BACKSPACE, vk_backspace}, {SDLK_ESCAPE, vk_escape}, {SDLK_PAGEUP, vk_pageup},
    {SDLK_PAGEDOWN, vk_pagedown}, {SDLK_END, vk_end}, {SDLK_HOME, vk_home},
    {SDLK_INSERT, vk_insert}, {SDLK_DELETE, vk_delete},
    {SDLK_PRINTSCREEN, vk_printscreen}, {SDLK_CAPSLOCK, vk_caps}, {SDLK_SCROLLLOCK, vk_scroll},
    {SDLK_PAUSE, vk_pause}, {SDLK_LGUI, vk_lsuper}, {SDLK_RGUI, vk_rsuper}
  };

  std::unordered_map<SDL_Keycode, int> inverse_keymap;
}

static SDL_Event_Handler eventHandler;
static std::array<SDL_Cursor*, -enigma_user::cr_size_all+1> cursors;

void handleInput() {
  input_push();
  joystick_update();
  pushGamepads();
}

void initCursors() {
  // cursors are negative ids 0 to -22
  cursors[-enigma_user::cr_arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
  cursors[-enigma_user::cr_default] = cursors[-enigma_user::cr_arrow];
  cursors[-enigma_user::cr_cross] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
  cursors[-enigma_user::cr_beam] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
  cursors[-enigma_user::cr_size_nesw] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
  cursors[-enigma_user::cr_size_ns] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
  cursors[-enigma_user::cr_size_nwse] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
  cursors[-enigma_user::cr_size_we] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
  cursors[-enigma_user::cr_no] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
  cursors[-enigma_user::cr_nodrop] = cursors[-enigma_user::cr_no];
  cursors[-enigma_user::cr_handpoint] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
  cursors[-enigma_user::cr_size_all] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
}

void cleanupCursors() {
  for (int i = enigma_user::cr_size_all; i <= 0; ++i) SDL_FreeCursor(cursors[-i]);
}

void initInput() {
  for (int i = 0; i <= 9; i++) {
    enigma::keyboard::keymap[SDLK_0 + i] = 48 + i;
  }

  for (char i = 'A'; i <= 'Z'; i++) {
    int idx = i - 'A';
    enigma::keyboard::keymap[SDLK_a + idx] = 65 + idx;
  }

  keyboard::inverse_keymap = inverse_map(keyboard::keymap);
  initCursors();
  joystick_init();
  initGamepads();
}

void destroyWindow() {
  cleanupGamepads();
  cleanupCursors();
  SDL_DestroyWindow(windowHandle);
  SDL_Quit();
}

int handleEvents() { return eventHandler.processEvents(); }

}  // namespace enigma

namespace enigma_user {

static int currentIconIndex = -1;
static unsigned currentIconFrame;

int window_get_icon_index() {
  return currentIconIndex;
}

unsigned window_get_icon_subimg() {
  return currentIconFrame;
}

void window_set_icon(int ind, unsigned subimg) {
  // the line below prevents glitchy minimizing when 
  // icons are changed rapidly (i.e. for animation).
  if (window_get_minimized()) return;

  // needs to be visible first to prevent segfault
  if (!window_get_visible()) window_set_visible(true);
  enigma::SetIconFromSprite(windowHandle, ind, subimg);

  currentIconIndex = ind;
  currentIconFrame = subimg;
}

int window_get_visible() {
  Uint32 flags = SDL_GetWindowFlags(windowHandle);
  return (flags & SDL_WINDOW_SHOWN);
}

void window_set_visible(bool visible) { (visible) ? SDL_ShowWindow(windowHandle) : SDL_HideWindow(windowHandle); }

std::string window_get_caption() { return SDL_GetWindowTitle(windowHandle); }

void window_set_caption(const std::string& caption) { SDL_SetWindowTitle(windowHandle, caption.c_str()); }

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

#if SDL_VERSION_ATLEAST(2, 0, 4)
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

#else
#warning "display_mouse_get_x(), display_mouse_get_y() and display_mouse_set() require SDL >= 2.0.4"
#endif

#if SDL_VERSION_ATLEAST(2, 0, 5)
bool window_get_stayontop() {
  Uint32 flags = SDL_GetWindowFlags(windowHandle);
  return (flags & SDL_WINDOW_ALWAYS_ON_TOP);
}

#else
#warning "window_get_stayontop() and window_set_stayontop() require SDL >= 2.0.5"
#endif

void window_set_stayontop(bool top) {}

bool window_get_sizeable() {
  Uint32 flags = SDL_GetWindowFlags(windowHandle);
  return (flags & SDL_WINDOW_RESIZABLE);
}

#if SDL_VERSION_ATLEAST(2, 0, 5)
void window_set_sizeable(bool resize) { SDL_SetWindowResizable(windowHandle, (resize) ? SDL_TRUE : SDL_FALSE); }
#endif

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
  int viewportWidth;
  int viewportHeight;
  SDL_GL_GetDrawableSize(windowHandle, &viewportWidth, &viewportHeight);
  return viewportWidth;

}

int window_get_height() {
  int viewportWidth;
  int viewportHeight;
  SDL_GL_GetDrawableSize(windowHandle, &viewportWidth, &viewportHeight);
  return viewportHeight;

}

bool window_get_fullscreen() {
  Uint32 flags = SDL_GetWindowFlags(windowHandle);
  return (flags & SDL_WINDOW_FULLSCREEN || flags & SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void window_set_fullscreen(bool fullscreen) {
  if (fullscreen) {
    int r = SDL_SetWindowFullscreen(windowHandle, SDL_WINDOW_FULLSCREEN_DESKTOP);

    if (r != 0) r = SDL_SetWindowFullscreen(windowHandle, SDL_WINDOW_FULLSCREEN);

    if (r != 0) DEBUG_MESSAGE(std::string("Could not set window to fullscreen! SDL Error: ") + SDL_GetError(), MESSAGE_TYPE::M_WARNING);
  } else {
    int r = SDL_SetWindowFullscreen(windowHandle, 0);
    if (r != 0) DEBUG_MESSAGE(std::string("Could not unset window fullscreen! SDL Error: ") + SDL_GetError(), MESSAGE_TYPE::M_WARNING);
  }
}

int window_set_cursor(int cursorID) {
  if (cursorID == cr_none)
    SDL_ShowCursor(SDL_DISABLE);
  else
    SDL_ShowCursor(SDL_ENABLE);

  if (cursorID <= 0 && cursorID >= cr_size_all && enigma::cursors[-cursorID] != nullptr) {
    SDL_SetCursor(enigma::cursors[-cursorID]);
    return 1;
  }

#ifdef DEBUG_MODE
  DEBUG_MESSAGE("Cursor lookup failure", MESSAGE_TYPE::M_ERROR);
#endif

  return 0;
}

int display_get_x() {
  SDL_Rect r;
  int d = SDL_GetWindowDisplayIndex(windowHandle);
  return (SDL_GetDisplayBounds(d, &r) == 0) ? r.x : 0;
}

int display_get_y() {;
  SDL_Rect r;
  int d = SDL_GetWindowDisplayIndex(windowHandle);
  return (SDL_GetDisplayBounds(d, &r) == 0) ? r.y : 0;
}

int display_get_width() {
  SDL_DisplayMode DM;
  int d = SDL_GetWindowDisplayIndex(windowHandle);
  SDL_GetCurrentDisplayMode(d, &DM);
  return DM.w;
}

int display_get_height() {
  SDL_DisplayMode DM;
  int d = SDL_GetWindowDisplayIndex(windowHandle);
  SDL_GetCurrentDisplayMode(d, &DM);
  return DM.h;
}

bool keyboard_check_direct(int key) {
  const Uint8* state = SDL_GetKeyboardState(nullptr);
  const SDL_Keycode keycode = enigma::keyboard::inverse_keymap[key];
  const SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode);
  return state[scancode];
}

void keyboard_key_press(int key) {
  SDL_Event sdlevent = { };
  sdlevent.type = SDL_KEYDOWN;
  sdlevent.key.keysym.sym = enigma::keyboard::inverse_keymap[key];

  SDL_PushEvent(&sdlevent);
}

void keyboard_key_release(int key) {
  SDL_Event sdlevent = { };
  sdlevent.type = SDL_KEYUP;
  sdlevent.key.keysym.sym = enigma::keyboard::inverse_keymap[key];

  SDL_PushEvent(&sdlevent);
}

}  // namespace enigma_user
