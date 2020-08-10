/*

 MIT License
 
 Copyright © 2020 Samuel Venable
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
*/

#include <map>
#include <climits>

#include "insecure_bullshit.h"

#include "Platforms/General/PFfilemanip.h"

#include "videoplayer.h"

// FIXME: use Makefile
#ifndef _WIN32
#define UNIX_LIKE 1
#else
#define UNIX_LIKE 0
#endif

using std::string;

static std::map<video_t, string> vidmap;
static std::map<video_t, string> widmap;
static video_t id = 1; // starts at one because zero is reserved
// using "UINT_MAX - id" for index to avoid conflicting procinfo

namespace enigma_user {

video_t video_add(std::string fname) {
  video_t ind = UINT_MAX - id;
  fname = enigma_insecure::echo(UINT_MAX, fname); // expand environment variables
  enigma_insecure::process_clear_out(UINT_MAX); // grab output right by the pussy
  enigma_insecure::process_clear_pid(UINT_MAX); // grab procid right by the pussy
  // prevent hijacking terminal / command prompt by checking for an existent file
  if (file_exists(fname)) {
    vidmap.insert(std::make_pair(ind, fname)); 
    id++;
  }
  return ind;
}

void video_play(video_t ind, wid_t wid) {
  enigma_insecure::process_execute_async(ind, "mpv --wid=" + wid + " --no-osc \"" + vidmap.find(ind)->second + "\"");
  widmap.insert(std::make_pair(ind, wid));
}

void video_stop(video_t ind) {
  // process id of 1 is reserved on unix-likes but not on windows
  // process id of 0 is reserved on all platforms even on windows
  if (enigma_insecure::process_current(ind) <= UNIX_LIKE) {
    return;
  }
  enigma_insecure::pid_kill(process_current(ind));
}

bool video_is_playing(video_t ind) {
  // process id of 1 is reserved on unix-likes but not on windows
  // process id of 0 is reserved on all platforms even on windows
  if (enigma_insecure::process_current(ind) <= UNIX_LIKE) {
    return false;
  }
  return (enigma_insecure::pid_exists(process_current(ind)) && enigma_insecure::process_previous(ind) != enigma_insecure::process_current(ind));
}

wid_t video_get_winid(video_t ind) {
  return widmap.find(ind)->second;
}

bool video_exists(video_t ind) {
  return (!vidmap.find(ind)->second.empty());
}

void video_delete(video_t ind) {
  enigma_insecure::process_clear_out(ind);
  enigma_insecure::process_clear_pid(ind);
  vidmap.erase(ind);
  widmap.erase(ind);
}

} // namespace enigma_user
