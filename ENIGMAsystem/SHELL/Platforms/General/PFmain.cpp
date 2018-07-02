#include "PFmain.h"

#include "Platforms/platforms_mandatory.h"

#include <unistd.h>  //getcwd, usleep

namespace enigma {

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
      usleep(100000);
      return -1;
    }
  }

  frames_count++;

  return 0;
}

void set_room_speed(int rs) { current_room_speed = rs; }

unsigned long get_timer() {  // microseconds since the start of the game
  return current_time_mcs;
}

void set_program_args(int argc, char** argv) {
  parameters = new std::string[argc];
  parameterc = argc;
  for (int i = 0; i < argc; i++) parameters[i] = argv[i];
}

int enigma_main(int argc, char** argv) {
  // Set the working_directory
  set_working_directory();

  // Copy our parameters
  set_program_args(argc, argv);

  if (!initGameWindow()) {
    printf("Failed to create game window\n");
    return -4;
  }

  initInput();

  EnableDrawing(nullptr);

  // Call ENIGMA system initializers; sprites, audio, and what have you
  initialize_everything();
  initTimer();
  showWindow();

  while (!game_isending) {
    if (updateTimer() != 0) continue;
    if (handleEvents() != 0) break;
    if (gameWait() != 0) continue;

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

std::string working_directory = "";
std::string program_directory = "";
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
