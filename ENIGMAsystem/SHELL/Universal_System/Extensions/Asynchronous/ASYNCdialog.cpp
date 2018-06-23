/** Copyright (C) 2014 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY {
} without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <sstream> // std::stringstream, std::stringbuf

#include "ASYNCdialog.h"
#include "Platforms/General/PFthreads.h"
#include "Widget_Systems/General/WSdialogs.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/Extensions/DataStructures/include.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/instance.h"

// include after variant
#include "implement.h"

namespace enigma {
  namespace extension_cast {
    extension_async *as_extension_async(object_basic*);
  }
}

using namespace enigma_user;

using enigma::threads;
using enigma::MessageData;
using enigma::createThread;

static void fireAsyncDialogEvent() {
  enigma::instance_event_iterator = new enigma::inst_iter(NULL,NULL,NULL);
  for (enigma::iterator it = enigma::instance_list_first(); it; ++it)
  {
    enigma::object_basic* const inst = ((enigma::object_basic*)*it);
    enigma::extension_async* const inst_async = enigma::extension_cast::as_extension_async(inst);
    inst_async->myevent_asyncdialog();
  }
}

static void* showMessageAsync(void* data) {
  const MessageData* const md = (MessageData*)data;
  threads[md->id]->ret = show_message(md->text1);
  threads[md->id]->active = false;
  ds_map_replaceanyway(async_load, "id", md->id);
  ds_map_replaceanyway(async_load, "status", true); //TODO: Stupido is so god damn retarded, it gives a cancel operation for a rhetorical message according to the manual
  fireAsyncDialogEvent();
  return NULL;
}

static void* showQuestionAsync(void* data) {
  const MessageData* const md = (MessageData*)data;
  threads[md->id]->ret = show_question(md->text1);
  threads[md->id]->active = false;
  ds_map_replaceanyway(async_load, "id", md->id);
  ds_map_replaceanyway(async_load, "status", threads[md->id]->ret);
  fireAsyncDialogEvent();
  return NULL;
}

static void* getStringAsync(void* data) {
  const MessageData* const md = (MessageData*)data;
  threads[md->id]->ret = get_string(md->text1, md->text2, md->text3);
  threads[md->id]->active = false;
  ds_map_replaceanyway(async_load, "id", md->id);
  ds_map_replaceanyway(async_load, "status", true);
  ds_map_replaceanyway(async_load, "result", threads[md->id]->ret);
  fireAsyncDialogEvent();
  return NULL;
}

static void* getIntegerAsync(void* data) {
  const MessageData* const md = (MessageData*)data;
  threads[md->id]->ret = get_integer(md->text1, md->text2, md->text3);
  threads[md->id]->active = false;
  ds_map_replaceanyway(async_load, "id", md->id);
  ds_map_replaceanyway(async_load, "status", true);
  ds_map_replaceanyway(async_load, "result", threads[md->id]->ret);
  fireAsyncDialogEvent();
  return NULL;
}

static void* getLoginAsync(void* data) {
  const MessageData* const md = (MessageData*)data;
  threads[md->id]->ret = get_login(md->text1, md->text2, md->text3);
  threads[md->id]->active = false;
  ds_map_replaceanyway(async_load, "id", md->id);
  string ret = threads[md->id]->ret;
  size_t end = ret.find('\0', 0);
  string username, password;
  // must still check if the string is empty which is the case when the user cancels the dialog
  if (end != string::npos) {
    username = ret.substr(0, end);
    password = ret.substr(end + 1, ret.size() - end);
  }
  ds_map_replaceanyway(async_load, "username", username);
  ds_map_replaceanyway(async_load, "password", password);
  fireAsyncDialogEvent();
  return NULL;
}

namespace enigma_user {
  unsigned async_load = 0;

  int show_message_async(string str) {
    MessageData* md = new MessageData(str, "", "");
    return createThread(showMessageAsync, md);
  }

  int show_question_async(string str) {
    MessageData* md = new MessageData(str, "", "");
    return createThread(showQuestionAsync, md);
  }

  int get_string_async(string message, string def, string cap) {
    MessageData* md = new MessageData(message, def, cap);
    return createThread(getStringAsync, md);
  }

  int get_integer_async(string message, string def, string cap) {
    MessageData* md = new MessageData(message, def, cap);
    return createThread(getIntegerAsync, md);
  }

  int get_login_async(string username, string password, string cap) {
    MessageData* md = new MessageData(username, password, cap);
    return createThread(getLoginAsync, md);
  }
}
