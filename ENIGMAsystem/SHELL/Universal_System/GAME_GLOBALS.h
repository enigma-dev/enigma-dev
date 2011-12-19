/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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

/*************************************************************
GM Global variables
******************/

bool argument_relative=false;
/*global:     background_alpha
global:     background_blend
global:     background_foreground
global:     background_height
global:     background_hspeed
global:     background_htiled
global:     background_index
global:     background_visible
global:     background_vspeed
global:     background_vtiled
global:     background_width
global:     background_x
global:     background_xscale
global:     background_y
global:     background_yscale
 */
string caption_score="Score:", caption_lives="Lives:", caption_health="Health:";

/*
global:     current_day
global:     current_hour
global:     current_minute
global:     current_month
global:     current_second
global:     current_time
global:     current_weekday
global:     current_year
global:     cursor_sprite
global:     error_last
global:     error_occurred
global:     event_action
global:     event_number
global:     event_object
global:     event_type*/
double      fps;/*
global:     game_id
*/
double health=100;
/*
global:     instance_id
global:     keyboard_key
global:     keyboard_lastchar
global:     keyboard_lastkey
global:     keyboard_string
*/
double lives=3;
double score=0;
/*
global:     secure_mode
 */
bool show_score=0, show_lives=0, show_health=0;
/*
global:     temp_directory
global:     transition_kind
global:     transition_steps
global:     transition_time*/
/*global:  working_directory*/
bool automatic_redraw = true;
/*********************
End GM global variables
 *********************/
