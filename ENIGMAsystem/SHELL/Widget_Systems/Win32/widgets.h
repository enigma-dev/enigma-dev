
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

int wgt_window_create(int w, int h);
void wgt_window_show(int wgt);

int wgt_layout_create(int win, string layout, int hpad, int vpad);
void wgt_layout_insert_widget(int layout, string cell, int wgt);

bool wgt_exists(int id);

int wgt_button_create(string text);
int wgt_combobox_create(string contents);
int wgt_checkbox_create(string text);
int wgt_textline_create(string text, int numchars);

int wgt_button_get_pressed(int butn);
bool wgt_checkbox_get_checked(int cbox);
string wgt_textline_get_text(int tline);
int wgt_combobox_get_selection(int cbbox);
string wgt_combobox_get_selected_text(int cbbox);
