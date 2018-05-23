/** Copyright (C) 2017 Faissal I. Bensefia
*** Copyright (C) 2008-2011 IsmAvatar <cmagicj@nni.com>, Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2014 Seth N. Hetu
*** Copyright (C) 2015 Harijs Grinbergs
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <unistd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <stdlib.h> //malloc
#include <time.h> //clock
#include <map>
#include <stdlib.h> //getenv and system

#include "Platforms/platforms_mandatory.h"
#include "../General/PFsystem.h"
#include "../General/PFfilemanip.h"

#include "fillin.h"

#include "Universal_System/var4.h"
#include "Universal_System/CallbackArrays.h"
#include "Universal_System/roomsystem.h"

#include <time.h>

using uint = unsigned;

namespace enigma {
  extern bool gameWindowFocused;
  extern bool freezeOnLoseFocus;
}

namespace enigma_user {

string os_get_config() {
  return "";
}

int os_get_info() {
  return 0;
}

string os_get_language() {
  char *s = getenv("LANG");
  if (!s || !*s || !s[1]) return "";
  if (!s[2] || s[2] == '.' || s[2] == '_') {
    return string(s, 2);
  }
  return s; // It won't match people's ISO-639 checks, but it's better than "".
}

string os_get_region() {
  // Most distributions are only aware of location to the extent required to
  // give accurate time information; we can't accurately give an ISO 3166-1
  // compliant string for the device.
  return "";
}

bool os_is_network_connected() {
  return true; // Please change to false should the year drop below 2010
}

bool os_is_paused() {
  return false;
}

void os_lock_orientation(bool enable) {
}

void os_powersave_enable(bool enable) {
}

}

//////////
// INIT //
//////////

namespace enigma_user
{

void window_set_visible(bool visible)
{
}

int window_get_visible()
{
	return false;
}

void window_set_caption(string caption) {
}

string window_get_caption()
{
	return "";
}

}

inline int getMouse(int i)
{
  return -1;
}

enum {
  _NET_WM_STATE_REMOVE,
  _NET_WM_STATE_ADD,
  _NET_WM_STATE_TOGGLE
};

typedef struct
{
  unsigned long   flags;
  unsigned long   functions;
  unsigned long   decorations;
  long            inputMode;
  unsigned long   status;
} Hints;


namespace enigma_user
{

int display_mouse_get_x() { return -1; }
int display_mouse_get_y() { return -1; }
int window_mouse_get_x()  { return -1; }
int window_mouse_get_y()  { return -1; }

void window_set_stayontop(bool stay) {
}

bool window_get_stayontop() {
  return false;
}

void window_set_sizeable(bool sizeable) {
}

bool window_get_sizeable() {
  return false;
}

void window_set_showborder(bool show) {
}

bool window_get_showborder() {
  return false;
}

void window_set_showicons(bool show) {
}

bool window_get_showicons() {
  return false;
}

void window_set_minimized(bool minimized) {
}

void window_set_maximized(bool maximized) {
}

bool window_get_minimized() {
	return false;
}

bool window_get_maximized() {
  return false;
}

void window_default(bool center_size)
{
}

void window_mouse_set(int x,int y) {
}

void display_mouse_set(int x,int y) {
}

}

////////////
// WINDOW //
////////////

namespace enigma_user
{

//Getters
int window_get_x()      { return -1; }
int window_get_y()      { return -1; }
int window_get_width()  { return -1; }
int window_get_height() { return -1; }

//Setters
void window_set_position(int x,int y)
{
}

void window_set_size(unsigned int w,unsigned int h) {
}

void window_set_rectangle(int x,int y,int w,int h) {
}

//Center
void window_center()
{
}

}

////////////////
// FULLSCREEN //
////////////////

namespace enigma_user
{

void window_set_freezeonlosefocus(bool freeze)
{
}

bool window_get_freezeonlosefocus()
{
    return false;
}

void window_set_fullscreen(bool full)
{
}

bool window_get_fullscreen()
{
  return false;
}

}
                 //default    +   -5   I    \    |    /    -    ^   ...  drg  no  -    |  drg3 ...  X  ...  ?   url  +
short curs[] = { 68, 68, 68, 130, 52, 152, 135, 116, 136, 108, 114, 150, 90, 68, 108, 116, 90, 150, 0, 150, 92, 60, 52};

namespace enigma
{
  std::map<int,int> keybdmap;

  inline unsigned short highbyte_allornothing(short x) {
    return x & 0xFF00? x | 0xFF00 : x;
  }

  void initkeymap() {}
}

#include <sys/time.h>

namespace enigma_user {
  extern double fps;
}

namespace enigma {
  string* parameters;
  int parameterc;
  int current_room_speed;
  int cursorInt;
  void windowsystem_write_exename(char* x)
  {
    unsigned irx = 0;
    if (enigma::parameterc)
      for (irx = 0; enigma::parameters[0][irx] != 0; irx++)
        x[irx] = enigma::parameters[0][irx];
    x[irx] = 0;
  }
  #define hielem 9
  void set_room_speed(int rs)
  {
    current_room_speed = rs;
  }
}

#include "Universal_System/globalupdate.h"

namespace enigma_user
{

void io_handle()
{
}

int window_set_cursor(int c)
{
  enigma::cursorInt = c;
  return 0;
}

int window_get_cursor()
{
  return enigma::cursorInt;
}

void keyboard_wait()
{
}

void keyboard_set_map(int key1, int key2)
{
}

int keyboard_get_map(int key)
{
  return key;
}

void keyboard_unset_map()
{
}

void keyboard_clear(const int key)
{
}

bool keyboard_check_direct(int key)
{
  return false;
}

void window_set_region_scale(double scale, bool adaptwindow)
{
}

double window_get_region_scale()
{
    return 0;
}

void window_set_region_size(int w, int h, bool adaptwindow)
{
}

int window_get_region_width()
{
    return 0;
}

int window_get_region_height()
{
    return 0;
}

int window_get_region_width_scaled()
{
    return 0;
}

int window_get_region_height_scaled()
{
    return 0;
}

void window_set_color(int color)
{
}

int window_get_color()
{
    return -1;
}

void clipboard_set_text(string text)
{
}

string clipboard_get_text()
{
  return "";
}

bool clipboard_has_text() {
  return false;
}

}

namespace enigma_user {
  const int os_type = os_linux;
  extern int keyboard_key;
  extern int keyboard_lastkey;
  extern string keyboard_lastchar;
  extern string keyboard_string;
}

namespace enigma
{
  void ENIGMA_events(void); //TODO: Synchronize this with Windows by putting these two in a single header.

  void (*WindowResizedCallback)();
  void EnableDrawing();
  void DisableDrawing();
  void WindowResized();

}

namespace enigma
{
  extern int windowColor;

  void input_initialize()
  {
  }

  void input_push()
  {
  }
}

//TODO: Implement pause events
unsigned long current_time_mcs = 0; // microseconds since the start of the game

namespace enigma_user {
  std::string working_directory = "";
  extern double fps;
  unsigned long current_time = 0; // milliseconds since the start of the game
  unsigned long delta_time = 0; // microseconds since the last step event

  unsigned long get_timer() {  // microseconds since the start of the game
    return current_time_mcs;
  }
}

#include <unistd.h>
int main(int argc,char** argv)
{
    // Set the working_directory
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) != NULL)
       fprintf(stdout, "Current working dir: %s\n", buffer);
    else
       perror("getcwd() error");
    enigma_user::working_directory = string( buffer );

    // Copy our parameters
    enigma::parameters = new string[argc];
    enigma::parameterc = argc;
    for (int i=0; i<argc; i++)
        enigma::parameters[i]=argv[i];


    //enigma::EnableDrawing();
    //#include "initialize.h"

    //Call ENIGMA system initializers; sprites, audio, and what have you
    enigma::initialize_everything();

    struct timespec time_offset;
    struct timespec time_offset_slowing;
    struct timespec time_current;
    clock_gettime(CLOCK_MONOTONIC, &time_offset);
    time_offset_slowing.tv_sec = time_offset.tv_sec;
    time_offset_slowing.tv_nsec = time_offset.tv_nsec;
    int frames_count = 0;

    while (!enigma::game_isending)
    {
        using enigma::current_room_speed;
        clock_gettime(CLOCK_MONOTONIC, &time_current);
        {
            long passed_mcs = (time_current.tv_sec - time_offset.tv_sec)*1000000 + (time_current.tv_nsec/1000 - + time_offset.tv_nsec/1000);
            passed_mcs = enigma::clamp(passed_mcs, 0, 1000000);
            if (passed_mcs >= 1000000) { // Handle resetting.

                enigma_user::fps = frames_count;
                frames_count = 0;
                time_offset.tv_sec += passed_mcs/1000000;
                time_offset_slowing.tv_sec = time_offset.tv_sec;
                time_offset_slowing.tv_nsec = time_offset.tv_nsec;
            }
        }
        long spent_mcs = 0;
        long last_mcs = 0;
        if (current_room_speed > 0) {
            spent_mcs = (time_current.tv_sec - time_offset_slowing.tv_sec)*1000000 + (time_current.tv_nsec/1000 - time_offset_slowing.tv_nsec/1000);
            spent_mcs = enigma::clamp(spent_mcs, 0, 1000000);
            long remaining_mcs = 1000000 - spent_mcs;
            long needed_mcs = long((1.0 - 1.0*frames_count/current_room_speed)*1e6);
            const int catchup_limit_ms = 50;
            if (needed_mcs > remaining_mcs + catchup_limit_ms*1000) {
                // If more than catchup_limit ms is needed than is remaining, we risk running too fast to catch up.
                // In order to avoid running too fast, we advance the offset, such that we are only at most catchup_limit ms behind.
                // Thus, if the load is consistently making the game slow, the game is still allowed to run as fast as possible
                // without any sleep.
                // And if there is very heavy load once in a while, the game will only run too fast for catchup_limit ms.
                time_offset_slowing.tv_nsec += 1000*(needed_mcs - (remaining_mcs + catchup_limit_ms*1000));
                spent_mcs = (time_current.tv_sec - time_offset_slowing.tv_sec)*1000000 + (time_current.tv_nsec/1000 - time_offset_slowing.tv_nsec/1000);
                spent_mcs = enigma::clamp(spent_mcs, 0, 1000000);
                remaining_mcs = 1000000 - spent_mcs;
                needed_mcs = long((1.0 - 1.0*frames_count/current_room_speed)*1e6);
            }
            if (remaining_mcs > needed_mcs) {
                const long sleeping_time = std::min((remaining_mcs - needed_mcs)/5, long(999999));
                usleep(std::max(long(1), sleeping_time));
                continue;
            }
        }

        //TODO: The placement of this code is inconsistent with Win32 because events are handled after, ask Josh.
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

        enigma::ENIGMA_events();
        enigma::input_push();

        frames_count++;
    }

    enigma::game_ending();
    return enigma::game_return;
}

namespace enigma_user
{

string parameter_string(int num) {
  return num < enigma::parameterc ? enigma::parameters[num] : "";
}

int parameter_count() {
  return enigma::parameterc;
}

void execute_shell(string fname, string args)
{
  if (system(NULL)) {
    system((fname + args + " &").c_str());
  } else {
    printf("execute_shell cannot be used as there is no command processor!");
    return;
  }
}

void execute_shell(string operation, string fname, string args)
{
  if (system(NULL)) {
    system((fname + args + " &").c_str());
  } else {
    printf("execute_shell cannot be used as there is no command processor!");
    return;
  }
}

void execute_program(string operation, string fname, string args, bool wait)
{
  if (system(NULL)) {
    system((fname + args + (wait?" &":"")).c_str());
  } else {
    printf("execute_program cannot be used as there is no command processor!");
    return;
  }
}

void execute_program(string fname, string args, bool wait)
{
  if (system(NULL)) {
    system((fname + args + (wait?" &":"")).c_str());
  } else {
    printf("execute_program cannot be used as there is no command processor!");
    return;
  }
}

int display_get_width() { return 0;}
int display_get_height() { return 0; }

string environment_get_variable(string name) {
  char *ev = getenv(name.c_str());
  return ev? ev : "";
}

void set_program_priority(int value) {
  setpriority(PRIO_PROCESS, getpid(), value);
}

}

/* UNIX-ready port of file manipulation */

// File iteration functions and environment functions

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

static DIR* fff_dir_open = NULL;
static string fff_mask, fff_path;
static int fff_attrib;

#define u_root 0

namespace enigma_user
{
  string file_find_next()
  {
    if (fff_dir_open == NULL)
      return "";

    dirent *rd = readdir(fff_dir_open);
    if (rd==NULL)
      return "";
    string r = rd->d_name;

    // Preliminary filter

    const int not_attrib = ~fff_attrib;

    if (r == "." or r == ".." // Don't return ./ and
    or ((r[0] == '.' or r[r.length()-1] == '~') and not_attrib & fa_hidden) // Filter hidden files
    ) return file_find_next();

    struct stat sb;
    const string fqfn = fff_path + r;
    stat(fqfn.c_str(), &sb);

    if ((sb.st_mode & S_IFDIR and not_attrib & fa_directory) // Filter out/for directories
    or (sb.st_uid == u_root and not_attrib & fa_sysfile) // Filter system files
    or (not_attrib & fa_readonly and access(fqfn.c_str(),W_OK)) // Filter read-only files
    ) return file_find_next();

    return r;
  }
  string file_find_first(string name, int attrib)
  {
    if (fff_dir_open != NULL)
      closedir(fff_dir_open);

    fff_attrib = attrib;
    size_t lp = name.find_last_of("/");
    if (lp != string::npos)
      fff_mask = name.substr(lp+1),
      fff_path = name.substr(0,lp+1),
      fff_dir_open = opendir(fff_path.c_str());
    else
      fff_mask = name,
      fff_path = "./",
      fff_dir_open = opendir("./");
    fff_attrib = attrib;
    return file_find_next();
  }
  void file_find_close()
  {
    if (fff_dir_open != NULL)
      closedir(fff_dir_open);
    fff_dir_open = NULL;
  }
}


// TODO: Implement these...
/*
bool file_attributes(string fname,int attr);

void export_include_file(string fname);
void export_include_file_location(string fname,string location);
void discard_include_file(string fname);

extern unsigned game_id;
extern string temp_directory;



*/
