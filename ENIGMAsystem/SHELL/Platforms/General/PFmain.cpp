#include "PFmain.h"

#include "PFwindow.h"
#include "PFsystem.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Platforms/platforms_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/roomsystem.h"

#include <chrono> // std::chrono::microseconds
#include <thread> // sleep_for

namespace enigma {

std::vector<std::function<void()> > extension_update_hooks;

bool game_isending = false;
int game_return = 0;
int pausedSteps = 0;
int current_room_speed;
std::string* parameters;
int parameterc;
int frames_count = 0;
unsigned long current_time_mcs = 0;
bool game_window_focused = true;

void initGame(unsigned started) {
  // game window will become visible at step zero when splash screen is turned
  // on, and there will be a briefly running call to enigma_user::sleep(), and
  // if splash is turned off, wait until step two to become visible w/o sleep.
  // this is the exact behavior of GameMaker Studio. since this is executed at
  // a time that is after rooms and views init, the window will be sized right
  if (started == 2) {
    // In pull request 1831, it was decided to adopt GMS behavior instead of GM8.
    // The window is no longer moved, centered, or resized when switching rooms.
    // This is always true, even if the room sizes are different.
    enigma_user::window_default(false);
    // window sized by first room, can make visible now
    enigma_user::window_set_visible(true);
    // allow time for game to open for measuring titlebar
    // and border to calculate proper window positioning.
  } else if (started == 10) {
    // required for global game setting resizeable window
    enigma_user::window_set_sizeable(isSizeable);
    // required for global game setting borderless window
    enigma_user::window_set_showborder(showBorder);
    // don't ask me why this is necessary, but it 100% is
    enigma_user::window_center();
    // required for global game setting fullscreen window
    enigma_user::window_set_fullscreen(isFullScreen);
    // required for Draw GUI event
    enigma_user::screen_init();
    enigma_user::screen_refresh();
  }
}

int gameWait() {
  if (enigma_user::os_is_paused()) {
    if (pausedSteps < 1) {
      pausedSteps += 1;
    } else {
      std::this_thread::sleep_for(std::chrono::microseconds(100000));
      return -1;
    }
  }

  frames_count++;

  return 0;
}

void set_room_speed(int rs) { current_room_speed = rs; }

void set_program_args(int argc, char** argv) {
  parameters = new std::string[argc];
  parameterc = argc;
  for (int i = 0; i < argc; i++) parameters[i] = argv[i];
}

int enigma_main(int argc, char** argv) {
  // Initialize directory globals
  initialize_directory_globals();

  // Copy our parameters
  set_program_args(argc, argv);

  if (!initGameWindow()) {
    DEBUG_MESSAGE("Failed to create game window", MESSAGE_TYPE::M_FATAL_ERROR);
    return -4;
  }

  initTimer();
  initInput();

  EnableDrawing(nullptr);

  // Call ENIGMA system initializers; sprites, audio, and what have you
  initialize_everything();

  unsigned started = 0;
  while (!game_isending) {
    initGame(started); 
    if (started < 11) started++;

    if (!((std::string)enigma_user::room_caption).empty())
      enigma_user::window_set_caption(enigma_user::room_caption);
    update_mouse_variables();

    if (updateTimer() != 0) continue;
    if (handleEvents() != 0) break;
    if (gameWait() != 0) continue;

    // if any extensions need updated, update them now
    // just before we fire off user events like step
    for (auto update_hook : extension_update_hooks)
      update_hook();

    ENIGMA_events();
    handleInput();
  }

  game_ending();
  DisableDrawing(nullptr);
  destroyWindow();
  return game_return;
}

}  //namespace enigma

namespace enigma_user {

const int os_browser = browser_not_a_browser;
std::string working_directory = "";
std::string program_directory = "";
std::string temp_directory = "";
std::string game_save_id = "";
std::string keyboard_string = "";
int keyboard_key = 0;
double fps = 0;
unsigned long delta_time = 0;
unsigned long current_time = 0;

bool os_is_paused() { return !enigma::game_window_focused && enigma::freezeOnLoseFocus; }

std::string parameter_string(int num) { return num < enigma::parameterc ? enigma::parameters[num] : ""; }

int parameter_count() { return enigma::parameterc; }

void sleep(int ms) { enigma::Sleep(ms); }

void game_end(int ret) {
  enigma::game_isending = true;
  enigma::game_return = ret;
}

void action_end_game() { return game_end(); }

}  //namespace enigma_user
