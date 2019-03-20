#include "PFmain.h"

#include "PFwindow.h"
#include "PFsystem.h"
#include "Platforms/platforms_mandatory.h"
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

long clamp(long value, long min, long max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
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
    printf("Failed to create game window\n");
    return -4;
  }

  initTimer();
  initInput();

  EnableDrawing(nullptr);

  // Call ENIGMA system initializers; sprites, audio, and what have you
  initialize_everything();

  while (!game_isending) {
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
