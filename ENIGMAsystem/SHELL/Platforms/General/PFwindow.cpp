#include "PFwindow.h"
#include "PFmain.h"

#include "Universal_System/roomsystem.h"  // room_width

#include <map>

namespace enigma {

int window_min_width = -1;
int window_max_width = -1;
int window_min_height = -1;
int window_max_height = -1;

char mousestatus[3];
char last_mousestatus[3];
char last_keybdstatus[256];
char keybdstatus[256];
static std::map<int, int> keybdmap;

bool windowAdapt = true;
int windowWidth = 0, windowHeight = 0;
int cursorInt = 0, regionWidth = 0, regionHeight = 0, windowX = 0, windowY = 0;
double scaledWidth = 0, scaledHeight = 0;
int parWidth = 0, parHeight = 0;

void input_initialize() {
  //Clear the input arrays
  for (int i = 0; i < 3; i++) {
    last_mousestatus[i] = 0;
    mousestatus[i] = 0;
  }
  for (int i = 0; i < 256; i++) {
    last_keybdstatus[i] = 0;
    keybdstatus[i] = 0;
  }
}

void input_push() {
  for (int i = 0; i < 3; i++) {
    last_mousestatus[i] = mousestatus[i];
  }
  for (int i = 0; i < 256; i++) {
    last_keybdstatus[i] = keybdstatus[i];
  }
  enigma_user::mouse_hscrolls = enigma_user::mouse_vscrolls = 0;
}

void compute_window_scaling() {
  if (!regionWidth) return;
  int isFullScreen = enigma_user::window_get_fullscreen();
  parWidth = isFullScreen ? enigma_user::display_get_width() : windowWidth;
  parHeight = isFullScreen ? enigma_user::display_get_height() : windowHeight;
  if (viewScale > 0) {  //Fixed Scale
    double viewDouble = viewScale / 100.0;
    scaledWidth = regionWidth * viewDouble;
    scaledHeight = regionHeight * viewDouble;
  } else if (viewScale == 0) {  //Full Scale
    scaledWidth = parWidth;
    scaledHeight = parHeight;
  } else {  //Keep Aspect Ratio
    double fitWidth = parWidth / double(regionWidth), fitHeight = parHeight / double(regionHeight);
    if (fitWidth < fitHeight) {
      scaledWidth = parWidth;
      scaledHeight = regionHeight * fitWidth;
    } else {
      scaledWidth = regionWidth * fitHeight;
      scaledHeight = parHeight;
    }
  }
}

void compute_window_size() {
  if (!regionWidth) return;
  int isFullScreen = enigma_user::window_get_fullscreen();
  if (!isFullScreen) {
    if (windowAdapt && viewScale > 0) {  // If the window is to be adapted and Fixed Scale
      if (scaledWidth > windowWidth) windowWidth = scaledWidth;
      if (scaledHeight > windowHeight) windowHeight = scaledHeight;
    }
    enigma_user::window_set_rectangle(windowX, windowY, windowWidth, windowHeight);
  } else {
    enigma_user::window_set_rectangle(0, 0, parWidth, parHeight);
  }
}

}  //namespace enigma

namespace enigma_user {

std::string keyboard_lastchar = "";
int keyboard_lastkey = 0;

double mouse_x, mouse_y;
int mouse_button, mouse_lastbutton;
int display_aa = 0;
short mouse_hscrolls = 0;
short mouse_vscrolls = 0;

void io_clear() {
  for (int i = 0; i < 255; i++) enigma::keybdstatus[i] = enigma::last_keybdstatus[i] = 0;
  for (int i = 0; i < 3; i++) enigma::mousestatus[i] = enigma::last_mousestatus[i] = 0;
}

bool keyboard_check(int key) {
  if (key == vk_anykey) {
    for (int i = 0; i < 255; i++)
      if (enigma::keybdstatus[i] == 1) return 1;
    return 0;
  }
  if (key == vk_nokey) {
    for (int i = 0; i < 255; i++)
      if (enigma::keybdstatus[i] == 1) return 0;
    return 1;
  }
  return enigma::keybdstatus[key & 0xFF];
}

bool keyboard_check_pressed(int key) {
  if (key == vk_anykey) {
    for (int i = 0; i < 255; i++)
      if (enigma::keybdstatus[i] && !enigma::last_keybdstatus[i]) return 1;
    return 0;
  }
  if (key == vk_nokey) {
    for (int i = 0; i < 255; i++)
      if (enigma::keybdstatus[i] && !enigma::last_keybdstatus[i]) return 0;
    return 1;
  }
  return enigma::keybdstatus[key & 0xFF] && !enigma::last_keybdstatus[key & 0xFF];
}

bool keyboard_check_released(int key) {
  if (key == vk_anykey) {
    for (int i = 0; i < 255; i++)
      if (!enigma::keybdstatus[i] && enigma::last_keybdstatus[i]) return 1;
    return 0;
  }
  if (key == vk_nokey) {
    for (int i = 0; i < 255; i++)
      if (!enigma::keybdstatus[i] && enigma::last_keybdstatus[i]) return 0;
    return 1;
  }
  return enigma::keybdstatus[key & 0xFF] == 0 && enigma::last_keybdstatus[key & 0xFF] == 1;
}

void keyboard_wait() {
  io_clear();
  for (;;) {
    io_handle();
    for (int i = 0; i < 255; i++)
      if (enigma::keybdstatus[i]) {
        io_clear();
        return;
      }
    enigma::Sleep(10);  // Sleep 1/100 second
  }
}

void keyboard_clear(const int key) { enigma::keybdstatus[key] = enigma::last_keybdstatus[key] = 0; }

void keyboard_set_map(int key1, int key2) {
  std::map<int, int>::iterator it = enigma::keybdmap.find(key1);
  if (enigma::keybdmap.end() != it) {
    it->second = key2;
  } else {
    enigma::keybdmap.insert(std::map<int, int>::value_type(key1, key2));
  }
}

int keyboard_get_map(int key) {
  std::map<int, int>::iterator it = enigma::keybdmap.find(key);
  if (enigma::keybdmap.end() != it) {
    return it->second;
  } else {
    return key;
  }
}

void keyboard_unset_map() { enigma::keybdmap.clear(); }

bool mouse_wheel_up() { return (mouse_vscrolls > 0); }

bool mouse_wheel_down() { return (mouse_vscrolls < 0); }

bool mouse_check_button(int button) {
  switch (button) {
    case mb_any:
      return enigma::mousestatus[0] | enigma::mousestatus[1] | enigma::mousestatus[2];
    case mb_none:
      return !(enigma::mousestatus[0] | enigma::mousestatus[1] | enigma::mousestatus[2]);
    case 1:
    case 2:
    case 3:
      return enigma::mousestatus[button - 1];
    default:
      return 0;
  }
}

bool mouse_check_button_pressed(int button) {
  switch (button) {
    case mb_any:
      return (enigma::mousestatus[0] &&
              !enigma::last_mousestatus[0])  // The trend is "pressed this step, but not last step"
             || (enigma::mousestatus[1] && !enigma::last_mousestatus[1]) ||
             (enigma::mousestatus[2] && !enigma::last_mousestatus[2]);
    case mb_none:
      return !((enigma::mousestatus[0] &&
                !enigma::last_mousestatus[0])  // The trend is "pressed this step, but not last step"
               || (enigma::mousestatus[1] && !enigma::last_mousestatus[1]) ||
               (enigma::mousestatus[2] && !enigma::last_mousestatus[2]));
    case 1:
    case 2:
    case 3:
      return enigma::mousestatus[button - 1] && !enigma::last_mousestatus[button - 1];
    default:
      return 0;
  }
}

bool mouse_check_button_released(int button) {
  switch (button) {
    case mb_any:
      return (!enigma::mousestatus[0] && enigma::last_mousestatus[0]) ||
             (!enigma::mousestatus[1] && enigma::last_mousestatus[1]) ||
             (!enigma::mousestatus[2] && enigma::last_mousestatus[2]);
    case mb_none:
      return !((!enigma::mousestatus[0] && enigma::last_mousestatus[0]) ||
               (!enigma::mousestatus[1] && enigma::last_mousestatus[1]) ||
               (!enigma::mousestatus[2] && enigma::last_mousestatus[2]));
    case 1:
    case 2:
    case 3:
      return !enigma::mousestatus[button - 1] && enigma::last_mousestatus[button - 1];
    default:
      return 0;
  }
}

void mouse_clear(const int button) { enigma::mousestatus[button - 1] = enigma::last_mousestatus[button - 1] = 0; }

void mouse_wait() {
  for (;;) {
    io_handle();
    for (int i = 0; i < 3; i++)
      if (enigma::mousestatus[i]) return;
    enigma::Sleep(10);  // Sleep 1/100 second
  }
}

void window_set_color(int color) { enigma::windowColor = color; }

int window_get_color() { return enigma::windowColor; }

void window_default(bool center_size) {
  int xm = room_width, ym = room_height;
  if (view_enabled) {
    int tx = 0, ty = 0;
    for (int i = 0; i < 8; i++)
      if (view_visible[i]) {
        if (view_xport[i] + view_wport[i] > tx) tx = (int)(view_xport[i] + view_wport[i]);
        if (view_yport[i] + view_hport[i] > ty) ty = (int)(view_yport[i] + view_hport[i]);
      }
    if (tx and ty) xm = tx, ym = ty;
  } else {
    int screen_width = display_get_width();
    int screen_height = display_get_height();
    // By default if the room is too big instead of creating a gigantic ass window
    // make it not bigger than the screen to full screen it, this is what 8.1 and Studio
    // do, if the user wants to manually override this they can using
    // views/screen_set_viewport or window_set_size/window_set_region_size
    // We won't limit those functions like GM, just the default.
    if (xm > screen_width) xm = screen_width;
    if (ym > screen_height) ym = screen_height;
  }
  bool center = true;
  if (center_size) {
    center = (xm != enigma::windowWidth || ym != enigma::windowHeight);
  }

  enigma::windowWidth = enigma::regionWidth = xm;
  enigma::windowHeight = enigma::regionHeight = ym;

  enigma::compute_window_size();
  if (center) window_center();
}

void window_center() {
  window_set_position(display_get_width() / 2 - window_get_width() / 2,
                      display_get_height() / 2 - window_get_height() / 2);
}

void window_set_freezeonlosefocus(bool freeze) { enigma::freezeOnLoseFocus = freeze; }

bool window_get_freezeonlosefocus() { return enigma::freezeOnLoseFocus; }

int window_get_region_width() { return enigma::regionWidth; }

int window_get_region_height() { return enigma::regionHeight; }

int window_get_region_width_scaled() { return enigma::scaledWidth; }

int window_get_region_height_scaled() { return enigma::scaledHeight; }

void window_set_region_scale(double scale, bool adaptwindow) {
  enigma::viewScale = int(scale * 100);
  enigma::windowAdapt = adaptwindow;
  enigma::compute_window_size();
}

double window_get_region_scale() { return enigma::viewScale / 100.0; }

void window_set_region_size(int w, int h, bool adaptwindow) {
  if (w <= 0 || h <= 0) return;

  enigma::regionWidth = w;
  enigma::regionHeight = h;
  enigma::windowAdapt = adaptwindow;
  enigma::compute_window_size();
}

}  //namespace enigma_user
