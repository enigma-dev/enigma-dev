#include "include.h"
#include "Platforms/General/PFmain.h"

#include <gtest/gtest.h>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

namespace enigma {

void gtest_binary(string expression, string left_value, string right_value,
                  string left_exp, string right_exp, string operator_english,
                  bool pass, bool assert, string user_message,
                  string script, int line) {
  if (pass) {
    cout << "Pass: " << expression << endl;
    return;
  }
  ADD_FAILURE_AT(script.c_str(), line)
      << "Failure: Not true that " << left_exp << " (which is " << left_value
      << ") " << operator_english << " " << right_exp << " (which is "
      << right_value << ")."
      << (user_message.empty() ? "" : "\n" + user_message);
  if (assert) exit(42);
  else game_return = 43;
}

void gtest_unary(string expression, string value, string expected_value,
                 bool pass, bool assert, string user_message,
                 string script, int line) {
  if (pass) {
    cout << "Pass: " << expression << " is " << expected_value << endl;
    return;
  }
  ADD_FAILURE_AT(script.c_str(), line)
      << "Failure: Not true that " << expression << " is " << expected_value
      << "; it is actually " << value
      << (user_message.empty() ? "." : ".\n" + user_message);
  if (assert) exit(42);
  else game_return = 43;
}

}  // namespace enigma
