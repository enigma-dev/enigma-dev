/*********************************************************************************\
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
\*********************************************************************************/

/*************************************************************
GM Global variables
******************/
/*
global:     argument_relative
global:     background_alpha
global:     background_blend*/
double      background_color=16777215;/*
global:     background_foreground
global:     background_height
global:     background_hspeed
global:     background_htiled
global:     background_index*/
double      background_showcolor=1;/*
global:     background_visible
global:     background_vspeed
global:     background_vtiled
global:     background_width
global:     background_x
global:     background_xscale
global:     background_y
global:     background_yscale
global:     caption_health
global:     caption_lives
global:     caption_score
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
double      fps=0;/*
global:     game_id
global:     health*/
double      instance_count=0;/*
global:     instance_id
global:     keyboard_key
global:     keyboard_lastchar
global:     keyboard_lastkey
global:     keyboard_string
global:     lives*/
double      mouse_button;
double      mouse_lastbutton;
double      mouse_x;
double      mouse_y;
enigma::roomv room;
std::string room_caption="ENIGMA--The sand is the limit.";
double      room_first=0;
double      room_height=480;
double      room_last=0;
double      room_persistent=0;
double      room_speed=60;
double      room_width=640;
/*global:   score
global:     secure_mode
global:     show_health
global:     show_lives
global:     show_score
global:     temp_directory
global:     transition_kind
global:     transition_steps
global:     transition_time*/
double      view_angle=0;
double      view_current;
double      view_enabled=0;
double      view_hborder[9]={0,0,0,0,0,0,0,0};
double      view_hport[9]={0,0,0,0,0,0,0,0};
double      view_hspeed[9]={0,0,0,0,0,0,0,0};
double      view_hview[9]={0,0,0,0,0,0,0,0};
double      view_object[9]={0,0,0,0,0,0,0,0};
double      view_vborder[9]={0,0,0,0,0,0,0,0};
double      view_visible[9]={0,0,0,0,0,0,0,0};
double      view_vspeed[9]={0,0,0,0,0,0,0,0};
double      view_wport[9]={0,0,0,0,0,0,0,0};
double      view_wview[9]={0,0,0,0,0,0,0,0};
double      view_xport[9]={0,0,0,0,0,0,0,0};
double      view_xview[9]={0,0,0,0,0,0,0,0};
double      view_yport[9]={0,0,0,0,0,0,0,0};
double      view_yview[9]={0,0,0,0,0,0,0,0};
/*global:  working_directory*/
/*********************
End GM global variables
 *********************/
