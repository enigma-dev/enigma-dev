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

#include <sstream>      // std::stringstream, std::stringbuf

#include "ASYNCdialog.h"
#include "Platforms/General/PFthreads.h"
#include "Widget_Systems/General/WSdialogs.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/var4.h"
#include "Universal_System/Extensions/DataStructures/include.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/instance.h"

struct MessageData {
	unsigned id;
	string text1;
	string text2;
	string text3;
	MessageData(string t1, string t2, string t3): id(-1), text1(t1), text2(t2), text3(t3) { }
};

using namespace enigma_user;

static std::vector<std::string> &string_split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

static std::vector<std::string> string_split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    string_split(s, delim, elems);
    return elems;
}

static void fireAsyncDialogEvent() {
	enigma::instance_event_iterator = new enigma::inst_iter(NULL,NULL,NULL);
	for (enigma::iterator it = enigma::instance_list_first(); it; ++it)
	{
	  it->myevent_asyncdialog();
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
	if (ret.find('|', 0) != std::string::npos) {
		vector<string> split = string_split(ret,'|');
		ds_map_replaceanyway(async_load, "username", split[0]);
		ds_map_replaceanyway(async_load, "password", split[1]);
	}
	fireAsyncDialogEvent();
	return NULL;
}

static int createThread(void (*fnc)(void*), MessageData* md) {
  ethread* newthread = new ethread();
  md->id = threads.size();
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
  DWORD dwThreadId;
  newthread->handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fnc, (LPVOID)md, 0, &dwThreadId);
 
  //TODO: May need to check if ret is -1L, and yes it is quite obvious the return value is
  //an unsigned integer, but Microsoft says to for some reason. See their documentation here.
  //http://msdn.microsoft.com/en-us/library/kdzttdcb.aspx
  //NOTE: Same issue is in Platforms/General/PFthreads.cpp
  if (newthread->handle == NULL) {
#else
  if (pthread_create(&newthread->handle, NULL, fnc, md)) {
#endif
    delete md; delete newthread;
    return -1;
  }
  threads.push_back(newthread);
  return threads.size() - 1;
}

namespace enigma_user {
  //TODO: According to Studio's manual each time these async functions are called
  //they get their own map and async_load is only set that map when the async dialog
  //event is fired.
  //This is inferred from the get_login_async documentation.
  unsigned async_load = ds_map_create();

  int show_message_async(string str) {
    MessageData* md = new MessageData(str, "", "");
    return createThread((void (*)(void*))showMessageAsync, md);
  }

  int show_question_async(string str) {
    MessageData* md = new MessageData(str, "", "");
    return createThread((void (*)(void*))showQuestionAsync, md);
  }

  int get_string_async(string message, string def, string cap) {
    MessageData* md = new MessageData(message, def, cap);
    return createThread((void (*)(void*))getStringAsync, md);
  }

  int get_integer_async(string message, string def, string cap) {
    MessageData* md = new MessageData(message, def, cap);
    return createThread((void (*)(void*))getIntegerAsync, md);
  }

  int get_login_async(string username, string password, string cap) {
    MessageData* md = new MessageData(username, password, cap);
    return createThread((void (*)(void*))getLoginAsync, md);
  }
}
