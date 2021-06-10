#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFfilemanip.h"
#include "Platforms/General/PFthreads.h"
#include "Platforms/General/PFthreads_impl.h"

#include <filesystem>
#include <chrono>
#include <thread>

namespace fs = std::filesystem;
using enigma::threads;

namespace enigma {

void initialize_directory_globals() {
  enigma_user::working_directory = enigma_user::program_directory = fs::current_path().u8string();

  // Set the temp_directory
  enigma_user::temp_directory = fs::temp_directory_path().u8string();
  
  // Set the game_save_id
  enigma_user::game_save_id = enigma_user::temp_directory + "/" + std::to_string(enigma_user::game_id);
}

void windowsystem_write_exename(char* x) {
  unsigned irx = 0;
  if (enigma::parameterc)
    for (irx = 0; enigma::parameters[0][irx] != 0; irx++) x[irx] = enigma::parameters[0][irx];
  x[irx] = 0;
}

void initTimer() {
  // TODO: impl
}

int updateTimer() {
  if (current_room_speed > 0) {

  }
  //enigma_user::delta_time = dt;
  //enigma_user::current_time += enigma_user::delta_time / 1000;
  return 0;
}

void Sleep(int ms) {
  if (ms >= 1000) enigma_user::sleep(ms / 1000);
  if (ms > 0) std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

}

namespace enigma_user {
int thread_start(int thread) {
  if (threads[thread]->active) { return -1; }
  std::thread thr(enigma::thread_script_func, threads[thread]->sd);
  threads[thread]->active = true;
  return 0;
}

}

int main(int argc, char** argv) {
  return enigma::enigma_main(argc, argv);
}
