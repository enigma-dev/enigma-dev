/** Copyright (C) 2013 forthevin
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

#ifndef _INTEGRATION_TESTING__H
#define _INTEGRATION_TESTING__H

#include <string>

namespace enigma_user {
  enum result_t {
    te_success,
    te_failure,
    te_unknown
  };

  enum status_t {
    st_compiled_and_ran,
    st_failed_compilation,
    st_timeout,
    st_crash,
    st_error
  };

  /**
   * Must be called at the start of a test.
   *
   * @param test_name Name of the test, must be valid C++ variable name, and the same name as the game source (without extension suffix). The test name must be unique in the test's group.
   * @param description Description of the test. May not contain newlines.
   */
  void integration_testing_begin(std::string test_name, std::string description);

  /**
   * If true, records success with no message. If false, records failure with given message.
   *
   * @param subtest_name Name of the subtest, must be unique amongst the test's subtests as well as a valid C++ variable name.
   * @param succeeded True if success, false if failure.
   * @param error_message Error message shown if failure. May not contain newlines.
   */
  void integration_testing_record_conditional(std::string subtest_name, bool succeeded, std::string error_message);

  /**
   * Records a subtest result.
   *
   * @param subtest_name Name of the subtest, must be unique amongst the test's subtests as well as a valid C++ variable name.
   * @param result result_t, result of the test, must be one of the available enums.
   * @param message Error message when not successful, which can describe any issues further. May not contain newlines.
   */
  void integration_testing_record_test_result(std::string subtest_name, result_t result, std::string message);

  /**
   * Ends a test, writes its status, and ends the game. Should always be called after subtests are done.
   *
   * @param end_status status_t, final status of the test.
   */
  void integration_testing_end(status_t end_status);
}

#endif // _INTEGRATION_TESTING__H

