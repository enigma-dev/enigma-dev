#include "PFmain.h"

#include "PFwindow.h"
#include "PFsystem.h"
#include "Platforms/platforms_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/mathnc.h" // enigma_user::clamp

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
// Monotic non-wall clock timer is required for accurate frame limiting.
// https://github.com/enigma-dev/enigma-dev/pull/2259
std::chrono::steady_clock::time_point timer_start;
std::chrono::steady_clock::time_point timer_offset;
std::chrono::steady_clock::time_point timer_offset_slowing;
std::chrono::steady_clock::time_point timer_current;
unsigned long current_time_mcs = 0;
bool game_window_focused = true;

void platform_focus_gained() {
  game_window_focused = true;
  pausedSteps = 0;
  input_initialize();
}

void platform_focus_lost() {
  game_window_focused = false;
  for (int i = 0; i < 255; i++) {
    last_keybdstatus[i] = keybdstatus[i];
    keybdstatus[i] = 0;
  }
  for (int i=0; i < 3; i++) {
    last_mousestatus[i] = mousestatus[i];
    mousestatus[i] = 0;
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

void initTimer() {
  timer_start = std::chrono::steady_clock::now();
  timer_offset = timer_start;
  timer_offset_slowing = timer_start;
  timer_current = timer_start;
}

void update_current_time() {
  timer_current = std::chrono::steady_clock::now();
}

long get_current_offset_difference_mcs() {
  auto delta = std::chrono::duration_cast<std::chrono::microseconds>(timer_current - timer_offset).count();
  return enigma_user::clamp(delta, 0, 1000000);
}

long get_current_offset_slowing_difference_mcs() {
  auto delta = std::chrono::duration_cast<std::chrono::microseconds>(timer_current - timer_offset_slowing).count();
  return enigma_user::clamp(delta, 0, 1000000);
}

void increase_offset_slowing(long increase_mcs) {
  timer_offset_slowing += std::chrono::microseconds(increase_mcs);
}

void offset_modulus_one_second() {
  long passed_mcs = get_current_offset_difference_mcs();
  // rounds towards 0
  timer_offset += std::chrono::duration_cast<std::chrono::seconds>(std::chrono::microseconds(passed_mcs));
  timer_offset_slowing = timer_offset;
}

long last_mcs = 0;
long spent_mcs = 0;
long remaining_mcs = 0;
long needed_mcs = 0;

int updateTimer() {
  // Update current time.
  update_current_time();
  {
    // Find diff between current and offset.

    long passed_mcs = enigma::get_current_offset_difference_mcs();
    if (passed_mcs >= 1000000) {  // Handle resetting.
      // If more than one second has passed, update fps variable, reset frames count,
      // and advance offset by difference in seconds, rounded down.

      enigma_user::fps = frames_count;
      frames_count = 0;
      enigma::offset_modulus_one_second();
    }
  }

  if (current_room_speed > 0) {
    spent_mcs = enigma::get_current_offset_slowing_difference_mcs();

    remaining_mcs = 1000000 - spent_mcs;
    needed_mcs = long((1.0 - 1.0 * frames_count / current_room_speed) * 1e6);
    const int catchup_limit_ms = 50;
    if (needed_mcs > remaining_mcs + catchup_limit_ms * 1000) {
      // If more than catchup_limit ms is needed than is remaining, we risk running too fast to catch up.
      // In order to avoid running too fast, we advance the offset, such that we are only at most catchup_limit ms behind.
      // Thus, if the load is consistently making the game slow, the game is still allowed to run as fast as possible
      // without any sleep.
      // And if there is very heavy load once in a while, the game will only run too fast for catchup_limit ms.
      enigma::increase_offset_slowing(needed_mcs - (remaining_mcs + catchup_limit_ms * 1000));

      spent_mcs = enigma::get_current_offset_slowing_difference_mcs();
      remaining_mcs = 1000000 - spent_mcs;
      needed_mcs = long((1.0 - 1.0 * frames_count / current_room_speed) * 1e6);
    }
    if (remaining_mcs > needed_mcs) {
      const long sleeping_time = std::min((remaining_mcs - needed_mcs) / 5, long(999999));
      std::this_thread::sleep_for(std::chrono::microseconds(std::max(long(1), sleeping_time)));
      return -1;
    }
  }

  //TODO: The placement of this code is inconsistent with XLIB because events are handled before, ask Josh.
  unsigned long dt = 0;
  if (spent_mcs > last_mcs) {
    dt = (spent_mcs - last_mcs);
  } else {
    //TODO: figure out what to do here this happens when the fps is reached and the timers start over
    dt = enigma_user::delta_time;
  }
  last_mcs = spent_mcs;
  enigma_user::delta_time = dt;
  current_time_mcs += enigma_user::delta_time;
  enigma_user::current_time += enigma_user::delta_time / 1000;

  return 0;
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
std::string game_save_id = "";
std::string keyboard_string = "";
double fps = 0;
unsigned long delta_time = 0;
unsigned long current_time = 0;

bool os_is_paused() { return !enigma::game_window_focused && enigma::freezeOnLoseFocus; }

std::string parameter_string(int num) { return num < enigma::parameterc ? enigma::parameters[num] : ""; }

int parameter_count() { return enigma::parameterc; }

void sleep(int ms) { enigma::Sleep(ms); }

unsigned long get_timer() {
  enigma::update_current_time();
  return std::chrono::duration_cast<std::chrono::microseconds>(enigma::timer_current - enigma::timer_start).count();
}

void game_end(int ret) {
  enigma::game_isending = true;
  enigma::game_return = ret;
}

void game_end() {
  enigma::game_isending = true;
}

void action_end_game() { return game_end(); }

}  //namespace enigma_user
