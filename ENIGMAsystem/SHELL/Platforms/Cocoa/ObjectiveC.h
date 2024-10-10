/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>                      **
 **                                                                              **
 **  This file is a part of the ENIGMA Development Environment.                  **
 **                                                                              **
 **                                                                              **
 **  ENIGMA is free software: you can redistribute it and/or modify it under the **
 **  terms of the GNU General Public License as published by the Free Software   **
 **  Foundation, version 3 of the license or any later version.                  **
 **                                                                              **
 **  This application and its source code is distributed AS-IS, WITHOUT ANY      **
 **  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
 **  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
 **  details.                                                                    **
 **                                                                              **
 **  You should have recieved a copy of the GNU General Public License along     **
 **  with this code. If not, see <http://www.gnu.org/licenses/>                  **
 **                                                                              **
 **  ENIGMA is an environment designed to create games and other programs with a **
 **  high-level, fully compilable language. Developers of ENIGMA or anything     **
 **  associated with ENIGMA are in no way responsible for its users or           **
 **  applications created by its users, or damages caused by the environment     **
 **  or programs made in the environment.                                        **
 **                                                                              **
 \********************************************************************************/

#ifndef ObjectiveC_h
#define ObjectiveC_h

#ifdef __cplusplus
extern "C" {
#endif
	int mainO(int argc, char *argv[]);
	int getWindowDimension(int i);
	void cocoa_window_set_size(unsigned int w,unsigned int h);
	int getMouse(int i);
	void cocoa_window_set_caption(const char* caption);
	const char* cocoa_window_get_caption();
	void cocoa_screen_refresh();
	int cocoa_window_set_visible(int visible);
    void cocoa_flush_opengl();
    int cocoa_window_get_region_width();
    int cocoa_window_get_region_height();
    
#ifdef __cplusplus
}
#endif
#endif