/** Copyright (C) 2014, 2018 Robert B. Colton
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

#include "ASYNCdialog.h"
#include "Widget_Systems/General/WSdialogs.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/platforms_mandatory.h"
#include "Universal_System/Extensions/DataStructures/include.h"
#include "Universal_System/Instances/instance_system.h"
#include "Universal_System/Instances/instance.h"

// include after evariant
#include "implement.h"

#include <future>
#include <thread>
#include <chrono>
#include <vector>

using namespace enigma_user;

namespace enigma {
  namespace extension_cast {
    extension_async *as_extension_async(object_basic*);
  }
}

namespace {

static void fireAsyncDialogEvent() {
  enigma::instance_event_iterator = &enigma::dummy_event_iterator;
  for (enigma::iterator it = enigma::instance_list_first(); it; ++it)
  {
    enigma::object_basic* const inst = ((enigma::object_basic*)*it);
    enigma::extension_async* const inst_async = enigma::extension_cast::as_extension_async(inst);
    inst_async->myevent_asyncdialog();
  }
}

std::vector<std::function<void()> > async_jobs;

void process_async_jobs() {
  static size_t last_job = 0;
  static std::future<void> last_job_future;
  static bool last_job_started = false;
  // if we have completed all jobs we can just return
  if (last_job == async_jobs.size()) return;

  // if the current job hasn't been launched yet, do so now
  if (!last_job_started) {
    auto job = async_jobs[last_job];
    last_job_started = true;
    // create a task which allows us to get a future
    std::packaged_task<void()> task(job);
    // get the future to tell us when the job "finished"
    // aka the user responded to the dialog
    last_job_future = task.get_future();
    // run task on new thread
    std::thread(std::move(task)).detach();
  }

  // if the previously launched job has finished, complete the async_load map
  // and fire the async dialog event from the main thread
  if (last_job_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
    // increment job counter so we can move to the next job
    ds_map_overwrite(async_load, "id", last_job++);
    last_job_started = false;
    // only one async dialog event should be fired at a time
    // and it should be fired from the main thread
    fireAsyncDialogEvent();
  }
}

int queue_async_job(std::function<void()> job) {
  if (!ds_map_exists(async_load)) async_load = ds_map_create();
  int id = async_jobs.size();
  async_jobs.push_back(job);
  process_async_jobs();
  return id;
}

}

namespace enigma {

void extension_async_init() {
  extension_update_hooks.push_back(process_async_jobs);
}

} // namespace enigma

namespace enigma_user {
  unsigned async_load = -1;

  int show_message_async(string str) {
    auto fnc = [=] {
      show_message(str);
      //TODO: Stupido lolz, gives a cancel operation for a rhetorical message according to the manual
      ds_map_overwrite(async_load, "status", true);
    };
    return queue_async_job(fnc);
  }

  int show_question_async(string str) {
    auto fnc = [=] {
      bool status = show_question(str);
      ds_map_overwrite(async_load, "status", status);
    };
    return queue_async_job(fnc);
  }

  int get_string_async(string str, string def) {
    auto fnc = [=] {
      string result = get_string(str, def);
      ds_map_overwrite(async_load, "status", true);
      ds_map_overwrite(async_load, "result", result);
    };
    return queue_async_job(fnc);
  }

  int get_integer_async(string str, double def) {
    auto fnc = [=] {
      double result = get_integer(str, def);
      ds_map_overwrite(async_load, "status", true);
      ds_map_overwrite(async_load, "result", result);
    };
    return queue_async_job(fnc);
  }

  int get_login_async(string username, string password) {
    auto fnc = [=] {
      string result = get_login(username, password);
      size_t end = result.find('\0', 0);
      string username, password;
      // must still check if the string is empty which is the case when the user cancels the dialog
      if (end != string::npos) {
        username = result.substr(0, end);
        password = result.substr(end + 1, result.size() - end);
        ds_map_overwrite(async_load, "status", true);
      } else {
        ds_map_overwrite(async_load, "status", false);
      }
      ds_map_overwrite(async_load, "username", username);
      ds_map_overwrite(async_load, "password", password);
    };
    return queue_async_job(fnc);
  }
}
