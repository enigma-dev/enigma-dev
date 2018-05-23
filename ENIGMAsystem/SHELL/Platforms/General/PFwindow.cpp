#include "PFwindow.h"
#include "PFmain.h"

#include <map>

namespace enigma {

char mousestatus[3];
char last_mousestatus[3];
char last_keybdstatus[256];
char keybdstatus[256];
static std::map<int, int> keybdmap;

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

}  //namespace enigma

namespace enigma_user {

std::string keyboard_lastchar = "";
int keyboard_lastkey = 0;

double mouse_x, mouse_y;
int mouse_button, mouse_lastbutton;
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

void mouse_clear(const int button) { enigma::mousestatus[button] = enigma::last_mousestatus[button] = 0; }

void mouse_wait() {
  for (;;) {
    io_handle();
    for (int i = 0; i < 3; i++)
      if (enigma::mousestatus[i]) return;
    enigma::Sleep(10);  // Sleep 1/100 second
  }
}

}  //namespace enigma_user