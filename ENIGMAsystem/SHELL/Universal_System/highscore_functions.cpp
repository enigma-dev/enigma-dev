/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2011 Alasdair Morrison <tgmg@g-java.com>                      **
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


#include "highscore_functions.h"
#include "libEGMstd.h"
#include <algorithm>
#include <vector>
#include <string>

#include "Widget_Systems/widgets_mandatory.h" // show_error


extern double score;

namespace enigma {

    int highscore_background;
    bool highscore_border;
    int highscore_back_color;
    int highscore_new_entry_color;
    int highscore_other_entries_color;
    std::string highscore_caption;
    std::string highscore_nobody;
    std::string highscore_escape;
    std::vector< playerScore > highscore_list;

static bool cmp_highscore( const playerScore& a, const playerScore& b ) {
    return a > b;
}

    playerScore::playerScore(std::string name, int score) {
        player_name=name;
        player_score = score;

    }

    playerScore::playerScore() {

    }

    void highscore_init() {
        highscore_background=-1;
        highscore_border=true;
        /*highscore_back_color=c_black;
        highscore_new_entry_color=c_red;
        highscore_other_entries_color-c_yellow;*/
        highscore_caption="Top Ten Players";
        highscore_nobody="<nobody>";
        highscore_escape="press <Escape> to close";
        highscore_clear();

    }

}

extern int room_width;
extern int room_height;
void screen_refresh();

void io_handle();
bool keyboard_check(int i);
void screen_redraw();
void keyboard_wait();

void highscore_show(int numb) {
    highscore_add("player name",numb);
     while(!keyboard_check(27 /*vk_escape*/)) {
    draw_highscore(20, 0, room_width-20, room_height);

         screen_refresh();
         io_handle();
     }
}

void action_highscore_show(int background,int border,double newColor,double otherColor,std::string font,int a,int b,int c,int d,int e,int f) {
    highscore_show(score);
}

void highscore_set_background(int back) {enigma::highscore_background=back;}

void highscore_set_border(bool show) {enigma::highscore_border=show;}

void highscore_set_font(std::string name, int size, int style) {
    //not cross platform compatible?
    show_error("Warning function: highscore_set_font doesn't work, please use draw_set_font", false);
}

void highscore_set_colors(int back, int newcol, int othercol) {
    enigma::highscore_back_color=back;
    enigma::highscore_new_entry_color=newcol;
    enigma::highscore_other_entries_color=othercol;
}

void highscore_set_strings(std::string caption, std::string nobody, std::string escape) {
    enigma::highscore_caption=caption;
    enigma::highscore_nobody=nobody;
    enigma::highscore_escape=escape;
}

void highscore_show_ext(int numb, int back, int show, int newcol, int othercol, std::string name, int size) {
    enigma::highscore_background=back;
    enigma::highscore_border=show;
    enigma::highscore_new_entry_color=newcol;
    enigma::highscore_other_entries_color=othercol;

    highscore_show(numb);
}

void highscore_clear() {
    enigma::highscore_list.clear();
    for (int i=0; i < 10; i++) {
        enigma::highscore_list.push_back(enigma::playerScore(enigma::highscore_nobody,0));
    }
}

void highscore_add(std::string str, int numb) {
    enigma::highscore_list.push_back(enigma::playerScore(str,numb));
    std::sort( enigma::highscore_list.begin(), enigma::highscore_list.end(), enigma::cmp_highscore );
    if (enigma::highscore_list.size()>10)
    enigma::highscore_list.resize(10);
}

void highscore_add_current() {
    highscore_add("Ask for Name!", score);
}

int highscore_value(int place) {
    try {
    return enigma::highscore_list.at(place).player_score;
    } catch (...) {
        return 0;
    }
}

std::string highscore_name(int place) {
    return enigma::highscore_list[place].player_name;
}

#include "var4.h"
void draw_text(int x,int y,variant str);
int string_width(variant str);


void draw_highscore(int x1, int y1, int x2, int y2) {

    for (size_t i=0; i<enigma::highscore_list.size(); i++) {
        draw_text(x1, (i*((y2-y1)/10))+y1+10, enigma::highscore_list[i].player_name.c_str());
        draw_text(x2-string_width(toString((var)enigma::highscore_list[i].player_score)), (i*((y2-y1)/10))+y1+10 , toString((var)enigma::highscore_list[i].player_score));
    }
}





