#include "PFmain.h"

#include "PFwindow.h"
#include "PFsystem.h"
#include "Platforms/platforms_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/lives.h"

#include <chrono> // std::chrono::microseconds
#include <thread> // sleep_for
#include <sstream> // ostringstream

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

void game_format_caption() {
  std::ostringstream out;
  // round ALL the numbers, like 5.5 to 6 lives
  // e.g, GM8.1 behavior
  out.precision(0);

  // DON'T append anything that isn't set so the main loop
  // could optimize the caption out completely
  if (!((std::string)enigma_user::room_caption).empty())
    out << (std::string)enigma_user::room_caption << " ";
  if (enigma_user::show_score && enigma_user::score != 0)
    out << enigma_user::caption_score << std::fixed << enigma_user::score << " ";
  if (enigma_user::show_lives)
    out << enigma_user::caption_lives << std::fixed << (double)enigma_user::lives << " ";
  if (enigma_user::show_health)
    out << enigma_user::caption_health << std::fixed << enigma_user::health;

  std::string caption = out.str();
  if (!caption.empty()) enigma_user::window_set_caption(caption);
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

  while (!game_isending) {
    game_format_caption();
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

std::string caption_score = "Score: ", caption_lives = "Lives: ", caption_health = "Health: ";
bool show_score = 0, show_lives = 0, show_health = 0;
double score = 0;
double health = 100;

bool os_is_paused() { return !enigma::game_window_focused && enigma::freezeOnLoseFocus; }

std::string parameter_string(int num) { return num < enigma::parameterc ? enigma::parameters[num] : ""; }

int parameter_count() { return enigma::parameterc; }

void sleep(int ms) { enigma::Sleep(ms); }

void game_end(int ret) {
  enigma::game_isending = true;
  enigma::game_return = ret;
}

void game_end() {
  enigma::game_isending = true;
}

void action_end_game() { return game_end(); }

}  //namespace enigma_user
