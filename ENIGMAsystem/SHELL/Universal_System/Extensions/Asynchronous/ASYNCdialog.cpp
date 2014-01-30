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

#include <process.h>

#include "ASYNCdialog.h"
#include "Platforms/General/PFthreads.h"
#include "Widget_Systems/General/WSdialogs.h"
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

static void fireAsyncDialogEvent() {
	enigma::instance_event_iterator = new enigma::inst_iter(NULL,NULL,NULL);
	for (enigma::iterator it = enigma::instance_list_first(); it; ++it)
	{
	  it->myevent_dialog();
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

static int createThread(void (*fnc)(void*), MessageData* md) {
	  ethread* newthread = new ethread();
	  md->id = threads.size();
#if CURRENT_PLATFORM_ID == OS_WINDOWS
	  uintptr_t ret = _beginthread(fnc, 0, md); //TODO: Wtf Microsoft? Why return unsigned if you need to compare it to -1L?
	  if (ret == -1L || ret == NULL) {
#else
	  if (pthread_create(&newthread->me, NULL, fnc, md)) {
#endif
		delete newthread;
		return -1;
	  }
	  threads.push_back(newthread);
	  return threads.size() - 1;
}

namespace enigma_user {
	int async_load = ds_map_create();
	
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
		//TODO: Needs a custom dialog.
		return 0;
	}
}
