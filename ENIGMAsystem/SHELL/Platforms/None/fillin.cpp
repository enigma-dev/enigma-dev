/** Copyright (C) 2017 Faissal I. Bensefia
*** Copyright (C) 2008-2011 IsmAvatar <cmagicj@nni.com>, Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2014 Seth N. Hetu
*** Copyright (C) 2015 Harijs Grinbergs
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

#include "Platforms/platforms_mandatory.h"
#include "Platforms/General/PFsystem.h"
#include "Platforms/General/PFfilemanip.h"
#include "Platforms/POSIX/GameLoop.h"

#include "Universal_System/var4.h"
#include "Universal_System/CallbackArrays.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/loading.h"

#include <map>
#include <unistd.h> //getcwd

namespace enigma
{
  void setwindowsize(int forceX=-1, int forceY=-1) {}
  void platformEvents() {}
}

namespace enigma_user 
{

string os_get_config() {
  return "";
}

int os_get_info() {
  return 0;
}

string os_get_language() {
  char *s = getenv("LANG");
  if (!s || !*s || !s[1]) return "";
  if (!s[2] || s[2] == '.' || s[2] == '_') {
    return string(s, 2);
  }
  return s; // It won't match people's ISO-639 checks, but it's better than "".
}

string os_get_region() {
  // Most distributions are only aware of location to the extent required to
  // give accurate time information; we can't accurately give an ISO 3166-1
  // compliant string for the device.
  return "";
}

bool os_is_network_connected() {
  return true; // Please change to false should the year drop below 2010
}

bool os_is_paused() {
  return false;
}

void os_lock_orientation(bool enable) {
}

void os_powersave_enable(bool enable) {
}

void window_set_visible(bool visible)
{
}

int window_get_visible()
{
	return false;
}

void window_set_caption(string caption) {
}

string window_get_caption()
{
	return "";
}

inline int getMouse(int i)
{
  return -1;
}


int display_mouse_get_x() { return -1; }
int display_mouse_get_y() { return -1; }
int window_mouse_get_x()  { return -1; }
int window_mouse_get_y()  { return -1; }

void window_set_stayontop(bool stay) {
}

bool window_get_stayontop() {
  return false;
}

void window_set_sizeable(bool sizeable) {
}

bool window_get_sizeable() {
  return false;
}

void window_set_showborder(bool show) {
}

bool window_get_showborder() {
  return false;
}

void window_set_showicons(bool show) {
}

bool window_get_showicons() {
  return false;
}

void window_set_minimized(bool minimized) {
}

void window_set_maximized(bool maximized) {
}

bool window_get_minimized() {
	return false;
}

bool window_get_maximized() {
  return false;
}

void window_default(bool center_size)
{
}

void window_mouse_set(int x,int y) {
}

void display_mouse_set(int x,int y) {
}

////////////
// WINDOW //
////////////

//Getters
int window_get_x()      { return -1; }
int window_get_y()      { return -1; }
int window_get_width()  { return -1; }
int window_get_height() { return -1; }

//Setters
void window_set_position(int x,int y)
{
}

void window_set_size(unsigned int w,unsigned int h) {
}

void window_set_rectangle(int x,int y,int w,int h) {
}

//Center
void window_center()
{
}

////////////////
// FULLSCREEN //
////////////////


void window_set_freezeonlosefocus(bool freeze)
{
}

bool window_get_freezeonlosefocus()
{
    return false;
}

void window_set_fullscreen(bool full)
{
}

bool window_get_fullscreen()
{
  return false;
}

void io_handle()
{
}

int window_set_cursor(int c)
{
  enigma::cursorInt = c;
  return 0;
}

int window_get_cursor()
{
  return enigma::cursorInt;
}

void keyboard_wait()
{
}

void keyboard_set_map(int key1, int key2)
{
}

int keyboard_get_map(int key)
{
  return key;
}

void keyboard_unset_map()
{
}

void keyboard_clear(const int key)
{
}

bool keyboard_check_direct(int key)
{
  return false;
}

void window_set_region_scale(double scale, bool adaptwindow)
{
}

double window_get_region_scale()
{
    return 0;
}

void window_set_region_size(int w, int h, bool adaptwindow)
{
}

int window_get_region_width()
{
    return 0;
}

int window_get_region_height()
{
    return 0;
}

int window_get_region_width_scaled()
{
    return 0;
}

int window_get_region_height_scaled()
{
    return 0;
}

void window_set_color(int color)
{
}

int window_get_color()
{
    return -1;
}

void clipboard_set_text(string text)
{
}

string clipboard_get_text()
{
  return "";
}

bool clipboard_has_text() {
  return false;
}

} //namespace enigma_user

int main(int argc,char** argv)
{
    // Set the working_directory
    char buffer[1024];
    if (getcwd(buffer, sizeof(buffer)) != NULL)
       fprintf(stdout, "Current working dir: %s\n", buffer);
    else
       perror("getcwd() error");
    enigma_user::working_directory = string( buffer );

  return enigma::gameLoop(argc, argv);

}
