
#include "GameLoop.h"

#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFsystem.h"
#include "Platforms/platforms_mandatory.h"

#include <string>
#include <unistd.h> //fork()
#include <time.h> //clock
#include <sys/resource.h> //setpriority

namespace enigma_user
{

std::string working_directory = "";
unsigned long current_time = 0; // milliseconds since the start of the game
unsigned long delta_time = 0; // microseconds since the last step event

const int os_type = os_linux;

unsigned long get_timer() 
{  // microseconds since the start of the game
  return enigma::current_time_mcs;
}

std::string parameter_string(int num) {
  return num < enigma::parameterc ? enigma::parameters[num] : "";
}

int parameter_count() {
  return enigma::parameterc;
}

void execute_shell(std::string fname, std::string args)
{
  if (system(NULL)) {
    system((fname + args + " &").c_str());
  } else {
    printf("execute_shell cannot be used as there is no command processor!");
    return;
  }
}

void execute_shell(std::string operation, std::string fname, std::string args)
{
  if (system(NULL)) {
    system((fname + args + " &").c_str());
  } else {
    printf("execute_shell cannot be used as there is no command processor!");
    return;
  }
}

void execute_program(std::string operation, std::string fname, std::string args, bool wait)
{
  if (system(NULL)) {
    system((fname + args + (wait?" &":"")).c_str());
  } else {
    printf("execute_program cannot be used as there is no command processor!");
    return;
  }
}

void execute_program(std::string fname, std::string args, bool wait)
{
  if (system(NULL)) {
    system((fname + args + (wait?" &":"")).c_str());
  } else {
    printf("execute_program cannot be used as there is no command processor!");
    return;
  }
}

void game_end(int ret) {
  enigma::game_isending = true;
  enigma::game_return = ret;
}

void action_end_game() {
  game_end(0);
}

void url_open(std::string url,std::string target,std::string options)
{
	if (!fork()) {
		execlp("xdg-open","xdg-open",url.c_str(),NULL);
		exit(0);
	}
}

void url_open_ext(std::string url,std::string target)
{
	url_open(url,target);
}

void url_open_full(std::string url,std::string target,std::string options)
{
	url_open(url,target, options);
}

void action_webpage(const std::string &url)
{
	url_open(url);
}

std::string environment_get_variable(std::string name) {
  char *ev = getenv(name.c_str());
  return ev? ev : "";
}

void set_program_priority(int value) {
  setpriority(PRIO_PROCESS, getpid(), value);
}

} //namesapace enigma