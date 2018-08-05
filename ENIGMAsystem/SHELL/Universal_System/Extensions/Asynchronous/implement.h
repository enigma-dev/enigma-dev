/** Copyright (C) 2014 Robert B. Colton
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

#ifndef ASYNC_EXT_SET
#define ASYNC_EXT_SET

namespace enigma {
  struct extension_async
  {
    extension_async();

    virtual variant myevent_asyncdialog() { return 0; }
    virtual variant myevent_asynchttp() { return 0; }
    virtual variant myevent_asyncimageloaded() { return 0; }
    virtual variant myevent_asyncsoundloaded() { return 0; }
    virtual variant myevent_asyncnetworking() { return 0; }
    virtual variant myevent_asynciap() { return 0; }
    virtual variant myevent_asynccloud() { return 0; }
    virtual variant myevent_asyncsteam() { return 0; }
    virtual variant myevent_asyncsocial() { return 0; }
    virtual variant myevent_asyncpushnotification() { return 0; }
  };
}

#endif // ASYNC_EXT_SET
