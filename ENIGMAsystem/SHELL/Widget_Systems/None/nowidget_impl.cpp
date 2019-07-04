/** Copyright (C) 2008, 2017 Josh Ventura
*** Copyright (C) 2014, 2017 Robert B. Colton
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

#include <string>
using std::string;
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/Instances/instance_system.h"
#include <cstdlib>
#include <cstdio>

#include "GameSettings.h"

#include <iostream>
#include <iomanip>
using namespace std;

#ifdef _WIN32
#  include <windows.h>
#  define TC_WINDOWS 1
#else
#  include <termios.h>
#  include <unistd.h>
#endif

class PasswordContext {
# ifdef TC_WINDOWS
  DWORD old_attrs = 0;
  HANDLE hStdin = 0;
# else
  termios old_attrs;
# endif

 public:
  PasswordContext() {
#   ifdef TC_WINDOWS
      hStdin = GetStdHandle(STD_INPUT_HANDLE);
      GetConsoleMode(hStdin, &old_attrs);
      SetConsoleMode(hStdin, old_attrs & (~ENABLE_ECHO_INPUT));
#   else
      tcgetattr(STDIN_FILENO, &old_attrs);
      termios new_attrs = old_attrs;
      new_attrs.c_lflag &= ~ECHO;
      tcsetattr(STDIN_FILENO, TCSANOW, &new_attrs);
#   endif
  }

  ~PasswordContext() {
#   ifdef TC_WINDOWS
      SetConsoleMode(hStdin, old_attrs);
#   else
      tcsetattr(STDIN_FILENO, TCSANOW, &old_attrs);
#   endif
  }
};

namespace enigma {

  bool widget_system_initialize() {
    return 0;
  }
}

namespace enigma_user {

void show_debug_message(string err, MESSAGE_TYPE type) {
if (type == MESSAGE_TYPE::M_USER_ERROR || type == MESSAGE_TYPE::M_FATAL_USER_ERROR) {
    printf((enigma::error_type(type) + ": in some action of some event for object %d, instance id %d: %s\n").c_str(),
           (enigma::instance_event_iterator == NULL? enigma_user::global :
              enigma::instance_event_iterator->inst == NULL? enigma_user::noone :
                enigma::instance_event_iterator->inst->object_index),
           (enigma::instance_event_iterator == NULL? enigma_user::global :
              enigma::instance_event_iterator->inst == NULL? enigma_user::noone :
                enigma::instance_event_iterator->inst->id),
           err.c_str()
      );
  } else printf((enigma::error_type(type) + ": %s\n").c_str(), err.c_str()); 
  
  if (type == MESSAGE_TYPE::M_FATAL_ERROR || type == MESSAGE_TYPE::M_FATAL_USER_ERROR) exit(0);
  ABORT_ON_ALL_ERRORS();
}

int show_message(const string &message)
{
  printf("show_message: %s\n", message.c_str());
  return 0;
}

void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) {
  printf("%s\n%s\n", caption.c_str(), info.c_str());
}

void show_info() {
  show_info(enigma::gameInfoText, enigma::gameInfoBackgroundColor, enigma::gameInfoLeft, enigma::gameInfoTop, enigma::gameInfoWidth, enigma::gameInfoHeight, enigma::gameInfoEmbedGameWindow, enigma::gameInfoShowBorder,
	enigma::gameInfoAllowResize, enigma::gameInfoStayOnTop, enigma::gameInfoPauseGame, enigma::gameInfoCaption);
}

bool show_question(string str) {
  cout << str;
  char answer = 0;
  while (answer != 'N' && answer != 'Y') {
    cout << endl << "[Y/N]:";
    cin >> answer;
    answer = toupper(answer);
  }
  return (answer == 'Y');
}

string get_login(string username, string password) {
  string input;
  cout << "Username: " << flush;
  cin >> input;
  {
    PasswordContext pw;
    input += '\0';
    cout << "Password: " << flush;
    cin >> input;
    cout << endl;
  }
  return input;
}

string get_string(string str, string def) {
  printf("%s\n", str.c_str());
  string input;
  cin >> input;
  return (input.empty()) ? def : input;
}

double get_integer(string str, double def) {
  printf("%s\n", str.c_str());
  string input;
  cin >> input;
  if (input.empty()) return def;
  return strtod(input.c_str(), NULL);
}

}
