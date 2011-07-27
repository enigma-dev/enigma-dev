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


#ifndef HIGHSCORE_FUNCTIONS_H
#define HIGHSCORE_FUNCTIONS_H 1

#include <string>


namespace enigma {

class playerScore
{
public:
    std::string player_name;
    int player_score;

    playerScore();
    playerScore(std::string name, int score);


    bool operator<(const playerScore& other) const
    {
        return player_score < other.player_score;
    }
    bool operator>(const playerScore& other) const
    {
        return player_score > other.player_score;
    }
};

}


/*
 HighScore functions
 */


//#ifdef __APPLE__
void highscore_show(int numb);

void action_highscore_show(int background,int border,double newColor,double otherColor,std::string font,int a,int b,int c,int d,int e,int f);

//#endif

void highscore_set_background(int back) ;

void highscore_set_border(bool show) ;

void highscore_set_font(std::string name, int size, int style) ;

void highscore_set_colors(int back, int newcol, int othercol) ;

void highscore_set_strings(std::string caption, std::string nobody, std::string escape) ;
void highscore_show_ext(int numb, int back, int show, int newcol, int othercol, std::string name, int size) ;
void highscore_clear() ;
void highscore_add(std::string str, int numb) ;
void highscore_add_current() ;
int highscore_value(int place);
std::string highscore_name(int place) ;
void draw_highscore(int x1, int y1, int x2, int y2) ;namespace enigma {

    void highscore_init();
}



#endif
