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

#include <iostream>
#include <fstream>

#include "Platforms/platforms_mandatory.h"

#include "integration_testing.h"

namespace enigma {
  int has_been_initialized = false;
  std::string file_name;

  using namespace enigma_user;

  std::string result_string(result_t result) {

    switch (result) {
    case te_success: return "success";
    case te_failure: return "failure";
    case te_unknown:
    default : return "unknown";
    }
  }

  std::string status_string(status_t status) {

    switch (status) {
    case st_compiled_and_ran: return "compiled_and_ran";
    case st_failed_compilation: return "failed_compilation";
    case st_timeout: return "timeout";
    case st_crash: return "crash";
    case st_error:
    default : return "error";
    }
  }
}

namespace enigma_user {
  void integration_testing_begin(std::string test_name, std::string description) {
    if (enigma::has_been_initialized) return;

    enigma::has_been_initialized = true;
    enigma::file_name = test_name + ".inttest";

    std::ofstream file;
    file.open(enigma::file_name.c_str(), std::ofstream::out | std::ofstream::app);
    file << description << std::endl;
    file.close();
  }

  void integration_testing_record_conditional(std::string subtest_name, bool succeeded, std::string error_message) {
    if (succeeded) {
      integration_testing_record_test_result(subtest_name, te_success, "");
    }
    else {
      integration_testing_record_test_result(subtest_name, te_failure, "Error: " + error_message);
    }
  }

  void integration_testing_record_test_result(std::string subtest_name, result_t result, std::string message) {
    if (!enigma::has_been_initialized) return;

    std::ofstream file;
    file.open(enigma::file_name.c_str(), std::ofstream::out | std::ofstream::app);
    file << "    subtest=" << subtest_name << ":" << enigma::result_string(result) << ":" << message << std::endl;
    file.close();
  }

  void integration_testing_end(status_t end_status) {
    if (!enigma::has_been_initialized) return;

    std::ofstream file;
    file.open(enigma::file_name.c_str(), std::ofstream::out | std::ofstream::app);
    file << "    end_status=" << enigma::status_string(end_status) << std::endl;
    file.close();

    game_end();
  }
}

