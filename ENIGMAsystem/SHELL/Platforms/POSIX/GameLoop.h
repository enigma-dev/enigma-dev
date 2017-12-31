#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "Platforms/platforms_mandatory.h"

#include <string>

namespace enigma
{
  extern int windowWidth, windowHeight;
  extern bool freezeOnLoseFocus;
  extern unsigned char keymap[512];
  extern int windowColor;
  extern double fps;
  extern bool gameWindowFocused;
  extern bool freezeOnLoseFocus;
  extern bool isSizeable, showBorder, showIcons, freezeOnLoseFocus, isFullScreen;
  extern int viewScale, windowColor;
  extern std::string* parameters;
  extern int parameterc;
  extern bool game_isending;
  extern int game_return;
  extern unsigned long current_time_mcs;
  extern int cursorInt;
  extern unsigned pausedSteps;
  extern void (*WindowResizedCallback)();
  
  void EnableDrawing();
  void DisableDrawing();
  void WindowResized();
  int game_ending();
  void set_room_speed(int rs);
  int gameLoop(int argc, char** argv);
}

namespace enigma_user
{
  extern double fps;
  extern int keyboard_key;
  extern int keyboard_lastkey;
  extern std::string keyboard_lastchar;
  extern std::string keyboard_string;
}

#endif