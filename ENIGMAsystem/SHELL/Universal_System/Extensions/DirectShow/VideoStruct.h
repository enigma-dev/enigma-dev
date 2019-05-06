/** Copyright (C) 2013 Robert B. Colton
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

#ifndef ENIGMA_VIDEO_STRUCT
#define ENIGMA_VIDEO_STRUCT

#include <dshow.h>
#include <vector>
#include <string>
using std::vector;
using std::string;

namespace enigma {

struct VideoStruct {
    IGraphBuilder *pGraph = NULL;

	VideoStruct() {

	}

	~VideoStruct() {
		pGraph->Release();
	}
};

extern vector<VideoStruct*> videoStructs;

}

#ifdef DEBUG_MODE
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_videor(s,id,r) \
    if (unsigned(id) >= enigma::videoStructs.size() || id < 0) { \
      enigma_user::show_error("Cannot access video with id " + toString(id), false); \
      return r; \
    } enigma::VideoStruct* s = enigma::videoStructs[id];
  #define get_video(s,id) \
    if (unsigned(id) >= enigma::videoStructs.size() || id < 0) { \
      enigma_user::show_error("Cannot access video with id " + toString(id), false); \
      return; \
    } enigma::VideoStruct* s = enigma::videoStructs[id];
#else
  #define get_videor(s,id,r) \
    enigma::VideoStruct* s = enigma::videoStructs[id];
  #define get_video(s,id) \
    enigma::VideoStruct* s = enigma::videoStructs[id];
#endif

#endif
