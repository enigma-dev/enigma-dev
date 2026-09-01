#include <iostream>
#include <iomanip>
#include <string>

#include <cstdlib>
#include <cstdio>

#include "Widget_Systems/widgets_mandatory.h"

namespace enigma {

  bool widget_system_initialize() {
    return true;
  }

} // namespace enigma

namespace enigma_user {

  void show_info() { }

  int show_message(std::string message) {
    printf("%s\n", message.c_str());
    return 1;
  }

  int show_question(std::string str) {
    std::cout << str;
    char answer = 0;
    while (answer != 'N' && answer != 'Y') {
      std::cout << std::endl << "[Y/N]:";
      std::cin >> answer;
      answer = std::toupper(answer);
    }
    return (answer == 'Y');
  }

  std::string get_string(std::string str, std::string def) {
    printf("%s\n", str.c_str());
    std::string input;
    std::cin >> input;
    return (input.empty()) ? def : input;
  }

  double get_integer(std::string str, double def) {
    printf("%s\n", str.c_str());
    std::string input;
    std::cin >> input;
    return (input.empty()) ? def : strtod(input.c_str(), nullptr);
  }
 
} // namespace enigma_user
